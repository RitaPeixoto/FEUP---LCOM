#pragma once


#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <machine/int86.h>
#include <math.h>
#include "video_gr_macros.h"


/** @defgroup video_gr  video_gr
 * @{
 *
 * Basic functions to work with the VBE in graphics mode
 */


/**
* @brief Function that initializes packed vbe_mode__info_t structure passed as an address with the VBE information on the input mode
* @param mode - Mode whose information should be returned 
* @param v - Address of vbe_mode_info_t structure to be initialized 
* @return Return 0 upon success, not zero otherwise
*/
int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *v);
/**
* @brief Function that return the vertical resolution of the screen
* @returns Return the vertical resolution of the screen
*/
uint16_t getvresolution();
/**
* @brief Function that return the horizontal resolution of the screen
* @returns Return the horizontal resolution of the screen
*/
uint16_t gethresolution();
/**
* @brief Function that initializes the video module in graphics mode
* @param mode - 16-bit VBE mode to set 
*/
void *(vg_init)(uint16_t mode);
/**
* @brief Functiont that draws a horizontal line with the specified length and color, starting at the specified coordinates
* @param x - Horizontal coordinate of the initial point 
* @param y - Vertical coordinate of the initial point 
* @param len - Line's length in pixels 
* @param color - Color to set the pixel 
* @return Return 0 upon success, not zero otherwise
*/
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len,uint32_t color);
/**
* @brief Function that draws a filled rectangle with the specified width, height and color, starting at the specified coordinates
* @param x - Horizontal coordinate of the initial point 
* @param y - Vertical coordinate of the initial point 
* @param width - Rectangle's width in pixels 
* @param height - Rectangle's height in pixels 
* @param color - Color to set the pixel 
* @return Return 0 upon success, not zero otherwise
*/
int (vg_draw_retangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);

/**
* @brief Function that draws a xpm starting on the given x and y coordinates
* @param x - Horizontal coordinate of the position of the xpm 
* @param y - Vertical coordinate of the position of the xpm 
* @param img - Image of the xpm
* @return Returns 0 upon success, not zero otherwise
*/
int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img);
/**
* @brief Function that cleans a certain xpm
* @param img - Image of the xpm to clean
* @param x - Horizontal coordinate of the position of the xpm 
* @param y - Vertical coordinate of the position of the xpm 
* @return Returns 0 upon success, not zero otherwise
*/
int clean_xpm(xpm_image_t img, uint16_t x, uint16_t y);
/**
* @brief Function that cleans the screen
*/
void clean_screen();
/**
* @brief Function that informs if the color of the pixel is the color given 
* @param x - Horizontal coordinate of the pixel 
* @param y - Vertical coordinate of the pixel
* @param color - Color to compare
* @return Return true if it the color, false otherwise
*/
bool (color_of_pixel)(uint16_t x, uint16_t y, uint32_t color);
/**
* @brief Function that changes the color of a pixel
* @param x - Horizontal coordinate of the pixel 
* @param y - Vertical coordinate of the pixel
* @param color - Color to set the pixel
* @return Returns 0 upon success, not zero otherwise
*/
int (change_pixel_color_32)(uint16_t x, uint16_t y, uint32_t color);
/**
* @brief Function that changes the color of a pixel
* @param x - Horizontal coordinate of the pixel 
* @param y - Vertical coordinate of the pixel
* @param color - Pointer to a color to set the pixel
* @return Returns 0 upon success, not zero otherwise
*/
int (change_pixel_color)(uint16_t x, uint16_t y, uint8_t *color);

/**
* @brief Function that updates VRAM with the information saved on the auxiliar variable, used to implement double buffering
*/
void(updateBuffer)(void);

/**@}*/
