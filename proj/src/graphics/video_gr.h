#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdlib.h>



void *(vg_init) (uint16_t mode);

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int(vg_draw_rectangle)(uint16_t x, uint16_t y, u_int16_t width, u_int16_t height, uint32_t color);

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step, uint16_t mode);

int(vg_draw_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y);

int(vg_draw_character)(xpm_map_t xpm, uint16_t x, uint16_t y, uint16_t xpm_desl,
uint16_t character_width, uint16_t character_height);

int(vg_move_pixmap)(xpm_map_t xpm, uint16_t *current_x, uint16_t *current_y, uint16_t xf, uint16_t yf, int16_t speed,bool draw);

int vg_flip_buffers();

void vg_clear_buffer(char* buffer);

char* get_first_buffer();
char* get_second_buffer();
char* get_drawing_buffer();
char* get_arena_buffer();
unsigned get_h_res();

#endif
