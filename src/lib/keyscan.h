#ifndef __KEYSCAN_H_
#define __KEYSCAN_H_

#include "bsp.h"

#define KEY_COUNT 10

typedef enum {
  KEY_BTA = 0,
  KEY_BTB,
  KEY_BTC,
  KEY_BTD,
  KEY_FXL,
  KEY_FXR,
  KEY_START,
  KEY_COIN,
  KEY_TEST,
  KEY_SRV,
} KeyIndex;

void KeyScan_Init();
void KeyScan_Scan();
uint8_t KeyScan_GetKeyDebouncedStatus(uint8_t keyIndex);
uint16_t KeyScan_GetAllKeyDebouncedStatus();

#endif // !__KEYSCAN_H_
