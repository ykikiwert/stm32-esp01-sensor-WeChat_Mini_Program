#ifndef __DS18B20_H
#define __DS18B20_H 
#include <stm32f10x.h>  
#include "oled.h"
#include "delay.h"


//IO��������
#define DS18B20_IO_IN()     DQ1820_IN() 
#define DS18B20_IO_OUT()   DQ1820_OUT()
////IO��������											
#define	DS18B20_DQ_OUT(X)  GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)X)		//���ݶ˿�	PA12
#define	DS18B20_DQ_IN 	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)  		//���ݶ˿�	PA12

void SoftReset(void);
uint8_t DS18B20_Init(void);				//��ʼ��DS18B20
void DQ1820_IN(void);
void DQ1820_OUT(void);
short DS18B20_Get_Temp(void);		//��ȡ�¶�
void DS18B20_Start(void);			//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);	//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);			//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);			//����һ��λ
uint8_t DS18B20_Check(void);				//����Ƿ����DS18B20
void DS18B20_Rst(void);				//��λDS18B20
//void DS18B20_Test(void); 			//DS18B20����
void DS18B20_WHILE(void); 			//DS18B20���
float DS18B20_GetTemperature(void);

#endif















