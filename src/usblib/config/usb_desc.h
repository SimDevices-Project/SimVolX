#ifndef __USB_DESC_H
#define __USB_DESC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32v20x.h"

#define VENDOR_ID_H                       0x80
#define VENDOR_ID_L                       0x88
#define PRODUCT_ID_H                      0x00
#define PRODUCT_ID_L                      0x22

// 0x0021 SimGEKI
// 0x0022 SimVolX

#define PRODUCT_BCD_H                     0x01
#define PRODUCT_BCD_L                     0x00

#define USB_DEVICE_DESCRIPTOR_TYPE        0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE 0x02
#define USB_STRING_DESCRIPTOR_TYPE        0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE     0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE      0x05

#define USB_IAD_DESCRIPTOR_TYPE           0x0B

#define DEF_USBD_UEP0_SIZE                16
#define DEF_USBD_MAX_PACK_SIZE            64

#define USBD_SIZE_DEVICE_DESC             18
#define USBD_SIZE_CONFIG_DESC             137

#define USBD_SIZE_REPORT_DESC             272
#define USBD_SIZE_HID2_REPORT_DESC        27
#define USBD_SIZE_HID3_REPORT_DESC        27
#define USBD_SIZE_CUSTOM_REPORT_DESC      27

#define USBD_NUMOF_REPORT_DESC            4

#define USBD_NUMOF_STRING_DESC            7

#define USBD_SIZE_STRING_LANGID           16
#define USBD_SIZE_STRING_VENDOR           22
#define USBD_SIZE_STRING_PRODUCT          38
#define USBD_SIZE_STRING_SERIAL           50
#define USBD_SIZE_STRING_CONFIG           22
#define USBD_SIZE_STRING_HIDIO            28
#define USBD_SIZE_STRING_CUSTOM_HID       30

#define USBD_SIZE_STRING_PRODUCT_zh_hans  24
#define USBD_SIZE_STRING_CUSTOM_HID_zh_hans 30

extern uint8_t USBD_DeviceDescriptor[USBD_SIZE_DEVICE_DESC];
extern const uint8_t USBD_ConfigDescriptor[USBD_SIZE_CONFIG_DESC];

extern const uint8_t USBD_HidRepDesc[USBD_SIZE_REPORT_DESC];
extern const uint8_t USBD_Hid2Desc[USBD_SIZE_HID2_REPORT_DESC];
extern const uint8_t USBD_Hid3Desc[USBD_SIZE_HID3_REPORT_DESC];
extern const uint8_t USBD_HidCustomDesc[USBD_SIZE_CUSTOM_REPORT_DESC];

extern const uint8_t USBD_StringLangID[USBD_SIZE_STRING_LANGID];
extern const uint8_t USBD_StringVendor[USBD_SIZE_STRING_VENDOR];
extern uint8_t USBD_StringSerial[USBD_SIZE_STRING_SERIAL];

extern const uint8_t USBD_StringProduct[USBD_SIZE_STRING_PRODUCT];
extern const uint8_t USBD_StringProduct_zh_hans[USBD_SIZE_STRING_PRODUCT_zh_hans];

extern const uint8_t USBD_StringConfig[USBD_SIZE_STRING_CONFIG];

extern const uint8_t USBD_StringHIDIO[USBD_SIZE_STRING_HIDIO];

extern const uint8_t USBD_StringCustomHID[USBD_SIZE_STRING_CUSTOM_HID];
extern const uint8_t USBD_StringCustomHID_zh_hans[USBD_SIZE_STRING_CUSTOM_HID_zh_hans];

void USBD_SerialNumUpdate();
void USBD_DeviceDescriptorUpdate();

#ifdef __cplusplus
}
#endif

#endif /* __USB_DESC_H */
