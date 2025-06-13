#include "exit.h"
#include "delay.h"

uint32_t hall_pulse_count = 0; // �������������

//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�жϳ�ʼ��
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ��GPIOB��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // ����PB1Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ����PB1Ϊ�ⲿ�ж�Դ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    
    // �����ⲿ�ж���1
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // ˫���ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// �ⲿ�ж�1������� (PB1)
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        hall_pulse_count++; // ���������1
        EXTI_ClearITPendingBit(EXTI_Line1); // ����жϱ�־λ
    }
}

// ��ȡ�����������
uint32_t Get_Hall_Pulse_Count(void)
{
    return hall_pulse_count;
}

// ���û����������
void Reset_Hall_Pulse_Count(void)
{
    hall_pulse_count = 0;
}

