/*
 * HAL_CAN_INIT.h
 *
 *  Created on: 2016¦~01¤ë06¤é
 *      Author: ELied
 */

void put_float(float a, uint8_t *Data);
float get_float(uint8_t *Data);
void put_double(double a, uint8_t *Data);
double get_double(uint8_t *Data);

void STM32f4_CAN_Init(CAN_HandleTypeDef *hcan,   CAN_FilterConfTypeDef *FilterConf, CanTxMsgTypeDef *TxMsg, CanRxMsgTypeDef *RxMsg);
void STM32f4_CAN_MeasureResultReceive(CAN_HandleTypeDef *hcan);
void STM32f4_CAN_MeasureResultReceive_INT8wTW(CAN_HandleTypeDef *hcan, uint8_t *totle_track_num, int8_t *DistanceOffsetStorgeMatrix, int8_t *TrackEdgeStorgeMatrix);

#define CAN_TRANSMIT_UNLOCK				(1)
#define CAN_RECEIVE_IRQ_UNLOCK			(1)
