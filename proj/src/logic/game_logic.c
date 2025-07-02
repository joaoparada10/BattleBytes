/**
 * @file game_logic.c
 * @brief Contains functions related to game logic and collision detection.
 */

#include "game_logic.h"

/**
 * @brief Checks for collision between two sprites.
 * 
 * @param sp1 Pointer to the first sprite.
 * @param sp2 Pointer to the second sprite.
 * @return true if collision occurs, false otherwise.
 */
// check doc/collision-algorithm-visualisation.jpg
bool sprite_collision(Sprite *sp1, Sprite *sp2) {
  if (sp1->x < sp2->x + sp2->width && sp1->x + sp1->width > sp2->x &&
      sp1->y < sp2->y + sp2->height && sp1->y + sp1->height > sp2->y) {
    return true;
  }
  return false;
}

/**
 * @brief Checks for collision when spawning an object.
 * 
 * @param x X-coordinate of the object.
 * @param y Y-coordinate of the object.
 * @param width Width of the object.
 * @param height Height of the object.
 * @return true if collision occurs, false otherwise.
 */
bool spawn_collision(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  Sprite* tank_sprite = get_tank_sprite();
  if (x < tank_sprite->x + tank_sprite->width && x + width + SPAWN_OFFSET > tank_sprite->x && // adding offset to make the game more fair
        y < tank_sprite->y + tank_sprite->height && y + height + SPAWN_OFFSET > tank_sprite->y) {
    return true;
  }
  y -= HEADER_HEIGHT;
  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      if (!is_walkable(x + j, y + i)) {
        return true; // Collision detected
      }
    }
  }

  return false;
}

/**
 * @brief Checks for collision when moving a sprite.
 * 
 * @param sprite Pointer to the sprite.
 * @param x_move Horizontal movement amount.
 * @param y_move Vertical movement amount.
 * @return true if collision occurs, false otherwise.
 */
bool move_collision(Sprite *sprite, int x_move, int y_move) {
  int x = sprite->x;
  int y = sprite->y - HEADER_HEIGHT;
  int new_x = x + x_move;
  int new_y = y + y_move;
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      if (!is_walkable(new_x + j, new_y + i)) {
        return true; // Collision detected
      }
    }
  }
  return false; // No collision
}

/**
 * @brief Moves a sprite to the specified coordinates while handling collision.
 * 
 * @param sp Pointer to the sprite.
 * @param xf Final X-coordinate.
 * @param yf Final Y-coordinate.
 * @param collision Flag indicating whether collision should be checked.
 * @return 0 on success.
 */
int(move_sprite_to)(Sprite *sp, uint16_t xf, uint16_t yf, bool collision) {
  int16_t x_distance = xf - sp->x;
  int16_t y_distance = yf - sp->y;
  int16_t x_move = 0;
  int16_t y_move = 0;
  if (x_distance != 0) {
    if (abs(x_distance) <= sp->xspeed) {
      x_move = x_distance;
    }
    else {
      if (x_distance < 0)
        x_move = -sp->xspeed;
      else
        x_move = sp->xspeed;
    }
  }
  if (y_distance != 0) {
    if (abs(y_distance) <= sp->yspeed) {
      y_move = y_distance;
    }
    else {
      if (y_distance < 0)
        y_move = -sp->yspeed;
      else
        y_move = sp->yspeed;
    }
  }
  if (x_move != 0 || y_move != 0) {
    if (collision) {
      if (!move_collision(sp, x_move, y_move)) {
        sp->x += x_move;
        sp->y += y_move;
      }
      else if(!move_collision(sp,x_move,0)) sp->x += x_move;
      else if(!move_collision(sp,0,y_move)) sp->y += y_move;
    }
    else{
      sp->x += x_move;
      sp->y += y_move;
    }
  }
  return 0;
}

/**
 * @brief Handles keyboard input for game controls.
 * 
 * @param bytes Array of keyboard input bytes.
 * @param size Size of the input array.
 */
void handle_game_keyboard(uint8_t* bytes, uint8_t size) {
  GameUnit* tank = get_tank_model();
  uint8_t code;
  int x = 0;
  int y = 0;
  if (size == 2) code = bytes[1];
  else code = bytes[0];
  switch(code){
    case W_MAKE_CODE:
      y = -10;
      break;
      case S_MAKE_CODE:
      y = 10;
      break;
      case A_MAKE_CODE:
      x = -10;
      break;
      case D_MAKE_CODE:
      x = 10;
      break;
      case SPACEBAR_BREAK_CODE:
      set_state(LOADING_PAUSE);
      break;
  }
  if (!move_collision(tank->sprite.sp, x, y)) {
    tank->sprite.sp->x += x;
    tank->sprite.sp->y += y;
    calculate_tank_direction();
  }
}

/**
 * @brief Moves the cursor on the screen.
 * 
 * @param delta_x Change in X-coordinate.
 * @param delta_y Change in Y-coordinate.
 * @return 0 on success.
 */
int move_cursor(int16_t delta_x, int16_t delta_y) {
  Sprite *cursor = get_cursor();
  if (delta_x >= 0)
    if (cursor->x + cursor->width + delta_x >= ARENA_WIDTH)
      cursor->x = 799 - cursor->width;
    else
      cursor->x += delta_x;
  else if (cursor->x + delta_x < 0)
    cursor->x = 0;
  else
    cursor->x += delta_x;
  if (delta_y >= 0)
    if (cursor->y + cursor->height + delta_y >= 580)
      cursor->y = 579 - cursor->height;
    else
      cursor->y += delta_y;
  else if (cursor->y + delta_y < 20)
    cursor->y = 20;
  else
    cursor->y += delta_y;
  return 0;
}

/**
 * @brief Calculates the direction of the tank based on the position of the crosshair.
 */
void calculate_tank_direction() {
  static double prev_angle_deg = -1;
  Sprite *crosshair = get_crosshair();
  GameUnit *tank = get_tank_model();
  int crosshair_x = crosshair->x + (crosshair->width / 2);
  int crosshair_y = crosshair->y + (crosshair->height / 2);
  int tank_x = tank->sprite.sp->x + (tank->sprite.sp->width / 2);
  int tank_y = tank->sprite.sp->y + (tank->sprite.sp->height / 2);
  int x_diff = crosshair_x - tank_x;
  int y_diff = crosshair_y - tank_y;
  double angle_rad = atan2(y_diff, x_diff);
  double angle_deg = angle_rad * (180.0 / M_PI);
  if (angle_deg < 0) {
    angle_deg += 360.0;
  }
  angle_deg += 75.0; // shift to match the directions of the xpms...
  if (angle_deg >= 360.0) {
    angle_deg -= 360.0;
  }
  if (prev_angle_deg < 0 || fabs(angle_deg - prev_angle_deg) > HYSTERESIS_THRESHOLD) {
        uint8_t new_dir = (uint8_t)(angle_deg / 30.0);
        tank->direction = new_dir;
        prev_angle_deg = angle_deg;
    }
}

/**
 * @brief Fires a shot from the tank towards the crosshair.
 */
void shoot() {
  Sprite *crosshair = get_crosshair();
  Sprite *current_sprite;
  int crosshair_x = crosshair->x + 10;
  int crosshair_y = crosshair->y + 10;
  create_explosion(crosshair_x, crosshair_y);
  crosshair_x = crosshair->x + (crosshair->width / 2);
  crosshair_y = crosshair->y + (crosshair->height / 2);
  Enemy *current = get_enemy_list();
  while (current != NULL) {
    if (current->model->type == STATIC_SPRITE)
      current_sprite = current->model->sprite.sp;
    else
      current_sprite = current->model->sprite.asp->sp;
    if (crosshair_x >= current_sprite->x && crosshair_x <= current_sprite->x + current_sprite->width &&
        crosshair_y >= current_sprite->y && crosshair_y <= current_sprite->y + current_sprite->height) {
      destroy_enemy(current);
      increase_score();
      return;
    }
    current = current->next;
  }
}
