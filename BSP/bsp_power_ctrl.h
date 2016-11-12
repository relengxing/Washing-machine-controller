/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : bsp_power_ctrl.h
* Author            :  
* Date First Issued : 2015.07.15
* Version           : V1.0
* Description       : 
*----------------------------------------历史版本信息-------------------------------------------
* History           :
* //2008        : V
* Description       : 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
#ifndef __bsp_power_ctrl_H 
#define __bsp_power_ctrl_H
/* Includes-----------------------------------------------------------------------------------*/
#include "bsp_config.h"

/* Exported types ------------------------------------------------------------*/
//
/* Exported constants --------------------------------------------------------*/

typedef enum{
	BSP_ENUM_POWE_ADC1=0,
	BSP_ENUM_POWE_ADC2,
	BSP_ENUM_POWE_BATT,
	BSP_ENUM_POWE_485,
	BSP_ENUM_POWE_VADC,
	BSP_ENUM_POWE_EEPROM,
	BSP_ENUM_POWE_GPRS,
	BSP_ENUM_POWE_NULL
}__Bsp_Enum_Power_Name ;//Adc_Cmp ;

typedef enum{BSP_ADC_POWER_INIT,BSP_ADC_POWER_ON,BSP_ADC_POWER_OFF}__Bsp_Enum_Adc_Switch ;


/* Private variables ---------------------------------------------------------*/
typedef struct __Tydef_Bsp_Adc_Power
{
	__Bsp_Enum_Power_Name sBsp_Enum_Power_Name;
	__Bsp_Enum_Adc_Switch sBsp_Enum_Adc_Switch;
}__Bsp_Adc_PowerStr ;

/* Private define-------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void Bsp_Power_Operate(__Bsp_Adc_PowerStr *pBsp_Adc_PowerStr);




#endif   //
