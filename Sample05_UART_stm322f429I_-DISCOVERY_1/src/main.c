/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComPolling/Src/main.c 
  * @author  MCD Application Team
  * @version V1.2.5
  * @date    29-January-2016 
  * @brief   This sample code shows how to use STM32F4xx UART HAL API to transmit 
  *          and receive a data buffer with a communication process based on
  *          polling transfer. 
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "HAL_UART_out.h"
#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_rcc.h>

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComPolling
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define TRANSMITTER_BOARD 
/*can*/
#define KEY_PRESSED     0x01
#define KEY_NOT_PRESSED 0x00
#define TelID(OP,VAL) (OP<<4|VAL)
static int flag=0;
//static int UART_Rx_flag;
static int CAN_flag;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling **** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
__IO ITStatus UartReady = RESET;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  /*__attribute__ ((aligned (8))) */
  uint32_t txbox_num;
  ubKeyNumber = 0x0;
  RxMessagePending_flag = 0;
  totle_track_num = 0;
  uint8_t press_cnt = 0;

  HAL_Init();
  
  /* Configure LED3 and LED4 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
//  init_GPIO();
  /*�ڪ��D�ncode*/
  /*test git*/
  /*test_git_2*/
  STM32f4_UART_Init(&UartHandle);
  print_string("UART init done.\r\n");
  STM32f4_CAN_Init(&CanHandle, &sFilterConfig, &TxMessage, &RxMessage);
  print_string("CAN init done.\r\n");
//  if(HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)
//  {
//    /* Reception Error */
//	  Error_Handler();
//  }
  /* Configure USER Button */
//  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  /* Wait for USER Button press before starting the Communication */
//  while (BSP_PB_GetState(BUTTON_KEY) == RESET)
//  {
//  }
//  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Turn LED3 on: Transfer in transmission process is correct */
//    /* then Off for next transmission */
//    BSP_LED_On(LED3);
//    HAL_Delay(200);
//    BSP_LED_Off(LED3);
//
//    /*##-3- Put UART peripheral in reception process ###########################*/
//    if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
//    {
//      Error_Handler();
//    }

    /* Turn LED3 on: Transfer in transmission process is correct */
    /* then Off for next transmission */
     BSP_LED_On(LED3);
    HAL_Delay(200);
    BSP_LED_Off(LED3);
    BSP_LED_On(LED4);
    HAL_Delay(200);
    BSP_LED_Off(LED4);
    print_string("please insert poweron or poweroff.\r\n");
	 if(HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)
	 {
	   /* Reception Error */
		  Error_Handler();
	 }
//	 if(HAL_UART_Receive_IT(&UartHandle, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
//	 {
//		    //Error_Handler();
//	 }
  extern int UART_Rx_flag;
  while(1)
  {

//	  HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
	  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
	  {
		     Error_Handler();
      }

	  if(flag == 1)
	  {
		  press_cnt = 0;
		  CanHandle.pTxMsg->StdId = 0x302;
		  CanHandle.pTxMsg->IDE = CAN_ID_STD;
		  CanHandle.pTxMsg->DLC = 1;
		  CanHandle.pTxMsg->Data[0] = TelID(0X01,0X01);
		  if(HAL_CAN_Transmit(&CanHandle, 10) != HAL_OK)//0ms
		  {
		  }
		  press_cnt = 0;
		  print_string("CAN TX(0x302):\r\n");
		  print_u32x(CanHandle.pTxMsg->StdId);


		  if(CAN_flag==1)
		  {
			    BSP_LED_On(LED3);
				print_string("CAN RX(0x302):\r\n");
				print_u32x(CanHandle.pRxMsg->StdId);
				print_string("DATA Loading.\r\n");
				print_string("flag:");
				print_u8x(flag);
				HAL_Delay(1000);
				BSP_LED_Off(LED3);
		  }

	  }

	  if(flag == 2)
	  {
		  press_cnt = 0;
		  CanHandle.pTxMsg->StdId = 0x202;
		  CanHandle.pTxMsg->IDE = CAN_ID_STD;
		  CanHandle.pTxMsg->DLC = 1;
		  CanHandle.pTxMsg->Data[0] = TelID(0X03,0X00);
//		  print_string("CAN TX SECOND:\r\n");
		  if(HAL_CAN_Transmit(&CanHandle, 10) != HAL_OK)//0ms
		  {
		  }
		  press_cnt = 0;
		  print_string("CAN TX(0x202):\r\n");
		  print_u32x(CanHandle.pTxMsg->StdId);
		  if(CAN_flag==2)
		  {
				print_string("CAN RX(0x202):\r\n");
				print_u32x(CanHandle.pRxMsg->StdId);
				print_string("DATA loading over.\r\n");
				flag = 0;
		  }
	  }
	  if(UART_Rx_flag == 1)
	  {
		  if(aRxBuffer[6]==0x6E)
		  {
			  print_string("power on\r\n");
			  flag = 1;
		  }
		  else
		  {
			  print_string("power off");
		  }
		  UART_Rx_flag = 0;
	  }
//	  press_cnt = 0;
//	  CanHandle.pTxMsg->StdId = 0x202;
//	  CanHandle.pTxMsg->IDE = CAN_ID_STD;
//	  CanHandle.pTxMsg->DLC = 1;
//	  CanHandle.pTxMsg->Data[0] = TelID(0X03,0X00);
//	  HAL_CAN_Transmit(&CanHandle, 0);//0ms
//	  HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
//	  press_cnt = 0;


  }

  /*�ڪ��D�ncode*/

#ifdef TRANSMITTER_BOARD

  /* Configure USER Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  /* Wait for USER Button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
  }
  
  /* The board sends the message and expects to receive it back */
  
  /*##-2- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
  {
    Error_Handler();   
  }
  
  /* Turn LED3 on: Transfer in transmission process is correct */
  /* then Off for next transmission */
  BSP_LED_On(LED3);
  HAL_Delay(200);
  BSP_LED_Off(LED3);
  
  /*##-3- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
  {
    Error_Handler();  
  }
    
  /* Turn LED3 on: Transfer in transmission process is correct */
  /* then Off for next transmission */
  BSP_LED_On(LED3);
  HAL_Delay(200);
  BSP_LED_Off(LED3);
  
#else
  
  /* The board receives the message and sends it back */

  /*##-2- Put UART peripheral in reception process ###########################*/
  //print_u8x(8);

  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
  //{
//	 print_string("HAL_UART_Receive(&UartHandle, ...) Failed.\r\n");
     //Error_Handler();
  //}
  //print_string("HAL_UART_Receive(&UartHandle, ...) Done.\r\n");
  /* Turn LED3 on: Transfer in transmission process is correct */
  /* then Off for next transmission */
  BSP_LED_On(LED3);
  HAL_Delay(200);
  BSP_LED_Off(LED3);
  print_string("Toggled LED3.\r\n");
  }
  while(1);
  /*##-3- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */

  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
  {
    Error_Handler();
  }

  print_string("HAL_UART_Transmit.. Done.\r\n");
  /* Turn LED3 on: Transfer in transmission process is correct */
  /* then Off for next transmission */
  BSP_LED_On(LED3);
  HAL_Delay(200);
  BSP_LED_Off(LED3);
  
#endif /* TRANSMITTER_BOARD */
  
  /*##-4- Compare the sent and received buffers ##############################*/
  // If aTxBuffer != aRxBuffer in content, go to Error_Handler().
  if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,RXBUFFERSIZE))
  {
    Error_Handler();  
  }
  
  /* Infinite loop */
  while (1)
  {
	print_string("Test \r\n");
	print_u8x(8);
//	HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_13);
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    
    /* Wait for 40ms */
    HAL_Delay(500);
  }
}	//end main()







/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */


void init_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct = {
		.Pin = GPIO_PIN_13| GPIO_PIN_14,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
		.Pull =GPIO_PULLDOWN
	};
	__HAL_RCC_GPIOG_CLK_ENABLE();
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}






static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED4 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED4); 
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{
	static int digitStatus = 1;
	uint8_t CAN_Rx_IT_State=0xF;
	 if(hcan->pRxMsg->StdId ==0x302)
	 {

		CAN_flag = 1;
		flag = 2;
	 }
	 if(hcan->pRxMsg->StdId == 0x202)
	 {
		 CAN_flag = 2;
	     flag = 0;
	 }
//	 if(HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)
//	 {
//	   /* Reception Error */
////		  Error_Handler();
//	 }
	 CAN_Rx_IT_State = HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
	 if(CAN_Rx_IT_State != HAL_OK)
	 {
		 print_string("CabRxITState:");print_u32d(CAN_Rx_IT_State);
//		 print_string("hcan->State:");print_u32d(tmp_record);
		 while(1);
		 //Error_Handler();
	 }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */

//  BSP_LED_On(LED4);
  while(1)
  {
	  HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14);
	  HAL_Delay(500);
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
