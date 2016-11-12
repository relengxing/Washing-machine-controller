/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/
#ifndef _APP_485RN_H
#define _APP_485RN_H
/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "sysTick.h"
#include "bsp_power_ctrl.h"
#include "App_Para_Interface.h"
#include "App_Data_interface.h " 
#include "App_Head.h"
#include "Bsp_Uart.h"
#include "bsp_rtc.h"
#include "bsp_Led.h"
#include "App_NL.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum __App_State_Enum__
{
	APP_485_STATE_POWER_OFF,
	APP_485_STATE_POWER_ON,
	APP_485_STATE_START,
	APP_485_STATE_WAIT,
	APP_485_STATE_RESTART,
	APP_485_STATE_READMETER_OK,
	APP_485_STATE_READMETER_FAIL,
}__App_State_Enum;
typedef enum __App_Setup_Enum__
{	
	APP_485_SETUP_POWER_OFF,
	APP_485_SETUP_POWER_ON,
	APP_485_SETUP_READ_METER,
	APP_485_SETUP_CLOSE,
}__App_Setup_Enum;

typedef struct	__App_485_Globe_Str__				
{
	__App_State_Enum	gState;
	__App_Setup_Enum	gSetup;
	INT32U  gDelayTime;
	INT32U	MAX_DelayTime;
	INT8U	gReCount;
	INT8U	MAX_ReCount;
	INT8U   WorkState ;
} __App_485_Globe_Str;


typedef struct			
{
	INT8U FrameStart;											
	INT8U MeterType;										
	INT8U Address[7];										
	INT8U Contorl;											
	INT8U DataLenth;										
	INT8U *Data;										
	INT8U CS;												
	INT8U FrameStop;										
}__APP_485_T188_Frame;
/* Private define ------------------------------------------------------------*/
#define APP_485_DELAY_1S		1000
#define APP_485_DELAY_2S		(2*APP_485_DELAY_1S)
#define APP_485_DELAY_3S		(3*APP_485_DELAY_1S)
#define APP_485_DELAY_5S		(5*APP_485_DELAY_1S)
#define APP_485_DELAY_60S		(60*APP_485_DELAY_1S)
#define APP_485_DELAY_180S		(180*APP_485_DELAY_1S)
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__App_485_Globe_Str App_485_Globe_Str;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void App_485_Power_ON(void);
void App_485_Power_OFF(void);
void App_ReadMeterMain(void);













#endif
/***********************************************************************************/

