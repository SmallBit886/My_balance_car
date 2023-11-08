#include "stm32f10x.h"               
#include "MyI2C.h"
#include "MPU6050_Soft_Reg.h"
#include "MPU6050_Soft.h"

#define MPU6050_ADDRESS		0xD0	//д��ַ
#define soft 1	//��0ΪӲ��IIC����1Ϊ���IIC
#if soft
/***********************************************************
*@fuction	:MPU6050_WriteReg
*@brief		:ָ����ַдһ���ֽ�
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);	//���ʹӻ���ַ
	MyI2C_ReceiveAck();					//����Ӧ�𣻷���Ӧ��λ�����ж�
	MyI2C_SendByte(RegAddress);			//����ָ���Ĵ���������ݣ�MPU6050��ǰ��ַָ����
	MyI2C_ReceiveAck();					//����Ӧ��
	/****************************
	������������ѭ��������д����ֽ�
	****************************/
	MyI2C_SendByte(Data);				//�����ֽڣ�д��ָ���Ĵ�����ַ�µ�����
	MyI2C_ReceiveAck();					//����Ӧ��
	
	MyI2C_Stop();						//��ֹʱ��
}

/***********************************************************
*@fuction	:MPU6050_ReadReg
*@brief		:ָ����ַ��һ���ֽ�
*@param		:--
*@return	:uint8_t Data
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;		//�洢�����ֽ�
	/****************************
	ָ����ַ
	****************************/
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	/****************************
	����ָ����дλ
	****************************/
	MyI2C_Start();								//�ظ���ʼλ
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);		//����0x01 ��д��ַ ��Ϊ ����ַ
	MyI2C_ReceiveAck();
	/****************************
	������������ѭ�������Զ�����ֽ�
	****************************/
	Data = MyI2C_ReceiveByte();					//�����ֽ�
	MyI2C_SendAck(1);							//�����ӻ�Ӧ��
	
	MyI2C_Stop();								//��ֹʱ��
	
	return Data;
}
#else
/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_WaitEvent
  ** @brief         : ��ֹ��ѭ������������ʱ�˳�����
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;//�ɼ���������ܣ����ӡ��
		}
	}
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_WriteReg
  ** @brief         : ָ����ַ��д��Ĵ�������
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : ���ϸ�ʽָ����ַ����
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)//д�Ĵ���
{	

	I2C_GenerateSTART(I2C2, ENABLE);  						//��ʼ������(д��ͽ������˳�������������ʽ�ĺ�����)
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);	//�ȴ�EV5�¼�����ȷ�����溯��ִ�е�λ��
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); //���ʹӻ���ַ
					/***�����ͺͽ��������Դ���Ӧ��***/
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   //�ȴ�EV6�¼�
	
	I2C_SendData(I2C2, RegAddress);								//д��DR���������ݣ��Զ�ת����λ�Ĵ�����
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//�ȴ�EV8�¼�
	
	I2C_SendData(I2C2, Data);									//�������һ���ֽ�Data
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//�ȴ�EV8_2�¼�����λ����ˣ����ٽ��������ˣ����ֽ��Ѿ��������
	
	I2C_GenerateSTOP(I2C2, ENABLE);		//��ֹʱ��
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_ReadReg
  ** @brief         : ָ����ַ�������Ĵ�������
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : ���ϸ�ʽָ����ַ����
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
uint8_t MPU6050_ReadReg(uint8_t RegAddress)//���Ĵ���
{
	uint8_t Data;

	I2C_GenerateSTART(I2C2, ENABLE);  //��ʼ����
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�EV5�¼�
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); //���ʹӻ���ַ
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//�ȴ�EV6�¼�
	
	I2C_SendData(I2C2, RegAddress);//��������
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//�ȴ�EV8�¼�
/******************************************************************************************
	
******************************************************************************************/
	I2C_GenerateSTART(I2C2, ENABLE);//�ظ���ʼ����
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�EV5�¼�
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver); //���ʹӻ���ַ
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); //�ȴ�EV6�¼�
/******************************************************************************************
	�������һ���ֽ�֮ǰ����ǰ����ACK��STOP
******************************************************************************************/
	I2C_AcknowledgeConfig(I2C2, DISABLE);//��ǰ��ACK��0��������Ӧ��
	I2C_GenerateSTOP(I2C2, ENABLE);//��ǰ���������ֹ������STOP=1
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//�ȴ�EV7�¼�����������һ�������Ѿ��浽DR�У�
	Data = I2C_ReceiveData(I2C2);//��ȡDR�����ݣ��浽Data��
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);//ACK�ظ�Ĭ�ϵ�1����Ϊ�˷���ָ����ַ�ն���ֽڣ�
	
	return Data;
}
#endif
/***********************************************************
*@fuction	:MPU6050_Init
*@brief		:��ʼ��
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MPU6050_Init(void)		//������MyI2C֮��
{
#if soft  //���IIC
	MyI2C_Init();			//�̳�һ�µײ�
#else //Ӳ��IIC
	/******************************����ʱ��*****************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	//GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	/*****************************����ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	/****************************��������ģʽ*******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		//���ÿ�©�����Ӳ��I2CЭ������Ҫ��Ӳ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/****************************����IIC2����*******************************/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;			//ģʽ
	I2C_InitStructure.I2C_ClockSpeed = 50000;			//50KHZ
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//ʱ��ռ�ձ�
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;			//Ĭ�ϸ�Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//STM32��Ϊ�ӻ�ģʽʱ��������Ӧ7λ��ַ
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;	//STM32��Ϊ�ӻ�ģʽʱ������ָ��STM32�����ַ���������������У���Ҫ�������豸�ظ���
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);	//ʹ��
#endif
	/*********************************
			MPU6050�Ĵ���������
	*********************************/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//���õ�Դ����Ĵ���1
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//���õ�Դ����Ĵ���1
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//�����ʷ�Ƶ��10��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);		//���ֵ�ͨ�˲���
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//������
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//���ٶȼ�
}

/***********************************************************
*@fuction	:MPU6050_GetID
*@brief		:��ȡ�豸ID��
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_GetData
  ** @brief         : ��ȡ���ݼĴ���
  ** @param         : None
  ** @retval        : int16_t *AccX, int16_t *AccY, int16_t *AccZ,
					  int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;//��8λ����8λ
	/*******************************
	            ���ٶ�ֵ
	*******************************/
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//��ȡ�߰�λ��ֵ
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//��ȡ�ڰ�λ��ֵ
	*AccX = (DataH << 8) | DataL;		//�߰�λ����8λ���ٻ��ϵڰ�λ�����ٶȼ�X���16λ���ݣ���ͨ��ָ��������أ�
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	/*******************************
	            ������ֵ
	*******************************/	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
