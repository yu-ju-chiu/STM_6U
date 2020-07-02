/*
 * HAL_UART_out.c
 *
 *  Created on: 2014年11月5日
 *      Author: ELied
 */

#include <stdio.h>
#include "stm32f429xx.h"
#include "HAL_UART_out.h"
#include "stm32f429i_discovery.h"
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
#if(USARTx_IDLE_INTERRUPT==1)
static void UART_SetConfig (UART_HandleTypeDef *huart);
#endif
/*#define MaxPrintByte	(50)
char temp_buffer[MaxPrintByte] ="";*/

/* UART handler declaration */
//UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
//uint8_t UART_TxBuffer[] = " **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling **** ";
//uint8_t UART_TxBuffer[] = " **** **** ****";
/* Buffer used for reception */
//uint8_t UART_RxBuffer[RXBUFFERSIZE];

extern __IO ITStatus UsartReady_0 = RESET;
extern __IO ITStatus UsartReady_Rx = RESET;

void STM32f4_UART_Init(UART_HandleTypeDef *huart){
	/*##-1.2- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = 9600 baud
		- Hardware flow control disabled (RTS and CTS signals) */
	USARTx_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();
	//USARTx_TX_GPIO_CLK_ENABLE();與USARTx_RX_GPIO_CLK_ENABLE()相同
	//__HAL_RCC_USART1_FORCE_RESET();

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = USART1_TX_PIN; // 設定第 09 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USART1_TX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/******** 設定 PC11 為 Rx 覆用  ********/
	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = GPIO_PIN_10; // 設定第 10 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USART1_RX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	huart->Instance          = USARTx;

	huart->Init.BaudRate     = 9600;
	huart->Init.WordLength   = UART_WORDLENGTH_8B;
	huart->Init.StopBits     = UART_STOPBITS_1;
	huart->Init.Parity       = UART_PARITY_NONE;
	huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart->Init.Mode         = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(huart);
	/*if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		Error_Handler();
	}*/

	/*##-3.2- Start the transmission process #####################################*/
	/* While the UART in reception process, user can transmit data through
	  "aTxBuffer" buffer */
	//HAL_UART_Transmit(huart, (uint8_t*)UART_TxBuffer, TXBUFFERSIZE, 5000);
	
	//=============
	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(USART1_IRQn, 6, 2);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	//=============
	
	
	HAL_UART_Transmit(huart, (uint8_t*)UsartTxBuffer_0, TXBUFFERSIZE, 5000);
//	print_string("\r\n");
	/*if(HAL_UART_Transmit(&UartHandle, (uint8_t*)UART_TxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
	{
		Error_Handler();
	}*/
}

void STM32f4_UARTn_Init(UART_HandleTypeDef *huart\
		, uint16_t USARTn_TX_PIN, GPIO_TypeDef *USARTn_TX_PORT, uint8_t USARTn_TX_AF\
		, uint16_t USARTn_RX_PIN, GPIO_TypeDef *USARTn_RX_PORT, uint8_t USARTn_RX_AF\
		, USART_TypeDef *USARTn, uint32_t BaudRate, uint8_t Parity)
{
	/*##-1.2- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = 9600 baud
		- Hardware flow control disabled (RTS and CTS signals) */
	/* Enable USART1 clock */
	if(USARTn==USART1)		{__HAL_RCC_USART1_CLK_ENABLE();}
	else if(USARTn==USART2)	{__HAL_RCC_USART2_CLK_ENABLE();}
	else if(USARTn==USART3)	{__HAL_RCC_USART3_CLK_ENABLE();}
	else if(USARTn==USART6)	{__HAL_RCC_USART6_CLK_ENABLE();}
	else if(USARTn==UART4)	{__HAL_RCC_UART4_CLK_ENABLE();}
	else if(USARTn==UART5)	{__HAL_RCC_UART5_CLK_ENABLE();}
	else if(USARTn==UART7)	{__HAL_RCC_UART7_CLK_ENABLE();}
	else if(USARTn==UART8)	{__HAL_RCC_UART8_CLK_ENABLE();}
	/* Enable USART GPIO clock */
	if((USARTn_TX_PORT==GPIOA)||(USARTn_RX_PORT==GPIOA)){__HAL_RCC_GPIOA_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOB)||(USARTn_RX_PORT==GPIOB)){__HAL_RCC_GPIOB_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOC)||(USARTn_RX_PORT==GPIOC)){__HAL_RCC_GPIOC_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOD)||(USARTn_RX_PORT==GPIOD)){__HAL_RCC_GPIOD_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOE)||(USARTn_RX_PORT==GPIOE)){__HAL_RCC_GPIOE_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOF)||(USARTn_RX_PORT==GPIOF)){__HAL_RCC_GPIOF_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOG)||(USARTn_RX_PORT==GPIOG)){__HAL_RCC_GPIOG_CLK_ENABLE();}

	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t temp_flag, UartInitMsg[] = " ^^^^ ^^^^ ^^^^";
	IRQn_Type USARTn_IRQ;

	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = USARTn_TX_PIN; // 設定第 09 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USARTn_TX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USARTn_TX_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/******** 設定 PC11 為 Rx 覆用  ********/
	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = USARTn_RX_PIN; // 設定第 10 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USARTn_RX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USARTn_RX_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	HAL_UART_DeInit(huart);

	huart->Instance          = USARTn;

	huart->Init.BaudRate     = BaudRate;
	huart->Init.WordLength   = UART_WORDLENGTH_8B;
	huart->Init.StopBits     = UART_STOPBITS_1;
	huart->Init.Parity       = UART_PARITY_NONE;
	huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart->Init.Mode         = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	switch(Parity)
	{
		case 0:huart->Init.Parity=UART_PARITY_NONE;break;//無校驗
		case 1:huart->Init.Parity=UART_PARITY_ODD;break;//奇數同位檢查
		case 2:huart->Init.Parity=UART_PARITY_EVEN;break;//偶校驗
	}
	if(HAL_UART_Init(huart) != HAL_OK)
	{
		Error_Handler();
	}
	/******** Configure NVIC ********/
	__HAL_UART_CLEAR_FLAG(huart, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
	temp_flag = 0;
	if(USARTn==USART1)	{USARTn_IRQ = USART1_IRQn;temp_flag=1;}
	else if(USARTn==USART2){USARTn_IRQ = USART2_IRQn;temp_flag=1;}
	else if(USARTn==USART3){USARTn_IRQ = USART3_IRQn;temp_flag=1;}
	else if(USARTn==USART6){USARTn_IRQ = USART6_IRQn;temp_flag=1;}
	else if(USARTn==UART4){USARTn_IRQ = UART4_IRQn;temp_flag=1;}
	else if(USARTn==UART5){USARTn_IRQ = UART5_IRQn;temp_flag=1;}
	else if(USARTn==UART7){USARTn_IRQ = UART7_IRQn;temp_flag=1;}
	else if(USARTn==UART8){USARTn_IRQ = UART8_IRQn;temp_flag=1;}
	/* NVIC configuration for USART TC interrupt */
	if(temp_flag==1)
	{
		HAL_NVIC_SetPriority(USARTn_IRQ, 7, 0);
		HAL_NVIC_EnableIRQ(USARTn_IRQ);
	}
	/*##-3.2- Start the transmission process #####################################*/
	/* While the UART in reception process, user can transmit data through
	  "aTxBuffer" buffer */
	//HAL_UART_Transmit(huart, (uint8_t*)UART_TxBuffer, TXBUFFERSIZE, 5000);
	//print_string("\r\n");
	HAL_UART_Transmit(huart, (uint8_t*)UartInitMsg, TXBUFFERSIZE, 5000);
	//print_string("\r\n");
	/*if(HAL_UART_Transmit(&UartHandle, (uint8_t*)UART_TxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
	{
		Error_Handler();
	}*/
}

void STM32f4_UART_DMA_Init(UART_HandleTypeDef *huart){
	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = 9600 baud
		- Hardware flow control disabled (RTS and CTS signals) */
	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t UartInitMsg[] = " **** **** ****";
	/* Enable USART1 clock */
	USARTx_CLK_ENABLE();
	/* Enable DMA1 clock */
	DMAx_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();
	//USARTx_TX_GPIO_CLK_ENABLE();//與USARTx_RX_GPIO_CLK_ENABLE()相同
	//__HAL_RCC_USART1_FORCE_RESET();

	/*##-2- Configure peripheral GPIO ##########################################*/
	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = USART1_TX_PIN; // 設定第 09 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USART1_TX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/******** 設定 PC11 為 Rx 覆用  ********/
	//GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	//GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = GPIO_PIN_10; // 設定第 10 腳
	//GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USART1_RX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	huart->Instance          = USARTx;

	huart->Init.BaudRate     = 115200;
	huart->Init.WordLength   = UART_WORDLENGTH_8B;
	huart->Init.StopBits     = UART_STOPBITS_1;
	huart->Init.Parity       = UART_PARITY_NONE;
	huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart->Init.Mode         = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
#if(USARTx_IDLE_INTERRUPT==1)
	huart->Instance->CR1 |= USART_CR1_IDLEIE;//開啟IDLE中斷
#endif
	HAL_UART_Init(huart);

	/*##-3- Configure the DMA streams ##########################################*/
	/* Configure the DMA handler for Transmission process */
	hdma_tx.Instance                 = USART1_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = USART1_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
#if(USARTx_IDLE_INTERRUPT==1)
	//hdma_tx.Init.Mode                = DMA_CIRCULAR;
#endif

	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_Init(&hdma_tx);

	/* Associate the initialized DMA handle to the the UART handle */
	__HAL_LINKDMA(huart, hdmatx, hdma_tx);

	/* Configure the DMA handler for Transmission process */
	hdma_rx.Instance                 = USART1_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = USART1_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	HAL_DMA_DeInit(&hdma_rx);
	HAL_DMA_Init(&hdma_rx);

	/* Associate the initialized DMA handle to the the UART handle */
	__HAL_LINKDMA(huart, hdmarx, hdma_rx);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
#if(USARTx_IDLE_INTERRUPT==0)
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 1);
#else
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 2);
#endif
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
#if(USARTx_IDLE_INTERRUPT==0)
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 5, 0);
#else
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 6, 1);
#endif
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

	/* NVIC configuration for USART TC interrupt */
#if(USARTx_IDLE_INTERRUPT==0)
	HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
#else
	HAL_NVIC_SetPriority(USART1_IRQn, 6, 2);
#endif
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	/*##-5- Start the transmission process #####################################*/
	/* While the UART in reception process, user can transmit data through
	  "UART_TxBuffer" buffer */
	print_string("\r\n");
	HAL_UART_Transmit_DMA(huart, (uint8_t*)UartInitMsg, TXBUFFERSIZE);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
	print_string_DMA("\r\n");
}

void STM32f4_UARTn_DMA_Init(UART_HandleTypeDef *huart\
		, uint16_t USARTn_TX_PIN, GPIO_TypeDef *USARTn_TX_PORT, uint8_t USARTn_TX_AF\
		, uint16_t USARTn_RX_PIN, GPIO_TypeDef *USARTn_RX_PORT, uint8_t USARTn_RX_AF\
		, USART_TypeDef *USARTn, uint32_t BaudRate\
		, DMA_Stream_TypeDef *USARTnTX_DMAstream, uint32_t USARTnTX_DMAchannel\
		, DMA_Stream_TypeDef *USARTnRX_DMAstream, uint32_t USARTnRX_DMAchannel)\
{
	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = 9600 baud
		- Hardware flow control disabled (RTS and CTS signals) */
	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t temp_flag, UartInitMsg[] = " ";
	/* Enable USART1 clock */
	if(USARTn==USART1)		{__HAL_RCC_USART1_CLK_ENABLE();}
	else if(USARTn==USART2)	{__HAL_RCC_USART2_CLK_ENABLE();}
	else if(USARTn==USART3)	{__HAL_RCC_USART3_CLK_ENABLE();}
	else if(USARTn==USART6)	{__HAL_RCC_USART6_CLK_ENABLE();}
	else if(USARTn==UART4)	{__HAL_RCC_UART4_CLK_ENABLE();}
	else if(USARTn==UART5)	{__HAL_RCC_UART5_CLK_ENABLE();}
	else if(USARTn==UART7)	{__HAL_RCC_UART7_CLK_ENABLE();}
	else if(USARTn==UART8)	{__HAL_RCC_UART8_CLK_ENABLE();}
	/* Enable DMA1 clock */
	if((USARTnTX_DMAstream==DMA1_Stream0)||(USARTnTX_DMAstream==DMA1_Stream1)||(USARTnTX_DMAstream==DMA1_Stream2)||(USARTnTX_DMAstream==DMA1_Stream3)||(USARTnTX_DMAstream==DMA1_Stream4)||(USARTnTX_DMAstream==DMA1_Stream5)\
	 ||(USARTnTX_DMAstream==DMA1_Stream6)||(USARTnTX_DMAstream==DMA1_Stream7)||(USARTnRX_DMAstream==DMA1_Stream0)||(USARTnRX_DMAstream==DMA1_Stream1)||(USARTnRX_DMAstream==DMA1_Stream2)||(USARTnRX_DMAstream==DMA1_Stream3)\
	 ||(USARTnRX_DMAstream==DMA1_Stream4)||(USARTnRX_DMAstream==DMA1_Stream5)||(USARTnRX_DMAstream==DMA1_Stream6)||(USARTnRX_DMAstream==DMA1_Stream7))	{__HAL_RCC_DMA1_CLK_ENABLE();}
	if((USARTnTX_DMAstream==DMA2_Stream0)||(USARTnTX_DMAstream==DMA2_Stream1)||(USARTnTX_DMAstream==DMA2_Stream2)||(USARTnTX_DMAstream==DMA2_Stream3)||(USARTnTX_DMAstream==DMA2_Stream4)||(USARTnTX_DMAstream==DMA2_Stream5)\
	 ||(USARTnTX_DMAstream==DMA2_Stream6)||(USARTnTX_DMAstream==DMA2_Stream7)||(USARTnRX_DMAstream==DMA2_Stream0)||(USARTnRX_DMAstream==DMA2_Stream1)||(USARTnRX_DMAstream==DMA2_Stream2)||(USARTnRX_DMAstream==DMA2_Stream3)\
	 ||(USARTnRX_DMAstream==DMA2_Stream4)||(USARTnRX_DMAstream==DMA2_Stream5)||(USARTnRX_DMAstream==DMA2_Stream6)||(USARTnRX_DMAstream==DMA2_Stream7))	{__HAL_RCC_DMA2_CLK_ENABLE();}
	/* Enable USART GPIO clock */
	if((USARTn_TX_PORT==GPIOA)||(USARTn_RX_PORT==GPIOA)){__HAL_RCC_GPIOA_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOB)||(USARTn_RX_PORT==GPIOB)){__HAL_RCC_GPIOB_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOC)||(USARTn_RX_PORT==GPIOC)){__HAL_RCC_GPIOC_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOD)||(USARTn_RX_PORT==GPIOD)){__HAL_RCC_GPIOD_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOE)||(USARTn_RX_PORT==GPIOE)){__HAL_RCC_GPIOE_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOF)||(USARTn_RX_PORT==GPIOF)){__HAL_RCC_GPIOF_CLK_ENABLE();}
	if((USARTn_TX_PORT==GPIOG)||(USARTn_RX_PORT==GPIOG)){__HAL_RCC_GPIOG_CLK_ENABLE();}

	/*##-2- Configure peripheral GPIO ##########################################*/
	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = USARTn_TX_PIN; // 設定第 09 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USARTn_TX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USARTn_TX_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOA

	/******** 設定 PC11 為 Rx 覆用  ********/
	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;// 設置為覆用
	GPIO_InitStructure.Pin = USARTn_RX_PIN; // 設定第 10 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 設定 GPIO 速度為 50 MHz
	GPIO_InitStructure.Alternate = USARTn_RX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(USARTn_RX_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOA

	huart->Instance          = USARTn;

	huart->Init.BaudRate     = BaudRate;
	huart->Init.WordLength   = UART_WORDLENGTH_8B;
	huart->Init.StopBits     = UART_STOPBITS_1;
	huart->Init.Parity       = UART_PARITY_NONE;
	huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart->Init.Mode         = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
#if(USARTx_IDLE_INTERRUPT==1)
	huart->Instance->CR1 |= USART_CR1_IDLEIE;//開啟IDLE中斷
#endif
#if(UART_ENABLE_DELAY==0)
	HAL_UART_Init(huart);
#else
	HAL_UART_Init_WithoutUARTenable(huart);
#endif

	/*##-3- Configure the DMA streams ##########################################*/
	/* Configure the DMA handler for Transmission process */
	hdma_tx.Instance                 = USARTnTX_DMAstream;

	hdma_tx.Init.Channel             = USARTnTX_DMAchannel;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
#if(USARTx_IDLE_INTERRUPT==1)
	//hdma_tx.Init.Mode                = DMA_CIRCULAR;
#endif

	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_Init(&hdma_tx);

	/* Associate the initialized DMA handle to the the UART handle */
	__HAL_LINKDMA(huart, hdmatx, hdma_tx);

	/* Configure the DMA handler for Transmission process */
	hdma_rx.Instance                 = USARTnRX_DMAstream;

	hdma_rx.Init.Channel             = USARTnRX_DMAchannel;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	HAL_DMA_DeInit(&hdma_rx);
	HAL_DMA_Init(&hdma_rx);

	/* Associate the initialized DMA handle to the the UART handle */
	__HAL_LINKDMA(huart, hdmarx, hdma_rx);

	/*##-4- Configure the NVIC for DMA #########################################*/

	IRQn_Type USARTnTxDMA_IRQ, USARTnRxDMA_IRQ, USARTn_IRQ;
	temp_flag = 0;
	if(USARTnTX_DMAstream==DMA1_Stream0)	{USARTnTxDMA_IRQ = DMA1_Stream0_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream1){USARTnTxDMA_IRQ = DMA1_Stream1_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream2){USARTnTxDMA_IRQ = DMA1_Stream2_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream3){USARTnTxDMA_IRQ = DMA1_Stream3_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream4){USARTnTxDMA_IRQ = DMA1_Stream4_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream5){USARTnTxDMA_IRQ = DMA1_Stream5_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream6){USARTnTxDMA_IRQ = DMA1_Stream6_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA1_Stream7){USARTnTxDMA_IRQ = DMA1_Stream7_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream0){USARTnTxDMA_IRQ = DMA2_Stream0_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream1){USARTnTxDMA_IRQ = DMA2_Stream1_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream2){USARTnTxDMA_IRQ = DMA2_Stream2_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream3){USARTnTxDMA_IRQ = DMA2_Stream3_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream4){USARTnTxDMA_IRQ = DMA2_Stream4_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream5){USARTnTxDMA_IRQ = DMA2_Stream5_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream6){USARTnTxDMA_IRQ = DMA2_Stream6_IRQn;temp_flag=1;}
	else if(USARTnTX_DMAstream==DMA2_Stream7){USARTnTxDMA_IRQ = DMA2_Stream7_IRQn;temp_flag=1;}
	/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
	if(temp_flag==1)
	{
#if(USARTx_IDLE_INTERRUPT==0)
		HAL_NVIC_SetPriority(USARTnTxDMA_IRQ, 5, 1);
#else
		HAL_NVIC_SetPriority(USARTnTxDMA_IRQ, 5, 1);
#endif
		HAL_NVIC_EnableIRQ(USARTnTxDMA_IRQ);
	}
	temp_flag = 0;
	if(USARTnRX_DMAstream==DMA1_Stream0)	{USARTnRxDMA_IRQ = DMA1_Stream0_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream1){USARTnRxDMA_IRQ = DMA1_Stream1_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream2){USARTnRxDMA_IRQ = DMA1_Stream2_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream3){USARTnRxDMA_IRQ = DMA1_Stream3_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream4){USARTnRxDMA_IRQ = DMA1_Stream4_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream5){USARTnRxDMA_IRQ = DMA1_Stream5_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream6){USARTnRxDMA_IRQ = DMA1_Stream6_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA1_Stream7){USARTnRxDMA_IRQ = DMA1_Stream7_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream0){USARTnRxDMA_IRQ = DMA2_Stream0_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream1){USARTnRxDMA_IRQ = DMA2_Stream1_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream2){USARTnRxDMA_IRQ = DMA2_Stream2_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream3){USARTnRxDMA_IRQ = DMA2_Stream3_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream4){USARTnRxDMA_IRQ = DMA2_Stream4_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream5){USARTnRxDMA_IRQ = DMA2_Stream5_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream6){USARTnRxDMA_IRQ = DMA2_Stream6_IRQn;temp_flag=1;}
	else if(USARTnRX_DMAstream==DMA2_Stream7){USARTnRxDMA_IRQ = DMA2_Stream7_IRQn;temp_flag=1;}
	/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
	if(temp_flag==1)
	{
#if(USARTx_IDLE_INTERRUPT==0)
		HAL_NVIC_SetPriority(USARTnRxDMA_IRQ, 5, 0);
#else
		HAL_NVIC_SetPriority(USARTnRxDMA_IRQ, 6, 0);
#endif
		HAL_NVIC_EnableIRQ(USARTnRxDMA_IRQ);
	}
	temp_flag = 0;
	if(USARTn==USART1)	{USARTn_IRQ = USART1_IRQn;temp_flag=1;}
	else if(USARTn==USART2){USARTn_IRQ = USART2_IRQn;temp_flag=1;}
	else if(USARTn==USART3){USARTn_IRQ = USART3_IRQn;temp_flag=1;}
	else if(USARTn==USART6){USARTn_IRQ = USART6_IRQn;temp_flag=1;}
	else if(USARTn==UART4){USARTn_IRQ = UART4_IRQn;temp_flag=1;}
	else if(USARTn==UART5){USARTn_IRQ = UART5_IRQn;temp_flag=1;}
	else if(USARTn==UART7){USARTn_IRQ = UART7_IRQn;temp_flag=1;}
	else if(USARTn==UART8){USARTn_IRQ = UART8_IRQn;temp_flag=1;}
	/* NVIC configuration for USART TC interrupt */
	if(temp_flag==1)
	{
#if(USARTx_IDLE_INTERRUPT==0)
		HAL_NVIC_SetPriority(USARTn_IRQ, 6, 0);
#else
		HAL_NVIC_SetPriority(USARTn_IRQ, 6, 1);
#endif
		HAL_NVIC_EnableIRQ(USARTn_IRQ);
	}

}
void print_u32d(unsigned int int32){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%u\r\n", int32);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt, 5000);
}
void print_u8x(uint8_t int8){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "0x%02X\r\n", int8);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt, 5000);
}
void print_u32x(unsigned int int32){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "0x%08X\r\n", int32);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt, 5000);
}
void print_32d(int int32){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%d\r\n", int32);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt, 5000);
}
void print_u8d_DMA(uint8_t uint8){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%u\r\n", uint8);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_u8x_DMA(uint8_t uint8x){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "0x%02X\r\n", uint8x);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_8d_DMA(int8_t int8){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%d\r\n", int8);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_u32d_DMA(unsigned int uint32){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%u\r\n", uint32);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_u32x_DMA(unsigned int uint32x){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "0x%08X\r\n", uint32x);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_32d_DMA(int int32){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%d\r\n", int32);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_float_DMA(float f1){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%4.2f\r\n", f1);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_double_DMA(double d1){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%4.10f\r\n", d1);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}
void print_string(char str[]){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%s", str);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt, 5000);
}
void print_string_DMA(char str[]){
	uint8_t print_cnt=0;
	sprintf(UsartTxBuffer_0, "%s", str);
	while(UsartTxBuffer_0[print_cnt]!='\0'){print_cnt++;}
	HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)UsartTxBuffer_0, print_cnt);
	while (UsartReady_0 != SET){}
	UsartReady_0 = RESET;
}

/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void USART1_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void USART1_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

/**
  * @brief  This function handles USARTx interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Set transmission flag: transfer complete*/
	UsartReady_0 = SET;
	/* Turn LED3 on: Transfer in transmission process is correct */
	BSP_LED_Toggle(LED3);
}

 /**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
int UART_Rx_flag=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Set transmission flag: transfer complete*/
	//UsartReady_0 = SET;
	UsartReady_Rx = SET;
	UART_Rx_flag = 1;

	/* Turn LED4 on: Transfer in reception process is correct */
//	BSP_LED_Toggle(LED4);
}
