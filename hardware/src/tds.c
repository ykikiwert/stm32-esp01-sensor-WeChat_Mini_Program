#include "tds.h"
#include "bsp_adc.h" 
/*****************���絥Ƭ�����******************
											STM32
 * �ļ�			:	TDS�絼�ʴ�����c�ļ�                   
 * �汾			: V1.0
 * ����			: 2024.2.2
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							
 * BILIBILI	:	���絥Ƭ�����
 * CSDN			:	���絥Ƭ�����
 * ����			:	����

**********************BEGIN***********************/

void TDS_Init(void)
{
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (TDS_GPIO_CLK, ENABLE );	// �� ADC IO�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = TDS_GPIO_PIN;					// ���� ADC IO ����ģʽ
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// ����Ϊģ������
		
		GPIO_Init(TDS_GPIO_PORT, &GPIO_InitStructure);				// ��ʼ�� ADC IO

		ADCx_Init();
	}

}


uint16_t TDS_ADC_Read(void)
{
	//����ָ��ADC�Ĺ�����ͨ��������ʱ��
	return ADC_GetValue(ADC_CHANNEL, ADC_SampleTime_55Cycles5);
}


uint16_t TDS_GetData(void)
{
	

	uint32_t  tempData = 0;
	for (uint8_t i = 0; i < TDS_READ_TIMES; i++)
	{
		tempData += TDS_ADC_Read();
		DelayXms(5);
	}

	tempData /= TDS_READ_TIMES;
	return tempData;
	
}

float TDS_GetData_PPM(void)
{
	float  tempData = 0;
	float TDS_DAT;

	for (uint8_t i = 0; i < TDS_READ_TIMES; i++)
	{
		tempData += TDS_ADC_Read();
		DelayXms(5);
	}
	tempData /= TDS_READ_TIMES;
	
	TDS_DAT = (tempData/4095.0)*3.3;
	TDS_DAT = 66.71*TDS_DAT*TDS_DAT*TDS_DAT-127.93*TDS_DAT*TDS_DAT+428.7*TDS_DAT;
	if(TDS_DAT<20) TDS_DAT = 0;
	
	return TDS_DAT;

}



