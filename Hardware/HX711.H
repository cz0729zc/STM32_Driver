#ifndef __HX711_H__
#define __HX711_H__

#include "stm32f10x.h"

//IO设置
#define HX711_GPIO  GPIOA

#define SDA_Pin  GPIO_Pin_14
#define SCK_Pin  GPIO_Pin_13

#define HX711_SDA(x) x?  GPIO_WriteBit(HX711_GPIO,SDA_Pin,Bit_SET):GPIO_WriteBit(HX711_GPIO,SDA_Pin,Bit_RESET)
#define HX711_SCK(x) x?  GPIO_WriteBit(HX711_GPIO,SCK_Pin,Bit_SET):GPIO_WriteBit(HX711_GPIO,SCK_Pin,Bit_RESET)


//函数或者变量声明
extern void Delay_hx711_us(void);
extern unsigned long HX711_Read(void);
void InitPort_HX711();

#endif