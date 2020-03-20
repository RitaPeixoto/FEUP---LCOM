#pragma once


#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "auxiliar_structs.h"
#include "video_gr.h"

/** @defgroup maze  maze
 * @{
 *
 * Functions related to the creation of the maze
 */
/**
* @brief Function that returns the variable walls
* @return Returns the variable walls
*/
Wall* getwalls();
/**
* @brief Function that return the variable blank_spaces
* @return Returns the variable blank_spaces
*/
Blank_space* getBlankSpace();
/**
* @brief Function that returns the variable number of blank spaces
* @return Returns the variable number_of_blank_spaces
*/
int getNumberOfBlankSpaces();
/**
* @brief Function that decrements the number of blank spaces 
*/
void decrementBlankSpaces();
/**
* @brief Function that initializes the maze where each cell starts with four walls
* @param width - Width of the maze
* @param height - Height of the maze
* @return Return 0 upon success, not zero otherwise
*/
int (init)(uint16_t width, uint16_t height);
/**
* @brief Function that tries to connect the node to a random neighbor
* @param n - Current node
* @param width - Width of the maze 
* @param height - Height of the maze 
* @return Returns the adress of the next node that should be visited
*/
Node *Link(Node *n, uint16_t width, uint16_t height);
/**
* @brief Function that draws the generated maze 
* @param width - Width of the maze
* @param height - Height of the maze
*/
void draw_maze(uint16_t width, uint16_t height);
/**
* @brief Function that creates the maze 
* @param width - Width of the maze
* @param height - Height of the maze
* @return Returns 0 upon success, not zero otherwise
*/
int create_maze(uint16_t width, uint16_t height);


/**@}*/
