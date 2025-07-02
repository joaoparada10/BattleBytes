#ifndef __STATE_H
#define __STATE_H

#include <stdint.h>

typedef enum {
  INITIAL,
  INITIAL_MENU,
  LOADING_MAIN_MENU,
  MAIN_MENU,
  LOADING_HELP,
  HELP_MENU,
  LOADING_HIGHSCORES,
  HIGHSCORES_MENU,
  WAITING,
  LOADING_GAME,
  INGAME,
  LOADING_PAUSE,
  PAUSE_MENU,
  GAME_END,
  GAME_OVER,
  NEW_HIGHSCORE,
  KILL
} State;

typedef struct {
    uint32_t score;
    uint32_t timer;
    uint32_t game_time;
    uint8_t difficulty;
    State state;
} GameState;


State init_game_state();

uint32_t get_score();
void set_score(uint32_t new_score);
void increase_score();

uint32_t get_timer();
void set_timer(uint32_t new_timer);
void increase_timer();

uint32_t get_game_time();
void set_game_time(uint32_t new_game_time);
void increase_game_time();

uint8_t get_difficulty();
void set_difficulty(uint8_t new_difficulty);
void increase_difficulty();

GameState get_game_state();
void set_game_state(GameState new_game_state);

State get_state();
void set_state(State new_state);

void reset_game_stats(GameState state);

#endif
