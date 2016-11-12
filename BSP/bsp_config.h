/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_config.h
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

#ifndef __Bsp_config_H 
#define __Bsp_config_H


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stddef.h>
#include "stdint.h"
#include "stdbool.h"
#include "em_gpio.h"
#include "em_cmu.h"

/* Private typedef -----------------------------------------------------------*/
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity       */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity       */
typedef signed   short INT16S;                   /* Signed   16 bit quantity       */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity       */
typedef float          FP32;                     /* Single precision floating point*/
typedef double         FP64;                     /* Double precision floating point*/
typedef unsigned long long int			INT64U;		// 可变的无符号64位整型变量
typedef signed long long int			INT64S;		// 可变的有符号64位整型变量

typedef enum
{
    APP_TASK_NULL,
    APP_TASK_FREE,
	APP_TASK_BUSY,
}__App_Task_State;


typedef enum
{
	KEY_NONE = 0,	
	KEY_SHORT,
	KEY_LONG,
}Key_State;



/* Private define ------------------------------------------------------------*/

#define	OFFSETOF				 offsetof

#define BSP_LEN_RUN_PIN		   5
#define BSP_LEN_RUN_PORT	   gpioPortF

#define BSP_LEN_WARM_PIN	   4
#define BSP_LEN_WARM_PORT	   gpioPortF

#define BSP_LEN_LINK_UP_PIN	   3
#define BSP_LEN_LINK_UP_PORT   gpioPortF

#define BSP_LEN_LINK_DOWN_PIN  2
#define BSP_LEN_LINK_DOWN_PORT gpioPortF



//pin define 
#define BSP_UART_0_LOCATION    LEUART_ROUTE_LOCATION_LOC2
#define BSP_UART_0_TXPORT      gpioPortE
#define BSP_UART_0_TXPIN       14
#define BSP_UART_0_RXPORT      gpioPortE
#define BSP_UART_0_RXPIN       15

#define BSP_485_PORT			gpioPortA
#define BSP_485_PIN				15

/*
#define BSP_UART_1_LOCATION    USART_ROUTE_LOCATION_LOC1
#define BSP_UART_1_TXPORT      gpioPortE
#define BSP_UART_1_TXPIN       7
#define BSP_UART_1_RXPORT      gpioPortE
#define BSP_UART_1_RXPIN       6
*/
#define BSP_UART_1_LOCATION    USART_ROUTE_LOCATION_LOC0
#define BSP_UART_1_TXPORT      gpioPortE
#define BSP_UART_1_TXPIN       10
#define BSP_UART_1_RXPORT      gpioPortE
#define BSP_UART_1_RXPIN       11

#define BSP_UART_2_LOCATION    USART_ROUTE_LOCATION_LOC1
#define BSP_UART_2_TXPORT      gpioPortD
#define BSP_UART_2_TXPIN       0
#define BSP_UART_2_RXPORT      gpioPortD
#define BSP_UART_2_RXPIN       1

//
#define BSP_POWER_ADC_PORT	   gpioPortB		   
#define BSP_POWER_ADC_PIN	   4

#define BSP_POWER_UART_PORT	   gpioPortE		   
#define BSP_POWER_UART_PIN	   13

#define BSP_POWER_BATT_PORT	   gpioPortD		   
#define BSP_POWER_BATT_PIN	   4

#define BSP_POWER_EEPROM_PORT  gpioPortA		   
#define BSP_POWER_EEPROM_PIN   2

#define BSP_POWER_GPRS_PORT    gpioPortB		   
#define BSP_POWER_GPRS_PIN     5


#define BSP_POWER_ONOFF_PORT   gpioPortB		   
#define BSP_POWER_ONOFF_PIN    3
//spi
#define BSP_SPI_MOSI_PORT	   gpioPortD		   
#define BSP_SPI_MOSI_PIN	   0

#define BSP_SPI_MISO_PORT	   gpioPortD		   
#define BSP_SPI_MISO_PIN	   1

#define BSP_SPI_CLK_PORT	   gpioPortD		   
#define BSP_SPI_CLK_PIN	   	   2

#define BSP_SPI_CS_PORT	       gpioPortD		   
#define BSP_SPI_CS_PIN	       3

#define SPI_1				   USART1
#define	SPI_1_SPEED			   4000000ul  //debug 400k 实际会用到2M
#define SPI_ROUTE  			  (USART_ROUTE_LOCATION_LOC1   | USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN )//USART_ROUTE_CSPEN

//RF
#define BSP_RF_RST_PIN		  	7
#define BSP_RF_RST_PORT		  	gpioPortC

#define BSP_RF_TRX_CTRL_PIN	  	7
#define BSP_RF_TRX_CTRL_PORT  	gpioPortD

#define BSP_RF_ENABLE_PIN	  	6			//EN_PE4259
#define BSP_RF_EBABLE_PORT  	gpioPortD

#define BSP_RF_D0_PIN	  		6			//
#define BSP_RF_D0_PORT  		gpioPortC

#define BSP_RF_D1_PIN	  		8			//
#define BSP_RF_D1_PORT  		gpioPortD

//MBUS
#define BSP_MBUS_POWER_PIN		5
#define BSP_MBUS_POWER_PORT		gpioPortE

#define BSP_MBUS_RST_PIN	  	12
#define BSP_MBUS_RST_PORT  		gpioPortC

#define BSP_MBUS_WARM_PIN	  	4			
#define BSP_MBUS_WARM_PORT  	gpioPortE

//ADC
#define BSP_MBUS_AD_PIN			5
#define BSP_MBUS_AD_PORT		gpioPortD

//DAC
#define BSP_MBUS_DA_PIN			13
#define BSP_MBUS_DA_PORT		gpioPortC

//eeprom
#define BSP_EEPROM_SDA_PIN	  	0
#define BSP_EEPROM_SDA_PORT  	gpioPortA

#define BSP_EEPROM_SCL_PIN	  	1			
#define BSP_EEPROM_SCL_PORT  	gpioPortA

#define BSP_EEPROM_LOCTION 		0

//key
#define BSP_KEY_1_PIN	  		13			
#define BSP_KEY_1_PORT  		gpioPortA

//BATT 
#define BSP_BATT_AD_PIN			4
#define BSP_BATT_AD_PORT		gpioPortD


#define TRUE	true
#define FALSE	false
#define NOP()	__nop();



#ifndef BIT
#define BIT(x)					(1uL<<(x))
#endif

//#define NULL	(void*)0

#define SYS_TICK_COUNT	  1000ul      
#define OS_TICKS_PER_SEC  SYS_TICK_COUNT 		//与systick 有关

#define SYS_DELAY_10ms			 (OS_TICKS_PER_SEC/100)
#define SYS_DELAY_20ms			 (OS_TICKS_PER_SEC/50)
#define SYS_DELAY_25ms			 (OS_TICKS_PER_SEC/40)
#define SYS_DELAY_40ms			 (OS_TICKS_PER_SEC/25)
#define SYS_DELAY_50ms			 (OS_TICKS_PER_SEC/20)
#define SYS_DELAY_100ms			 (OS_TICKS_PER_SEC/10)
#define SYS_DELAY_250ms			 (OS_TICKS_PER_SEC/4)
#define SYS_DELAY_500ms			 (OS_TICKS_PER_SEC/2)
#define SYS_DELAY_1000ms		 (OS_TICKS_PER_SEC/1)
#define SYS_DELAY_1s			 (OS_TICKS_PER_SEC/1)
#define SYS_DELAY_2s			 (OS_TICKS_PER_SEC*2)
#define SYS_DELAY_3s			 (OS_TICKS_PER_SEC*3)
#define SYS_DELAY_4s			 (OS_TICKS_PER_SEC*4)
#define SYS_DELAY_5s			 (OS_TICKS_PER_SEC*5)
#define SYS_DELAY_6s			 (OS_TICKS_PER_SEC*6)
#define SYS_DELAY_10s			 (OS_TICKS_PER_SEC*10)
#define SYS_DELAY_15s			 (OS_TICKS_PER_SEC*15)
#define SYS_DELAY_20s			 (OS_TICKS_PER_SEC*20)
#define SYS_DELAY_30s			 (OS_TICKS_PER_SEC*30)
#define SYS_DELAY_90s			 (OS_TICKS_PER_SEC*90)
#define SYS_DELAY_1M			 (OS_TICKS_PER_SEC*60)
#define SYS_DELAY_1_5M			 (OS_TICKS_PER_SEC*90)
#define SYS_DELAY_2M			 (OS_TICKS_PER_SEC*120)
#define SYS_DELAY_4M			 (OS_TICKS_PER_SEC*240)
#define SYS_DELAY_5M			 (OS_TICKS_PER_SEC*300)
#define SYS_DELAY_10M			 (OS_TICKS_PER_SEC*600)





/* Private macro -------------------------------------------------------------*/
//typedef enum{BSP_ADC_EN_ADC_1,BSP_ADC_EN_ADC_2,BSP_BATT_ENUM,BSP_485_ENUM,BSP_VADC_ENUM,BSP_EEPROM_ENUM}__Bsp_Enum_Adc_Cmp ;

//typedef enum{BSP_ENUM_LED_RUN = 0,BSP_ENUM_LED_WARM,BSP_ENUM_LED_UP_LINK,BSP_ENUM_LED_LINK_DOWN,BSP_ENUM_LED_END}__Bsp_Enum_Led_Name ;


///* Private variables ---------------------------------------------------------*/
//typedef struct __Tydef_Bsp_Adc_Power
//{
//	__Bsp_Enum_Adc_Cmp sBsp_Enum_Adc_Cmp;
//	__Bsp_Enum_Adc_Switch sBsp_Enum_Adc_Switch;
//}__Bsp_Adc_PowerStr ;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


#endif//

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
