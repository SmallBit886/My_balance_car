#include "sys.h" 

float Pitch,Roll,Yaw;						//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;						//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int PWM_MAX=7200,PWM_MIN=-7200;	//PWM限幅变量
int MOTO1,MOTO2;								//电机装载变量

extern int Vertical_out,Velocity_out,Turn_out;
int main(void)	
{

	NVIC_Config();
	Usart1_Init(115200);
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	Motor_Init();
	TIM1_Init(0,7199);
  while(1)	
	{
		OLED_Float(0,10,Pitch,1);
		OLED_Float(2,20,Roll,1);
		OLED_Float(4,10,Encoder_Left,1);
		OLED_Float(6,20,Encoder_Right,1);
//		OLED_ShowNumber(30,0,Velocity_out,3,16);//OK
	} 	
}


