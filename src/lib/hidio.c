#include "hidio.h"
#include "led.h"
#include "keyscan.h"
#include "roller.h"
#include "string.h"
#include "data.h"

#include "timeout.h"

#include "usb_lib.h"
#include "usb_prop.h"

#define OUT_EP ENDP1

uint8_t HID_Buffer_OUT[64] = {0x00};
uint8_t HID_Buffer_IN[64]  = {0x00};

uint8_t HID_SGIO4_Buffer_IN[64] = {0x00};

static DataReceive *dataReceive = (DataReceive *)HID_Buffer_OUT;
static DataUpload *dataUpload   = (DataUpload *)HID_SGIO4_Buffer_IN;

static uint16_t prevKeyStatus    = 0;
static uint16_t activeKeyStatus  = 0;
static uint16_t changedKeyStatus = 0;

static uint16_t prevRollerValue   = 0;
static uint16_t activeRollerValue = 0;

void HIDIO_Receive_Handler()
{
  switch (dataReceive->reportID) {
    case 0x10:
      switch (dataReceive->command) {
        case SET_COMM_TIMEOUT: {
          dataUpload->systemStatus = 0x30;
          break;
        }
        case SET_SAMPLING_COUNT: {
          dataUpload->systemStatus = 0x30;
          break;
        }
        case CLEAR_BOARD_STATUS: {
          dataUpload->systemStatus = 0x00;

          dataUpload->coin[0].count     = 0;
          dataUpload->coin[0].condition = NORMAL;
          dataUpload->coin[1].count     = 0;
          dataUpload->coin[1].condition = NORMAL;
          break;
        }
        case SET_GENERAL_OUTPUT: {
          uint8_t led_bits = dataReceive->payload[0];
          for (uint8_t i = 0; i < PWM_LED_COUNT; i++) {
            if (led_bits & (1 << i)) {
              LED_PWM_SetBrightness(i, PWM_MAX_BRIGHTNESS);
            } else {
              LED_PWM_SetBrightness(i, 0);
            }
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    default: {
      break;
    }
  }
}

uint8_t HIDIO_SGIO4_Upload()
{
  memset(HID_Buffer_IN, 0, 64);
  memcpy(HID_Buffer_IN, HID_SGIO4_Buffer_IN, 64);
  return USBD_ENDPx_DataUp(OUT_EP, HID_Buffer_IN, 64);
}

void HIDIO_Upload()
{
  static uint8_t timeoutID = 0xFF;
  uint8_t usbResult = HIDIO_SGIO4_Upload();
  if (usbResult != USB_SUCCESS) {
    clearTimeout(timeoutID);
    timeoutID = setTimeout(HIDIO_Upload, 1);
  }
}

void HIDIO_SGIO4_FreshData()
{
  dataUpload->analog[0] = activeRollerValue;

  dataUpload->buttons[0] = 0x00;
  dataUpload->buttons[1] = 0x00;
  dataUpload->buttons[2] = 0x00;
  dataUpload->buttons[3] = 0x00;

  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    if (i == KEY_COIN_INDEX) {
      continue;
    }
    if (KeyScan_GetKeyDebouncedStatus(i)) {
      dataUpload->buttons[hid_key_map[i][0]] |= hid_key_map[i][1];
    }
  }

  if ((changedKeyStatus & (1 << KEY_COIN_INDEX)) && KeyScan_GetKeyDebouncedStatus(KEY_COIN_INDEX)) {
    dataUpload->coin[0].count++;
    dataUpload->coin[1].count++;
  }
}

void HIDIO_Update()
{
  uint8_t freshRequired = 0;
  KeyScan_Scan();
  activeKeyStatus = KeyScan_GetAllKeyDebouncedStatus();
  if (activeKeyStatus != prevKeyStatus) {
    freshRequired = 1;
  }
  changedKeyStatus = activeKeyStatus ^ prevKeyStatus;
  prevKeyStatus    = activeKeyStatus;

  Roller_Update();
  activeRollerValue = ~Roller_GetValue();
  if (activeRollerValue != prevRollerValue) {
    freshRequired = 1;
  }
  prevRollerValue = activeRollerValue;

  if (freshRequired) {
    HIDIO_SGIO4_FreshData();
    HIDIO_Upload();
  }
}

xdata void HIDIO_Init()
{
  KeyScan_Init();
  Roller_Init();
  dataUpload->reportID = 0x01;

  dataUpload->analog[0] = 0x8000;
  dataUpload->systemStatus = 0x30;

  HIDIO_SGIO4_FreshData();
}
