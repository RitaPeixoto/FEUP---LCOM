#include "mouse.h"

int hookid = 2;
uint8_t content = 0;
int16_t lx=0, ly=0 ; //length of the moviment in the x and y direction, started with 0 because it's the initial position
enum state_t mov = Initial;
struct mouse_ev event;
static bool lb = false, rb = false, mb = false; //detects if it a certain button was pressed before


int (mouse_subscribe_int)(uint8_t *bit_no){ //Subscribes Mouse interrupt
    *bit_no = BIT(hookid);
    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE , &hookid) != 0){
      printf("It was detected an error in sys_irqsetpolicy!\n");
      return 1;
    }
  return 0;
}

int (mouse_unsubscribe_int)() { //Unsubscribes Mouse interrupt
  
  if(sys_irqrmpolicy(&hookid) != 0){ 
    printf("It was detected an error in sys_irqrmpolicy!\n");
    return 1;
  }
  return 0;
}

void (read_command)(uint8_t *aux){
  uint8_t str;
  for (int i = 0; i < 4; i++) {  
    if (util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in util_sys_inb!\n"); //detects if function call worked well
      return; 
    }

    if(str & OBF) { //output buffer can't be empty
      if ((str & (PAR_ERROR | TO_ERROR)) == 0 ){ // we have to detect if there is any error (parity or timeout)
        if(util_sys_inb(OUT_BUF, aux) != 0){
          printf("It was detected an error in util_sys_inb!\n");
          return;
        }
        return;
      }
    }  
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

int write_command(uint8_t command){

  uint8_t ack = 0;
 
  do{ // versao criado com o professor Nuno
    write_kbc_byte(CMD_REG, WRITE_BYTE);
    write_kbc_byte(OUT_BUF, command);
    tickdelay(micros_to_ticks(DELAY_US));
    util_sys_inb(IN_BUF, &ack);
    if(ack == ERROR){
      printf("ERROR: Second consecutive invalid byte \n");
      return -1;
    }
  } while(ack != ACK);
  
  /* versao menos compacta e eficaz
  uint8_t str, ack;
  
  do
  {
    if (util_sys_inb(STATUS_REGISTER, &str) != OK) return -1;

    if ((str & IBF)) continue; 
    if (sys_outb(CMD_REG, WRITE_BYTE) != OK)  return -1;

    if (util_sys_inb(STATUS_REGISTER, &str) != OK) return -1;

    if ((str & IBF)) continue; 
    if (sys_outb(OUT_BUF, command) != OK) return -1;

    tickdelay(micros_to_ticks(DELAY_US));
    if (util_sys_inb(IN_BUF, &ack) != OK) return -1;

    if(ack == ERROR){
      printf("ERROR: Second consecutive invalid byte \n");
      return -1;
    }

  } while (ack != ACK);*/

  return 0;
}

int (write_kbc_byte)(int port, uint8_t cmd){ //port could be argument or command
 
  uint8_t str;
 
  for (int i = 0; i< 16; i++) {
    if (util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in kbd_util_sys_inb!\n"); //detects if function call worked well
      return 1;
    }
     
    if(!(str & IBF)) {
      if(sys_outb(port, cmd) != 0){
        printf("It was detected an error in kbd_util_sys_inb!\n"); //detects if function call worked well
        return 1;
      }
      return 0;
    }
  }  
  return 1;
}

void (mouse_packet_parse)(struct packet *p, uint8_t pack[3]){

  for(int i=0; i<3; i++){
    p->bytes[i] = pack[i];
  }

  uint8_t aux = pack[0] & MOUSE_LB;
  if (aux) p->lb = true; 
  else p->lb = false; 
  aux = pack[0] & MOUSE_RB;
  if(aux) p->rb = true;
  else p->rb = false; 
  aux = pack[0] & MOUSE_MB;
  if(aux) p->mb = true;
  else p->mb = false; 
  aux = pack[0] & MOUSE_X_OV;
  if(aux)  p->x_ov = true;
  else  p->x_ov = false; 
  aux = pack[0] & MOUSE_Y_OV;
  if(aux)  p->y_ov = true;
  else  p->y_ov = false;

  //pass 2s complement to uint16_t
  if (pack[0] & MOUSE_DELTA_X){ // it means x is negative
    p->delta_x = pack[1] - 256; // pow: there are 9 bits --> so this is the 8th bit
  }
  else {
    p->delta_x = pack[1];
  }

  if (pack[0] & MOUSE_DELTA_Y){ // it means x is negative
    p->delta_y = pack[2] - 256; // pow: there are 9 bits --> so this is the 8th bit
  }
  else {
    p->delta_y = pack[2];
  }
}


struct mouse_ev* (detect_event_mouse)(struct packet *p){ //Detects mouse events relevant for the gesture to be matched. This is the lower state-machine of a 2-layered state-machine.

  event.delta_x = p->delta_x;
  event.delta_y = p->delta_y;

  if(!lb && p->lb && !p->mb && !p->rb){ //Left button pressed, when all buttons were released. 
    lb = true;
    event.type = LB_PRESSED;
  }
  else if(lb && !p->lb && !p->mb && !p->rb){ // left button released, when only left button was pressed. 
    lb = false;
    event.type = LB_RELEASED;
  }
  else if(!rb && p->rb && !p->mb && !p->lb){ //Right button pressed, when all buttons were released. 
    rb = true;
    event.type = RB_PRESSED;
  }
  else if(rb && !p->rb && !p->mb && !p->lb){ // Right button released, when only right button was pressed
    rb = false;
    event.type = RB_RELEASED;
  }
  else if(!p->rb && !p->lb && p->mb && !mb){ //Any other mouse buttons event.  
    event.type = BUTTON_EV;
    mb = true;
  }
  else if (!p->rb && !p->lb && !p->mb && mb){ //Any other mouse buttons event. 
    event.type = BUTTON_EV;
    mb = false;
  }
  else {
    event.type = MOUSE_MOV; //Mouse movement event. 
  }

  return &event;
}

void (mouse_ih)(void){
  read_command(&content); 
}


bool (mouse_gesture)(struct packet *p, uint8_t x_len, uint8_t tolerance){
  //mouse_ev: type (0.LB_PRESSED, 1.LB_RELEASED, 2.RB_PRESSED, 3.RB_RELEASED, 4.BUTTON_EV, 5.MOUSE_MOV), DELTA_X,DELTA_Y
  struct mouse_ev *mouse_event = detect_event_mouse(p);
  //struct mouse_ev *mouse_event = mouse_detect_event(p); //-->professor

  switch(mov){
    case Initial:
      //The start of a line is marked by the pressing down of the relevant button when all buttons are released
      if(mouse_event->type== LB_PRESSED){
        mov = First_draw;
      }

      lx=0;
      ly=0;
      break; 

    case First_draw:
      if(mouse_event->type == LB_RELEASED){
        /*The absolute value of the slope of each line must be larger than 1 and the value of the displacement of each line along the x-direction must have the minimum value specified in the first argument, x_len.*/
        if((lx >= x_len)){
          if(mouse_event->delta_x != 0 && (abs(mouse_event->delta_y/mouse_event->delta_x)>1)){
            mov = Vertex;
          }
          if (mouse_event->delta_x == 0){
            mov = Vertex;
          }
          
        }
        
        else {
          mov = Initial; //start again as soon as it determines that the mouse events do not satisfy the requirements -- p.e another button pressed (by default this one is executed )
        }
      }

      else if(mouse_event->type == MOUSE_MOV){
        if(mouse_event->delta_x < 0 && (abs(mouse_event->delta_x)>tolerance)){
          mov = Initial;
        }
        else if(mouse_event->delta_y < 0 && (abs(mouse_event->delta_y)>tolerance)){
          mov = Initial;
        }
        else{
          lx += mouse_event->delta_x;//add the x movement
          ly += mouse_event->delta_y;//add the y movement
        }
      }

      else {
        mov = Initial;  //start again as soon as it determines that the mouse events do not satisfy the requirements -- p.e another button pressed (by default this one is executed)      
      }
      break;

    case Vertex:
      lx=0;//reset x position
      ly=0;//reset y position
      if(mouse_event->type == RB_PRESSED){//right button pressed, when all buttons released
        mov= Second_draw;
      }

      //Every movement reported between the two button-related events that define the vertex must be residual only, i.e. the absolute value in both the x and the y directions cannot be larger than the value of the second, tolerance, argument. 
      else if(mouse_event->type == MOUSE_MOV){
        if(abs(mouse_event->delta_x)<tolerance && abs(mouse_event->delta_y)<tolerance){
          mov = Vertex;
        }
      }
    
      else {
        mov = Initial;  //start again as soon as it determines that the mouse events do not satisfy the requirements -- p.e another button pressed (by default this one is executed)  
      }
       
      break;

    case Second_draw:
      if(mouse_event->type == RB_RELEASED){
        /*The absolute value of the slope of each line must be larger than 1 and the value of the displacement of each line along the x-direction must have the minimum value specified in the first argument, x_len.*/
       
        if((lx >= x_len)){
          if(mouse_event->delta_x != 0 && (abs(mouse_event->delta_y/mouse_event->delta_x)>1)){
            return true;
          }
          if (mouse_event->delta_x == 0){
            return true;
          }
        
        }
        else {
          mov=Initial;  //start again as soon as it determines that the mouse events do not satisfy the requirements -- p.e another button pressed (by default this one is executed)  
        }
      }
      else if(mouse_event->type == MOUSE_MOV){
        if(mouse_event->delta_x < 0 && (abs(mouse_event->delta_x)>tolerance)){
            mov = Initial;
        }
        else if(mouse_event->delta_y > 0 && (abs(mouse_event->delta_y)>tolerance)){
          mov = Initial;
        }
        else{
          lx += mouse_event->delta_x;//add the x movement
          ly += mouse_event->delta_y;//add the y movement
        }
      }

      else {
        mov=Initial;  //start again as soon as it determines that the mouse events do not satisfy the requirements -- p.e another button pressed (by default this one is executed)  
        }

      break;
  }
  return false; 
}
   
