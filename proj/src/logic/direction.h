#ifndef __DIRECTION_H
#define __DIRECTION_H

#include <lcom/lcf.h>

#define NUM_DIRECTIONS 12

typedef enum {  // using clock directions atm
  DIRECTION_1,  // 1 o'clock
  DIRECTION_2,  // 2 o'clock, etc.
  DIRECTION_3,
  DIRECTION_4,
  DIRECTION_5,
  DIRECTION_6,
  DIRECTION_7,
  DIRECTION_8,
  DIRECTION_9,
  DIRECTION_10,
  DIRECTION_11,
  DIRECTION_12
} Direction;

#endif
