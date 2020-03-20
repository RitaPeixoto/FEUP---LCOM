#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include "keyboard.h"
#include "timer.c" 


extern uint32_t counter; //the total count of sys_inb() kernel calls
uint8_t keyirq = 0, timerirq = 0;
extern uint8_t scancode;
extern uint32_t timer_counter;
static bool two_bytes = false; // if the scancode is two byte long 

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() { //Tests reading of scancodes via KBD interrupts. 

  /*
  1. Subscribe the KBC interrupts;
  2. The passing of data between the interrupt handler and the other function must be done via global variables. Call kbc_ih();
  3. Every time the IH receives a byte from the keyboard, kbd_test_scan() should try to assemble a scancode (we don't have to do code in assembly). If it succeeds it should print it on the console using kbd_print_scancode;
  4. kbd_test_scan() function should exit when the user releases the Esc key, whose break code is 0x81 (ESC_BREAK_CODE);
  5. Unsubscribe the KBC interrupts;
  6. Call kbd_print_no_sysinb to print the value of counter; */

  int r; // driver_receive() returns an integer
	message msg;
  int ipc_status;

  //size--> size of the scancode in bytes; bytes[2]--> array of bytes of the scancode (max==2) 
  uint8_t size, bytes[2];	
	bool make; //if it is a make or break code 


	if (kbd_subscribe_int(&keyirq) != 0) { //subscribe the KBC interrupts
		printf("It was detected an error in kbd_subscribe_int!\n");
		return 1;
	}
	while (scancode != ESC_BREAK_CODE) {			// function should exit when the user releases the Esc key
		
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("ERROR: driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 									// received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: 																	// hardware interrupt notification
				if (msg.m_notify.interrupts & keyirq ) { // subscribed interrupt
                  
          make = true; // initialize make as true --> it will only be changed in this iteration if it is a break code

					kbc_ih();   // keyboard handler

          if(two_bytes){
            two_bytes = false;
            bytes[1] = scancode;//second byte in second position
            size = 2; 
          }
          else if(scancode == TWO_BYTE_LONG){ //if it is a two-byte scancode
            bytes[0] = scancode; //first byte in first position
            size = 1;
            two_bytes = true;  
          }

          else{ //if it is a one-byte scancode
            bytes[0] = scancode; //byte in first position
            size = 1; 
            two_bytes = false;
          }
            
          if(scancode & BIT(7)){
            make = false; //it is a break code
          }

          kbd_print_scancode(make, size, bytes); //Prints the input scancode
				}
				break;
			default:
				break;	// no other notifications expected: do nothing
			}
		}
		else { 			// received a standard message, not a notification  
			// no standard messages expected: do nothing
		}
	}

  if (kbd_unsubscribe_int() != 0) { //unsubscribe the KBC interrupts
		printf("It was detected an error in kbd_unsubscribe_int!\n"); // Checks if subscription worked
		return 1;
	}
  
	kbd_print_no_sysinb(counter); // prints the number of sys_inb calls, no need to be implemented
	counter = 0; //we have to reset the counter
	
	return 0;
}

int(kbd_test_poll)() {
  
  scancode = 0;
  uint8_t size, bytes[2];	
	bool make;
  uint8_t str;


    
  while (scancode != ESC_BREAK_CODE){

    if(kbd_util_sys_inb(STATUS_REGISTER, &str) != 0){
      printf("It was detected an error in kbd_util_sys_inb!\n"); // Checks if subscription worked
		  return 1;
    }

    kbd_read(&scancode);

    make = true; // initialize make as true --> it will only be changed in this iteration if it is a break code

    if(two_bytes){
      two_bytes = false;
      bytes[1] = scancode;//second byte in second position
      size = 2; 
    }
		else if(scancode == TWO_BYTE_LONG){ //if it is a two-byte scancode
      bytes[0] = scancode; //first byte in first position
      size = 1;
      two_bytes = true;  
    }

    else{ //if it is a one-byte scancode
      bytes[0] = scancode; //byte in first position
      size = 1; 
      two_bytes = false;
    }

          
    if(scancode & BIT(7)){
      make = false; //it is a break code
    }

    kbd_print_scancode(make, size, bytes); //Prints the input scancode

  }

  

  if(enable_interrupts() != 0){//cancel the subscription of the KBC interrupt before terminating
    printf("It was detected an error in kbd_util_sys_inb!\n"); // Checks if subscription worked
		return 1;
  }

  kbd_print_no_sysinb(counter);//counts the number of times that sys_inb is called 
  counter = 0;


  return 0;
}

int(kbd_test_timed_scan)(uint8_t idle) {
  int r; // driver_receive() returns an integer
	message msg;
  int ipc_status;

  //size--> size of the scancode in bytes; bytes[2]--> array of bytes of the scancode (max==2) 
  uint8_t size, bytes[2];	
	bool make; //if it is a make or break code 

  if(timer_subscribe_int(&timerirq)!=0){//subscription of the timer
    printf("It was detected an error in timer_subscribe_int!\n"); // Checks if subscription worked
		return 1;
  }

  if(kbd_subscribe_int(&keyirq)!=0){//subscription of the keyboard
     printf("It was detected an error in kbd_subscribe_int!\n"); // Checks if subscription worked
		return 1;
  }

  while (scancode != ESC_BREAK_CODE && (timer_counter/60.0) < idle) {	// function should exit when the user releases the Esc key
  
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("ERROR: driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 									// received notification
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: 																	// hardware interrupt notification

        if (msg.m_notify.interrupts & timerirq){ //subscribed timer interrupt
          timer_int_handler();
        }

        if (msg.m_notify.interrupts & keyirq ) { // subscribed keyboard interrupt

          make = true; // initialize make as true --> it will only be changed in this iteration if it is a break code

          kbc_ih();   // keyboard handler

          if(two_bytes){
            two_bytes = false;
            bytes[1] = scancode;//second byte in second position
            size = 2; 
          }
          else if(scancode == TWO_BYTE_LONG){ //if it is a two-byte scancode
            bytes[0] = scancode; //first byte in first position
            size = 1;
            two_bytes = true;  
          }

          else{ //if it is a one-byte scancode
            bytes[0] = scancode; //byte in first position
            size = 1; 
            two_bytes = false;
          }
          
          if(scancode & BIT(7)){
            make = false; //it is a break code
          }

          kbd_print_scancode(make, size, bytes); //Prints the input scancode
          timer_counter = 0;//reset the counter
        }


        break;
      default:
        break;	// no other notifications expected: do nothing
      }
    }
    else { 			// received a standard message, not a notification
      // no standard messages expected: do nothing
    }
	}

  if (kbd_unsubscribe_int() != 0) { //unsubscribe the KBC interrupts
		printf("It was detected an error in kbd_unsubscribe_int!\n"); // Checks if subscription worked
		return 1;
	}
  
  if (timer_unsubscribe_int() != 0) { //unsubscribe the timer interrupts
		printf("It was detected an error in timer_unsubscribe_int!\n"); // Checks if subscription worked
		return 1;
	}


  kbd_print_no_sysinb(counter);//counts the number of times that sys_inb is called 
  counter = 0;
  

  return 0;
}




