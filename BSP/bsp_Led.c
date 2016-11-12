/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp_Led.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    __Bsp_Enum_Led_Name LedName ;
	GPIO_Port_TypeDef   port;
	INT32U              pin;
} LedArray;
/* Private define ------------------------------------------------------------*/
static const LedArray ledArray[ BSP_LEDS_AMOUNT ] =
{
	{ BSP_ENUM_LED_RUN			,BSP_LEN_RUN_PORT		,BSP_LEN_RUN_PIN}
	,{BSP_ENUM_LED_WARM			,BSP_LEN_WARM_PORT     	,BSP_LEN_WARM_PIN}
	,{BSP_ENUM_LED_UP_LINK		,BSP_LEN_LINK_UP_PORT  	,BSP_LEN_LINK_UP_PIN}
	,{BSP_ENUM_LED_LINK_DOWN	,BSP_LEN_LINK_DOWN_PORT	,BSP_LEN_LINK_DOWN_PIN}	
};

INT8U Bsp_LedTime[BSP_LEDS_AMOUNT];
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void Bsp_All_Pin(void)  //10:28--11.48 稳定
{
	int i;
	/* 时钟设置----------------------------------------*/
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	for ( i=0; i<16; i++ )
	{
		GPIO_PinModeSet(gpioPortA, i, gpioModePushPull, 0);  //only PA 1mA--4.9uA
		if((7!=i)&&(8!=i))
		{
			GPIO_PinModeSet(gpioPortB, i, gpioModePushPull, 0);
			
		}
		if(15!=i)
			GPIO_PinModeSet(gpioPortC, i, gpioModePushPull, 0);
		GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
		GPIO_PinModeSet(gpioPortE, i, gpioModePushPull, 0);
		if((0!=i)&&(1!=i))
			GPIO_PinModeSet(gpioPortF, i, gpioModePushPull, 0);
	}
	
}


/******************************************************************************
* Description: 板载LED灯初始化
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_Init(void)
{
	int i;
	/* 时钟设置----------------------------------------*/
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
//	Bsp_All_Pin();
	/* GPIO设置----------------------------------------*/
	for ( i=0; i<BSP_LEDS_AMOUNT; i++ )
	{
		GPIO_PinModeSet(ledArray[i].port, ledArray[i].pin, gpioModePushPull, 1);
	}
//	GPIO_PinModeSet(BSP_MBUS_AD_PORT, BSP_MBUS_AD_PIN, gpioModeInput, 0);

}


/******************************************************************************
* Description: LED灯翻转
* Input: LED灯的名字，在 __Bsp_Led_Name 中定义
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_Toggle(__Bsp_Enum_Led_Name  Bsp_Led_Num)
{
	if (Bsp_Led_Num < BSP_ENUM_LED_END)
	{
		GPIO_PinOutToggle(ledArray[Bsp_Led_Num].port, ledArray[Bsp_Led_Num].pin);
	}
}
/******************************************************************************
* Description: LED灯熄灭
* Input:   LED灯的名字，在 __Bsp_Led_Name 中定义
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_SetL(__Bsp_Enum_Led_Name  Bsp_Led_Num)
{
	if (Bsp_Led_Num < BSP_ENUM_LED_END)
	{
		GPIO_PinOutSet(ledArray[Bsp_Led_Num].port, ledArray[Bsp_Led_Num].pin);
	}
}
/******************************************************************************
* Description: LED灯点亮
* Input:   LED灯的名字，在 __Bsp_Led_Name 中定义
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_SetH(__Bsp_Enum_Led_Name  Bsp_Led_Num)
{
	if (Bsp_Led_Num < BSP_ENUM_LED_END)
	{
		GPIO_PinOutClear(ledArray[Bsp_Led_Num].port, ledArray[Bsp_Led_Num].pin);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
* Description: LED灯点亮多长时间
* Input:   LED灯的名字，在 __Bsp_Led_Name 中定义  time：时间 单位ms
* Output: Nothing
* Return: Nothing
******************************************************************************/
void Bsp_Led_On(__Bsp_Enum_Led_Name  Bsp_Led_Num,INT16U time)
{
//	time = 50;
	if (Bsp_Led_Num < BSP_ENUM_LED_END)
	{
		Bsp_LedTime[Bsp_Led_Num] = time ;
		Bsp_Led_SetH(Bsp_Led_Num);
		
	}
}


void Bsp_Led_TimeCount(void)
{
	INT8U i ;
	for(i = 0 ; i < BSP_LEDS_AMOUNT ;i++)
	{
		if(Bsp_LedTime[i])
		{
			Bsp_LedTime[i]--;
		}
	}
//	Bsp_Led_Is_OnState();
}

//处理灯亮的时间 返回1 表示有灯需要处理，0 空闲
INT8U  Bsp_Led_Is_OnState(void)
{
	INT8U i ;
	INT8U ret = 0 ;
	for(i = 0 ; i < BSP_LEDS_AMOUNT ;i++)
	{
		if(Bsp_LedTime[i])
		{
			Bsp_Led_SetH((__Bsp_Enum_Led_Name)i);
			ret = 1 ;
			break ;
		}
		else
		{
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)i);
			
		}
	}
	return ret ;
}

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
