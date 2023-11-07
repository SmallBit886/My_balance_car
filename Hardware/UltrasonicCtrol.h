#ifndef __ULTRASONICCTROL_H_
#define __ULTRASONICCTROL_H_

#define Echo_PIN		GPIO_Pin_6
#define Echo_GPIO		GPIOB
#define Echo_Set		GPIO_SetBits(GPIOB,GPIO_Pin_6);
#define Echo_Reset		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
#define Echo_Read_IO	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)

#define Trig_PIN		GPIO_Pin_1
#define Trig_GPIO		GPIOA
#define Trig_Set		GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define Trig_Reset		GPIO_ResetBits(GPIOA,GPIO_Pin_1);

extern volatile unsigned int distance_cm;//µ±«∞æ‡¿Î
void UltraSound_Init(void);
void GetDistanceDelay(void);


#endif
