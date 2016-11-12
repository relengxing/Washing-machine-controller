/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/
#ifndef _APP_NL_H
#define _APP_NL_H
/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stdio.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "sysTick.h"
#include "bsp_rtc.h"
#include "Bsp_Uart.h"
#include "App_Head.h"
#include "bsp_wdt.h"
#include "App_Para_Interface.h"
#include "App_Data_interface.h "
#include "bsp_iic.h"
#include "App_M6310.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct __App_NL_Struct
{
	INT8U FrameStart;		//起始帧															
	INT8U InfoDomain[2];	//信息域
	INT8U DataLenth[2];		//数据域长度
	INT8U *Data;			//数据域
	INT8U CS;				//校验位				
	INT8U FrameStop;		//结束位
}App_NL_Struct;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
INT8U App_NL_Receive(INT8U ComPort,INT8U *pBuff,INT8U Lenth);
void App_NL_Report(void);














#endif

