#include "tds.h"
#include "bsp_adc.h" 
#include "ds18b20.h"
#include <string.h>
#include "usart.h"

extern float temperature1;  // 存储计算后的温度值
void TDS_Init(void)
{
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (TDS_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
		GPIO_InitStructure.GPIO_Pin = TDS_GPIO_PIN;					// 配置 ADC IO 引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
		
		GPIO_Init(TDS_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO

		ADCx_Init();
	}

}


uint16_t TDS_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
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


uint16_t TDS_GetData_PPM(void)  // 返回 uint16_t 类型
{
    float tempData = 0;
    float TDS_DAT;
	  float temp_comp=0;     //温度补偿
	  float TDS_comp=0;         //TDS修正
    char buffer[50];
    char buf[50];
    // 1. 读取多次 ADC 并取平均
    for (uint8_t i = 0; i < TDS_READ_TIMES; i++)
    {
        tempData += TDS_ADC_Read();
        DelayXms(5);
    }
    tempData /= TDS_READ_TIMES;
    
    // 2. 转换为电压值（假设 ADC 是 12 位，参考电压 3.3V）
    TDS_DAT = (tempData / 4095.0) * 3.3;
    
		// 4. 温度补偿计算
		temp_comp=1+0.02*(temperature1-25);
		TDS_comp=temp_comp*TDS_DAT;
    // 5. 计算 TDS 值（原公式）
    TDS_comp = 66.71 * TDS_comp * TDS_comp * TDS_comp - 127.93 * TDS_comp * TDS_comp + 428.7 * TDS_comp;
    
    // 6. 限制范围并转换为整数
    if (TDS_comp < 20) 
        TDS_comp = 0;
    else if (TDS_comp > 1000)  // 限制最大值
        TDS_comp = 1000;
    
    return (uint16_t)(TDS_comp + 0.5);  // 四舍五入
}

