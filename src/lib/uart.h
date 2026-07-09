#ifndef __UART_H_
#define __UART_H_

#include "bsp.h"

void UART_Init(void);
void UART_SendByte(uint8_t data);
void UART_SendData(uint8_t *data, uint8_t len);

#endif // !__UART_H_
