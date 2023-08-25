#ifndef __MODULE_USART_H
#define __MODULE_USART_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"

#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

#define  Array_Size(Array)				(sizeof(Array)/sizeof((Array)[0]))		

void USART_Config(void);
void Usart_SendByte( USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_Send_32Word( USART_TypeDef *pUSARTx, uint32_t ch );
void Usart_SendArray( USART_TypeDef *pUSARTx, uint8_t *Array, uint8_t Array_Size );
void Usart_String( USART_TypeDef *pUSARTx, uint8_t *String );

#endif