
#include "nrf24l01.h"
#include "spi.h"
#include "led.h"

#include <string.h>

const uint8_t TX_ADDRESS[]= {0x11,0x22,0x33,0x44,0x55};
const uint8_t RX_ADDRESS[]= {0x11,0x22,0x33,0x44,0x55};

uint8_t nrf24l01_rx_data[32];






const uint8_t NRF24L01_register_init[NRF24L01_NUM_INIT_REGISTER][2] = {
	{EN_AA, 0x01}, //Enable ‘Auto Acknowledgment’ Function
	{EN_RXADDR, 0x01}, //Enable data pipe 0
	{RF_CH, NRF24L01_CHANNEL}, //Set channel to 45
	{RX_PW_P0, RX_PLOAD_WIDTH}, //Set Number of bytes in RX payload in data pipe 0 to 32 bytes
	{RF_SETUP, 0x0F}, //Set RF power to 0dm, Data Rate to 2Mbps
	{NRF24L01_CONFIG, 0x0F}, //Set to RX mode, enable CRC
};






void NRF24L01_Read_Register(uint8_t reg, uint8_t *pData, uint16_t size)
{
	uint8_t command = R_REGISTER + reg;
	
	HAL_GPIO_WritePin(NRF_24L01_CSN_PORT, NRF_24L01_CSN_PIN, GPIO_PIN_RESET);
	
	SPI2_Transmit(&command, 1);
	SPI2_Receive(pData, size);
	
	HAL_GPIO_WritePin(NRF_24L01_CSN_PORT, NRF_24L01_CSN_PIN, GPIO_PIN_SET);
}







void NRF24L01_Init(void)
{
	NRF24L01_Init_Pin();
	SPI2_Init();
	
	HAL_GPIO_WritePin(NRF_24L01_CE_PORT, NRF_24L01_CE_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NRF_24L01_CSN_PORT, NRF_24L01_CSN_PIN, GPIO_PIN_SET);
	
	NRF24L01_RX_Mode();
	
	while (NRF24L01_Receive_Data() == NRF24L01_CONNECTION_FAIL)
	{
		Delay_ms(10);
	}
}






void NRF24L01_Init_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = NRF_24L01_CE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(NRF_24L01_CE_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = NRF_24L01_CSN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(NRF_24L01_CSN_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(NRF_24L01_CE_PORT, NRF_24L01_CE_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(NRF_24L01_CSN_PORT, NRF_24L01_CSN_PIN, GPIO_PIN_SET);
}








void NRF24L01_RX_Mode(void)
{
	
	HAL_GPIO_WritePin(NRF_24L01_CE_PORT, NRF_24L01_CE_PIN, GPIO_PIN_RESET); //Disable RX mode
	
	NRF24L01_Write_Register(RX_ADDR_P0, (uint8_t *) &RX_ADDRESS[0], RX_ADR_WIDTH); //Set address
	
	//initialize register
	for (uint8_t i = 0; i < NRF24L01_NUM_INIT_REGISTER; i++)
	{
		NRF24L01_Write_Register(NRF24L01_register_init[i][0], (uint8_t *) &(NRF24L01_register_init[i][1]), 1);
	}
	
	HAL_GPIO_WritePin(NRF_24L01_CE_PORT, NRF_24L01_CE_PIN, GPIO_PIN_SET); //Enable RX mode
}








void NRF24L01_Write_Register(uint8_t reg, uint8_t *pData, uint16_t size)
{
	uint8_t command = W_REGISTER + reg;
	
	HAL_GPIO_WritePin(NRF_24L01_CSN_PORT, NRF_24L01_CSN_PIN, GPIO_PIN_RESET);
	
	SPI2_Transmit(&command, 1);
	SPI2_Transmit(pData, size);
	
	HAL_GPIO_WritePin(NRF_24L01_CSN_PORT, NRF_24L01_CSN_PIN, GPIO_PIN_SET);
}













NRF24L01_Status NRF24L01_Receive_Data(void)
{
	NRF24L01_Status status = NRF24L01_CONNECTION_SUCCESS;
	uint8_t state_reg;
	uint8_t flush_rx = 0xFF;
	
	NRF24L01_Read_Register(STATUS, &state_reg, 1); //Get NRF24L01 status
	NRF24L01_Write_Register(STATUS, &state_reg, 1); // Clear STATUS TX_DS and MAX_RT bit
	
	if (state_reg & RF_OK)
	{
		NRF24L01_Read_Register(R_RX_PAYLOAD, &nrf24l01_rx_data[0], RX_PLOAD_WIDTH); //Get data
		NRF24L01_Write_Register((FLUSH_RX-W_REGISTER), &flush_rx, 1); // flush FIFO RX
	} 
	else 
	{
		status = NRF24L01_CONNECTION_FAIL;
	}
	
	return status;
}






NRF24L01_Status NRF24L01_Check_RX_Data(void)
{
	NRF24L01_Status status = NRF24L01_CONNECTION_SUCCESS;
	
	uint8_t nrf24l01_crc;
	
	for (uint8_t i = 0; i < 31; i++)
	{
		nrf24l01_crc += nrf24l01_rx_data[i];
	}
	
	if (nrf24l01_crc != nrf24l01_rx_data[31])
	{
		status = NRF24L01_CONNECTION_FAIL;
	}
	
	return status;
}






void NRF24L01_Get_RX_Data(uint8_t *pData)
{
	uint8_t size = sizeof(nrf24l01_rx_data);
	memcpy(pData, &nrf24l01_rx_data[0], size); //copy rx data
	memset(&nrf24l01_rx_data[0], 0, size); //clear rx data
}







NRF24L01_Status NRF24L01_Check_Connection(void)
{
	NRF24L01_Status status = NRF24L01_CONNECTION_SUCCESS;
	uint8_t send_buf[5] = {0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t receive_buf[5] = {0, 0, 0, 0, 0};
	
	NRF24L01_Write_Register(TX_ADDR, &send_buf[0], 5);
	NRF24L01_Read_Register(TX_ADDR, &receive_buf[0], 5);
	
	for (uint8_t i = 0; i < 5; i++)
	{
		if (receive_buf[i] != send_buf[i])
		{
			status = NRF24L01_CONNECTION_FAIL;
			break;
		}
	}
	
	return status;
}


