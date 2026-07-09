#ifndef __HIDCONFIG_H_
#define __HIDCONFIG_H_

#include "usb_lib.h"
#include "usb_prop.h"
#include "led.h"

typedef enum {
  HIDCONFIG_REPORT_ID = 0xAA,
} __packed HidconfigReportID;

typedef enum {
  LEDTAG_RGB_LEFT  = 0x01,
  LEDTAG_RGB_RIGHT = 0x00,

  LEDTAG_ALL = 0xFF,
} __packed HidconfigLedTag;

typedef enum {
  INPUT_MODE_GET = 0x01,
  INPUT_MODE_SET = 0x02, 

  GET_SERIAL_NUMBER = 0x10,

  RELOAD_DATA = 0x80,
  SAVE_DATA   = 0x81,

  ROLLER_SET_OFFSET = 0xA0,
  ROLLER_GET_DATA   = 0xA1,

  LED_SET_MODE = 0xB0,

  SP_LED_SET         = 0xE0,
  SP_INPUT_GET       = 0xE1,
  SP_INPUT_GET_START = 0xE2,
  SP_INPUT_GET_END   = 0xE3,

  UPDATE_FIRMWARE = 0xF1,
  CMD_NOT_SUPPORT = 0xFF,
} __packed HidconfigCommand;

typedef struct {
  HidconfigReportID reportID;
  uint8_t symbol;
  HidconfigCommand command;
  uint8_t state;
  union {
    uint8_t payload[60];
    struct
    {
      uint8_t device_mode;
    };
    struct {
      uint16_t roller_value;
      uint16_t roller_raw_value;
    };
    struct
    {
      HidconfigLedTag led_tag;
      uint8_t led_index;
      uint8_t mode;
      uint8_t brightness;
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t _dammy_[53];
    };
    struct
    {
      uint8_t board_id;
      struct
      {
        uint8_t led_rgb_left[6][3];
        uint8_t led_rgb_right[6][3];
      };
    };
    struct
    {
      uint16_t roller_value_sp;
      uint16_t input_status;
    };
  };
} __packed HidconfigData;

#define STATE_OK     0x01
#define STATE_ACTIVE 0x10
#define STATE_ERROR  0xFF

void HIDCONFIG_Receive_Handler();

extern uint8_t HIDCFG_Buffer_OUT[64];
extern uint8_t HIDCFG_Buffer_IN[64];

#endif
