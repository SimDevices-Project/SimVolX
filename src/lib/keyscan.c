#include "keyscan.h"

uint8_t KeyTriggerStatus[KEY_COUNT] = {0};

#define KEY_DEBOUNCE_BITS 16

#if KEY_DEBOUNCE_BITS <= 8
uint8_t KeyDebounceStatus[KEY_COUNT] = {0};
#define KEY_DEBOUNCE_MASK 0xFF
#elif KEY_DEBOUNCE_BITS <= 16
uint16_t KeyDebounceStatus[KEY_COUNT] = {0};
#define KEY_DEBOUNCE_MASK 0xFFFF
#else
uint32_t KeyDebounceStatus[KEY_COUNT] = {0};
#define KEY_DEBOUNCE_MASK 0xFFFFFFFF
#endif

uint8_t KeyStatus[KEY_COUNT] = {0};

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} KeyPinDef;

static const KeyPinDef KeyPins[KEY_COUNT] = {
  {GPIOB, GPIO_Pin_6},  // BTA
  {GPIOB, GPIO_Pin_5},  // BTB
  {GPIOB, GPIO_Pin_4},  // BTC
  {GPIOB, GPIO_Pin_3},  // BTD
  {GPIOC, GPIO_Pin_14}, // FXL
  {GPIOC, GPIO_Pin_13}, // FXR
  {GPIOC, GPIO_Pin_15}, // START
  {GPIOA, GPIO_Pin_4},  // COIN
  {GPIOA, GPIO_Pin_1},  // TEST
  {GPIOA, GPIO_Pin_0},  // SRV
};

xdata void KeyScan_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    GPIO_InitStructure.GPIO_Pin = KeyPins[i].pin;
    GPIO_Init(KeyPins[i].port, &GPIO_InitStructure);
  }
}

uint8_t GetKeyTriggerStatus(uint8_t keyIndex)
{
  return KeyTriggerStatus[keyIndex];
}

void DebounceKey(uint8_t keyIndex)
{
  KeyDebounceStatus[keyIndex] <<= 1;
  KeyDebounceStatus[keyIndex] |= GetKeyTriggerStatus(keyIndex);

  if (KeyDebounceStatus[keyIndex] == 0b11 || KeyDebounceStatus[keyIndex] == KEY_DEBOUNCE_MASK) {
    KeyStatus[keyIndex] = 1;
  } else if (KeyDebounceStatus[keyIndex] == KEY_DEBOUNCE_MASK - 0b11 || KeyDebounceStatus[keyIndex] == 0) {
    KeyStatus[keyIndex] = 0;
  }
}

void KeyScan_Scan()
{
  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    if (GPIO_ReadInputDataBit(KeyPins[i].port, KeyPins[i].pin) == Bit_RESET) {
      KeyTriggerStatus[i] = 1;
    } else {
      KeyTriggerStatus[i] = 0;
    }
  }

  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    DebounceKey(i);
  }
}

uint8_t KeyScan_GetKeyDebouncedStatus(uint8_t keyIndex)
{
  return KeyStatus[keyIndex];
}

uint16_t KeyScan_GetAllKeyDebouncedStatus()
{
  uint16_t keyStatus = 0;
  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    keyStatus |= KeyStatus[i] << i;
  }
  return keyStatus;
}
