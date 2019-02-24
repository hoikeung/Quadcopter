


#include "nrf24l01.h"
#include "SPI.h"
#include <string.h>

#undef SUCCESS
#define SUCCESS 0
#undef FAILED
#define FAILED  1


#define MAX_TX  		0x10  
#define TX_OK   		0x20  
#define RX_OK   		0x40  




#define SPI_CSN_H    (GPIOB->BSRR = GPIO_Pin_12)
#define SPI_CSN_L     (GPIOB->BRR = GPIO_Pin_12)  
#define NRF24L01_CE_H (GPIOA->BSRR = GPIO_Pin_11)   
#define NRF24L01_CE_L  (GPIOA->BRR = GPIO_Pin_11)  
#define NRF24L01_IRQ   (GPIOB->IDR&GPIO_Pin_0)


#include "nrf24l01.h"
#include "SPI.h"
#include <string.h>

#undef SUCCESS
#define SUCCESS 0
#undef FAILED
#define FAILED  1

const uint8_t TX_ADDRESS[]= {0x11,0x22,0x33,0x44,0x55};	
const uint8_t RX_ADDRESS[]= {0x11,0x22,0x33,0x44,0x55};	




#define Set_NRF24L01_CSN    (GPIOB->BSRR = GPIO_Pin_12) 
#define Clr_NRF24L01_CSN     (GPIOB->BRR = GPIO_Pin_12)   
#define Set_NRF24L01_CE (GPIOA->BSRR = GPIO_Pin_11)   
#define Clr_NRF24L01_CE  (GPIOA->BRR = GPIO_Pin_11)   
#define READ_NRF24L01_IRQ   (GPIOA->IDR&GPIO_Pin_8)


void NRF24L01_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                  
	Set_NRF24L01_CSN;                                  


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);

	SPI_Config();                               
	Clr_NRF24L01_CE; 	                            
	Set_NRF24L01_CSN;                                 
}



u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);          	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;					   
	if(i!=5)return 1;                            
	return 0;		                         
}	 	 


u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;              
  	status =SPI_RW(regaddr); 
  	SPI_RW(data);          
  	Set_NRF24L01_CSN;                 
  	return(status);       		    
}



u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
	Clr_NRF24L01_CSN;               		
  	SPI_RW(regaddr);  
  	reg_val=SPI_RW(0XFF);
  	Set_NRF24L01_CSN;               		    
  	return(reg_val);             
}	




u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;              
  	status=SPI_RW(regaddr);   	   
	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI_RW(0XFF);
  	Set_NRF24L01_CSN;                  
  	return status;                       
}



u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
	Clr_NRF24L01_CSN;                               
  	status = SPI_RW(regaddr);              
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI_RW(*pBuf++); 
  	Set_NRF24L01_CSN;                                  
  	return status;                                   
}				   




u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;   
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
	Set_NRF24L01_CE;                                     
	while(READ_NRF24L01_IRQ!=0);                       
	state=NRF24L01_Read_Reg(STATUS);                    
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);   
	if(state&MAX_TX)                                   
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);             
		return MAX_TX; 
	}
	if(state&TX_OK)                                   
	{
		return SUCCESS;
	}
	return FAILED;                                      
}




u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                  	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); 
	if(state&RX_OK)                               
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX,0xff);       
		return SUCCESS; 
	}	   
	return FAILED;                                    
}




void RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);

     
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    
   	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); 
	 	  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,45);	 
  	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+NCONFIG, 0x0f); 
   
  	Set_NRF24L01_CE;                                
}			



void TX_Mode(void)
{														 
	Clr_NRF24L01_CE;	    
   
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    
   
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 

   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     
   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); 
   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);
  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,45);      
   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //0x27  250K   0x07 1M
   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+NCONFIG,0x0e);    

	Set_NRF24L01_CE;                                  
}		  

void NRF24L01_init(void)
{
	NRF24L01_Configuration();
  do
	{
    RX_Mode();
	}while(NRF24L01_Check() == FAILED);
}




























