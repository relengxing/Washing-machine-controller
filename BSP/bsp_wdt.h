/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_wdt.h
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Bsp_wdt_H
#define __Bsp_wdt_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_wdog.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

void Bsp_Wdt_Init(void);

//如果8s内不喂狗，则重启
void Bsp_Wdt_Feed(void) ;
#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




