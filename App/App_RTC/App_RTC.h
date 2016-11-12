/***********************(C) COPY RIGHT 2015*************************
* File Name          App_RTC.c
* Author             pentral
* Date First Issued  2015.06.18
* Version            V1.00
* Description        广播校时
*******************************************************************************
* Historyú 
******************************************************************************/
/**************************************************************************************/

#include "Bsp_SX127X.h"
#include "sx1276.h"
#include "sx1276-LoRaMisc.h"
#include "LoRa.h"

#include "App_ReadData_Interface.h"
#include "convertDataInterface.h"
#include "App_Router_Interface.h"
#include "bsp_rtc.h"
#include "App_ParaDefine.h"
#include "App_ReadMeter_Interface.h"


/* Private define-------------------------------------------------------*/

/* Private typedef------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/ 


/* Private macro--------------------------------------------------------*/


/* Private variables----------------------------------------------------*/
typedef enum{APP_RTC_PARA_NULL = 0 ,APP_RTC_PARA_DIFF  ,APP_RTC_PARA_MAP_NOTFULL,APP_RTC_PARA_MAP_FULL	\
				,APP_RTC_PARA_SAME,APP_RTC_PARA_ISEND,APP_RTC_PARA_NOREG}__App_RTC_MsgTpye ;



#define APP_RTC_MAP_AMOUNT	16

__packed typedef struct __AppMeterStruct__
{
	INT8U  AdjustTime_StartFlag ;			//校时标志
	INT8U  Time_SetFlag  ;
	INT32U WaitingTime  ;					//UNIT SENCOND 发送数据完等待时间
	INT16U Point ;							//路由排列的序号
	INT8U  SouceAddr   ;		
	INT8U  MainRelayNum ;
	INT8U  MainRout[4];			//主路径
	INT16U MapBit ;
	INT8U  MapCount ;
	INT16U StartNo ;			//群抄的起始编号
	__App_RTC_MsgTpye  LastTpye ;			
	INT8U  Tpye ;
	INT16U MeterRegID ;		  //表计注册的ID号	
	INT8U  *pRbuff ;		  //接收帧缓冲
	INT8U  LenRbuff;          //长度 
}__App_Rtc_Main_Str ;

__App_Rtc_Main_Str App_Rtc_Main_Str ;

/* Private function prototypes------------------------------------------*/

/* Private functions----------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/ 





/******************************************************************************
* Description		:对抄表参数的处理
* Input 			:pMeterInform 参见 	__App_Rtc_Main_Str 											
* Output			:参见 __App_RTC_MsgTpye
* Contributor		:pentral
* Date First Issued :2014.11.24  此函数后续需要根据不同接口做修改，目前只是框架不能用
******************************************************************************/
__App_RTC_MsgTpye App_RTC_ParaGet(__App_Rtc_Main_Str *pMeterInform)
{
	STRUCT_NODE  tNode ;
//	INT8U  tmp ; 
	INT8U cmArry_Zero[4];
	memset(cmArry_Zero,0,sizeof(cmArry_Zero));

	if((pMeterInform->Point > POINTAMOUNT)||(!pMeterInform->Point))
	{
		return APP_RTC_PARA_ISEND;		
	}
	App_Route_GetWirelessMeterInfo(pMeterInform->Point,&tNode);
	if((!tNode.NOdo) ||(tNode.NOdo > POINTAMOUNT ))
	{
		if(pMeterInform->MapCount)
		{
			return APP_RTC_PARA_DIFF ;
		}
		return APP_RTC_PARA_ISEND ;
	}	
//	isReg = tNode.IsRegistration ;
	if(!tNode.IsRegistration)	
	{
		return APP_RTC_PARA_DIFF ;
	}
	//路由是否一样
	if(!memcmp(pMeterInform->MainRout,cmArry_Zero,sizeof(pMeterInform->MainRout)))
	{
		memcpy(pMeterInform->MainRout,tNode.MainRoad,sizeof(pMeterInform->MainRout));
		pMeterInform->MapCount = 0 ;		
		pMeterInform->MapBit = 0 ;				
	}
	if(memcmp(tNode.MainRoad,pMeterInform->MainRout,sizeof(pMeterInform->MainRout)))
	{
		return APP_RTC_PARA_DIFF;
	}

	pMeterInform->SouceAddr = App_Route_GetConcentratorNumber() ;

	if(APP_RTC_PARA_SAME!=pMeterInform->LastTpye)
	{
		pMeterInform->StartNo =  pMeterInform->Point ;
		pMeterInform->MapCount = 0 ;		
		pMeterInform->MapBit = 0 ;	
    }		
	pMeterInform->MapBit |=BIT(pMeterInform->MapCount);                                                                              	
	pMeterInform->MapCount++;
	if(pMeterInform->MapCount >= 16)
	{
		pMeterInform->LastTpye = APP_RTC_PARA_MAP_FULL ;
		return APP_RTC_PARA_MAP_FULL;			
	}
	pMeterInform->LastTpye = APP_RTC_PARA_SAME ;
	return APP_RTC_PARA_SAME ;
		
}



/******************************************************************************
* Description		:组抄表帧 
* Input 			:pMeterInform 参见 	__AppMeter_Inform 		
*					 pSendBuff 组网发送缓冲  Len：组网发送缓冲大小
* Output			:0xff 路径无效  其他为返回的级数 
* Contributor		:pentral
* Date First Issued :2014.11.24 
******************************************************************************/
INT8U App_RTC_RelayLeves(INT8U *pBuff,INT8U Len)
{
	INT8U i =0 ;
	for(i= 0 ; i < 4 ; i ++)
	{
		if((0xFF==*pBuff)||(!*pBuff))
		{
			return i ;
		}
		pBuff++;
	}
	return i ;
}
/******************************************************************************
* Description		:组对时帧
* Input 			:pMeterInform 参见 	__AppMeter_Inform 		
*					 pSendBuff 组网发送缓冲  Len：组网发送缓冲大小
* Output			:发送帧的长度
* Contributor		:pentral
* Date First Issued :2014.11.24 
******************************************************************************/
#define APP_RTC_BUFF_AMOUNT	60  
INT8U App_RTC_SendBuff[APP_RTC_BUFF_AMOUNT];

static INT8U App_RTC_MakeFrame_RF(__App_Rtc_Main_Str *pMeterInform,const volatile _BSPRTC_TIME *pRTC,INT8U *pSendBuff,INT16U Len)
{
	INT8U *pBuff ;
	INT8U tLen = 0 ;
	INT8U tCmp_FF[4] ;
	INT16U crc16 = 0 ;
	INT8U relayNum = 0;
	memset(tCmp_FF,0xff,sizeof(tCmp_FF));
	if(NULL==pSendBuff)
	{
		return 0 ;
	}
	pBuff = pSendBuff;
	*(pBuff+tLen) = 0x28 ;
	tLen+=1 ;
	relayNum = App_RTC_RelayLeves(pMeterInform->MainRout,4);
	pMeterInform->MainRelayNum =  relayNum ;
	//cmd
	if(0==relayNum)
	{
		*(pBuff+tLen) = APP_NL_RF_ORD_TIME_ADJST ;
		
	}
	else
	{
		*(pBuff+tLen) = APP_NL_RF_ORD_TIME_ADJST_RELY ;				
	}
	tLen++;
	//len
	*(pBuff+tLen) = 0 ;
	tLen++;	
	if(0!=relayNum)
	{
		//relay amount 
		*(pBuff+tLen) = relayNum | (relayNum<<3) ;
		tLen++;
		//souce ID
		*(pBuff+tLen) = pMeterInform->SouceAddr ;//*pMeterInform->MainRout ;
		tLen++;
		//relay addr & des ID  =D0
		memcpy(pBuff+tLen,pMeterInform->MainRout,relayNum);
		tLen+=relayNum;
	}
	else
	{
		//souce ID
		*(pBuff+tLen) = pMeterInform->SouceAddr ;//*pMeterInform->MainRout ;
		tLen++;
		// destionNode ID
		*(pBuff+tLen) = 0 ;
		tLen++;
	}	
	//Start number
	*(pBuff+tLen) = pMeterInform->StartNo ;
	tLen++;
	//mapbit
	memcpy(pBuff+tLen,(INT8U*)&pMeterInform->MapBit,sizeof(pMeterInform->MapBit));
	tLen+=sizeof(pMeterInform->MapBit) ;
	//rtc
	*(pBuff+tLen) = pRTC->Second;
	tLen+=1 ;		
	*(pBuff+tLen) = pRTC->Minute;
	tLen+=1 ;		
	*(pBuff+tLen) = pRTC->Hour;
	tLen+=1 ;		
	*(pBuff+tLen) = pRTC->Day;
	tLen+=1 ;		
	*(pBuff+tLen) = pRTC->Month;
	tLen+=1 ;		
	*(pBuff+tLen) = pRTC->Year;
	tLen+=1 ;		
	
	//timeOut 
	*(pBuff+tLen) = pMeterInform->WaitingTime;
	tLen+=1 ;		
	//renew cal len
	*(pBuff+2) = tLen+2 ;	//add cal len
	
	crc16 = Crc16_IBM(pBuff,tLen);
	memcpy(pBuff+tLen,(INT8U*)&crc16,sizeof(crc16));
	tLen+=2 ;
	return tLen ;
}

/******************************************************************************
* Description		:1s调用一次 
* Input 			:none												
* Output			:none
* Contributor		:pentral
* Date First Issued :2014.11.24
******************************************************************************/
void App_RTC_Time_Second(void)
{
	if(App_Route_IsNetWorking())
	{
		return ;
	}
	if(App_Rtc_Main_Str.WaitingTime)
	{
		App_Rtc_Main_Str.WaitingTime -- ;
	}

}

/******************************************************************************
* Description		:判断是否可以开启本任务 
* Input 			:none												
* Output			:1---暂无其它任务在工作，本任务可以开启工作，0，不行
* Contributor		:pentral
* Date First Issued :2014.11.24
******************************************************************************/
static INT8U App_RTC_Work_Can_Start(void)
{
	if(App_Route_IsNetWorking())
	{
		return 0 ;
	}
	if(App_Rm_State())
	{
		return 0 ;
	}
	return 1 ;
}

/******************************************************************************
* Description		:抄表主任务
* Input 			:none												
* Output			:none
* Contributor		:pentral
* Date First Issued :2015.06.18
******************************************************************************/
INT8U App_Rtc_TxAmountByte = 0  ;
void App_RTC_Main(void)
{
	__App_RTC_MsgTpye tRmMsg ;
	volatile _BSPRTC_TIME pTime ;
///////////////////////////////////////这种方式比较耗电 不适合电池/////////////////////////////////
	if(!App_Rtc_Main_Str.Time_SetFlag)
	{
		if((!Bsp_Rtc_IsSetTime()))
		{
			return ;			
		}
		else
		{
			App_Rtc_Main_Str.AdjustTime_StartFlag = 1 ;				
		}			
	}
	BSP_RTCGetTime(&pTime);
	if((0x3==pTime.Hour)&&(0x35==pTime.Minute)&&(pTime.Second >0x56))
	{
		App_Rtc_Main_Str.AdjustTime_StartFlag = 1 ;
	}
///////////////////////////////////////这种方式比较耗电///////////////////////////////////////////	

	//	/*是否处于组网状怿*/
	if(!App_RTC_Work_Can_Start())
	{
		return ;
	}

	if(App_Rtc_Main_Str.WaitingTime)
	{
		return ;
	}
	if(!App_Rtc_Main_Str.AdjustTime_StartFlag)
	{
		App_Rtc_Main_Str.Point = 0 ;
		return ;
	}
	App_Rtc_Main_Str.Point++ ;		
	
	/*如果超时 进入读参数状怿*/
	if(App_Rtc_Main_Str.Point > POINTAMOUNT)
	{
		App_Rtc_Main_Str.AdjustTime_StartFlag = 0 ;
		App_Rtc_Main_Str.Point = 0 ;
		App_Rtc_Main_Str.LastTpye = APP_RTC_PARA_NULL ;
		return ;
	}
	tRmMsg = App_RTC_ParaGet(&App_Rtc_Main_Str);
//	if(APP_RTC_PARA_ISEND == tRmMsg)
//	{
//		App_Rtc_Main_Str.AdjustTime_StartFlag = 0 ;
//		App_Rtc_Main_Str.MapCount = 0 ;
//		App_Rtc_Main_Str.MapBit  = 0 ;
//		
//	}
	if((APP_RTC_PARA_DIFF == tRmMsg)||(APP_RTC_PARA_MAP_FULL == tRmMsg))
	{
		
		App_Rtc_Main_Str.WaitingTime = App_Rtc_Main_Str.MainRelayNum * 3 +3;			
		/*组帧*/
		App_Rtc_TxAmountByte = App_RTC_MakeFrame_RF(&App_Rtc_Main_Str,&pTime ,App_RTC_SendBuff,APP_RTC_BUFF_AMOUNT);
		
		/*发送对时帧*/
		if(App_Rtc_TxAmountByte)
		{
			if(	0xA0 == (App_RTC_SendBuff[1]&0xA0) )
			{
				LoRaSetStartSend(App_RTC_SendBuff,App_Rtc_TxAmountByte,RF_SEND_TYPE_MAST_TO_METER);										
				
			}
			else
			{
				LoRaSetStartSend(App_RTC_SendBuff,App_Rtc_TxAmountByte,RF_SEND_TYPE_MAST_TO_COLL);										
			}	
			App_Rtc_Main_Str.MapCount = 0 ;
			App_Rtc_Main_Str.MapBit  = 0 ;
		}		
		if(App_Rtc_Main_Str.Point)
		{
			App_Rtc_Main_Str.Point -- ;
			memset(App_Rtc_Main_Str.MainRout,0,sizeof(App_Rtc_Main_Str.MainRout));
		}			
	}
	App_Rtc_Main_Str.Time_SetFlag = 0x55;	
	App_Rtc_TxAmountByte = 0 ;
	if(APP_RTC_PARA_ISEND == tRmMsg)
	{
		App_Rtc_Main_Str.AdjustTime_StartFlag = 0 ;
		App_Rtc_Main_Str.MapCount = 0 ;
		App_Rtc_Main_Str.MapBit  = 0 ;
		App_Rtc_Main_Str.LastTpye = APP_RTC_PARA_NULL ;
		
	}
}



/************************(C)COPYRIGHT *****END OF FILE****************************/



