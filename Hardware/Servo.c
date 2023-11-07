
#if Servo 
#include "sys.h"
void Servo_TIM2_Init(void)		//舵机初始化
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Servo_PIN;			//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//配置GPIO模式,复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//配置GPIO端口速度
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	Servo_Set;		//默认给高电位
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR   72 000 000 / 72 = 1 000 000 /20 000=50hz   T=1/f=1/50=0.02=20ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC	72000000/72=1 000 000
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR     500/20000 = 0.5ms/20ms(0)  2500/20000 = 2.5ms/20ms(180)
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}
void PWM2_SetCompare1(uint16_t Compare)  //改变占空比
{
	TIM_SetCompare1(TIM2, Compare);
}
void Servo_SetAngle(float Angle)	
	//  y = m*x+n
	//  x:  0 -- y:500   500=m*0+n       n=500
	//  x:180 -- y:2000  2000=m*180+n    m = （2500-500）/180 =2000/180
{
	PWM2_SetCompare1(Angle / 180 * 2000 + 500);

}
#endif
