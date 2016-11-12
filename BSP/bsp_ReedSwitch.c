/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_ReedSwitch.c
* Author             : lichao
* Version            : V1.00
* Date               : 2015.08.31
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Bsp_ReedSwitch.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Description: ∞Â‘ÿLEDµ∆≥ı ºªØ
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Reed_Init(void)
{
	// ±÷”≈‰÷√
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	//GPIO…Ë÷√
	GPIO_PinModeSet(BSP_REED_PORT,BSP_REED_PIN,gpioModeInputPullFilter,1);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	GPIO_IntConfig(BSP_REED_PORT,BSP_REED_PIN,true,false,true);
	
}
/******************************************************************************
* Description: 
* Input: 
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Reed_ISR(void)
{
	
}


/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
