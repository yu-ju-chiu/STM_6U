/*
 * HAL_CAN_INIT.c
 *
 *  Created on: 2017年01月06日
 *      Author: ELied
 */

/*#include <stdio.h>*/
#include "stm32f429xx.h"
#include "main.h"

void STM32f4_CAN_Init(CAN_HandleTypeDef *hcan,   CAN_FilterConfTypeDef *FilterConf, CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg)
{
	/*Enable CAN & GPIO clock*/
	CANx_CLK_ENABLE();
	CANx_GPIO_CLK_ENABLE();

	/*Init GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = CANx_TX_PIN; // 設定第 01 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 設定 GPIO 速度為  50~200 MHz
	GPIO_InitStructure.Alternate = CANx_TX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/******** 設定 PC11 為 Rx 覆用  ********/
	GPIO_InitStructure.Pull = GPIO_PULLUP; // 使用上拉電阻
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 設置為覆用
	GPIO_InitStructure.Pin = CANx_RX_PIN; // 設定第 02 腳
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 設定 GPIO 速度為 50~200 MHz
	GPIO_InitStructure.Alternate = CANx_RX_AF; // 設定 GPIO 速度為 50 MHz	//UART TX&RX Alternate function皆相同 故僅需初始化一次
	HAL_GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStructure); // 套用以上 GPIO 設置，並初始化 GPIOC

	/*CAN_Config*/

	/*##-1- Configure the CAN peripheral #######################################*/
	hcan->Instance = CANx;
	hcan->pTxMsg = TxMsg;
	hcan->pRxMsg = RxMsg;

	hcan->Init.TTCM = DISABLE;
	hcan->Init.ABOM = ENABLE;
	hcan->Init.AWUM = DISABLE;
	hcan->Init.NART = DISABLE;
	hcan->Init.RFLM = DISABLE;
	hcan->Init.TXFP = DISABLE;

	//hcan->Init.Mode = CAN_MODE_NORMAL; //CAN_MODE_SILENT_LOOPBACK;
	hcan->Init.Mode = CAN_MODE_LOOPBACK;
	hcan->Init.SJW = CAN_SJW_1TQ;//設定CAN的傳輸速率
	hcan->Init.BS1 = CAN_BS1_2TQ;
	hcan->Init.BS2 = CAN_BS2_2TQ;
	hcan->Init.Prescaler = 9;    //定義bit的時間
	if(HAL_CAN_Init(hcan) != HAL_OK)
	{
	  /* Initialization Error */
	}

	/*##-2- Configure the CAN Filter ###########################################*/
	FilterConf->FilterNumber = 0;
	FilterConf->FilterMode = CAN_FILTERMODE_IDMASK;
	FilterConf->FilterScale = CAN_FILTERSCALE_32BIT;
	FilterConf->FilterIdHigh = 0x0000;//定義id應該是 RX(前兩個)
	FilterConf->FilterIdLow = 0x0000; //定義id應該是id RX(後兩個)
	FilterConf->FilterMaskIdHigh = 0x0000;//0000都不比
	FilterConf->FilterMaskIdLow = 0x0000; //ffff都比
	FilterConf->FilterFIFOAssignment = 0;
	FilterConf->FilterActivation = ENABLE;
	FilterConf->BankNumber = 14;

	if(HAL_CAN_ConfigFilter(hcan, FilterConf) != HAL_OK)
	{
	  /* Filter configuration Error */
	}

	/*##-3- Configure Transmission process #####################################*/
	hcan->pTxMsg->StdId = 0x321;
	hcan->pTxMsg->ExtId = 0x01;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->DLC = 2;

	/*##-3- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt (CAN1_TX) */
/*	HAL_NVIC_SetPriority(CANx_TX_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(CANx_TX_IRQn);*/
	/* NVIC configuration for DMA transfer complete interrupt (CAN1_RX) */
	HAL_NVIC_SetPriority(CANx_RX_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(CANx_RX_IRQn);
}

void put_float(float a, uint8_t *Data)
{
	memcpy(Data, &a, sizeof(float));
}
float get_float(uint8_t *Data)
{
  float a;
  a = *(float*)Data;
  return a;
}
void put_double(double a, uint8_t *Data)
{
	memcpy(Data, &a, sizeof(double));
}
double get_double(uint8_t *Data)
{
  double a;
  a = *(double*)Data;
  return a;
}

#include "HAL_UART_out.h"

void STM32f4_CAN_MeasureResultReceive(CAN_HandleTypeDef *hcan)
{
	if( (hcan->pRxMsg->Data[0]==((START_SYNBOL_24BITS&(0xFF<<16))>>16))&&(hcan->pRxMsg->Data[1]==((START_SYNBOL_24BITS&(0xFF<<8))>>8))&&(hcan->pRxMsg->Data[2]==(START_SYNBOL_24BITS&(0xFF))) )
	{
		float DistanceOffset_Est;
		DistanceOffset_Est = get_float(&(hcan->pRxMsg->Data[4]));
		//print_string_DMA("TrackNumber:");print_u8x_DMA(hcan->pRxMsg->Data[3]);
		//print_string_DMA("DistanceOffset_Est:");print_float_DMA(DistanceOffset_Est);
	}
}

void STM32f4_CAN_MeasureResultReceive_INT8wTW(CAN_HandleTypeDef *hcan, uint8_t *totle_track_num, int8_t *DistanceOffsetStorgeMatrix, int8_t *TrackEdgeStorgeMatrix)
{
	if( (hcan->pRxMsg->Data[0]==((START_SYNBOL_24BITS&(0xFF<<16))>>16))&&(hcan->pRxMsg->Data[1]==((START_SYNBOL_24BITS&(0xFF<<8))>>8))&&(hcan->pRxMsg->Data[2]==(START_SYNBOL_24BITS&(0xFF))) )
	{
		(*totle_track_num) = hcan->pRxMsg->Data[3];
		if((*totle_track_num)==1)
		{
			DistanceOffsetStorgeMatrix[0] = (int8_t)((int)(hcan->pRxMsg->Data[4])-(int)OFFSET_DISTANCE_LIMITS*10);
			TrackEdgeStorgeMatrix[0] = (int8_t)((int)(hcan->pRxMsg->Data[5])-(int)OFFSET_DISTANCE_LIMITS*10);
			TrackEdgeStorgeMatrix[1] = (int8_t)((int)(hcan->pRxMsg->Data[6])-(int)OFFSET_DISTANCE_LIMITS*10);
		}
		else if((*totle_track_num)>1)
		{
			for(int i=0;i<(*totle_track_num);i++)
			{
				DistanceOffsetStorgeMatrix[i] = (int8_t)((int)(hcan->pRxMsg->Data[4+i])-(int)OFFSET_DISTANCE_LIMITS*10);
			}
		}
		//print_string_DMA("TrackNumber:");print_u8x_DMA(hcan->pRxMsg->Data[3]);
		//for(int i=0;i<totle_track_num;i++){print_string_DMA("TW");print_u32d_DMA(i);print_32d_DMA(DistanceOffsetStorgeMatrix[i]);}
	}
}
