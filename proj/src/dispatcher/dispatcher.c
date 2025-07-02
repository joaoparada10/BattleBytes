/**
 * @file dispatcher.c
 * @brief Dispatcher for handling game states and inputs
 */

#include "dispatcher.h"

/** @brief Pointer to the current menu being displayed. */
static Menu *current_menu = NULL;


/**
 * @brief Handles the game state transitions and actions.
 * 
 * This function manages the various states of the game, performing actions
 * such as loading menus, updating game elements, and rendering the screen
 * based on the current state.
 */
void game_state_handler() {

  GameState game_state = get_game_state();
  State state = game_state.state;
  uint32_t timer = game_state.timer;

  switch (state) {
    case INITIAL:
      load_menu_fonts();
      load_game_fonts();
      load_game_sprites();
      current_menu = get_initial_menu();
      set_state(INITIAL_MENU);
      break;
    case INITIAL_MENU:
    case HIGHSCORES_MENU:
    case MAIN_MENU:
    case GAME_OVER:
    case PAUSE_MENU:
    case HELP_MENU:
      display_menu(current_menu);
      draw_cursor();
      vg_flip_buffers();
      break;
    case LOADING_MAIN_MENU:
      free_menu(current_menu);
      current_menu = get_main_menu();
      display_menu(current_menu);
      set_state(MAIN_MENU);
      break;
    case LOADING_HELP:
      free_menu(current_menu);
      current_menu = get_help_menu();
      display_menu(current_menu);
      set_state(HELP_MENU);
      break;
    case LOADING_HIGHSCORES:
      free_menu(current_menu);
      current_menu = get_highscores_menu();
      display_menu(current_menu);
      set_state(HIGHSCORES_MENU);
      break;
    case LOADING_PAUSE:
      current_menu = get_pause_menu();
      display_menu(current_menu);
      set_state(PAUSE_MENU);
      break;
    case WAITING:
      break;
    case LOADING_GAME:
      free_menu(current_menu);
      cleanup_elements();
      create_game_elements();
      reset_game_stats(game_state);
      draw_arena();
      set_state(INGAME);
      break;
    case INGAME:
      if ((timer + 1) % (5 * 30) == 0) {
        increase_difficulty();
        spawn_enemy_wave();
      }

      draw_game();
      vg_flip_buffers();

      increase_timer();
      if (timer % 30 == 0)
        increase_game_time();
      break;
    case GAME_END:
      destroy_arena(get_current_arena());
      current_menu = get_game_over_menu();
      set_state(GAME_OVER);
      break;
    case KILL:
      printf("Killing game\n");
      free_menu_fonts();
      free_game_fonts();
      free_menu(current_menu);

      break;
    default:
      break;
  }
}

/**
 * @brief Handles keyboard input based on the current state.
 * 
 * @param make True if the key was pressed, false if it was released.
 * @param size Size of the bytes array.
 * @param bytes Array containing the scancode bytes.
 */
void kbd_state_handler(bool make, uint8_t size, uint8_t *bytes) {
  State state = get_state();
  switch (state) {
    case INITIAL_MENU:
    case HIGHSCORES_MENU:
    case MAIN_MENU:
    case GAME_OVER:
    case PAUSE_MENU:
    case HELP_MENU:
      handle_menu_keyboard(current_menu, bytes, size);
      break;
    case INGAME:
      handle_game_keyboard(bytes, size);
      break;
    default:
      break;
  }
}

/**
 * @brief Handles mouse input based on the current state.
 * 
 * @param pp Packet structure containing mouse event data.
 */
void mouse_state_handler(struct packet *pp) {
  State state = get_state();
  switch (state) {
    case INITIAL_MENU:
    case HIGHSCORES_MENU:
    case MAIN_MENU:
    case GAME_OVER:
    case PAUSE_MENU:
    case HELP_MENU:
      move_cursor(pp->delta_x, -pp->delta_y);
      handle_menu_hover(current_menu);
      if (pp->lb) {
        handle_menu_click(current_menu);
      }
      break;
    case INGAME:
      move_cursor(pp->delta_x, -pp->delta_y);
      if (pp->lb) {
        shoot();
      }
      break;
    default:
      break;
  }
}
