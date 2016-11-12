/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_ReedSwitch.h
* Author             : lichao
* Version            : V1.00
* Date               : 2015.8.31
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BSP_REEDSWITCH_H
#define _BSP_REEDSWITCH_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define BSP_REED_PORT		gpioPortC
#define BSP_REED_PIN		12
/* Exported functions ------------------------------------------------------- */


/******************************************************************************
* Function Name: 
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Reed_Init(void);





#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




