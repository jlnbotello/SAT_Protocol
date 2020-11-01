#ifndef LED_H
#define LED_H
#include "stdint.h"


typedef enum {L1, L2, L3, L4} LedId_t;

void Led_Init();

void Led_On(LedId_t led_idx);

void Led_Off(LedId_t led_idx);

void Led_Toggle(LedId_t led_idx);

#endif //LED_H