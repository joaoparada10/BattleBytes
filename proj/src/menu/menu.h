#ifndef _MENU_H_
#define _MENU_H_

#include <lcom/lcf.h>
#include "../view/game_view.h"
#include "../graphics/sprite.h"
#include "../dispatcher/state.h"
#include "../device/i8042.h"
#include "../../assets/xpm/menus/initial_menu.xpm"
#include "../../assets/xpm/menus/main_menu.xpm"
#include "../../assets/xpm/menus/highscores_menu.xpm"
#include "../../assets/xpm/menus/help_menu.xpm"
#include "../../assets/xpm/menus/pause_menu.xpm"
#include "../../assets/xpm/menus/game_over_menu.xpm"
#include "../../assets/xpm/menus/new_highscore_menu.xpm"
#include "../../assets/xpm/text/menu_font_selected.xpm"
#include "../../assets/xpm/text/menu_font_unselected.xpm"
#include "../../assets/xpm/text/menu_numbers.xpm"
#include "../../assets/xpm/text/title_font.xpm"

typedef struct{
  Sprite *sp;
  char* title;
  char** options;
  int num_options;
  int title_x;
  int title_y;
  int *options_x;
  int *options_y;
  int *options_x_hitbox;
  int *options_y_hitbox;
  int *options_height;
  int *options_width;
  int selected_option;
  State *options_state;
} Menu;


Menu *create_menu(Sprite *sp, char *title, int title_x, int title_y, char **options, int *options_x, int *options_y, int *options_x_hitbox,
 int *options_y_hitbox, int* options_height, int* options_width,State *options_state, int num_options);

void free_menu(Menu *menu);

void load_menu_fonts();

void free_menu_fonts();


Menu*load_initial_menu();

Menu *load_main_menu();

Menu *load_help_menu();

Menu *load_highscores_menu();

Menu *load_pause_menu();

Menu *load_game_over_menu();

Menu *load_new_highscore_menu();

void(display_menu)(Menu *menu);

void draw_title(Menu *menu);

void draw_options(Menu *menu);

void draw_character(char c, uint16_t x, uint16_t y, uint16_t font_offset, uint16_t font_height,
                 uint16_t font_width, uint8_t* font_ptr, xpm_image_t font);

void draw_string(char* str, int x, int y, int offset, uint16_t font_height, uint16_t font_width, uint8_t *font_ptr, xpm_image_t font);    

void handle_menu_click(Menu *menu);

void handle_menu_hover(Menu *menu);

void handle_menu_keyboard(Menu *menu, uint8_t* bytes, uint8_t size);

Menu *get_initial_menu();

Menu *get_main_menu();

Menu *get_help_menu();

Menu *get_highscores_menu();
Menu *get_pause_menu();

Menu *get_game_over_menu();

Menu *get_new_highscore_menu();

#endif

