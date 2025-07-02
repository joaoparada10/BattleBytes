/**
 * @file arena.c
 * @brief Implementation of the game arena.
 */

#include "arena.h"

/// @brief Pointer to the current arena.
static Arena *current_arena;

/**
 * @brief Creates a new arena.
 *
 * @param xpm The XPM map representing the arena layout.
 * @param ground_color The color representing the walkable ground in the arena.
 * @return Pointer to the newly created arena, or NULL if memory allocation fails.
 */

Arena *create_arena(xpm_map_t xpm, uint32_t ground_color) {
  Arena *arena = (Arena *) malloc(sizeof(Arena));
  if (arena == NULL) {
    return NULL;
  }

  arena->ground_color = ground_color;

  for (int i = 0; i < ARENA_HEIGHT; i++) {
    for (int j = 0; j < ARENA_WIDTH; j++) {
      arena->grid[i][j] = 0; // 0 for walkable, 1 for obstacle
    }
  }

  xpm_image_t img;
  unsigned char *map;
  map = xpm_load(xpm, XPM_8_8_8, &img);
  char *buffer = get_arena_buffer();
  uint32_t color;
  uint32_t desl;
  int color_index;
  uint16_t height = img.height;
  uint16_t width = img.width;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      desl = (i * width + j) * 3;
      color = map[desl] | (map[desl + 1] << 8) | (map[desl + 2] << 16);
      if (color != ground_color) {
        arena->grid[i][j] = 1;  // obstacle found, mark it
      }
      if (color != xpm_transparency_color(XPM_8_8_8)) {
        color_index = (H_RES * (HEADER_HEIGHT + i) + j) * 3;
        buffer[color_index] = color & 0xFF;
        buffer[color_index + 1] = (color >> 8) & 0xFF;
        buffer[color_index + 2] = (color >> 16) & 0xFF;
      }
    }
  }
  free(map);
  current_arena = arena;
  return arena;
}

/**
 * @brief Checks if a given coordinate in the arena is walkable.
 *
 * @param x The X coordinate.
 * @param y The Y coordinate.
 * @return True if the coordinate is walkable, false otherwise.
 */

bool is_walkable(int x, int y) {
  Arena* arena = get_current_arena();
  if (x <= 0 || x >= ARENA_WIDTH || y <= HEADER_HEIGHT || y >= (ARENA_HEIGHT + FOOTER_HEIGHT)) {
    return false; // Out of bounds
  }
  return !arena->grid[y][x];
}

/**
 * @brief Checks if a given sprite collides with any obstacles in the arena.
 *
 * @param sprite Pointer to the sprite to check for collisions.
 * @return True if the sprite collides with an obstacle, false otherwise.
 */

bool arena_collision(Sprite *sprite) {
  uint16_t x = sprite->x;
  uint16_t y = sprite->y - HEADER_HEIGHT;
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      if (!is_walkable(x + j, y + i)) {
        return true; // Collision detected
      }
    }
  }
  return false; // No collision
}

/**
 * @brief Destroys the given arena and frees allocated memory.
 *
 * @param arena Pointer to the arena to be destroyed.
 */

void destroy_arena(Arena *arena) {
  if (arena) {
    free(arena);
  }
}

/**
 * @brief Gets the current arena.
 *
 * @return Pointer to the current arena.
 */

Arena *get_current_arena() {
  return current_arena;
}
