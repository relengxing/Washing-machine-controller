/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Dac.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.09.13
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

//#include "em_device.h"
#include "bsp_config.h"
#include "bsp_dac.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
INT8U Ch_Dac =0;

/******************************************************************************
* @brief  Setup DAC
* Configures and starts the DAC
*******************************************************************************/
void Bsp_DAC_setup(void)
{
//    /* Use default settings */
//    DAC_Init_TypeDef        init        = DAC_INIT_DEFAULT;
//    DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;
//    
//    /* Enable the DAC clock */
//    CMU_ClockEnable(cmuClock_DAC0, true);
//	   
//    /* Calculate the DAC clock prescaler value that will result in a DAC clock
//    * close to 500kHz. Second parameter is zero, if the HFPERCLK value is 0, the
//    * function will check what the current value actually is. */
//    init.prescale = DAC_PrescaleCalc(500000, 0);
//    
//    /* Set DAC's outmode to output to pin */
//    init.outMode = dacOutputADC;//acOutputPinADC ;//dacOutputADC ;//dacOutputPin;//
//    
//    /* Set reference voltage to 1.25V */
//    init.reference = dacRefVDD ;//dacRef1V25;
//    
//    /* Initialize the DAC and DAC channel. */
//    DAC_Init(DAC0, &init);

//////PD0 & PB11
////    DAC0->OPA0MUX &= ~ (_DAC_OPA0MUX_OUTMODE_MASK | _DAC_OPA0MUX_OUTPEN_MASK);
////    DAC0->OPA0MUX |= DAC_OPA0MUX_OUTMODE_ALT | DAC_OPA0MUX_OUTPEN_OUT4;

////PC13
//    DAC0->OPA1MUX &= ~ (_DAC_OPA1MUX_OUTPEN_MASK | _DAC_OPA1MUX_OUTMODE_MASK);
//    DAC0->OPA1MUX |= DAC_OPA1MUX_OUTMODE_ALT | DAC_OPA1MUX_OUTPEN_OUT1;


//	DAC_InitChannel(DAC0, &initChannel, Ch_Dac);//
//	/* Enable DAC channel 0,  */
//    DAC_Enable(DAC0, Ch_Dac, true);
	
	
    /* Use default settings */
    DAC_Init_TypeDef        init        = DAC_INIT_DEFAULT;
    DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;
    
    /* Enable the DAC clock */
    CMU_ClockEnable(cmuClock_DAC0, true);
	   
    /* Calculate the DAC clock prescaler value that will result in a DAC clock
    * close to 500kHz. Second parameter is zero, if the HFPERCLK value is 0, the
    * function will check what the current value actually is. */
    init.prescale = DAC_PrescaleCalc(500000, 0);
    
    /* Set DAC's outmode to output to pin */
    init.outMode = dacOutputPin ;//dacOutputADC;//dacOutputPinADC ;//dacOutputADC ;//dacOutputPin;//
    
    /* Set reference voltage to 1.25V */
    init.reference = dacRefVDD ;//dacRef1V25;
    
    /* Initialize the DAC and DAC channel. */
    DAC_Init(DAC0, &init);

////PD0 & PB11
//    DAC0->OPA0MUX &= ~ (_DAC_OPA0MUX_OUTMODE_MASK | _DAC_OPA0MUX_OUTPEN_MASK);
//    DAC0->OPA0MUX |= DAC_OPA0MUX_OUTMODE_ALT | DAC_OPA0MUX_OUTPEN_OUT4;

////PC13
//    DAC0->OPA1MUX &= ~ (_DAC_OPA1MUX_OUTPEN_MASK | _DAC_OPA1MUX_OUTMODE_MASK);
//    DAC0->OPA1MUX |= DAC_OPA1MUX_OUTMODE_ALT | DAC_OPA1MUX_OUTPEN_OUT1;


	DAC_InitChannel(DAC0, &initChannel, Ch_Dac);//
	/* Enable DAC channel 0,  */
    DAC_Enable(DAC0, Ch_Dac, true);
	
	
}


/******************************************************************************
* @brief  Write DAC conversion value
*******************************************************************************/
void Bsp_DAC_WriteData(DAC_TypeDef *dac, unsigned int value, unsigned int ch)
{
    /* Write data output value to the correct register. */
    if (!ch)
    {
        dac->CH0DATA = value;
    }
    else
    {
        dac->CH1DATA = value;
    }
}

void Bsp_Mbus_DAC_WriteData(DAC_TypeDef *dac, unsigned int value)
{
    /* Write data output value to the correct register. */
   Bsp_DAC_WriteData(dac,value,Ch_Dac);
}

INT32U DAC_Value = 0 ;
int test_dac(void)
{
//    /* Initialize chip */
//    CHIP_Init();
//    
//    uint32_t DAC_Value;
//    
    /* Initialise the DAC */
    Bsp_DAC_setup();
	DAC_Value = 2000;//(uint32_t)((1.25 * 4095) / 1.25);    
	if(DAC_Value>4095)
	{
		DAC_Value = 0 ;
	}
    /* Write the new value to DAC register */
    Bsp_DAC_WriteData(DAC0, DAC_Value, Ch_Dac);
    
    while(1);
}

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/

