#include "sys.h"
/**
  * @brief  ADC1单次转换，非扫描转换模式
  * @param  
  * @retval 
  * @attention  
  */
void AD_Init(void)
{
	/*****************************开启时钟******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/****************************ADCCLK分频器*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频；72MHZ/6=12MHZ
	
	/*****************************引脚模式配置******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************************规则组输入通道配置******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	//规则组输入通道
	
	/****************************ADC结构体初始化*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换选择：内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//单次转换	//需要不断触发
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//非扫描转换模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//通道数目为1~16之间
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);		//开启ADC电源
	/*****************************ADC进行校准******************************/
	ADC_ResetCalibration(ADC1);		//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//获取校准状态
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启软件触发转换
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//获取标志位状态；规则组转换完成标志位（55.5+12.5=68T/12M=5.6um）
	return ADC_GetConversionValue(ADC1);		//获取转换的结果
}

/**
  * @brief  ADC1连续转换，非扫描转换模式
  * @param  
  * @retval 
  * @attention  
  */
#if 0
void AD_Init(void)
{
	/*****************************开启时钟******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/****************************ADCCLK分频器*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频；72MHZ/6=12MHZ
	
	/*****************************引脚模式配置******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************************规则组输入通道配置******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	//规则组输入通道
	
	/****************************ADC结构体初始化*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换选择：内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//单次转换	//需要不断触发
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//非扫描转换模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//通道数目为1~16之间
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);		//开启ADC电源
	/*****************************ADC进行校准******************************/
	ADC_ResetCalibration(ADC1);		//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//获取校准状态
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启一次软件触发转换即可
}

uint16_t AD_GetValue(void)
{
	return ADC_GetConversionValue(ADC1);		//获取转换的结果
}
#endif

/**
  * @brief  ADC1单次转换，非扫描转换模式，实现多通道转换
  * @param  
  * @retval 
  * @attention  
  */
#if 0
void AD_Init(void)
{
	/*****************************开启时钟******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/****************************ADCCLK分频器*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频；72MHZ/6=12MHZ
	
	/*****************************引脚模式配置******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*****************************规则组输入通道配置放到读取函数里******************************/
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	//规则组输入通道
	
	/****************************ADC结构体初始化*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换选择：内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//单次转换	//需要不断触发
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//非扫描转换模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//通道数目为1~16之间
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);		//开启ADC电源
	/*****************************ADC进行校准******************************/
	ADC_ResetCalibration(ADC1);		//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//获取校准状态
	
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	/*****************************规则组输入通道配置放到读取函数里******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启软件触发转换
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//获取标志位状态；规则组转换完成标志位（55.5+12.5=68T/12M=5.6um）
	return ADC_GetConversionValue(ADC1);		//获取转换的结果
}
#endif

/**
  * @brief  ADC1单次扫描模式，DMA单次转运模式
  * @param  
  * @retval 
  * @attention  
  */
#if 0
uint16_t AD_Value[5];//存储的位置
void AD_DMA_Init(void)
{
	/*****************************开启时钟******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*****************************结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	/****************************ADCCLK分频器*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频；72MHZ/6=12MHZ
	
	/*****************************引脚模式配置******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*****************************规则组输入通道配置放到读取函数里******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_55Cycles5);	//规则组输入通道
	/****************************ADC结构体初始化*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换选择：内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//单次转换	//需要不断触发
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//扫描转换模式
	ADC_InitStructure.ADC_NbrOfChannel = 5;					//通道数目为1~16之间
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_DMACmd(ADC1,ENABLE);	//ADC——DMA使能
	ADC_Cmd(ADC1, ENABLE);		//开启ADC电源

	/***************************DMA参数配置********************************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//外设站点的起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度，半字，取DR寄存器的低16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//是否自增，不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//存储器站点的起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度，字节
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//是否自增，自增
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向：外设站点到存储器
	DMA_InitStructure.DMA_BufferSize = 5;//缓存区大小；其实就是传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//传输模式；其实就是是否使用自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//是否是存储器到存储器；其实就是是否是硬件触发，还是软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化
	
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能
	/*****************************ADC进行校准******************************/
	ADC_ResetCalibration(ADC1);		//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//获取校准状态
	
}

void  AD_DMA_GetValue(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//失能
	DMA_SetCurrDataCounter(DMA1_Channel1, 5);//给传输计数器赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启软件触发转换
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待转运完成标志位置1
	DMA_ClearFlag(DMA1_FLAG_TC1);//清除转运完成标志位
}
#endif
/**
  * @brief  ADC1连续扫描模式，DMA连续循环转运模式
  * @param  
  * @retval 
  * @attention  
  */
#if 1
uint16_t AD_Value[5];//存储的位置
void AD_DMA_Init(void)
{
	/*****************************开启时钟******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*****************************结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	/****************************ADCCLK分频器*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频；72MHZ/6=12MHZ
	
	/*****************************引脚模式配置******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*****************************规则组输入通道配置放到读取函数里******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);	//规则组输入通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_55Cycles5);	//规则组输入通道
	/****************************ADC结构体初始化*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换选择：内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//连续转换	//不需要不断触发
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//扫描转换模式
	ADC_InitStructure.ADC_NbrOfChannel = 5;					//通道数目为1~16之间
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_DMACmd(ADC1,ENABLE);	//ADC——DMA使能
	ADC_Cmd(ADC1, ENABLE);		//开启ADC电源

	/***************************DMA参数配置********************************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//外设站点的起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度，半字，取DR寄存器的低16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//是否自增，不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//存储器站点的起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度，字节
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//是否自增，自增
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向：外设站点到存储器
	DMA_InitStructure.DMA_BufferSize = 5;//缓存区大小；其实就是传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//传输模式；循环模式；使用自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//是否是存储器到存储器；其实就是是否是硬件触发，还是软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化
	
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能
	/*****************************ADC进行校准******************************/
	ADC_ResetCalibration(ADC1);		//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//判断校准是否完成（准备下次校准）
	ADC_StartCalibration(ADC1);		//启动校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//获取校准状态
	/***************************直接放在初始化最后一行********************************/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//开启软件触发转换
}
/*******直接不需要了
void  AD_DMA_GetValue(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//失能
	DMA_SetCurrDataCounter(DMA1_Channel1, 5);//给传输计数器赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待转运完成标志位置1
	DMA_ClearFlag(DMA1_FLAG_TC1);//清除转运完成标志位
}*/
#endif
