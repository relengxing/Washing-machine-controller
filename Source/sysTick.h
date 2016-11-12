/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : sysTick.h
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.13
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sysTick_H
#define __sysTick_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
extern INT32U SW_Version;		//软件版本
extern INT32U HW_Version;		//硬件版本				
extern INT8U SW_ReleaseData[3];
extern INT8U HW_ReleaseData[3];


/* Exported functions ------------------------------------------------------- */

void Delay(INT32U dlyTicks);

INT32U SysTick_Get_Second_Counts(void);

INT32U SysTick_Get_Counts(void);

INT32U Sys_Get_RTC_Count(void);

void DelayMs(INT32U dlyMs);

void GPIO_Key_TimeCount(void);			//按键计时

INT8U SysGetKeyIntFlag(void);

void DelayMs(INT32U dlyMs);



#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




