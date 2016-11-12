/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : sysTick.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "App_M6310.h"

/* Private define ------------------------------------------------------------*/
#define APP_NEOM6310_RXBUFFLEN	100
#define APP_NEOM6310_TXBUFFLEN	200

#define APP_GPRS_DELAY_1S   1000
#define APP_GPRS_DELAY_2S   (APP_GPRS_DELAY_1S*2)
#define APP_GPRS_DELAY_3S   (APP_GPRS_DELAY_1S*3)
#define APP_GPRS_DELAY_5S   (APP_GPRS_DELAY_1S*5)
#define APP_GPRS_DELAY_10S  (APP_GPRS_DELAY_1S*10)
#define APP_GPRS_DELAY_15S  (APP_GPRS_DELAY_1S*15)
#define APP_GPRS_DELAY_20S  (APP_GPRS_DELAY_1S*20)
#define APP_GPRS_DELAY_25S  (APP_GPRS_DELAY_1S*25)
#define APP_GPRS_DELAY_30S  (APP_GPRS_DELAY_1S*30)
#define APP_GPRS_DELAY_50S  (APP_GPRS_DELAY_1S*50)
#define APP_GPRS_DELAY_180S (APP_GPRS_DELAY_1S*180)

/* Private typedef -----------------------------------------------------------*/
typedef struct	__App_NeoM6310_Usart_Str__		//GPRS所用串口缓冲区定义
{

	INT8U RxBuff[APP_NEOM6310_RXBUFFLEN];
	INT16U RxLen ;
	INT8U TxBuff[APP_NEOM6310_TXBUFFLEN];
	INT16U TxLen ;
} __App_NeoM6310_Usart_Str;

typedef enum __App_State_Enum__
{	APP_GPRS_STATE_POWER_OFF,			//模块断电
	APP_GPRS_STATE_POWER_ON,			//模块上电
	APP_GPRS_STATE_START,				//开始发送
	APP_GPRS_STATE_RESTART,				//重新发送
	APP_GPRS_STATE_TIME_WAITING,		//等待接收
	APP_GPRS_STATE_RX_OK,				//接收正确
	APP_GPRS_STATE_RX_ERROR,			//接收错误
	APP_GPRS_STATE_TIMEOUT,				//接收超时
	APP_GPRS_STATE_CONNET_OK,			//连接成功
	APP_GPRS_STATE_CONNET_FAIL,			//连接失败
}__App_State_Enum;

typedef enum {	
	APP_GPRS_SETUP_POWER_OFF,			//关机状态

	APP_GPRS_SETUP_POWER_ON,			//开机
	
	APP_GPRS_SETUP_BAUDRATE,			//设置波特率

	APP_GPRS_SETUP_CSQ,					//检测信号强度

	APP_GPRS_SETUP_QIMODE,				//设置透传模式

	APP_GPRS_SETUP_TCPSETUP,			//建立TCP/IP连接

	APP_GPRS_SETUP_REPORT,				//通讯
	
	APP_GPRS_SETUP_DROP,				//退出数据模式

	APP_GPRS_SETUP_TCPCLOSE,			//断开TCP/IP连接

	APP_GPRS_SETUP_POWER_CLOSE,			//关机
	
	APP_GPRS_SETUP_REPOWER,				//重新开机
	
	APP_GPRS_SETUP_CLOSE_RET,			//关闭回显
	
	APP_GPRS_SETUP_REP_DATA,			//上报的数据
	
	APP_GPRS_SETUP_QSCLK,				//睡眠模式
	
}__App_Setup_Enum;


typedef struct	__App_Gprs_Globe_Str__				
{
	__App_State_Enum	gState;
	__App_Setup_Enum	gSetup;
	INT32U  gDelayTime;
	INT32U	MAX_DelayTime;
	INT8U	gReCount;
	INT8U	MAX_ReCount;
	INT8U   WorkState ;
} __App_Gprs_Globe_Str;



/* Private macro -------------------------------------------------------------*/
INT8U GPRS_Interval;
INT8U GPRS_Time;
/* Private variables ---------------------------------------------------------*/
/*-AT命令！------------------------------------------------------------------*/
INT8U App_AT_IPR[]="AT+IPR=115200&W\r\n";
INT8U App_AT_CSQ_Buff[] = "AT+CSQ\r\n";
INT8U App_AT_ATE_Buff[] = "ATE0\r\n";
INT8U App_AT_QMODE[]="AT+QIMODE=1\r\n";
INT8U App_AT_TCPSETUP_Buff[] = "AT+QIOPEN=\"TCP\",\"100.100.100.100\",\"1234\"\r\n\0\0\0\0";//设置主站IP，可修改
//INT8U App_AT_TCPSETUP_Buff[] = "AT+QIOPEN=\"TCP\",\"101.69.242.138\",\"7001\"\r\n\0\0\0\0";//设置主站IP，可修改
INT8U App_AT_TCPCLOSE[]="AT+QICLOSE\r\n";

INT8U App_AT_QSCLK[] = "AT+QSCLK=1\r\n";

//INT8U App_AT_Back_RDY[] = "RDY";
//INT8U App_AT_Back_CallReady[] = "Call Ready";
//INT8U App_AT_Back_OK[]="OK";
//INT8U App_AT_Back_CONNECT[]="CONNECT";
//INT8U App_AT_Back_Ask[]="\r\n";				//主站应答

/* Private function prototypes -----------------------------------------------*/
__App_NeoM6310_Usart_Str App_NeoM6310_Usart_Str;	//GPRS所用串口缓冲区实例
__App_Gprs_Globe_Str	App_Gprs_Ctrl_Str;		//gprs 控制信息全局结构体
/* Private functions ---------------------------------------------------------*/








/******************************************************************************
*
*
*
*
*******************************************************************************/
void App_GprsCommInit(void)
{
	S_UARTx_Set UARTInit;						// 参数结构
	UARTInit.BaudRate = 115200;
	UARTInit.Parity   = BSPUART_PARITY_NO;
	UARTInit.StopBits = BSPUART_STOPBITS_1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8;
	UARTInit.RxBuf    = App_NeoM6310_Usart_Str.RxBuff;
	UARTInit.RxBufLen = APP_NEOM6310_RXBUFFLEN ;
	UARTInit.TxBuf    = App_NeoM6310_Usart_Str.TxBuff;
	UARTInit.TxBufLen = APP_NEOM6310_TXBUFFLEN ;
	UARTInit.Mode     = UART_DEFAULT_MODE;	// UART_DEFAULT_MODE
	BSP_UART_Init(APP_GPRS_COMM,&UARTInit);	
}

/******************************************************************************
*	flag = 0;	禁止上报
*	flag = 1;	允许上报
*	flag = 2;	开始上报
*
*******************************************************************************/

void App_Gprs_Set_WorkState(INT8U flag)
{
	App_Gprs_Ctrl_Str.WorkState = flag;
}
INT8U App_Gprs_Get_WorkState(void)
{
	return App_Gprs_Ctrl_Str.WorkState;
}

/**********************************************************************************************
* Description       :开机流程
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
static void App_Gprs_On_Off_Ctrl(void)
{	
	GPIO_PinModeSet(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN,gpioModePushPull,1);
	Delay(100);
//	GPIO_PinOutClear(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN);
	GPIO_PinOutSet(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN);
	//等待 >2S
	Delay(2000);
//	GPIO_PinOutSet(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN);
	GPIO_PinOutClear(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN);	
}
/**********************************************************************************************
* Description       :开机 BATT
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
//上电后，rst 等待1s 
void App_Gprs_Power_On(void)
{
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_GPRS ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_ON ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);								//打开模块电源	
	//>30ms
	Delay(100);	
	App_Gprs_On_Off_Ctrl();												//启动模块时序
}
/**********************************************************************************************
* Description       :关机
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
//关机前 最好先RST 等待2s 关闭电源
void App_Gprs_Power_Off(void)										
{
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_GPRS ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_OFF ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);							//关闭模块电源
}


void App_Gprs_Task_TimeCall(void)					//延时函数
{
	if(App_Gprs_Ctrl_Str.gDelayTime)
	{
		App_Gprs_Ctrl_Str.gDelayTime--;
	}
}

INT8U App_Gprs_GetCSQ(INT8U *dataBuff,INT8U lenth)
{
	INT8U signal = 0;				//信号强度
	INT8U level = 0;				//返回用，信号等级
	INT8U i;
	INT8U *pArray;
	pArray = dataBuff;	
	for(i=0; i<lenth; i++)
	{
		if(pArray[i] == ':')			//检测到‘:’
		{
			if((pArray[i+2]<='9')&&(pArray[i+2]>='0'))		//有个位
			{
				signal = (pArray[i+1] - '0')*10 + (pArray[i+2] - '0');
			}
			else
			{
				signal = (pArray[i+1] - '0');
			}
		}
	}
	if(signal > 28)
	{
		level = 4;
	}else if(signal > 22)
	{
		level = 3;
	}else if(signal > 16)
	{
		level = 2;
	}
	else if(signal > 10)
	{
		level = 1;
	}	
	else
	{
		level = 0;
	}
	return level;
}
/*****************************************************************************
//发送AT命令
//第一个参数：AT命令的字符串
//第二个参数：AT命令的长度
//第三个参数：AT命令返回值  
******************************************************************************/
 __App_State_Enum App_Gprs_AT_CMD(INT8U* pATString,INT16U FrameLen,char* pCmpString)			
{
	/*-变量-*/
	INT8U rxLen;
	INT8U strLen = strlen((char*)pCmpString);
	/*-状态-*/
	if(App_Gprs_Ctrl_Str.gState == APP_GPRS_STATE_START)	//第一次进来
	{
		App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_RESTART;
	}
	if(App_Gprs_Ctrl_Str.gState == APP_GPRS_STATE_RESTART)	//重连进来
	{
		BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//清空收发缓冲区
		BSP_UART_ClearTxBuff(APP_GPRS_COMM);
		App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		//延时时间等于最大值
		if(FrameLen > 0)			
		{
			BSP_UART_Write(APP_GPRS_COMM,pATString,FrameLen);				//发送命令
		}
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIME_WAITING;		//等待接收
	}
	if(App_Gprs_Ctrl_Str.gDelayTime == 0)	//延时时间到
	{
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIMEOUT;		//超时
	}
	rxLen = BSP_UART_IsRxFrame(APP_GPRS_COMM);
	if(rxLen)
	{
		if(strstr((char *)App_NeoM6310_Usart_Str.RxBuff,pCmpString) != NULL)		//正确的应答码
		{
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_RX_OK;				//接收正确
			return APP_GPRS_STATE_RX_OK;
		}
	}
	if(App_Gprs_Ctrl_Str.gState == APP_GPRS_STATE_TIMEOUT)	//超时了还没有接收到数据
	{
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_RESTART;				//重新开始
		App_Gprs_Ctrl_Str.gReCount--;									//重连计数++
	}
	if(App_Gprs_Ctrl_Str.gReCount == 0)	//重连次数大于设定值
	{
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_CONNET_FAIL;			//连接失败
		return APP_GPRS_STATE_CONNET_FAIL;
	}
	return App_Gprs_Ctrl_Str.gState;
}



void App_Gprs_SetConentIP(INT8U IP1,INT8U IP2,INT8U IP3,INT8U IP4,INT16U Port)//无APN
{
	__App_Para_InterfaceStr para_data;
	INT8U *p;
	INT8U lenth;
	INT8U IPBuff[28];
	
	
	/*把IP写入数组*/
	//INT8U App_AT_TCPSETUP_Buff[] = "AT+QIOPEN=\"TCP\",\"100.100.100.100\",\"1234\"\r\n\0\0\0\0";//设置主站IP，可修改
	p = App_AT_TCPSETUP_Buff;
	p += 17;
	lenth = sprintf((char*)p,"%d",IP1);		//第一段IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IP2);		//第二段IP
	p += lenth;
	*p++ = '.'; 
	lenth = sprintf((char*)p,"%d",IP3);		//第三段IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IP4);		//第四段IP
	p += lenth;
	*p++ = '"';
	*p++ = ',';
	*p++ = '"';			
	lenth = sprintf((char*)p,"%d",Port);			//端口号
	p += lenth;
	*p++ = '"';
	*p++ = '\r';
	*p++ = '\n';
	*p++ = '\0';
	/*把IP写入E2*/
	IPBuff[0] = IP1;			//IP1	
	IPBuff[1] = IP2;			//IP2
	IPBuff[2] = IP3;			//IP3
	IPBuff[3] = IP4;			//IP4
	IPBuff[4] = (INT8U)(Port);			//端口
	IPBuff[5] = (INT8U)(Port>>8);	
	para_data.afn = METER_WRITE;	
	para_data.IDIndex = 0x0402;
	para_data.point = 1;
	para_data.dbuff = IPBuff;			//指向IP地址
	para_data.dlen  = 28 ;
	GWPara_function(&para_data);							//把IP地址写入E2；


}
//获取主站IP 地址和端口号，放到给定的缓冲中，返回实际的字符长度,不能超过给定的缓冲长度
void App_Gprs_GetConentIP(void)
{
	__App_Para_InterfaceStr para_data;
	
	INT8U IPBuff[28];
	INT8U lenth;
	INT8U *p;
	INT16U port;

	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0402;
	para_data.point = 1;
	para_data.dbuff = IPBuff;				
	para_data.dlen  =  sizeof(IPBuff) ;
	GWPara_function(&para_data);							//把IP地址写入E2；
	
	p = App_AT_TCPSETUP_Buff;				//
	p += 17;
	lenth = sprintf((char*)p,"%d",IPBuff[0]);		//第一段IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IPBuff[1]);		//第二段IP
	p += lenth;
	*p++ = '.'; 
	lenth = sprintf((char*)p,"%d",IPBuff[2]);		//第三段IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IPBuff[3]);		//第四段IP
	p += lenth;
	*p++ = '"';
	*p++ = ',';
	*p++ = '"';
	port = IPBuff[4]+IPBuff[5]*256;					
	lenth = sprintf((char*)p,"%d",port);			//端口号
	p += lenth;
	*p++ = '"';
	*p++ = '\r';
	*p++ = '\n';
	*p++ = '\0';
	
//	Len = sizeof(App_AT_TCPSETUP_Buff);
//	memcpy(pData,App_AT_TCPSETUP_Buff,Len);

//	return Len;
}


/******************************************************************************
*
*
*
*
*******************************************************************************/
void App_Gprs_Receive(void)			//GPRS接收程序
{
	INT8U rxLen = 0;
	INT8U *pArray;
	pArray = App_NeoM6310_Usart_Str.RxBuff;
	rxLen = BSP_UART_IsRxFrame(APP_GPRS_COMM);			//接收帧的数据长度
	if(rxLen > 0)							//如果接收到数据
	{	
		App_NL_Receive(APP_GPRS_COMM,pArray,rxLen);
//		BSP_UART_ClearRxBuff(APP_GPRS_COMM);
	}
}
/******************************************************************************
*	模块第一次运行时运行该函数
*	1.使模块的波特率固定在115200
*	2.获取模块的信号强度
*
*******************************************************************************/
void App_Gprs_Repower(void)						//单片机重新上电
{
	while(1)
	{
		if(App_Gprs_Ctrl_Str.gSetup == APP_GPRS_SETUP_POWER_OFF)			//关机状态
		{		
			BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//清空收发缓冲区 
			BSP_UART_ClearTxBuff(APP_GPRS_COMM);
			App_GprsCommInit();									//初始化串口
			App_Gprs_Power_On();								//开机
			Delay(3000);
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_BAUDRATE;			//准备设置波特率
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_10S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		}	
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_BAUDRATE)			//设置波特率
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_10S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 3;
			switch(App_Gprs_AT_CMD(App_AT_IPR,strlen((char *)App_AT_IPR),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_CSQ;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_CLOSE_RET)
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_10S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 3;
			switch(App_Gprs_AT_CMD(App_AT_ATE_Buff,strlen((char *)App_AT_ATE_Buff),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_CSQ;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
//		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_CSQ)		//读取信号强度
//		{
//	//		INT8U App_AT_Buff[] = "AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n";		//设置APN的命令
//			switch(App_Gprs_AT_CMD(App_AT_CSQ_Buff,strlen((char *)App_AT_CSQ_Buff),"OK"))		
//			{			
//				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步
//					{
//						//接收信号强度
//	//					INT8U signal;
//	//					signal = App_Gprs_GetCSQ(App_NeoM6310_Usart_Str.RxBuff,25);
//	//					LCD_Display_Signal((_Bsp_LCD_Signal)signal);
//						App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
//						App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
//					}
//					break;
//				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
//					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
//					break;
//				default:
//					break;
//			}
//		}
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_POWER_CLOSE)		//关机
		{
			App_Gprs_Power_Off();
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_OFF;
			break;
		}
		Delay(500);
	}
}

void App_Gprs_Set(void)
{
	if(App_Gprs_Get_WorkState() == 0)		//禁止GPRS工作
	{	
		if(SysGetKeyIntFlag() == KEY_SHORT)		//如果短按
		{
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
			App_Gprs_Set_WorkState(1);		//使能GPRS工作
		}
	}
	if(App_Gprs_Get_WorkState()==1)	
	{
		if(App_Gprs_Ctrl_Str.gSetup == APP_GPRS_SETUP_POWER_OFF)			//关机状态
		{
			Bsp_Led_SetH((__Bsp_Enum_Led_Name)1);			//代表开始测试
			BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//清空收发缓冲区
			BSP_UART_ClearTxBuff(APP_GPRS_COMM);
			App_GprsCommInit();									//初始化串口
			App_Gprs_Power_On();								//开机
			Delay(3000);
			
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_CLOSE_RET;			//准备关闭回显
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 2;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_CLOSE_RET)			//关闭回显
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 2;
			switch(App_Gprs_AT_CMD(App_AT_ATE_Buff,strlen((char *)App_AT_ATE_Buff),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_BAUDRATE;		//设置波特率
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);		//测试失败
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_BAUDRATE)			//设置波特率
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 2;
			switch(App_Gprs_AT_CMD(App_AT_IPR,strlen((char *)App_AT_IPR),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_REPOWER;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);		//测试失败
					break;
				default:
					break;
			}
		}		
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_REPOWER)		//重启
		{
			App_Gprs_Power_Off();		//关机
			Delay(2000);				//等待2秒
			App_GprsCommInit();									//初始化串口
			App_Gprs_Power_On();								//开机
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIME_WAITING;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_ON;			//状态变成开机
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_15S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_POWER_ON)		//开机状态
		{	
			//--初始化操作				
			switch(App_Gprs_AT_CMD(NULL,0,"Call Ready"))			//收到call ready表示模块准备就绪可以直接上网
			{
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;			//下一步：设置透传模式
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)2);	//开机回码成功
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);		//测试失败
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_POWER_CLOSE)		//关机
		{
			App_Gprs_Power_Off();
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_OFF;
			App_Gprs_Ctrl_Str.WorkState = 0;
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);			//代表测试结束
		}
	}
}
void App_Gprs_Report(void)
{
	App_NL_Report();
}

void App_Gprs_Main(void)
{
	if(App_Gprs_Get_WorkState()==2)				//如果开始上报
	{
		if(App_Gprs_Ctrl_Str.gSetup == APP_GPRS_SETUP_POWER_OFF)			//关机状态
		{		
//			INT8U pData[40];
//			INT8U len;
			BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//清空收发缓冲区
			BSP_UART_ClearTxBuff(APP_GPRS_COMM);
			App_GprsCommInit();									//初始化串口
			App_Gprs_Power_On();								//开机
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIME_WAITING;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_ON;			//状态变成开机
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_15S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
			
 			App_Gprs_GetConentIP();
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_POWER_ON)		//开机状态
		{	
			//--初始化操作				
			switch(App_Gprs_AT_CMD(NULL,0,"Call Ready"))			//收到call ready表示模块准备就绪可以直接上网
			{
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步
	//				App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_QIMODE;			//下一步：设置透传模式
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_QSCLK;			//下一步：进入慢时钟配置
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_QSCLK)			//设置为慢时钟
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			switch(App_Gprs_AT_CMD(App_AT_QSCLK,strlen((char *)App_AT_QSCLK),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步,建立TCP/IP连接
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_QIMODE;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
	
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_QIMODE)			//设置为透传模式
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			switch(App_Gprs_AT_CMD(App_AT_QMODE,strlen((char *)App_AT_QMODE),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步,建立TCP/IP连接
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_TCPSETUP;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_TCPSETUP)		//建立TCP/IP连接
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_15S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			switch(App_Gprs_AT_CMD(App_AT_TCPSETUP_Buff,strlen((char *)App_AT_TCPSETUP_Buff),"CONNECT"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//接收正确，进入下一步
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_REPORT;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
				//发送一条数据帧
					BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//清空收发缓冲区
					BSP_UART_ClearTxBuff(APP_GPRS_COMM);
					App_Gprs_Ctrl_Str.MAX_ReCount = 1;
					App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
					App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_30S;
					App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;
					App_Gprs_Report();
					//--DTR拉高
					GPIO_PinModeSet(gpioPortD, 8, gpioModePushPull,1);
					GPIO_PinOutClear(gpioPortD,8);
				
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//接收错误，直接结束
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;        
			} 
		}    
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_REPORT)		//上报数据
		{
			//--上报数据和接收数据处理	
		//TODO:
			if(App_Gprs_Ctrl_Str.gDelayTime == 0)				//30秒没接收到数据
			{
				App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;
				if(App_Gprs_Ctrl_Str.gReCount > 0)
					App_Gprs_Ctrl_Str.gReCount--;
				
			}
			if(App_Gprs_Ctrl_Str.gReCount == 0)
			{
				App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
			}
			/*接收处理函数*/
			App_Gprs_Receive();			
		}
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_POWER_CLOSE)		//关机
		{
			App_Gprs_Power_Off();
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_OFF;
			App_Gprs_Set_WorkState(0);
		}

		
		
	}
}


/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

