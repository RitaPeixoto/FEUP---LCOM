#include "menu.h"
static uint8_t keyirq=1, mouseirq=12,timerirq=0;
static Menu *menu_struct;
static Mouse *mouse;
static int level = 1;
static enum state_game state;
static bool initial = true;
static RealTime *realtime;
 
int getLevel(){
  return level;
} 
void setLevel(int l){
  level=l;
}
  
int menu(){
 
  Numbers *numbers, *numbers_rtc;
  message msg;
  int r;
  int ipc_status;
  xpm_image_t img2, img3, img4, img5;
  uint8_t *aux;
  int choice;
  bool esc =false;
  bool quitgame = false;
 
  uint16_t width = 19 + level*4, height = 19+level*4;
 
 
  if(initial){
    state = state_menu;
    menu_struct = malloc(sizeof(Menu));
    create_menu(menu_struct);
    initial = false;
    mouse = malloc(sizeof(Mouse));
    create_mouse(mouse);
    aux = xpm_load(level2_xpm, XPM_8_8_8_8, &img2);
    aux = xpm_load(level3_xpm, XPM_8_8_8_8, &img3);
    aux = xpm_load(level4_xpm, XPM_8_8_8_8, &img4);
    aux = xpm_load(level5_xpm, XPM_8_8_8_8, &img5);
    numbers = malloc(sizeof(Numbers));
    create_numbers(numbers, false);
    numbers_rtc = malloc(sizeof(Numbers));
    create_numbers(numbers_rtc, true);
    realtime = malloc(sizeof(RealTime));
  }
 
  updateBuffer();
  if(timer_subscribe_int(&timerirq)!=0){//subscription of the timer
    printf("It was detected an error in timer_subscribe_int!\n"); // Checks if subscription worked
    return 1;
  }
 
 
  if(kbd_subscribe_int(&keyirq)!=0){//subscription of the keyboard
    printf("It was detected an error in kbd_subscribe_int!\n"); // Checks if subscription worked
    return 1;
  }
 
  if(write_command(MOUSE_ENABLE) != 0) { //enable data reporting
    printf("It was detected an error in write_command!\n");
    return 1;
  }
   
  if (mouse_subscribe_int(&mouseirq) != 0) { //subscribe the mouse interrupts
    printf("It was detected an error in mouse_subscribe_int!\n"); // Checks if subscription worked
    return 1;
    }

  
  while (true) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("ERROR: driver_receive failed with: %d", r);
      continue;
    }
 
 
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
       
      case HARDWARE: // hardware interrupt notification
       switch(state){

        case state_menu:// case of being in the main menu

          choice = interrupts_state_menu(msg, keyirq, mouseirq, timerirq, menu_struct, mouse, numbers_rtc, realtime);

          if(choice == 1){//choose play
            state = state_instructions;
          }
          else if (choice == 2){//choose quit
            quitgame = true;
          }

        break;

        case state_instructions:// case of being in the instructions

          choice = interrupts_state_instructions(msg, keyirq, mouseirq, timerirq, menu_struct, mouse);

          if(choice == 1){//choose the hom button
            state = state_menu;
          }
          else if(choice == 2){//passed to the levels
            state = state_level;
          }
        break;

        case state_level://case of being in the levels menu

          choice = interrupts_state_level(msg, keyirq, mouseirq, timerirq, menu_struct, mouse, img2, img3, img4, img5, esc);
          esc = false;
          if(choice == 1){//choose the corresponding level
            state = state_maze;
          }
          else if(choice == 2){//choose quit
            quitgame = true;
          }
        break;

        case state_maze://case of being in the maze 

          choice = interrupts_state_maze(msg, keyirq, mouseirq, timerirq, menu_struct, width, height, numbers);

          if(choice == 1){//ended the level
            state = state_message;
          }

          else if(choice == 2){//pressed esc--- quitted the level 
            esc = true;
            state = state_level;
          }

        break;

        case state_message://case of being in the message display

          choice = interrupts_state_message(msg, keyirq, mouseirq, timerirq, menu_struct);
          if((menu_struct->menu_type == message_all_passed) && (level ==6)){//passed all levels
            quitgame = true;
          }
          if(choice == 1){//returns to levels menu
            state = state_level;
          }

        break;

       }
        
      break;
      default:
        break;  // no other notifications expected: do nothing
      }
    }
    else {
      //do nothing
    }

    if(quitgame){
      break;
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
   
  if (timer_unsubscribe_int() != 0) { //unsubscribe the timer interrupts
    printf("It was detected an error in timer_unsubscribe_int!\n"); // Checks if subscription worked
    return 1;
  }
 
  if (kbd_unsubscribe_int() != 0) { //unsubscribe the timer interrupts
    printf("It was detected an error in kbd_unsubscribe_int!\n"); // Checks if subscription worked
    return 1;
  }
 
  return 0;
}
