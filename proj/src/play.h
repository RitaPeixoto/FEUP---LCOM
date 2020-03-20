#pragma once

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "video_gr.h"
#include "maze.h"
#include "keyboard.h"
#include "player.h"



/** @defgroup play  play
 * @{
 *
 * Function that creates the maze and deals with all needed in the game part
 */

/**
 * @brief Function that creates the maze and all its objects
 * @brief width - width of the maze 
 * @brief height - height of the maze
 * @return Returns 0 upon success, non-zero otherwise
 */
void create_game(uint16_t width, uint16_t height);

/**
* @brief Function that deals with the game
* @brief width - width of the maze 
* @brief height - height of the maze
* @return Returns 0 upon success, non-zero otherwise
*/
int play(uint16_t width, uint16_t height);

/**@}*/
