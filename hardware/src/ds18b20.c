#include "ds18b20.h"
#include <stdio.h>
#include "oled.h"
 /**************************************************************
*功  能：中断复位函数
*参  数: 无
*返回值: 无
**************************************************************/
void SoftReset(void)
{
	__set_FAULTMASK(1);		//关闭所有中断
	NVIC_SystemReset();		//软件复位
}

/**************************************************************
*功  能：复位DS18B20
*参  数: 无
*返回值: 无 
**************************************************************/
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT(0); //拉低DQ
    DelayUs(750);    //拉低750us
    DS18B20_DQ_OUT(1); //DQ=1 
	DelayUs(15);     //15US
}

/**************************************************************
*功  能：等待DS18B20的回应
*参  数: 无
*返回值: 返回1:未检测到DS18B20的存在   返回0:存在
**************************************************************/
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

/**************************************************************
*功  能：从DS18B20读取一个位
*参  数: 无
*返回值: 1/0 
**************************************************************/
uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
    uint8_t data;
	DS18B20_IO_OUT();	//SET PA0 OUTPUT
    DS18B20_DQ_OUT(0); 
	DelayUs(2);
    DS18B20_DQ_OUT(1); 
	DS18B20_IO_IN();	//SET PA0 INPUT
	DelayUs(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    DelayUs(50);           
    return data;
}

/**************************************************************
*功  能：从DS18B20读取一个字节
*参  数: 无
*返回值: 读到的数据 
**************************************************************/
uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
    uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

/**************************************************************
*功  能：写一个字节到DS18B20
*参  数: dat：要写入的字节
*返回值: 无 
**************************************************************/
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
	DS18B20_IO_OUT();			//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT(0);	// Write 1
            DelayUs(2);                            
            DS18B20_DQ_OUT(1);
            DelayUs(60);             
        }
        else 
        {
            DS18B20_DQ_OUT(0);	// Write 0
            DelayUs(60);             
            DS18B20_DQ_OUT(1);
            DelayUs(2);                          
        }
    }
}

/**************************************************************
*功  能：开始温度转换
*参  数: 无
*返回值: 无 
**************************************************************/
void DS18B20_Start(void)		// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 

/**************************************************************
*功  能：初始化DS18B20的IO口 DQ 同时检测DS的存在
*参  数: 无
*返回值: 返回1:不存在  返回0:存在
**************************************************************/
uint8_t DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PG端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//PG11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO口
 	GPIO_SetBits(GPIOA,GPIO_Pin_12);			//PG11 输出高

	DS18B20_Rst();

	return DS18B20_Check();
}    

/**************************************************************
*功  能：配置输出端口
*参  数: 无
*返回值: 无 
**************************************************************/
void DQ1820_OUT(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//开漏输出
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**************************************************************
*功  能：配置输入端口
*参  数: 无
*返回值: 无 
**************************************************************/
void DQ1820_IN(void)  
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA,&GPIO_InitStructure);
} 
 
/**************************************************************
*功  能：从ds18b20得到温度值，精度：0.1C  温度值 （-550~1250）
*参  数: 无
*返回值: 无 
**************************************************************/
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
	short tem;
    DS18B20_Start ();			//ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	//skip rom
    DS18B20_Write_Byte(0xbe);	//convert	    
    TL=DS18B20_Read_Byte();		//LSB   
    TH=DS18B20_Read_Byte();		//MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;				//温度为负  
    }else temp=1;			//温度为正	  	  
    tem=TH;					//获得高八位
    tem<<=8;    
    tem+=TL;				//获得低八位
    tem=(float)tem*0.625;	//转换     
	if(temp)return tem;		//返回温度值
	else return -tem;    
} 

/**************************************************************
*功  能：DS18B20传感器检测
*参  数: 无
*返回值: 无 
**************************************************************/
void DS18B20_WHILE(void)			
{
	unsigned char init_0 = 0;
	while(DS18B20_Init())
	{
		init_0 ++;
		if(init_0 == 240)
			SoftReset();
	//	OLED_Show_Str(16*4,4,"初始化失败",16);
	}
	//OLED_Show_Str(16*4,4,"初始化成功",16);
}

/**************************************************************
*功  能：OLED显示温度值
*参  数: 无
*返回值: 无 
**************************************************************/
void DS18B20_Test(void) 
{
	short temperature;
    char  date[20];
	
	temperature=DS18B20_Get_Temp();
	if(temperature<0)
	{
		sprintf(date,"-%2.1f℃",temperature/10.0);
//		OLED_Show_Str(53,4,date,16);
		OLED_ShowString(6,4,date,16);
	}
	else
	{
	    sprintf(date," %2.1f℃",temperature/10.0);
//		OLED_Show_Str(53,4,date,16);
		OLED_ShowString(6,4,date,16);
	}
	DelayXms(300);
}
 
