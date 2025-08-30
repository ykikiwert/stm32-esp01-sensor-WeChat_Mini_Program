#include "bsp_GeneralTim.h" 
extern unsigned short timeCount;	//���ͼ������
static void TIM4_Config(void)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
    TIM_TimeBaseStructure.TIM_Period=9999;
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;	
    TIM_TimeBaseStructure.TIM_ClockDivision=0;		
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
//		// ����������жϱ�־λ
//    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

// �ж����ȼ�����
static void TIM4_NVICConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
		// ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// �ظ���������ֵ��û�õ����ù�
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
		// ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM, DISABLE);
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();	
  TIM4_Config();
  TIM4_NVICConfig();	
}



void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {   timeCount=1;   
			
    }

    
    // ��� TIM2 ���жϱ�־λ
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		
}