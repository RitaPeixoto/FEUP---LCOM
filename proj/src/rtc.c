#include "rtc.h"

static int second_prev, minute_prev, hour_prev, day_prev, month_prev, year_prev;

int bcd_decimal_converter(int value){
  return (((value & 0xF0)>>4)*10 + (value & 0x0F));
}

int read_rtc_value(uint32_t reg){

  uint32_t reg_aux, result, result_aux;

  do{

    if(sys_outb(RTC_ADDR_REG, RTC_REG_A)!= 0){ // write the address of the register to the RTC_ADDR_REG
      printf("It was detected an error in sys_outb!\n");
      return -1;
    }

    if(sys_inb(RTC_DATA_REG, &reg_aux)){ // read one byte from the RTC_DATA_REG
      printf("It was detected an error in sys_inb!\n");
      return -1;
    } 
    
  } while(reg_aux & RTC_UIP); //update in progress; do not access time/date registers

  if(sys_outb(RTC_ADDR_REG, reg)!= 0){ 
    printf("It was detected an error in sys_outb!\n");
    return -1;
  }

  if(sys_inb(RTC_DATA_REG, &result_aux)){ // read one byte from the RTC_DATA_REG
    printf("It was detected an error in sys_inb!\n");
    return -1;
  } 

  result = (int)result_aux;
  return result;
}


void getRealTime(RealTime *realtime, Numbers *numbers, bool first, Menu *menu_struct){

  int seconds = read_rtc_value(SECOND);
  int minutes = read_rtc_value(MINUTE);
  int hours = read_rtc_value(HOUR);
  int day = read_rtc_value(DAY);
  int month = read_rtc_value(MONTH);
  int year = read_rtc_value(YEAR);

  
  if(!(RTC_REG_B & RTC_BCD_MODE) && (seconds != -1) && (minutes != -1) && (hours != -1) && (day != -1) &&  (month != -1) && (year != -1)){
    seconds = bcd_decimal_converter(seconds);
    realtime->second = seconds;

    minutes = bcd_decimal_converter(minutes);
    realtime->minute = minutes;
    
    hours = bcd_decimal_converter(hours);
    realtime->hour = hours;

    day = bcd_decimal_converter(day);
    realtime->day = day;

    month = bcd_decimal_converter(month);
    realtime->month = month;

    year = bcd_decimal_converter(year);
    realtime->year = year;
  }
  if(!first){
    clean_time(realtime, menu_struct);
  }
  
  draw_real_time(realtime, numbers);
}


void draw_real_time(RealTime *realtime, Numbers *numbers){


  int first, second;
  uint16_t x = 60,  y = getvresolution() - 46; 
  xpm_image_t aux;

  
  first = (realtime->day)/10;
  second = (realtime->day)%10;
  aux = numbers_to_xpm(numbers, first);
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers_to_xpm(numbers, second);
  draw_xpm(x, y, aux);
  

  x += aux.width + 2;
  draw_xpm(x, y, numbers->slash);

  x += numbers->slash.width + 2;
  first = (realtime->month)/10;
  second = (realtime->month)%10;
  aux = numbers_to_xpm(numbers, first);
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers_to_xpm(numbers, second);
  draw_xpm(x, y, aux);
  
  x += aux.width + 2;
  draw_xpm(x, y, numbers->slash);

  x += numbers->slash.width + 2;
  aux = numbers->two;
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers->zero;
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  first = (realtime->year)/10;
  second = (realtime->year)%10;
  aux = numbers_to_xpm(numbers, first);
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers_to_xpm(numbers, second);
  draw_xpm(x, y, aux);

  x += aux.width + 40;
  first = (realtime->hour)/10;
  second = (realtime->hour)%10;
  aux = numbers_to_xpm(numbers, first);
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers_to_xpm(numbers, second);
  draw_xpm(x, y, aux);

  x += aux.width + 2;
  draw_xpm(x, y, numbers->colon);

  x += numbers->colon.width + 2;
  first = (realtime->minute)/10;
  second = (realtime->minute)%10;
  aux = numbers_to_xpm(numbers, first);
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers_to_xpm(numbers, second);
  draw_xpm(x, y, aux);

  x += aux.width + 2;
  draw_xpm(x, y, numbers->colon);

  x += numbers->colon.width + 2;
  first = (realtime->second)/10;
  second = (realtime->second)%10;
  aux = numbers_to_xpm(numbers, first);
  draw_xpm(x, y, aux);
  x += aux.width + 2;
  aux = numbers_to_xpm(numbers, second);
  draw_xpm(x, y, aux);
  realtime->wt = x + aux.width;


  second_prev = realtime->second, minute_prev = realtime->minute, hour_prev = realtime->hour, day_prev = realtime->day, month_prev = realtime->month, year_prev = realtime->year;
  return;

}

void clean_time(RealTime *realtime, Menu *menu_struct){

  xpm_image_t img;

  if(menu_struct->menu_type == initial_stat) {
    img = menu_struct->img_initial;
  }

  else if(menu_struct->menu_type == play_stat){
    img = menu_struct->img_play;
  }

  else if(menu_struct->menu_type == quit_stat){
    img = menu_struct->img_play;
  }

  uint32_t *m = (uint32_t *) img.bytes;
  int width = realtime->wt;

  for(int aux_y = getvresolution() - 60; aux_y < getvresolution(); aux_y++){
    for(int aux_x = 60; aux_x < width + 40; aux_x++){
      uint32_t aux = *(m + aux_x + aux_y*gethresolution());
      change_pixel_color_32(aux_x, aux_y, aux);    
  
    }
  }

}
