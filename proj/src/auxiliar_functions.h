#pragma once


#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "auxiliar_structs.h"
#include "video_gr.h"
#include "cursor2.xpm"
#include "0.xpm"
#include "1.xpm"
#include "2.xpm"
#include "3.xpm"
#include "4.xpm"
#include "5.xpm"
#include "6.xpm"
#include "7.xpm"
#include "8.xpm"
#include "9.xpm"
#include "colon.xpm"
#include "0_rtc.xpm"
#include "1_rtc.xpm"
#include "2_rtc.xpm"
#include "3_rtc.xpm"
#include "4_rtc.xpm"
#include "5_rtc.xpm"
#include "6_rtc.xpm"
#include "7_rtc.xpm"
#include "8_rtc.xpm"
#include "9_rtc.xpm"
#include "colon_rtc.xpm"
#include "slash.xpm"
#include "game_instructions.xpm"
#include "instructions_home.xpm"
#include "level_completed.xpm"
#include "level_failed.xpm"
#include "all_passed.xpm"
#include "menu_play.xpm"
#include "menu_quit.xpm"
#include "main_menu.xpm"
#include "level1.xpm"
#include "level2.xpm"
#include "level3.xpm"
#include "level4.xpm"
#include "level5.xpm"




/** @defgroup auxiliar_functions auxiliar_functions
 * @{
 *
 * Auxiliar functions that deal with the mouse movement on the screen  
 */

/**
* @brief Function that creates the mouse in its' start position
* @param mouse - Mouse struct where the information of the mouse image on the screen will be saved
*/
void create_mouse(Mouse *mouse);
/**
* @brief Function that draws the mouse on the screen with the information on the parameter
* @param mouse - Mouse struct with the information needed to draw the mouse
* @return Returns 0 upon success, not zero otherwise
*/
int draw_mouse(Mouse *mouse);
/**
* @brief Function that calculates the new coordinates of the mouse upon movement, checking if it does not exceed the screen limit and dealing with this kind of situations
* @param p - Struct packet with the information of the mouse packet
* @param mouse - Mouse struct with the current information of the mouse
* @param mainmenu - True if the program is in the main menu, false otherwise
* @return Return true if there hasn't been any overflow, false otherwise
*/
bool check_possible_mov(struct packet p, Mouse *mouse, bool mainmenu);
/**
* @brief Function that deletes the mouse from the screen
* @param mouse_x - Mouse x coordinate
* @param mouse_y - Mouse y coordinate
* @param img - Image needed to "clean" the mouse
*/
void clean_mouse(int mouse_x, int mouse_y, xpm_image_t img);
/**
* @brief Function that draws the menu
* @param menu_struct - Menu struct with the information needed to draw the menu
*/
void draw_menu(Menu *menu_struct);

/**
* @brief Function that creates the struct menu
* @param menu_struct - Struct where all the menu information will be saved
*/
void create_menu(Menu *menu_struct);

/**
* @brief Function that loads all images needed to show the counter
* @param numbers - Struct that will contain all the images (numbers and symbols) needed to show the time or a counter
* @param rtc - True if the counter is RTC, false otherwise
*/
void create_numbers(Numbers * numbers, bool rtc);

/**
 * @brief Function that returns the xpm of the number passed as argument
 * @param numbers - Struct that contains all the images (numbers and symbols) needed to show the current time
 * @param number - Number that it will be returned as an image
 * @return Return the xpm of the number passed as argument
*/
xpm_image_t numbers_to_xpm(Numbers *numbers, int number);

/**
 * @brief Function that separates the time and updates the struct time_now with the parameters given by the argument time
 * @param seconds - Time that it will be separated
 * @param numbers - Struct that contains all the images (numbers and symbols) needed to show the current time
 */ 

void divide_time(int seconds, Numbers *numbers);

/**
 * @brief Function that shows a counter on the screen
 * @param seconds - Time that passed since the game have started
 * @param numbers - Numbers struct where all information of the numbers' image is saved
 * @param width - Width of the maze
 * @param height - Height of the maze
 * @param first - If it is true, the game has started in that moment
 */

void visualize_counter(int seconds, Numbers *numbers, uint16_t width, uint16_t height, bool first);

/**@}*/
