/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_adc.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "em_adc.h"
#include "em_cmu.h"

#include "bsp_adc.h"

//#include <string.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

//调试
//void XXXX_Bsp_Power_Adc_On(void)
//{
//	GPIO_PinModeSet(BSP_POWER_ADC_PORT, BSP_POWER_ADC_PIN, gpioModePushPull, 0);
//	GPIO_PinOutSet(BSP_POWER_ADC_PORT, BSP_POWER_ADC_PIN);
//	
//}

/**********************************************************************************************
* Description       : 获取采样值 
* Input             : 通道号
* Output            : none 
* Contributor       : pentral
* Date First Issued : 2015/07/15
***********************************************************************************************/
INT32U Bsp_Get_AdcData(__Bsp_Enum_Adc Adc_Num) 
{
	INT32U adcData ;
	//clock init 
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_ADC0, true);
	//Pin config
	if(BSP_ENUM_ADC_MBUS ==Adc_Num)
	{
		GPIO_PinModeSet(BSP_MBUS_AD_PORT, BSP_MBUS_AD_PIN, gpioModeInput, 0);
	}
	else if(BSP_ENUM_ADC_BATT ==Adc_Num)
	{
		GPIO_PinModeSet(BSP_BATT_AD_PORT, BSP_BATT_AD_PIN, gpioModeInput, 0);
	
	}
	else
	{
		return 0 ;
	}

//	XXXX_Bsp_Power_Adc_On();
	ADC_Reset(ADC0);	
	//DC_Init
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;

	init.ovsRateSel                = adcOvsRateSel2;
	init.lpfMode                   = adcLPFilterBypass;
	init.warmUpMode                = adcWarmupNormal;
	init.timebase                  = ADC_TimebaseCalc(0);
	init.prescale                  = ADC_PrescaleCalc(ADC_CONVERT_SPEED, 0);//clock
	init.tailgate                  = 0;

	ADC_Init(ADC0, &init);

	// $[ADC_InitSingle]
	ADC_InitSingle_TypeDef initsingle = ADC_INITSINGLE_DEFAULT;

	initsingle.prsSel              = adcPRSSELCh0;
	initsingle.acqTime             = adcAcqTime1;
	initsingle.reference           = ADC_REFVOLT ;
	initsingle.resolution          = ADC_SAMPLEBIT;
	if(BSP_ENUM_ADC_MBUS ==Adc_Num)
	{
		initsingle.input               = ADC_CHANNEL_1;		
	}
	else if(BSP_ENUM_ADC_BATT ==Adc_Num)
	{
		initsingle.input               = ADC_CHANNEL_2;		
	}
	else
	{
		initsingle.input               = ADC_CHANNEL_2;		
		
	}
	initsingle.diff                = 0;
	initsingle.prsEnable           = 0;
	initsingle.leftAdjust          = 0;
	initsingle.rep                 = 0;

	/* Initialize a single sample conversion.
	 * To start a conversion, use ADC_Start().
	 * Conversion result can be read with ADC_DataSingleGet(). */
	ADC_InitSingle(ADC0, &initsingle);
	

	 /* Start ADC convert by Software trigger */
    ADC_Start(ADC0, adcStartSingle);

    /* Wait while conversion is active */
    while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;
	adcData = ADC_DataSingleGet(ADC0);
	CMU_ClockEnable(cmuClock_ADC0, false);
	return adcData ;
}




/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

