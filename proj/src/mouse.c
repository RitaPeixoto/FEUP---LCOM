#include "mouse.h"

static int mouse_hook_id = 2;
static uint8_t content = 0;
static enum state_t mov = SOMETHING_ELSE;
static struct mouse_ev event;
static bool lb = false, rb = false, mb = false; //detects if it a certain button was pressed before

uint8_t getContent(){
  return content;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  *lsb = (uint8_t)val; // does the casting of val(2 bytes) to an 8 bit integer obtaining the least significant byte of val

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  val = val>>8; //shift right that gets the most significant byte in the least significant byte position
  *msb = (uint8_t)val; //does the casting of val(2 bytes) to an 8 bit integer obtaining the least significant byte of val, that by now represents the most significant byte 

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t aux;
  if(sys_inb(port, &aux) != 0){
    printf("It was detected an error in sys_inb!\n");
	  return 1;
  }

  *value = aux & 0xFF;

  return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no){ //Subscribes Mouse interrupts and enables data report
    *bit_no = BIT(mouse_hook_id);
    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE , &mouse_hook_id) != 0){
      printf("It was detected an error in sys_irqsetpolicy!\n");
      return 1;
    }
  return 0;
}

int (mouse_unsubscribe_int)() { //Unsubscribes Mouse interrupts and disable data report 
  
  if(sys_irqrmpolicy(&mouse_hook_id) != 0){ 
    printf("It was detected an error in sys_irqrmpolicy!\n");
    return 1;
  }
  return 0;
}

void (read_command)(uint8_t *aux){
  uint8_t str;
  for (int i = 0; i < 4; i++) {  
    if (util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in util_sys_inb!\n"); 
      return; 
    }

    if(str & OBF) { //output buffer can't be empty
      if ((str & (PAR_ERROR | TO_ERROR)) == 0 ){ // detects if there is any error (parity or timeout)
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
 
  do{
    write_kbc_byte(CMD_REG, WRITE_BYTE);//Sends write byte to mouse command to the control register
    write_kbc_byte(OUT_BUF, command);//Reads mouse command from the output buffer
    tickdelay(micros_to_ticks(DELAY_US));
    util_sys_inb(IN_BUF, &ack);//Sends mouse command to the Input Buffer
    if(ack == ERROR){//Receives ack  and checks if ack is correct 
      printf("ERROR: Second consecutive invalid byte \n");
      return -1;
    }
  } while(ack != ACK);//if it is not correct tries again
  
  return 0;
}

int (write_kbc_byte)(int port, uint8_t cmd){ //port could be argument or command
 
  uint8_t str;
 
  for (int i = 0; i< 16; i++) {
    if (util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in kbd_util_sys_inb!\n");
      return 1;
    }
     
    if(!(str & IBF)) {
      if(sys_outb(port, cmd) != 0){
        printf("It was detected an error in kbd_util_sys_inb!\n");
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


struct mouse_ev* (detect_event_mouse)(struct packet *p){ //Detects mouse events relevant to the program . This is the lower state-machine of a 2-layered state-machine.

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


int (mouse_gestures)(Menu *menu, struct packet *p, int mouse_x, int mouse_y, RealTime *realtime, Numbers* numbers_rtc){
  struct mouse_ev *mouse_event = detect_event_mouse(p);
  uint16_t max_y1 = 371+80, max_y2 = 581+80, max_x = 427+300, min_y1 = 371, min_y2 = 581, min_x=427;
  uint16_t min_yplay = 360, max_yplay = 360+109, min_xplay =381, max_xplay=381+383, min_yquit=584, min_xquit=392, max_yquit=584+112, max_xquit=392+384;

  switch(mov){
    case SOMETHING_ELSE:
        if(mouse_event->type == LB_RELEASED){//
          mov = BUTTON_RELEASED;
        }
        else if(mouse_event->type == MOUSE_MOV){
          if((mouse_x >= min_x) && (mouse_x <= max_x)){
            if((mouse_y>= min_y1) && (mouse_y <= max_y1) && (menu->menu_type != play_stat)){
              menu->menu_type = play_stat;
              draw_menu(menu);
              getRealTime(realtime, numbers_rtc, false, menu);
            }
            else if((mouse_y>= min_y1) && (mouse_y <= max_y1)){
              //do nothing
            }

            else if((mouse_y >= min_y2) && (mouse_y <= max_y2) && (menu->menu_type != quit_stat)){
              menu->menu_type = quit_stat;
              draw_menu(menu);
              getRealTime(realtime, numbers_rtc, false, menu);
            }
            else if ((mouse_y >= min_y2) && (mouse_y <= max_y2)){
              //do nothing
            }

            else if (menu->menu_type != initial_stat){
              menu->menu_type = initial_stat;
              draw_menu(menu);
              getRealTime(realtime, numbers_rtc, false, menu);
            }
          }
          else if (menu->menu_type != initial_stat){
            menu->menu_type = initial_stat;
            draw_menu(menu);
            getRealTime(realtime, numbers_rtc, false, menu);
          }

          
          
          break;
        }   

    case BUTTON_RELEASED:
      if((mouse_x >= min_xplay) && (mouse_x<=max_xplay)){
        if((mouse_y>= min_yplay) && (mouse_y <= max_yplay)){
          return 1;
        }
      }
      if((mouse_x >= min_xquit) && (mouse_x<=max_xquit)){
        if((mouse_y >= min_yquit) && (mouse_y <= max_yquit)){
          return 2;
        }
      }
      mov = SOMETHING_ELSE;
      break;
  }
  return 0; 
}
   


int (mouse_button)(Menu *menu, struct packet *p, int mouse_x, int mouse_y, int level){
  struct mouse_ev *mouse_event = detect_event_mouse(p);
  uint16_t x_minq = 930.0, x_maxq= 930.0+179.0, y_minq=783.0, y_maxq=783.0+48.0;

  switch(mov){
    case SOMETHING_ELSE:
      if(mouse_event->type == LB_RELEASED){
        mov = BUTTON_RELEASED;
      }
      else if(mouse_event->type == MOUSE_MOV){
        break;
      }
         
    case BUTTON_RELEASED:
      if((mouse_x >= x_minq) && (mouse_x <= x_maxq)){
        if((mouse_y >= y_minq) && (mouse_y <= y_maxq)){
          return 2;
        }
      }
      else if(level == 1){
        if((mouse_x >= 130.0) && (mouse_x<=(130.0+179.0))){
          if((mouse_y >= 420.0) && (mouse_y <= (420.0+48.0))){
            return 1;
          }
        }
      }
      else if (level ==2){
        if((mouse_x >= 312.0) && (mouse_x<=(312.0+179.0))){
          if((mouse_y>= 579.0) && (mouse_y <= (579.0+48.0))){
            return 1;
          }
        }
      }
      else if (level ==3){
        if((mouse_x >= 498.0) && (mouse_x<=(498.0+179.0))){
          if((mouse_y>= 420.0) && (mouse_y <= (420.0+48.0))){
            return 1;
          }
        }
      }
      else if (level == 4){
        if((mouse_x >= 678.0) && (mouse_x<=(678.0+179.0))){
          if((mouse_y>= 575.0) && (mouse_y <= (575.0+48.0))){
            return 1;
          }
        }
      }
      else if (level == 5){
        if((mouse_x >= 845.0) && (mouse_x<=(845.0+179.0))){
          if((mouse_y>= 419.0) && (mouse_y <= (419.0+48.0))){
            return 1;
          }
        }
    }
      
      mov = SOMETHING_ELSE;
    break;
  }
  return 0; 
}

int (mouse_instructions)(Menu *menu, struct packet *p, int mouse_x, int mouse_y){
  struct mouse_ev *mouse_event = detect_event_mouse(p);
  uint16_t min_xhome = 1043, max_xhome = 1043+80, min_yhome = 744, max_yhome=744+80;
  

  switch(mov){
    case SOMETHING_ELSE:
        if(mouse_event->type == LB_RELEASED){
          mov = BUTTON_RELEASED;
        }
        else if(mouse_event->type == MOUSE_MOV){
          if((mouse_x >= min_xhome) && (mouse_x <= max_xhome)){
            if((mouse_y>= min_yhome) && (mouse_y <= max_yhome) && (menu->menu_type != instructions_button_stat)){
              menu->menu_type = instructions_button_stat;
              draw_menu(menu);
            }
            else if((mouse_y>= min_yhome) && (mouse_y <= max_yhome)){
              //do nothing
            }
            else if (menu->menu_type != instructions_stat){
              menu->menu_type = instructions_stat;
              draw_menu(menu);
            }
          }
          else if (menu->menu_type != instructions_stat){
            menu->menu_type = instructions_stat;
            draw_menu(menu);
          }
          break;
        }   

    case BUTTON_RELEASED:
      
      if((mouse_x >= min_xhome) && (mouse_x <= max_xhome)){
        if((mouse_y >= min_yhome) && (mouse_y <= max_yhome)){
          menu->menu_type = initial_stat;
          return 1;
        }
      }
      
      mov = SOMETHING_ELSE;
      break;
  }
  return 0; 
}




/*

Home:
 -x = 1043
 -y = 744
 -width = 80
 -height = 80

Main Menu:
  Play:
  -x=427
  -y=371
  -width=300
  -height=80

  Quit:
  -x=427
  -y=581
  -width=300
  -height=80
Play Menu:
  Play:
  -x=381
  -y=360
  -width=383
  -height=109

   Quit:
  -x=427
  -y=581
  -width=300
  -height=80

Quit Menu:
  Play:
  -x=427
  -y=371
  -width=300
  -height=80

  Quit :
  -x=392
  -y=584
  -width=384
  -height=112  

*/












