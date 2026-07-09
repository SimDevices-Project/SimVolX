#ifndef __PWM_H_
#define __PWM_H_

#include "bsp.h"

#define PWM_MAX_BRIGHTNESS 1000

typedef enum {
  PWM_LED_BTA = 0,
  PWM_LED_BTB,
  PWM_LED_BTC,
  PWM_LED_BTD,
  PWM_LED_FXL,
  PWM_LED_FXR,
  PWM_LED_START,
  PWM_LED_COUNT,
} PwmLedIndex;

void PWM_Init(void);
void PWM_SetBrightness(PwmLedIndex led, uint16_t brightness);

#endif // !__PWM_H_
