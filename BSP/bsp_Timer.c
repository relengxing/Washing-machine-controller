/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : bsp_Timer.c
* Author             : lichao
* Version            : V1.00
* Date               : 2015.08.21
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp_Timer.h"
#include "bsp_Led.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Description: Timer模块初始化初始化
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Timer_Config(INT32U ticks)
{
	 /* Enable clock for TIMER0 module */
	CMU_ClockEnable(cmuClock_TIMER0, true);
	/* Timer 初始化*/
	TIMER_Init_TypeDef timer_Init_Struct = 
	{
		.enable     = true, 
        .debugRun   = true, 
        .prescale   = timerPrescale1024, 
        .clkSel     = timerClkSelHFPerClk, 
        .fallAction = timerInputActionNone, 
        .riseAction = timerInputActionNone, 
        .mode       = timerModeUp, 
        .dmaClrAct  = false,
        .quadModeX4 = false, 
        .oneShot    = false, 
        .sync       = false, 
	};
	/* Enable overflow interrupt */
    TIMER_IntEnable(TIMER0, TIMER_IF_OF);
    
    /* Enable TIMER0 interrupt vector in NVIC */
    NVIC_EnableIRQ(TIMER0_IRQn);
    
    /* Set TIMER Top value */
    TIMER_TopSet(TIMER0,CMU_ClockFreqGet(cmuClock_TIMER0)/1024/1000 * ticks);
    
    /* Configure TIMER */
    TIMER_Init(TIMER0, &timer_Init_Struct);
}

void TIMER0_IRQHandler(void)
{ 
    /* Clear flag for TIMER0 overflow interrupt */
    Bsp_Timer_ISR();
}


/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
