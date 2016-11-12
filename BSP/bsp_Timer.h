/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : bsp_Timer.h
* Author             : lichao
* Version            : V1.00
* Date               : 2015.08.21
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


/******************************************************************************
* Function Name: 
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Timer_Config(INT32U ticks);
void TIMER0_IRQHandler(void);
void Bsp_Timer_ISR(void);

#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




