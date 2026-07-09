#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_conf.h"

uint8_t USBD_DeviceDescriptor[] = {
    USBD_SIZE_DEVICE_DESC, // bLength
    0x01,                  // bDescriptorType
    0x10, 0x01,            // bcdUSB
    0x00,                  // bDeviceClass (defined at interface level)
    0x00,                  // bDeviceSubClass
    0x00,                  // bDeviceProtocol
    DEF_USBD_UEP0_SIZE,    // bMaxPacketSize0
    0, 0,                  // VENDOR_ID
    0, 0,                  // PRODUCT_ID
    0, 0,                  // PRODUCT_BCD
    0x01,                  // iManufacturer
    0x02,                  // iProduct
    0x03,                  // iSerialNumber
    0x01,                  // bNumConfigurations
};

xdata_rodata const uint8_t USBD_ConfigDescriptor[USBD_SIZE_CONFIG_DESC] = {
    /* Configuration Descriptor */
    0x09,                                                     // bLength
    0x02,                                                     // bDescriptorType
    USBD_SIZE_CONFIG_DESC & 0xFF, USBD_SIZE_CONFIG_DESC >> 8, // wTotalLength
    0x04,                                                     // bNumInterfaces
    0x01,                                                     // bConfigurationValue
    0x04,                                                     // iConfiguration
    0x80,                                                     // bmAttributes: Bus Powered
    0xfa,                                                     // MaxPower: 500mA

    /* interface 0 (HID joystick) descriptor */
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x00, // bInterfaceNumber 0
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x03, // bInterfaceClass (HID)
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol
    0x05, // iInterface (String Index: HIDIO)

    /* interface 0 HID descriptor */
    0x09,                                                     // bLength
    0x21,                                                     // bDescriptorType
    0x11, 0x01,                                               // bcdHID
    0x00,                                                     // bCountryCode
    0x01,                                                     // bNumDescriptors
    0x22,                                                     // bDescriptorType
    USBD_SIZE_REPORT_DESC & 0xFF, USBD_SIZE_REPORT_DESC >> 8, // wDescriptorLength

    /* interface 0 endpoint descriptor EP1_IN */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x81,                    // bEndpointAddress (IN)
    0x03,                    // bmAttributes (Interrupt)
    ENDP1_PACKET_SIZE, 0x00, // wMaxPacketSize 64
    0x01,                    // bInterval 1

    /* interface 0 endpoint descriptor EP1_OUT */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x01,                    // bEndpointAddress (OUT)
    0x03,                    // bmAttributes (Interrupt)
    ENDP1_PACKET_SIZE, 0x00, // wMaxPacketSize 64
    0x05,                    // bInterval 5

    /* interface 1 (HID EP2) descriptor */
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x01, // bInterfaceNumber 1
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x03, // bInterfaceClass (HID)
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol
    0x00, // iInterface

    /* interface 1 HID descriptor */
    0x09,                                                        // bLength
    0x21,                                                        // bDescriptorType
    0x11, 0x01,                                                  // bcdHID
    0x00,                                                        // bCountryCode
    0x01,                                                        // bNumDescriptors
    0x22,                                                        // bDescriptorType
    USBD_SIZE_HID2_REPORT_DESC & 0xFF, USBD_SIZE_HID2_REPORT_DESC >> 8, // wDescriptorLength

    /* interface 1 endpoint descriptor EP2_IN */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x82,                    // bEndpointAddress (IN)
    0x03,                    // bmAttributes (Interrupt)
    ENDP2_PACKET_SIZE, 0x00, // wMaxPacketSize
    0x05,                    // bInterval 5

    /* interface 1 endpoint descriptor EP2_OUT */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x02,                    // bEndpointAddress (OUT)
    0x03,                    // bmAttributes (Interrupt)
    ENDP2_PACKET_SIZE, 0x00, // wMaxPacketSize
    0x05,                    // bInterval 5

    /* interface 2 (HID EP3) descriptor */
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x02, // bInterfaceNumber 2
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x03, // bInterfaceClass (HID)
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol
    0x00, // iInterface

    /* interface 2 HID descriptor */
    0x09,                                                        // bLength
    0x21,                                                        // bDescriptorType
    0x11, 0x01,                                                  // bcdHID
    0x00,                                                        // bCountryCode
    0x01,                                                        // bNumDescriptors
    0x22,                                                        // bDescriptorType
    USBD_SIZE_HID3_REPORT_DESC & 0xFF, USBD_SIZE_HID3_REPORT_DESC >> 8, // wDescriptorLength

    /* interface 2 endpoint descriptor EP3_IN */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x83,                    // bEndpointAddress (IN)
    0x03,                    // bmAttributes (Interrupt)
    ENDP3_PACKET_SIZE, 0x00, // wMaxPacketSize
    0x05,                    // bInterval 5

    /* interface 2 endpoint descriptor EP3_OUT */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x03,                    // bEndpointAddress (OUT)
    0x03,                    // bmAttributes (Interrupt)
    ENDP3_PACKET_SIZE, 0x00, // wMaxPacketSize
    0x05,                    // bInterval 5

    /* interface 3 (HID config) descriptor */
    0x09, // bLength
    0x04, // bDescriptorType (Interface)
    0x03, // bInterfaceNumber 3
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x03, // bInterfaceClass (HID)
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol
    0x06, // iInterface (String Index: CustomHID)

    /* interface 3 HID descriptor */
    0x09,                                                                   // bLength
    0x21,                                                                   // bDescriptorType
    0x11, 0x01,                                                             // bcdHID
    0x00,                                                                   // bCountryCode
    0x01,                                                                   // bNumDescriptors
    0x22,                                                                   // bDescriptorType
    USBD_SIZE_CUSTOM_REPORT_DESC & 0xFF, USBD_SIZE_CUSTOM_REPORT_DESC >> 8, // wDescriptorLength

    /* interface 3 endpoint descriptor EP4_IN */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x84,                    // bEndpointAddress (IN)
    0x03,                    // bmAttributes (Interrupt)
    ENDP4_PACKET_SIZE, 0x00, // wMaxPacketSize 64
    0x05,                    // bInterval 5

    /* interface 3 endpoint descriptor EP4_OUT */
    0x07,                    // bLength
    0x05,                    // bDescriptorType (Endpoint)
    0x04,                    // bEndpointAddress (OUT)
    0x03,                    // bmAttributes (Interrupt)
    ENDP4_PACKET_SIZE, 0x00, // wMaxPacketSize 64
    0x05,                    // bInterval 5
};

/* USB String Descriptors */
xdata_rodata const uint8_t USBD_StringLangID[USBD_SIZE_STRING_LANGID] = {
    USBD_SIZE_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x04, 0x08, // LangID = 0x0804
    0x04, 0x10, // LangID = 0x1004
    0x04, 0x0C, // LangID = 0x0C04
    0x04, 0x04, // LangID = 0x0404
    0x04, 0x14, // LangID = 0x1404
    0x11, 0x04, // LangID = 0x0411
    0x09, 0x04, // LangID = 0x0409
};

xdata_rodata const uint8_t USBD_StringVendor[USBD_SIZE_STRING_VENDOR] = {
    USBD_SIZE_STRING_VENDOR,
    USB_STRING_DESCRIPTOR_TYPE,
    'S', 0, 'E', 0, 'G', 0, 'A', 0};

xdata_rodata const uint8_t USBD_StringProduct[USBD_SIZE_STRING_PRODUCT] = {
    USBD_SIZE_STRING_PRODUCT,
    USB_STRING_DESCRIPTOR_TYPE,
    0x53, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x47, 0x00, 0x45, 0x00, 0x4b, 0x00, 0x49, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x74, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x72, 0x00};

xdata_rodata const uint8_t USBD_StringProduct_zh_hans[USBD_SIZE_STRING_PRODUCT_zh_hans] = {
    USBD_SIZE_STRING_PRODUCT_zh_hans,
    USB_STRING_DESCRIPTOR_TYPE,
    0x53, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x47, 0x00, 0x45, 0x00, 0x4b, 0x00, 0x49, 0x00, 0x57, 0x88, 0x3a, 0x67, 0xce, 0x98, 0x3c, 0x68, 0xa7, 0x63, 0x36, 0x52, 0x68, 0x56};

uint8_t USBD_StringSerial[USBD_SIZE_STRING_SERIAL] = {
    USBD_SIZE_STRING_SERIAL,
    USB_STRING_DESCRIPTOR_TYPE,
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
};

xdata_rodata const uint8_t USBD_StringConfig[USBD_SIZE_STRING_CONFIG] = {
    USBD_SIZE_STRING_CONFIG,
    USB_STRING_DESCRIPTOR_TYPE,
    'S', 0, 'i', 0, 'm', 0, 'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0, 's', 0};

xdata_rodata const uint8_t USBD_StringHIDIO[USBD_SIZE_STRING_HIDIO] = {
    USBD_SIZE_STRING_HIDIO,
    USB_STRING_DESCRIPTOR_TYPE,
    'I', 0, '/', 0, 'O', 0, ' ', 0, 'C', 0, 'O', 0, 'N', 0, 'T', 0, 'R', 0, 'O', 0, 'L', 0, ' ', 0, 'B', 0, 'D', 0, ';', 0,
    '1', 0, '5', 0, '2', 0, '5', 0, '7', 0, ';', 0,
    '0', 0, '1', 0, ';', 0,
    '9', 0, '0', 0, ';', 0,
    '1', 0, '8', 0, '3', 0, '1', 0, ';', 0,
    '6', 0, '6', 0, '7', 0, '9', 0, 'A', 0, ';',
    0, '0', 0, '0', 0, ';', 0,

    'G', 0, 'O', 0, 'U', 0, 'T', 0, '=', 0, '1', 0, '4', 0, '_', 0,
    'A', 0, 'D', 0, 'I', 0, 'N', 0, '=', 0, '8', 0, ',', 0, 'E', 0, '_', 0,
    'R', 0, 'O', 0, 'T', 0, 'I', 0, 'N', 0, '=', 0, '4', 0, '_', 0,
    'C', 0, 'O', 0, 'I', 0, 'N', 0, 'I', 0, 'N', 0, '=', 0, '2', 0, '_', 0,
    'S', 0, 'W', 0, 'I', 0, 'N', 0, '=', 0, '2', 0, ',', 0, 'E', 0, '_', 0,
    'U', 0, 'Q', 0, '1', 0, '=', 0, '4', 0, '1', 0, ',', 0, '6', 0};

xdata_rodata const uint8_t USBD_StringCustomHID[USBD_SIZE_STRING_CUSTOM_HID] = {
    USBD_SIZE_STRING_CUSTOM_HID,
    USB_STRING_DESCRIPTOR_TYPE,
    'S', 0, 'i', 0, 'm', 0, 'G', 0, 'E', 0, 'K', 0, 'I', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0, 'f', 0, 'i', 0, 'g', 0};

xdata_rodata const uint8_t USBD_StringCustomHID_zh_hans[USBD_SIZE_STRING_CUSTOM_HID_zh_hans] = {
    USBD_SIZE_STRING_CUSTOM_HID_zh_hans,
    USB_STRING_DESCRIPTOR_TYPE,
    0x53, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x47, 0x00, 0x45, 0x00, 0x4b, 0x00, 0x49, 0x00, 0xbe, 0x8b, 0x6e, 0x7f};

/* HID Report Descriptor for EP1 (Joystick) */
xdata_rodata const uint8_t USBD_HidRepDesc[USBD_SIZE_REPORT_DESC] =
{
    0x05, 0x01,                   // Usage Page (Generic Desktop Ctrls)
    0x09, 0x04,                   // Usage (Joystick)
    0xA1, 0x01,                   // Collection (Application)
    0x85, 0x01,                   //   Report ID (1)
    0x09, 0x01,                   //   Usage (Pointer)
    0xA1, 0x00,                   //   Collection (Physical)
    0x09, 0x30,                   //     Usage (X)
    0x09, 0x31,                   //     Usage (Y)
    0x09, 0x30,                   //     Usage (X)
    0x09, 0x31,                   //     Usage (Y)
    0x09, 0x30,                   //     Usage (X)
    0x09, 0x31,                   //     Usage (Y)
    0x09, 0x30,                   //     Usage (X)
    0x09, 0x31,                   //     Usage (Y)
    0x09, 0x33,                   //     Usage (Rx)
    0x09, 0x34,                   //     Usage (Ry)
    0x09, 0x33,                   //     Usage (Rx)
    0x09, 0x34,                   //     Usage (Ry)
    0x09, 0x36,                   //     Usage (Slider)
    0x09, 0x36,                   //     Usage (Slider)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     Logical Maximum (65535)
    0x35, 0x00,                   //     Physical Minimum (0)
    0x47, 0xFF, 0xFF, 0x00, 0x00, //     Physical Maximum (65535)
    0x95, 0x0E,                   //     Report Count (14)
    0x75, 0x10,                   //     Report Size (16)
    0x81, 0x02,                   //     Input (Data,Var,Abs)
    0xC0,                         //   End Collection
    0x05, 0x02,                   //   Usage Page (Sim Ctrls)
    0x05, 0x09,                   //   Usage Page (Button)
    0x19, 0x01,                   //   Usage Minimum (0x01)
    0x29, 0x30,                   //   Usage Maximum (0x30)
    0x15, 0x00,                   //   Logical Minimum (0)
    0x25, 0x01,                   //   Logical Maximum (1)
    0x45, 0x01,                   //   Physical Maximum (1)
    0x75, 0x01,                   //   Report Size (1)
    0x95, 0x30,                   //   Report Count (48)
    0x81, 0x02,                   //   Input (Data,Var,Abs)
    0x09, 0x00,                   //   Usage (0x00)
    0x75, 0x08,                   //   Report Size (8)
    0x95, 0x1D,                   //   Report Count (29)
    0x81, 0x01,                   //   Input (Const,Array,Abs)
    0x06, 0xA0, 0xFF,             //   Usage Page (Vendor Defined 0xFFA0)
    0x09, 0x00,                   //   Usage (0x00)
    0x85, 0x10,                   //   Report ID (16)
    0xA1, 0x01,                   //   Collection (Application)
    0x09, 0x00,                   //     Usage (0x00)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x26, 0xFF, 0x00,             //     Logical Maximum (255)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x3F,                   //     Report Count (63)
    0x91, 0x02,                   //     Output (Data,Var,Abs)
    0xC0,                         //   End Collection
    0xC0,                         // End Collection

    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
    0x85, 0x21, //   Report ID (0x21)
    0x09, 0x01, //   Usage (Pointer)
    0xA1, 0x00, //   Collection (Physical)
    0x05, 0x09, //     Usage Page (Button)
    0x19, 0x01, //     Usage Minimum (0x01)
    0x29, 0x05, //     Usage Maximum (0x05)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x75, 0x01, //     Report Size (1)
    0x95, 0x05, //     Report Count (5)
    0x81, 0x02, //     Input (Data,Var,Abs)
    0x75, 0x03, //     Report Size (3)
    0x95, 0x01, //     Report Count (1)
    0x81, 0x01, //     Input (Const,Array,Abs)
    0x05, 0x01, //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30, //     Usage (X)
    0x09, 0x31, //     Usage (Y)
    0x09, 0x38, //     Usage (Wheel)
    0x15, 0x81, //     Logical Minimum (-127)
    0x25, 0x7F, //     Logical Maximum (127)
    0x75, 0x08, //     Report Size (8)
    0x95, 0x03, //     Report Count (3)
    0x81, 0x06, //     Input (Data,Var,Rel)
    0xC0,       //   End Collection
    0xC0,       // End Collection

    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,       // Usage (Mouse)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x22,       //   Report ID (0x22)
    0x09, 0x01,       //   Usage (Pointer)
    0xA1, 0x00,       //   Collection (Physical)
    0x05, 0x01,       //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,       //     Usage (X)
    0x09, 0x31,       //     Usage (Y)
    0x15, 0x00,       //     Logical Minimum (0)
    0x26, 0xFF, 0xFF, //     Logical Maximum (65535)
    0x35, 0x00,       //     Physical Minimum (0)
    0x46, 0xFF, 0xFF, //     Physical Maximum (65535)
    0x75, 0x10,       //     Report Size (16)
    0x95, 0x02,       //     Report Count (2)
    0x81, 0x02,       //     Input (Data, Var, Abs)
    0xC0,             //   End Collection
    0xC0,             // End Collection

    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,       // Usage (Keyboard)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x31,       //   Report ID (0x31)
    0x05, 0x07,       //   Usage Page (Kbrd/Keypad)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x19, 0xE0,       //   Usage Minimum (0xE0)
    0x29, 0xE7,       //   Usage Maximum (0xE7)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x08,       //   Report Count (8)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x05, 0x07,       //   Usage Page (Kbrd/Keypad)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x19, 0x00,       //   Usage Minimum (0x00)
    0x29, 0x91,       //   Usage Maximum (0x91)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x10,       //   Report Count (16)
    0x81, 0x00,       //   Input (Data,Array,Abs)
    0xC0,             // End Collection

    0x05, 0x0C,       // Usage Page (Consumer)
    0x09, 0x01,       // Usage (Consumer Control)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x32,       //   Report ID (0x32)
    0x05, 0x0C,       //   Usage Page (Consumer)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x7F, //   Logical Maximum (32767)
    0x19, 0x00,       //   Usage Minimum (0x00)
    0x2A, 0xFF, 0x02, //   Usage Maximum (0x2FF)
    0x75, 0x10,       //   Report Size (16)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x00,       //   Input (Data,Array,Abs)
    0xC0,             // End Collection
};

/* HID Report Descriptor for EP2 */
xdata_rodata const uint8_t USBD_Hid2Desc[USBD_SIZE_HID2_REPORT_DESC] = {
    0x06, 0x00, 0xFF, // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,       // Usage (Vendor Usage 1)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x02,       // Report ID (2)
    0x15, 0x00,       // Logical Minimum (0)
    0x26, 0xFF, 0x00, // Logical Maximum (255)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x1F,       // Report Count (31)
    0x09, 0x02,       // Usage (Vendor Usage 2)
    0x81, 0x02,       // Input (Data,Var,Abs)
    0x09, 0x03,       // Usage (Vendor Usage 3)
    0x91, 0x02,       // Output (Data,Var,Abs)
    0xC0              // End Collection
};

/* HID Report Descriptor for EP3 */
xdata_rodata const uint8_t USBD_Hid3Desc[USBD_SIZE_HID3_REPORT_DESC] = {
    0x06, 0x00, 0xFF, // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,       // Usage (Vendor Usage 1)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x03,       // Report ID (3)
    0x15, 0x00,       // Logical Minimum (0)
    0x26, 0xFF, 0x00, // Logical Maximum (255)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x1F,       // Report Count (31)
    0x09, 0x02,       // Usage (Vendor Usage 2)
    0x81, 0x02,       // Input (Data,Var,Abs)
    0x09, 0x03,       // Usage (Vendor Usage 3)
    0x91, 0x02,       // Output (Data,Var,Abs)
    0xC0              // End Collection
};

/* HID Report Descriptor for EP4 (Config) */
xdata_rodata const uint8_t USBD_HidCustomDesc[USBD_SIZE_CUSTOM_REPORT_DESC] = {
    0x06, 0x00, 0xFF, // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,       // Usage (Vendor Usage 1)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xAA,       // Report ID (170 0xAA)
    0x15, 0x00,       // Logical Minimum (0)
    0x26, 0xFF, 0x00, // Logical Maximum (255)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x3F,       // Report Count (63)
    0x09, 0x02,       // Usage (Vendor Usage 2)
    0x81, 0x02,       // Input (Data,Var,Abs)
    0x09, 0x03,       // Usage (Vendor Usage 3)
    0x91, 0x02,       // Output (Data,Var,Abs)
    0xC0              // End Collection
};

xdata void _IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
  for (uint8_t idx = 0; idx < len; idx++) {
    if (((value >> 28)) < 0xA) {
      pbuf[2 * idx] = (value >> 28) + '0';
    } else {
      pbuf[2 * idx] = (value >> 28) + 'A' - 10;
    }
    value             = value << 4;
    pbuf[2 * idx + 1] = 0;
  }
}

xdata void USBD_SerialNumUpdate()
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
  Device_Serial0 = *(uint32_t *)0x1FFFF7E8;
  Device_Serial1 = *(uint32_t *)0x1FFFF7EC;
  Device_Serial2 = *(uint32_t *)0x1FFFF7F0;

  if (Device_Serial0 != 0) {
    _IntToUnicode(Device_Serial0, &USBD_StringSerial[2], 8);
    _IntToUnicode(Device_Serial1, &USBD_StringSerial[18], 8);
    _IntToUnicode(Device_Serial2, &USBD_StringSerial[34], 8);
  }
}

xdata void USBD_DeviceDescriptorUpdate()
{
  USBD_DeviceDescriptor[8]  = VENDOR_ID_L;
  USBD_DeviceDescriptor[9]  = VENDOR_ID_H;
  USBD_DeviceDescriptor[10] = PRODUCT_ID_L;
  USBD_DeviceDescriptor[11] = PRODUCT_ID_H;
  USBD_DeviceDescriptor[12] = PRODUCT_BCD_L;
  USBD_DeviceDescriptor[13] = PRODUCT_BCD_H;
}
