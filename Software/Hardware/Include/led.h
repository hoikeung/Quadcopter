
#ifndef __LED_H__

#define __LED_H__

#include "stm32f1xx_hal.h"

#define LED_PORT GPIOB

#define LED1	GPIO_PIN_1
#define	LED2	GPIO_PIN_9
#define	LED3	GPIO_PIN_2
#define	LED4	GPIO_PIN_8

#define	LED_ON	GPIO_PIN_RESET
#define	LED_OFF	GPIO_PIN_SET

void LED_Init(void);
void LED_Write(uint16_t GPIO_Pin, GPIO_PinState PinState);
void LED_Toogle(uint16_t GPIO_Pin);
void LED_Flash(uint16_t GPIO_Pin, uint8_t num, GPIO_PinState final_state);

#endif
