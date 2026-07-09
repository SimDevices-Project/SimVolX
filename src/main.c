/*
 * SimGEKI
 * Copyright (c) 2024 SimDevices, Handle
 */

#include "bsp.h"
#include "timeout.h"

#include "data.h"

#include "led.h"
#include "hidio.h"
#include "uart.h"
#include "sleep.h"

#include "usb_lib.h"

#include "debug.h"

int main(void)
{
  SystemInit();
  SystemCoreClockUpdate();

  RCC_PCLK2Config(RCC_HCLK_Div2);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  Delay_Init();

  Delay_Ms(50);

  LoadData();
  Set_USBConfig();
  USB_Interrupts_Config();
  USB_Init();

  Delay_Ms(10);

  Timeout_Init();
  Sleep_Init();

  LED_Init();
  HIDIO_Init();
  UART_Init();

  LED_Animation_PowerOn();

  setInterval(LED_Refresh, 20);
  setInterval(HIDIO_Update, 1);

  while (1) {
    Timer_Process();
  }
}
