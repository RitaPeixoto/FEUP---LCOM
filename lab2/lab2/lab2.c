#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern uint32_t COUNTER;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {

  uint8_t st;  
  if(timer < 0 || timer > 2){
    printf("ERROR: Invalid timer!\n"); //detects if function call worked well
	  return 1;
  }

  if(timer_get_conf(timer, &st) != 0){
    printf("It was detected an error in timer_get_conf!\n"); //detects if function call worked well
	  return 1;
  }

  if(timer_display_conf(timer, st, field) != 0){
    printf("It was detected an error in timer_display_conf!\n"); //detects if function call worked well
	  return 1;
  }

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  
  if(timer_set_frequency(timer, freq) != 0){
    printf("It was detected an error in timer_set_frequency!\n");
    return 1;
  }

  return 0;
}

int(timer_test_int)(uint8_t time) {

  /*

  STEPS:
  1. Subscribe TImer 0 interrupts
  2. Print message at 1 second intervals, by calling the LCF function: void timer_print_elapsed_time()
  3. Unsubscribe TImer 0 at the end
  
  
  */
  
  int ipc_status;
	message msg;
  int r; // driver_receive() returns an integer
  uint8_t bit_no = 1;
  uint8_t irq_set = BIT(1);

  if(timer_subscribe_int(&bit_no) != 0){ //step 1
    printf("It was detected an error in timer_subscribe_int!\n");
    return 1;
  }

	while (COUNTER/60.0 < time) { 
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
			printf("driver_receive failed with: %d!", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* verifies if it is a notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt --> verifies if it is */
				  
          //step 2
          timer_int_handler(); //increments COUNTER

          if((COUNTER % 60) == 0){ // we have to print a message once per second
            timer_print_elapsed_time(); // returns the current time count ---> we can't detect errors
          }
          
				}
				break;

			default:
				break; /* no other notifications expected: do nothing*/
			}

		}
		else { /* received a standard message, not a notification*/
			   /* no standard messages expected: do nothing*/
		}
	}

  if(timer_unsubscribe_int() != 0){ // step 3
    printf("It was detected an error in timer_unsubscribe_int!\n");
    return 1;
  }


  return 0;
  
}
