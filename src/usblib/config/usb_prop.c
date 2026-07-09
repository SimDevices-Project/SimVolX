#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

uint8_t Request = 0;

volatile uint8_t HID_Idle_Value[4]     = {0};
volatile uint8_t HID_Protocol_Value[4] = {0};

DEVICE Device_Table =
    {
        EP_NUM,
        1};

DEVICE_PROP Device_Property =
    {
        USBD_init,
        USBD_Reset,
        USBD_Status_In,
        USBD_Status_Out,
        USBD_Data_Setup,
        USBD_NoData_Setup,
        USBD_Get_Interface_Setting,
        USBD_GetDeviceDescriptor,
        USBD_GetConfigDescriptor,
        USBD_GetStringDescriptor_MutiLang,
        0,
        DEF_USBD_UEP0_SIZE};

USER_STANDARD_REQUESTS User_Standard_Requests =
    {
        USBD_GetConfiguration,
        USBD_SetConfiguration,
        USBD_GetInterface,
        USBD_SetInterface,
        USBD_GetStatus,
        USBD_ClearFeature,
        USBD_SetEndPointFeature,
        USBD_SetDeviceFeature,
        USBD_SetDeviceAddress};

ONE_DESCRIPTOR Device_Descriptor =
    {
        (uint8_t *)USBD_DeviceDescriptor,
        USBD_SIZE_DEVICE_DESC};

ONE_DESCRIPTOR Config_Descriptor =
    {
        (uint8_t *)USBD_ConfigDescriptor,
        USBD_SIZE_CONFIG_DESC};

ONE_DESCRIPTOR String_Descriptor[USBD_NUMOF_STRING_DESC] =
    {
        {(uint8_t *)USBD_StringLangID, USBD_SIZE_STRING_LANGID},
        {(uint8_t *)USBD_StringVendor, USBD_SIZE_STRING_VENDOR},
        {(uint8_t *)USBD_StringProduct, USBD_SIZE_STRING_PRODUCT},
        {(uint8_t *)USBD_StringSerial, USBD_SIZE_STRING_SERIAL},
        {(uint8_t *)USBD_StringConfig, USBD_SIZE_STRING_CONFIG},
        {(uint8_t *)USBD_StringHIDIO, USBD_SIZE_STRING_HIDIO},
        {(uint8_t *)USBD_StringCustomHID, USBD_SIZE_STRING_CUSTOM_HID},
        {(uint8_t *)USBD_StringBtnA, USBD_SIZE_STRING_BTN_A},
        {(uint8_t *)USBD_StringBtnB, USBD_SIZE_STRING_BTN_B},
        {(uint8_t *)USBD_StringBtnC, USBD_SIZE_STRING_BTN_C},
        {(uint8_t *)USBD_StringBtnD, USBD_SIZE_STRING_BTN_D},
        {(uint8_t *)USBD_StringBtnFXL, USBD_SIZE_STRING_BTN_FXL},
        {(uint8_t *)USBD_StringBtnFXR, USBD_SIZE_STRING_BTN_FXR},
        {(uint8_t *)USBD_StringBtnStart, USBD_SIZE_STRING_BTN_START},
        {(uint8_t *)USBD_StringBtnTest, USBD_SIZE_STRING_BTN_TEST},
        {(uint8_t *)USBD_StringBtnService, USBD_SIZE_STRING_BTN_SERVICE},
        {(uint8_t *)USBD_StringBtnCoin, USBD_SIZE_STRING_BTN_COIN},
        {(uint8_t *)USBD_StringVolLeft, USBD_SIZE_STRING_VOL_LEFT},
        {(uint8_t *)USBD_StringVolRight, USBD_SIZE_STRING_VOL_RIGHT},
};

MUTI_LANG_DESCRIPTOR Muti_Lang_String_Descriptor[USBD_NUMOF_STRING_DESC] =
    {
        {
            {(uint8_t *)USBD_StringLangID, USBD_SIZE_STRING_LANGID},
            {(uint8_t *)USBD_StringLangID, USBD_SIZE_STRING_LANGID},
            {(uint8_t *)USBD_StringLangID, USBD_SIZE_STRING_LANGID},
            {(uint8_t *)USBD_StringLangID, USBD_SIZE_STRING_LANGID},
        },
        {
            {(uint8_t *)USBD_StringVendor, USBD_SIZE_STRING_VENDOR},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringProduct, USBD_SIZE_STRING_PRODUCT},
            {(uint8_t *)USBD_StringProduct_zh_hans, USBD_SIZE_STRING_PRODUCT_zh_hans},
            {(uint8_t *)USBD_StringProduct_zh_hant, USBD_SIZE_STRING_PRODUCT_zh_hant},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringSerial, USBD_SIZE_STRING_SERIAL},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringConfig, USBD_SIZE_STRING_CONFIG},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringHIDIO, USBD_SIZE_STRING_HIDIO},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringCustomHID, USBD_SIZE_STRING_CUSTOM_HID},
            {(uint8_t *)USBD_StringCustomHID_zh_hans, USBD_SIZE_STRING_CUSTOM_HID_zh_hans},
            {(uint8_t *)USBD_StringCustomHID_zh_hant, USBD_SIZE_STRING_CUSTOM_HID_zh_hant},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnA, USBD_SIZE_STRING_BTN_A},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnB, USBD_SIZE_STRING_BTN_B},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnC, USBD_SIZE_STRING_BTN_C},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnD, USBD_SIZE_STRING_BTN_D},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnFXL, USBD_SIZE_STRING_BTN_FXL},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnFXR, USBD_SIZE_STRING_BTN_FXR},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnStart, USBD_SIZE_STRING_BTN_START},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnTest, USBD_SIZE_STRING_BTN_TEST},
            {(uint8_t *)USBD_StringBtnTest_zh_hans, USBD_SIZE_STRING_BTN_TEST_zh_hans},
            {(uint8_t *)USBD_StringBtnTest_zh_hant, USBD_SIZE_STRING_BTN_TEST_zh_hant},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnService, USBD_SIZE_STRING_BTN_SERVICE},
            {(uint8_t *)USBD_StringBtnService_zh_hans, USBD_SIZE_STRING_BTN_SERVICE_zh_hans},
            {(uint8_t *)USBD_StringBtnService_zh_hant, USBD_SIZE_STRING_BTN_SERVICE_zh_hant},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringBtnCoin, USBD_SIZE_STRING_BTN_COIN},
            {(uint8_t *)USBD_StringBtnCoin_zh_hans, USBD_SIZE_STRING_BTN_COIN_zh_hans},
            {(uint8_t *)USBD_StringBtnCoin_zh_hant, USBD_SIZE_STRING_BTN_COIN_zh_hant},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringVolLeft, USBD_SIZE_STRING_VOL_LEFT},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
        {
            {(uint8_t *)USBD_StringVolRight, USBD_SIZE_STRING_VOL_RIGHT},
            {NULL, 0},
            {NULL, 0},
            {NULL, 0},
        },
};

ONE_DESCRIPTOR Report_Descriptor[4] =
    {
        {(uint8_t *)USBD_HidRepDesc, USBD_SIZE_REPORT_DESC},
        {(uint8_t *)USBD_Hid2Desc, USBD_SIZE_HID2_REPORT_DESC},
        {(uint8_t *)USBD_Hid3Desc, USBD_SIZE_HID3_REPORT_DESC},
        {(uint8_t *)USBD_HidCustomDesc, USBD_SIZE_CUSTOM_REPORT_DESC},
};

ONE_DESCRIPTOR Hid_Descriptor[4] =
    {
        {(uint8_t *)&USBD_ConfigDescriptor[18], 0x09},
        {(uint8_t *)&USBD_ConfigDescriptor[50], 0x09},
        {(uint8_t *)&USBD_ConfigDescriptor[82], 0x09},
        {(uint8_t *)&USBD_ConfigDescriptor[114], 0x09},
};

void USBD_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0) {
    bDeviceState = CONFIGURED;
  }
}

void USBD_SetDeviceAddress(void)
{
  bDeviceState = ADDRESSED;
}

void USBD_SetDeviceFeature(void)
{
}

void USBD_ClearFeature(void)
{
}

void USBD_Status_In(void)
{
}

void USBD_Status_Out(void)
{
}

void USBD_init(void)
{
  uint8_t i;

  pInformation->Current_Configuration = 0;
  PowerOn();
  for (i = 0; i < 8; i++) _SetENDPOINT(i, _GetENDPOINT(i) & 0x7F7F & EPREG_MASK);
  _SetISTR((uint16_t)0x00FF);
  USB_SIL_Init();
  bDeviceState = UNCONNECTED;

  USB_Port_Set(DISABLE, DISABLE);
  Delay_Ms(20);
  USB_Port_Set(ENABLE, ENABLE);
}

void USBD_Reset(void)
{
  pInformation->Current_Configuration = 0;
  pInformation->Current_Feature       = USBD_ConfigDescriptor[7];
  pInformation->Current_Interface     = 0;

  SetBTABLE(BTABLE_ADDRESS);

  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxStatus(ENDP0, EP_RX_VALID);
  _ClearDTOG_RX(ENDP0);
  _ClearDTOG_TX(ENDP0);

  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPRxAddr(ENDP1, ENDP1_RXADDR);
  SetEPRxCount(ENDP1, DEF_USBD_MAX_PACK_SIZE);
  SetEPRxStatus(ENDP1, EP_RX_VALID);
  _ClearDTOG_TX(ENDP1);
  _ClearDTOG_RX(ENDP1);

  SetEPType(ENDP2, EP_INTERRUPT);
  SetEPTxStatus(ENDP2, EP_TX_NAK);
  SetEPTxAddr(ENDP2, ENDP2_TXADDR);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, ENDP2_PACKET_SIZE);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
  _ClearDTOG_RX(ENDP2);
  _ClearDTOG_TX(ENDP2);

  SetEPType(ENDP3, EP_INTERRUPT);
  SetEPTxStatus(ENDP3, EP_TX_NAK);
  SetEPTxAddr(ENDP3, ENDP3_TXADDR);
  SetEPRxAddr(ENDP3, ENDP3_RXADDR);
  SetEPRxCount(ENDP3, ENDP3_PACKET_SIZE);
  SetEPRxStatus(ENDP3, EP_RX_VALID);
  _ClearDTOG_RX(ENDP3);
  _ClearDTOG_TX(ENDP3);

  SetEPType(ENDP4, EP_INTERRUPT);
  SetEPTxStatus(ENDP4, EP_TX_NAK);
  SetEPTxAddr(ENDP4, ENDP4_TXADDR);
  SetEPRxAddr(ENDP4, ENDP4_RXADDR);
  SetEPRxCount(ENDP4, DEF_USBD_MAX_PACK_SIZE);
  SetEPRxStatus(ENDP4, EP_RX_VALID);
  _ClearDTOG_TX(ENDP4);
  _ClearDTOG_RX(ENDP4);

  SetDeviceAddress(0);

  bDeviceState = ATTACHED;
}

uint8_t *USBD_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

uint8_t *USBD_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

uint8_t *USBD_GetStringDescriptor(uint16_t Length)
{
  uint8_t stringIndex = pInformation->USBwValue0;

  if (stringIndex >= USBD_NUMOF_STRING_DESC) {
    return NULL;
  } else {
    return Standard_GetDescriptorData(Length, &String_Descriptor[stringIndex]);
  }
}

uint8_t *USBD_GetStringDescriptor_MutiLang(uint16_t Length)
{
  uint8_t stringIndex = pInformation->USBwValue0;
  uint16_t stringID   = pInformation->USBwIndex;

  if (stringIndex >= USBD_NUMOF_STRING_DESC) {
    return NULL;
  } else {
    switch (stringID) {
      case 0x0408:
      case 0x0410:
        if (Muti_Lang_String_Descriptor[stringIndex].__zh_hans.Descriptor_Size == 0) {
          return Standard_GetDescriptorData(Length, &Muti_Lang_String_Descriptor[stringIndex].__en_us);
        }
        return Standard_GetDescriptorData(Length, &Muti_Lang_String_Descriptor[stringIndex].__zh_hans);
        break;
      case 0x0404:
      case 0x040c:
      case 0x0414:
        if (Muti_Lang_String_Descriptor[stringIndex].__zh_hant.Descriptor_Size == 0) {
          return Standard_GetDescriptorData(Length, &Muti_Lang_String_Descriptor[stringIndex].__en_us);
        }
        return Standard_GetDescriptorData(Length, &Muti_Lang_String_Descriptor[stringIndex].__zh_hant);
        break;
      case 0x1104:
        if (Muti_Lang_String_Descriptor[stringIndex].__ja_jp.Descriptor_Size == 0) {
          return Standard_GetDescriptorData(Length, &Muti_Lang_String_Descriptor[stringIndex].__en_us);
        }
        return Standard_GetDescriptorData(Length, &Muti_Lang_String_Descriptor[stringIndex].__ja_jp);
        break;
      case 0x0904:
        return Standard_GetDescriptorData(Length, &(Muti_Lang_String_Descriptor[stringIndex].__en_us));
        break;
      default:
        return Standard_GetDescriptorData(Length, &(Muti_Lang_String_Descriptor[stringIndex].__en_us));
    }
  }
}

uint8_t *USBD_GetReportDescriptor(uint16_t Length)
{
  uint8_t wIndex0 = pInformation->USBwIndex0;

  if (wIndex0 >= USBD_NUMOF_REPORT_DESC) {
    return NULL;
  } else {
    return Standard_GetDescriptorData(Length, &Report_Descriptor[wIndex0]);
  }
}

uint8_t *USBD_GetHidDescriptor(uint16_t Length)
{
  uint8_t wIndex0 = pInformation->USBwIndex0;

  if (wIndex0 >= USBD_NUMOF_REPORT_DESC) {
    return NULL;
  } else {
    return Standard_GetDescriptorData(Length, &Hid_Descriptor[wIndex0]);
  }
}

RESULT USBD_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0) {
    return USB_UNSUPPORT;
  } else if (Interface > 3) {
    return USB_UNSUPPORT;
  }

  return USB_SUCCESS;
}

RESULT USBD_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);
  uint8_t wValue1;
  CopyRoutine = NULL;
  wValue1     = pInformation->USBwValue1;
  if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) {
    if (wValue1 == HID_REPORT_DESCRIPTOR) {
      CopyRoutine = USBD_GetReportDescriptor;
    } else if (wValue1 == HID_DESCRIPTOR) {
      CopyRoutine = USBD_GetHidDescriptor;
    }
    if (CopyRoutine) {
      pInformation->Ctrl_Info.CopyData = CopyRoutine;
      (*CopyRoutine)(0);
    } else {
      return USB_UNSUPPORT;
    }
  } else {
    return USB_UNSUPPORT;
  }
  if (CopyRoutine) {
    pInformation->Ctrl_Info.CopyData    = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);
  } else {
    return (USB_UNSUPPORT);
  }
  return USB_SUCCESS;
}

RESULT USBD_NoData_Setup(uint8_t RequestNo)
{
  return USB_UNSUPPORT;
}
