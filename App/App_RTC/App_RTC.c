/***********************(C) COPY RIGHT 2015*************************
* File Name          App_RTC.c
* Author             pentral
* Date First Issued  2015.06.18
* Version            V1.00
* Description        �㲥Уʱ
*******************************************************************************
* History�� 
******************************************************************************/
/**************************************************************************************/

#include "App_Head.h " 
#include "App_Para_Interface.h" 
#include "bsp_rtc.h"
#include "convertDataInterface.h"


/* Private define-----------------------------------------------------------*/

/* Private typedef----------------------------------------------------------*/

/* Private typedef ---------------------------------------------------------*/ 


/* Private macro-------------------------------------------------------------*/


/* Private variables---------------------------------------------------------*/

/* Private function prototypes-----------------------------------------------*/

/* Private functions---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/ 







/******************************************************************************
* Description		:1s����һ�� 
* Input 			:none												
* Output			:none
* Contributor		:pentral
* Date First Issued :2014.11.24
******************************************************************************/
void App_RTC_Time_Second(void)
{
//	if(App_Route_IsNetWorking())
//	{
//		return ;
//	}
//	if(App_Rtc_Main_Str.WaitingTime)
//	{
//		App_Rtc_Main_Str.WaitingTime -- ;
//	}

}


///******************************************************************************
//* Description		:����������
//* Input 			:none												
//* Output			:none
//* Contributor		:pentral
//* Date First Issued :2015.06.18
//******************************************************************************/
//INT8U App_Rtc_TxAmountByte = 0  ;
//void App_RTC_Main(void)
//{
//	__App_RTC_MsgTpye tRmMsg ;
//	volatile _BSPRTC_TIME pTime ;
/////////////////////////////////////////���ַ�ʽ�ȽϺĵ� ���ʺϵ��/////////////////////////////////
//	if(!App_Rtc_Main_Str.Time_SetFlag)
//	{
//		if((!Bsp_Rtc_IsSetTime()))
//		{
//			return ;			
//		}
//		else
//		{
//			App_Rtc_Main_Str.AdjustTime_StartFlag = 1 ;				
//		}			
//	}
//	BSP_RTCGetTime(&pTime);
//	if((0x3==pTime.Hour)&&(0x35==pTime.Minute)&&(pTime.Second >0x56))
//	{
//		App_Rtc_Main_Str.AdjustTime_StartFlag = 1 ;
//	}
/////////////////////////////////////////���ַ�ʽ�ȽϺĵ�///////////////////////////////////////////	

//	//	/*�Ƿ�������״��*/
//	if(!App_RTC_Work_Can_Start())
//	{
//		return ;
//	}

//	if(App_Rtc_Main_Str.WaitingTime)
//	{
//		return ;
//	}
//	if(!App_Rtc_Main_Str.AdjustTime_StartFlag)
//	{
//		App_Rtc_Main_Str.Point = 0 ;
//		return ;
//	}
//	App_Rtc_Main_Str.Point++ ;		
//	
//	/*�����ʱ ���������״��*/
//	if(App_Rtc_Main_Str.Point > POINTAMOUNT)
//	{
//		App_Rtc_Main_Str.AdjustTime_StartFlag = 0 ;
//		App_Rtc_Main_Str.Point = 0 ;
//		App_Rtc_Main_Str.LastTpye = APP_RTC_PARA_NULL ;
//		return ;
//	}
//	tRmMsg = App_RTC_ParaGet(&App_Rtc_Main_Str);
////	if(APP_RTC_PARA_ISEND == tRmMsg)
////	{
////		App_Rtc_Main_Str.AdjustTime_StartFlag = 0 ;
////		App_Rtc_Main_Str.MapCount = 0 ;
////		App_Rtc_Main_Str.MapBit  = 0 ;
////		
////	}
//	if((APP_RTC_PARA_DIFF == tRmMsg)||(APP_RTC_PARA_MAP_FULL == tRmMsg))
//	{
//		
//		App_Rtc_Main_Str.WaitingTime = App_Rtc_Main_Str.MainRelayNum * 3 +3;			
//		/*��֡*/
//		App_Rtc_TxAmountByte = App_RTC_MakeFrame_RF(&App_Rtc_Main_Str,&pTime ,App_RTC_SendBuff,APP_RTC_BUFF_AMOUNT);
//		
//		/*���Ͷ�ʱ֡*/
//		if(App_Rtc_TxAmountByte)
//		{
//			if(	0xA0 == (App_RTC_SendBuff[1]&0xA0) )
//			{
//				LoRaSetStartSend(App_RTC_SendBuff,App_Rtc_TxAmountByte,RF_SEND_TYPE_MAST_TO_METER);										
//				
//			}
//			else
//			{
//				LoRaSetStartSend(App_RTC_SendBuff,App_Rtc_TxAmountByte,RF_SEND_TYPE_MAST_TO_COLL);										
//			}	
//			App_Rtc_Main_Str.MapCount = 0 ;
//			App_Rtc_Main_Str.MapBit  = 0 ;
//		}		
//		if(App_Rtc_Main_Str.Point)
//		{
//			App_Rtc_Main_Str.Point -- ;
//			memset(App_Rtc_Main_Str.MainRout,0,sizeof(App_Rtc_Main_Str.MainRout));
//		}			
//	}
//	App_Rtc_Main_Str.Time_SetFlag = 0x55;	
//	App_Rtc_TxAmountByte = 0 ;
//	if(APP_RTC_PARA_ISEND == tRmMsg)
//	{
//		App_Rtc_Main_Str.AdjustTime_StartFlag = 0 ;
//		App_Rtc_Main_Str.MapCount = 0 ;
//		App_Rtc_Main_Str.MapBit  = 0 ;
//		App_Rtc_Main_Str.LastTpye = APP_RTC_PARA_NULL ;
//		
//	}
//}



/************************(C)COPYRIGHT *****END OF FILE****************************/



