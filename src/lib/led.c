#include "led.h"
#include "timeout.h"
#include "ledmanager.h"

#define WS2812_FREQ         800000
#define TIMER_CLOCK_FREQ    144000000

#define WS2812_TIME_0BIT    45
#define WS2812_TIME_1BIT    115

#define BUFF_FRONT_OFFSET   5
#define BUFF_END_OFFSET     80

#define TIMER_PER_BUFF_SIZE (BUFF_FRONT_OFFSET + RGB_COUNT_PER_PORT * 24 + BUFF_END_OFFSET)
#define TIMER_PERIOD        ((TIMER_CLOCK_FREQ / WS2812_FREQ) - 1)

uint32_t colorList[RGB_PORT_COUNT][RGB_COUNT_PER_PORT] = {0};
uint8_t colorPWM[RGB_PORT_COUNT][TIMER_PER_BUFF_SIZE]  = {0};

void setRgbColor32(uint8_t port, uint8_t index, uint32_t color)
{
  colorList[port][index] = color;
  for (uint8_t i = 0; i < 24; i++) {
    colorPWM[port][BUFF_FRONT_OFFSET + index * 24 + i] = ((color >> (23 - i)) & 0x01) ? WS2812_TIME_1BIT : WS2812_TIME_0BIT;
  }
}

void setRgbColor(uint8_t port, uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
  if (index >= RGB_COUNT_PER_PORT) {
    return;
  }
  setRgbColor32(port, index, ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b);
}

void setRgbColorAll(uint8_t r, uint8_t g, uint8_t b)
{
  for (uint8_t i = 0; i < RGB_PORT_COUNT; i++) {
    for (uint8_t j = 0; j < RGB_COUNT_PER_PORT; j++) {
      setRgbColor(i, j, r, g, b);
    }
  }
}

void setRgbColorPort(uint8_t port, uint8_t r, uint8_t g, uint8_t b)
{
  for (uint8_t i = 0; i < RGB_COUNT_PER_PORT; i++) {
    setRgbColor(port, i, r, g, b);
  }
}

xdata void Timer4_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  TIM_InternalClockConfig(TIM4);

  TIM_PrescalerConfig(TIM4, 0, TIM_PSCReloadMode_Immediate);
  TIM_CounterModeConfig(TIM4, TIM_CounterMode_Up);
  TIM_SetAutoreload(TIM4, TIMER_PERIOD);
  TIM_SetClockDivision(TIM4, TIM_CKD_DIV1);

  TIM_OC2PolarityConfig(TIM4, TIM_OCPolarity_High);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_OC3PolarityConfig(TIM4, TIM_OCPolarity_High);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_SetCompare2(TIM4, 0);
  TIM_SelectOCxM(TIM4, TIM_Channel_2, TIM_OCMode_PWM1);
  TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable);

  TIM_SetCompare3(TIM4, 0);
  TIM_SelectOCxM(TIM4, TIM_Channel_3, TIM_OCMode_PWM1);
  TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable);

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize         = TIMER_PER_BUFF_SIZE;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

  // LED_LEFT (PB7), TIM4 CH2, DMA1 Channel2
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM4->CH2CVR);
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)colorPWM[LED_LEFT];
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel2, ENABLE);

  // LED_RIGHT (PB8), TIM4 CH3, DMA1 Channel3
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM4->CH3CVR);
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)colorPWM[LED_RIGHT];
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel3, ENABLE);

  TIM_DMACmd(TIM4, TIM_DMA_CC2 | TIM_DMA_CC3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
}

void WS2812_Refresh()
{
  if (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET) { return; }
  if (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET) { return; }

  DMA_ClearFlag(DMA1_FLAG_TC2);
  DMA_ClearFlag(DMA1_FLAG_TC3);

  TIM_Cmd(TIM4, DISABLE);

  DMA_Cmd(DMA1_Channel2, DISABLE);
  DMA_Cmd(DMA1_Channel3, DISABLE);

  DMA_SetCurrDataCounter(DMA1_Channel2, TIMER_PER_BUFF_SIZE);
  DMA_SetCurrDataCounter(DMA1_Channel3, TIMER_PER_BUFF_SIZE);

  DMA_Cmd(DMA1_Channel2, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);

  TIM_Cmd(TIM4, ENABLE);
}

xdata void WS2812_Init(void)
{
  setRgbColorAll(0, 0, 0);
  for (uint8_t i = 0; i < RGB_PORT_COUNT; i++) {
    colorPWM[i][TIMER_PER_BUFF_SIZE - 1] = 0;
  }

  Timer4_Config();
}

xdata void LED_Init_RGB()
{
  WS2812_Init();
}

xdata void LED_Effect_Red()
{
  LED_RGB_SetPort(LED_LEFT, 0xFF, 0x00, 0x00);
  LED_RGB_SetPort(LED_RIGHT, 0xFF, 0x00, 0x00);
}

xdata void LED_Effect_Green()
{
  LED_RGB_SetPort(LED_LEFT, 0x00, 0xFF, 0x00);
  LED_RGB_SetPort(LED_RIGHT, 0x00, 0xFF, 0x00);
}

xdata void LED_Effect_Blue()
{
  LED_RGB_SetPort(LED_LEFT, 0x00, 0x00, 0xFF);
  LED_RGB_SetPort(LED_RIGHT, 0x00, 0x00, 0xFF);
}

xdata void LED_Effect_White()
{
  LED_RGB_SetPort(LED_LEFT, 0xFF, 0xFF, 0xFF);
  LED_RGB_SetPort(LED_RIGHT, 0xFF, 0xFF, 0xFF);
}

xdata void LED_Effect_Normal()
{
  LED_RGB_SetPort(LED_LEFT, 0xFF, 0x00, 0xFF);
  LED_RGB_SetPort(LED_RIGHT, 0xFF, 0x00, 0xFF);
}

xdata void LED_Effect_Close()
{
  LED_RGB_SetPort(LED_LEFT, 0x00, 0x00, 0x00);
  LED_RGB_SetPort(LED_RIGHT, 0x00, 0x00, 0x00);
}

xdata void LED_Animation_PowerOn()
{
  setTimeout(LED_Effect_Red, 500);
  setTimeout(LED_Effect_Green, 1000);
  setTimeout(LED_Effect_Blue, 1500);
  setTimeout(LED_Effect_White, 2000);
  setTimeout(LED_Effect_Normal, 3000);
}

xdata void LED_Init()
{
  LED_Init_RGB();
  PWM_Init();
}

void LED_Refresh()
{
  LEDManager_Handle();
  WS2812_Refresh();
}

void LED_RGB_Set(LED_RGB_Port port, uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
  setRgbColor(port, index, r, g, b);
}

void LED_RGB_SetAll(uint8_t r, uint8_t g, uint8_t b)
{
  setRgbColorAll(r, g, b);
}

void LED_RGB_SetPort(LED_RGB_Port port, uint8_t r, uint8_t g, uint8_t b)
{
  setRgbColorPort(port, r, g, b);
}

void LED_PWM_SetBrightness(PwmLedIndex led, uint16_t brightness)
{
  PWM_SetBrightness(led, brightness);
}
