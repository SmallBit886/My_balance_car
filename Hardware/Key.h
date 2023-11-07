#ifndef __KEY_H
#define __KEY_H

#define KEY1_PIN		GPIO_Pin_1
#define KEY1_GPIO		GPIOB
#define KEY1_Set		GPIO_SetBits(GPIOB,GPIO_Pin_1);
#define KEY1_Reset		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
#define KEY1_Read_IO	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)

#define KEY2_PIN		GPIO_Pin_14
#define KEY2_GPIO		GPIOB
#define KEY2_Set		GPIO_SetBits(GPIOB,GPIO_Pin_14);
#define KEY2_Reset		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
#define KEY2_Read_IO	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)

uint8_t Key_GetNum(void);

void Key_Init(void);




#endif
