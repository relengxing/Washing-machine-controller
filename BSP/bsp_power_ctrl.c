/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_vcmp.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.15  
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_power_ctrl.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __Tydef_Bsp_Adc_PowerAll
{
	__Bsp_Enum_Power_Name sBsp_Enum_Power_Name;
	GPIO_Port_TypeDef  	sGPIO ;
	INT8U		   		PinNum ;

}__sBsp_Adc_PowerAll ;
/* Private define ------------------------------------------------------------*/
#define BSP_NULL_PORT	((GPIO_Port_TypeDef)0)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


const __sBsp_Adc_PowerAll Bsp_Power_Array[ ]=
{
	{BSP_ENUM_POWE_ADC1,	BSP_NULL_PORT			,		0}
	,{BSP_ENUM_POWE_ADC2,	BSP_NULL_PORT			,		0}
	,{BSP_ENUM_POWE_BATT,	BSP_POWER_BATT_PORT		,	BSP_POWER_BATT_PIN}	
	,{BSP_ENUM_POWE_485,	BSP_POWER_UART_PORT		,	BSP_POWER_UART_PIN}
	,{BSP_ENUM_POWE_VADC,	BSP_POWER_ADC_PORT		,	BSP_POWER_ADC_PIN}
	,{BSP_ENUM_POWE_EEPROM,	BSP_POWER_EEPROM_PORT	,	BSP_POWER_EEPROM_PIN}
	,{BSP_ENUM_POWE_GPRS,	BSP_POWER_GPRS_PORT	 	,	BSP_POWER_GPRS_PIN}
	,{BSP_ENUM_POWE_NULL,   BSP_NULL_PORT           ,	0   }
		
};	




void Bsp_Power_Operate(__Bsp_Adc_PowerStr *pBsp_Adc_PowerStr)
{
	if(pBsp_Adc_PowerStr->sBsp_Enum_Power_Name >= BSP_ENUM_POWE_NULL)	
	{
		return ;
	}
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
	if(BSP_ADC_POWER_ON ==pBsp_Adc_PowerStr->sBsp_Enum_Adc_Switch)
	{
		GPIO_PinModeSet(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].PinNum,gpioModePushPull, 1);  
//		GPIO_PinOutSet(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].PinNum);	
	}
	else
	{
		GPIO_PinModeSet(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].PinNum,gpioModePushPull, 0); 
//		GPIO_PortOutClear(Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].sGPIO, Bsp_Power_Array[pBsp_Adc_PowerStr->sBsp_Enum_Power_Name].PinNum);	
		
	}
}



/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

