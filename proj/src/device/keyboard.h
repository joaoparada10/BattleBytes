#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

int (kbc_subscribe_int)(uint8_t *bit_no);

int (kbc_unsubscribe_int)();

int (kbc_issue_cmd)(uint8_t p, uint8_t cmd);

uint8_t (kbc_read_cmd)();

void (kbc_write_cmd)(uint8_t cmd);

int (kbc_read_value)();

void (kbc_ih)();

bool is_valid_kb_st(uint8_t stat);

void process_scancode(bool* is_make,uint8_t* size, uint8_t* bytes);

#endif

