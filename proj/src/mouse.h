#pragma once
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "i8254.h"
#include "auxiliar_structs.h"
#include "auxiliar_functions.h"
#include "rtc.h"

/** @defgroup mouse  mouse
 * @{
 *
 * Functions to interact with the mouse
 */

/**
* @brief Function that returns the variable content
* @return Returns the variable content
*/
uint8_t getContent();

/**
 * @brief Function that subscribes interrupts and enables data report
 * @param bit_no - Address of memory to be initialized with the mouse peripheral interface controller bit number to be set in the mask returned upon an interrupt
 * @return Returns 0 upon success, not zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function that unsubscribes interrupts and disables data report.
 * @return Returns 0 upon success, not zero otherwise
 */
int (mouse_unsubscribe_int)();
/**
 * @brief Function that reads the content in th output buffer, checking for possible erros 
 * @param aux - Adress of memory where it will be written what is read from the output buffer
 *
*/
void (read_command)(uint8_t *aux);
/**
* @brief Function that writes byte to mouse
* @param command - Command that will be written to the mouse
* @return Returns 0 upon success, not zero otherwise 
*/
int (write_command)(uint8_t command);
/**
* @brief Function that writes command to a specified port of the kbc
* @param port - Adress of the port where the command will be written 
* @param cmd - Command that will be written to the KBC
* @return Returns 0 upon success, not zero otherwise 
*/
int (write_kbc_byte)(int port, uint8_t cmd);
/**
* @brief Function that receives a packet and parses it 
* @param p - Address of the struct where it will be saved the packet information
* @param pack - 3 bytes array containing the mouse information 
*/
void (mouse_packet_parse)(struct packet *p, uint8_t pack[3]);
/**
* @brief Function that detects mouse events relevant to the program
* @param p - Address of the struct containing the mouse information
* @return Return the address of the mouse event detected 
*/
struct mouse_ev* (detect_event_mouse)(struct packet *p);
/**
* @brief Function that handles the mouse interrupt
*/
void (mouse_ih)(void);
/**
* @brief Function that deals with the mouse event, checking if any button has been pressed and released or if it's a normal mouse event
* @param menu - Menu struct consisting of all the information needed to update the frame 
* @param p - Address of the struct of the mouse packet 
* @param mouse_x - Updated x coordinate of the position mouse
* @param mouse_y - Updated y coordinate of the position mouse
* @param realtime - Struct that contains all the values needed to show the real time clock
* @param numbers_rtc - Struct that contains all the images needed to show real time clock 
* @return Return 0 if a button has not been chosen, an integer (1,2) that is the menu choice otherwise
*/
int(mouse_gestures)(Menu *menu, struct packet *p, int mouse_x, int mouse_y, RealTime *realtime, Numbers* numbers_rtc);
/**
* @brief Function that deals with the mouse event, checking if any button has been pressed and released or if it's a normal mouse event in the levels menu
* @param menu -  Menu struct consisting of all the information needed to update the frame
* @param p - Address of the struct of the mouse packet 
* @param mouse_x - Updated x coordinate of the position mouse
* @param mouse_y- Updated y coordinate of the position mouse
* @param level - current level of the player
* @return Return 0 if a button has not been chosen, an integer (1,2) that is the menu choice otherwise
*/
int (mouse_button)(Menu *menu, struct packet *p, int mouse_x, int mouse_y,int level);

/**
* @brief Function that deals with the mouse event, checking if any button has been pressed and released or if it's a normal mouse event
* @param menu - Menu struct consisting of all the information needed to update the frame 
* @param p - Address of the struct of the mouse packet 
* @param mouse_x - Updated x coordinate of the position mouse
* @param mouse_y - Updated y coordinate of the position mouse
* @return Return 1 if the button to return to the main menu has been chosen, 0 otherwise
*/
int (mouse_instructions)(Menu *menu, struct packet *p, int mouse_x, int mouse_y);

/**@}*/
