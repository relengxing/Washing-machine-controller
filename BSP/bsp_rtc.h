/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_RTC.h
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
* Description        : This file contains all the functions prototypes for the
*                      ADC firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Bsp_RTC_H
#define __Bsp_RTC_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "em_rtc.h"

/* Exported types ------------------------------------------------------------*/

__packed typedef struct
{
	INT8U Second;								// 秒
	INT8U Minute;								// 分
	INT8U Hour;									// 时
	INT8U Day;									// 日
	INT8U Month;								// 月
	INT8U Year;									// 年
	INT8U Week;									// 周
}_BSPRTC_TIME;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
extern volatile INT32U BSP_Rtc_TimeCount ;

/* Exported functions ------------------------------------------------------- */
/*******************************************************************************
* Description    : 内部RTC 相关时钟和设备初始化  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_RTC_Init(void);

/***********************************************************************************************
* Function		: BSP_RTCSetTime
* Description	: 重新设置RTC时间
* Input			: *pTime:要设的时间数据指针 BCD码输入
* Output		: 1/0
* Contributor	: 150617	
***********************************************************************************************/
INT8U BSP_RTCSetTime(volatile _BSPRTC_TIME *pTime);

/***********************************************************************************************
* Function		: BSP_RTCGetTime
* Description	: 获取RTC时间
* Input			: *pTime:要设的时间数据指针 BCD码输出
* Output		: 1/0
* Contributor	: 150617	
***********************************************************************************************/
INT8U BSP_RTCGetTime(volatile _BSPRTC_TIME *pTime);



void BSP_RTC_Count_Second_Call(void);

/***********************************************************************************************
* Description	: 是否有设置时间过 
* Input			: none 
* Output		: 1/0
* Contributor	: 150617	
***********************************************************************************************/
INT8U Bsp_Rtc_IsSetTime(void);



#endif /*__Bsp_RTC_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/



