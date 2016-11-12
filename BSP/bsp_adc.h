/*****************************************Copyright(C)******************************************
*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName          : bsp_adc.h
* Author            :  
* Date First Issued : 2015.07.15
* Version           : V1.0
* Description       : 
*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
* History           :
* //2008        : V
* Description       : 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
#ifndef __bsp_adc_H 
#define __bsp_adc_H
/* Includes-----------------------------------------------------------------------------------*/
#include "bsp_config.h"
#include "em_adc.h"
#include "bsp_power_ctrl.h"


/* Exported types ------------------------------------------------------------*/
typedef enum{BSP_ENUM_ADC_MBUS,BSP_ENUM_ADC_BATT }__Bsp_Enum_Adc ;
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


/* Private define-----------------------------------------------------------------------------*/
#define ADC_CONVERT_SPEED   400000            	//ADCת������
#define ADC_REFVOLT         adcRefVDD           //ADC�ο���ѹѡ��  
#define ADC_SAMPLEBIT       adcRes12Bit         //ADCת���ֱ���ѡ��

#define ADC_CHANNEL_1       adcSingleInpCh5   	//ADC����ͨ��ѡ��
#define ADC_CHANNEL_2       adcSingleInpCh4   	//ADC����ͨ��ѡ��

#define ADC_SCAN_INPUTMASK_1	ADC_SCANCTRL_INPUTMASK_CH5
#define ADC_SCAN_INPUTMASK_2	ADC_SCANCTRL_INPUTMASK_CH4

/* Exported function prototypes -----------------------------------------------*/

INT32U Bsp_Get_AdcData(__Bsp_Enum_Adc Adc_Num) ;


#endif   //
