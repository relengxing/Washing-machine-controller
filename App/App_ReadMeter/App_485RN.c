/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "App_485RN.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DEFAULT_BAUDRATE 2400
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
INT8U ReedDataFrame[] = {0xFE,0xFE,0xFE,0xFE,
	0x68,0x25,
	0x00,0x00,0x00,0x00,0x00,0x11,0x11,
	0x01,
	0x03,
	0x1F,0x90,0x12,
	0x74,0x16
};

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#define APP_FRAMEBUFF_LEN	100
#define APP_485_COMM		1
INT8U App_485_RXTXBuff[APP_FRAMEBUFF_LEN];
/**********************************************************************************************
* Description       : 
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
static void App_485_ClearBuff(void)
{
	BSP_UART_ClearRxBuff(APP_485_COMM);
	BSP_UART_ClearTxBuff(APP_485_COMM);
}

/**********************************************************************************************
* Description       : 
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
void App_485_Init(void)
{
	__App_Para_InterfaceStr para_data;	
	S_UARTx_Set UARTInit;						// 
	INT8U Buff[10];
	
	
	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0406;
	para_data.point = 1;
	para_data.dbuff = Buff;						//把集中器户表数据读出
	para_data.dlen  =  10 ;
	GWPara_function(&para_data);	
	
	switch(Buff[0]>>5)			//右移5位
	{
		case 0:UARTInit.BaudRate = DEFAULT_BAUDRATE;
			break;
		case 1:UARTInit.BaudRate = 600;
			break;
		case 2:UARTInit.BaudRate = 1200;
			break;
		case 3:UARTInit.BaudRate = 2400;
			break;
		case 4:UARTInit.BaudRate = 4800;
			break;
		case 5:UARTInit.BaudRate = 7200;
			break;
		case 6:UARTInit.BaudRate = 9600;
			break;
		case 7:UARTInit.BaudRate = 19200;
			break;
		default:
			break;
	}
//	UARTInit.BaudRate = 2400;
	UARTInit.Parity   = BSPUART_PARITY_EVEN ;//usartEvenParity;
	UARTInit.StopBits = BSPUART_STOPBITS_1  ;//usartStopbits1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8 ;//usartDatabits8;
	UARTInit.RxBuf    = App_485_RXTXBuff ;  //App_485_Comm_Data_Str.pRxBuff;
	UARTInit.RxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.TxBuf    = App_485_RXTXBuff;
	UARTInit.TxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.Mode     = UART_RS485_MODE;	// UART_DEFAULT_MODE  UART_RS485_MODE
	BSP_UART_Init(APP_485_COMM,&UARTInit);	
	
	App_485_Power_ON();
}

void App_485_ManipulatorInit(void)
{
	S_UARTx_Set UARTInit;
	UARTInit.BaudRate = 2400;
	UARTInit.Parity   = BSPUART_PARITY_EVEN ;//usartEvenParity;
	UARTInit.StopBits = BSPUART_STOPBITS_1  ;//usartStopbits1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8 ;//usartDatabits8;
	UARTInit.RxBuf    = App_485_RXTXBuff ;  //App_485_Comm_Data_Str.pRxBuff;
	UARTInit.RxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.TxBuf    = App_485_RXTXBuff;
	UARTInit.TxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.Mode     = UART_RS485_MODE;	// UART_DEFAULT_MODE  UART_RS485_MODE
	BSP_UART_Init(APP_485_COMM,&UARTInit);	
	
	App_485_Power_ON();	
}

void App_485_Power_ON(void)
{
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_485 ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_OFF ;		//此处和正常情况反过来
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);
}
void App_485_Power_OFF(void)
{
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_485 ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_ON ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);
	
	GPIO_PinModeSet( BSP_UART_0_RXPORT, BSP_UART_0_RXPIN, gpioModeInput, 1 );
	GPIO_PinModeSet( BSP_UART_0_TXPORT, BSP_UART_0_TXPIN, gpioModeInput, 1 );
	GPIO_PinModeSet( BSP_485_PORT, BSP_485_PORT, gpioModeInput, 1 );
}

void App_485_Set_WorkState(INT8U flag)
{
	App_485_Globe_Str.WorkState = flag;
}
INT8U App_485_Get_WorkState(void)
{
	return App_485_Globe_Str.WorkState;
}
/**********************************************************************************************
* Description       : 
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
void App_485_Task_TimeCall(void)
{
	if(App_485_Globe_Str.gDelayTime > 0)
	{
		App_485_Globe_Str.gDelayTime--;
	}
}
/**********************************************************************************************
* Description       : 
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
void App_485_SendArray(INT8U *pArray,INT16U Lenth)				//
{
	App_485_ClearBuff();
	BSP_UART_Write(APP_485_COMM,pArray,Lenth);
}
void App_485_SendStruct(__APP_485_T188_Frame *APP_485_T188_Frame)
{
	INT8U* FrameBuff = App_485_RXTXBuff;
	INT8U i = 0;
	INT8U Lenth = APP_485_T188_Frame->DataLenth;	
	App_485_ClearBuff();

	*FrameBuff++ = 0xfe;
	*FrameBuff++ = 0xfe;
	*FrameBuff++ = 0xfe;
	*FrameBuff++ = 0xfe;
	*FrameBuff++ = APP_485_T188_Frame->FrameStart;
	*FrameBuff++ = APP_485_T188_Frame->MeterType;
	*FrameBuff++ = APP_485_T188_Frame->Address[0];
	*FrameBuff++ = APP_485_T188_Frame->Address[1];
	*FrameBuff++ = APP_485_T188_Frame->Address[2];
	*FrameBuff++ = APP_485_T188_Frame->Address[3];
	*FrameBuff++ = APP_485_T188_Frame->Address[4];
	*FrameBuff++ = APP_485_T188_Frame->Address[5];
	*FrameBuff++ = APP_485_T188_Frame->Address[6];
	*FrameBuff++ = APP_485_T188_Frame->Contorl;
	*FrameBuff++ = APP_485_T188_Frame->DataLenth;
	for(i = 0; i <Lenth; i++)
	{
		*FrameBuff++ = *(APP_485_T188_Frame->Data+i);
	}
	*FrameBuff++ = APP_485_T188_Frame->CS;
	*FrameBuff++ = APP_485_T188_Frame->FrameStop;	

	for(i = 0; i <Lenth; i++)
	{
		*FrameBuff++ = *(APP_485_T188_Frame->Data+i);
	}
	*FrameBuff++ = APP_485_T188_Frame->CS;
	*FrameBuff++ = APP_485_T188_Frame->FrameStop;
	BSP_UART_Write(APP_485_COMM,App_485_RXTXBuff,17+Lenth);
}

/**********************************************************************************************
* Description       :?485???????
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
//INT16U App_485_ReceArray(INT8U *pArray,INT16U Lenth)			//???
//{
//	INT8U RxLen;
//	RxLen = BSP_UART_IsRxFrame(APP_485_COMM);
//	if(RxLen != 0)
//	{
//		pArray = App_485_RXTXBuff;
//	}
//	return RxLen;
//}
INT8U App_485_ReceStruct(__APP_485_T188_Frame *APP_485_T188_Frame)
{
	INT8U *pReceive;
	INT8U rxLen = 0;
	INT16U DataLenth;
	INT8U CS = 0;
	INT8U i;
	rxLen = BSP_UART_IsRxFrame(APP_485_COMM);
	pReceive = NULL;
	if(rxLen)
	{
		for(i=0;i<rxLen;i++)
		{
			if(App_485_RXTXBuff[i] == 0x68)				//pReceive指向帧头
			{
				//此处应添加一个帧尾的判断
				pReceive = App_485_RXTXBuff+i;
				APP_485_T188_Frame->FrameStart = *pReceive++;				
				break;
			}
		}			
		if(pReceive != NULL)
		{
			APP_485_T188_Frame->MeterType  = *pReceive++;			//表记类型
			APP_485_T188_Frame->Address[0] = *pReceive++;				//地址域
			APP_485_T188_Frame->Address[1] = *pReceive++;
			APP_485_T188_Frame->Address[2] = *pReceive++;
			APP_485_T188_Frame->Address[3] = *pReceive++;
			APP_485_T188_Frame->Address[4] = *pReceive++;
			APP_485_T188_Frame->Address[5] = *pReceive++;
			APP_485_T188_Frame->Address[6] = *pReceive++;
								
			APP_485_T188_Frame->Contorl 	= *pReceive++;				//控制码
			APP_485_T188_Frame->DataLenth	= *pReceive++;				//帧长
			DataLenth = APP_485_T188_Frame->DataLenth;					
//			APP_485_T188_Frame->Data = pReceive ;						//数据域
			memcpy(APP_485_T188_Frame->Data,pReceive,APP_485_T188_Frame->DataLenth);		//把数据域的数据复制出来
			pReceive += DataLenth;										
			APP_485_T188_Frame->CS = *pReceive++;						//CS校验
			APP_485_T188_Frame->FrameStop = *pReceive++;					//结束位
				
			CS += APP_485_T188_Frame->FrameStart;							//CS??
			CS += APP_485_T188_Frame->MeterType;		
			CS += APP_485_T188_Frame->Address[0];
			CS += APP_485_T188_Frame->Address[1];
			CS += APP_485_T188_Frame->Address[2];
			CS += APP_485_T188_Frame->Address[3];
			CS += APP_485_T188_Frame->Address[4];
			CS += APP_485_T188_Frame->Address[5];
			CS += APP_485_T188_Frame->Address[6];
			CS += APP_485_T188_Frame->Contorl;
			CS += APP_485_T188_Frame->DataLenth;
			for(i = 0;i< APP_485_T188_Frame->DataLenth;i++)
			{
				CS += APP_485_T188_Frame->Data[i];
			}
			if(CS == APP_485_T188_Frame->CS)						//CS校验成功
			{
				return 1;
			}
		}
	}
	return 0;																									
}
/**********************************************************************************************
* Description       :
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
void App_485_FillCSCheck(__APP_485_T188_Frame *APP_485_T188_Frame)
{
	INT8U CS = 0;
	INT8U i;
	CS += APP_485_T188_Frame->FrameStart;							//CS??
	CS += APP_485_T188_Frame->MeterType;		
	CS += APP_485_T188_Frame->Address[0];
	CS += APP_485_T188_Frame->Address[1];
	CS += APP_485_T188_Frame->Address[2];
	CS += APP_485_T188_Frame->Address[3];
	CS += APP_485_T188_Frame->Address[4];
	CS += APP_485_T188_Frame->Address[5];
	CS += APP_485_T188_Frame->Address[6];
	CS += APP_485_T188_Frame->Contorl;
	CS += APP_485_T188_Frame->DataLenth;
	for(i = 0;i< APP_485_T188_Frame->DataLenth;i++)
	{
		CS += APP_485_T188_Frame->Data[i];
	}
	APP_485_T188_Frame->CS = CS;
}
/**********************************************************************************************
* Description       :
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/

void App_485_FillAddress(__APP_485_T188_Frame *BeFill,__APP_485_T188_Frame *Fill)
{
	BeFill->FrameStart = 0x68;
	BeFill->MeterType  = 0x10;
	BeFill->Address[0] = Fill->Address[0];
	BeFill->Address[1] = Fill->Address[1];
	BeFill->Address[2] = Fill->Address[2];
	BeFill->Address[3] = Fill->Address[3];
	BeFill->Address[4] = Fill->Address[4];
	BeFill->Address[5] = Fill->Address[5];
	BeFill->Address[6] = Fill->Address[6];
	BeFill->FrameStop  = 0x16;
	App_485_FillCSCheck(BeFill);
}
/**********************************************************************************************
* Description       : 写入抄表帧，返回抄表状态
* Input             :  抄表数据
* Output            : 
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
__App_State_Enum App_485_ReadMeter(__APP_485_T188_Frame *APP_485_T188_Frame)
{
	INT32U rxLen;	
	__App_Para_InterfaceStr para_data;
	if(App_485_Globe_Str.gState == APP_485_STATE_START)				//第一次抄表
	{
		App_485_Globe_Str.MAX_ReCount = 2;					//最大抄表次数2次
		App_485_Globe_Str.gReCount = App_485_Globe_Str.MAX_ReCount;		//载入最大重抄次数
		App_485_Globe_Str.gState = APP_485_STATE_RESTART;		
	}
	if(App_485_Globe_Str.gState == APP_485_STATE_RESTART)                    
	{
//		App_485_SendStruct(APP_485_T188_Frame);					//发送抄表帧
		App_485_ClearBuff();					//清空收发缓冲区
		BSP_UART_Write(APP_485_COMM,ReedDataFrame,sizeof(ReedDataFrame));
		App_485_Globe_Str.gDelayTime = APP_485_DELAY_3S;			//抄表等待时间3秒
		App_485_Globe_Str.gState = APP_485_STATE_WAIT;
	}
	if(App_485_Globe_Str.gDelayTime == 0)				//延时时间到
	{
		App_485_Globe_Str.gReCount--;		
	}
	if(App_485_Globe_Str.gReCount == 0)					//没有重抄次数
	{
		App_485_Globe_Str.gState = APP_485_STATE_READMETER_FAIL;
		return APP_485_STATE_READMETER_FAIL;
	}
	rxLen = BSP_UART_IsRxFrame(APP_485_COMM);	
	if(rxLen)			//如果接收到数据
	{
		if(App_485_ReceStruct(APP_485_T188_Frame) == 1)						//如果接收到的数据正确，返回抄表成功，并把抄表数据写入E2，
		{
			App_485_Globe_Str.gState = APP_485_STATE_READMETER_OK;
			/*把接收到的数据写入E2*/
			para_data.afn = METER_WRITE;	
			para_data.IDIndex = 0x0c01;
			para_data.point = 1;
			para_data.dbuff = APP_485_T188_Frame->Data;
			para_data.dlen  =  APP_485_T188_Frame->DataLenth;
			GWFirstData_function(&para_data);		
			
			return APP_485_STATE_READMETER_OK;
		}
		else
		{
			App_485_ClearBuff();					//清空收发缓冲区
			App_485_Globe_Str.gReCount--;
		}
	}
	return APP_485_STATE_WAIT;
}

void App_ManipulatorMain(void)
{
	INT32U rxLen;	
	
	switch(SysGetKeyIntFlag())
	{
		case KEY_SHORT:				//短按,允许操控器控制
			App_485_Set_WorkState(3);		//设置为操控器状态
			App_485_Globe_Str.gDelayTime = APP_485_DELAY_60S;			//60S内允许操控器控制
			App_485_ManipulatorInit();						//485初始化
			Bsp_Led_SetH(BSP_ENUM_LED_UP_LINK);		//操控器指示灯亮
			break;
		case KEY_LONG:				//长按，关闭
			App_485_Globe_Str.gDelayTime = 0;	//操控时间清零
			break;
		default:
			break;
	}
	if(App_485_Get_WorkState() == 3)			//处在操控器状态
	{
//		Bsp_Led_On(BSP_ENUM_LED_UP_LINK,5);
		if(App_485_Globe_Str.gDelayTime == 0)				//延时时间到
		{
			App_485_Power_OFF();				//485电源关闭
			App_485_Set_WorkState(0);		//处在禁止抄表状态
			Bsp_Led_SetL(BSP_ENUM_LED_UP_LINK);//操控器指示灯灭
		}
		rxLen = BSP_UART_IsRxFrame(APP_485_COMM);	
		if(rxLen)			//如果接收到数据
		{
			App_NL_Receive(APP_485_COMM,App_485_RXTXBuff,rxLen);	//把数据交给NL的协议处理
		}
	}
}
/**********************************************************************************************
* Description       : 
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
INT8U READBUFF[100];
void App_ReadMeterMain(void)
{

	__APP_485_T188_Frame APP_485_T188_Frame;
	__App_Para_InterfaceStr para_data;
	INT8U Buff[100];
	APP_485_T188_Frame.Data = Buff;
	if(App_485_Get_WorkState() == 2)	//立即开始抄表
	{
		if(App_485_Globe_Str.gSetup == APP_485_SETUP_POWER_OFF)		//如果处在关机状态
		{
			App_485_Init();
			App_485_Globe_Str.gSetup = APP_485_SETUP_POWER_ON;
		}
		if(App_485_Globe_Str.gSetup == APP_485_SETUP_POWER_ON)		//如果处在开机状态
		{
			App_485_Globe_Str.gSetup = APP_485_SETUP_READ_METER;
			App_485_Globe_Str.gState = APP_485_STATE_START;
		}
		if(App_485_Globe_Str.gSetup == APP_485_SETUP_READ_METER)	//抄表
		{
			switch(App_485_ReadMeter(&APP_485_T188_Frame))
			{
				case APP_485_STATE_READMETER_FAIL:
					App_485_Globe_Str.gSetup = APP_485_SETUP_CLOSE;
					break;
				case APP_485_STATE_READMETER_OK:						//抄表成功则把数据写入E2
					App_485_Globe_Str.gSetup = APP_485_SETUP_CLOSE;
						para_data.afn = METER_WRITE;	
						para_data.IDIndex = 0x0c01;
						para_data.point = 1;
						para_data.dbuff = APP_485_T188_Frame.Data+3;		//指向实际数据
						para_data.dlen  =  APP_485_T188_Frame.DataLenth-3;
						GWFirstData_function(&para_data);
					break;
				default:
					break;
			}
			
		}
		if(App_485_Globe_Str.gSetup == APP_485_SETUP_CLOSE)			//关闭
		{
			App_485_Set_WorkState(0);			//禁止抄表
			App_485_Globe_Str.gSetup = APP_485_SETUP_POWER_OFF;
		}
	}
}













/***********************************************************************************/
