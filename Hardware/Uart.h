#ifndef __UART_H
#define __UART_H
#include <stdio.h>
#include "sys.h"
void Usart1_Init(u32 bound);
		  
void Usart1_SendByte(uint8_t Byte);
void Usart1_SendArray(uint8_t *Array, uint16_t Length);
void Usart1_SendString(char *String);
void Usart1_SendNumber(uint32_t Number, uint8_t Length);
void Usart1_Printf(char *format, ...);

#endif
