#ifndef _GAME_MODEL_H_
#define _GAME_MODEL_H_

#include "arena.h"
#include "../graphics/sprite.h"
#include "../view/game_view.h"
#include "../view/constants.h"
#include "../dispatcher/state.h"
#include "../logic/direction.h"
#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>

typedef enum {
  STATIC_SPRITE,
  ANIMATED_SPRITE
} SpriteType;

typedef struct {
  uint16_t hp;
  SpriteType type;
  union{
    Sprite *sp;
    AnimSprite *asp;
  } sprite;
  Direction direction;
} GameUnit;

typedef enum {
  VIRUS1,
  VIRUS2
} EnemyType;

typedef struct Enemy {
  GameUnit *model;
  EnemyType enemy_type;
  struct Enemy *next; // storing references to each enemy dynamically, so we can spawn/destroy enemies at runtime
} Enemy;

int create_game_elements();

GameUnit* create_static_game_element(uint16_t hp, Sprite *sp, Direction direction);

GameUnit* create_animated_game_element(uint16_t hp, AnimSprite *asp, Direction direction);

Enemy* create_enemy(void* sp, Direction direction, int x, int y, EnemyType enemy_type);

void destroy_game_element(GameUnit *element,SpriteType type);

void destroy_enemy(Enemy *enemy);

void free_enemies();

void cleanup_elements();

void spawn_enemy(EnemyType new_enemy);

void spawn_enemy_wave();

void update_enemies();

void calculate_tank_direction();

void shoot();

GameUnit* get_tank_model();

Enemy* get_enemy_list();

#endif

