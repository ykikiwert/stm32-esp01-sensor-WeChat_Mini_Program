#include "ds18b20.h"
#include <stdio.h>
#include "oled.h"
 /**************************************************************
*��  �ܣ��жϸ�λ����
*��  ��: ��
*����ֵ: ��
**************************************************************/
void SoftReset(void)
{
	__set_FAULTMASK(1);		//�ر������ж�
	NVIC_SystemReset();		//�����λ
}

/**************************************************************
*��  �ܣ���λDS18B20
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT(0); //����DQ
    DelayUs(750);    //����750us
    DS18B20_DQ_OUT(1); //DQ=1 
	DelayUs(15);     //15US
}

/**************************************************************
*��  �ܣ��ȴ�DS18B20�Ļ�Ӧ
*��  ��: ��
*����ֵ: ����1:δ��⵽DS18B20�Ĵ���   ����0:����
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
*��  �ܣ���DS18B20��ȡһ��λ
*��  ��: ��
*����ֵ: 1/0 
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
*��  �ܣ���DS18B20��ȡһ���ֽ�
*��  ��: ��
*����ֵ: ���������� 
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
*��  �ܣ�дһ���ֽڵ�DS18B20
*��  ��: dat��Ҫд����ֽ�
*����ֵ: �� 
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
*��  �ܣ���ʼ�¶�ת��
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DS18B20_Start(void)		// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 

/**************************************************************
*��  �ܣ���ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
*��  ��: ��
*����ֵ: ����1:������  ����0:����
**************************************************************/
uint8_t DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��PG�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//PG11�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO��
 	GPIO_SetBits(GPIOA,GPIO_Pin_12);			//PG11 �����

	DS18B20_Rst();

	return DS18B20_Check();
}    

/**************************************************************
*��  �ܣ���������˿�
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DQ1820_OUT(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//��©���
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**************************************************************
*��  �ܣ���������˿�
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DQ1820_IN(void)  
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA,&GPIO_InitStructure);
} 
 
/**************************************************************
*��  �ܣ���ds18b20�õ��¶�ֵ�����ȣ�0.1C  �¶�ֵ ��-550~1250��
*��  ��: ��
*����ֵ: �� 
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
        temp=0;				//�¶�Ϊ��  
    }else temp=1;			//�¶�Ϊ��	  	  
    tem=TH;					//��ø߰�λ
    tem<<=8;    
    tem+=TL;				//��õͰ�λ
    tem=(float)tem*0.625;	//ת��     
	if(temp)return tem;		//�����¶�ֵ
	else return -tem;    
} 

/**************************************************************
*��  �ܣ�DS18B20���������
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DS18B20_WHILE(void)			
{
	unsigned char init_0 = 0;
	while(DS18B20_Init())
	{
		init_0 ++;
		if(init_0 == 240)
			SoftReset();
	//	OLED_Show_Str(16*4,4,"��ʼ��ʧ��",16);
	}
	//OLED_Show_Str(16*4,4,"��ʼ���ɹ�",16);
}

/**************************************************************
*��  �ܣ�OLED��ʾ�¶�ֵ
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DS18B20_Test(void) 
{
	short temperature;
    char  date[20];
	
	temperature=DS18B20_Get_Temp();
	if(temperature<0)
	{
		sprintf(date,"-%2.1f��",temperature/10.0);
//		OLED_Show_Str(53,4,date,16);
		OLED_ShowString(6,4,date,16);
	}
	else
	{
	    sprintf(date," %2.1f��",temperature/10.0);
//		OLED_Show_Str(53,4,date,16);
		OLED_ShowString(6,4,date,16);
	}
	DelayXms(300);
}
 
