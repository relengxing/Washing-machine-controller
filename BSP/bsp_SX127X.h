/***********************(C) COPY RIGHT 2011 nenglian tech*************************
* File Name: Bsp_SX127X.h
* Author: zhangdan
* Date First Issued: 2014-11-14
* Version 1.0.0
* Description: head file for Bsp_Fm.c
*******************************************************************************
* History
* 2014-11-14:  V1.0
* Description: created
*******************************************************************************/

#ifndef __BSP_SX127X_H_
#define __BSP_SX127X_H_

#include "bsp_config.h"



void SX127XCS(void);
void SX127XDisCS(void);
void RST_RF_Pin_Control(uint8_t NewState);
void RX_TX_CTRLPin_Control(uint8_t NewState);
uint8_t D0Pin_Control(void);
uint8_t D1Pin_Control(void);
void LED_RFRXD_GPin_Control(uint8_t NewState);
void LED_RFTXD_RPin_Control(uint8_t NewState); 
void EN_PE4259Pin_Control(uint8_t NewState);
INT8U SPI2_SendRcvByte(INT8U dt);
void SX1276_RelatePIN_Open(void);

void Bsp_Rf_PowerOffPinConfig(void);
#endif //
