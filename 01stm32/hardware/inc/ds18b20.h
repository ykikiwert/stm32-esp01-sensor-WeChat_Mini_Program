#ifndef __DS18B20_H
#define __DS18B20_H 
#include <stm32f10x.h>  
#include "oled.h"
#include "delay.h"


//IO方向设置
#define DS18B20_IO_IN()     DQ1820_IN() 
#define DS18B20_IO_OUT()   DQ1820_OUT()
////IO操作函数											
#define	DS18B20_DQ_OUT(X)  GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)X)		//数据端口	PA12
#define	DS18B20_DQ_IN 	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)  		//数据端口	PA12

void SoftReset(void);
uint8_t DS18B20_Init(void);				//初始化DS18B20
void DQ1820_IN(void);
void DQ1820_OUT(void);
short DS18B20_Get_Temp(void);		//获取温度
void DS18B20_Start(void);			//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);	//写入一个字节
uint8_t DS18B20_Read_Byte(void);			//读出一个字节
uint8_t DS18B20_Read_Bit(void);			//读出一个位
uint8_t DS18B20_Check(void);				//检测是否存在DS18B20
void DS18B20_Rst(void);				//复位DS18B20
//void DS18B20_Test(void); 			//DS18B20测试
void DS18B20_WHILE(void); 			//DS18B20检测
float DS18B20_GetTemperature(void);

#endif















