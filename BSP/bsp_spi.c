/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_spi.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_spi.h"
#include "em_usart.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************
* Description       : SPI初始化
* Input             : none
* Output            : none 
* Contributor       : 
* Date First Issued : 2015/07/15
***********************************************************************************************/
void SPI_Initial(void)
{
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART1, true);
    
    USART_InitSync_TypeDef SPI_init = USART_INITSYNC_DEFAULT;
    SPI_init.baudrate = SPI_1_SPEED;			// 速度
    
    USART_InitSync(SPI_1, &SPI_init);//
   
    SPI_1->CMD   = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
    SPI_1->CTRL &= ~USART_CTRL_AUTOCS;


	
    GPIO_PinModeSet(BSP_SPI_MOSI_PORT,BSP_SPI_MOSI_PIN,gpioModePushPull,1); //tx
    GPIO_PinModeSet(BSP_SPI_MISO_PORT,BSP_SPI_MISO_PIN,gpioModeInput,	1); //rx
    GPIO_PinModeSet(BSP_SPI_CLK_PORT ,BSP_SPI_CLK_PIN ,gpioModePushPull,1); //clk
    GPIO_PinModeSet(BSP_SPI_CS_PORT  ,BSP_SPI_CS_PIN  ,gpioModePushPull,1); //cs
    
    SPI_1->ROUTE = SPI_ROUTE;

////	SystemCoreClockUpdate();
//	CMU_ClockEnable(cmuClock_HFPER, true);
//    CMU_ClockEnable(cmuClock_GPIO, true);
//    CMU_ClockEnable(cmuClock_USART1, true);
//    
//    USART_InitSync_TypeDef SPI_init = USART_INITSYNC_DEFAULT;
//    SPI_init.baudrate = 2000000;
//    
//    USART_InitSync(USART1, &SPI_init);
//   /* Enabling Master, TX and RX */
//    SPI_1->CMD   = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
//    SPI_1->CTRL |= USART_CTRL_AUTOCS;
//    GPIO_PinModeSet(gpioPortD,0,gpioModePushPull,1); //tx
//    GPIO_PinModeSet(gpioPortD,1,gpioModeInput,1);    //rx
//    GPIO_PinModeSet(gpioPortD,2,gpioModePushPull,1); //clk
//    GPIO_PinModeSet(gpioPortD,3,gpioModePushPull,1); //cs
//    
//    USART1->ROUTE |= USART_ROUTE_CSPEN | USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_LOCATION_LOC1;
}

/******************************************************************
* Function Name: SPI_SendRcvByte
* Description:   通过SPI发送、接受字节数据 
* Input:  dt - 通过SPI发送的字节数据
* Output: 
* Return: 通过SPI读取的字节数据
* Contributor:                                
* Date First Issued: 2015-12-25
******************************************************************/
INT8U SPI_SendRcvByte(SPI_TypeDef* SPIx, INT8U dt, INT8U *statu)
{
//	INT32U count1 = 0;

//	Delay200NS();
//	*statu = TRUE;

//	/* 等待数据寄存器空 */
//	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
//	{
//	    //如果非空，等待发送结束
//		if( (count1++) >= 0x3000 )
//		{
//			//延时总线出错，复位总线
//			SPI_Cmd(SPIx, DISABLE);
//			DelayUS(5);	
//			SPI_Cmd(SPIx, ENABLE);
//			DelayUS(5);

//			count1 = 0;
//			*statu = FALSE;
//			break;
//		}	
//	}

//	/* 通过SPI1接口发送数据 */
//	SPI_I2S_SendData(SPIx, dt);

//	/* 等待接收到一个字节的数据 */
//	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
//	{
//	    //如果空，等待接收结束
//		if( (count1++) >= 0x3000)	  //0x3000
//		{
//			//延时总线出错，复位总线
//			SPI_Cmd(SPIx, DISABLE);
//			DelayUS(5);	
//			SPI_Cmd(SPIx, ENABLE);
//			DelayUS(5);

//			*statu = FALSE;
//			break;
//		} 
//	}

//	/* 返回接收的数据 */
//	return SPI_I2S_ReceiveData(SPIx);
	*statu = USART_SpiTransfer(SPIx,dt);
	return *statu ;
}



/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

