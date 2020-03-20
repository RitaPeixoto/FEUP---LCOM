#include "keyboard.h"


static uint8_t scancode = 0;
static int kbd_hook_id = KBD_IRQ;

uint8_t getScancode(){
  return scancode;
}


int (kbd_subscribe_int)(uint8_t *bit_no){
 
    *bit_no = BIT(kbd_hook_id);
    /*we want the variable received as paramater to have the bit of the mask of the keyboard as 1*/
    //sys_irqsetpolicy() should be used to subscribe a notification on every interrupt in the input irq_line

    if(sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE , &kbd_hook_id) != 0){ //Use not only theIRQ_REENABLEbut also theIRQ_EXCLUSIVEpolicy insys_irqsetpolicy(), i.e. useIRQ_REENABLE|IRQ_EXCLUSIVE
    //hook_id guarda o nosso id

      printf("It was detected an error in sys_irqsetpolicy!\n");
      return 1;
    }
  return 0;
}

int (kbd_unsubscribe_int)() { //Unsubscribes Keyboard interrupt
  
  //sys_irqrmpolicy() unsubscribes a previous subscription of the interrupt notification associated with the specified hook_id 

  if(sys_irqrmpolicy(&kbd_hook_id) != 0){ 

    //note: hook_id is the value returned by sys_irqsetpolicy() in its hook_id argument

      printf("It was detected an error in sys_irqrmpolicy!\n");
      return 1;
    }

  return 0;
}


int (kbd_util_sys_inb)(int port, uint8_t *value) {

  uint32_t aux;
  if(sys_inb(port, &aux) != 0){
    printf("It was detected an error in sys_inb!\n");//detects if function call worked well
	  return 1;
  }
  *value = aux & 0xFF;
  
  return 0;
}


void (kbc_ih)(void){
  
  kbd_read(&scancode);

}

void (kbd_read)(uint8_t *aux){
    uint8_t str;
    for (int i = 0; i< 16; i++) {  
    if (kbd_util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in kbd_util_sys_inb!\n"); //detects if function call worked well
      return; 
    }
     
    if(str & OBF) { //output buffer can't be empty
      
      if ((str & (PAR_ERROR | TO_ERROR| AUX)) == 0 ){ // we have to detect if there is any error (parity or timeout) and if it is a mouse data
        if(kbd_util_sys_inb(OUT_BUF, aux) != 0){
          printf("It was detected an error in kbd_util_sys_inb!\n"); //detects if function call worked well
          return;
        }
        return;
      }
    }  
    tickdelay(micros_to_ticks(DELAY_US)); // SECTION 4.2 ---> This function is similar to sleep() in that it blocks the process that executes it for the time interval specified in its argument. After that time interval, the process resumes execution by executing the instruction that follows tickdelay().
  }
}


void (kbd_write)(int port, uint8_t cmd){ //port could be argument or command
  
  uint8_t str;

  for (int i = 0; i< 16; i++) { 
    if (kbd_util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in kbd_util_sys_inb!\n"); //detects if function call worked well

      return; 
    }
     
    if(!(str & IBF)) { 
      if((str & (PAR_ERROR | TO_ERROR| AUX)) == 0 ){
        if(sys_outb(port, cmd) != 0){
          printf("It was detected an error in kbd_util_sys_inb!\n"); //detects if function call worked well
          return;
        }
      }
      
      return;
    }
  }  
  tickdelay(micros_to_ticks(DELAY_US));
}

int (enable_interrupts)(){
  uint8_t byte = 0;

  kbd_write(STATUS_REGISTER, READ_CMD_BYTE); //it sends read command byte; enables read
  kbd_read(&byte); //it reads command byte from the output buffer

  byte = byte | KBC_INT; // it chenges bit(0) to 1 --> enable interrupt on OBF, from keyboard

  kbd_write(STATUS_REGISTER, WRITE_CMD_BYTE); //it sends write command byte; enables write
  kbd_write(OUT_BUF, byte); // it writes the output buffer in the register 


  return 0;

}


