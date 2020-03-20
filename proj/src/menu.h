#pragma once

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "i8042.h"
#include "i8254.h"
#include "macros_rtc.h"
#include "video_gr.h"
#include "keyboard.h"
#include "auxiliar_structs.h"
#include "auxiliar_functions.h"
#include "play.h"
#include "maze.h"
#include "player.h"
#include "rtc.h"
#include "mouse.h"
#include "interrupts.h"



/** @defgroup menu  menu
 * @{
 *
 * Basic functions to operate on the menu functionalities
 to operate with collisions of game entities.
 */

/**
* @brief Function that returns the variable level
* @return Returns the variable level
*/
int getLevel();
/**
*@brief Function that allows to change the value of variable level
* @param l - Value to update the variable level
*/

void setLevel(int l);

/**
* @brief Function that leads with all the program, all interrupts, all menus, all mazes and every condition
* @return Returns 0 upon success, not zero otherwise
*/
int menu();


/**@}*/
/*1152x864---screen resolution*/

