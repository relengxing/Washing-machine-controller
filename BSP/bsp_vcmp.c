/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_vcmp.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.15  
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_vcmp.h"
#include "em_vcmp.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************
* Description       : 没调试过。
* Input             : none
* Output            : none 
* Contributor       : 
* Date First Issued : 2015/07/15
***********************************************************************************************/
INT8U Bsp_Vcmp_Init(void) 
{
	// VCMP_Init
	VCMP_Init_TypeDef init = VCMP_INIT_DEFAULT;

	init.halfBias                  = 1;
	init.biasProg                  = 7;		//电流大小
	init.irqFalling                = 0;
	init.irqRising                 = 0;
	init.inactive                  = 0;
	init.warmup                    = vcmpWarmTime4Cycles;
	init.hyst                      = vcmpHystNone;
	init.lowPowerRef               = 0;
	init.triggerLevel              = 25;
	
    /* Initialize VCMP */
	VCMP_Init(&init);				

    /* Wait for VCMP warm-up */
    while (!VCMP_Ready()) ;
    
    unsigned char ucFlag = 0;
    /* if VDD > trigger level ==> return ture, else return false*/
    ucFlag = VCMP_VDDHigher();
    
    /*Close VCMP*/
    VCMP_Disable();
    
    return (ucFlag)	;
}




/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

