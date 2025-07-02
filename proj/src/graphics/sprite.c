/**
 * @file sprite.c
 * @brief Implementation file for sprite management
 */

#include "sprite.h"

/**
 * @brief Creates a sprite
 *
 * This function allocates memory for a sprite object and initializes it
 * with the provided pixmap, position, and speed.
 *
 * @param pic The pixmap for the sprite
 * @param x The initial x position of the sprite
 * @param y The initial y position of the sprite
 * @param xspeed The initial x speed of the sprite
 * @param yspeed The initial y speed of the sprite
 * @return A pointer to the created sprite, or NULL if creation fails
 */

Sprite *create_sprite(const char *pic[], int x, int y,
                      int xspeed, int yspeed) {
  // allocate space for the "object"
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL)
    return NULL;
  // read the sprite pixmap
  sp->map = xpm_load(pic, XPM_8_8_8, &img);
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }
  sp->width = img.width;
  sp->height = img.height;
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  return sp;
}

/**
 * @brief Destroys a sprite
 *
 * This function frees the memory allocated for a sprite, including its pixmap
 * and the sprite structure itself.
 *
 * @param sp The sprite to destroy
 */

void destroy_sprite(Sprite *sp) {
  if (sp == NULL)
    return;
  if (sp->map)
    free(sp->map);
  free(sp);
  sp = NULL; // XXX: pointer is passed by value
  // should do this @ the caller
}

/**
 * @brief Animates a sprite
 *
 * This function updates the sprite's position based on its speed and then
 * draws the sprite at its new position.
 *
 * @param sp The sprite to animate
 * @return Always returns 0
 */

// XXX: move_sprite would be a more appropriate name
int animate_sprite(Sprite *sp) {
  sp->x += sp->xspeed;
  sp->y += sp->yspeed;

  draw_sprite(sp);
  return 0;
}


/**
 * @brief Draws a sprite
 *
 * This function draws the sprite on the screen by copying its pixmap to
 * the drawing buffer.
 *
 * @param sp The sprite to draw
 * @return Always returns 0
 */

/* Some useful non-visible functions */
int draw_sprite(Sprite *sp) {
  char* buffer = get_drawing_buffer();
  unsigned h_res = get_h_res();
  uint32_t color;
  uint32_t desl;
  int color_index;
  int x = sp->x;
  int y = sp->y;
  uint16_t height = sp->height;
  uint16_t width = sp->width;
  unsigned char* map_ptr = sp->map;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      desl = (i * width + j) * 3;
      color = map_ptr[desl] | (map_ptr[desl + 1] << 8) | (map_ptr[desl + 2] << 16);
      if(color != xpm_transparency_color(XPM_8_8_8)){
        color_index = (h_res * (y + i) + (x + j)) * 3;
        buffer[color_index] = color & 0xFF;
        buffer[color_index + 1] = (color >> 8) & 0xFF;
        buffer[color_index + 2] = (color >> 16) & 0xFF;
      }
    }
  }
  return 0;
}

/**
 * @brief Draws a sprite to a specified buffer
 *
 * This function draws the sprite by copying its pixmap to the specified buffer.
 *
 * @param sp The sprite to draw
 * @param buffer The buffer to draw the sprite onto
 * @return Always returns 0
 */

int draw_sprite_to_buffer(Sprite *sp, char* buffer) {
  uint32_t color;
  uint32_t desl;
  int color_index;
  int x = sp->x;
  int y = sp->y;
  uint16_t height = sp->height;
  uint16_t width = sp->width;
  unsigned char* map_ptr = sp->map;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      desl = (i * width + j) * 3;
      color = map_ptr[desl] | (map_ptr[desl + 1] << 8) | (map_ptr[desl + 2] << 16);
      if(color != xpm_transparency_color(XPM_8_8_8)){
        color_index = (width * (y + i) + (x + j)) * 3;
        buffer[color_index] = color & 0xFF;
        buffer[color_index + 1] = (color >> 8) & 0xFF;
        buffer[color_index + 2] = (color >> 16) & 0xFF;
      }
    }
  }
  return 0;
}

