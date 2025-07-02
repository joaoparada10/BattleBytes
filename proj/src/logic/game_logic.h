#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../model/arena.h"
#include "../view/constants.h"
#include "../graphics/sprite.h"
#include "../model/game_model.h"
#include "../view/game_view.h"

bool sprite_collision(Sprite *sp1, Sprite *sp2);

bool spawn_collision(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

bool move_collision(Sprite *sprite, int x_move, int y_move);

int(move_sprite_to)(Sprite *sp, uint16_t xf, uint16_t yf,bool collision);

void handle_game_keyboard(uint8_t* bytes, uint8_t size);

int move_cursor(int16_t delta_x,int16_t delta_y);

void calculate_tank_direction();

void shoot();

#endif
