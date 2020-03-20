// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"



//global variables
uint8_t keyirq=0, timerirq, mouseirq;
extern uint8_t content;
extern uint32_t timer_counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    
  int r; // driver_receive() returns an integer
	message msg;
  int ipc_status;
  struct packet p;
  int number_bytes = 0; //counts the number of bytes already "processed"/printed
  uint32_t counter = 0;
  uint8_t pack[3];

  if(write_command(MOUSE_ENABLE) != 0) { //enable data reporting
		printf("It was detected an error in write_command!\n");
		return 1;
  }
	
  if (mouse_subscribe_int(&mouseirq) != 0) { //subscribe the mouse interrupts
		printf("It was detected an error in mouse_subscribe_int!\n"); // Checks if subscription worked
		return 1;
	}

	while (cnt > counter) {	//terminates after printing the number of packets specified in its argument cnt
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("ERROR: driver_receive failed with: %d", r);
			continue;
		}

  
		if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        
			case HARDWARE: 																	// hardware interrupt notification
				if (msg.m_notify.interrupts & mouseirq) { // subscribed interrupt    
          mouse_ih();  //must read only one byte     
          switch(number_bytes){

            case 0:
            if(content & MOUSE_CHECK_BIT){
              pack[0] = content;
              number_bytes++;
            }           
            break;

            case 1:
              pack[1] = content;
              number_bytes++;
              break;

            case 2: //when we have read the 3 bytes of the packets we print it using mouse_print_packet()
              pack[2] = content;
              number_bytes = 0; //reset of the variable
              counter++;
              if((pack[0] & MOUSE_CHECK_BIT) == 0){
                printf("ERROR: erase pack because first byte is not correct!\n");
                break;
              }
              mouse_packet_parse(&p, pack); ////functions that parses the packet in the struct packet components
 
              mouse_print_packet(&p);//prints the packet
              break;
            
          }
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


  if (mouse_unsubscribe_int() != 0) { //unsubscribe the mouse interrupts
		printf("It was detected an error in mouse_unsubscribe_int!\n"); // Checks if it worked
		return 1;
	}

  if(write_command(MOUSE_DISABLE) != 0) { //disable data reporting
		printf("It was detected an error in write_command!\n"); 
		return 1;
	}


  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  struct packet p;
  int number_bytes = 0; //counts the number of bytes already "processed"/printed
  uint32_t counter = 0;
  uint8_t pack[3];

  while(counter<cnt){
    if( write_command(READ_DATA)!= 0) { //read data
		  printf("It was detected an error in write_command!\n"); // Checks if subscription worked
		  return 1;
	  }
     mouse_ih();//must read only one byte   
     switch(number_bytes){
      case 0:
        if(content & MOUSE_CHECK_BIT){
          pack[0] = content;
          number_bytes++;
          }           
          break;

      case 1:
        pack[1] = content;
        number_bytes++;
        break;

      case 2:
        pack[2] = content;
        number_bytes = 0;
        counter++;
        if((pack[0] & MOUSE_CHECK_BIT) == 0){
          printf("ERROR: erase pack because first byte is not correct!\n");
          break;
        }
        tickdelay(micros_to_ticks(period*1000)); //miliseconds
        mouse_packet_parse(&p, pack);
        mouse_print_packet(&p);
        break;
      
    }
   
  }

  if(write_command(STREAM_MODE) != 0) { //enable stream mode
		printf("It was detected an error in write_command!\n"); // Checks if subscription worked
		return 1;
	}

  if(write_command(MOUSE_DISABLE) != 0) { //ensure data reporting is disable
		printf("It was detected an error in write_command!\n"); 
		return 1;
	}

  //Reset the KBC’s command byte to its default value
  if(write_kbc_byte(CMD_REG,WRITE_CMD_BYTE)!=0){
    printf("It was detected an error in write_kbc_byte!\n");
		return 1;
  }

  if(write_kbc_byte(IN_BUF,minix_get_dflt_kbc_cmd_byte())!=0){
    printf("It was detected an error in write_kbc_byte!\n"); 
		return 1;
  }
 ////
  return 0;

}

int (mouse_test_async)(uint8_t idle_time) {
  int r; // driver_receive() returns an integer
	message msg;
  int ipc_status;

  struct packet p;
  int number_bytes = 0;
  uint8_t pack[3];
  uint8_t idle=0;


  if(write_command(MOUSE_ENABLE) != 0) { //enable data reporting
		printf("It was detected an error in write_command!\n"); // Checks if subscription worked
		return 1;
	}
	if (mouse_subscribe_int(&mouseirq) != 0) { //subscribe the mouse interrupts
		printf("It was detected an error in mouse_subscribe_int!\n");
		return 1;
	}
  if(timer_subscribe_int(&timerirq)!=0){ //subscribe the timer interrupts
    	printf("It was detected an error in timer_subscribe_int!\n");
		return 1;
  }



	while (idle_time > idle) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("ERROR: driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { 									// received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: 																	// hardware interruptotification
        
				if (msg.m_notify.interrupts & mouseirq ) { // mouse interrupt    
          timer_counter=0;//reset timer_counter
          mouse_ih();      
          switch(number_bytes){

            case 0:
            if(content & MOUSE_CHECK_BIT){
              pack[0] = content;
              number_bytes++;
            }           
            break;

            case 1:
              pack[1] = content;
              number_bytes++;
              break;

            case 2:
              pack[2] = content;
              number_bytes = 0;
              if((pack[0] & MOUSE_CHECK_BIT) == 0){
                printf("ERROR: erase pack because first byte is not correct!\n");
                break;
              }
              mouse_packet_parse(&p, pack);
              mouse_print_packet(&p);
              break;
            
          }
          idle =0;
          continue;
				}
        if(msg.m_notify.interrupts & timerirq ){//timer interrupt
          timer_int_handler();
          if (timer_counter%60 ==0)
            idle++;
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


  if (mouse_unsubscribe_int() != 0) { //unsubscribe the mouse interrupts
		printf("It was detected an error in mouse_unsubscribe_int!\n"); // Checks if subscription worked
		return 1;
	}

  if(write_command(MOUSE_DISABLE) != 0) { //disable data reporting
		printf("It was detected an error in write_command!\n");
		return 1;
	}
  
  if(timer_unsubscribe_int()!=0){//unsubscribe time interrupts
    printf("It was detected an error in timer_unsubscribe_int!\n");
		return 1;
  }


  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int r; // driver_receive() returns an integer
	message msg;
  int ipc_status;
  struct packet p;
  int number_bytes = 0;
  uint8_t pack[3];
  bool symbol=false;


  if (mouse_subscribe_int(&mouseirq) != 0) { //subscribe the mouse interrupts
		printf("It was detected an error in mouse_subscribe_int!\n");// Checks if subscription worked
		return 1;
	}

  if(write_command(MOUSE_ENABLE) != 0) { //disable data reporting
		printf("It was detected an error in write_command!\n"); 
		return 1;
	}

  while (!symbol) {			//the function should terminate if the user draws a logical and symbol, i.e. an inverted V, from left to right
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("ERROR: driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 									// received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: 																	// hardware interruptotification

				if (msg.m_notify.interrupts & mouseirq ) { // subscribed interrupt    
          mouse_ih();      
          switch(number_bytes){

            case 0:
            if(content & MOUSE_CHECK_BIT){
              pack[0] = content;
              number_bytes++;
            }           
            break;

            case 1:
              pack[1] = content;
              number_bytes++;
              break;

            case 2:
              pack[2] = content;
              number_bytes = 0;
              if((pack[0] & MOUSE_CHECK_BIT) == 0){
                printf("ERROR: erase pack because first byte is not correct!\n");
                break;
              }
              mouse_packet_parse(&p, pack);
              symbol=mouse_gesture(&p, x_len, tolerance);
              mouse_print_packet(&p);
              break;
            
          }
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
	



   if (mouse_unsubscribe_int() != 0) { //unsubscribe the mouse interrupts
		printf("It was detected an error in mouse_unsubscribe_int!\n"); // Checks if subscription worked
		return 1;
	}

  if(write_command(MOUSE_DISABLE) != 0) { //disable data reporting
		printf("It was detected an error in write_command!\n"); 
		return 1;
	}


  return 0; 
}
