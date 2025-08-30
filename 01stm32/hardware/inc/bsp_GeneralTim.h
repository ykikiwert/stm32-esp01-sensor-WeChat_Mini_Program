#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"


/**************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ����Ҫ�ĸ���ʱ����ʱ��ֻ��Ҫ������ĺ궨��ĳ�1����
#define GENERAL_TIM2    1

#if  GENERAL_TIM2
#define            GENERAL_TIM                   TIM2
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM_Period            (50000-1)
#define            GENERAL_TIM_Prescaler         7200-1
#define            GENERAL_TIM_IRQ               TIM2_IRQn
#define            GENERAL_TIM_IRQHandler        TIM2_IRQHandler
#endif
/**************************��������********************************/

void GENERAL_TIM_Init(void);


#endif	/* __BSP_GENERALTIME_H */


