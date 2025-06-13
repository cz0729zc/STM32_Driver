#include "HX711.h"
#include "delay.h"

void InitPort_HX711()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SCK_Pin;
	GPIO_Init(HX711_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_Init(HX711_GPIO,&GPIO_InitStructure);	
}

//****************************************************
//��ʱ����
//****************************************************
void Delay_hx711_us(void)
{	
	delay_us(1);
}

//****************************************************
//��ȡHX711
//****************************************************
unsigned long HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 
	unsigned int later_1;
	
	InitPort_HX711();

  	HX711_SDA(1); 
		Delay_hx711_us();
  	HX711_SCK(0); 
  	 
	while(GPIO_ReadInputDataBit(HX711_GPIO,SDA_Pin)==1); //  	while((GPIO_ReadInputDataBit(HX711_GPIO,SDA_Pin)==1)&&(later_1<10000))//��ʱ��ֹ��ѭ��; 
//	while((GPIO_ReadInputDataBit(HX711_GPIO,SDA_Pin)==1)&&(later_1<10000))//��ʱ��ֹ��ѭ��;
//	{
//		later_1++;
//	}
		
		count=0;
  	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK(1); 
	  	count=count<<1;
			delay_us(1);		
			HX711_SCK(0); 
			delay_us(1);
	  	if(GPIO_ReadInputDataBit(HX711_GPIO,SDA_Pin)==1)
			count++; 
	} 
 	HX711_SCK(1); 
  count=count^0x800000;//��25�������½�����ʱ��ת������
	
	Delay_hx711_us();
	HX711_SCK(0);  
	return(count);
}





