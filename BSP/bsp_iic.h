/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : bsp_iic.h
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
#ifndef __bsp_iic_H 
#define __bsp_iic_H
/* Includes-----------------------------------------------------------------------------------*/
#include "bsp_config.h"
#include "em_i2c.h"
#include "bsp_power_ctrl.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  I2C_TypeDef           *port;          /**< Peripheral port */
  GPIO_Port_TypeDef     sclPort;        /**< SCL pin port number */
  uint8_t               sclPin;         /**< SCL pin number */
  GPIO_Port_TypeDef     sdaPort;        /**< SDA pin port number */
  uint8_t               sdaPin;         /**< SDA pin number */
  uint8_t               portLocation;   /**< Port location */
  uint32_t              i2cRefFreq;     /**< I2C reference clock */
  uint32_t              i2cMaxFreq;     /**< I2C max bus frequency to use */
  I2C_ClockHLR_TypeDef  i2cClhr;        /**< Clock low/high ratio control */
} I2CSPM_Init_TypeDef;

#define I2CSPM_INIT_DEFAULT     \
  { I2C0,                       /* Use I2C instance 0 */                        \
    BSP_EEPROM_SCL_PORT,                  /* SCL port */                                  \
    BSP_EEPROM_SCL_PIN,                          /* SCL pin */                                   \
    BSP_EEPROM_SDA_PORT,  	                /* SDA port */                                  \
    BSP_EEPROM_SDA_PIN,                          /* SDA pin */                                   \
    BSP_EEPROM_LOCTION,                          /* Location */                                  \
    0,                          /* Use currently configured reference clock */  \
    I2C_FREQ_STANDARD_MAX,      /* Set to standard rate  */                     \
    i2cClockHLRStandard,        /* Set to use 4:4 low/high duty cycle */        \
  }

  
/* Exported constants --------------------------------------------------------*/
  
  
/* Private define-------------------------------------------------------------*/
 
#define I2CSPM_TRANSFER_TIMEOUT 300000
  
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/*****************************************************************************
* Description       : I2c初始化
* Input             : init I2C0/I2C1
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/15
******************************************************************************/
void Bsp_I2CSPM_Init(void);
void Bsp_I2CSPM_Disable(void);
/**********************************************************************************************
* Description       : 根据电源状态初始化I2c 
* Input             : ,BSP_ADC_POWER_ON or BSP_ADC_POWER_OFF
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/15
***********************************************************************************************/
void Bsp_I2CSPM_BasePower_Init(__Bsp_Enum_Adc_Switch Switch);  

  
INT32U Bsp_EEPROM_Write(INT32U offset,INT8U *data, INT32U len);

INT32U Bsp_EEPROM_Read(INT32U offset,INT8U *data, INT32U len);


#endif   //
