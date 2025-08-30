#ifndef __WATER_H
#define	__WATER_H
#include "stm32f10x.h"
#include "bsp_adc.h"
#include "delay.h"
#include "math.h"

/*****************���絥Ƭ�����******************
											STM32
 * �ļ�			:	TDS�絼�ʴ�����h�ļ�                   
 * �汾			: V1.0
 * ����			: 2025.2.2
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							
 * BILIBILI	:	���絥Ƭ�����
 * CSDN			:	���絥Ƭ�����
 * ����			:	����

**********************BEGIN***********************/

#define TDS_READ_TIMES	10  //TDS������ADCѭ����ȡ����

/***************�����Լ��������****************/
// TDS GPIO�궨��

#define		TDS_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	TDS_GPIO_PORT								GPIOA
#define 	TDS_GPIO_PIN								GPIO_Pin_0
#define   ADC_CHANNEL									ADC_Channel_0	// ADC ͨ���궨��


/*********************END**********************/


void TDS_Init(void);
uint16_t TDS_GetData(void);
float TDS_GetData_PPM(void);

#endif /* __TDS_H */

