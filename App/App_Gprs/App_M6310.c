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
typedef struct	__App_NeoM6310_Usart_Str__		//GPRS���ô��ڻ���������
{

	INT8U RxBuff[APP_NEOM6310_RXBUFFLEN];
	INT16U RxLen ;
	INT8U TxBuff[APP_NEOM6310_TXBUFFLEN];
	INT16U TxLen ;
} __App_NeoM6310_Usart_Str;

typedef enum __App_State_Enum__
{	APP_GPRS_STATE_POWER_OFF,			//ģ��ϵ�
	APP_GPRS_STATE_POWER_ON,			//ģ���ϵ�
	APP_GPRS_STATE_START,				//��ʼ����
	APP_GPRS_STATE_RESTART,				//���·���
	APP_GPRS_STATE_TIME_WAITING,		//�ȴ�����
	APP_GPRS_STATE_RX_OK,				//������ȷ
	APP_GPRS_STATE_RX_ERROR,			//���մ���
	APP_GPRS_STATE_TIMEOUT,				//���ճ�ʱ
	APP_GPRS_STATE_CONNET_OK,			//���ӳɹ�
	APP_GPRS_STATE_CONNET_FAIL,			//����ʧ��
}__App_State_Enum;

typedef enum {	
	APP_GPRS_SETUP_POWER_OFF,			//�ػ�״̬

	APP_GPRS_SETUP_POWER_ON,			//����
	
	APP_GPRS_SETUP_BAUDRATE,			//���ò�����

	APP_GPRS_SETUP_CSQ,					//����ź�ǿ��

	APP_GPRS_SETUP_QIMODE,				//����͸��ģʽ

	APP_GPRS_SETUP_TCPSETUP,			//����TCP/IP����

	APP_GPRS_SETUP_REPORT,				//ͨѶ
	
	APP_GPRS_SETUP_DROP,				//�˳�����ģʽ

	APP_GPRS_SETUP_TCPCLOSE,			//�Ͽ�TCP/IP����

	APP_GPRS_SETUP_POWER_CLOSE,			//�ػ�
	
	APP_GPRS_SETUP_REPOWER,				//���¿���
	
	APP_GPRS_SETUP_CLOSE_RET,			//�رջ���
	
	APP_GPRS_SETUP_REP_DATA,			//�ϱ�������
	
	APP_GPRS_SETUP_QSCLK,				//˯��ģʽ
	
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
/*-AT���------------------------------------------------------------------*/
INT8U App_AT_IPR[]="AT+IPR=115200&W\r\n";
INT8U App_AT_CSQ_Buff[] = "AT+CSQ\r\n";
INT8U App_AT_ATE_Buff[] = "ATE0\r\n";
INT8U App_AT_QMODE[]="AT+QIMODE=1\r\n";
INT8U App_AT_TCPSETUP_Buff[] = "AT+QIOPEN=\"TCP\",\"100.100.100.100\",\"1234\"\r\n\0\0\0\0";//������վIP�����޸�
//INT8U App_AT_TCPSETUP_Buff[] = "AT+QIOPEN=\"TCP\",\"101.69.242.138\",\"7001\"\r\n\0\0\0\0";//������վIP�����޸�
INT8U App_AT_TCPCLOSE[]="AT+QICLOSE\r\n";

INT8U App_AT_QSCLK[] = "AT+QSCLK=1\r\n";

//INT8U App_AT_Back_RDY[] = "RDY";
//INT8U App_AT_Back_CallReady[] = "Call Ready";
//INT8U App_AT_Back_OK[]="OK";
//INT8U App_AT_Back_CONNECT[]="CONNECT";
//INT8U App_AT_Back_Ask[]="\r\n";				//��վӦ��

/* Private function prototypes -----------------------------------------------*/
__App_NeoM6310_Usart_Str App_NeoM6310_Usart_Str;	//GPRS���ô��ڻ�����ʵ��
__App_Gprs_Globe_Str	App_Gprs_Ctrl_Str;		//gprs ������Ϣȫ�ֽṹ��
/* Private functions ---------------------------------------------------------*/








/******************************************************************************
*
*
*
*
*******************************************************************************/
void App_GprsCommInit(void)
{
	S_UARTx_Set UARTInit;						// �����ṹ
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
*	flag = 0;	��ֹ�ϱ�
*	flag = 1;	�����ϱ�
*	flag = 2;	��ʼ�ϱ�
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
* Description       :��������
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
	//�ȴ� >2S
	Delay(2000);
//	GPIO_PinOutSet(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN);
	GPIO_PinOutClear(BSP_POWER_ONOFF_PORT,BSP_POWER_ONOFF_PIN);	
}
/**********************************************************************************************
* Description       :���� BATT
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
//�ϵ��rst �ȴ�1s 
void App_Gprs_Power_On(void)
{
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_GPRS ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_ON ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);								//��ģ���Դ	
	//>30ms
	Delay(100);	
	App_Gprs_On_Off_Ctrl();												//����ģ��ʱ��
}
/**********************************************************************************************
* Description       :�ػ�
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
//�ػ�ǰ �����RST �ȴ�2s �رյ�Դ
void App_Gprs_Power_Off(void)										
{
	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
	tBsp_Adc_PowerStr.sBsp_Enum_Power_Name = BSP_ENUM_POWE_GPRS ;
	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_OFF ;
	Bsp_Power_Operate(&tBsp_Adc_PowerStr);							//�ر�ģ���Դ
}


void App_Gprs_Task_TimeCall(void)					//��ʱ����
{
	if(App_Gprs_Ctrl_Str.gDelayTime)
	{
		App_Gprs_Ctrl_Str.gDelayTime--;
	}
}

INT8U App_Gprs_GetCSQ(INT8U *dataBuff,INT8U lenth)
{
	INT8U signal = 0;				//�ź�ǿ��
	INT8U level = 0;				//�����ã��źŵȼ�
	INT8U i;
	INT8U *pArray;
	pArray = dataBuff;	
	for(i=0; i<lenth; i++)
	{
		if(pArray[i] == ':')			//��⵽��:��
		{
			if((pArray[i+2]<='9')&&(pArray[i+2]>='0'))		//�и�λ
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
//����AT����
//��һ��������AT������ַ���
//�ڶ���������AT����ĳ���
//������������AT�����ֵ  
******************************************************************************/
 __App_State_Enum App_Gprs_AT_CMD(INT8U* pATString,INT16U FrameLen,char* pCmpString)			
{
	/*-����-*/
	INT8U rxLen;
	INT8U strLen = strlen((char*)pCmpString);
	/*-״̬-*/
	if(App_Gprs_Ctrl_Str.gState == APP_GPRS_STATE_START)	//��һ�ν���
	{
		App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_RESTART;
	}
	if(App_Gprs_Ctrl_Str.gState == APP_GPRS_STATE_RESTART)	//��������
	{
		BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//����շ�������
		BSP_UART_ClearTxBuff(APP_GPRS_COMM);
		App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		//��ʱʱ��������ֵ
		if(FrameLen > 0)			
		{
			BSP_UART_Write(APP_GPRS_COMM,pATString,FrameLen);				//��������
		}
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIME_WAITING;		//�ȴ�����
	}
	if(App_Gprs_Ctrl_Str.gDelayTime == 0)	//��ʱʱ�䵽
	{
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIMEOUT;		//��ʱ
	}
	rxLen = BSP_UART_IsRxFrame(APP_GPRS_COMM);
	if(rxLen)
	{
		if(strstr((char *)App_NeoM6310_Usart_Str.RxBuff,pCmpString) != NULL)		//��ȷ��Ӧ����
		{
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_RX_OK;				//������ȷ
			return APP_GPRS_STATE_RX_OK;
		}
	}
	if(App_Gprs_Ctrl_Str.gState == APP_GPRS_STATE_TIMEOUT)	//��ʱ�˻�û�н��յ�����
	{
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_RESTART;				//���¿�ʼ
		App_Gprs_Ctrl_Str.gReCount--;									//��������++
	}
	if(App_Gprs_Ctrl_Str.gReCount == 0)	//�������������趨ֵ
	{
		App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_CONNET_FAIL;			//����ʧ��
		return APP_GPRS_STATE_CONNET_FAIL;
	}
	return App_Gprs_Ctrl_Str.gState;
}



void App_Gprs_SetConentIP(INT8U IP1,INT8U IP2,INT8U IP3,INT8U IP4,INT16U Port)//��APN
{
	__App_Para_InterfaceStr para_data;
	INT8U *p;
	INT8U lenth;
	INT8U IPBuff[28];
	
	
	/*��IPд������*/
	//INT8U App_AT_TCPSETUP_Buff[] = "AT+QIOPEN=\"TCP\",\"100.100.100.100\",\"1234\"\r\n\0\0\0\0";//������վIP�����޸�
	p = App_AT_TCPSETUP_Buff;
	p += 17;
	lenth = sprintf((char*)p,"%d",IP1);		//��һ��IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IP2);		//�ڶ���IP
	p += lenth;
	*p++ = '.'; 
	lenth = sprintf((char*)p,"%d",IP3);		//������IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IP4);		//���Ķ�IP
	p += lenth;
	*p++ = '"';
	*p++ = ',';
	*p++ = '"';			
	lenth = sprintf((char*)p,"%d",Port);			//�˿ں�
	p += lenth;
	*p++ = '"';
	*p++ = '\r';
	*p++ = '\n';
	*p++ = '\0';
	/*��IPд��E2*/
	IPBuff[0] = IP1;			//IP1	
	IPBuff[1] = IP2;			//IP2
	IPBuff[2] = IP3;			//IP3
	IPBuff[3] = IP4;			//IP4
	IPBuff[4] = (INT8U)(Port);			//�˿�
	IPBuff[5] = (INT8U)(Port>>8);	
	para_data.afn = METER_WRITE;	
	para_data.IDIndex = 0x0402;
	para_data.point = 1;
	para_data.dbuff = IPBuff;			//ָ��IP��ַ
	para_data.dlen  = 28 ;
	GWPara_function(&para_data);							//��IP��ַд��E2��


}
//��ȡ��վIP ��ַ�Ͷ˿ںţ��ŵ������Ļ����У�����ʵ�ʵ��ַ�����,���ܳ��������Ļ��峤��
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
	GWPara_function(&para_data);							//��IP��ַд��E2��
	
	p = App_AT_TCPSETUP_Buff;				//
	p += 17;
	lenth = sprintf((char*)p,"%d",IPBuff[0]);		//��һ��IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IPBuff[1]);		//�ڶ���IP
	p += lenth;
	*p++ = '.'; 
	lenth = sprintf((char*)p,"%d",IPBuff[2]);		//������IP
	p += lenth;
	*p++ = '.';
	lenth = sprintf((char*)p,"%d",IPBuff[3]);		//���Ķ�IP
	p += lenth;
	*p++ = '"';
	*p++ = ',';
	*p++ = '"';
	port = IPBuff[4]+IPBuff[5]*256;					
	lenth = sprintf((char*)p,"%d",port);			//�˿ں�
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
void App_Gprs_Receive(void)			//GPRS���ճ���
{
	INT8U rxLen = 0;
	INT8U *pArray;
	pArray = App_NeoM6310_Usart_Str.RxBuff;
	rxLen = BSP_UART_IsRxFrame(APP_GPRS_COMM);			//����֡�����ݳ���
	if(rxLen > 0)							//������յ�����
	{	
		App_NL_Receive(APP_GPRS_COMM,pArray,rxLen);
//		BSP_UART_ClearRxBuff(APP_GPRS_COMM);
	}
}
/******************************************************************************
*	ģ���һ������ʱ���иú���
*	1.ʹģ��Ĳ����ʹ̶���115200
*	2.��ȡģ����ź�ǿ��
*
*******************************************************************************/
void App_Gprs_Repower(void)						//��Ƭ�������ϵ�
{
	while(1)
	{
		if(App_Gprs_Ctrl_Str.gSetup == APP_GPRS_SETUP_POWER_OFF)			//�ػ�״̬
		{		
			BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//����շ������� 
			BSP_UART_ClearTxBuff(APP_GPRS_COMM);
			App_GprsCommInit();									//��ʼ������
			App_Gprs_Power_On();								//����
			Delay(3000);
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_BAUDRATE;			//׼�����ò�����
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_10S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		}	
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_BAUDRATE)			//���ò�����
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_10S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 3;
			switch(App_Gprs_AT_CMD(App_AT_IPR,strlen((char *)App_AT_IPR),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_CSQ;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
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
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_CSQ;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
//		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_CSQ)		//��ȡ�ź�ǿ��
//		{
//	//		INT8U App_AT_Buff[] = "AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n";		//����APN������
//			switch(App_Gprs_AT_CMD(App_AT_CSQ_Buff,strlen((char *)App_AT_CSQ_Buff),"OK"))		
//			{			
//				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��
//					{
//						//�����ź�ǿ��
//	//					INT8U signal;
//	//					signal = App_Gprs_GetCSQ(App_NeoM6310_Usart_Str.RxBuff,25);
//	//					LCD_Display_Signal((_Bsp_LCD_Signal)signal);
//						App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
//						App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
//					}
//					break;
//				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
//					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
//					break;
//				default:
//					break;
//			}
//		}
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_POWER_CLOSE)		//�ػ�
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
	if(App_Gprs_Get_WorkState() == 0)		//��ֹGPRS����
	{	
		if(SysGetKeyIntFlag() == KEY_SHORT)		//����̰�
		{
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
			App_Gprs_Set_WorkState(1);		//ʹ��GPRS����
		}
	}
	if(App_Gprs_Get_WorkState()==1)	
	{
		if(App_Gprs_Ctrl_Str.gSetup == APP_GPRS_SETUP_POWER_OFF)			//�ػ�״̬
		{
			Bsp_Led_SetH((__Bsp_Enum_Led_Name)1);			//����ʼ����
			BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//����շ�������
			BSP_UART_ClearTxBuff(APP_GPRS_COMM);
			App_GprsCommInit();									//��ʼ������
			App_Gprs_Power_On();								//����
			Delay(3000);
			
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_CLOSE_RET;			//׼���رջ���
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 2;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_CLOSE_RET)			//�رջ���
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 2;
			switch(App_Gprs_AT_CMD(App_AT_ATE_Buff,strlen((char *)App_AT_ATE_Buff),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_BAUDRATE;		//���ò�����
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);		//����ʧ��
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_BAUDRATE)			//���ò�����
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 2;
			switch(App_Gprs_AT_CMD(App_AT_IPR,strlen((char *)App_AT_IPR),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��,
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_REPOWER;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);		//����ʧ��
					break;
				default:
					break;
			}
		}		
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_REPOWER)		//����
		{
			App_Gprs_Power_Off();		//�ػ�
			Delay(2000);				//�ȴ�2��
			App_GprsCommInit();									//��ʼ������
			App_Gprs_Power_On();								//����
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIME_WAITING;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_ON;			//״̬��ɿ���
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_15S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_POWER_ON)		//����״̬
		{	
			//--��ʼ������				
			switch(App_Gprs_AT_CMD(NULL,0,"Call Ready"))			//�յ�call ready��ʾģ��׼����������ֱ������
			{
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;			//��һ��������͸��ģʽ
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)2);	//��������ɹ�
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);		//����ʧ��
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_POWER_CLOSE)		//�ػ�
		{
			App_Gprs_Power_Off();
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_OFF;
			App_Gprs_Ctrl_Str.WorkState = 0;
			Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);			//������Խ���
		}
	}
}
void App_Gprs_Report(void)
{
	App_NL_Report();
}

void App_Gprs_Main(void)
{
	if(App_Gprs_Get_WorkState()==2)				//�����ʼ�ϱ�
	{
		if(App_Gprs_Ctrl_Str.gSetup == APP_GPRS_SETUP_POWER_OFF)			//�ػ�״̬
		{		
//			INT8U pData[40];
//			INT8U len;
			BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//����շ�������
			BSP_UART_ClearTxBuff(APP_GPRS_COMM);
			App_GprsCommInit();									//��ʼ������
			App_Gprs_Power_On();								//����
			App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_TIME_WAITING;
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_ON;			//״̬��ɿ���
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_15S;
			App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;		
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
			
 			App_Gprs_GetConentIP();
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_POWER_ON)		//����״̬
		{	
			//--��ʼ������				
			switch(App_Gprs_AT_CMD(NULL,0,"Call Ready"))			//�յ�call ready��ʾģ��׼����������ֱ������
			{
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��
	//				App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_QIMODE;			//��һ��������͸��ģʽ
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_QSCLK;			//��һ����������ʱ������
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_QSCLK)			//����Ϊ��ʱ��
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			switch(App_Gprs_AT_CMD(App_AT_QSCLK,strlen((char *)App_AT_QSCLK),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��,����TCP/IP����
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_QIMODE;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
	
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_QIMODE)			//����Ϊ͸��ģʽ
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_5S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			switch(App_Gprs_AT_CMD(App_AT_QMODE,strlen((char *)App_AT_QMODE),"OK"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��,����TCP/IP����
					App_Gprs_Ctrl_Str.gSetup =APP_GPRS_SETUP_TCPSETUP;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;
			}
		}
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_TCPSETUP)		//����TCP/IP����
		{
			App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_15S;
			App_Gprs_Ctrl_Str.MAX_ReCount = 1;
			switch(App_Gprs_AT_CMD(App_AT_TCPSETUP_Buff,strlen((char *)App_AT_TCPSETUP_Buff),"CONNECT"))		
			{			
				case APP_GPRS_STATE_RX_OK:				//������ȷ��������һ��
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_REPORT;
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
				//����һ������֡
					BSP_UART_ClearRxBuff(APP_GPRS_COMM);			//����շ�������
					BSP_UART_ClearTxBuff(APP_GPRS_COMM);
					App_Gprs_Ctrl_Str.MAX_ReCount = 1;
					App_Gprs_Ctrl_Str.gReCount = App_Gprs_Ctrl_Str.MAX_ReCount;
					App_Gprs_Ctrl_Str.MAX_DelayTime = APP_GPRS_DELAY_30S;
					App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;
					App_Gprs_Report();
					//--DTR����
					GPIO_PinModeSet(gpioPortD, 8, gpioModePushPull,1);
					GPIO_PinOutClear(gpioPortD,8);
				
					break;
				case APP_GPRS_STATE_CONNET_FAIL:		//���մ���ֱ�ӽ���
					App_Gprs_Ctrl_Str.gState = APP_GPRS_STATE_START;
					App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
					break;
				default:
					break;        
			} 
		}    
		if(App_Gprs_Ctrl_Str.gSetup ==APP_GPRS_SETUP_REPORT)		//�ϱ�����
		{
			//--�ϱ����ݺͽ������ݴ���	
		//TODO:
			if(App_Gprs_Ctrl_Str.gDelayTime == 0)				//30��û���յ�����
			{
				App_Gprs_Ctrl_Str.gDelayTime = App_Gprs_Ctrl_Str.MAX_DelayTime;
				if(App_Gprs_Ctrl_Str.gReCount > 0)
					App_Gprs_Ctrl_Str.gReCount--;
				
			}
			if(App_Gprs_Ctrl_Str.gReCount == 0)
			{
				App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_CLOSE;
			}
			/*���մ�����*/
			App_Gprs_Receive();			
		}
		if(App_Gprs_Ctrl_Str.gSetup==APP_GPRS_SETUP_POWER_CLOSE)		//�ػ�
		{
			App_Gprs_Power_Off();
			App_Gprs_Ctrl_Str.gSetup = APP_GPRS_SETUP_POWER_OFF;
			App_Gprs_Set_WorkState(0);
		}

		
		
	}
}


/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

