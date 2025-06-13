/***  Header:     Main.c       ***/                         
/***  File Name:                   ***/                         
/***  Author:  ����                ***/         
/***  Date:   2015-05-01       ***/          
/***  Version:  V1.0                ***/            

#include "stm32f10x.h"
#include "NRF2401.h"

#define uchar unsigned char
#define uint  unsigned int

//#define NRF2401_GPIO  GPIOA
//#define MISO_GPIO     GPIO_Pin_4
//#define MOSI_GPIO     GPIO_Pin_5
//#define SCK_GPIO      GPIO_Pin_6
//#define CE_GPIO       GPIO_Pin_7
//#define CSN_GPIO      GPIO_Pin_8

#define NRF2401_GPIO  GPIOB
#define MISO_GPIO     GPIO_Pin_7
#define MOSI_GPIO     GPIO_Pin_6
#define SCK_GPIO      GPIO_Pin_5
#define CE_GPIO       GPIO_Pin_4
#define CSN_GPIO      GPIO_Pin_3

#define MISO(x) x?  	GPIO_WriteBit(NRF2401_GPIO,MISO_GPIO,Bit_SET):GPIO_WriteBit(NRF2401_GPIO,MISO_GPIO,Bit_RESET)

#define MOSI(x) x?    GPIO_WriteBit(NRF2401_GPIO,MOSI_GPIO,Bit_SET):GPIO_WriteBit(NRF2401_GPIO,MOSI_GPIO,Bit_RESET)

#define SCK(x)  x?    GPIO_WriteBit(NRF2401_GPIO,SCK_GPIO,Bit_SET):GPIO_WriteBit(NRF2401_GPIO,SCK_GPIO,Bit_RESET)

#define CE(x)  x?    	GPIO_WriteBit(NRF2401_GPIO,CE_GPIO,Bit_SET):GPIO_WriteBit(NRF2401_GPIO,CE_GPIO,Bit_RESET)

#define CSN(x)  x?    GPIO_WriteBit(NRF2401_GPIO,CSN_GPIO,Bit_SET):GPIO_WriteBit(NRF2401_GPIO,CSN_GPIO,Bit_RESET)


uchar   sta;//״̬��־
#define RX_DR (sta&0x40)
#define TX_DS (sta&0x20)
#define MAX_RT (sta&0x08)


//*********************************************NRF24L01*************************************

#define TX_ADR_WIDTH    5    // 5 uints TX address width 5�ֽڿ�ȵķ���/���յ�ַ
#define RX_ADR_WIDTH    5    // 5 uints RX address width 
uchar  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //���ص�ַ����һ����̬���͵�ַ
uchar  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //���յ�ַ

unsigned char Tx_Buf[TX_PLOAD_WIDTH];
unsigned char Rx_Buf[TX_PLOAD_WIDTH];//��������

//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define READ_REG        0x00   // ���Ĵ���ָ��
#define WRITE_REG       0x20  // д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61   // ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0   // д��������ָ��
#define FLUSH_TX        0xE1  // ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2   // ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3   // �����ظ�װ������ָ��
#define NOP             0xFF   // ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//-------------------------------------------------------------

void NRF2401_IO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MOSI_GPIO|SCK_GPIO|CE_GPIO|CSN_GPIO;
	GPIO_Init(NRF2401_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MISO_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(NRF2401_GPIO,&GPIO_InitStructure);
	
	
	CE(0);// SPI
	CSN(1);// SPI��ֹ
	SCK(0);// SPIʱ���õ�
	RX_Mode(40);			  //����ģʽ

}

uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // ѭ��8��
	{
		if(byte&0x80)// byte���λ�����MOSI
		 MOSI(1);
		else MOSI(0);
		byte=(byte<<1);// ��һλ��λ�����λ
		SCK(1);// ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
		if(GPIO_ReadInputDataBit(NRF2401_GPIO,MISO_GPIO))
//		if(MISO)// ��MISO��byte���λ
		byte|=0x01;
		else byte|=0x00;
		SCK(0);
	} 
	return(byte);
}
//-------------------------------------------------------------
//������RX��TXģʽʱʹ��
uchar SPI_RW_Reg(uchar reg,uchar value)// ��Ĵ���REGдһ���ֽڣ�ͬʱ����״̬�ֽ�
{
	uchar status;
	CSN(0);// CSN�õͣ���ʼ��������
	status=SPI_RW(reg);// ѡ��Ĵ�����ͬʱ����״̬��
	SPI_RW(value);// Ȼ��д���ݵ��üĴ���
	CSN(1);// CSN���ߣ��������ݴ���
	return(status);// ����״̬�Ĵ���
}
//-------------------------------------------------------------
//ֻ���ڶ�ȡģ��״̬�Ĵ���ֵSTATUS
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	CSN(0); //CSN�õͣ���ʼ��������
	SPI_RW(reg);//ѡ��Ĵ���
	reg_val=SPI_RW(0);// Ȼ��ӸüĴ���������
	CSN(1); //CSN���ߣ��������ݴ���
	return(reg_val);// ���ؼĴ�������
}
//-------------------------------------------------------------
//�����еı�������Ϊ:�Ĵ������ƣ�Ҫ���͵����飬�ֽڸ���
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,byte_ctr;
	CSN(0);// Set CSN low, init SPI tranactionCSN�õͣ���ʼ��������
	status=SPI_RW(reg);// Select register to write to and read status byteѡ��Ĵ�����ͬʱ����״̬��
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
		SPI_RW(*pBuf++);//����ֽ�д��nRF24L01
	CSN(1);// Set CSN high againCSN���ߣ��������ݴ���
	return(status);//return nRF24L01 status byte����״̬�Ĵ���
}
//-------------------------------------------------------------
//void TX_Mode(void)//���÷���ģʽ
//{
//	SPI_Write_Buf(WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    // д�뷢�͵�ַNRF24L01
//	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // д����յ�ַ  Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
//	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);// Enable Auto.Ack:Pipe0 ʹ�ܽ���ͨ��0�Զ�Ӧ��
//	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01);// Enable Pipe0 ʹ�ܽ���ͨ��0
//	SPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);// 500us + 86us, 10 retrans...1a�Զ��ط���ʱ�ȴ�500us+86us���Զ��ط�10��
//	SPI_RW_Reg(WRITE_REG+RF_CH, 40);// Select RF channel 40 ѡ����Ƶͨ��0x40   ����һ��
//	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x07);// TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
//	SPI_RW_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH); //���ý������ݳ���
//	delay_ms(1);
//}
//-------------------------------------------------------------
void RX_Mode(uchar rf)//���ý���ģʽ
{	
	CE(0);
	SPI_Write_Buf(WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH); //�����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01);//  ʹ�ܽ���ͨ��0
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...1a
	SPI_RW_Reg(WRITE_REG+RF_CH,rf);// ѡ����Ƶͨ��0x40   һ��
	SPI_RW_Reg(WRITE_REG+RX_PW_P0,TX_PLOAD_WIDTH);//���ý������ݳ��ȣ���������Ϊ2�ֽ� ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x07);// TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	SPI_RW_Reg(WRITE_REG+CONFIG,0x0f);// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	CE(1);// ����CE���������豸
//	delay_ms(1);
}

//-------------------------------------------------------------
//���ݷ����Ӻ���
void TxPacket(unsigned char * tx_buf)
{
	CE(0);//StandBy Iģʽ 
	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ   Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);//д���ݰ���TX FIFO
	SPI_RW_Reg(WRITE_REG+CONFIG,0x0e);// IRQ�շ�����ж���Ӧ��16λCRC��������
	CE(1);//�ø�CE���������ݷ���
//	delay_ms(1);
}
//-------------------------------------------------------------
//���յ�����ʱͨ��SPI����Rx_buf
uchar SPI_Read_Buf(uchar reg, uchar *pBuf,uchar bytes)
{
	uchar status,ctr;
	CSN(0);//Set CSN low, init SPI tranaction CSN�õͣ���ʼ��������
	status=SPI_RW(reg);//Select register to write to and read status uchar ѡ��Ĵ�����ͬʱ����״̬��
	for(ctr=0;ctr<bytes;ctr++)
		pBuf[ctr]=SPI_RW(0);// ����ֽڴ�nRF24L01����
	CSN(1);// CSN���ߣ��������ݴ���
	return(status);//return nRF24L01 status uchar ����״̬�Ĵ���
}
//-------------------------------------------------------------
//������յ����ݰ����ݴ���Rx_buf��
void RxPacket(unsigned char* rx_buf)
{
//	unsigned char revale=0;
		sta=SPI_Read(STATUS);// ��ȡ״̬�Ĵ������ж����ݽ���״��
		if(RX_DR)// �ж��Ƿ���յ�����
		{		
			CE(0);//SPIʹ��
			SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer	
			SPI_RW_Reg(WRITE_REG+STATUS,sta);//���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
			CE(1);
		}
}


