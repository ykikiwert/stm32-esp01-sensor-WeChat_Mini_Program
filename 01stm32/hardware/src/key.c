//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "key.h"
#include "delay.h"
#include "led.h"

/*
************************************************************
*	�������ƣ�	Key_Init
*
*	�������ܣ�	������ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Key_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	EXTI_InitTypeDef exti_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//��GPIOB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;				//����Ϊ����
	gpio_initstruct.GPIO_Pin = GPIO_Pin_8;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	GPIO_Init(GPIOB, &gpio_initstruct);							//��ʼ��GPIO
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	
	exti_initstruct.EXTI_Line = EXTI_Line8;
	exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_initstruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_initstruct);
	
	nvic_initstruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_initstruct);
	EXTI_ClearITPendingBit(EXTI_Line8);

}


