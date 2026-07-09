#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "usb_prop.h"

#include "hidio.h"
#include "hidconfig.h"

uint8_t HID2_Buffer_OUT[64] = {0x00};
uint8_t HID2_Buffer_IN[64]  = {0x00};

uint8_t HID3_Buffer_OUT[64] = {0x00};
uint8_t HID3_Buffer_IN[64]  = {0x00};

void EP1_IN_Callback(void)
{
}

void EP1_OUT_Callback(void)
{
  if (USB_SIL_Read(EP1_OUT, HID_Buffer_OUT) == ENDP1_PACKET_SIZE) {
    HIDIO_Receive_Handler();
  }
  SetEPRxValid(ENDP1);
}

void EP4_IN_Callback(void)
{
}

void EP4_OUT_Callback(void)
{
  if (USB_SIL_Read(EP4_OUT, HIDCFG_Buffer_OUT) == ENDP4_PACKET_SIZE) {
    HIDCONFIG_Receive_Handler();
  }
  SetEPRxValid(ENDP4);
}

void EP2_IN_Callback(void)
{
}

void EP2_OUT_Callback(void)
{
  USB_SIL_Read(EP2_OUT, HID2_Buffer_OUT);
  SetEPRxValid(ENDP2);
}

void EP3_IN_Callback(void)
{
}

void EP3_OUT_Callback(void)
{
  USB_SIL_Read(EP3_OUT, HID3_Buffer_OUT);
  SetEPRxValid(ENDP3);
}

uint8_t USBD_ENDPx_DataUp(uint8_t endp, uint8_t *pbuf, uint16_t len)
{
  switch (endp) {
    case ENDP1: {
      if (GetEPTxStatus(ENDP1) == EP_TX_VALID) {
        return USB_ERROR;
      }
      USB_SIL_Write(EP1_IN, pbuf, len);
      SetEPTxStatus(ENDP1, EP_TX_VALID);
      break;
    }
    case ENDP2: {
      if (GetEPTxStatus(ENDP2) == EP_TX_VALID) {
        return USB_ERROR;
      }
      USB_SIL_Write(EP2_IN, pbuf, len);
      SetEPTxStatus(ENDP2, EP_TX_VALID);
      break;
    }
    case ENDP3: {
      if (GetEPTxStatus(ENDP3) == EP_TX_VALID) {
        return USB_ERROR;
      }
      USB_SIL_Write(EP3_IN, pbuf, len);
      SetEPTxStatus(ENDP3, EP_TX_VALID);
      break;
    }
    case ENDP4: {
      if (GetEPTxStatus(ENDP4) == EP_TX_VALID) {
        return USB_ERROR;
      }
      USB_SIL_Write(EP4_IN, pbuf, len);
      SetEPTxStatus(ENDP4, EP_TX_VALID);
      break;
    }
    default:
      return USB_ERROR;
  }
  return USB_SUCCESS;
}
