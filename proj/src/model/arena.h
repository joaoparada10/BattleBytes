#ifndef ARENA_H
#define ARENA_H

#include "../view/constants.h"
#include "../graphics/video_gr.h"
#include "../graphics/sprite.h"

typedef struct {
  int grid[ARENA_HEIGHT][ARENA_WIDTH];
  uint32_t ground_color;

} Arena;

Arena* create_arena(xpm_map_t xpm, uint32_t ground_color);

bool is_walkable(int x, int y);

bool arena_collision(Sprite *sprite);

void destroy_arena(Arena *arena);

Arena* get_current_arena();

#endif
