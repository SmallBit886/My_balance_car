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
