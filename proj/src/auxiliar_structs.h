#pragma once
#include <lcom/lcf.h>
#include <lcom/liblm.h>



/** @defgroup auxiliar_structs  auxiliar_structs
 * @{
 *
 * Auxiliar Structs that represents sprites, data return and enums needed throughout the program  
 */

enum state_game{
  state_menu,
  state_level,
  state_maze,
  state_instructions,
  state_message
};

typedef struct
{
	uint16_t x, y;/**< @brief node position*/ 
	void *parent; /**< @brief pointer to the parent node*/
	char c; /**< @brief character to be displayed; if it is #, it's a wall */  
	char directions; /**< @brief Directions that still haven't been explored*/
} Node;

typedef struct 
{
	uint16_t x, y; /**< @brief Coordenates x and y of the wall*/
} Wall;

typedef struct {
	uint16_t x, y; /**< @brief Coordenates x and y of the wall*/
} Blank_space;


typedef struct {

  int x, y; /**< @brief Coordenates x and y of the mouse*/ 
  xpm_image_t img; /**< @brief Image of the mouse*/

} Mouse;


enum type{
  initial_stat,
  play_stat,
  quit_stat,
  level_stat,
  instructions_stat,
  instructions_button_stat,
  message_completed_stat,
  message_failed_stat,
  message_all_passed
};


typedef struct {

  uint16_t x, y;/**< @brief X and Y coordinates where the drawing of the menus starts*/
  xpm_image_t img_initial;/**< @brief Image of the initial menu*/
  xpm_image_t img_play;/**< @brief Image of the play menu*/
  xpm_image_t img_quit;/**< @brief Image of the quit menu*/
  xpm_image_t img_level;/**<@brief Image of the levels menu*/
  xpm_image_t img_instructions; /**<@brief Image of the instructions*/
  xpm_image_t img_instructions_button; /**<@brief Image of the instructions*/
  xpm_image_t img_message_complete; /**<@brief Image with the message "Level Completed"*/
  xpm_image_t img_message_failed; /**<@brief Image with the message "Level Failed"*/
  xpm_image_t img_all_passed; /**<@brief Image with the message "All Levels Completed"*/
  enum type menu_type;/**< @brief State of the menu "choice" */

} Menu;


enum state_t{
  SOMETHING_ELSE,
  BUTTON_RELEASED
};

struct Screen_pos{
  double xmin;/**< @brief Minimum x coordinate of the menu*/
  double xmax;/**< @brief Maximum x coordinate of the menu*/
  double ymin;/**< @brief Minimum y coordinate of the menu*/
  double ymax;/**< @brief Maximum y coordinate of the menu*/
};

typedef struct {

  uint16_t x, y, speed;/**< @brief Coordinates x and y of the ball and it's speed */
  xpm_image_t img;/**< @brief Image of the ball */ 

} Ball;

typedef struct {

  uint16_t x, y;/**< @brief Coordinates x and y of the star  */
  xpm_image_t img;/**< @brief Image of the star*/
  bool st_catch;/**< @brief Bool that is true if this star has been catched, false otherwise */

} Star;

typedef struct {

  xpm_image_t zero, one, two, three, four, five, six, seven, eight, nine, colon, slash;/**< @brief image of the respective number/symbol*/

} Numbers;

typedef struct {
  int m1, m2, s1, s2; /**<@brief current values of the counter (m1m2:s1s2)*/
  int w1, w2, w3, w4; /**<@brief widths of the current numbers of the counter*/
  xpm_image_t m1_xpm, m2_xpm, s1_xpm, s2_xpm; /**<@brief images of the current number of the counters*/

} Time;

typedef struct {

  int second, minute, hour, day, month, year; /**<@brief values of the current time*/
  int wt; /**<@brief total width of the clock on the screen*/

} RealTime;

/**@}*/

