// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "video_gr.h"
#include "keyboard.h"
#include "i8254.h"
#include "i8042.h"

extern uint16_t h_resolution, v_resolution;
uint8_t keyirq, timerirq;
extern uint8_t scancode;
extern uint32_t timer_counter;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  /*Inicializar o video card no modo grafico
  1. Configure the video card for the desired graphics mode
  2. We have to wait "delay" seconds
  3. Reset the video card controller to the text mode used by minix - provided function vg_exit()
  */

  if(vg_init(mode)==NULL){ //1.
    printf("It was detected an error in vbe_get_mode_info!\n");
    return 1;
  }

  sleep(delay);//2.

  vg_exit();//3.---> 0x03: 25 X 80 , 16 colors
  
  return 0;

}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,uint16_t width, uint16_t height,    uint32_t color) {
  
  if(vg_init(mode)==NULL){ //VBE graphics mode to use
    printf("It was detected an error in vg_init!\n");
    return 1;
  }


  if(vg_draw_retangle(x,y,width, height,color)!=0){
    printf("It was detected an error in vg_draw_retangle!\n");
    return 1;
  }
 
  if (kbd_subscribe_int(&keyirq) != 0) { //subscribe the KBC interrupts
      printf("It was detected an error in kbd_subscribe_int!\n");
      return 1;
  }
 
  function_esc_code();
 
  vg_exit();//3.---> 0x03: 25 X 80 , 16 colors
  if (kbd_unsubscribe_int() != 0) { //unsubscribe the KBC interrupts
        printf("It was detected an error in kbd_unsubscribe_int!\n"); // Checks if subscription worked
        return 1;
    }
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  

  if(vg_init(mode)==NULL){ //VBE graphics mode to use
    printf("It was detected an error in vg_init!\n");
    return 1;
  }
 
  if (kbd_subscribe_int(&keyirq) != 0) { //subscribe the KBC interrupts
      printf("It was detected an error in kbd_subscribe_int!\n");
      return 1;
  }


  int h = h_resolution/no_rectangles, v = v_resolution/no_rectangles;
  uint32_t color = first;
  
  for(int v_lines = 0; v_lines < no_rectangles; v_lines++){

    for(int h_lines = 0; h_lines < no_rectangles; h_lines++){
      
      color = getColor(mode, first, v_lines, h_lines, step, no_rectangles);

      if(vg_draw_retangle(h_lines*h, v_lines*v, h, v, color)!=0){
        printf("It was detected an error in vg_draw_retangle!\n");
        return 1;
      }
    }

  }


  function_esc_code();
 
  vg_exit();//3.---> 0x03: 25 X 80 , 16 colors
  if (kbd_unsubscribe_int() != 0) { //unsubscribe the KBC interrupts
        printf("It was detected an error in kbd_unsubscribe_int!\n"); // Checks if subscription worked
        return 1;
    }
  return 0;

}



int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  uint16_t mode = 0x105;
  uint8_t *aux;

  if(vg_init(mode)==NULL){ //VBE graphics mode to use
    printf("It was detected an error in vg_init!\n");
    return 1;
  }

  if (kbd_subscribe_int(&keyirq) != 0) { //subscribe the KBC interrupts
      printf("It was detected an error in kbd_subscribe_int!\n");
      return 1;
  }

  xpm_image_t img;

  aux = xpm_load(xpm, XPM_INDEXED, &img); // passes xpm_img to pixmap

  if(draw_xpm(x, y, img) != 0){
    printf("It was detected an error in draw_xpm!\n");
    return 1;
  }


  function_esc_code();
 
  vg_exit();//3.---> 0x03: 25 X 80 , 16 colors
  if (kbd_unsubscribe_int() != 0) { //unsubscribe the KBC interrupts
        printf("It was detected an error in kbd_unsubscribe_int!\n"); // Checks if subscription worked
        return 1;
    }
  return 0;

}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) { 
  
  uint16_t mode = 0x105;
  uint8_t *aux;
  int r; // driver_receive() returns an integer
	message msg;
  int ipc_status;
  int frame_counter = 0;


  if(vg_init(mode)==NULL){ //VBE graphics mode to use
    printf("It was detected an error in vg_init!\n");
    return 1;
  }

  xpm_image_t img;

  aux = xpm_load(xpm, XPM_INDEXED, &img); 

  if(draw_xpm(xi, yi, img) != 0){ //draw first image
    printf("It was detected an error in draw_xpm!\n");
    return 1;
  }
  

  if(timer_subscribe_int(&timerirq)!=0){//subscription of the timer
    printf("It was detected an error in timer_subscribe_int!\n"); // Checks if subscription worked
		return 1;
  }

  if(kbd_subscribe_int(&keyirq)!=0){//subscription of the keyboard
     printf("It was detected an error in kbd_subscribe_int!\n"); // Checks if subscription worked
		return 1;
  }

  while (scancode != ESC_BREAK_CODE) {	// function should exit when the user releases the Esc key
  
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("ERROR: driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 									// received notification
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: 																	// hardware interrupt notification

        if (msg.m_notify.interrupts & timerirq){ //subscribed timer interrupt
          timer_int_handler(); //increments timer_counter

          if(timer_counter % (sys_hz() / fr_rate) == 0){ //aligns pc's frame rate to the desired frame rate

            if(speed >= 0){  //when speed is positive: displacement in pixels between consecutive frames
            clean_screen(img, xi, yi); 

            yi += speed;
            if(yi > yf)
              yi = yf;
          

            xi+=speed;

            if(xi > xf)
              xi = xf;
          

            draw_xpm(xi, yi, img);
            }
            else{ //speed is negative: number of frames required for a displacement of one pixel
              frame_counter++;

              if(frame_counter == abs(speed)){
                clean_screen(img, xi, yi);

                yi++;
                if(yi >yf){
                  yi = yf;
                }

                xi++;
                if(xi > xf){
                  xi = xf;

                }

                draw_xpm(xi,yi,img);

                frame_counter = 0;                
              }
            }
            
          }

        }

        if (msg.m_notify.interrupts & keyirq ) { // subscribed keyboard interrupt
          kbc_ih();   // keyboard handler
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

  
  vg_exit();

  return 0;


}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
