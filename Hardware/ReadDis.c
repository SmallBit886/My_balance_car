#if KEY
#include "sys.h"
float Direct (void)
{
	float dis1;
	Servo_SetAngle(90);
	Delay_ms(200);
	GetDistanceDelay();
	dis1 = distance_cm;
	OLED_ShowNum(2,4,dis1,3);
	return dis1;
}
float Right (void)
{
	float dis2;
	Servo_SetAngle(45);
	Delay_ms(200);
	GetDistanceDelay();
	dis2 = distance_cm;
	OLED_ShowNum(2,4,dis2,3);
	return dis2;
}
float Left (void)
{
	float dis3;
	Servo_SetAngle(90);
	Delay_ms(200);
	Servo_SetAngle(135);
	Delay_ms(200);
	GetDistanceDelay();
	dis3 = distance_cm;
	OLED_ShowNum(2,4,dis3,3);
	return dis3;
}
void CSB_Avoidance(void)
{
//³¬Éù²¨±ÜÕÏ	`	
		Direct();
		OLED_ShowNum(2,4,Direct(),3);
		if(Direct() > 20)
	{
	    CarGo();
		Delay_ms (200);
	}
	    if(Direct() < 20)
	    {	
			CarStop();
			Delay_ms (200);
			Right();
			OLED_ShowNum(2,4,Right(),3);
			if(Right() > 25)
			{
				CarRight();
				Delay_ms (200);
			}
			else 
			{
				Left();
				OLED_ShowNum(2,4,Left(),3);
				if(Left() > 25)
				{
					CarLeft();
					Delay_ms (200);
			    }
				else{
					CarBack();
					Delay_ms (200);
					CarRight();
					Delay_ms (200);
				}
			}
		}
}
#endif
