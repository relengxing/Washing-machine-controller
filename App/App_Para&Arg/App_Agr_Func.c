/*****************************************Copyright(C)******************************************
* FileName			: App_Agr_Func.c	
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
#include "App_Para_Interface.h"
#include "convertDataInterface.h"
#include "App_Agr_MakeFrame.h"
#include "App_Data_interface.h " 
//#include "App_PlcInterface.h"
/* Private define-----------------------------------------------------------------------------*/

/* Private typedef----------------------------------------------------------------------------*/

/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/


/**********************************************************************************************
* Description       : 初始化规约的各个缓冲区参数,在初始化的时候必须放在第一个位置
* Input             : PoinIndex:测量点号 》=1
* Output            : none
***********************************************************************************************/
INT8U App_AgrIsValid(INT16U PoinIndex)
{
	if(PoinIndex&&(PoinIndex<=POINTAMOUNT))
	{
		__App_Para_InterfaceStr  dataStuct;
		INT8U  paraBuff[50];
//		INT16U dataID_S = 0x406;

		dataStuct.afn = METER_READ;  
		dataStuct.IDIndex = 0x406 ;
		dataStuct.point  = 	PoinIndex ;	
		dataStuct.dlen  = sizeof(paraBuff); 
		dataStuct.dbuff = paraBuff ;		
		GWPara_function(&dataStuct);
		if(dataStuct.dbuff[1]&&(dataStuct.dbuff[1]<10))
		{
			return TRUE ;
		}
	}
	return	FALSE ;		
}



///**********************************************************************************************
//* Function          : 规约中的写参数功能
//* Output            : none
//***********************************************************************************************/
//void App_Parameter_Write(__App_Frame_Str *pFrame_Str)
//{
//	INT16U  pointAmount ,pointIndex; 
//	INT8U  *pRx;
//	INT16U dataID = 0 ;
//	INT16U len = 0 ;
//    INT32U offset_frame = 0 ;/*多帧的时候，地址的偏移位置 */
//	__App_Para_InterfaceStr  parestuct;
//	INT8U  arrayPw[10] ;
//	INT8U  arrayCmpPW[9] = {0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
//	INT8U  tpyePw ;
//	INT16U countIndex ; 
//	

//	tpyePw = pFrame_Str->App_Frame_HeadStr->CMD&BIT(5);			//09.05	 规约需要有修改 C
//	tpyePw = tpyePw ? 1: 0 ;
////	/*终端密钥取参数区的 解密帧数据 FrameGW.Len*/
////	parestuct.afn = METER_READ; 
////	parestuct.IDIndex =0x0405;
////	parestuct.point = 0;
////	parestuct.dlen = 10;
////	parestuct.pSource = &tpyePw ;		  
////	parestuct.dbuff =arrayPw;
////	GWPara_function(&parestuct);
////	if(!memcmp(arrayCmpPW+1,arrayPw+1,8))
////	{
////		memset(arrayPw,0x88,10);
////	}
//	memset(arrayPw,0x88,10);
//	/*获取组数*/
//	pRx = pFrame_Str->pRx  ;				// 指向数据域

//	pFrame_Str->App_Frame_HeadStr->Seq ^= arrayPw[1];
//	pFrame_Str->App_Frame_HeadStr->Dt[0] ^= arrayPw[2];
//	pFrame_Str->App_Frame_HeadStr->Dt[1] ^= arrayPw[3];
//	
//	for(countIndex=0 ;countIndex < pFrame_Str->LenRx ; countIndex ++) //去掉了 seq dt[2];
//	{
//		*pRx ^=arrayPw[1 + countIndex%8] ;	//除去版本号
//		pRx ++ ;		
//	}
//		
//	/*计算其实际长度*/
//	pRx = pFrame_Str->pRx  ;				// 指向数据域
////	pRx = &pFrame_Str->App_Frame_HeadStr->Seq ;				// 解密前动过了指针位置，需重新赋值
//	/*查找最后一个0x80 */
//	len = 0 ;
//	for( countIndex = 0  ;countIndex < pFrame_Str->LenRx ; countIndex++ )
//	{
//		if(0x80 == *pRx )		/*记录其最后一个位置*/
//		{
//			len = countIndex ;	
//		}
//		pRx++ ;
//	}
//	if(!len)
//	{
//		return ;
//	}
//	/*其他是否 00*/
//	if(pFrame_Str->LenRx > (1+len))
//	{
//		INT8U tmpLen = pFrame_Str->LenRx-1-len;
//		memset(arrayCmpPW,0,9);
//		if(memcmp(pFrame_Str->pRx + len+1,arrayCmpPW+4,tmpLen))
//		{
//			pFrame_Str->App_Frame_HeadStr->CMD = 0xC4 ; 		
//			App_Agr_MakeFrame(pFrame_Str);
//		    return; 
//		}
//	
//	}
//	pFrame_Str->LenRx = countIndex ;							//实际长度

//	/*解密完成*/
//	pRx = pFrame_Str->pRx  ;									// 测量点数量位置

//	dataID = TwoByteToShort(pFrame_Str->App_Frame_HeadStr->Dt);
//	pointAmount = *(pRx+0) ;

//	pRx+= 1 ;					  							   /*指向第一个测量点号的位置*/
//	parestuct.afn = AFN_SETPARA;  
//	parestuct.IDIndex = dataID;

//	offset_frame = 0 ;
//	pointIndex = 0 ;
//	parestuct.pSource = pFrame_Str->pRx+1 ;
//    while((pFrame_Str->LenRx> offset_frame )&&(pointAmount>pointIndex))
//    {
//		pointIndex++ ;
//	    //测量点号 
//		parestuct.point = *(pRx+1)<<8;
//	    parestuct.point += *pRx;

//		parestuct.dbuff =(pRx +2);

//		len = GWPara_function(&parestuct);
//		if(!len)	 	/*有非法项不做处理*/
//		{
//			break;	
//		} 			                    
//		pRx+=len+2 ;	/*数据内容+测量点号*/
//		offset_frame += len+2 ;
//		pointIndex ++ ;
//    }//while end  

//    /*确认的消息类型 */ 
//	if(len)
//	{
//		pFrame_Str->App_Frame_HeadStr->CMD = 0x84 ; 
//	}
//	else
//	{
//		pFrame_Str->App_Frame_HeadStr->CMD = 0xC4 ; 		
//	}
//	App_Agr_MakeFrame(pFrame_Str);
//}


///************************************************************************
// ** 函数名称：void App_Parameter_Read
// ** 功能描述：参数查询处理
// ** 输    出：none
//*************************************************************************/
//void App_Parameter_Read(__App_Frame_Str *pFrame_Str)
//{
//	__App_Para_InterfaceStr  parestuct;
//	INT16U pointIndex,pointAmount ;
//	INT8U  *pSendBuff;
//	INT8U  *pRx;
//	INT8U  frameTpye = 0 ;
//	INT16U dataID = 0 ;
//	INT16U framMaxLen = 0 ; 
//	INT16U len = 0;

//	pRx = pFrame_Str->pRx ;
///*接收帧 相关参数初始化*/
//	pointAmount = *(pRx) ;					//数量
//	pRx+= 1 ;					  			/*指向第一个测量点号的位置*/

///*回复帧 相关参数初始化*/
//    framMaxLen= APP_AGR_TX_BUFFAMOUNT-20 ;	//每帧的最大字节数  APP_AGR_BUFFAMOUNT


///*本函数相关标志位，初始化*/
//	parestuct.afn = AFN_INQUIREPARA;  
//	dataID = TwoByteToShort(pFrame_Str->App_Frame_HeadStr->Dt);
//	parestuct.IDIndex = dataID;
//	pointIndex = 0 ;
//	pSendBuff = pFrame_Str->pTxData+1 ;		//测量点数量
//    pFrame_Str->LenTx =1;				   //预留 SEQ DI TA
//	while(pointIndex < pointAmount)
//    {
//		INT8U  buff[50];
//		pointIndex++ ;
//		/*获取测量点号*/
//		parestuct.point = *(pRx+1)	<< 8 ;
//		parestuct.point |= *(pRx+0);
//		if(parestuct.point > POINTAMOUNT)
//		{
//			pFrame_Str->App_Frame_HeadStr->CMD |= 0xc0 ;
//			break ;
//		}
//		parestuct.dbuff = buff ;
//		/*读取数据*/
//		len = GWPara_function(&parestuct);
//		if(!len)
//		{
//			pFrame_Str->App_Frame_HeadStr->CMD |= 0xc0 ;			
//			break;
//		}
//		if(len + pFrame_Str->LenTx > framMaxLen)
//		{
//			*pFrame_Str->pTxData = pointIndex ;	//数量
//			/*发送一个帧*/
//			frameTpye++;
//			frameTpye = frameTpye?2:1;  /*0单帧 1起始帧 2中间帧 3结束帧*/
//			pFrame_Str->App_Frame_HeadStr->Seq &=0x9f;
//			pFrame_Str->App_Frame_HeadStr->Seq|=(frameTpye<<5) ;
//			App_Agr_MakeFrame(pFrame_Str);
//			pFrame_Str->App_Frame_HeadStr->Seq ++ ;
//			pSendBuff = pFrame_Str->pTxData ;
//			pFrame_Str->LenTx = 0 ;
//			continue ;
//		}
//		else if(len)  /*没有超过一帧长度*/
//		{
//			pointIndex++;
//			*pSendBuff++ = *(pRx+0) ;	//测量点
//			*pSendBuff++ = *(pRx+1) ;
//			memcpy(pSendBuff,buff,len);	//数据内容
//			pSendBuff+=len ;
//			pFrame_Str->LenTx += (len + 2) ;
//			
//			pRx+=2 ;			/*偏移下发帧数据位置*/
//		}
//    } 
//	if(len)
//	{
//		*pFrame_Str->pTxData = pointIndex ;	//数量		
//		
//	}
//	pFrame_Str->App_Frame_HeadStr->Seq &=0x9f;
//	if(!frameTpye)
//	{
//		frameTpye =3 ;
//	}
//	else
//	{
//		frameTpye =1 ;
//	}
//	pFrame_Str->App_Frame_HeadStr->Seq|=(frameTpye<<5) ;	
//	App_Agr_MakeFrame(pFrame_Str);	
//}




INT8U DataReadMeterFlag = 0 ;
//一类数据读取				  
void App_ReadOneData(__App_Frame_Str *pFrame_Str)			
{
//    __App_Para_InterfaceStr  dataStuct;
//	INT16U pointIndex,pointAmount ,pointAountFrame ;  
//	INT8U  *pSendBuff;
//	INT8U  *pRx;
//	INT8U  frameTpye = 0 ;
//	INT16U dataID_S = 0 ;
//	INT16U framMaxLen = 0 ; 
////	INT8U  eventAmount = 0 ;
////	INT8U  eventStart = 0 ;
////	INT8U  buff[100];
//	INT8U  pointAllFlag = 0 ;
//	INT16U len = 0;	
//	

//	/*回复帧 相关参数初始化*/
//    framMaxLen= APP_AGR_TX_BUFFAMOUNT-20 ;	//每帧的最大字节数  APP_AGR_BUFFAMOUNT


//	/*本函数相关标志位，初始化*/
//	dataStuct.afn = AFN_INQUIREPARA;  
//	dataID_S = TwoByteToShort(pFrame_Str->App_Frame_HeadStr->Dt);
//	dataStuct.IDIndex = dataID_S;
//	pointIndex = 0 ;
//	pSendBuff = pFrame_Str->pTxData ;

//	pRx = pFrame_Str->pRx ;
//	/*接收帧 相关参数初始化*/
//	pointAmount = *(pRx) ;					//数量
//	pRx+= 1 ;					  			/*指向第一个测量点号的位置*/

//	/*本函数相关标志位，初始化*/
//	pointIndex = 0 ;
//    frameTpye = 0 ;
//	pointAountFrame = 0 ;
//	
//	dataStuct.afn = AFN_INQUIREPARA_ONE;  
//	dataStuct.IDIndex = dataID_S;
//	
//	dataStuct.pSource = pFrame_Str->pRx+1 ;
//	pSendBuff = pFrame_Str->pTxData ; 			//回复主站的数据区
//    pFrame_Str->LenTx =1;				   //预留 SEQ DI TA
//	while(pointIndex < pointAmount)
//    {
//		INT8U tmpArray[75];
////		INT8U  readMeterFlag = 0 ;
//		pointIndex ++ ;
//		/*获取测量点号*/
//				
//		if((0xff == *(pRx+0))&&(0xff == *(pRx+1)))
//		{
//			pointAllFlag =1 ;
//			pointAmount = POINTAMOUNT ;
//		}
//		if(pointAllFlag)
//		{
//			dataStuct.point = pointIndex ;

//			*(pSendBuff++) = pointIndex&0xff ; 
//			*(pSendBuff++) = (pointIndex>>8)&0xff ; 
//		}
//		else
//		{
//			dataStuct.point = ((INT16U)*(pRx+1))<< 8 ;
//			dataStuct.point |= *(pRx+0)	 ;


//			*(pSendBuff++) = *(pRx+0) ; 
//			*(pSendBuff++) = *(pRx+1) ; 			
//		}
//		if(dataStuct.point > POINTAMOUNT)
//		{
//			pFrame_Str->App_Frame_HeadStr->CMD |= 0xc0 ;
//			break ;
//		}
//		if(!App_AgrIsValid(dataStuct.point)&&((0x1C01==dataID_S)||(0xC01==dataID_S)||(0xC02==dataID_S)||(0xC0A==dataID_S)))
//		{
//			pSendBuff-=2 ;
//			len = 0 ;
//			//协议无效
//			if(pointAllFlag)
//			{
////				if(!(pointIndex%30))
////				{
////					WDG_clr_flag = 1;
////				}
//			}
//			else if((0xC02 ==dataID_S)||(0xC04 ==dataID_S))
//			{
//				pRx+=5 ;			/*px + 时标*/
//				dataStuct.pSource += 5 ;
//			}
//			else if(0xC0A ==dataID_S)
//			{
//				pRx += 6 ;			/*px + 时标*/
//				dataStuct.pSource += 6 ;
//			}
//			else
//			{
//				pRx+=2 ;
//			}
//			continue ;
//		}
//		dataStuct.dlen  = sizeof(tmpArray); 
//		dataStuct.dbuff = tmpArray ;
////		if(dataID_S == 0xC01)	  //点抄功能
////		{
////			INT16U dataID ;
////			INT16U timeCount = 0 ;
////			INT16U timeLimit = 	SYS_DELAY_1M/SYS_DELAY_3s;
////			/*发送点抄消息*/
////			dataID = 0x1F90; //数据项参照水表CJ188
////			DataReadMeterFlag = 1 ;
////			if(!App_SendRMeterMsg(dataStuct.point,(INT8U*)&dataID,2))
////			{
////				DataReadMeterFlag = 0 ;	
////				readMeterFlag =1 ;
////			}
////			while(DataReadMeterFlag)
////			{
////				timeCount++;
////				OSTimeDly(SYS_DELAY_3s);
////				if(timeCount > (timeLimit))
////				{
////					break ;
////				}
////			}
////			App_ClearRMeterMsg();
////			DataReadMeterFlag = 0 ;
////		}
//		/*读取数据*/
//		len = GWFirstData_function(&dataStuct);
//		/*是否有非法ID 或者长度不够*/
//		if(!len)
//		{
//			if(pointAllFlag)
//			{
//				pSendBuff-=2 ;
////				if(!(pointIndex%30))
////				{
//////					OSTimeDly(SYS_DELAY_10ms);
////					WDG_clr_flag = 1;
////				}
//				continue ;
//			}
//			else if((0xC02 ==dataID_S)||(0xC04 ==dataID_S))
//			{
//				pSendBuff-=2 ;
//				pRx+=5 ;			/*时标*/
//				dataStuct.pSource += 5 ;
//				continue ;
//			}
//			else if(0xC0A ==dataID_S)
//			{
//				pSendBuff-=2 ;
//				pRx+=6 ;			/*时标*/
//				dataStuct.pSource += 6 ;
//				continue ;
//			}
//			else
//			{
//				break ;				
//			}
//		}
//		pFrame_Str->LenTx +=2 ;  //point
////		if(dataID_S == 0xC01)	  //点抄功能
////		{
////			__App_ReadMeterInform *lcdReadMeterInform ;
////			INT8U flagMeter = 0 ;
////		    if(!readMeterFlag)
////			{
////				lcdReadMeterInform = App_ReadMeterInform_Function( );
////				flagMeter = lcdReadMeterInform->RMeter_Flag_Array[(dataStuct.point-1)/8];
////				flagMeter&=BIT((dataStuct.point-1)%8) ;
////			}
////			else
////			{
////				flagMeter = 0 ;	
////			}
////			if( !flagMeter )
////			{
////				memset(dataStuct.dbuff,0xee,len);	
////			}
////		}
//		/*帧有效+DI01*/
////		if(0x0C07!=dataID_S)
////		{
////			app_ReadOneDataFrame.Len+=2 ;
////		}
//		if( len + pFrame_Str->LenTx > framMaxLen)
//		{
//			/*倒退*/
//			pointIndex -- ;

//			if(0x0C07!=dataID_S)
//			{
//				*pFrame_Str->pTxData = pointAountFrame ;	//数量
//				pFrame_Str->LenTx -=2 ;  //point
//				
//			}
////			else
////			{
////				app_ReadOneDataFrame.DataBuffer[3]= eventStart  ;	//起始事件
////				app_ReadOneDataFrame.DataBuffer[4]= pointAountFrame ;	//事件数量

////			    eventStart = pointAountFrame ;
////			}

//			
//			/*发送一个帧*/
//			frameTpye++;
//			frameTpye = frameTpye?2:1;  /*0单帧 1起始帧 2中间帧 3结束帧*/
//			pFrame_Str->App_Frame_HeadStr->Seq &=0x9f;
//			pFrame_Str->App_Frame_HeadStr->Seq|=(frameTpye<<5) ;
//			App_Agr_MakeFrame(pFrame_Str);
//			pFrame_Str->App_Frame_HeadStr->Seq ++ ;
//			pSendBuff = pFrame_Str->pTxData ;
//			pFrame_Str->LenTx = 0 ;
//			
//			if(0x0C07!=dataID_S)
//			{
//				
//			}
//			else
//			{
//				pointAmount = 1 ;
////				eventStart =  pointIndex;
////				eventAmount = pointAountFrame;
//			}
//			pointAountFrame = 0 ;
//			continue ;
//		}
//		pointAountFrame ++ ;
//		memcpy(pSendBuff,tmpArray,len);				
//		pSendBuff+=len ;							//下个数据的起始位置
//		pFrame_Str->LenTx+=len;
//	    
//		if(!pointAllFlag)
//		{
//			if(0x0C0A==dataID_S)
//			{
//				pRx+=6 ;			/*偏移下发帧数据位置*/
//			
//			}
//			else if(0x0C07!=dataID_S)
//			{
//				pRx+=2 ;			/*偏移下发帧数据位置*/
//			}
//			
//			if((0xC02 ==dataID_S)||(0xC04 ==dataID_S))
//			{
//				pRx+=3 ;			/*时标*/
//				dataStuct.pSource += 5 ;
//			}
//			else if(0xC08 ==dataID_S)
//			{
//				pRx+=3 ;			/*时标*/
//			}			
//		}
//    } 
//	if(len)
//	{
//		*pFrame_Str->pTxData = pointIndex ;	//数量				
//	}
//	pFrame_Str->App_Frame_HeadStr->Seq &=0x9f;
//	if(!frameTpye)
//	{
//		frameTpye =3 ;
//	}
//	else
//	{
//		frameTpye =1 ;
//	}
//	pFrame_Str->App_Frame_HeadStr->Seq|=(frameTpye<<5) ;	
//	App_Agr_MakeFrame(pFrame_Str);	

		
}





/************************(C)COPYRIGHT *****END OF FILE****************************/








