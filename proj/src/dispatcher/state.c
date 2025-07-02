/**
 * @file state.c
 * @brief Implementation file for managing game state
 */

#include "state.h"

static GameState game_state; /**< Static instance of the game state */

/**
 * @brief Initializes the game state with default values
 * @return The initial state of the game
 */

State init_game_state(){
  game_state.score = 0;
  game_state.timer = 0;
  game_state.game_time = 0;
  game_state.difficulty = 0;
  game_state.state = INITIAL;
  return game_state.state;
}

/**
 * @brief Retrieves the current score
 * @return The current score
 */

uint32_t get_score() {
    return game_state.score;
}

/**
 * @brief Sets the score to a new value
 * @param new_score The new score value
 */

void set_score(uint32_t new_score) {
    game_state.score = new_score;
}

/**
 * @brief Increases the score based on the current difficulty level
 */

void increase_score(){
  game_state.score += 10 * game_state.difficulty;
}

/**
 * @brief Retrieves the current timer value
 * @return The current timer value
 */

uint32_t get_timer() {
    return game_state.timer;
}

/**
 * @brief Sets the timer to a new value
 * @param new_timer The new timer value
 */

void set_timer(uint32_t new_timer){
  game_state.timer = new_timer;
}

/**
 * @brief Increases the timer by 1
 */

void increase_timer(){
  game_state.timer++;
}

/**
 * @brief Retrieves the current game time
 * @return The current game time
 */

uint32_t get_game_time() {
    return game_state.game_time;
}

/**
 * @brief Sets the game time to a new value
 * @param new_game_time The new game time value
 */

void set_game_time(uint32_t new_game_time){
  game_state.game_time = new_game_time;
}

/**
 * @brief Increases the game time by 1
 */

void increase_game_time(){
  game_state.game_time++;
}

/**
 * @brief Retrieves the current difficulty level
 * @return The current difficulty level
 */

uint8_t get_difficulty(){
  return game_state.difficulty;
}

/**
 * @brief Sets the difficulty level to a new value
 * @param new_difficulty The new difficulty level
 */

void set_difficulty(uint8_t new_difficulty){
  game_state.difficulty = new_difficulty;
}

/**
 * @brief Increases the difficulty level by 1
 */

void increase_difficulty(){
  game_state.difficulty++;
}

/**
 * @brief Retrieves the current game state
 * @return The current game state
 */

GameState get_game_state(){
  return game_state;
}

/**
 * @brief Sets the game state to a new state
 * @param new_game_state The new game state
 */

void set_game_state(GameState new_game_state){
  game_state = new_game_state;
}

/**
 * @brief Retrieves the current state of the game
 * @return The current state of the game
 */

State get_state(){
  return game_state.state;
}

/**
 * @brief Sets the state of the game to a new state
 * @param new_state The new state of the game
 */

void set_state(State new_state){
  game_state.state = new_state;
}

/**
 * @brief Resets the game statistics to default values
 * @param state The current game state
 */

void reset_game_stats(GameState state){
  game_state.score = 0;
  game_state.timer = 0;
  game_state.game_time = 0;
  game_state.difficulty = 0;
}
