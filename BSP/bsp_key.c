/***********************(C) COPY RIGHT 2012 nenglian**************************
* File Name: Bsp_SX127X.c
* Author:    pentraL
* Date First Issued: 2015-10-13
* Version: 1.0.0
* Description: 
*********************************************************************************/
#include "bsp_key.h"

/******************************************************************************
* Description:按键配置，配置为双边沿触发，按键按下松手后算一次按键，区分长按短按，详情在sysTick.c文件中
* Input:   无
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Key_PinConfig(void)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(BSP_KEY_1_PORT, BSP_KEY_1_PIN, gpioModeInput, 1);			//
	
	/* Set falling edge interrupt */
	GPIO_IntConfig(BSP_KEY_1_PORT, BSP_KEY_1_PIN,  true, true,true);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

}





