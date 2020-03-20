
#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#pragma once

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "i8254.h"

/** @defgroup keyboard  keyboard
 * @{
 *
 * Functions to interact with the i8042 KBC
 */

/**
* @brief Functions that returns the variable scancode
* @return Return the variable scancode
*/
uint8_t getScancode();

/**
* @brief Function that invokes sys_inb() system call but reads the value into a uint8_t variable
* @param port - The input port that is to be read
* @param value - Address of 8-bit variable to be update with the value read
* @return Return 0 upon success and non-zero otherwise
*/
int (kbd_util_sys_inb)(int port, uint8_t *value);
/**
* @brief Function that subscribe keyboard interrupts
* @param bit_no - Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt  
* @return Return 0 upon success and non zero otherwise
*/
int (kbd_subscribe_int)(uint8_t *bit_no);
/**
* @brief Function that unsubscribes keyboard interrupts
* @return Return 0 upon success and non zero otherwise
*/
int (kbd_unsubscribe_int)();
/**
* @brief Function that handles keyboard interrupts
*/
void (kbc_ih)(void);
/**
* @brief Function that reads the content in th output buffer, checking for possible erros 
* @param aux - Adress of memory where it will be written what is read from the output buffer
*/
void (kbd_read)(uint8_t *aux);
/**
* @brief Function that writes a command or a argument to the keyboard controller, cheking for possible errors
* @param port - It could be an argument or a command  
* @param cmd - Contains command or argument code
*/
void (kbd_write)(int port, uint8_t cmd);
/**
* @brief Function that enable interrupts
* @return Return 0 upon success and non zero otherwise
*/
int (enable_interrupts)();


/**@}*/

#endif /* _LCOM_KEYBOARD_H */
