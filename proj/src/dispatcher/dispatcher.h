#ifndef __DISPATCHER_H
#define __DISPATCHER_H

#include <lcom/lcf.h>
#include "../menu/menu.h"
#include "../device/keyboard.h"
#include "../view/game_view.h"
#include "../model/game_model.h"
#include "../device/mouse.h"
#include "../graphics/video_gr.h"
#include "../logic/game_logic.h"
#include "state.h"


void game_state_handler();

void kbd_state_handler(bool make, uint8_t size, uint8_t *bytes);

void mouse_state_handler(struct packet *pp);


#endif
