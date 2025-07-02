/**
 * @file game_view.c
 * @brief Contains functions related to game rendering and view management.
 */
#include "game_view.h"

static Sprite *crosshair, *cursor, *tank_sprite;
uint8_t *tank_sprites[NUM_DIRECTIONS], *game_letters_ptr, *game_numbers_ptr;
xpm_image_t tank_images[NUM_DIRECTIONS], game_letters, game_numbers;
Explosion *explosion_list = NULL;

/**
 * @brief Loads game sprites into memory.
 * 
 * @return 0 on success.
 */
int load_game_sprites() {
  tank_sprites[0] = xpm_load(tank1_xpm, XPM_8_8_8, &tank_images[0]);
  tank_sprites[1] = xpm_load(tank2_xpm, XPM_8_8_8, &tank_images[1]);
  tank_sprites[2] = xpm_load(tank3_xpm, XPM_8_8_8, &tank_images[2]);
  tank_sprites[3] = xpm_load(tank4_xpm, XPM_8_8_8, &tank_images[3]);
  tank_sprites[4] = xpm_load(tank5_xpm, XPM_8_8_8, &tank_images[4]);
  tank_sprites[5] = xpm_load(tank6_xpm, XPM_8_8_8, &tank_images[5]);
  tank_sprites[6] = xpm_load(tank7_xpm, XPM_8_8_8, &tank_images[6]);
  tank_sprites[7] = xpm_load(tank8_xpm, XPM_8_8_8, &tank_images[7]);
  tank_sprites[8] = xpm_load(tank9_xpm, XPM_8_8_8, &tank_images[8]);
  tank_sprites[9] = xpm_load(tank10_xpm, XPM_8_8_8, &tank_images[9]);
  tank_sprites[10] = xpm_load(tank11_xpm, XPM_8_8_8, &tank_images[10]);
  tank_sprites[11] = xpm_load(tank12_xpm, XPM_8_8_8, &tank_images[11]);
  tank_sprite = create_sprite((const char **) tank9_xpm, 500, 300, 0, 0);
  crosshair = create_sprite((const char **) crosshair_xpm, 400, 300, 5, 5);
  cursor = create_sprite((const char **) cursor_xpm, 400, 300, 0, 0);
  return 0;
}

/**
 * @brief Frees memory allocated for game sprites.
 */
void free_game_sprites() {
  for (int i = 0; i < NUM_DIRECTIONS; i++) {
    if (tank_sprites[i] != NULL) {
      free(tank_sprites[i]);
    }
  }
  destroy_sprite(tank_sprite);
  destroy_sprite(crosshair);
  destroy_sprite(cursor);
}

/**
 * @brief Loads game fonts into memory.
 */
void load_game_fonts() {
  game_letters_ptr = xpm_load(game_letters_xpm, XPM_8_8_8, &game_letters);
  game_numbers_ptr = xpm_load(game_numbers_xpm, XPM_8_8_8, &game_numbers);
}

/**
 * @brief Frees memory allocated for game fonts.
 */
void free_game_fonts() {
  free(game_letters_ptr);
  free(game_numbers_ptr);
}
/**
 * @brief Creates an animated sprite for a virus type 2 enemy.
 * 
 * @param x X-coordinate of the sprite.
 * @param y Y-coordinate of the sprite.
 * @return Pointer to the created animated sprite.
 */
AnimSprite *create_virus2_asprite(int x, int y) {
  AnimSprite *new_enemy_asprite = create_asprite(10, 5, (const char **) virus50_1_xpm, (const char **) virus50_2_xpm, (const char **) virus50_3_xpm, (const char **) virus50_4_xpm,
                                                 (const char **) virus50_5_xpm);
  new_enemy_asprite->sp->xspeed = 2;
  new_enemy_asprite->sp->yspeed = 2;
  return new_enemy_asprite;
}

/**
 * @brief Creates a static sprite for a virus type 1 enemy.
 * 
 * @param x X-coordinate of the sprite.
 * @param y Y-coordinate of the sprite.
 * @return Pointer to the created sprite.
 */
Sprite *create_virus1_sprite(int x, int y) {
  Sprite *new_enemy = create_sprite((const char **) virus40_xpm, x, y, 1, 1);
  return new_enemy;
}

/**
 * @brief Creates an explosion effect at the specified coordinates.
 * 
 * @param x X-coordinate of the explosion.
 * @param y Y-coordinate of the explosion.
 * @return Pointer to the created explosion.
 */
Explosion *create_explosion(int x, int y) {
  Explosion *new_explosion = malloc(sizeof(Explosion));
  AnimSprite *new_asp = create_asprite(3, 20, (const char **) explosion1_xpm, (const char **) explosion2_xpm, (const char **) explosion3_xpm, (const char **) explosion4_xpm,
                                       (const char **) explosion5_xpm, (const char **) explosion6_xpm, (const char **) explosion7_xpm, (const char **) explosion8_xpm, (const char **) explosion9_xpm, (const char **) explosion10_xpm,
                                       (const char **) explosion11_xpm, (const char **) explosion12_xpm, (const char **) explosion13_xpm, (const char **) explosion14_xpm, (const char **) explosion15_xpm, (const char **) explosion16_xpm,
                                       (const char **) explosion17_xpm, (const char **) explosion18_xpm, (const char **) explosion19_xpm, (const char **) explosion20_xpm);
  new_explosion->explosion_asp = new_asp;
  new_explosion->explosion_asp->sp->x = x;
  new_explosion->explosion_asp->sp->y = y;
  new_explosion->next = explosion_list;
  explosion_list = new_explosion;
  return new_explosion;
}

/**
 * @brief Destroys an explosion and frees associated memory.
 * 
 * @param explosion Pointer to the explosion to destroy.
 */
void destroy_explosion(Explosion *explosion) {
  Explosion **current = &explosion_list;
  while (*current != NULL) {
    if (*current == explosion) {
      Explosion *to_free = *current;
      *current = (*current)->next;
      destroy_asprite(to_free->explosion_asp);
      free(to_free);
      return;
    }
    current = &((*current)->next);
  }
}

/**
 * @brief Draws the tank sprite on the screen.
 * 
 * @return 0 on success.
 */
int draw_tank() {
    static Direction last_direction = -1;
    static int last_tank_x = -1, last_tank_y = -1;
    GameUnit *tank = get_tank_model();
    int current_x = tank->sprite.sp->x;
    int current_y = tank->sprite.sp->y;

    // Only update the sprite if the direction or position has changed
    if (last_direction != tank->direction || last_tank_x != current_x || last_tank_y != current_y) {
        // Temporarily set the new sprite properties
        tank->sprite.sp->map = tank_sprites[tank->direction];
        tank->sprite.sp->width = tank_images[tank->direction].width;
        tank->sprite.sp->height = tank_images[tank->direction].height;

        // Check for collision
        if (!arena_collision(tank->sprite.sp)) {
            // Update only if there's no collision
            last_direction = tank->direction;
            last_tank_x = current_x;
            last_tank_y = current_y;
        } else {
            // Revert changes if there's a collision
            tank->sprite.sp->map = tank_sprites[last_direction];
            tank->sprite.sp->width = tank_images[last_direction].width;
            tank->sprite.sp->height = tank_images[last_direction].height;
        }
    }

    draw_sprite(tank->sprite.sp);
    return 0;
}

/**
 * @brief Draws enemy sprites on the screen.
 * 
 * @return 0 on success.
 */
int draw_enemies() {
  Enemy *current = get_enemy_list();
  while (current != NULL) {
    if (current->model->type == STATIC_SPRITE) {
      draw_sprite(current->model->sprite.sp);
    }
    if (current->model->type == ANIMATED_SPRITE) {
      draw_sprite(current->model->sprite.asp->sp);
      animate_asprite(current->model->sprite.asp, false);
    }
    current = current->next;
  }
  return 0;
}

/**
 * @brief Draws explosion animations on the screen.
 * 
 * @return 0 on success.
 */
int draw_explosions() {
  Explosion *current = explosion_list;
  while (current != NULL) {
    draw_sprite(current->explosion_asp->sp);
    Explosion *next = current->next;
    if (animate_asprite(current->explosion_asp, true) == 1) {
      destroy_explosion(current);
    };
    current = next;
  }
  return 0;
}

/**
 * @brief Draws the crosshair sprite on the screen.
 * 
 * @return 0 on success.
 */

int draw_crosshair() {
  if ((crosshair->x != cursor->x) || (crosshair->y != cursor->y)) {
    move_sprite_to(crosshair, cursor->x, cursor->y, false);
    calculate_tank_direction();
    draw_sprite(crosshair);
  }
  return 0;
}

/**
 * @brief Draws the cursor sprite on the screen.
 * 
 * @return 0 on success.
 */

int draw_cursor() {
  draw_sprite(cursor);
  return 0;
}

/**
 * @brief Draws all game elements on the screen.
 * 
 * @return 0 on success.
 */
int draw_elements() {
  draw_tank();
  draw_enemies();
  draw_crosshair();
  draw_cursor();
  return 0;
}

/**
 * @brief Draws the game arena on the screen.
 * 
 * @return 0 on success.
 */

int draw_arena() {
  create_arena(arena3_xpm, 0x007B35);
  return 0;
}

/**
 * @brief Draws the header section of the game interface.
 * 
 * @return 0 on success.
 */

int draw_header() {
  vg_draw_rectangle(0, 0, ARENA_WIDTH, 20, 0);
  GameState state = get_game_state();
  int score = state.score;
  int game_time = state.game_time;

  char *score_str = (char *) malloc(sizeof(char) * 20);

  sprintf(score_str, "%d", score);

  char *game_time_str = (char *) malloc(sizeof(char) * 20);

  sprintf(game_time_str, "%d", game_time);
  draw_string("SCORE", 21, 0, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_letters_ptr, game_letters);
  draw_string(score_str, 147, 0, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_numbers_ptr, game_numbers);
  draw_string("TIME", 611, 0, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_letters_ptr, game_letters);
  draw_string(game_time_str, 716, 0, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_numbers_ptr, game_numbers);

  free(score_str);
  free(game_time_str);

  return 0;
}

/**
 * @brief Draws the footer section of the game interface.
 * 
 * @return 0 on success.
 */

int draw_footer() {
  vg_draw_rectangle(0, 580, ARENA_WIDTH, 20, 0);
  GameState state = get_game_state();
  int hp = get_tank_model()->hp;
  int wave = state.difficulty;

  char *hp_str = (char *) malloc(sizeof(char) * 10);

  sprintf(hp_str, "%d", hp);

  char *wave_str = (char *) malloc(sizeof(char) * 20);

  sprintf(wave_str, "%d", wave);
  draw_string("HP", 21, 580, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_letters_ptr, game_letters);
  draw_string(hp_str, 84, 580, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_numbers_ptr, game_numbers);
  draw_string("WAVE", 611, 580, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_letters_ptr, game_letters);
  draw_string(wave_str, 716, 580, GAME_FONT_OFFSET, GAME_FONT_HEIGHT, GAME_FONT_WIDTH, game_numbers_ptr, game_numbers);

  free(hp_str);
  free(wave_str);

  return 0;
}

/**
 * @brief Draws the game interface.
 * 
 * @return 0 on success.
 */

int draw_game() {
  draw_explosions();
  update_enemies();
  draw_header();
  draw_footer();
  draw_timer();
  draw_elements();
  return 0;
}

/**
 * @brief Draws the game timer on the screen.
 * 
 * @param game_timer Current game timer value.
 * @return 0 on success.
 */

int draw_timer(uint32_t game_timer) {

  return 0;
}

/**
 * @brief Retrieves the tank sprite.
 * 
 * @return Pointer to the tank sprite.
 */

Sprite *get_tank_sprite() {
  return tank_sprite;
}

/**
 * @brief Retrieves the crosshair sprite.
 * 
 * @return Pointer to the crosshair sprite.
 */

Sprite *get_crosshair() {
  return crosshair;
}

/**
 * @brief Retrieves the cursor sprite.
 * 
 * @return Pointer to the cursor sprite.
 */
Sprite *get_cursor() {
  return cursor;
}
