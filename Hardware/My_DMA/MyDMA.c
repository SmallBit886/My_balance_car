#include "MyDMA.h"

uint16_t MyDMA_Size;
/**
  * @brief  DMA1_Channel1初始化，存储器到存储器转运
  * @param  
  * @retval 3个条件才能让DMA工作
  * @attention  1.传输计数器大于0  2.触发源有触发信号  3.DMA使能
  */
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	/***************************开启时钟********************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/***************************定义结构体变量********************************/
	DMA_InitTypeDef DMA_InitStructure;
	/***************************DMA参数配置********************************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//外设站点的起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//数据宽度，字节
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//存储器站点的起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//数据宽度，字节
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//是否自增
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向：外设站点到存储器
	DMA_InitStructure.DMA_BufferSize = Size;//缓存区大小；其实就是传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//传输模式；其实就是是否使用自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//是否是存储器到存储器；其实就是是否是硬件触发，还是软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化
	
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能
}
/**
  * @brief  DMA传输函数
  * @param  调用一次传输一次
  * @retval 
  * @attention  给DMA赋值要先让其**失能**
  */
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//失能
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);//给传输计数器赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待转运完成标志位置1
	DMA_ClearFlag(DMA1_FLAG_TC1);//清除转运完成标志位
}
