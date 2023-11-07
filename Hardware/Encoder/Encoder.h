#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"                  // Device header

void Encoder_TIM2_Init(void);
void Encoder_TIM3_Init(void);
int16_t Encoder_Get(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
int16_t Read_Speed(int TIMx);

#endif
