
#include "Bsp_Test.h"
#include "Bsp_Uart.h"
#include "Bsp_adc.h"
#include "Bsp_acmp.h"
#include "Bsp_spi.h"
#include "Bsp_iic.h"
#include "Bsp_lcd.h"
#include "Bsp_dac.h"
#include "sysTick.h"
#include "bsp_spi.h"
#include "App_M6310.h"
#include "App_Gprs_Interface.h"
#include "App_Para_Interface.h"
#include "App_Data_Interface.h"
#include "App_485_Interface.h"
#define COM_BUFF_AMOUNT 120
INT8U gComRxBuff[COM_BUFF_AMOUNT];
INT8U gComTxBuff[COM_BUFF_AMOUNT] = {0x55,0x99,0XAA,0x55,0x99,0XAA,0x55,0x99,0XAA,0x55,0x99,0XAA,};
INT8U gCount= 0 ;
volatile INT32U Adc_Data = 0 ;

void App_CommInit(void)
{
	S_UARTx_Set UARTInit;						// 参数结构
	UARTInit.BaudRate = 9600;
	UARTInit.Parity   = BSPUART_PARITY_NO;
	UARTInit.StopBits = BSPUART_STOPBITS_1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8;
	UARTInit.RxBuf    = gComRxBuff;
	UARTInit.RxBufLen = COM_BUFF_AMOUNT ;
	UARTInit.TxBuf    = gComTxBuff;
	UARTInit.TxBufLen = COM_BUFF_AMOUNT ;
	UARTInit.Mode     = UART_DEFAULT_MODE;	// UART_DEFAULT_MODE
	BSP_UART_Init(2,&UARTInit);	
}


void Bsp_Test(void)
{
//	App_CommInit();
//	INT8U data[130];
//		__App_Para_InterfaceStr para_data;	
//	S_UARTx_Set uartSet ;
//	INT8U comPort = 2; 
//	INT8U i = 0 ;
////	float ftmp = -99 ;
//	INT8U array[4]={0x34,0x56,0x48,0xc1} ;
//	INT32U tTmp = 0xc1480000 ;
////	memcpy((INT8U*)&ftmp,array,4);
//	SPI_Initial();
//	Bsp_I2CSPM_Init();
//	Bsp_LCD_Init();
//	if(ftmp==-12.5)
//	{
//		array[0] = 0x33;
//	}
//	uartSet.BaudRate = 9600 ;
//	uartSet.DataBits = usartDatabits8 ;
//	uartSet.StopBits = usartStopbits1 ;
//	uartSet.Parity 	 = usartEvenParity ;
//	uartSet.RxBuf    = gComRxBuff ;
//	uartSet.RxBufLen = sizeof(gComRxBuff);
//	uartSet.TxBuf    = gComTxBuff ;
//	uartSet.TxBufLen = sizeof(gComTxBuff);
//	BSP_UART_Init(comPort,&uartSet);
//	BSP_UART_Write(comPort,gComTxBuff,5);
//	test_dac();
//	App_GprsCommInit();   //先串口初始化再打开电源
//	App_Gprs_Power_On();
//	App_485_Init();

	while(1)
	{
		App_Gprs_Set();
//		INT8U st ;
//		SPI_SendRcvByte(SPI_1,array[i],&st);
//		i++;
		Delay(1000);
//		App_485_ProgramFunc();
//		Delay(5000);
//		App_Gprs_Connet_ProgramFunc();
//		Adc_Data = Bsp_Get_AdcData(BSP_ADC_EN_ADC_1);
//		Delay(1000);
//		Adc_Data = Bsp_Get_AdcData(BSP_ADC_EN_ADC_2);
//		Delay(1000);
//		Adc_Data = Bsp_Acmp_GetResult(BSP_485_ENUM);
//		Delay(1000);

//		USART_Tx(SPI_1,0x55);
//		Delay(10);
//		USART_Tx(SPI_1,0x96);
//		Delay(10);
//		memset(gComTxBuff,0x56,43);
//		para_data.afn = METER_WRITE;	
//		para_data.IDIndex = 0x0c01;
//		para_data.point = 1;
//		para_data.dbuff = gComTxBuff;		//指向实际数据
//		para_data.dlen  =  43;
//		GWFirstData_function(&para_data);
//		//Bsp_EEPROM_Read
//		memset(gComTxBuff,0x12,43);
////Bsp_EEPROM_Write
//		para_data.afn = METER_READ;	
//		para_data.dbuff = gComTxBuff;
//		para_data.dlen  = 43;
//		GWFirstData_function(&para_data);	
//		NOP();
//		
//		Delay(1000);
//		memset(data,0x22,120);
//		
//		Bsp_EEPROM_Write(0xa,data,120);
//		Delay(1000);
//		memset(data,0x99,120);
//		Bsp_EEPROM_Read(0xa,data,120);	
//		Delay(1000);
//		INT16U i = 0 ;
//		for(i = 0; i<100 ; i++)
//		{
//			LCD_Display_NUM(i);
//			Delay(200);
//		}
	}
	


}
