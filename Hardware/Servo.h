#ifndef __SERVO_H
#define __SERVO_H

#define Servo_PIN		GPIO_Pin_0
#define Servo_GPIO		GPIOA
#define Servo_Set		GPIO_SetBits(GPIOA,GPIO_Pin_0);
#define Servo_Reset		GPIO_ResetBits(GPIOA,GPIO_Pin_0);

void Servo_TIM2_Init(void);
void Servo_SetAngle(float Angle);


#endif
