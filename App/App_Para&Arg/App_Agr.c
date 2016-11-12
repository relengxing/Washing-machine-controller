/*****************************************Copyright(C)******************************************
* FileName			: App_Agr.c	
* Author			: 
* Date First Issued	: 
* Version		: V1.0
* History		: 
* //2008		: V
* Description		: 
*-----------------------------------------------------------------------------------------------

***********************************************************************************************/
/* Includes-----------------------------------------------------------------------------------*/
#include "App_Agr.h"
#include "App_ParaTable.h" 
#include "App_Para_Interface.h"
#include "App_Data_Interface.h"
#include "convertDataInterface.h"
#include "bsp_power_ctrl.h"
#include "bsp_rtc.h"
#include "bsp_wdt.h"
//#include "Lora.h"
//#include "sx1276.h"
//#include "bsp_sx127x.h"
#include "App_Agr_MakeFrame.h"
//#include "App_Agr_RF.h"
#include "em_emu.h"
#include "Bsp_iic.h"
#include "App_485_Interface.h"
#include "sysTick.h"

/* Private define-----------------------------------------------------------------------------*/
//#define APP_AGR_BUFFAMOUNT	500 



/* Private typedef----------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables--------------------------------------------------------------------------*/
INT8U App_Agr_Buff_Rx[APP_AGR_RX_BUFFAMOUNT];
INT8U App_Agr_Buff_Tx[APP_AGR_TX_BUFFAMOUNT];
//INT8U App_Agr_GlobeBuff[APP_AGR_RX_BUFFAMOUNT];
__App_Frame_Str	App_Frame_Str;

//_RS232_COM_MONITOR_TYPE App_Debug_Agr_Com_Flag = RS232_COM_MONITOR_NULL ;
/* Private function prototypes----------------------------------------------------------------*/
void App_Para_Main(__App_Frame_Str *pFrame_Str);

/* Private functions--------------------------------------------------------------------------*/
/**********************************************************************************************
* Function          : agreememt_init_rs232

* Output            : none
***********************************************************************************************/
void agreememt_init_rs232(void)
{
    S_UARTx_Set UARTInit;						// 参数结构
	UARTInit.BaudRate = 2400;
	UARTInit.Parity   = BSPUART_PARITY_EVEN;
	UARTInit.StopBits = BSPUART_STOPBITS_1;
	UARTInit.DataBits = BSPUART_WORDLENGTH_8;
	UARTInit.RxBuf    = App_Agr_Buff_Rx;
	UARTInit.RxBufLen = APP_AGR_RX_BUFFAMOUNT ;
	UARTInit.TxBuf    = App_Agr_Buff_Tx;
	UARTInit.TxBufLen = APP_AGR_TX_BUFFAMOUNT ;
	UARTInit.Mode     = UART_DEFAULT_MODE;	//UART_HALFDUP_MODE

	BSP_UART_Init(APP_RS232_COMM,&UARTInit);	//GuiyueOSQ_pevent
}

///**********************************************************************************************
//* Function          : agreememt_init_IRDA

//* Output            : none
//***********************************************************************************************/
//void agreememt_init_IRDA(void)
//{
////    S_UARTx_Set UARTInit;						// 参数结构

////	UARTInit.BaudRate = 1200;
////	UARTInit.Parity   = BSPUART_PARITY_EVEN;
////	UARTInit.StopBits = BSPUART_STOPBITS_1;
////	UARTInit.DataBits = BSPUART_WORDLENGTH_8;
////	UARTInit.RxBuf    = TxFrameBuff_IRD;
////	UARTInit.RxBufLen = BYTE_NUM_LEN_128 ;
////	UARTInit.TxBuf    = TxFrameBuff_IRD;
////	UARTInit.TxBufLen = BYTE_NUM_LEN_128 ;
////	UARTInit.Mode     = UART_HALFDUP_MODE;	//UART_HALFDUP_MODE	  UART_DEFAULT_MODE
////	BSP_UART_Init(IRDA_COMM,&UARTInit,GuiyueOSQ_pevent);  //GuiyueOSQ_pevent
//}


//static void agreememt_init_485_back(void)
//{	
////	S_UARTx_Set UARTInit;						// 参数结构
////	__Bsp_Adc_PowerStr tBsp_Adc_PowerStr ;
////	UARTInit.BaudRate = 2400;
////	UARTInit.Parity   = BSPUART_PARITY_EVEN ;//usartEvenParity;
////	UARTInit.StopBits = BSPUART_STOPBITS_1  ;//usartStopbits1;
////	UARTInit.DataBits = BSPUART_WORDLENGTH_8 ;//usartDatabits8;
////	UARTInit.RxBuf    = App_Agr_Buff_Rx;
////	UARTInit.RxBufLen = APP_AGR_RX_BUFFAMOUNT ;
////	UARTInit.TxBuf    = App_Agr_Buff_Tx;
////	UARTInit.TxBufLen = APP_AGR_TX_BUFFAMOUNT ;
////	UARTInit.Mode     = UART_RS485_MODE;	// UART_DEFAULT_MODE
////	BSP_UART_Init(APP_485_COMM,&UARTInit);	
////	
////	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Cmp = BSP_485_ENUM ;
////	tBsp_Adc_PowerStr.sBsp_Enum_Adc_Switch = BSP_ADC_POWER_ON ;
////	Bsp_Power_Operate(&tBsp_Adc_PowerStr);

//}


/**********************************************************************************************
* Function          : Init_Agreememt

* Output            : none
***********************************************************************************************/
void App_Agr_Init(void)
{

	__App_Para_InterfaceStr pDataStrct;
	pDataStrct.afn = METER_READ;
	pDataStrct.IDIndex = 0X200;
	pDataStrct.dbuff = TerAddress;
	pDataStrct.dlen = sizeof(TerAddress);
	GWPara_function(&pDataStrct);	

	
//	Init_guiyuehanshu();
// 	agreememt_init_IRDA();   //与串口公用，默认为串口
	agreememt_init_rs232();
//	agreememt_init_485_back();
	
//    Init_Para_Data();
//	BSP_LEDWrite(BSPLED_NUM_WARN,BSPLED_STATE_CLOSE);  /*对告警灯状态初始化*/
//    Deal_Ver_Change();

//	/*此接口在底层操作有任务调用，所以必须放在所有参数初始化好后调用他*/
//	agreememt_init_wireless();

}
/*****************************************************************   

  ****************************************************************/
//INT8U crcBUff[6]={0x71 ,0 ,2,1,0,0};
//INT16U crc16XXXX = 0 ;

INT32U UpDataTimeCount =0  ;// = SYS_DELAY_2M/SYS_DELAY_500ms ;	   /*远程升级无通讯的时间间隔，*/



/**********************************************************************************************
函数名称:CheckAddress(INT8U ComPort, INT8U *p) 
功能说明：终端地址校验
输 入：ComPort--串口号
		*p    ___  指向接收报文中地址域的首地址
输 出：TRUE  ___  校验通过
       FALSE ___  校验失败
*************************************************************************************************/
__ADDRTPYE CheckAddress(const __App_Frame_Str *pFrame_Str) 
{  
	//用广播地址下发 
	INT8U broadAddr[APP_TER_ADDR_LEN];
	memset(broadAddr,0x99,sizeof(broadAddr));
	if(!memcmp(broadAddr,pFrame_Str->TerAddr,sizeof(broadAddr)))
	{
		if((MY_ORD_WRITE_BROAD_ADDR ==pFrame_Str->Cmd)||(MY_ORD_READ_BROAD_ADDR ==pFrame_Str->Cmd))
		{
			return GROUP_BROADCASE ;							
		}
		return NULL_ADDR ;

	}
	//单地址

	if(!memcmp(TerAddress,pFrame_Str->TerAddr,sizeof(pFrame_Str->TerAddr)))
	{
		return TERNIMAL_ADDR;		
	}
	return NULL_ADDR ;
}


void guiyue_dispose_main(__App_Frame_Str *pFrame_Str)
{
	if((pFrame_Str->Cmd < 0x18)||(0x1e==pFrame_Str->Cmd))
	{		
		 App_Para_Main(pFrame_Str);		
	}
//	switch(pFrame_Str->Cmd)
//    {

////    	case AFN_ACK_DATA_OK:          	//主站上报确认			
////	        pro_Ack_Data(ComPort);    																			
////	        break;

////    	case AFN_ACK_OK:          		//确认	
////	        pro_Ack(ComPort);    																			
////	        break;
////	   	case AFN_RESET:         		//复位命令
////			PRstCmd(ComPort);Cmd
////			break;  
//	    case AFN_SETPARA:         		//设置参数 
//	        App_Parameter_Main(pFrame_Str);
//	    break;
////	    case AFN_CONGTRAL:         		//控制命令  	 
//////	        CtrlCommand(ComPort);
////	        break;  
////	    case AFN_ID:        
////			APP_PassWordAFN(ComPort);		     	  
////	        break;  
////		case AFN_CONFIG:
////			ReadTerminalInform(ComPort);
////			break;       
//	    case AFN_INQUIREPARA:          	//查询参数
//	        App_Parameter_Read(pFrame_Str);
//		 break;  
//////		case AFN_INQUIRETASK :			//请求任务
//////			Aggrememt_RequstTask(ComPort);
//////			break;
////	    case AFN_INQUIREPARA_ONE:        //1类数据
////			ReadOneData(pFrame_Str);
////	        break;  
////	    case AFN_INQUIREPARA_TWO:        //2类数据
////			App_ERC_AckFunction(ComPort);
//////			Read_TwoTpye_Data(ComPort);
////	        break;
//////	    case AFN_INQUIREPARA_THREE:      //3类数据(soe)
//////	        ReadEventRecord(ComPort);
//////	        break;
////	    case AFN_TRANS_FILE:      		//文件传输
////			App_TaskGuiYue_SetUpDataTime();			
////			Updata_Main2( ComPort,FrameGW.DataBuffer,FrameGW.Len);

//////			UpDataTimeCount = SYS_DELAY_5M/SYS_DELAY_500ms;
//////			ShengjiFlag = 1;
////	        break;
//////	    case AFN_TRANS_DATA:      		//数据转发
//////            //RelayDataFromMasterSatation(ComPort);	
//////			CascadeFunction(ComPort);
//////			break;
//////	    case AFN_UPDATA:    			//远程升级
//////		    Updata_Main( ComPort,FrameGW.DataBuffer+1,FrameGW.Len-1);
//////			UpDataTimeCount = SYS_DELAY_5M/SYS_DELAY_500ms;
//////			ShengjiFlag = 1;
//////            break;
//	    default:        				//无效帧
//	        break;            
//	}
	
}



/**********************************************************************************************
* Function          : Parse_Rx_Frame
* Description       : 程序中的数据，是根据国网规约偏移的，具体的请参阅国网规约
* Input             : pMsg：参数说明参见bsp_conf.h文件的说明
* Output            : none
***********************************************************************************************/
INT32U Parse_Rx_Frame(_BSP_MESSAGE *pMsg)
{
    INT16U offset=0,datalen=0,len = 0;
    INT8U  *p;

    len = pMsg->DataLen ;	   /*此语句勿移动*/
//    UsingPort = pMsg->DivNum;
    if(pMsg->DataLen>APP_AGR_RX_BUFFAMOUNT)
	{
		return FALSE;	
	}
    p = pMsg->pData; 
    while((offset < len) &&((len - offset )  > 14 ))   
	{
		if(APP_AGR_START_CHAR == *p )				// 帧头
		{
			__ADDRTPYE addrTpye ;
			memcpy(&App_Frame_Str.StartChar,p,14);
			App_Frame_Str.pBuff = p+14 ;
			App_Frame_Str.CrcData = TwoByteToShort(p+14+App_Frame_Str.Len);
			App_Frame_Str.EndChar = *(p+14+App_Frame_Str.Len+2);
			
 			if(APP_AGR_END_CHAR == App_Frame_Str.EndChar)				   // 结束符0x16
			{
				INT16U framCrc = 0 ;
				//校验和检查 CRC16
				framCrc = Crc16_IBM(p+1,13+App_Frame_Str.Len+2);
				if(!framCrc)
				{
					App_Frame_Str.Comm              = pMsg->DivNum;
					/*地址域检查, */
//					App_Frame_Str.App_Frame_HeadStr = (__App_Frame_HeadStr*)p ;
//					App_Frame_Str.pRx               = p +sizeof(__App_Frame_HeadStr) ;
//					App_Frame_Str.LenRx             = App_Frame_Str.App_Frame_HeadStr->LenData -3;

//					App_Frame_Str.pStartTx          = App_Agr_Buff_Tx ;
//					App_Frame_Str.pTxData           = App_Agr_Buff_Tx +sizeof(__App_Frame_HeadStr) ;
//					App_Frame_Str.LenTx             = 0;
					
					addrTpye = CheckAddress(&App_Frame_Str) ;
				   	if(NULL_ADDR!=addrTpye) 
				    {
						guiyue_dispose_main(&App_Frame_Str);	
//						return TRUE;
	                }
					offset  += (datalen + 11);
					p+= offset ;
					continue ;
				}
			}	 
	    }
		offset++;
		p += offset ;
	}
	return FALSE;
}


//void App_Agr_Rf_Main(void )
//{
//	INT8U ret = 0;
//	INT16U datasize = 0;	
//	ret = OnMaster();
//	if(ret == 1)
//	{
//		INT8U *pRec = App_Agr_Buff_Rx ;
//		SX1276GetRxPacket(pRec,&datasize);		
//		App_Agr_RF_Process(pRec,datasize);
//	}
//}
#include "sx1276-lora.h"
//extern void SX1276LoRaSetRFState( uint8_t state );
//extern void SX1276LoRaSetOpMode( uint8_t opMode );

//规约任务包
void TaskGuiYue(void)
{   
//	__App_Task_State taskState ;
	INT16U len = 0 ;
	INT8U comPort = APP_RS232_COMM ;//APP_RS232_COMM ;
//	_BSPRTC_TIME tTime ;
//	INT8U ret = 0 ;
//	if(App_Agr_Uart_IsUing())
//	{
//		return ;
//	}
	len = BSP_UART_IsRxFrame(comPort);
	if(len)
	{
		_BSP_MESSAGE tMsg ;
		tMsg.DataLen = len ;
		tMsg.DivNum  = comPort ;
		tMsg.pData   = App_Agr_Buff_Rx ;
		Parse_Rx_Frame(&tMsg);		
		BSP_UART_ClearRxBuff(comPort);
	}
//	taskState = App_Agr_Rf_Main();	
//	/*串口通讯时间超过5s，关闭EEPROM POWER*/
//	
//	/*抄表任务时间超过30s，关闭Mbus POWER*/
//	if((APP_TASK_FREE==taskState)&&(TRUE!=App_Get_PowerOn_Rf_State()))
//	{

////		SX1276LoRaSetRFState( RFLR_STATE_IDLE );
////		SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );	

//		Bsp_I2CSPM_BasePower_Init(BSP_ADC_POWER_OFF);
////		App_Mbus_Power_Off_PinInit();
////		App_SX1276_Free_Config();
//		EMU_EnterEM2(true);		
//	}

}

///////////////////////////////////////////////

//////////////////////////外部对接协议标志////////////////////////////////
volatile INT16U App_Agr_TimeOutMs =0 ;
void App_Agr_Time_msCall(void)
{
	if(App_Agr_TimeOutMs)
	{
		App_Agr_TimeOutMs--;
	}
}	






typedef struct
{
	INT16U ExternID;								
	INT16U LocalID;
	INT8U  Cmd ;
}_Para_Mapping_Struct;

const _Para_Mapping_Struct Para_Mapping_Table[] = 
{
	
	0x10	,0x40D	,AFN_SETPARA,
	0x11	,0x40D	,AFN_INQUIREPARA,

	0x12	,0x409	,AFN_SETPARA,
	0x13	,0x409	,AFN_INQUIREPARA,

	0x14	,0x402	,AFN_SETPARA,
	0x15	,0x402	,AFN_INQUIREPARA,

	0x16	,0x401	,AFN_SETPARA,
	0x17	,0x401	,AFN_INQUIREPARA,

	0x1E	,0x406	,AFN_INQUIREPARA,
	
	
	MY_ORD_WRITE_BROAD_ADDR	,0x200	,AFN_SETPARA,
	MY_ORD_READ_BROAD_ADDR	,0x200	,AFN_INQUIREPARA,
	
	MY_ORD_WRITE_METER_PARA,0x406,AFN_SETPARA,
		
};
void App_Para_Main(__App_Frame_Str *pFrame_Str)
{
	INT8U amount = sizeof(Para_Mapping_Table)/sizeof(_Para_Mapping_Struct);
	INT8U i= 0 ;
	for(i = 0 ; i < amount ;i ++)
	{
		if(pFrame_Str->Cmd == Para_Mapping_Table[i].ExternID)
		{
			__App_Para_InterfaceStr tPdata ;
			INT16U reLen = 0 ;
			INT8U  meterAmount = 1 ;
			INT8U  *pTmp = pFrame_Str->pBuff ;
			INT8U  iAmount ;
			INT8U  j = 0 ;
			tPdata.afn = Para_Mapping_Table[i].Cmd ;
			tPdata.IDIndex = Para_Mapping_Table[i].LocalID ;
			if(0x406==tPdata.IDIndex)
			{
				if(AFN_SETPARA ==tPdata.afn)
				{
					//
					meterAmount = *pTmp; 
				}
				else
				{					
					meterAmount = POINTAMOUNT ;
				}
				pTmp++ ;					
			}
			iAmount = 0 ;
			tPdata.point = 0 ;
			for(j = 0 ; j <meterAmount; j++ )
			{
				INT8U tLen = 0 ;
				
				if(0x406==tPdata.IDIndex)
				{
					
					if((AFN_INQUIREPARA ==tPdata.afn))
					{
						tPdata.dbuff   = pTmp+iAmount* PARA_0406_LEN;
						tPdata.point ++;
					}
					else
					{
						tPdata.dbuff   = pTmp+j* PARA_0406_LEN;
						tPdata.point    = TwoByteBcdToHex(tPdata.dbuff+7);					
					}
				}
				else
				{
					tPdata.dbuff   = pTmp;
				
				}
				tPdata.dlen    = pFrame_Str->Len  ;
				tLen= GWPara_function(&tPdata);
				reLen +=tLen;
				if((0x406==tPdata.IDIndex)&&(AFN_INQUIREPARA ==tPdata.afn))
				{
					//data is unvalid end read para	OR 00
					INT8U rData ;
					rData = App_DataMatchDataIsSame(tPdata.dbuff,PARA_0406_LEN,0x00) ;
					if(!rData)
					{
						rData = App_DataMatchDataIsSame(tPdata.dbuff,PARA_0406_LEN,0xff) ;
					}
					if(rData)
					{
						reLen -=tLen;
						continue ;
					}
					iAmount++ ;
					if(iAmount>=APP_FREAM_MAXPOINTAMOUNT)
					{
						*pFrame_Str->pBuff = iAmount;
						reLen+=1 ;
						pFrame_Str->Len = iAmount* PARA_0406_LEN +1;
						App_Agr_TimeOutMs = 3000;
						App_Agr_MakeFrame(pFrame_Str);
						//init para 
						reLen = 0 ;
						iAmount = 0 ;
						pTmp = pFrame_Str->pBuff+1 ;
						DelayMs(App_Agr_TimeOutMs);
						//delay time 
						continue ;						
					}					
				}
				
			}
			if(reLen)
			{
				if(AFN_SETPARA ==tPdata.afn)
				{
					pFrame_Str->Len = 1 ;
					*pFrame_Str->pBuff = 0x00 ;
					*(pFrame_Str->pBuff+1) = 0x00 ;
					if(0x200==tPdata.IDIndex)
					{
						memcpy(pFrame_Str->TerAddr,TerAddress,sizeof(pFrame_Str->TerAddr));
					}
				}
				else if(AFN_INQUIREPARA ==tPdata.afn )
				{
					if(0x406==tPdata.IDIndex)
					{
						reLen+=1 ;
						*pFrame_Str->pBuff = iAmount;
					}
					pFrame_Str->Len = reLen ;
				}
				App_Agr_MakeFrame(pFrame_Str);
				return ;
			}
			
		}
	}
}

/************************(C)COPYRIGHT *****END OF FILE****************************/








