#include "sys.h"

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/**
  * @brief  中断优先级
  * @param  
  * @retval 
  * @attention  配置MPU6050外部中断为最高，因为要时刻采集数据维持平衡车
  */
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4级抢占,4级响应。
	
	//配置MPU6050外部中断
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能该通道中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//主优先级0，最高
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//子优先级，最高
	NVIC_Init(&NVIC_InitStruct);//初始化
	
	//配置串口中断
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//串口中断
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能该通道中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//主优先级2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//子优先级2
	NVIC_Init(&NVIC_InitStruct);	//初始化
}
