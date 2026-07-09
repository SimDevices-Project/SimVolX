#include "pwm.h"

void PWM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
  TIM_OCInitTypeDef TIM_OCInitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  TIM_TimeBaseStructure.TIM_Period        = PWM_MAX_BRIGHTNESS - 1;
  TIM_TimeBaseStructure.TIM_Prescaler     = 144 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;
  TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
}

void PWM_SetBrightness(PwmLedIndex led, uint16_t brightness)
{
  if (brightness > PWM_MAX_BRIGHTNESS) {
    brightness = PWM_MAX_BRIGHTNESS;
  }

  switch (led) {
    case PWM_LED_BTA:
      TIM_SetCompare1(TIM3, brightness);
      break;
    case PWM_LED_BTB:
      TIM_SetCompare2(TIM3, brightness);
      break;
    case PWM_LED_BTC:
      TIM_SetCompare3(TIM3, brightness);
      break;
    case PWM_LED_BTD:
      TIM_SetCompare4(TIM3, brightness);
      break;
    case PWM_LED_FXL:
      TIM_SetCompare2(TIM1, brightness);
      break;
    case PWM_LED_FXR:
      TIM_SetCompare1(TIM1, brightness);
      break;
    case PWM_LED_START:
      TIM_SetCompare3(TIM1, brightness);
      break;
    default:
      break;
  }
}
