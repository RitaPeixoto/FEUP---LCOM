#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "video_gr_macros.h"
#include <machine/int86.h>
#include <math.h>

int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *v);
void *(vg_init)(uint16_t mode);
int (change_pixel_color)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len,uint32_t color);
int (vg_draw_retangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);
uint32_t getColorFirst(char c, uint32_t first, uint32_t mask);
uint32_t getColor(uint16_t mode, uint32_t first, uint8_t row, uint8_t col, uint8_t step, uint8_t no_rectangles);
int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img);
int clean_screen(xpm_image_t img, uint16_t x, uint16_t y);
