/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_led.h
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _APP_485_H
#define _APP_485_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
extern INT8U ReadMeter_Interval;		//抄表间隔
extern INT8U ReadMeter_Time;			//抄表时间
/* Exported functions ------------------------------------------------------- */


/******************************************************************************
* Function Name: 
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_485_Init(void);

void App_485_ProgramFunc(void);

/******************************************************************************
* Description: 进入LP时，把相应的管脚处理一下
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_Mbus_Power_Off_PinInit(void);

#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




