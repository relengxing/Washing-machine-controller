/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "App_485.h"
#include "bsp_adc.h"
#include "bsp_dac.h"
#include "bsp_led.h"
#include "Bsp_Uart.h"
#include "bsp_rtc.h"
#include "bsp_power_ctrl.h"
#include "em_adc.h"
#include "App_Head.h"
#include "convertDataInterface.h"
#include "App_Para_Interface.h"
#include "App_Data_interface.h"

/* Private define ------------------------------------------------------------*/
#define APP_FRAMEBUFF_LEN	100//150
#define APP_LED_METER_TIME	5  //UNIT MS
//#define APP_485_COMM		2
/* Private typedef -----------------------------------------------------------*/

//typedef enum{APP_MBUS_STEP_POWER_ON,APP_MBUS_STEP_IDEAL,APP_MBUS_STEP_POWER_OFF,BSP_MBUS_RST_PIN_L,APP_MBUS_STEP_RST_H,APP_MBUS_STEP_AD,APP_MBUS_STEP_DA \
//				,APP_MBUS_STEP_TX,APP_MBUS_STEP_RX}__App_Mbus_RM_Step ;
typedef enum{APP_MBUS_STEP_POWER_ON,APP_MBUS_STEP_DA,APP_MBUS_STEP_IDEAL,APP_MBUS_STEP_POWER_OFF,APP_MBUS_STEP_RST_L,APP_MBUS_STEP_RST_H,APP_MBUS_STEP_AD_1,APP_MBUS_STEP_AD_2 \
				,APP_MBUS_STEP_TX,APP_MBUS_STEP_RX,APP_MBUS_STEP_OK,APP_MBUS_STEP_WARM,APP_MBUS_STEP_BATT1,APP_MBUS_STEP_BATT2,APP_MBUS_STEP_BATT3}__App_Mbus_RM_Step ;
__packed typedef struct __App_Mbus_RM_Ctrl__
{
	__App_Mbus_RM_Step  Step ;
	INT16U  TimeOutMs ;
	INT8U   WindowsTime ;
	INT8U   TxAmount ;
	__Bsp_Enum_Adc_Switch   PowerState  ; //电源状态 
	INT8U   MbusAlarm ;  
	INT8U   RMeterFlag ;	/*是否启动抄表*/
	INT16U  Point ;
	INT8U   MapOk[POINTAMOUNT/8];	
	INT8U   RepCount;
}__App_Mbus_RM_Ctrl;




typedef struct	__App_485_Comm_Data_Str__				//485收发缓冲区
{
	INT8U 	*pRxBuff;
	INT16U 	RxLen ;
	INT8U 	*pTxBuff;
	INT16U 	TxLen ;
} __App_485_Comm_Data_Str;




/* Private macro -------------------------------------------------------------*/
INT8U ReadMeter_Interval;		//抄表间隔
INT8U ReadMeter_Time;			//抄表时间

/* Private variables ---------------------------------------------------------*/
INT8U App_ReadMeterBuff[APP_FRAMEBUFF_LEN];
INT8U App_ReadMeterFrame[APP_FRAMEBUFF_LEN];
__App_485_Comm_Data_Str		App_485_Comm_Data_Str=
{
	App_ReadMeterBuff,
	0,
	App_ReadMeterFrame,
	0
};
__App_Mbus_RM_Ctrl App_Mbus_RM_Ctrl ;

/* Private function prototypes -----------------------------------------------*/
void App_Mbus_Rm_Main(void);

INT16U App_Mbus_Get_Batt_Volt(void);

INT8U App_MBus_Alarm(void);

//转化为0.1的倍数
INT8U App_Batt_BinData(void);

INT8U App_Read_OneMeter_Func(__App_Mbus_RM_Ctrl *pRM_Ctrl);

INT8U App_RMeter_RecDeal(INT8U Point,INT8U *pData,INT8U Len);


/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Description: 485抄表通讯初始化
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_485_Init(void)
{
	S_UARTx_Set UARTInit;						// 参数结构
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	UARTInit.BaudRate = 2400;
	UARTInit.Parity   = BSPUART_PARITY_EVEN ;//usartEvenParity;
	UARTInit.StopBits = BSPUART_STOPBITS_1  ;//usartStopbits1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8 ;//usartDatabits8;
	UARTInit.RxBuf    = App_ReadMeterBuff ;  //App_485_Comm_Data_Str.pRxBuff;
	UARTInit.RxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.TxBuf    = App_ReadMeterFrame;
	UARTInit.TxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.Mode     = UART_RS485_MODE;//UART_DEFAULT_MODE ;//UART_RS485_MODE;	// UART_DEFAULT_MODE
	BSP_UART_Init(APP_485_COMM,&UARTInit);	
	
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_485 ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_OFF ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);
}

/******************************************************************************
* Description: 抄表口通讯初始化接口
* Input:  ChInform--抄表口参数，参见路由定义
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_Meter_PortInit(INT8U ChInform)
{
	S_UARTx_Set UARTInit;						// 参数结构
	INT8U tmp ;
//rate
	tmp = ChInform&0x07;
	if((tmp>5)||(!tmp))
	{
		tmp = 3 ;
	}
	tmp-- ;
	UARTInit.BaudRate = 600*(1<<tmp);			
	
//port
//	tmp = (ChInform >> 3)&0x03;

//even /odd
	tmp = (ChInform >> 5)&0x03;
	UARTInit.Parity   = BSPUART_PARITY_EVEN ;//usartEvenParity;
	if(!tmp)
	{
		UARTInit.Parity = BSPUART_PARITY_NO ;
	}
	else if(1==tmp)
	{
		UARTInit.Parity = BSPUART_PARITY_ODD ;
	}	
	UARTInit.StopBits = BSPUART_STOPBITS_1  ;//usartStopbits1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8 ;//usartDatabits8;
	UARTInit.RxBuf    = App_ReadMeterBuff ;  //App_485_Comm_Data_Str.pRxBuff;
	UARTInit.RxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.TxBuf    = App_ReadMeterBuff;
	UARTInit.TxBufLen = APP_FRAMEBUFF_LEN ;
	UARTInit.Mode     = UART_DEFAULT_MODE ;//UART_RS485_MODE;UART_HALFDUP_MODE	// UART_DEFAULT_MODE
	BSP_UART_Init(APP_485_COMM,&UARTInit);	
}

/******************************************************************************
* Description: 485抄表发送指令
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_485_Send(INT8U *pArray,INT16U Lenth)
{
	BSP_UART_Write(APP_485_COMM,pArray,Lenth);
}

INT8U App_OneData_Deal(__App_Para_InterfaceStr *pData)
{
//			__App_Para_InterfaceStr tData ;
	INT8U rLen = 0;
	rLen = GWFirstData_function(pData);
	return rLen ;

}

/******************************************************************************
* Description: 485抄表接收解析并写入数据
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
INT8U App_RMeter_RecDeal(INT8U Point,INT8U *pData,INT8U Len)
{
	INT8U *pRec = pData ;
	INT8U i=0;
	for(i=0;i<Len;i++)
	{
		if(0x68 == *pRec)				//pReceive指向帧头
		{
			INT8U framLen = *(pRec+10)+11;
			INT8U cs = 0 ;
			INT8U countIndex ;
			__App_Para_InterfaceStr tData ;
			for(countIndex=0 ;countIndex < framLen;countIndex++)
			{
				cs+=pRec[countIndex];
			}
			if((cs!=pRec[countIndex])||(0x16!=pRec[countIndex+1]))
			{
				return 0 ;
			}
			/**/
			tData.afn  		= METER_WRITE;
			tData.dbuff		= pRec+11 ;	 
			tData.dlen		= 10    ;
			tData.IDIndex	= 0xC01 ;
			tData.point		= Point ;
			return GWFirstData_function(&tData);
		}
		pRec++ ;
	}
	return 0;
}



////////////////////////////////////rf 抄表调用////////////////////////////////////////////////
INT32U DAC_ValueTest1 = 0 ;
INT32U DAC_ValueTest2 = 0 ;
INT32U DAC_ValueTest = 1000 ;

INT32U App_Batt_Value1 = 0 ;
INT32U App_Batt_Value2 = 0 ;
INT32U App_Batt_Value3 = 0 ;

INT16U xxxxCHAOBIAO ;
INT16U xxxxCHAOBIAOWarm ;
INT16U xxxxCHAOBIAOEEor ;


INT8U App_ReadMeter_Main(INT8U *pBuff,INT8U *pLen,INT8U ChInform)
{
	if((NULL==pBuff)||(NULL==pLen))
	{
		*pLen = 0 ;
		return 0;
	}
	App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_POWER_ON  ;		

	BSP_UART_ClearRxBuff(APP_485_COMM);
xxxxCHAOBIAO ++ ;
	App_Mbus_RM_Ctrl.TxAmount = 0 ;
	while(1)
	{
//		INT32U adcData = 0 ;
		INT8U  rxLen = 0 ;
		if((APP_MBUS_STEP_RX==App_Mbus_RM_Ctrl.Step) &&(!App_Mbus_RM_Ctrl.TimeOutMs))
		{
			App_Mbus_RM_Ctrl.TxAmount++ ;
			if(App_Mbus_RM_Ctrl.TxAmount > 3)
			{
				xxxxCHAOBIAOEEor++ ;
				App_Mbus_RM_Ctrl.TxAmount = 0 ;
				App_Mbus_Power_Off_PinInit();
				*pLen = 0 ;
				return *pLen ;
				
			}
			App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_TX ;
		}
		if((App_Mbus_RM_Ctrl.TimeOutMs)&&(APP_MBUS_STEP_RX!=App_Mbus_RM_Ctrl.Step ))
		{
			continue ;
		}
		switch(App_Mbus_RM_Ctrl.Step)
		{
			case APP_MBUS_STEP_POWER_ON :
				CMU_ClockEnable(cmuClock_HFPER, true);
				CMU_ClockEnable(cmuClock_GPIO, true);
				GPIO_PinModeSet(BSP_MBUS_POWER_PORT, BSP_MBUS_POWER_PIN, gpioModePushPull, 1); 
				App_Mbus_RM_Ctrl.TimeOutMs = 0;//光电//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_RST_H ;
				break;
			case APP_MBUS_STEP_RST_H :
//				App_485_Init(); // weile tiaoshi TX H
				App_Meter_PortInit(ChInform);
				GPIO_PinModeSet(BSP_MBUS_RST_PORT, BSP_MBUS_RST_PIN, gpioModePushPull, 1);  //脉冲
				App_Mbus_RM_Ctrl.TimeOutMs = 200 ;//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_RST_L ;
				break;
			case APP_MBUS_STEP_RST_L :
				GPIO_PinModeSet(BSP_MBUS_RST_PORT, BSP_MBUS_RST_PIN, gpioModePushPull, 0);
				App_Mbus_RM_Ctrl.TimeOutMs = 1000 ;//光电需要--UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_WARM ;
				break ;
			case APP_MBUS_STEP_WARM:
				App_Mbus_RM_Ctrl.MbusAlarm = 0;
							
				GPIO_PinModeSet(BSP_MBUS_WARM_PORT, BSP_MBUS_WARM_PIN, gpioModeInput, 0);		
				if(GPIO_PinInGet(BSP_MBUS_WARM_PORT, BSP_MBUS_WARM_PIN))
				{
xxxxCHAOBIAOWarm++ ;
					/* is warm  give up*/
					App_Mbus_Power_Off_PinInit();
					App_Mbus_RM_Ctrl.PowerState = BSP_ADC_POWER_OFF ;
					App_Mbus_RM_Ctrl.MbusAlarm = 1;
					*pLen = 0 ;
					Bsp_Led_On(BSP_ENUM_LED_WARM,APP_LED_METER_TIME);
					return *pLen ;
				}
				App_Mbus_RM_Ctrl.PowerState = BSP_ADC_POWER_ON ;
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_BATT1 ;

			case APP_MBUS_STEP_BATT1:
				App_Batt_Value1 = Bsp_Get_AdcData(BSP_ENUM_ADC_BATT);
				App_Mbus_RM_Ctrl.TimeOutMs = 5 ;//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_BATT2 ;
				
				break;
			case APP_MBUS_STEP_BATT2:
				App_Batt_Value2 = Bsp_Get_AdcData(BSP_ENUM_ADC_BATT);
				App_Mbus_RM_Ctrl.TimeOutMs = 5 ;//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_BATT3 ;
				
				break;
			case APP_MBUS_STEP_BATT3:
				App_Batt_Value3 = Bsp_Get_AdcData(BSP_ENUM_ADC_BATT);
				App_Mbus_RM_Ctrl.TimeOutMs = 5 ;//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_AD_1 ;				
				break;

			case APP_MBUS_STEP_AD_1 :
				DAC_ValueTest1 = Bsp_Get_AdcData(BSP_ENUM_ADC_MBUS);
				App_Mbus_RM_Ctrl.TimeOutMs = 5 ;//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_AD_2 ;
				break;
			case APP_MBUS_STEP_AD_2 :
				DAC_ValueTest2 = Bsp_Get_AdcData(BSP_ENUM_ADC_MBUS);
				App_Mbus_RM_Ctrl.TimeOutMs = 10 ;//UNIT MS
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_DA ;
				/*this case have don't break*/
				break;
			case APP_MBUS_STEP_DA :
				Bsp_DAC_setup();
				DAC_ValueTest = (DAC_ValueTest1+DAC_ValueTest2)/2;
				DAC_ValueTest *= 2;  
				if(DAC_ValueTest > 4095)
				{
					DAC_ValueTest = 4095 ;
				}
				/* Write the new value to DAC register */
				Bsp_Mbus_DAC_WriteData(DAC0,DAC_ValueTest);			
				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_TX ;
				App_Mbus_RM_Ctrl.TimeOutMs = 10 ;//UNIT MS

				break;
			case APP_MBUS_STEP_TX:
				BSP_UART_ClearRxBuff(APP_485_COMM);
//				memcpy(App_ReadMeterBuff,pBuff,*pLen);
				BSP_UART_Write(APP_485_COMM,pBuff,*pLen);

				App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_RX ;
				App_Mbus_RM_Ctrl.TimeOutMs = 1500 ;//UNIT MS
				Bsp_Led_On(BSP_ENUM_LED_LINK_DOWN,APP_LED_METER_TIME);
				break ;
			case APP_MBUS_STEP_RX:
				rxLen = BSP_UART_IsRxFrame(APP_485_COMM);
				if(rxLen > 10)
				{
//					INT16S tmp ;
					 
					Bsp_Led_On(BSP_ENUM_LED_LINK_DOWN,APP_LED_METER_TIME);
					App_Mbus_Power_Off_PinInit();
					
//					//添加采集器信息 RSSI SNR volt alarm
//					/*获取RSSI & SNR 值 */
//					tmp = SX1276LoRaGetPacketRssi() ;
//					tmp += 40;
//					if(tmp < -128)
//					{
//						tmp = 0x80;
//					}
//					
//					*pBuff ++ = tmp ;//SX1276LoRaGetPacketRssi();
//					*pBuff ++ = SX1276LoRaGetPacketSnr();
//					*pBuff ++ = App_Batt_BinData();
//					*pBuff ++ = App_MBus_Alarm();
					/*解析并写数据*/
					if(App_RMeter_RecDeal(App_Mbus_RM_Ctrl.Point,App_ReadMeterBuff,rxLen))
					{
						App_Mbus_RM_Ctrl.TxAmount = 0 ;
						return APP_MBUS_STEP_OK ;
						
					}
				}
				break ;
			case APP_MBUS_STEP_IDEAL:
				break;
			default:
				return 0;
		}
	}	
}

INT16U App_Mbus_Get_Batt_Volt(void)
{
	INT16U minTmp,maxTmp ;
	INT16U battVolt;
	if(App_Batt_Value1 > App_Batt_Value2)
	{
		
		maxTmp = App_Batt_Value1;
		minTmp = App_Batt_Value2 ;
	}
	else
	{
		minTmp = App_Batt_Value1;
		maxTmp = App_Batt_Value2 ;
	}
	if(maxTmp < App_Batt_Value3)
	{
		maxTmp = App_Batt_Value3 ;
	}
	if(minTmp > App_Batt_Value3)
	{
		minTmp = App_Batt_Value3 ;
	}
	battVolt = (App_Batt_Value1+App_Batt_Value2+App_Batt_Value3) - minTmp - maxTmp ;
	return  battVolt ;

}

//转化为0.1的倍数
INT8U App_Batt_BinData(void)
{
	INT32U tmp = App_Mbus_Get_Batt_Volt();
	
	tmp *= 33*4 ;  //基准是3.3V
	tmp/=4095 ;
    return tmp ;   	
}

INT8U App_MBus_Alarm(void)
{
	return App_Mbus_RM_Ctrl.MbusAlarm ;
}
/******************************************************************************
* Description: 进入LP时，把相应的管脚处理一下
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_Mbus_Power_Off_PinInit(void)
{
//	if(BSP_ADC_POWER_ON ==App_Mbus_RM_Ctrl.PowerState )
	{
		USART_Reset(USART0);
		DAC_Reset(DAC0);
		ADC_Reset(ADC0);
		//tx rx 
		GPIO_PinModeSet( BSP_UART_1_TXPORT, BSP_UART_1_TXPIN, gpioModePushPull, 0 );
		GPIO_PinModeSet( BSP_UART_1_RXPORT, BSP_UART_1_RXPIN, gpioModePushPull, 1 );
		//mbus_on/off mbus_rst 
		GPIO_PinModeSet( BSP_MBUS_POWER_PORT, BSP_MBUS_POWER_PIN, gpioModePushPull, 0 );
		GPIO_PinModeSet( BSP_MBUS_RST_PORT  , BSP_MBUS_RST_PIN  , gpioModePushPull, 0 );
		//vda/vad/alarm
		GPIO_PinModeSet( BSP_MBUS_WARM_PORT , BSP_MBUS_WARM_PIN , gpioModePushPull, 0 );
		GPIO_PinModeSet( BSP_MBUS_AD_PORT   , BSP_MBUS_AD_PIN   , gpioModePushPull, 0 );
		GPIO_PinModeSet( BSP_MBUS_DA_PORT   , BSP_MBUS_DA_PIN   , gpioModePushPull, 0 );
	}
	App_Mbus_RM_Ctrl.PowerState = BSP_ADC_POWER_OFF ;
}



//////////////////////////////////////////////mbus///////////////////////////////////////////////////
//#include "bsp_rtc.h"
//void TestMbus(void)
//{
//	_BSPRTC_TIME t_BSPRTC_TIME;
//	BSP_RTCGetTime(&t_BSPRTC_TIME);
//	if(!(t_BSPRTC_TIME.Second%5))
//	{
//		INT8U arrary[50]={	0xFE,0xFE ,0xFE ,0xFE ,0x68 ,0x10 ,0x14 ,0x00 ,0x10 ,0x12 ,0x00 ,0x00 ,0x00 ,0x01 ,0x03 ,0x90 ,0x1F ,0x00 ,0x61 ,0x16};
//			INT8U len = 20 ;
//		App_ReadMeter_Main(arrary,&len);
//		
//	}
//}



//typedef enum{APP_MBUS_STEP_POWER_OFF,APP_MBUS_STEP_POWER_ON,APP_MBUS_STEP_RST,APP_MBUS_STEP_AD,APP_MBUS_STEP_DA \
//				,APP_MBUS_STEP_TX,APP_MBUS_STEP_RX}__App_Mbus_RM_Step ;
//__packed typedef struct __App_Mbus_RM_Ctrl__
//{
//	INT8U  Step ;
//	INT8U  TimeOut ;
//	INT8U  WindowsTime ;
//}__App_Mbus_RM_Ctrl__;

void App_Mbus_Time_msCall(void)
{
	if(App_Mbus_RM_Ctrl.TimeOutMs)
	{
		App_Mbus_RM_Ctrl.TimeOutMs-- ;
	}

}

#define APP_READMETER_COUNT	3
__App_Task_State App_Mbus_ReadMeter_Main(void)
{

	if(!App_Mbus_RM_Ctrl.RMeterFlag)
	{
		/*读取上报时间点，提前1小时启动*/
		__App_Para_InterfaceStr  pareStuct;
		INT8U  arrayPara[10] ;
		INT8U  tmp = 0 ;
		_BSPRTC_TIME tTime;

		pareStuct.afn = METER_READ; 
		pareStuct.IDIndex = 0x409;
		pareStuct.point = 0;
		pareStuct.dlen = sizeof(arrayPara);
		pareStuct.dbuff =arrayPara;
		GWPara_function(&pareStuct);
		if(APP_TASK_REP_OPEN!=*pareStuct.dbuff)
		{
			return APP_TASK_FREE ;
		}
		/*开启--> 检测周期*/
		BSP_RTCGetTime(&tTime);
		tmp = *(pareStuct.dbuff+1);
		switch(tmp)
		{
			case APP_TASK_CYCLE_MONTH:
				if(tTime.Day!=*(pareStuct.dbuff+3))
				{
					return APP_TASK_FREE ;				
				}
			case APP_TASK_CYCLE_WEEK:
				if(tTime.Week!=*(pareStuct.dbuff+2))
				{
					return APP_TASK_FREE;
				}
				break;
			case APP_TASK_CYCLE_DAY:
				break;
			case APP_TASK_CYCLE_HOUR:
				break;
			default:
				return APP_TASK_FREE;
			
		}
		tmp = ByteBcdToHex(*(pareStuct.dbuff+4)) ;
		if(tmp)
		{
			tmp--;
		}
		tmp = ByteHexToBcd(tmp);
		if(tTime.Hour!=tmp)
		{
			return APP_TASK_FREE ;				
		}
		if(tTime.Minute!=*(pareStuct.dbuff+5))
		{
			return APP_TASK_FREE ;				
		}
		App_Mbus_RM_Ctrl.RMeterFlag =1 ;
		App_Mbus_RM_Ctrl.RepCount = 0 ;		
	}
	/*启动抄表程序*/
	else if(App_Mbus_RM_Ctrl.RMeterFlag)
	{


//__packed typedef struct __App_Mbus_RM_Ctrl__
//{
//	__App_Mbus_RM_Step  Step ;
//	INT16U  TimeOutMs ;
//	INT8U   WindowsTime ;
//	INT8U   TxAmount ;
//	__Bsp_Enum_Adc_Switch   PowerState  ; //电源状态 
//	INT8U   MbusAlarm ;  
//	INT8U   RMeterFlag ;	/*是否启动抄表*/
//}__App_Mbus_RM_Ctrl;

		/*补抄是否超限*/
		if(App_Mbus_RM_Ctrl.RepCount >APP_READMETER_COUNT)
		{
			App_Mbus_RM_Ctrl.RMeterFlag = 0 ; 
			App_Mbus_RM_Ctrl.Point      = 0 ;
			App_Mbus_RM_Ctrl.RepCount   = 0 ;
			return APP_TASK_FREE ;			
		}
		if(App_Mbus_RM_Ctrl.Point > POINTAMOUNT)
		{
//			App_Mbus_RM_Ctrl.RMeterFlag = 0 ; 
			App_Mbus_RM_Ctrl.Point      = 0 ;
			App_Mbus_RM_Ctrl.RepCount++ ;
//			return APP_TASK_FREE ;
		}
		/*是否补抄阶段*/
		if(App_Mbus_RM_Ctrl.RepCount>1)
		{
			if(App_Mbus_RM_Ctrl.MapOk[App_Mbus_RM_Ctrl.Point/8]&BIT(App_Mbus_RM_Ctrl.Point%8))
			{
				App_Mbus_RM_Ctrl.Point++ ;
				return APP_TASK_BUSY ;
			}
		}
		else
		{
			App_Mbus_RM_Ctrl.MapOk[App_Mbus_RM_Ctrl.Point/8]&=~BIT(App_Mbus_RM_Ctrl.Point%8);
			
		}
		App_Mbus_RM_Ctrl.Point++ ;
		if(TRUE == App_Read_OneMeter_Func(&App_Mbus_RM_Ctrl))
		{
			/*解析并写入*/
			
			App_Mbus_RM_Ctrl.MapOk[App_Mbus_RM_Ctrl.Point/8]|=BIT(App_Mbus_RM_Ctrl.Point%8);
		}
	}
	return APP_TASK_FREE ;
	
	

}

INT8U App_DataIsValidBcd(INT8U*pData,INT8U Len)
{
	INT8U i ;
	for(i = 0 ; i < Len ;i++)
	{
		INT8U tmpH = *pData>>4 ;
		INT8U tmpL = *pData&0X0F;
		
		if((tmpH > 0x09)||(tmpL>0x09))
		{
			return FALSE  ;
		}
	}
	return TRUE ;
}


INT8U App_Read_OneMeter_Func(__App_Mbus_RM_Ctrl *pRM_Ctrl)
{
	__App_Para_InterfaceStr  pareStuct;
	INT8U  arrayPara[10] ;
//	INT8U  tmp = 0 ;
	INT8U  *pFrame ;
	INT8U  lenFrame ;
	INT8U  cs = 0 ;
	INT8U  i = 0 ;
	if(!pRM_Ctrl->Point)
	{
		return FALSE ;
	}

	pareStuct.afn = METER_READ; 
	pareStuct.IDIndex = 0x406;
	pareStuct.point = pRM_Ctrl->Point;
	pareStuct.dlen = sizeof(arrayPara);
	pareStuct.dbuff =arrayPara;
	GWPara_function(&pareStuct);
	if(TRUE != App_DataIsValidBcd(arrayPara,7))
	{
		return FALSE ;	
	}
	/*组帧了 抄表*/
	lenFrame = 0 ;
	pFrame = App_ReadMeterFrame ;

	memset(pFrame,0xfe,4);
	pFrame  +=4;
	lenFrame+=4 ;
	//start char
	*pFrame++  = 0x68 ;
	lenFrame++ ;
	//type;
	*pFrame++  = 0x10 ;
	lenFrame++ ;
	//addr
	memcpy(pFrame,pareStuct.dbuff,7);
	pFrame  +=7;
	lenFrame+=7 ;
	//cmd
	*pFrame++  = 0x1 ;
	lenFrame++ ;
	//len
	*pFrame++  = 0x3 ;
	lenFrame++ ;
	//id
	*pFrame++  = 0x90 ;
	lenFrame++ ;
	*pFrame++  = 0x1f ;
	lenFrame++ ;
	//seq
	*pFrame++  = 0x00 ;
	lenFrame++ ;
	//cs
	cs = 0 ;
	for(i = 0 ; i < lenFrame ; i ++)
	{
		cs+=App_ReadMeterFrame[4+i];
	}
	*pFrame++  = cs ;
	lenFrame++ ;
	//end char 
	*pFrame++  = 0x16 ;
	lenFrame++ ;
	//里面有while
	if(APP_MBUS_STEP_OK == App_ReadMeter_Main(App_ReadMeterFrame,&lenFrame,0x53))
	{
		return TRUE ;
	}
	return FALSE ;
	
}


//void App_Mbus_Rm_Main(void)
//{
//	INT32U adcData = 0 ;
//	INT8U  rxLen = 0 ;
//	if(App_Mbus_RM_Ctrl.TimeOutMs)
//	{
//		return ;
//	}
//	switch(App_Mbus_RM_Ctrl.Step)
//	{
//		case APP_MBUS_STEP_POWER_ON :
//			CMU_ClockEnable(cmuClock_HFPER, true);
//			CMU_ClockEnable(cmuClock_GPIO, true);
//			GPIO_PinModeSet(BSP_MBUS_POWER_PORT, BSP_MBUS_POWER_PIN, gpioModePushPull, 1);
//			App_Mbus_RM_Ctrl.TimeOutMs = 1000 ;//UNIT MS
//		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_RST_L ;
//			break;
//		case APP_MBUS_STEP_RST_L :
//			App_485_Init(); // weile tiaoshi TX H
//			GPIO_PinModeSet(BSP_MBUS_RST_PORT, BSP_MBUS_RST_PIN, gpioModePushPull, 0);
//			App_Mbus_RM_Ctrl.TimeOutMs = 1000 ;//UNIT MS
//		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_RST_H ;
//			break;
//		case APP_MBUS_STEP_RST_H :
//			GPIO_PinModeSet(BSP_MBUS_RST_PORT, BSP_MBUS_RST_PIN, gpioModePushPull, 1);
//			App_Mbus_RM_Ctrl.TimeOutMs = 500 ;//UNIT MS
//		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_AD_1 ;
//			GPIO_PinModeSet(BSP_MBUS_WARM_PORT, BSP_MBUS_WARM_PIN, gpioModeInput, 1);		
////			if(!GPIO_PinInGet(BSP_MBUS_WARM_PORT, BSP_MBUS_WARM_PIN))
////			{
////				/* is warm  give up*/
////				return ;
////			}
//			break;
//		case APP_MBUS_STEP_AD_1 :
////			DAC_ValueTest = Bsp_Get_AdcData(BSP_ENUM_ADC_MBUS);
////			App_Mbus_RM_Ctrl.TimeOutMs = 500 ;//UNIT MS
//		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_DA ;
//			break;
//		case APP_MBUS_STEP_DA :
////			test_dac();
//			Bsp_DAC_setup();
//			DAC_ValueTest *= 2;  
//			if(DAC_ValueTest > 4095)
//			{
//				DAC_ValueTest = 4095 ;
//			}
//			/* Write the new value to DAC register */
//			Bsp_Mbus_DAC_WriteData(DAC0,DAC_ValueTest);			
//		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_TX ;//APP_MBUS_STEP_TX ;
//			App_Mbus_RM_Ctrl.TimeOutMs = 50 ;//UNIT MS
//			break;
////		case APP_MBUS_STEP_TX:
////			App_485_Send(readFrame,20);
////		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_RX ;
////			App_Mbus_RM_Ctrl.TimeOutMs = 3000 ;//UNIT MS
////			break ;
////		case APP_MBUS_STEP_RX:
////			rxLen = BSP_UART_IsRxFrame(APP_485_COMM);
////			if(rxLen != 0)
////			{
////				App_485_Rece();
////				BSP_UART_ClearRxBuff(APP_485_COMM);
////			}
//		    App_Mbus_RM_Ctrl.Step = APP_MBUS_STEP_IDEAL ;			
//			break ;
//		case APP_MBUS_STEP_IDEAL:
//		    break;
//		default:
//			break;
//	}

//}


/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
