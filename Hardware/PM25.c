#include "PM25.h"
#include "string.h"
#include "usart1.h"

static PM25_Data current_data;
static uint8_t rx_buffer[DATA_LENGTH];
static uint8_t rx_index = 0;
static uint8_t frame_started = 0;

//#define DEBUG_PM25_PROTOCOL 0

// У��ͼ��㣨�������ʵ�֣�
uint8_t FucCheckSum(uint8_t *buf, uint8_t len) {
    uint8_t temp = 0;
    for(uint8_t j = 1; j < len-1; j++) {
        temp += buf[j];
    }
    return (uint8_t)(~temp + 1);
}


// ���ݽ�������
static void ParsePM25Data(uint8_t *data) {
    // У��֡ͷ��У���
    if(data[0] != FRAME_HEADER) {
        //Serial_Printf("\r\n֡ͷ����");
        memset(rx_buffer, 0, DATA_LENGTH); // ���㻺����
        rx_index = 0;                      // ���ý�������
        frame_started = 0;                 // ��λ״̬��
        current_data.data_valid = 0;
        return;
    }
    //���У���
    if(FucCheckSum(data, DATA_LENGTH) != data[CHECKSUM_POS]) {
        //Serial_Printf("\r\nУ��ʹ���");
        memset(rx_buffer, 0, DATA_LENGTH); // ���㻺����
        rx_index = 0;
        frame_started = 0;
        current_data.data_valid = 0;
        return;
    }

    // ����ռ�ձ�
//	for(int i=0;i<8;i++)
////		Serial_SendByte(data[i]);
//	Serial_SendByte(data[3]);
//	Serial_SendByte(data[4]);
	
    uint8_t hex_integer  = data[3];
    uint8_t hex_decimal  = data[4];
	//Serial_Printf("\r\n hex_integer:%#X",hex_integer);
	//Serial_Printf("\r\n hex_decimal:%#X",hex_decimal);
	
	//Serial_Printf("\r\n hex_integer:%d",hex_integer);
	//Serial_Printf("\r\n hex_decimal:%d",hex_decimal);
	
	
    current_data.duty_cycle = (int)hex_integer + (int)hex_decimal/100.0f;
	//u1_printf("\r\n duty_cycle:%f",current_data.duty_cycle);
    // ����PM2.5Ũ�ȣ����ݴ��������Ե���ϵ����
    current_data.pm25_value = (current_data.duty_cycle - 0.5) * 400 / 49.5;

    //Serial_Printf("\r\n pm25_value: %04f\n", current_data.pm25_value);
    //����޷�����
    if(current_data.pm25_value < 0) 
        current_data.pm25_value = 0;
    else if(current_data.pm25_value > 400) 
        current_data.pm25_value = 400;

    current_data.data_valid = 1;
}

// �������ݴ������ڴ����ж��е��ã�
void PM25_ReceiveHandler(uint8_t data) {
    static uint8_t frame_started = 0;
    
    if(!frame_started) {
        if(data == FRAME_HEADER) {
            rx_index = 0;
            rx_buffer[rx_index++] = data; // ����֡ͷ
            frame_started = 1;
        }
    } else {
        if(rx_index >= DATA_LENGTH) {
            ParsePM25Data(rx_buffer);     // У��ʧ��ʱ���Զ�����
            frame_started = 0;           // ���۳ɹ���񶼸�λ
        } else {
            rx_buffer[rx_index++] = data; // ������仺����
        }
    }

}

// ��ȡ��ǰ����
PM25_Data PM25_GetCurrentData(void) {
	//Serial_Printf("��ȡ��ǰPM2.5����");
    return current_data;
}

// ģ���ʼ��
void PM25_Init(void) {
    memset(&current_data, 0, sizeof(PM25_Data));
}
