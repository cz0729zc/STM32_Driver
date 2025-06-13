#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4

extern uint8_t key_state, key_count;

void Key_Init(void);

#endif
