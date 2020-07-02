/*
 * HAL_UART_out.h
 *
 *  Created on: 2014年11月5日
 *      Author: ELied
 */

#ifndef HAL_UART_OUT_H_
#define HAL_UART_OUT_H_

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void STM32f4_UART_Init(UART_HandleTypeDef *huart);
void STM32f4_UARTn_Init(UART_HandleTypeDef *huart\
		, uint16_t USARTn_TX_PIN, GPIO_TypeDef *USARTn_TX_PORT, uint8_t USARTn_TX_AF\
		, uint16_t USARTn_RX_PIN, GPIO_TypeDef *USARTn_RX_PORT, uint8_t USARTn_RX_AF\
		, USART_TypeDef *USARTn, uint32_t BaudRate, uint8_t Parity);
void STM32f4_UART_DMA_Init(UART_HandleTypeDef *huart);
void STM32f4_UARTn_DMA_Init(UART_HandleTypeDef *huart\
		, uint16_t USARTn_TX_PIN, GPIO_TypeDef *USARTn_TX_PORT, uint8_t USARTn_TX_AF\
		, uint16_t USARTn_RX_PIN, GPIO_TypeDef *USARTn_RX_PORT, uint8_t USARTn_RX_AF\
		, USART_TypeDef *USARTn, uint32_t BaudRate\
		, DMA_Stream_TypeDef *USARTnTX_DMAstream, uint32_t USARTnTX_DMAchannel\
		, DMA_Stream_TypeDef *USARTnRX_DMAstream, uint32_t USARTnRX_DMAchannel);
void print_u32d(unsigned int int32);
void print_u8x(uint8_t int8);
void print_u32x(unsigned int int32);
void print_32d(int int32);
void print_u8d_DMA(uint8_t uint8);
void print_u8x_DMA(uint8_t uint8x);
void print_8d_DMA(int8_t int8);
void print_u32d_DMA(unsigned int uint32);
void print_u32x_DMA(unsigned int uint32x);
void print_32d_DMA(int int32);

void print_float_DMA(float f1);
void print_double_DMA(double d1);

void print_string(char str[]);
void print_string_DMA(char str[]);

void USART1_IRQHandler(void);
void USART1_DMA_RX_IRQHandler(void);
void USART1_DMA_TX_IRQHandler(void);

/* User can use this section to tailor USARTx/UARTx instance used and associated resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for USARTx Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_AF                     GPIO_AF7_USART1
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's DMA */
/* Definition for USARTx's NVIC */
#define USART1_DMA_TX_IRQn               DMA2_Stream7_IRQn
#define USART1_DMA_RX_IRQn               DMA2_Stream5_IRQn
#define USART1_DMA_TX_IRQHandler         DMA2_Stream7_IRQHandler
#define USART1_DMA_RX_IRQHandler         DMA2_Stream5_IRQHandler
//#define USART1_IRQn                      USART1_IRQn
#define USART1_IRQHandler                USART1_IRQHandler

/* Definition for USARTx's DMA */
#define USART1_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define USART1_TX_DMA_STREAM             DMA2_Stream7
#define USART1_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define USART1_RX_DMA_STREAM             DMA2_Stream5

/* Size of Transmission buffer */
//#define TXBUFFERSIZE                     (COUNTOF(UART_TxBuffer) - 1)
#define TXBUFFERSIZE                     (7)	//Unit:byte
/* Size of Reception buffer */
#define RXBUFFERSIZE                     TXBUFFERSIZE

#define MaxPrintByte					(300)
#define MaxReceiveByte					(96)	//重要! UART_RX_DMA buffer緩衝區容量,同時也作為RX buffer容量.


#if(USARTx_IDLE_INTERRUPT==1)
uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Channelx);
#endif

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
__IO ITStatus UsartReady_0, UsartReady_Rx;

/* Buffer used for transmission and reception*/
uint8_t UsartTxBuffer_0[MaxPrintByte], UsartRxBuffer_0[MaxReceiveByte];
#endif
