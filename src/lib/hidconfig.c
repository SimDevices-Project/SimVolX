#include "hidconfig.h"
#include "roller.h"

#include "string.h"

#include "keyscan.h"
#include "data.h"

uint8_t HIDCFG_Buffer_OUT[64] = {0x00};
uint8_t HIDCFG_Buffer_IN[64]  = {0x00};

static HidconfigData *dataReceive = (HidconfigData *)HIDCFG_Buffer_OUT;
static HidconfigData *dataUpload  = (HidconfigData *)HIDCFG_Buffer_IN;

#define HID_CFG_EP ENDP4

uint8_t HIDCONFIG_Upload();
void HIDCONFIG_Receive_Handler();

volatile uint8_t sp_input_state = 0;

void SP_INPUT_Upload()
{
  memset(HIDCFG_Buffer_IN, 0, 64);

  dataUpload->reportID = HIDCONFIG_REPORT_ID;
  dataUpload->symbol   = 0x00;
  dataUpload->command  = SP_INPUT_GET;
  dataUpload->state    = STATE_OK;

  dataUpload->roller_value_sp = Roller_GetValue();
  dataUpload->input_status    = KeyScan_GetAllKeyDebouncedStatus();

  HIDCONFIG_Upload();
}

void HIDCONFIG_Get_SerialNumber(uint8_t *buf)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
  Device_Serial0 = *(uint32_t *)0x1FFFF7E8;
  Device_Serial1 = *(uint32_t *)0x1FFFF7EC;
  Device_Serial2 = *(uint32_t *)0x1FFFF7F0;

  buf[3]  = (Device_Serial0 & 0xFF);
  buf[2]  = ((Device_Serial0 >> 8) & 0xFF);
  buf[1]  = ((Device_Serial0 >> 16) & 0xFF);
  buf[0]  = ((Device_Serial0 >> 24) & 0xFF);
  buf[7]  = (Device_Serial1 & 0xFF);
  buf[6]  = ((Device_Serial1 >> 8) & 0xFF);
  buf[5]  = ((Device_Serial1 >> 16) & 0xFF);
  buf[4]  = ((Device_Serial1 >> 24) & 0xFF);
  buf[11] = (Device_Serial2 & 0xFF);
  buf[10] = ((Device_Serial2 >> 8) & 0xFF);
  buf[9]  = ((Device_Serial2 >> 16) & 0xFF);
  buf[8]  = ((Device_Serial2 >> 24) & 0xFF);
}

void SP_INPUT_OnDataUpdate_Handler()
{
  SP_INPUT_Upload();
}

void HIDCONFIG_Receive_Handler()
{
  switch (dataReceive->reportID) {
    case HIDCONFIG_REPORT_ID: {
      memset(HIDCFG_Buffer_IN, 0, 64);

      dataUpload->reportID = dataReceive->reportID;
      dataUpload->symbol   = dataReceive->symbol;

      switch (dataReceive->command) {
        case RELOAD_DATA: {
          LoadData();
          dataUpload->command = RELOAD_DATA;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case SAVE_DATA: {
          SaveData();
          dataUpload->command = SAVE_DATA;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case ROLLER_GET_DATA: {
          uint16_t rollerValue    = Roller_GetValue();
          uint16_t rollerRawValue = Roller_GetRawValue();
          dataUpload->command          = ROLLER_GET_DATA;
          dataUpload->state            = STATE_OK;
          dataUpload->roller_value     = rollerValue;
          dataUpload->roller_raw_value = rollerRawValue;
          HIDCONFIG_Upload();
          break;
        }
        case ROLLER_SET_OFFSET: {
          Roller_ResetOffset();

          dataUpload->command = ROLLER_SET_OFFSET;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case INPUT_MODE_GET: {
          dataUpload->command     = INPUT_MODE_GET;
          dataUpload->state       = STATE_OK;
          dataUpload->device_mode = GlobalData->DeviceMode;
          HIDCONFIG_Upload();
          break;
        }
        case INPUT_MODE_SET: {
          GlobalData->DeviceMode = dataReceive->device_mode;
          dataUpload->command    = INPUT_MODE_SET;
          dataUpload->state      = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case LED_SET_MODE: {
          if (dataReceive->led_tag == LEDTAG_ALL) {
            LED_RGB_SetAll(dataReceive->r, dataReceive->g, dataReceive->b);
          } else {
            if (dataReceive->led_index == 0xff) {
              LED_RGB_SetPort(dataReceive->led_tag, dataReceive->r, dataReceive->g, dataReceive->b);
            } else {
              LED_RGB_Set(dataReceive->led_tag, dataReceive->led_index, dataReceive->r, dataReceive->g, dataReceive->b);
            }
          }

          dataUpload->command = LED_SET_MODE;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case SP_LED_SET: {
          dataUpload->state   = STATE_OK;
          dataUpload->command = SP_LED_SET;
          switch (dataReceive->board_id) {
            case 0x00:
              LED_RGB_Set(LED_LEFT, 0, dataReceive->led_rgb_left[0][0], dataReceive->led_rgb_left[0][1], dataReceive->led_rgb_left[0][2]);
              LED_RGB_Set(LED_LEFT, 1, dataReceive->led_rgb_left[1][0], dataReceive->led_rgb_left[1][1], dataReceive->led_rgb_left[1][2]);
              LED_RGB_Set(LED_LEFT, 2, dataReceive->led_rgb_left[2][0], dataReceive->led_rgb_left[2][1], dataReceive->led_rgb_left[2][2]);
              LED_RGB_Set(LED_LEFT, 3, dataReceive->led_rgb_left[3][0], dataReceive->led_rgb_left[3][1], dataReceive->led_rgb_left[3][2]);
              LED_RGB_Set(LED_LEFT, 4, dataReceive->led_rgb_left[4][0], dataReceive->led_rgb_left[4][1], dataReceive->led_rgb_left[4][2]);
              LED_RGB_Set(LED_LEFT, 5, dataReceive->led_rgb_left[5][0], dataReceive->led_rgb_left[5][1], dataReceive->led_rgb_left[5][2]);
              LED_RGB_Set(LED_RIGHT, 0, dataReceive->led_rgb_right[0][0], dataReceive->led_rgb_right[0][1], dataReceive->led_rgb_right[0][2]);
              LED_RGB_Set(LED_RIGHT, 1, dataReceive->led_rgb_right[1][0], dataReceive->led_rgb_right[1][1], dataReceive->led_rgb_right[1][2]);
              LED_RGB_Set(LED_RIGHT, 2, dataReceive->led_rgb_right[2][0], dataReceive->led_rgb_right[2][1], dataReceive->led_rgb_right[2][2]);
              LED_RGB_Set(LED_RIGHT, 3, dataReceive->led_rgb_right[3][0], dataReceive->led_rgb_right[3][1], dataReceive->led_rgb_right[3][2]);
              LED_RGB_Set(LED_RIGHT, 4, dataReceive->led_rgb_right[4][0], dataReceive->led_rgb_right[4][1], dataReceive->led_rgb_right[4][2]);
              LED_RGB_Set(LED_RIGHT, 5, dataReceive->led_rgb_right[5][0], dataReceive->led_rgb_right[5][1], dataReceive->led_rgb_right[5][2]);
              break;
            default:
              dataUpload->state = STATE_ERROR;
              break;
          }
          break;
        }
        case SP_INPUT_GET: {
          SP_INPUT_Upload();
          break;
        }
        case SP_INPUT_GET_START: {
          sp_input_state      = 1;
          dataUpload->command = SP_INPUT_GET_START;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case SP_INPUT_GET_END: {
          sp_input_state      = 0;
          dataUpload->command = SP_INPUT_GET_END;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        case GET_SERIAL_NUMBER: {
          HIDCONFIG_Get_SerialNumber(dataUpload->payload);
          dataUpload->command = GET_SERIAL_NUMBER;
          dataUpload->state   = STATE_OK;
          HIDCONFIG_Upload();
          break;
        }
        default: {
          dataUpload->command = CMD_NOT_SUPPORT;
          dataUpload->state   = STATE_ERROR;
          HIDCONFIG_Upload();
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
}

uint8_t HIDCONFIG_Upload()
{
  return USBD_ENDPx_DataUp(HID_CFG_EP, HIDCFG_Buffer_IN, 64);
}
