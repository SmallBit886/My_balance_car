#ifndef __REDSENSOR_H
#define __REDSENSOR_H

#define HW_R1_PIN		GPIO_Pin_15
#define HW_R1_GPIO		GPIOA
#define HW_R1_Set		GPIO_SetBits(GPIOA,GPIO_Pin_15);
#define HW_R1_Reset		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
#define HW_R1_Read_IO	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)

#define HW_R2_PIN		GPIO_Pin_5
#define HW_R2_GPIO		GPIOB
#define HW_R2_Set		GPIO_SetBits(GPIOB,GPIO_Pin_5);
#define HW_R2_Reset		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
#define HW_R2_Read_IO	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define HW_L1_PIN		GPIO_Pin_3
#define HW_L1_GPIO		GPIOB
#define HW_L1_Set		GPIO_SetBits(GPIOB,GPIO_Pin_3);
#define HW_L1_Reset		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
#define HW_L1_Read_IO	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)

#define HW_L2_PIN		GPIO_Pin_4
#define HW_L2_GPIO		GPIOB
#define HW_L2_Set		GPIO_SetBits(GPIOB,GPIO_Pin_4);
#define HW_L2_Reset		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
#define HW_L2_Read_IO	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)


void RedSensor_Init(void);
void HW_Trailing(void);

#endif
