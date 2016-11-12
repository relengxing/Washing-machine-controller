/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_led.h
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Bsp_led_H
#define __Bsp_led_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"

/* Exported types ------------------------------------------------------------*/
typedef enum{BSP_ENUM_LED_RUN = 0,
			BSP_ENUM_LED_WARM = 1,
			BSP_ENUM_LED_UP_LINK = 2,
			BSP_ENUM_LED_LINK_DOWN = 3,
			BSP_ENUM_LED_END
}__Bsp_Enum_Led_Name ;


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

//#define BSP_GPIO_LEDS	
#define BSP_LEDS_AMOUNT  4
//#define BSP_GPIO_LEDARRAY_INIT {{gpioPortA,15}}

/* Exported functions ------------------------------------------------------- */


/******************************************************************************
* Function Name: 
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_Init(void);

void Bsp_Led_Toggle(__Bsp_Enum_Led_Name  Bsp_Led_Num);

void Bsp_Led_SetL(__Bsp_Enum_Led_Name  Bsp_Led_Num);

void Bsp_Led_SetH(__Bsp_Enum_Led_Name  Bsp_Led_Num);


//处理灯亮的时间 返回1 表示有灯需要处理，0 空闲
INT8U  Bsp_Led_Is_OnState(void);

/******************************************************************************
* Description: LED灯点亮多长时间
* Input:   LED灯的名字，在 __Bsp_Led_Name 中定义  time：时间 单位ms
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_On(__Bsp_Enum_Led_Name  Bsp_Led_Num,INT16U time);

//时间计数
void Bsp_Led_TimeCount(void);


#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




