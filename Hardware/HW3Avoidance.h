#ifndef __HW3AVOIDANCE_H
#define __HW3AVOIDANCE_H

#define HW_L_PIN		GPIO_Pin_13
#define HW_L_GPIO		GPIOC
#define HW_L_Set		GPIO_SetBits(GPIOC,GPIO_Pin_13);
#define HW_L_Reset		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
#define HW_L_Read_IO	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)

#define HW_R_PIN		GPIO_Pin_14
#define HW_R_GPIO		GPIOC
#define HW_R_Set		GPIO_SetBits(GPIOC,GPIO_Pin_14);
#define HW_R_Reset		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
#define HW_R_Read_IO	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)

#define HW_M_PIN		GPIO_Pin_15
#define HW_M_GPIO		GPIOC
#define HW_M_Set		GPIO_SetBits(GPIOC,GPIO_Pin_15);
#define HW_M_Reset		GPIO_ResetBits(GPIOC,GPIO_Pin_15);
#define HW_M_Read_IO	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)

void HW_Avoidance_Init(void);

void HW_Avoidance(void);

#endif
