#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"
#include "pwm.h"

#define RGB_PORT_COUNT      2
#define RGB_COUNT_PER_PORT  35

typedef enum {
  LED_RIGHT = 0x00,
  LED_LEFT  = 0x01,
} __packed LED_RGB_Port;

typedef enum {
  LED_OFF = 0x00,
  LED_ON  = 0x01,
} __packed LED_State;

void LED_Init(void);

void LED_Animation_PowerOn();

void LED_RGB_Set(LED_RGB_Port port, uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void LED_RGB_SetAll(uint8_t r, uint8_t g, uint8_t b);
void LED_RGB_SetPort(LED_RGB_Port port, uint8_t r, uint8_t g, uint8_t b);

void LED_Refresh(void);

void LED_PWM_SetBrightness(PwmLedIndex led, uint16_t brightness);

#ifdef __cplusplus
}
#endif

#endif // !__LED_H
