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
        case ROLLER_SET_OFFSET: {
          Roller_ResetOffset();

          dataUpload->command = ROLLER_SET_OFFSET;
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
