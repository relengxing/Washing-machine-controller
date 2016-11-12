/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_iic.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_iic.h"
//#include "i2cspm.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/** Number of bytes in EEPROM */
#define EEPROM_DVK_LEN      32768

/** Pagesize in EEPROM */
#define EEPROM_DVK_PAGESIZE    64

#define EEPROM_I2C_NUM		I2C0
#define EEPROM_ADDRESS		0xa0

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************
* Description       : I2c初始化
* Input             : init I2C0/I2C1
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/15
***********************************************************************************************/
void Bsp_I2CSPM_Init(void)
{
	INT32U i;
	CMU_Clock_TypeDef i2cClock;
//	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr; 
	I2C_Init_TypeDef i2cInit ;//= (I2C_Init_TypeDef)I2CSPM_INIT_DEFAULT ;//I2CSPM_INIT_DEFAULT;
	I2CSPM_Init_TypeDef tInit = I2CSPM_INIT_DEFAULT ;
	I2CSPM_Init_TypeDef *init = &tInit ;
	
	EFM_ASSERT(init != NULL);
	CMU_ClockEnable(cmuClock_HFPER, true);
   
	/* Select I2C peripheral clock */
	if (false)
	{
		#if defined( I2C0 )
		}
		else if (init->port == I2C0)
		{
		i2cClock = cmuClock_I2C0;
		#endif
		#if defined( I2C1 )
		}
		else if (init->port == I2C1)
		{
		i2cClock = cmuClock_I2C1;
		#endif
	}
	else
	{
		/* I2C clock is not defined */
		EFM_ASSERT(false);
	return;
	}
	CMU_ClockEnable(i2cClock, true);

	/* Output value must be set to 1 to not drive lines low. Set
	 SCL first, to ensure it is high before changing SDA. */
	GPIO_PinModeSet(init->sclPort, init->sclPin, gpioModeWiredAndPullUp, 1);
	GPIO_PinModeSet(init->sdaPort, init->sdaPin, gpioModeWiredAndPullUp, 1);
  

  
	//power		
//	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);
//	GPIO_PinOutSet(gpioPortA,2);
//	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_ON ;
//	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_EEPROM ;
//    Bsp_Power_Operate(&tBsp_Adc_PowerStr);
  /* In some situations, after a reset during an I2C transfer, the slave
     device may be left in an unknown state. Send 9 clock pulses to
     set slave in a defined state. */
	for (i = 0; i < 9; i++)
	{
		GPIO_PinOutSet(init->sclPort, init->sclPin);
		GPIO_PinOutClear(init->sclPort, init->sclPin);
	}

	/* Enable pins and set location */
	init->port->ROUTE = I2C_ROUTE_SDAPEN |
					  I2C_ROUTE_SCLPEN |
					  (init->portLocation << _I2C_ROUTE_LOCATION_SHIFT);

	/* Set emlib init parameters */
	i2cInit.enable = true;
	i2cInit.master = true; /* master mode only */
	i2cInit.freq = init->i2cMaxFreq;
	i2cInit.refFreq = init->i2cRefFreq;
	i2cInit.clhr = init->i2cClhr;

	I2C_Init(init->port, &i2cInit);
}


void Bsp_I2CSPM_Disable(void)
{
	//power	
//	GPIO_PinModeSet(BSP_POWER_EEPROM_PORT, BSP_POWER_EEPROM_PIN, gpioModePushPull, 1);
//	GPIO_PinOutClear(BSP_POWER_EEPROM_PORT,BSP_POWER_EEPROM_PIN);
//	//SCLSDA
//	GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 1);
//	GPIO_PinOutClear(gpioPortC, 6);
//	GPIO_PinModeSet(gpioPortC, 7, gpioModePushPull, 1);
//	GPIO_PinOutClear(gpioPortC, 7);
//	//I2C
//	I2C_Enable(EEPROM_I2C_NUM,0);
}

/**********************************************************************************************
* Description       : 根据电源状态初始化I2c 
* Input             : ,BSP_ADC_POWER_ON or BSP_ADC_POWER_OFF
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/15
***********************************************************************************************/
void Bsp_I2CSPM_BasePower_Init(__Bsp_Enum_Adc_Switch Switch)
{
	static INT8U powerStateEEprom = BSP_ADC_POWER_OFF ;
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr; 
	if(BSP_ADC_POWER_ON == Switch)
	{

		Bsp_I2CSPM_Init();
	}
	else
	{
		if(BSP_ADC_POWER_OFF ==powerStateEEprom)
		{
			return ;
		}

		GPIO_PinModeSet(BSP_EEPROM_SDA_PORT, BSP_EEPROM_SDA_PIN, gpioModePushPull, 0);
		GPIO_PinModeSet(BSP_EEPROM_SCL_PORT, BSP_EEPROM_SCL_PIN, gpioModePushPull, 0);

	}
	powerStateEEprom = Switch ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = Switch ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_EEPROM ;
    Bsp_Power_Operate(&tBsp_Adc_PowerStr);

}



/**********************************************************************************************
* Description       : I2c 发送数据
* Input             : i2c I2C0/I2C1  seq参见 I2C_TransferSeq_TypeDef
* Output            : 数据 
* Contributor       : lichao
* Date First Issued : 2015/07/15
***********************************************************************************************/
I2C_TransferReturn_TypeDef Bsp_I2CSPM_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq)
{
	I2C_TransferReturn_TypeDef ret;
	INT32U timeout = I2CSPM_TRANSFER_TIMEOUT;
	/* Do a polled transfer */
	ret = I2C_TransferInit(i2c, seq);
	while (ret == i2cTransferInProgress && timeout--)
	{
	ret = I2C_Transfer(i2c);
	}
	return ret;
}


/**********************************************************************************************
* Description       :  
* Input             :  
* Output            :   
* Contributor       :  
* Date First Issued : 2015/07/15
***********************************************************************************************/
static INT32U Bsp_EEPROM_AckPoll(I2C_TypeDef *i2c, INT16U addr)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	/* Do acknowledge polling waiting for write process to finish in EEPROM */
	seq.addr  = addr;
	seq.flags = I2C_FLAG_WRITE;
	/* Just access device with write operation */
	seq.buf[0].data = NULL;
	seq.buf[0].len  = 0;

	/* Wait for ACK from device */
	while (1)
	{
		ret = Bsp_I2CSPM_Transfer(i2c, &seq);
		if (ret == i2cTransferDone)
		{
			break;
		}
		else if (ret == i2cTransferNack)
		{
			continue;
		}
		else
		{
			return((INT32U) ret);
		}
	}

	return(0);
}



INT32U Bsp_EEPROM_Read(INT32U offset,INT8U *data,INT32U len)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	INT8U                    offsetLoc[2];

	if (offset >= EEPROM_DVK_LEN)
	{
		return(0);
	}
	Bsp_I2CSPM_BasePower_Init(BSP_ADC_POWER_ON);
	if ((offset + len) > EEPROM_DVK_LEN)
	{
		len = EEPROM_DVK_LEN - offset;
	}

	seq.addr  = EEPROM_ADDRESS;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select offset to start reading from */
	offsetLoc[0]    = (INT8U) (offset>>8);
	offsetLoc[1]    = (INT8U) (offset&0xff);
	seq.buf[0].data = offsetLoc;
	seq.buf[0].len  = 2;
	/* Select location/length of data to be read */
	seq.buf[1].data = data;
	seq.buf[1].len  = len;

	ret = Bsp_I2CSPM_Transfer(EEPROM_I2C_NUM, &seq);
	if (ret != i2cTransferDone)
	{
		return((INT32U) ret);
	}

	return((INT32U) len);
}

INT32U Bsp_EEPROM_Write(INT32U offset,INT8U *data,INT32U len)
{
//	I2C_TypeDef *i2c = I2C0;
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	INT32U                     tmp;
	INT32U               		chunk;
	INT32U               		max;
	INT8U                    offsetLoc[2];

	if (offset >= EEPROM_DVK_LEN)
	{
		return(0);
	}
	Bsp_I2CSPM_BasePower_Init(BSP_ADC_POWER_ON);

	if ((offset + len) > EEPROM_DVK_LEN)
	{
		len = EEPROM_DVK_LEN - offset;
	}

	/* Write max one page at a time */
	while (len)
	{
		max = EEPROM_DVK_PAGESIZE - (offset % EEPROM_DVK_PAGESIZE);

		if (len > max)
		{
			chunk = max;
		}
		else
		{
			chunk = len;
		}

		seq.addr  = EEPROM_ADDRESS;
		seq.flags = I2C_FLAG_WRITE_WRITE;
		/* Select offset to start writing to */
		offsetLoc[0]    = (INT8U) (offset>>8);
		offsetLoc[1]    = (INT8U) (offset&0xff);
		seq.buf[0].data = offsetLoc;
		seq.buf[0].len  = 2;
		/* Select location/length of data to be written */
		seq.buf[1].data = data;
		seq.buf[1].len  = chunk;

//		ret = Bsp_I2CSPM_Transfer(i2c, &seq);
		ret = Bsp_I2CSPM_Transfer(EEPROM_I2C_NUM, &seq);
		if (ret != i2cTransferDone)
		{
			return((INT32U) ret);
		}

		/* Update counters etc */
		data   += chunk;
		offset += chunk;
		len    -= chunk;

		/* Do acknowledge polling waiting for write process to finish in EEPROM */
		tmp = Bsp_EEPROM_AckPoll(EEPROM_I2C_NUM, EEPROM_ADDRESS);
		if (tmp)
		{
			return(tmp);
		}
	}

	return((INT32U) len);
}


/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

