/**
 * @file game_model.c
 * @brief Implementation of the game model.
 */

#include "game_model.h"

/// @brief Pointer to the tank game unit.
static GameUnit *tank;

/// @brief Pointer to the linked list of enemies.
static Enemy *enemy_list = NULL;

/**
 * @brief Creates the game elements.
 *
 * @return 0 on success, -1 on failure.
 */

int create_game_elements() {
  Sprite *tank_sprite = get_tank_sprite();
  tank = create_static_game_element(5, tank_sprite, DIRECTION_3);
  return 0;
}

/**
 * @brief Creates a static game element.
 *
 * @param hp The health points of the element.
 * @param sp Pointer to the sprite of the element.
 * @param direction The direction of the element.
 * @return Pointer to the newly created game unit.
 */

GameUnit *create_static_game_element(uint16_t hp, Sprite *sp, Direction direction) {
  GameUnit *element = malloc(sizeof(GameUnit));
  element->hp = hp;
  element->sprite.sp = sp;
  element->direction = direction;
  element->type = STATIC_SPRITE;
  return element;
}

/**
 * @brief Creates an animated game element.
 *
 * @param hp The health points of the element.
 * @param asp Pointer to the animated sprite of the element.
 * @param direction The direction of the element.
 * @return Pointer to the newly created game unit.
 */

GameUnit *create_animated_game_element(uint16_t hp, AnimSprite *asp, Direction direction) {
  GameUnit *element = malloc(sizeof(GameUnit));
  element->hp = hp;
  element->sprite.asp = asp;
  element->direction = direction;
  element->type = ANIMATED_SPRITE;
  return element;
}

/**
 * @brief Creates an enemy.
 *
 * @param sprite Pointer to the sprite or animated sprite of the enemy.
 * @param direction The direction of the enemy.
 * @param x The X coordinate of the enemy.
 * @param y The Y coordinate of the enemy.
 * @param enemy_type The type of the enemy.
 * @return Pointer to the newly created enemy.
 */

Enemy *create_enemy(void *sprite, Direction direction, int x, int y, EnemyType enemy_type) {
  GameUnit *enemy_model;
  uint16_t hp;

  if (enemy_type == VIRUS1) {
    hp = 1;
    enemy_model = create_static_game_element(hp, (Sprite *) sprite, direction);
    enemy_model->sprite.sp->x = x;
    enemy_model->sprite.sp->y = y;
  }
  else if (enemy_type == VIRUS2) {
    hp = 2;
    enemy_model = create_animated_game_element(hp, (AnimSprite *) sprite, direction);
    AnimSprite *enemy_asprite = enemy_model->sprite.asp;
    enemy_asprite->sp->x = x;
    enemy_asprite->sp->y = y;
  }
  else {
    return NULL;
  }

  Enemy *new_enemy = malloc(sizeof(Enemy));
  new_enemy->model = enemy_model;
  new_enemy->enemy_type = enemy_type;
  new_enemy->next = enemy_list;
  enemy_list = new_enemy;

  return new_enemy;
}

/**
 * @brief Destroys a game element.
 *
 * @param element Pointer to the game element to be destroyed.
 * @param type The type of the sprite.
 */

void destroy_game_element(GameUnit *element, SpriteType type) {
  if (type == STATIC_SPRITE)
    destroy_sprite(element->sprite.sp);
  else if (type == ANIMATED_SPRITE)
    destroy_asprite(element->sprite.asp);
  free(element);
}

/**
 * @brief Destroys an enemy.
 *
 * @param enemy Pointer to the enemy to be destroyed.
 */

void destroy_enemy(Enemy *enemy) {
  Enemy **current = &enemy_list;
  while (*current != NULL) {
    if (*current == enemy) {
      Enemy *to_free = *current;
      *current = (*current)->next;
      destroy_game_element(to_free->model, to_free->model->type);
      free(to_free);
      return;
    }
    current = &((*current)->next);
  }
}

/**
 * @brief Frees all enemies.
 */

void free_enemies() {
  Enemy *current = enemy_list;
  Enemy *next;

  while (current != NULL) {
    next = current->next;
    destroy_game_element(current->model, current->model->type);
    free(current);
    current = next;
  }

  // Set the enemy_list to NULL after freeing all enemies
  enemy_list = NULL;
}

/**
 * @brief Cleans up game elements.
 */

void cleanup_elements() {
  free_enemies();
  free(tank);
}

/**
 * @brief Spawns an enemy of the given type.
 *
 * @param enemy_type The type of enemy to spawn.
 */

void spawn_enemy(EnemyType enemy_type) {
  int x, y;
  do {
    x = rand() % ARENA_WIDTH;
    y = (rand() % ARENA_HEIGHT);
  } while (spawn_collision(x, y, VIRUS1_WIDTH, VIRUS1_HEIGHT));

  if (enemy_type == VIRUS1) {
    Sprite *new_sprite = create_virus1_sprite(x, y);
    create_enemy(new_sprite, DIRECTION_1, x, y, enemy_type);
  }
  if (enemy_type == VIRUS2) {
    AnimSprite *new_asprite = create_virus2_asprite(x, y);
    create_enemy(new_asprite, DIRECTION_1, x, y, enemy_type);
  }

}

/**
 * @brief Spawns a wave of enemies.
 */

void spawn_enemy_wave() {
  uint8_t difficulty = get_difficulty();
  for (int i = 0; i < difficulty; i++) {
    spawn_enemy(VIRUS1);
  }
  for (int i = 0; i < difficulty / 2; i++) {
    spawn_enemy(VIRUS2);
  }
}

/**
 * @brief Updates the enemies.
 */

void update_enemies() {
  Enemy *current = enemy_list;
  uint16_t tank_x = tank->sprite.sp->x;
  uint16_t tank_y = tank->sprite.sp->y;
  Sprite *current_sprite;
  while (current != NULL) {
    if (current->model->type == STATIC_SPRITE) {
      current_sprite = current->model->sprite.sp;
    }
    else {
      current_sprite = current->model->sprite.asp->sp;
    }

    move_sprite_to(current_sprite, tank_x, tank_y, true);
    if (sprite_collision(current_sprite, tank->sprite.sp)) {
      current->model->hp--;
      if (current->model->hp <= 0) {
        Enemy *to_destroy = current;
        current = current->next;
        destroy_enemy(to_destroy);
      }
      tank->hp--;
      if (tank->hp <= 0) {
        set_state(GAME_END);
      }
    }
    else {
      current = current->next;
    }
  }
}

/**
 * @brief Gets the tank model.
 *
 * @return Pointer to the tank model.
 */

GameUnit *get_tank_model() {
  return tank;
}

/**
 * @brief Gets the list of enemies.
 *
 * @return Pointer to the list of enemies.
 */

Enemy *get_enemy_list() {
  return enemy_list;
}
