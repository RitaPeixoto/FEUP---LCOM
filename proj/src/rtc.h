#pragma once

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "auxiliar_structs.h"
#include "auxiliar_functions.h"
#include "macros_rtc.h"
#include "video_gr.h"


/** @defgroup rtc rtc
 * @{
 *
 * Functions to deal with RTC   
 */

/**
* @brief Function that converts a value from binary coded decimal to decimal
* @param value - The value to be converted
* @return Return the resulting binary value after conversion
*/
int bcd_decimal_converter(int value);

/**
* @brief Function that reads a register from the real time clock 
* @param reg - Register of the rtc we need to read
* @return Return the content of the register upon success; -1 otherwise
*/
int read_rtc_value(uint32_t reg);

/**
* @brief Function that reads the currents values from the rtc and converts them to decimal before updating the struct realtime, cleaning the previous clock (if first is false) and showing the new one  
* @param realtime - Struct that will be updated with the new information from the rtc
* @param numbers - Struct that contains all the images (numbers and symbols) needed to show the current time
* @param first - If true, it means that the clock will be shown for the first time
* @param menu_struct - Menu struct consisting of all the information needed to "clean" the previous clock
*/
void getRealTime(RealTime *realtime, Numbers * numbers, bool first, Menu *menu_struct);


/**
* @brief Function that shows on the screen the current time
* @param realtime - Struct that contains all the information needed to show the current time
* @param numbers - Struct that contains all the images (numbers and symbols) needed to show the current time
*/
void draw_real_time(RealTime *realtime, Numbers * numbers);

/**
* @brief Function that deletes the previous time from the screen
* @param realtime - Struct that contains all the information needed to delete the previous time
* @param menu_struct - Menu struct consisting of all the information needed to "clean" the previous clock
*/
void clean_time(RealTime *realtime, Menu *menu_struct);


/**@}*/

