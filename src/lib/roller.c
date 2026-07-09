#include "bsp.h"
#include "roller.h"
#include "data.h"

#define VALUE_OFFSET_MASK      0xFFFF
#define VALUE_DEFAULT          0x8000

#define _offset                GlobalData->RollerOffset

#define ADC_DISCARD_SAMPLES 8
#define ADC_VALID_SAMPLES   64
#define ADC_TOTAL_SAMPLES   (ADC_DISCARD_SAMPLES + ADC_VALID_SAMPLES)

uint16_t ADCValue[ADC_TOTAL_SAMPLES * 2] = {0};

volatile uint16_t EncoderValue  = 0;
volatile uint16_t EncoderValue2 = 0;

#define DEBOUNCE_LENGTH  8
#define DEBOUNCE_LIMIT_A 0x0100
#define DEBOUNCE_LIMIT_B 0x0040

typedef struct {
  uint16_t outputValue;
  uint16_t debounceBuffer[DEBOUNCE_LENGTH];
  uint32_t debounceSumValue;
  uint16_t debounceAvgValue;
  uint8_t debounceIndex;
} DebounceState;

void DMA_Tx_Init_ADC(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
  DMA_InitTypeDef DMA_InitStructure = {0};

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA_CHx);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
  DMA_InitStructure.DMA_MemoryBaseAddr     = memadr;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = bufsize;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA_CHx, &DMA_InitStructure);
}

xdata void Roller_ADC_Init()
{
  ADC_InitTypeDef ADC_InitStructure   = {0};
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);

  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 2;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_55Cycles5);

  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  ADC_BufferCmd(ADC1, DISABLE);
  ADC_ResetCalibration(ADC1);
  while (ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while (ADC_GetCalibrationStatus(ADC1));

  DMA_Tx_Init_ADC(DMA1_Channel1, (u32)&ADC1->RDATAR, (u32)&ADCValue, ADC_TOTAL_SAMPLES * 2);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  DMA_Cmd(DMA1_Channel1, ENABLE);
}

xdata void Roller_Init()
{
  Roller_ADC_Init();
}

void Roller_Update()
{
  if (!DMA_GetFlagStatus(DMA1_FLAG_TC1)) {
    return;
  }
  DMA_ClearFlag(DMA1_FLAG_TC1);

  uint8_t i;
  uint32_t sum1 = 0;
  uint32_t sum2 = 0;
  for (i = 0; i < ADC_VALID_SAMPLES; i++) {
    sum1 += ADCValue[(ADC_DISCARD_SAMPLES + i) * 2];
    sum2 += ADCValue[(ADC_DISCARD_SAMPLES + i) * 2 + 1];
  }

  EncoderValue = (sum1 >> 6);
  EncoderValue = ~EncoderValue;
  EncoderValue <<= 4;

  EncoderValue2 = (sum2 >> 6);
  EncoderValue2 = ~EncoderValue2;
  EncoderValue2 <<= 4;

  DMA_Cmd(DMA1_Channel1, DISABLE);
  DMA_SetCurrDataCounter(DMA1_Channel1, ADC_TOTAL_SAMPLES * 2);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

uint16_t Roller_GetRawValue()
{
  return EncoderValue;
}

uint16_t Roller_GetRawValue2()
{
  return EncoderValue2;
}

static uint16_t applyOffsetDebounce(uint16_t rawVal, DebounceState *state)
{
  uint8_t refreshFlag = 0;

  if ((rawVal > state->debounceAvgValue ? rawVal - state->debounceAvgValue : state->debounceAvgValue - rawVal) > DEBOUNCE_LIMIT_A) {
    refreshFlag = 1;
  }
  if (refreshFlag == 1) {
    for (uint8_t i = 0; i < DEBOUNCE_LENGTH; i++) {
      state->debounceBuffer[i] = rawVal;
    }
    state->debounceAvgValue  = rawVal;
    state->debounceSumValue  = rawVal * DEBOUNCE_LENGTH;
  } else {
    state->debounceSumValue -= state->debounceBuffer[state->debounceIndex];
    state->debounceBuffer[state->debounceIndex] = rawVal;
    state->debounceSumValue += rawVal;
    state->debounceIndex    = (state->debounceIndex + 1) % DEBOUNCE_LENGTH;
    state->debounceAvgValue = state->debounceSumValue / DEBOUNCE_LENGTH;
  }
  if ((state->outputValue > state->debounceAvgValue ? state->outputValue - state->debounceAvgValue : state->debounceAvgValue - state->outputValue) > DEBOUNCE_LIMIT_B) {
    state->outputValue = state->debounceAvgValue;
  }

  uint16_t finalOutputValue;

  if (state->outputValue <= VALUE_OFFSET_MASK - _offset) {
    finalOutputValue = state->outputValue + _offset;
  } else {
    finalOutputValue = ((_offset + state->outputValue) & VALUE_OFFSET_MASK);
  }

  return finalOutputValue;
}

uint16_t Roller_GetValue()
{
  static DebounceState state = {0};
  return applyOffsetDebounce(Roller_GetRawValue(), &state);
}

uint16_t Roller_GetValue2()
{
  static DebounceState state = {0};
  return applyOffsetDebounce(Roller_GetRawValue2(), &state);
}

void Roller_ResetOffset()
{
  uint16_t newVal;
  uint16_t rawVal = Roller_GetRawValue();
  if (rawVal <= VALUE_DEFAULT) {
    newVal = VALUE_DEFAULT - rawVal;
  } else {
    newVal = VALUE_OFFSET_MASK - rawVal + VALUE_DEFAULT + 1;
  }
  _offset = newVal;
}
