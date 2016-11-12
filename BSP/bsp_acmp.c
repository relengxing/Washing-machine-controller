/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_acmp.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.15  
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_acmp.h"
#include "em_acmp.h"

/* Private typedef -----------------------------------------------------------*/
//typedef struct __Tydef_Bsp_Adc_PowerAll
//{
//		__Bsp_Enum_Adc_Cmp 	sBsp_Enum_Adc_Cmp;
//		GPIO_Port_TypeDef  	sGPIO ;
//	    INT8U		   		PinNum ;

//}__sBsp_Adc_PowerAll ;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

//#define BSP_NULL_PORT	((GPIO_Port_TypeDef)0)


//const __sBsp_Adc_PowerAll Bsp_Power_Array[ ]=
//{
//	{BSP_ADC_EN_ADC_1,	BSP_NULL_PORT,		0}
//	,{BSP_ADC_EN_ADC_2,	BSP_NULL_PORT,		0}
//	,{BSP_BATT_ENUM,	BSP_POWER_BATT_PORT,	BSP_POWER_BATT_PIN}	
//	,{BSP_485_ENUM,		BSP_POWER_UART_PORT,	BSP_POWER_UART_PIN}
//	,{BSP_VADC_ENUM,	BSP_POWER_ADC_PORT,		BSP_POWER_ADC_PIN}
//	,{BSP_EEPROM_ENUM,	BSP_POWER_EEPROM_PORT,	BSP_POWER_EEPROM_PIN}
//		
//};	


//void Bsp_Power_Adc_On(__Bsp_Adc_PowerStr *pBsp_Adc_PowerStr)
//{
//	if(BSP_NULL_PORT==Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].sGPIO)	
//	{
//		return ;
//	}
//	GPIO_PinModeSet(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].PinNum,gpioModePushPull, 0);  //     BSP_POWER_UART_PIN, gpioModePushPull, 0);
//	if(BSP_ADC_POWER_ON ==pBsp_Adc_PowerStr->sBsp_Enum_Adc_Switch)
//	{
//		GPIO_PinOutSet(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].PinNum);	
//	}
//	else
//	{
//		GPIO_PortOutClear(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Adc_Cmp].PinNum);	
//		
//	}
//}


/**********************************************************************************************
* Description       : 电压比较的情况
* Input             : Bsp_Enum_Adc_Cmp 参见 __Bsp_Enum_Adc_Cmp
* Output            : 比较值  
* Contributor       : pentral
* Date First Issued : 2015/07/15
***********************************************************************************************/
INT8U Bsp_Acmp_GetResult( __Bsp_Enum_Power_Name Bsp_Enum_Adc_Cmp) 
{
	ACMP_Init_TypeDef init = ACMP_INIT_DEFAULT;
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr;
	INT8U retCmpData = 0 ;
	//first power on
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name 	= Bsp_Enum_Adc_Cmp ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch	= BSP_ADC_POWER_ON ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);
	//clock on
	CMU_ClockEnable(cmuClock_ACMP0, true);
	//acmp init
	init.fullBias                  = 0;
	init.halfBias                  = 0;
	init.biasProg                  = 0;
	init.warmTime                  = acmpWarmTime32;
	init.hysteresisLevel           = acmpHysteresisLevel2;
	init.lowPowerReferenceEnabled  = 0;
	init.vddLevel                  = 61;
	init.interruptOnFallingEdge    = 0;
	init.interruptOnRisingEdge     = 0;
	init.inactiveValue             = 0;

	/* Plain ACMP init. For capacitive sense applications, use ACMP_CapsenseInit() and
	 * ACMP_CapsenseChannelSet() instead. */
	ACMP_Init(ACMP0, &init);
	// [ACMP Initialization]$

	// $[ACMP Channel config]
	ACMP_ChannelSet(ACMP0, acmpChannel2V5, acmpChannel5);	
	 /* Wait for warmup */
    while (!(ACMP0->STATUS & ACMP_STATUS_ACMPACT)) ;
	retCmpData = ACMP0->STATUS&ACMP_STATUS_ACMPOUT ;
	CMU_ClockEnable(cmuClock_ACMP0, false);	
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch	= BSP_ADC_POWER_OFF ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);
	return (retCmpData);

}




/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

