/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : Bsp_Dac.h
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
#ifndef __bsp_dac_H 
#define __bsp_dac_H
/* Includes-----------------------------------------------------------------------------------*/
#include "em_cmu.h"
#include "em_emu.h"
#include "em_dac.h"

/* Private define-----------------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

int test_dac(void);

void Bsp_DAC_setup(void);

void Bsp_DAC_WriteData(DAC_TypeDef *dac, unsigned int value, unsigned int ch);

void Bsp_Mbus_DAC_WriteData(DAC_TypeDef *dac, unsigned int value);




#endif   //
