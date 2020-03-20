#pragma once

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <stdio.h> 
#include <stdlib.h> 
#include "b.xpm"
#include "star.xpm"
#include "star3.xpm"
#include "star2.xpm"
#include "auxiliar_structs.h"
#include "video_gr.h"
#include "menu.h"

/** @defgroup player  player
 * @{
 *
 * Functions that create all the objects of the game and deal with the movements of the player   
 */

/**
* @brief Function that creates the player and draws it
* @param width - width of the maze
* @param height - height of the maze
*/
void create_player(uint16_t width, uint16_t height);
/**
* @brief Function that draws the player
* @return Returns 0 upon success, not zero otherwise
*/
int draw_player();
/**
* @brief Function that draws the stars in the maze
* @param n - number of stars to be drawn 
* @param width - width of the maze
* @param height -height of the maze
* @return Returns 0 upon success, not zero otherwise
*/
int draw_star(int n, uint16_t width, uint16_t height);
/**
* @brief Function that randomly generates the coordinates where the new star will be drawn
* @param x - variable where the x coordinate generated will be saved 
* @param y - variable where the y coordinate generated will be saved 
* @return Returns 0 upon success, not zero otherwise
*/
int generate_star_coordinates(uint16_t *x , uint16_t *y);
/**
* @brief Functions that checks if the movement is possible and deals with all the different situations. It also checks if the player is exiting the maze and if all the conditions to pass the level where fulffilled
* @param bytes - 2-byte array
* @param make - bool that is true if it is a make code, false if it is a break code
* @param width - width of the maze
* @param height - height of the maze 
* @return Returns 1 if the movement  is to the  exit of the maze and it passed the level, 0 othwerwise
*/
int check_movement(uint8_t bytes[2], bool make, uint16_t width, uint16_t height);
/**
* @brief Function that checks if the movement of the player leeds to a colision with a wall
* @param up - True if the up arrow has been pressed, false otherwise
* @param down - True if the down arrow has been pressed, false otherwise
* @param left - True if the left arrow has been pressed, false otherwise
* @param right - True if the right arrow has been pressed, false otherwise
* @param x - x coordinate of the player already updated with the movement
* @param y - y coordinate of the player already updated with the movement
* @param width - width of the maze
* @param height - height of the maze
* @return Returns true if it does not colides with anything, false otherwise
*/
bool check_colisions(bool up, bool down, bool left, bool right, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
/**
* @brief Function that updates the struct Star sent has parameter to the catched state, updating the star bar to another catched star
* @param s - Struct Star to be modified to the catched state
*/
void catch_star(Star *s);
/**
* @brief Function that checks if with the player movement any star is being catched 
* @param up - True if the up arrow has been pressed, false otherwise
* @param down - True if the down arrow has been pressed, false otherwise
* @param left - True if the left arrow has been pressed, false otherwise
* @param right - True if the right arrow has been pressed, false otherwise
* @param x - x coordinate of the player already updated with the movement
* @param y - y coordinate of the player already updated with the movement
* @param width - width of the maze
* @param height - height of the maze
*/
void check_catch_star(bool up, bool down, bool left, bool right, uint16_t x, uint16_t y, uint16_t width, uint8_t height);
/**
* @brief Function that checks if all the requirements to pass the level were fulfilled
* @return Returns true if all the requirements to pass the level were fulfilled , false otherwise
*/
bool exit_maze();

/**@}*/
