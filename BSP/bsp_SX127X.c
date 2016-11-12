/***********************(C) COPY RIGHT 2015 nenglian**************************
* File Name: Bsp_SX127X.c
* Author: zhangdan
* Date First Issued: 2014-11-13
* Version: 1.0.0
* Description: This file define all the function used to control SX127X
*********************************************************************************/
#include "bsp_config.h"
#include "bsp_spi.h"
#include "bsp_SX127X.h"
#include "bsp_Led.h"



/*******************************************************************************
* Function Name  : BSP_SX127XCS
* Description    : SX127X 片选使能
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SX127XCS(void)
{
	GPIO_PinModeSet(BSP_SPI_CS_PORT  ,BSP_SPI_CS_PIN  ,gpioModePushPull,0); //cs

}

/*******************************************************************************
* Function Name  : BSP_SX127XDisCS
* Description    : SX127X 片选禁止
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SX127XDisCS(void)
{
	GPIO_PinModeSet(BSP_SPI_CS_PORT  ,BSP_SPI_CS_PIN  ,gpioModePushPull,1); //cs
}

void RST_RF_Pin_Control(uint8_t NewState)  //RST_RF_PIN控制
{
	 if(NewState)
	 {
//         GPIO_SetBits(BSP_RF_RST_PORT, BSP_RF_RST_PIN);
		 GPIO_PinModeSet(BSP_RF_RST_PORT, BSP_RF_RST_PIN,gpioModeWiredAnd,1);
     }
	 else
	 {
		 GPIO_PinModeSet(BSP_RF_RST_PORT, BSP_RF_RST_PIN,gpioModeWiredAnd,0);//gpioModePushPull
//	     GPIO_ResetBits(BSP_RF_RST_PORT, BSP_RF_RST_PIN);
     }
}

void RX_TX_CTRLPin_Control(uint8_t NewState)  //RX/TX选择
{
	 if(NewState)
	 {
		 GPIO_PinModeSet(BSP_RF_TRX_CTRL_PORT, BSP_RF_TRX_CTRL_PIN,gpioModePushPull,1);
     }
	 else
	 {
		 GPIO_PinModeSet(BSP_RF_TRX_CTRL_PORT, BSP_RF_TRX_CTRL_PIN,gpioModePushPull,0);
     }
}

uint8_t D0Pin_Control(void)  //D0脚检测
{
	return 	GPIO_PinInGet(BSP_RF_D0_PORT, BSP_RF_D0_PIN);
}

uint8_t D1Pin_Control(void)  //D1脚检测
{
    return GPIO_PinInGet(BSP_RF_D1_PORT, BSP_RF_D1_PIN);
}

void LED_RFRXD_GPin_Control(uint8_t NewState)  //LEDG控制
{
	 if(NewState)
	 {
		 Bsp_Led_SetL(BSP_ENUM_LED_LINK_DOWN);
     }
	 else
	 {
		 Bsp_Led_SetH(BSP_ENUM_LED_LINK_DOWN);
     }
}

void LED_RFTXD_RPin_Control(uint8_t NewState)  //LEDR控制
{
	 if(NewState)
	 {
		 Bsp_Led_SetL(BSP_ENUM_LED_LINK_DOWN);
     }
	 else
	 {
		 Bsp_Led_SetH(BSP_ENUM_LED_LINK_DOWN);
     }
}

void EN_PE4259Pin_Control(uint8_t NewState)  //EN_PE4259 使能
{
	 if(NewState)
	 {
		 GPIO_PinModeSet(BSP_RF_EBABLE_PORT, BSP_RF_ENABLE_PIN,gpioModePushPull,1);
     }
	 else
	 {
		 GPIO_PinModeSet(BSP_RF_EBABLE_PORT, BSP_RF_ENABLE_PIN,gpioModePushPull,0);
     }
}

INT8U SPI2_SendRcvByte(INT8U dt)
{
	INT8U statu;
	
	return SPI_SendRcvByte(SPI_1, dt, &statu);
}

void SX1276_RelatePIN_Open(void)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(BSP_RF_RST_PORT, BSP_RF_RST_PIN, gpioModePushPull, 1);
	
	GPIO_PinModeSet(BSP_RF_TRX_CTRL_PORT, BSP_RF_TRX_CTRL_PIN, gpioModePushPull, 1);
	
	GPIO_PinModeSet(BSP_RF_EBABLE_PORT, BSP_RF_ENABLE_PIN, gpioModePushPull, 1);
	
    /* Configure DIO0 DIO1as  Floating input            */	
	GPIO_PinModeSet(BSP_RF_D0_PORT, BSP_RF_D0_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(BSP_RF_D1_PORT, BSP_RF_D1_PIN, gpioModeInput, 0);	
	
	/* Set falling edge interrupt */
	GPIO_IntConfig(BSP_RF_D0_PORT, BSP_RF_D0_PIN,  true, false,true); 
	GPIO_IntConfig(BSP_RF_D1_PORT, BSP_RF_D1_PIN,  true, false,true); 
	
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

void Bsp_Rf_PowerOffPinConfig(void)
{
	//	GPIO_PinModeSet(BSP_RF_RST_PORT, BSP_RF_RST_PIN, gpioModePushPull, 0);		//cad模式不正常
	GPIO_PinModeSet(BSP_RF_EBABLE_PORT, BSP_RF_ENABLE_PIN, gpioModePushPull, 0);	 //先不关断
	GPIO_PinModeSet(BSP_RF_TRX_CTRL_PORT, BSP_RF_TRX_CTRL_PIN, gpioModePushPull, 0); 
	
//	/* Set falling edge interrupt */
//	GPIO_IntConfig(BSP_RF_D0_PORT, BSP_RF_D0_PIN,  true, false,false); 
//	GPIO_IntConfig(BSP_RF_D1_PORT, BSP_RF_D1_PIN,  true, false,false); 
//	GPIO_PinModeSet(BSP_RF_D0_PORT, BSP_RF_D0_PIN, gpioModePushPull, 0);  //rf 接收异常
//	GPIO_PinModeSet(BSP_RF_D1_PORT, BSP_RF_D1_PIN, gpioModePushPull, 0);	
//	GPIO_PinModeSet(BSP_SPI_MOSI_PORT, BSP_SPI_MOSI_PIN, gpioModePushPull, 0);  //rf 接收异常
//	GPIO_PinModeSet(BSP_SPI_MISO_PORT, BSP_SPI_MISO_PIN, gpioModePushPull, 0);	
//	GPIO_PinModeSet(BSP_SPI_CLK_PORT , BSP_SPI_CLK_PIN , gpioModePushPull, 0);  //rf 接收异常
//	GPIO_PinModeSet(BSP_SPI_CS_PORT  , BSP_SPI_CS_PIN  , gpioModePushPull, 0);	


}





