//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "oled.h" 
#include "tds.h"
#include "bsp_adc.h"  
#include "HX711.h"
#include "ds18b20.h"
#include "bsp_GeneralTim.h" 
//C库
#include <string.h>

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

uint16_t TDS=0;               //TDS初始值
uint16_t Volume_Shiwu=0;      //水量初始值
u8 temperature=0;             //温度初始值
unsigned short timeCount = 0;	//发送间隔变量
u8 SendWaitForAck=0;
float tds_data;
u8 Voice_reminder=0;                //声音播放标志位
volatile uint8_t timer_started = 0; // 标志位：定时器是否已启动
volatile uint8_t timer_expired = 0; // 标志位：定时器是否到期

void Hardware_Init(void);
void Display_Init(void);
void Refresh_Data(void);
void getData(void);

/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/
int main(void)
{
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//初始化外围硬件

	while(1)
	{
		
			ESP8266_Init();					//初始化ESP8266
		  OLED_Clear(); OLED_ShowString(0,0,"Connect MQTTs Server...",16);   //连接MQTT服务器
			while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
			DelayXms(500);
			OLED_ShowString(0,4,"Connect MQTT Server Success",16); DelayXms(500);
			OLED_Clear(); OLED_ShowString(0,0,"Device login ...",16);
			while(OneNet_DevLink())			//接入OneNET
			{
				ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
				DelayXms(500);
			}
				
			OneNET_Subscribe();  //Onenet平台消息订阅
			
			while(1)
			{
		//		UsartPrintf(USART_DEBUG,"%d ,%d ,%d \r\n",TDS,Volume_Shiwu,temperature);
				if(timeCount == 1)									//数据上报时间间隔，可以根据自己的网速更改
				{
					if(SendWaitForAck==2)
					{
						SendWaitForAck=0;
						break;
					}
					getData();
		//			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
					OneNet_SendData();									//发送数据	
					timeCount = 0;
					ESP8266_Clear();
				}
				
				dataPtr = ESP8266_GetIPD(0);
				if(dataPtr != NULL)
					OneNet_RevPro(dataPtr);

				switch(Voice_reminder)  //语音播报
				{
					case 1:Usart_SendString(USART1,"AF:30",6);DelayXms(1000);
								 Usart_SendString(USART1,"A7:00001",9);Voice_reminder=0;UsartPrintf(USART_DEBUG, "YESYES\r\n");break;
					default:Voice_reminder=0;
					break;
				}
			}
	}

}
/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	Delay_Init();									     //systick初始化
	
	Usart1_Init(9600);							   //串口1，连接JR6001语音播报模块
	
	Usart2_Init(115200);							 //串口2，驱动ESP8266用
	
	Usart3_Init(115200);               //串口3，打印调试信息用
  Key_Init();
	
	Led_Init();									       //LED灯初始化
	
	OLED_Init();			                 //初始化OLED 
  
	TDS_Init();	                       //TDS初始化
	
	Init_HX711pin();                   //HX711压力传感器初始化
	
	DS18B20_Init(); 			             //温度传感器初始化
	
	GPIOInit();         
	
	GENERAL_TIM_Init();                //基本定时器初始化
	
	Get_Maopi();				               //称毛皮重量
	DelayXms(1000);
	Get_Maopi();				               //重新获取毛皮重量
	
	OLED_Clear(); OLED_ShowString(0,0,"Hardware init OK",16); DelayMs(1000);
}


void getData(void)                   //获取传感器数据       
{
		TDS = TDS_GetData_PPM();         //获取TDS数据
	if(TDS<50)
		{
			GREEN=0;                       //指示灯显示
			YELLOW=1;
			RED=1;
		}
  else if(TDS>=50&&TDS<150)
	  {
		  GREEN=1;
			YELLOW=0;
			RED=1;
	  }
  else if(TDS>=300)
	  {
			GREEN=1;
			YELLOW=1;
			RED=0;
	  }
    OLED_ShowString(0,2,"TDS:",16);	
    OLED_ShowString(0,4,"Volume:",16);	
		
		OLED_ShowNum(36,2,TDS,4,16);
		
		Get_Weight();                  //获取水量数据
		Volume_Shiwu=Weight_Shiwu/1;
		OLED_ShowNum(56,4,Volume_Shiwu,4,16);
		
	  temperature=DS18B20_GetTemperature();//获取温度数据
	
}

void EXTI9_5_IRQHandler(void)            //按键中断
{
	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
	DelayXms(10);
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)==0)
	{
// 如果定时器未启动，则启动定时器
            if (!timer_started)
            {
                TIM_SetCounter(TIM2, 0); // 重置计数器
                TIM_Cmd(TIM2, ENABLE);   // 启动定时器
                timer_started = 1;       // 设置标志位
            }
	}
	EXTI_ClearITPendingBit(EXTI_Line8);
  }	
}

void TIM2_IRQHandler(void)                  //启动定时器中断，用于喝水定时
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
       GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 点亮 LED
			  Voice_reminder=1;
        TIM_Cmd(TIM2, DISABLE);             // 停止定时器
        timer_started = 0;                  // 清除标志位
        timer_expired = 1;                  // 设置到期标志位
    }

    // 清除 TIM2 的中断标志位
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
}

