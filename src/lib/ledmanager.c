#include "led.h"
#include "keyscan.h"
#include "data.h"

void LEDManager_Handle()
{
  if (GlobalData->LEDManager == 0) {
    return;
  }
  switch (GlobalData->DeviceMode) {
    case 1:
    case 2:
      return;
    case 3: {
      if (KeyScan_GetKeyDebouncedStatus(KEY_BTA)) {
        LED_PWM_SetBrightness(PWM_LED_BTA, PWM_MAX_BRIGHTNESS);
      } else {
        LED_PWM_SetBrightness(PWM_LED_BTA, 0);
      }
      if (KeyScan_GetKeyDebouncedStatus(KEY_BTB)) {
        LED_PWM_SetBrightness(PWM_LED_BTB, PWM_MAX_BRIGHTNESS);
      } else {
        LED_PWM_SetBrightness(PWM_LED_BTB, 0);
      }
      if (KeyScan_GetKeyDebouncedStatus(KEY_BTC)) {
        LED_PWM_SetBrightness(PWM_LED_BTC, PWM_MAX_BRIGHTNESS);
      } else {
        LED_PWM_SetBrightness(PWM_LED_BTC, 0);
      }
      if (KeyScan_GetKeyDebouncedStatus(KEY_BTD)) {
        LED_PWM_SetBrightness(PWM_LED_BTD, PWM_MAX_BRIGHTNESS);
      } else {
        LED_PWM_SetBrightness(PWM_LED_BTD, 0);
      }
      if (KeyScan_GetKeyDebouncedStatus(KEY_FXL)) {
        LED_PWM_SetBrightness(PWM_LED_FXL, PWM_MAX_BRIGHTNESS);
        LED_RGB_SetPort(LED_LEFT, 0xFF, 0x00, 0xFF);
      } else {
        LED_PWM_SetBrightness(PWM_LED_FXL, 0);
        LED_RGB_SetPort(LED_LEFT, 0xFF, 0xFF, 0xFF);
      }
      if (KeyScan_GetKeyDebouncedStatus(KEY_FXR)) {
        LED_PWM_SetBrightness(PWM_LED_FXR, PWM_MAX_BRIGHTNESS);
        LED_RGB_SetPort(LED_RIGHT, 0xFF, 0x00, 0xFF);
      } else {
        LED_PWM_SetBrightness(PWM_LED_FXR, 0);
        LED_RGB_SetPort(LED_RIGHT, 0xFF, 0xFF, 0xFF);
      }
      if (KeyScan_GetKeyDebouncedStatus(KEY_START)) {
        LED_PWM_SetBrightness(PWM_LED_START, PWM_MAX_BRIGHTNESS);
      } else {
        LED_PWM_SetBrightness(PWM_LED_START, 0);
      }
      break;
    }
    default:
      break;
  }
}
