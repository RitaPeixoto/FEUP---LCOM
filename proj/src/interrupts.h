#pragma once
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "auxiliar_structs.h"
#include "mouse.h"
#include "play.h"

/** @defgroup interrupts  interrupts
 * @{
 *
 * Functions to work with the interrupts in the different program states
 */

/**
* @brief Function that deals with the interrupts notifications when the program is on the main menu
* @param msg - Message used for interrupt notification
* @param keyirq - Value used in the interrupt notifications of the keyboard
* @param mouseirq - Value used in the interrupt notifications of the mouse
* @param timerirq - Value used in the interrupt notifications of the timer
* @param menu_struct - Menu struct consisting of all the information needed to update the frame 
* @param mouse - Mouse struct with the current information of the mouse
* @param numbers_rtc - Struct that contains all the images (numbers and symbols) needed to show the current time
* @param realtime - Struct that contains all the information of the current time
* @return Return 1 if the play button has been chosen; return 2 if the quit button has been chose; return 0 otherwise
*/
int interrupts_state_menu(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, Mouse *mouse, Numbers *numbers_rtc, RealTime *realtime);

/**
* @brief Function that deals with the interrupts notifications when the program is on the instructions state
* @param msg - Message used for interrupt notification
* @param keyirq - Value used in the interrupt notifications of the keyboard
* @param mouseirq - Value used in the interrupt notifications of the mouse
* @param timerirq - Value used in the interrupt notifications of the timer
* @param menu_struct - Menu struct consisting of all the information needed to update the frame 
* @param mouse - Mouse struct with the current information of the mouse
* @return Return 1 if the player chose to return to the main menu; return 2 after 10 seconds; 
return 0 otherwise
*/
int interrupts_state_instructions(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, Mouse *mouse);

/**
* @brief Function that deals with the interrupts notifications when the program is on the level menu
* @param msg - Message used for interrupt notification
* @param keyirq - Value used in the interrupt notifications of the keyboard
* @param mouseirq - Value used in the interrupt notifications of the mouse
* @param timerirq - Value used in the interrupt notifications of the timer
* @param menu_struct - Menu struct consisting of all the information needed to update the frame 
* @param mouse - Mouse struct with the current information of the mouse
* @param img2 - Image used to update the background when the player completed level 1
* @param img3 - Image used to update the background when the player completed level 2
* @param img4 - Image used to update the background when the player completed level 3
* @param img5 - Image used to update the background when the player completed level 4
* @param esc - true if it is in the level menu because of the user pressing esc on the maze, false otherwise
* @return Return 1 if the level button has been chosen; return 2 if the quit button has been chose; return 0 otherwise
*/
int interrupts_state_level(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, Mouse *mouse, xpm_image_t img2, xpm_image_t img3, xpm_image_t img4, xpm_image_t img5, bool esc);
/**
* @brief Function that deals with the interrupts notifications when the program is on the maze
* @param msg - Message used for interrupt notification
* @param keyirq - Value used in the interrupt notifications of the keyboard
* @param mouseirq - Value used in the interrupt notifications of the mouse
* @param timerirq - Value used in the interrupt notifications of the timer
* @param menu_struct - Menu struct consisting of all the information needed to update the frame
* @param width - Width of the maze
* @param height - Height of the maze
* @param numbers -Struct that contains all the images (numbers and symbols) needed to show the counter
* @return Return 1 if the player finished the level; return 2 if esc code was pressed; return 0 otherwise
*/
int interrupts_state_maze(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct, uint16_t width, uint16_t height, Numbers *numbers);
/**
* @brief Function that deals with the interrupts notifications when the program is showing a message after a certain level 
* @param msg - Message used for interrupt notification
* @param keyirq - Value used in the interrupt notifications of the keyboard
* @param mouseirq - Value used in the interrupt notifications of the mouse
* @param timerirq - Value used in the interrupt notifications of the timer
* @param menu_struct - Menu struct consisting of all the information needed to update the frame
* @return return 1 after 3 seconds; return 0 otherwise
*/
int interrupts_state_message(message msg, uint8_t keyirq, uint8_t mouseirq, uint8_t timerirq, Menu *menu_struct);

/**@}*/
