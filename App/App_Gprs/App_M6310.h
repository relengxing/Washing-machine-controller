/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : sysTick.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include	"Bsp_config.h " 
#include	"Bsp.h " 
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "systick.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "App_Head.h"
#include "Bsp_Uart.h"
#include "bsp_power_ctrl.h"
#include "bsp_adc.h"
#include "bsp_iic.h"
#include "App_Para_Interface.h"
#include "convertDataInterface.h"
#include "bsp_key.h"
#include "bsp_Led.h"
#include "App_NL.h"
/*
开关机



*/
/* Private typedef -----------------------------------------------------------*/
//typedef enum{
//	Gprs_XX;

//}Gprs_XXXX;
/* Private define ------------------------------------------------------------*/
extern INT8U KEY_STEP;
/* Private macro -------------------------------------------------------------*/
extern INT8U GPRS_Interval;
extern INT8U GPRS_Time;
/* Private variables ---------------------------------------------------------*/



void App_Gprs_Set(void);
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void App_Gprs_Set_WorkState(INT8U flag);
INT8U App_Gprs_Get_WorkState(void);

void App_Gprs_Task_TimeCall(void);					//延时函数
INT8U App_Gprs_GetCSQ(INT8U *dataBuff,INT8U lenth);
void App_GprsCommInit(void);
void App_Gprs_SetConentIP(INT8U IP1,INT8U IP2,INT8U IP3,INT8U IP4,INT16U Port);
void App_Gprs_GetConentIP(void);
void App_Gprs_Power_On(void);
void App_Gprs_Repower(void);
void App_Gprs_Main(void);


/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

