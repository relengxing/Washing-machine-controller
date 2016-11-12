/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : sysTick.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "sysTick.h"
#include "bsp_Uart.h"
#include "Bsp_Led.h"
#include "bsp_Timer.h"
//#include "Lora.h"
#include "App_Gprs_Interface.h"
#include "App_Para_Interface.h"
#include "App_485_Interface.h"
#include "bsp_rtc.h"
#include "App_M6310.h"
//#include "sx1276-LoRa.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
INT32U SW_Version;		//软件版本
INT32U HW_Version;		//硬件版本				
INT8U SW_ReleaseData[3];
INT8U HW_ReleaseData[3];

/* Private variables ---------------------------------------------------------*/

volatile INT32U msTicks; /* counts 1ms timeTicks */

volatile INT32U SecondTicks; /* counts 1s timeTicks */

volatile Key_State  SysKeyFlag ; /* key int flag */		//0:未按下    1:短按    2:长按   在bsp_config.h中有定义

volatile INT32U KeyTicks;
/* Private function prototypes -----------------------------------------------*/
INT32U SysTick_Get_Second_Counts(void);

/* Private functions ---------------------------------------------------------*/

/****************************************************************************
 * @brief SysTick_Handler   高速时钟有效时才可以用
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
	msTicks++;       /* increment counter necessary in Delay()*/

	SYS_UART_FSR(1);
	SYS_UART_FSR(2);
	SYS_UART_FSR(3);
//	App_Mbus_Time_msCall();	
	App_Agr_Time_msCall();
	Bsp_Led_TimeCount( );
	App_Gprs_Task_TimeCall();
	GPIO_Key_TimeCount();
//	App_Gprs_SendDelay();
	App_485_Task_TimeCall();
	if(!(msTicks%OS_TICKS_PER_SEC))
	{
		SecondTicks++ ;
//		if(TRUE==App_Get_PowerOn_Rf_State())
//		{
//			Bsp_Led_On(BSP_ENUM_LED_RUN,5);
//		}
		
	}	
		
}

INT32U Sys_Get_RTC_Count(void)
{
	return BSP_Rtc_TimeCount ;
}

/**************************************************************************//**
 * @brief RTC Interrupt Handler.
 *        Updates minutes and hours.
 *****************************************************************************/
void RTC_IRQHandler(void)
{
	_BSPRTC_TIME RtcTime;
	/* Clear interrupt source */
	RTC_IntClear(RTC_IFC_COMP0);
	BSP_Rtc_TimeCount++ ;
//	BSP_RTCGetTime(&gRtcTimeDefault);
//	App_Agr_Cad_On_Cycle();    
//	SX1276_Second_Call();
	BSP_RTCGetTime(&RtcTime);
//	GPIO_Key_TimeCount();
//	Bsp_Led_On(BSP_ENUM_LED_RUN,5);
	if((RtcTime.Hour == 0x00)&&(RtcTime.Minute == 0x58))		//如果时间经过00：50分时允许抄表一次
	{
		if(App_485_Get_WorkState() == 0)			
		{
			App_485_Set_WorkState(1);
		}
	}
	if((RtcTime.Hour == 0x00)&&(RtcTime.Minute == 0x59))		//时间经过0：59，抄表
	{
		if(App_485_Get_WorkState() == 1)			
		{
			App_485_Set_WorkState(2);
		}
	}
	if((RtcTime.Hour == 0x00)&&(RtcTime.Minute == 0x59))		//如果时间经过00：50分时允许上报一次
	{
		if(App_Gprs_Get_WorkState() == 0)			
		{
			App_Gprs_Set_WorkState(1);
		}
	}
	if((RtcTime.Hour == 0x01)&&(RtcTime.Minute == 0x00))		//时间经过1：00，且允许上报时上报
	{
		if(App_Gprs_Get_WorkState() == 1)			
		{
			App_Gprs_Set_WorkState(2);
		}
	}
}

/**************************************************************************//**
 * @brief 按键延时  
 *        Sets the minutes
 *****************************************************************************/
void GPIO_Key_TimeCount(void)			//按键计时
{
	if(KeyTicks > 0)
	{
		KeyTicks++;
	}
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler  
 *        Sets the minutes
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
	/* Acknowledge interrupt */
	if((GPIO_IntGet()& (1<<BSP_KEY_1_PIN)) == (1<<BSP_KEY_1_PIN))		//如果是来自按键1的中断
	{
		GPIO_IntClear(1 << BSP_KEY_1_PIN);						//清除中断
		if((GPIO_PortInGet(BSP_KEY_1_PORT)&(1<<BSP_KEY_1_PIN)) == (1<<BSP_KEY_1_PIN))					//如果是上升沿
		{							//开始一个计时
			KeyTicks = 1;				//开始++
		}
		if((GPIO_PortInGet(BSP_KEY_1_PORT)&(1<<BSP_KEY_1_PIN)) == 0)					//如果是下降沿
		{
			if(KeyTicks>500)			//长按大于0.5s
			{
				SysKeyFlag = KEY_LONG;				//返回长按标志
			}
			else if(KeyTicks>0)		//短按大于20ms						//低功耗情况下按键，无论短按长按都算短按
			{
				SysKeyFlag = KEY_SHORT ;			//返回短按标志
			}
			KeyTicks = 0;
		}	
		//处理按键程序
//		if(SysKeyFlag == KEY_SHORT)
//		{
//			Bsp_Led_Toggle((__Bsp_Enum_Led_Name)2);
//			SysKeyFlag = KEY_NONE;
//		}
//		if(SysKeyFlag == KEY_LONG)
//		{
//			Bsp_Led_Toggle((__Bsp_Enum_Led_Name)3);
//			SysKeyFlag = KEY_NONE;
//		}
		
	}
}




/**************************************************************************//**
 * @brief GPIO Interrupt handler  
 *        Sets the minutes
 *****************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
//	GPIO_IntGet();

	/* Acknowledge interrupt */
	GPIO_IntClear(1 << BSP_RF_D0_PIN);
	GPIO_IntClear(1 << BSP_RF_D1_PIN);

}


/**************************************************************************//**
 * @brief 
 *****************************************************************************/
INT8U SysGetKeyIntFlag(void)
{
	volatile INT8U ret = SysKeyFlag ;
	SysKeyFlag = KEY_NONE ;
	return ret ;
}

INT32U SysTick_Get_Second_Counts(void)
{
    return SecondTicks;
}

INT32U SysTick_Get_Counts(void)
{
    return msTicks;
}

//uint32_t Get_Tick_Count(void)
//{
//    return msTicks;
//}

void Bsp_Timer_ISR(void)
{
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
//#ifdef APP_GPRS_ON
    
//	App_Gprs_DelayTime();
//	
//	App_Gprs_SendDelay();
//#endif	
//    Bsp_Led_Toggle(BSP_ENUM_LED_RUN);
}

/*****************************************************************************
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(INT32U dlyTicks)
{
  INT32U curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

void DelayMs(INT32U dlyMs)
{
	Delay(dlyMs);
}


/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

