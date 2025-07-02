#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "i8042.h"

typedef enum {
    START,
    DRAWING1,
    PEAK,
    DRAWING2,
    COMPLETE
} MouseState;


int (m_kbc_subscribe_int)(uint8_t *bit_no);

int (m_kbc_unsubscribe_int)();

bool (is_valid_m_st)(uint8_t stat);

int (m_read_obf_byte)();

void (mouse_ih)();

int (mouse_packet_handler)(struct packet *pp);

int (kbc_issue_mouse_cmd)(uint8_t cmd);

int (kbc_issue_cmd)(uint8_t p, uint8_t cmd);

void (mouse_state_machine)(struct packet *pp, MouseState *currentState, uint8_t x_len, uint8_t tolerance);

void (state_change)(MouseState newState);

void (state_maintain)(struct packet *pp);

#endif
