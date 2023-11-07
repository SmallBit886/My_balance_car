#if KEY
#include "sys.h"
void HW_Avoidance_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = HW_L_PIN | HW_R_PIN | HW_M_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HW_M_GPIO, &GPIO_InitStructure);	
	
}

void HW_Avoidance(void)
{		
		CarGo();
		
		if (HW_L_Read_IO == 0 || HW_M_Read_IO == 0)
		{
			CarRight();
	
		}
		if (HW_R_Read_IO == 0 || HW_M_Read_IO == 0)
		{
			CarLeft();
		}
		if (HW_M_Read_IO == 0 )
		{
			CarBack();
			Delay_ms(500);
			CarRight();
			Delay_ms(400);
		}
		if (HW_R_Read_IO == 0 && HW_L_Read_IO == 0 && HW_M_Read_IO == 0)
		{
			CarBack();
			Delay_ms(500);
			CarLeft();
			Delay_ms(400);
		}
		

}
#endif

