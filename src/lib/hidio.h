#ifndef __HIDIO_H_
#define __HIDIO_H_

#include "bsp.h"
#include "keyscan.h"
#include "led.h"

#define KEY_COIN_INDEX KEY_COIN

typedef enum {
  NORMAL     = 0x00,
  JAM        = 0x01,
  DISCONNECT = 0x02,
  BUSY       = 0x03,
} __packed CoinCondition;

typedef enum {
  SET_COMM_TIMEOUT   = 0x01,
  SET_SAMPLING_COUNT = 0x02,
  CLEAR_BOARD_STATUS = 0x03,
  SET_GENERAL_OUTPUT = 0x04,
  SET_PWM_OUTPUT     = 0x05,
} __packed HidioCommand;

typedef struct {
  CoinCondition condition;
  uint8_t count;
} __packed CoinData;

typedef struct {
  uint8_t reportID;
  uint16_t analog[8];
  uint16_t rotary[4];
  CoinData coin[2];
  uint8_t buttons[4];
  uint8_t systemStatus;
  uint8_t usbStatus;
  uint8_t _unused[29];
} __packed DataUpload;

typedef struct {
  uint8_t reportID;
  HidioCommand command;
  uint8_t payload[62];
} __packed DataReceive;

typedef struct {
  uint8_t reportID;
  uint8_t ctrlkey;
  uint8_t keymap[16];
} __packed KbdData;

typedef struct {
  uint8_t reportID;
  uint8_t button;
  uint8_t x;
  uint8_t y;
  uint8_t wheel;
} __packed MseData;
typedef struct {
  uint8_t reportID;
  uint16_t x;
  uint16_t y;
} __packed SpMseData;

const static uint8_t hid_key_map[KEY_COUNT][2] = {
    {0x00, 0x01}, // BTA
    {0x00, 0x02}, // BTB
    {0x00, 0x04}, // BTC
    {0x00, 0x08}, // BTD
    {0x00, 0x10}, // FXL
    {0x00, 0x20}, // FXR
    {0x00, 0x40}, // START
    {0},          // COIN
    {0x01, 0x02}, // TEST
    {0x01, 0x01}, // SRV
};

const static uint8_t kbd_key_map[KEY_COUNT][2] = {
    {0x00, 0x16}, // BTA
    {0x00, 0x07}, // BTB
    {0x00, 0x09}, // BTC
    {0x00, 0x0D}, // BTD
    {0x00, 0x0E}, // FXL
    {0x00, 0x0F}, // FXR
    {0x00, 0x2C}, // START
    {0x00, 0x3C}, // COIN
    {0x00, 0x3A}, // TEST
    {0x00, 0x3B}, // SRV
};
void HIDIO_SGIO4_Heartbeat();
void HIDIO_Init();
void HIDIO_Receive_Handler();
void HIDIO_Update();

extern uint8_t HID_Buffer_OUT[64];
extern uint8_t HID_Buffer_IN[64];

#endif // !__HIDIO_H_
