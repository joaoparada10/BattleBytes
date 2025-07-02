/**
 * @file menu.c
 * @brief Implementation of the menu system for the game.
 */

#include "menu.h"


/// @brief Static variables for the menu fonts.
static xpm_image_t menu_font_selected, menu_font_unselected, menu_numbers, title_font;
static uint8_t *menu_font_selected_ptr, *menu_font_unselected_ptr;
static uint8_t *menu_numbers_ptr, *title_font_ptr;

/**
 * @brief Creates a new menu.
 *
 * @param sp Pointer to the sprite representing the menu background.
 * @param title The title of the menu.
 * @param title_x X coordinate for the title.
 * @param title_y Y coordinate for the title.
 * @param options Array of strings representing the menu options.
 * @param options_x Array of X coordinates for each option.
 * @param options_y Array of Y coordinates for each option.
 * @param options_x_hitbox Array of X coordinates for the hitboxes of each option.
 * @param options_y_hitbox Array of Y coordinates for the hitboxes of each option.
 * @param options_height Array of heights for each option's hitbox.
 * @param options_width Array of widths for each option's hitbox.
 * @param options_state Array of states associated with each option.
 * @param num_options The number of options in the menu.
 * @return Pointer to the newly created menu or NULL if memory allocation fails.
 */

Menu *create_menu(Sprite *sp, char *title, int title_x, int title_y, char **options, int *options_x, int *options_y, int *options_x_hitbox,
                  int *options_y_hitbox, int *options_height, int *options_width, State *options_state, int num_options) {
  Menu *menu = (Menu *) malloc(sizeof(Menu));
  if (menu == NULL) {
    return NULL;
  }

  menu->sp = sp;
  menu->title = strdup(title); // Duplicate the title string to avoid issues with original string lifetime
  menu->title_x = title_x;
  menu->title_y = title_y;
  menu->num_options = num_options;
  menu->selected_option = 0;

  menu->options = (char **) malloc(num_options * sizeof(char *));
  menu->options_x = (int *) malloc(num_options * sizeof(int));
  menu->options_y = (int *) malloc(num_options * sizeof(int));
  menu->options_x_hitbox = (int *) malloc(num_options * sizeof(int));
  menu->options_y_hitbox = (int *) malloc(num_options * sizeof(int));
  menu->options_height = (int *) malloc(num_options * sizeof(int));
  menu->options_width = (int *) malloc(num_options * sizeof(int));
  menu->options_state = (State *) malloc(num_options * sizeof(State));

  for (int i = 0; i < num_options; i++) {
    menu->options[i] = strdup(options[i]);
    menu->options_x[i] = options_x[i];
    menu->options_y[i] = options_y[i];
    menu->options_x_hitbox[i] = options_x_hitbox[i];
    menu->options_y_hitbox[i] = options_y_hitbox[i];
    menu->options_height[i] = options_height[i];
    menu->options_width[i] = options_width[i];
    menu->options_state[i] = options_state[i];
  }

  return menu;
}

/**
 * @brief Frees the memory allocated for the menu.
 *
 * @param menu Pointer to the menu to be freed.
 */

void free_menu(Menu *menu) {
  if (menu) {
    free(menu->title);
    for (int i = 0; i < menu->num_options; i++) {
      free(menu->options[i]);
    }
    free(menu->options);
    free(menu->options_x);
    free(menu->options_y);
    free(menu->options_x_hitbox);
    free(menu->options_y_hitbox);
    free(menu->options_height);
    free(menu->options_width);
    free(menu->options_state);
    free(menu);
  }
}

/**
 * @brief Loads the fonts used in the menu.
 */

void load_menu_fonts() {
  menu_font_selected_ptr = xpm_load(menu_font_selected_xpm, XPM_8_8_8, &menu_font_selected);
  menu_font_unselected_ptr = xpm_load(menu_font_unselected_xpm, XPM_8_8_8, &menu_font_unselected);
  menu_numbers_ptr = xpm_load(menu_numbers_xpm, XPM_8_8_8, &menu_numbers);
  title_font_ptr = xpm_load(title_font_xpm, XPM_8_8_8, &title_font);
}

/**
 * @brief Frees the memory allocated for the menu fonts.
 */

void free_menu_fonts() {
  free(menu_font_selected_ptr);
  free(menu_font_unselected_ptr);
  free(menu_numbers_ptr);
  free(title_font_ptr);
}

/**
 * @brief Loads the initial menu.
 *
 * @return Pointer to the newly loaded initial menu.
 */

Menu *load_initial_menu() {
  int initial_menu_options_x[1] = {72};
  int initial_menu_options_y[1] = {430};
  int options_x_hitbox[1] = {60};
  int options_y_hitbox[1] = {406};
  int options_height[1] = {78};
  int options_width[1] = {695};
  State options_state[1] = {LOADING_MAIN_MENU};
  Menu *initial_menu = create_menu(create_sprite((const char **) initial_menu_xpm, 0, 0, 0, 0), "BYTE BATTLES", 130, 100, (char *[]){"PRESS SPACEBAR TO START"},
                                   initial_menu_options_x, initial_menu_options_y, options_x_hitbox, options_y_hitbox, options_height, options_width, options_state, 1);

  return initial_menu;
}

/**
 * @brief Loads the main menu.
 *
 * @return Pointer to the newly loaded main menu.
 */

Menu *load_main_menu() {
  int main_menu_x[4] = {260, 350, 260, 360};
  int main_menu_y[4] = {210, 310, 410, 510};
  int options_x_hitbox[4] = {139, 139, 139, 139};
  int options_y_hitbox[4] = {180, 281, 382, 483};
  int options_height[4] = {85, 85, 85, 85};
  int options_width[4] = {521, 521, 521, 521};
  State options_state[4] = {LOADING_GAME, LOADING_HELP, LOADING_HIGHSCORES, KILL};
  Menu *main_menu = create_menu(create_sprite((const char **) main_menu_xpm, 0, 0, 0, 0), "MAIN MENU", 200, 100, (char *[]){"START GAME", "HELP", "HIGHSCORES", "EXIT"},
                                main_menu_x, main_menu_y, options_x_hitbox, options_y_hitbox, options_height, options_width, options_state, 4);
  return main_menu;
}

/**
 * @brief Loads the help menu.
 *
 * @return Pointer to the newly loaded help menu.
 */

Menu *load_help_menu() {
  int help_menu_x[1] = {255};
  int help_menu_y[1] = {525};
  int options_x_hitbox[1] = {139};
  int options_y_hitbox[1] = {497};
  int options_height[1] = {84};
  int options_width[1] = {521};
  State options_state[1] = {LOADING_MAIN_MENU};
  Menu *help_menu = create_menu(create_sprite((const char **) help_menu_xpm, 0, 0, 0, 0), "HELP MENU", 200, 100, (char *[]){"MAIN MENU"},
                                help_menu_x, help_menu_y, options_x_hitbox, options_y_hitbox, options_height, options_width, options_state, 1);
  return help_menu;
}

/**
 * @brief Loads the highscores menu.
 *
 * @return Pointer to the newly loaded highscores menu.
 */

Menu *load_highscores_menu() {
  int highscores_menu_x[1] = {0};
  int highscores_menu_y[1] = {0};
  int options_x_hitbox[1] = {0};
  int options_y_hitbox[1] = {0};
  int options_height[1] = {0};
  int options_width[1] = {0};
  State options_state[1] = {LOADING_MAIN_MENU};
  Menu *highscores_menu = create_menu(create_sprite((const char **) highscores_menu_xpm, 0, 0, 0, 0), "HIGHSCORES", 200, 100, (char *[]){""},
                                      highscores_menu_x, highscores_menu_y, options_x_hitbox, options_y_hitbox, options_height, options_width, options_state, 1);
  return highscores_menu;
}

/**
 * @brief Loads the pause menu.
 *
 * @return Pointer to the newly loaded pause menu.
 */

Menu *load_pause_menu() {
  int pause_menu_x[3] = {300, 256, 340};
  int pause_menu_y[3] = {256, 355, 458};
  int options_x_hitbox[3] = {139, 139, 139};
  int options_y_hitbox[3] = {229, 330, 431};
  int options_height[3] = {85, 85, 85};
  int options_width[3] = {522, 522, 522};
  State options_state[3] = {INGAME, LOADING_MAIN_MENU, KILL};
  Menu *pause_menu = create_menu(create_sprite((const char **) pause_menu_xpm, 0, 0, 0, 0), "GAME PAUSED", 200, 100, (char *[]){"RESUME", "MAIN MENU", "EXIT"},
                                 pause_menu_x, pause_menu_y, options_x_hitbox, options_y_hitbox, options_height, options_width, options_state, 3);
  return pause_menu;
}

/**
 * @brief Loads the game over menu.
 *
 * @return Pointer to the newly loaded game over menu.
 */

Menu *load_game_over_menu() {

  int num_options = 4;

  int game_over_menu_x[4] = {280, 280, 280, 330};
  int game_over_menu_y[4] = {210, 310, 410, 510};
  int options_x_hitbox[4] = {139, 139, 139, 139};
  int options_y_hitbox[4] = {180, 281, 382, 483};
  int options_height[4] = {85, 85, 85, 85};
  int options_width[4] = {522, 522, 522, 522};
  State options_state[4] = {GAME_OVER, LOADING_GAME, LOADING_MAIN_MENU, KILL};

  char *options[4] = {"SCORE", "PLAY AGAIN", "MAIN MENU", "EXIT"};

  Menu *game_over_menu = create_menu(
    create_sprite((const char **) game_over_menu_xpm, 0, 0, 0, 0),
    "GAME OVER",
    200, 100,
    options,
    game_over_menu_x,
    game_over_menu_y,
    options_x_hitbox,
    options_y_hitbox,
    options_height,
    options_width,
    options_state,
    num_options);

  return game_over_menu;
}

/**
 * @brief Loads the new highscore menu.
 *
 * @return Pointer to the newly loaded new highscore menu.
 */

Menu *load_new_highscore_menu() {
  int new_highscore_menu_x[1] = {0};
  int new_highscore_menu_y[1] = {0};
  int options_x_hitbox[3] = {139, 139, 139};
  int options_y_hitbox[3] = {229, 330, 431};
  int options_height[3] = {85, 85, 85};
  int options_width[3] = {522, 522, 522};
  State options_state[1] = {LOADING_MAIN_MENU};
  Menu *new_highscore_menu = create_menu(create_sprite((const char **) new_highscore_menu_xpm, 0, 0, 0, 0), "NEW HIGHSCORE", 200, 100, (char *[]){"SUBMIT"},
                                         new_highscore_menu_x, new_highscore_menu_y, options_x_hitbox, options_y_hitbox, options_height, options_width, options_state, 1);
  return new_highscore_menu;
}

/**
 * @brief Displays the given menu on the screen.
 *
 * @param menu Pointer to the menu to be displayed.
 */

void display_menu(Menu *menu) {
  draw_sprite(menu->sp);
  draw_title(menu);
  draw_options(menu);
}

/**
 * @brief Draws the title of the menu on the screen.
 *
 * @param menu Pointer to the menu whose title is to be drawn.
 */

void draw_title(Menu *menu) {
  draw_string(menu->title, menu->title_x, menu->title_y, TITLE_FONT_OFFSET, TITLE_FONT_HEIGHT, TITLE_FONT_WIDTH, title_font_ptr, title_font);
}

/**
 * @brief Draws the options of the menu on the screen.
 *
 * @param menu Pointer to the menu whose options are to be drawn.
 */

void draw_options(Menu *menu) {
  for (int i = 0; i < menu->num_options; i++) {
    if (menu->selected_option == i)
      draw_string(menu->options[i], menu->options_x[i], menu->options_y[i], MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_selected_ptr, menu_font_selected);

    else {
      draw_string(menu->options[i], menu->options_x[i], menu->options_y[i], MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_unselected_ptr, menu_font_unselected);
    }
  }
  GameState state = get_game_state();
  if (state.state == GAME_OVER) {
    int score = state.score;
    char *score_str = (char *) malloc(sizeof(char) * 20);
    sprintf(score_str, "%d", score);
    draw_string(score_str, 460, 210, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_numbers_ptr, menu_numbers);
  }
  if (state.state == HELP_MENU){
    draw_string("W A S D", 100, 200, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_selected_ptr, menu_font_selected);
    draw_string("TO MOVE", 340, 200, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_unselected_ptr, menu_font_unselected);
    draw_string("MOVE MOUSE", 100, 250, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_selected_ptr, menu_font_selected);
    draw_string("TO AIM", 430, 250, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_unselected_ptr, menu_font_unselected);
    draw_string("LEFT CLICK", 100, 300, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_selected_ptr, menu_font_selected);
    draw_string("TO SHOOT", 430, 300, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_unselected_ptr, menu_font_unselected);
    draw_string("SPACEBAR", 100, 350, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_selected_ptr, menu_font_selected);
    draw_string("TO PAUSE", 370, 350, MENU_FONT_OFFSET, MENU_FONT_HEIGHT, MENU_FONT_WIDTH, menu_font_unselected_ptr, menu_font_unselected);
  
  }
}

/**
 * @brief Draws a single character on the screen.
 *
 * @param c The character to be drawn.
 * @param x The X coordinate of the character.
 * @param y The Y coordinate of the character.
 * @param font_offset The offset between characters in the font.
 * @param font_height The height of the character.
 * @param font_width The width of the character.
 * @param font_ptr Pointer to the font data.
 * @param font The font image.
 */

void draw_character(char c, uint16_t x, uint16_t y, uint16_t font_offset, uint16_t font_height, uint16_t font_width, uint8_t *font_ptr, xpm_image_t font) {
  char *buffer = get_drawing_buffer();
  int char_pos;
  if (c >= 'A' && c <= 'Z') {
    char_pos = c - 'A';
  }
  else if (c >= '0' && c <= '9') {
    char_pos = c - '0';
  }
  else {
    return;
  }
  int xpm_width = font.width;
  int char_offset = char_pos * (font_width + font_offset) * 3;
  uint8_t *char_ptr = font_ptr + char_offset;
  uint32_t color = 0;
  uint32_t desl = 0;
  int color_index;
  for (int i = 0; i < font_height; i++) {
    for (int j = 0; j < font_width; j++) {
      desl = (i * xpm_width + j) * 3;
      color = char_ptr[desl] | (char_ptr[desl + 1] << 8) | (char_ptr[desl + 2] << 16);

      if (color != xpm_transparency_color(XPM_8_8_8)) {
        color_index = (H_RES * (y + i) + (x + j)) * 3;
        buffer[color_index] = color & 0xFF;
        buffer[color_index + 1] = (color >> 8) & 0xFF;
        buffer[color_index + 2] = (color >> 16) & 0xFF;
      }
    }
  }
}

/**
 * @brief Draws a string on the screen.
 *
 * @param str The string to be drawn.
 * @param x The X coordinate of the string.
 * @param y The Y coordinate of the string.
 * @param offset The offset between characters.
 * @param font_height The height of the characters.
 * @param font_width The width of the characters.
 * @param font_ptr Pointer to the font data.
 * @param font The font image.
 */

void draw_string(char *str, int x, int y, int offset, uint16_t font_height, uint16_t font_width, uint8_t *font_ptr, xpm_image_t font) {
  int current_x = x;
  while (*str) {
    draw_character(*str, current_x, y, offset, font_height, font_width, font_ptr, font);
    current_x += font_width + offset;
    str++;
  }
}

/**
 * @brief Handles menu option selection based on mouse click.
 *
 * @param menu Pointer to the menu.
 */

void handle_menu_click(Menu *menu) {
  int x = get_cursor()->x;
  int y = get_cursor()->y;
  for (int i = 0; i < menu->num_options; i++) {
    if (x >= menu->options_x_hitbox[i] && x <= menu->options_x_hitbox[i] + menu->options_width[i] &&
        y >= menu->options_y_hitbox[i] && y <= menu->options_y_hitbox[i] + menu->options_height[i]) {
      set_state(menu->options_state[i]);
    }
  }
}

/**
 * @brief Handles menu option hover based on mouse movement.
 *
 * @param menu Pointer to the menu.
 */

void handle_menu_hover(Menu *menu) {
  int x = get_cursor()->x;
  int y = get_cursor()->y;
  for (int i = 0; i < menu->num_options; i++) {
    if (x >= menu->options_x_hitbox[i] && x <= menu->options_x_hitbox[i] + menu->options_width[i] &&
        y >= menu->options_y_hitbox[i] && y <= menu->options_y_hitbox[i] + menu->options_height[i]) {
      menu->selected_option = i;
    }
    else if (menu->selected_option == i)
      menu->selected_option = -1;
  }
}

/**
 * @brief Handles menu navigation based on keyboard input.
 *
 * @param menu Pointer to the menu.
 * @param bytes Array of bytes representing the keyboard input.
 * @param size Size of the array of bytes.
 */

void handle_menu_keyboard(Menu *menu, uint8_t *bytes, uint8_t size) {
  uint8_t breakcode;
  if (size == 2)
    breakcode = bytes[1];
  else
    breakcode = bytes[0];
  switch (breakcode) {
    case ARROW_UP_BREAK_CODE:
      menu->selected_option--;
      if (menu->selected_option < 0) {
        menu->selected_option = menu->num_options - 1;
      }
      break;
    case ARROW_DOWN_BREAK_CODE:
      menu->selected_option++;
      if (menu->selected_option >= menu->num_options) {
        menu->selected_option = 0;
      }
      break;
    case SPACEBAR_BREAK_CODE:
      if (menu->selected_option >= 0 && menu->selected_option < menu->num_options) {
        set_state(menu->options_state[menu->selected_option]);
      }
      break;
    default:
      break;
  }
}


/**
 * @brief Gets the initial menu.
 *
 * @return Pointer to the initial menu.
 */

Menu *get_initial_menu() {
  Menu *initial_menu = load_initial_menu();
  return initial_menu;
}

/**
 * @brief Gets the main menu.
 *
 * @return Pointer to the main menu.
 */

Menu *get_main_menu() {
  Menu *main_menu = load_main_menu();
  return main_menu;
}


/**
 * @brief Gets the help menu.
 *
 * @return Pointer to the help menu.
 */

Menu *get_help_menu() {
  Menu *help_menu = load_help_menu();
  return help_menu;
}

/**
 * @brief Gets the highscores menu.
 *
 * @return Pointer to the highscores menu.
 */

Menu *get_highscores_menu() {
  Menu *highscores_menu = load_highscores_menu();
  return highscores_menu;
}

/**
 * @brief Gets the pause menu.
 *
 * @return Pointer to the pause menu.
 */

Menu *get_pause_menu() {
  Menu *pause_menu = load_pause_menu();
  return pause_menu;
}

/**
 * @brief Gets the game over menu.
 *
 * @return Pointer to the game over menu.
 */

Menu *get_game_over_menu() {
  Menu *game_over_menu = load_game_over_menu();
  return game_over_menu;
}

/**
 * @brief Gets the new highscore menu.
 *
 * @return Pointer to the new highscore menu.
 */

Menu *get_new_highscore_menu() {
  Menu *new_highscore_menu = load_new_highscore_menu();
  return new_highscore_menu;
}
