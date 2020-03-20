#include "interrupts.h"
static struct packet p;
static int number_bytes = 0;
static uint8_t pack[3];
static bool check = false;
static int choice_menu = 0;
static bool first_time = true;
static int x_prev, y_prev;
extern uint32_t timer_counter;
static bool start = true;

int interrupts_state_menu(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, Mouse *mouse, Numbers *numbers_rtc, RealTime *realtime){

  if(msg.m_notify.interrupts & mouseirq){
    mouse_ih();
    
    switch(number_bytes){
      case 0:
        if(getContent() & MOUSE_CHECK_BIT){
          pack[0] = getContent();
          number_bytes++;
        }
      break;

      case 1:
        pack[1] = getContent();
        number_bytes++;
      break;

      case 2:
        pack[2] = getContent();
        number_bytes = 0;

        if((pack[0] & MOUSE_CHECK_BIT) == 0) return 0;

        mouse_packet_parse(&p, pack);
        check = check_possible_mov(p, mouse, true);//checks the mouse mov

        if(check){
          choice_menu = mouse_gestures(menu_struct, &p, mouse->x, mouse->y, realtime, numbers_rtc);//checks if pressed any button
        }

        if(choice_menu == 2){
          return 2;//choose quit
        }

        else if (choice_menu == 1){//choose play
          first_time = true;
          menu_struct->menu_type = instructions_stat;//gets to the next menu
          return 1;
        }
      break;
    }
  }

  if (msg.m_notify.interrupts & keyirq){
    kbc_ih();
  }

  if(msg.m_notify.interrupts & timerirq){
    if(first_time ){//draws full screen
      timer_counter = 0;

      mouse->x = 61;
      mouse->y = getvresolution() - 41 - 60;

      x_prev = mouse->x;
      y_prev = mouse->y;

      draw_menu(menu_struct);
      draw_mouse(mouse);
    }

    timer_int_handler();//update timer_counter

    if(timer_counter%60 == 0 || first_time){//draws date and time 
      getRealTime(realtime, numbers_rtc, false, menu_struct);
    }
    
    first_time = false;

    if(check){//updates the screen accordingly

      if(menu_struct->menu_type == initial_stat){
        clean_mouse(x_prev, y_prev, menu_struct->img_initial);//normal
      }
      else if(menu_struct->menu_type == play_stat){
        clean_mouse(x_prev, y_prev, menu_struct->img_play);//on top of play button
      }
      else if(menu_struct->menu_type == quit_stat){
        clean_mouse(x_prev, y_prev, menu_struct->img_quit);//on top of the quit button
      }

      x_prev = mouse->x;
      y_prev = mouse->y;

      draw_mouse(mouse);

    }

    updateBuffer();
  }

  return 0;
}

int interrupts_state_instructions(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, Mouse *mouse){

  if(msg.m_notify.interrupts & mouseirq){
    mouse_ih();

    switch(number_bytes){
      case 0:
        if(getContent() & MOUSE_CHECK_BIT){
          pack[0] = getContent();
          number_bytes++;
        }
      break;

      case 1:
        pack[1] = getContent();
        number_bytes++;
      break;

      case 2:
        pack[2] = getContent();
        number_bytes = 0;
        
        if((pack[0] & MOUSE_CHECK_BIT) == 0) return 0;

        mouse_packet_parse(&p, pack);
        check = check_possible_mov(p, mouse, false);//checks the mouse mov

        if(check){
          choice_menu = mouse_instructions(menu_struct, &p, mouse->x, mouse->y);//checks if pressed any button

          if(choice_menu == 1){//choose to return to the main menu
            first_time = true;
            return 1;
          }

        }

      break;
    }
  }

  if(msg.m_notify.interrupts & keyirq){
    kbc_ih();
  }

  if(msg.m_notify.interrupts & timerirq){

    if(first_time){//draws full screen
      mouse->x = 61;
      mouse->y = getvresolution() - 41 - 60;

      x_prev = mouse->x;
      y_prev = mouse->y;

      first_time = false;
      timer_counter = 0;

      draw_menu(menu_struct);
      draw_mouse(mouse);
    }

    timer_int_handler();//update timer_counter

    if((timer_counter%60 == 0) && (timer_counter/60 == 10)){//display the instructions for 10 seconds the it goes to the level menu
      menu_struct->menu_type = level_stat;
      first_time = true;
      return 2;
    }

    else if(check){
      if(menu_struct->menu_type == instructions_stat){
        clean_mouse(x_prev, y_prev, menu_struct->img_instructions);//normal
      }
      else if(menu_struct->menu_type == instructions_button_stat){
        clean_mouse(x_prev, y_prev, menu_struct->img_instructions_button);//on top of the home button
      }

      x_prev = mouse->x;
      y_prev = mouse->y;

      draw_mouse(mouse);
    }

    updateBuffer();
  }

  return 0;
}

int interrupts_state_level(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, Mouse *mouse, xpm_image_t img2, xpm_image_t img3, xpm_image_t img4, xpm_image_t img5,bool esc){

  if(msg.m_notify.interrupts & mouseirq){
    mouse_ih();

    switch(number_bytes){
      case 0:
        if(getContent() & MOUSE_CHECK_BIT){
          pack[0] = getContent();
          number_bytes++;
        }
      break;

      case 1:
        pack[1] = getContent();
        number_bytes++;
      break;

      case 2:
        pack[2] = getContent();
        number_bytes = 0;
        
        if((pack[0] & MOUSE_CHECK_BIT) == 0) return 0;

        mouse_packet_parse(&p, pack);
        check = check_possible_mov(p, mouse, false);//checks the mouse mov

        if(check){
          choice_menu = mouse_button(menu_struct, &p, mouse->x, mouse->y, getLevel());//checks if pressed any button
        }

        if(choice_menu == 2){//choose leave the program
          return 2;
        }
        
        else if(choice_menu == 1){ //choose the next level
          start = true;
          return 1; 
        }

      break;
    }
  }

  if(msg.m_notify.interrupts & keyirq){
    kbc_ih();
  }

  if(msg.m_notify.interrupts & timerirq){
    if(first_time || esc){
      timer_counter = 0; 

      start = true;

      mouse->x = 61;
      mouse->y = getvresolution() - 41 - 60;

      x_prev = mouse->x;
      y_prev = mouse->y;

      first_time = false;
      if(getLevel() ==1){
      draw_menu(menu_struct);
      draw_mouse(mouse);
      }

      if(getLevel() == 2){
        menu_struct->img_level = img2;
        draw_menu(menu_struct);
        draw_mouse(mouse);
      }
      else if(getLevel() == 3){
        menu_struct->img_level = img3;
        draw_menu(menu_struct);
        draw_mouse(mouse);
      }
      else if(getLevel() == 4){
        menu_struct->img_level = img4;
        draw_menu(menu_struct);
        draw_mouse(mouse);
      }
      else if(getLevel() == 5){
        menu_struct->img_level = img5;
        draw_menu(menu_struct);
        draw_mouse(mouse);
      }

      
    }
    if(check){
      clean_mouse(x_prev, y_prev, menu_struct->img_level);
      x_prev = mouse->x;
      y_prev = mouse->y;

      draw_mouse(mouse);
    }

    updateBuffer();
  }

  return 0;
}


int interrupts_state_maze(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, uint16_t width, uint16_t height, Numbers *numbers){

  if(msg.m_notify.interrupts & mouseirq){
    mouse_ih();
  }

  if(msg.m_notify.interrupts & keyirq){
    kbc_ih();
    if(!start){
      int a = play(width+4*(getLevel()-1), height+4*(getLevel()-1));
    
      if(a == 2){//case of passing the level
        setLevel(getLevel()+1);//updates the level
        first_time = true;
        start = true;
        timer_counter = 0;
        if (getLevel() == 6){
          menu_struct->menu_type = message_all_passed;//passed all levels
        }
        else{
           menu_struct->menu_type = message_completed_stat;//displays "LEVEL COMPLETED" message
        }
       
        return 1;
      }

      else if (a == 1){//case of failing the level
        start = true;
        first_time = true;
        timer_counter = 0;
        menu_struct->menu_type = message_failed_stat;//displays "LEVEL FAILED" message
        return 1;
      }

      else if(getScancode() == ESC_BREAK_CODE){ //pressed esc--- quitted the level 
        start = true;
        timer_counter = 0;
        menu_struct->menu_type = level_stat;
        return 2;
      }

      
    }
  }

  if(msg.m_notify.interrupts & timerirq){
    if(start){
      start = false;
      create_game(width+4*(getLevel()-1), height+4*(getLevel()-1));//generates the maze
      visualize_counter(0, numbers, width+4*(getLevel()-1), height+4*(getLevel()-1), true);//displays the counter
      timer_counter = 0;
    }
    timer_int_handler();//updates timer_counter


    if(timer_counter % 60 == 0){
      visualize_counter(timer_counter/60, numbers, width+4*(getLevel()-1), height+4*(getLevel()-1), false);//updates the counter
    }

    updateBuffer();
  }

  return 0;
}

int interrupts_state_message(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct)
{
  if(msg.m_notify.interrupts & mouseirq){
    mouse_ih();
  }

  if(msg.m_notify.interrupts & keyirq){
    kbc_ih();
  }

  if(msg.m_notify.interrupts & timerirq){
    if(first_time){
      timer_counter = 0;
      draw_menu(menu_struct);
      first_time = false;
    }
    timer_int_handler();//updates the timer _counter

    if((timer_counter%60 == 0) && (timer_counter/60 >= 3)){
      first_time = true;
      return 1;
    }
    updateBuffer();    
  }

  return 0;
}




