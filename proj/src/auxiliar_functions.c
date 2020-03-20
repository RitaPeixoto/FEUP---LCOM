#include "auxiliar_functions.h"

static Time *time_now;
static int m1_prev = 0, m2_prev = 0, s1_prev = 0, s2_prev = 0;

void create_mouse(Mouse *mouse){
  
  uint8_t *aux;
  xpm_image_t img;

  aux = xpm_load(cursor2_xpm, XPM_8_8_8_8, &img);
  //initial position of the mouse
  mouse->x = 61;
  mouse->y = getvresolution() - 41 - 60;

  mouse->img = img;
  
  draw_mouse(mouse);//draws the mouse on the screen
}
 
int draw_mouse(Mouse *mouse){
 
  if(draw_xpm(mouse->x, mouse->y, mouse->img)!=0){
    return 1;
  }  
  return 0;
}

bool check_possible_mov(struct packet p, Mouse *mouse, bool mainmenu){

  if(p.x_ov || p.y_ov) return false;

  uint16_t x_min, y_min, x_max, y_max;

  if(mainmenu){
    //limits of the main menu
    x_min = 61;
    y_min = 61;
    x_max = gethresolution()-61;
    y_max = getvresolution()-61;
  }
  else {
    //limits of the rest of the menus 
    x_min = 0;
    y_min = 0;
    x_max = gethresolution();
    y_max = getvresolution();
  }

  
  int d_x, d_y;
  //does the incrementation of the movement of the mouse
  d_x = mouse->x + p.delta_x;
  d_y = mouse->y - p.delta_y;

//checks the movement
  if(d_x < x_min){ 
    mouse->x = x_min;
  }
  else if((d_x + 40) > x_max){
    mouse->x = x_max - 40;
  }
  else {
    mouse->x = d_x;
  }
  if(d_y < y_min){
    mouse->y = y_min;
  }
  else if((d_y + 40) > y_max){
    mouse->y = y_max - 40;
  }
  else {
    mouse->y = d_y;
  }
  
  return true;
}

void clean_mouse(int mouse_x, int mouse_y, xpm_image_t img){
  
  uint32_t *m = (uint32_t *) img.bytes;

  for(int aux_y = mouse_y; aux_y < mouse_y + 40; aux_y++){

    for(int aux_x = mouse_x; aux_x < mouse_x + 40; aux_x++){
      uint32_t aux = *(m + aux_x + aux_y*gethresolution());
      change_pixel_color_32(aux_x, aux_y, aux);    
  
    }
  }
}

void draw_menu(Menu *menu_struct){
 //paints on the screen the corresponding image of the program actual state
  if(menu_struct->menu_type == initial_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_initial);
  }
  else if(menu_struct->menu_type == play_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_play);
  }
  else if(menu_struct->menu_type == quit_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_quit);
  }
  else if(menu_struct->menu_type == level_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_level);
  }
  else if(menu_struct->menu_type == instructions_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_instructions);
  } 
  else if(menu_struct->menu_type == instructions_button_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_instructions_button);
  }
  else if(menu_struct->menu_type == message_completed_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_message_complete);
  }
  else if(menu_struct->menu_type == message_failed_stat){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_message_failed);
  }
  else if(menu_struct->menu_type == message_all_passed){
    draw_xpm(menu_struct->x, menu_struct->y, menu_struct->img_all_passed);
  }
}

void create_menu(Menu *menu_struct){
  xpm_image_t img;
  uint8_t *aux;

  menu_struct->x = 0;
  menu_struct->y = 0;

  //fills the struct with all the  images needed
  aux = xpm_load(main_menu_xpm, XPM_8_8_8_8, &img);
  menu_struct->menu_type = initial_stat;
  menu_struct->img_initial = img;

  draw_menu(menu_struct);

  aux = xpm_load(menu_play_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_play = img;

  aux = xpm_load(menu_quit_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_quit = img;

  aux = xpm_load(level1_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_level = img;

  aux = xpm_load(game_instructions_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_instructions = img;

  aux = xpm_load(instructions_home_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_instructions_button = img;

  aux = xpm_load(level_failed_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_message_failed = img;

  aux = xpm_load(level_completed_xpm, XPM_8_8_8_8, &img);
  menu_struct->img_message_complete = img;

  aux = xpm_load(all_passed_xpm, XPM_8_8_8_8,&img);
  menu_struct->img_all_passed = img;
  return;
}

void create_numbers(Numbers * numbers, bool rtc){
  xpm_image_t img;
  uint8_t *aux;

  if(!rtc){ //fils the struct number with the RTC numbers
    aux = xpm_load(zero_xpm, XPM_8_8_8_8, &img);
    numbers->zero = img;

    aux = xpm_load(one_xpm, XPM_8_8_8_8, &img);
    numbers->one = img;

    aux = xpm_load(two_xpm, XPM_8_8_8_8, &img);
    numbers->two = img;

    aux = xpm_load(three_xpm, XPM_8_8_8_8, &img);
    numbers->three = img;

    aux = xpm_load(four_xpm, XPM_8_8_8_8, &img);
    numbers->four = img;

    aux = xpm_load(five_xpm, XPM_8_8_8_8, &img);
    numbers->five = img;

    aux = xpm_load(six_xpm, XPM_8_8_8_8, &img);
    numbers->six = img;

    aux = xpm_load(seven_xpm, XPM_8_8_8_8, &img);
    numbers->seven = img;

    aux = xpm_load(eight_xpm, XPM_8_8_8_8, &img);
    numbers->eight = img;

    aux = xpm_load(nine_xpm, XPM_8_8_8_8, &img);
    numbers->nine = img;

    aux = xpm_load(colon_xpm, XPM_8_8_8_8, &img);
    numbers->colon = img;
    
    time_now = malloc(sizeof(time_now));
  }
  else { //fils the struct number with the maze counter numbers
    aux = xpm_load(zero_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->zero = img;

    aux = xpm_load(one_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->one = img;

    aux = xpm_load(two_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->two = img;

    aux = xpm_load(three_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->three = img;

    aux = xpm_load(four_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->four = img;

    aux = xpm_load(five_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->five = img;

    aux = xpm_load(six_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->six = img;

    aux = xpm_load(seven_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->seven = img;

    aux = xpm_load(eight_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->eight = img;

    aux = xpm_load(nine_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->nine = img;

    aux = xpm_load(colon_rtc_xpm, XPM_8_8_8_8, &img);
    numbers->colon = img;

    aux = xpm_load(slash_xpm, XPM_8_8_8_8, &img);
    numbers->slash = img;
  }
}

xpm_image_t numbers_to_xpm(Numbers *numbers, int number){
  //returns the image corresponding to the parameter number
  if(number == 0){
    return numbers->zero;
  }
  else if (number == 1){
    return numbers->one;
  }
  else if (number == 2){
    return numbers->two;
  }
  else if (number == 3){
    return numbers->three;
  }
  else if (number == 4){
    return numbers->four;
  }
  else if (number == 5){
    return numbers->five;
  }
  else if (number == 6){
    return numbers->six;
  }
  else if (number == 7){
    return numbers->seven;
  }
  else if (number == 8){
    return numbers->eight;
  }
  else if (number == 9){
    return numbers->nine;
  }
  return numbers->zero;
}


void divide_time(int seconds, Numbers *numbers){
  int minutes = seconds/60;
  int sec_aux = seconds%60;

  //if the number of minutes is lower or equal to nine is in the form hh:0m:ss
  if(minutes >= 0 && minutes <= 9){
    time_now->m1 = 0;
    time_now->m2 = minutes;
  }
  //if the number of minutes is higher than nine and lower than 60 is in the form hh:mm:ss
  else if(minutes > 9 && minutes <= 59){
    time_now->m1 = minutes/10;
    time_now->m2 = minutes%10;
  }
  // get the images to display the time
  time_now->m1_xpm = numbers_to_xpm(numbers, time_now->m1);
  time_now->w1 = time_now->m1_xpm.width;
  time_now->m2_xpm = numbers_to_xpm(numbers, time_now->m2);
  time_now->w2 = time_now->m2_xpm.width;

  if(sec_aux >= 0 && sec_aux <= 9){
    time_now->s1 = 0;
    time_now->s2 = sec_aux;
  }
  else if(sec_aux > 9 && sec_aux <= 59){
    time_now->s1 = sec_aux/10;
    time_now->s2 = sec_aux%10;
  }
  
  time_now->s1_xpm = numbers_to_xpm(numbers, time_now->s1);
  time_now->w3 = time_now->s1_xpm.width;
  time_now->s2_xpm = numbers_to_xpm(numbers, time_now->s2);
  time_now->w4 = time_now->s2_xpm.width;

}



void visualize_counter(int seconds, Numbers *numbers, uint16_t width, uint16_t height, bool first){

  uint16_t xcentrado = ceil(gethresolution()/2.0) - (ceil(width/2.0))*15;
	uint16_t ycentrado = ceil(getvresolution()/2.0) - (ceil(height/2.0)+2)*15;
  uint16_t x = (width*15 - 8*15) + xcentrado, y = height*15 + 2*15 + ycentrado, x_aux = x;
  
  //updates the time_now struct with the current counter 
  divide_time(seconds, numbers);

  //displays the counter on the screen
  if(first){
    draw_xpm(x, y, time_now->m1_xpm);
  }
  else if(time_now->m1 != m1_prev){
    clean_xpm(numbers_to_xpm(numbers, m1_prev), x_aux, y);
    draw_xpm(x, y, time_now->m1_xpm);
  }

  x += time_now->w1 + 2;
  x_aux += numbers_to_xpm(numbers, m1_prev).width + 2;

  if(first){
    draw_xpm(x, y, time_now->m2_xpm);
  }
  else if(time_now->m2 != m2_prev){
    clean_xpm(numbers_to_xpm(numbers, m2_prev), x_aux, y);
    draw_xpm(x, y, time_now->m2_xpm);
  }

  x+= time_now->w2 + 2;
  x_aux += numbers_to_xpm(numbers, m2_prev).width + 2;
  
  clean_xpm(numbers->colon, x_aux, y);
  draw_xpm(x, y, numbers->colon);
  
  x+= numbers->colon.width + 2;
  x_aux += numbers->colon.width + 2;

  if(first){
    draw_xpm(x, y, time_now->s1_xpm);
  }
  else if(time_now->s1 != s1_prev){
    clean_xpm(numbers_to_xpm(numbers, s1_prev), x_aux, y);
    draw_xpm(x, y, time_now->s1_xpm);
  }

  x += time_now->w3 + 2;
  x_aux += numbers_to_xpm(numbers, s1_prev).width + 2;
  
  if(first){
    draw_xpm(x, y, time_now->s2_xpm);
  }
  else if(time_now->s2 != s2_prev){
    clean_xpm(numbers_to_xpm(numbers, s2_prev), x_aux, y);
    draw_xpm(x, y, time_now->s2_xpm);
  }
  
  m1_prev = time_now->m1, m2_prev = time_now->m2, s1_prev = time_now->s1, s2_prev = time_now->s2;
}



