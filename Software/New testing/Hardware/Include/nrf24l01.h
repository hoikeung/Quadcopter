#ifndef __NRF24L01_H__

#define __NRF24L01_H__

#include "stm32f1xx_hal.h"


#define NRF_24L01_FAILED  1

#define NRF_24L01_CSN_PIN	GPIO_PIN_12
#define NRF_24L01_CE_PIN	GPIO_PIN_11

#define	NRF_24L01_CSN_PORT	GPIOB
#define	NRF_24L01_CE_PORT	GPIOA

#define NRF24L01_NUM_INIT_REGISTER	6

#define NRF24L01_CHANNEL	45

#define RF_OK	0x40

#define	RX_PLOAD_WIDTH	32
#define RX_ADR_WIDTH	5


typedef enum 
{
	/*
		NRF24L01 commands
	*/
	R_REGISTER = 0x00,
	W_REGISTER = 0x20,
	R_RX_PAYLOAD = 0x61,
	W_TX_PAYLOAD = 0xA0,
	FLUSH_TX = 0xE1,
	FLUSH_RX = 0xE2,
	REUSE_TX_PL = 0xE3,
	NOP = 0xFF,
	
	/*
		NRF24L01 register
	*/
	NRF24L01_CONFIG = 0x00,
	EN_AA = 0x01,
	EN_RXADDR = 0x02,
	SETUP_AW = 0x03,
	SETUP_RETR = 0x04,
	RF_CH = 0x05,
	RF_SETUP = 0x06,
	STATUS = 0x07,
	OBSERVE_TX = 0x08,
	CD = 0x09,
	RX_ADDR_P0 = 0x0A,
	RX_ADDR_P1 = 0x0B,
	RX_ADDR_P2 = 0x0C,
	RX_ADDR_P3 = 0x0D,
	RX_ADDR_P4 = 0x0E,
	RX_ADDR_P5 = 0x0F,
	TX_ADDR = 0x10,
	RX_PW_P0 = 0x11,
	RX_PW_P1 = 0x12,
	RX_PW_P2 = 0x13,
	RX_PW_P3 = 0x14,
	RX_PW_P4 = 0x15,
	RX_PW_P5 = 0x16,
	FIFO_STATUS = 0x17,
} NRF24L01_Register;

typedef enum
{
  NRF24L01_CONNECTION_FAIL = 0,
	NRF24L01_CONNECTION_SUCCESS = 1,
} NRF24L01_Status;

void NRF24L01_Init(void);
void NRF24L01_Init_Pin(void);
void NRF24L01_RX_Mode(void);
void NRF24L01_Write_Register(uint8_t reg, uint8_t *pData, uint16_t size);
NRF24L01_Status NRF24L01_Receive_Data(void);
NRF24L01_Status NRF24L01_Check_RX_Data(void);
void NRF24L01_Get_RX_Data(uint8_t *pData);
NRF24L01_Status NRF24L01_Check_Connection(void);

#endif
