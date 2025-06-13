#include "key.h"
#include "multi_button.h"

#define ACTIVE_LEVEL_HIGH 1
#define ACTIVE_LEVEL_LOW 0

// 定义button id
#define Button0_id 0
#define Button1_id 1
#define Button2_id 3

#define READ_KEY1 PBin(7)
#define READ_KEY2 PBin(8)
#define READ_KEY3 PBin(6)
// #define READ_KEY4 PBin(7)

uint8_t key_state = 0;
uint8_t key_count = 0;
// 建立按键对象
Button button0, button1, button2;

// 按键状态读取接口
uint8_t read_button0_GPIO(void)
{
    return READ_KEY1;
}

uint8_t read_button1_GPIO(void)
{
    return READ_KEY2;
}

uint8_t read_button2_GPIO(void)
{
    return READ_KEY3;
}

// uint8_t read_button3_GPIO(void)
// {
//     return READ_KEY4;
// }

// 按键按下回调函数
void bt0_press_down_Handler(void *btn)
{
    key_state = 1;
}

void bt1_press_down_Handler(void *btn)
{
    key_state = 2;
}

void bt2_press_down_Handler(void *btn)
{
    key_state = 3;
}

// void bt3_press_down_Handler(void *btn)
// {
//     key_state = 4;
// }

void bt1_long_press_start_Handler(void *btn)
{
    key_state = 4;
}

void Key_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // IO口配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 换IO口需要修改
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Key_Init(void)
{
    Key_GPIO_Init();
    /*初始化后还要在定时器中每5ms执行一次button_ticks()心跳函数*/
    // button Init
    button_init(&button0, read_button0_GPIO, ACTIVE_LEVEL_LOW, Button0_id);
    button_init(&button1, read_button1_GPIO, ACTIVE_LEVEL_LOW, Button1_id);
    button_init(&button2, read_button2_GPIO, ACTIVE_LEVEL_LOW, Button2_id);

    // attach handler
    button_attach(&button0, PRESS_DOWN, bt0_press_down_Handler);
    button_attach(&button1, PRESS_DOWN, bt1_press_down_Handler);
    button_attach(&button1, LONG_PRESS_START, bt1_long_press_start_Handler);
    button_attach(&button2, PRESS_DOWN, bt2_press_down_Handler);

    // start button
    button_start(&button0);
    button_start(&button1);
    button_start(&button2);
}
