#ifndef __WATER_H
#define	__WATER_H
#include "stm32f10x.h"
#include "bsp_adc.h"
#include "delay.h"
#include "math.h"

/*****************辰哥单片机设计******************
											STM32
 * 文件			:	TDS电导率传感器h文件                   
 * 版本			: V1.0
 * 日期			: 2025.2.2
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥

**********************BEGIN***********************/

#define TDS_READ_TIMES	10  //TDS传感器ADC循环读取次数

/***************根据自己需求更改****************/
// TDS GPIO宏定义

#define		TDS_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	TDS_GPIO_PORT								GPIOA
#define 	TDS_GPIO_PIN								GPIO_Pin_0
#define   ADC_CHANNEL									ADC_Channel_0	// ADC 通道宏定义


/*********************END**********************/


void TDS_Init(void);
uint16_t TDS_GetData(void);
float TDS_GetData_PPM(void);

#endif /* __TDS_H */

