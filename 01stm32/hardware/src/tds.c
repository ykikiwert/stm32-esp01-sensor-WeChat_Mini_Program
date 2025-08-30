#include "tds.h"
#include "bsp_adc.h" 
#include "ds18b20.h"
#include <string.h>
#include "usart.h"

extern float temperature1;  // �洢�������¶�ֵ
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


uint16_t TDS_GetData_PPM(void)  // ���� uint16_t ����
{
    float tempData = 0;
    float TDS_DAT;
	  float temp_comp=0;     //�¶Ȳ���
	  float TDS_comp=0;         //TDS����
    char buffer[50];
    char buf[50];
    // 1. ��ȡ��� ADC ��ȡƽ��
    for (uint8_t i = 0; i < TDS_READ_TIMES; i++)
    {
        tempData += TDS_ADC_Read();
        DelayXms(5);
    }
    tempData /= TDS_READ_TIMES;
    
    // 2. ת��Ϊ��ѹֵ������ ADC �� 12 λ���ο���ѹ 3.3V��
    TDS_DAT = (tempData / 4095.0) * 3.3;
    
		// 4. �¶Ȳ�������
		temp_comp=1+0.02*(temperature1-25);
		TDS_comp=temp_comp*TDS_DAT;
    // 5. ���� TDS ֵ��ԭ��ʽ��
    TDS_comp = 66.71 * TDS_comp * TDS_comp * TDS_comp - 127.93 * TDS_comp * TDS_comp + 428.7 * TDS_comp;
    
    // 6. ���Ʒ�Χ��ת��Ϊ����
    if (TDS_comp < 20) 
        TDS_comp = 0;
    else if (TDS_comp > 1000)  // �������ֵ
        TDS_comp = 1000;
    
    return (uint16_t)(TDS_comp + 0.5);  // ��������
}

