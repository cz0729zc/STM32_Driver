#ifndef __PM25_H
#define __PM25_H

#include "stm32f10x.h"

// ����֡��ʽ����
#define FRAME_HEADER      0xFF    // ֡ͷ��ʶ
#define DATA_LENGTH       9       // ����֡����
#define CHECKSUM_POS      8       // У���λ��


// PM2.5���ݽṹ��
typedef struct {
    float duty_cycle;      // ռ�ձȣ�0.5%~50%��
    float pm25_value;      // PM2.5Ũ��
    uint8_t data_valid;    // ������Ч�Ա�־
} PM25_Data;

// ��������
void PM25_Init(void);
void PM25_ReceiveHandler(uint8_t data);
PM25_Data PM25_GetCurrentData(void);

#endif

