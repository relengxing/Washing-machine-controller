/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Wdt.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Bsp_Wdt.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void Bsp_Wdt_Init(void)
{
	WDOG_Init_TypeDef init =
	{
	  .enable     = true,               /* Start watchdog when init done */
	  .debugRun   = false,              /* WDOG not counting during debug halt */
	  .em2Run     = true,               /* WDOG counting when in EM2 */
	  .em3Run     = true,               /* WDOG counting when in EM3 */
	  .em4Block   = false,              /* EM4 can be entered */
	  .swoscBlock = false,              /* Do not block disabling LFRCO/LFXO in CMU */
	  .lock       = false,              /* Do not lock WDOG configuration (if locked, reset needed to unlock) */
	  .clkSel     = wdogClkSelULFRCO,   /* Select 1kHZ WDOG oscillator */
	  .perSel     = wdogPeriod_1k,      /* Set the watchdog period to 2049 clock periods (ie ~8 seconds)      */
	};
	WDOG_Init(&init);
}

void Bsp_Wdt_Feed(void)
{
	WDOG_Enable(true);
	while (WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
	WDOG_Feed();
}


/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
