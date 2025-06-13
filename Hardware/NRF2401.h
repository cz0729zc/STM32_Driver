/***  Header:     Main.c       ***/                         
/***  File Name:                   ***/                         
/***  Author:  ����                ***/         
/***  Date:   2015-05-01       ***/          
/***  Version:  V1.0                ***/            

#ifndef _NRF2401_H_
#define _NRF2401_H_


#define TX_PLOAD_WIDTH  1  // 32 uints TX payload  ����ͨ����Ч���ݿ��
#define RX_PLOAD_WIDTH  1  // 32 uints TX payload

extern unsigned char Tx_Buf[TX_PLOAD_WIDTH];
extern unsigned char Rx_Buf[TX_PLOAD_WIDTH];//��������

void NRF2401_IO_Init(void);
unsigned char SPI_RW(unsigned char byte);
unsigned char SPI_RW_Reg(unsigned char reg,unsigned char value);// ��Ĵ���REGдһ���ֽڣ�ͬʱ����״̬�ֽ�
unsigned char SPI_Read(unsigned char reg);
unsigned char SPI_Write_Buf(unsigned char reg,unsigned char *pBuf,unsigned char bytes);
void RX_Mode(unsigned char rf);//���ý���ģʽ
void TxPacket(unsigned char * tx_buf);
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf,unsigned char bytes);
void RxPacket(unsigned char* rx_buf);

#endif 
