/***********************(C) COPY RIGHT 2012 nenglian**************************
* File Name: Bsp_SX127X.c
* Author:    pentraL
* Date First Issued: 2015-10-13
* Version: 1.0.0
* Description: 
*********************************************************************************/
#include "bsp_key.h"

/******************************************************************************
* Description:�������ã�����Ϊ˫���ش����������������ֺ���һ�ΰ��������ֳ����̰���������sysTick.c�ļ���
* Input:   ��
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





