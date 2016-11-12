/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : bsp_vcmp.h
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
#ifndef __bsp_vcmp_H 
#define __bsp_vcmp_H
/* Includes-----------------------------------------------------------------------------------*/
#include "bsp_config.h"
#include "bsp_power_ctrl.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Private define-------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/**********************************************************************************************
* Description       : 电压比较的情况
* Input             : Bsp_Enum_Adc_Cmp 参见 __Bsp_Enum_Adc_Cmp
* Output            : 比较值  
* Contributor       : pentral
* Date First Issued : 2015/07/15
***********************************************************************************************/
INT8U Bsp_Acmp_GetResult( __Bsp_Enum_Power_Name Bsp_Enum_Adc_Cmp) ;



#endif   //
