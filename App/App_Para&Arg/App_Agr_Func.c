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
* Description       : ��ʼ����Լ�ĸ�������������,�ڳ�ʼ����ʱ�������ڵ�һ��λ��
* Input             : PoinIndex:������� ��=1
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
//* Function          : ��Լ�е�д��������
//* Output            : none
//***********************************************************************************************/
//void App_Parameter_Write(__App_Frame_Str *pFrame_Str)
//{
//	INT16U  pointAmount ,pointIndex; 
//	INT8U  *pRx;
//	INT16U dataID = 0 ;
//	INT16U len = 0 ;
//    INT32U offset_frame = 0 ;/*��֡��ʱ�򣬵�ַ��ƫ��λ�� */
//	__App_Para_InterfaceStr  parestuct;
//	INT8U  arrayPw[10] ;
//	INT8U  arrayCmpPW[9] = {0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
//	INT8U  tpyePw ;
//	INT16U countIndex ; 
//	

//	tpyePw = pFrame_Str->App_Frame_HeadStr->CMD&BIT(5);			//09.05	 ��Լ��Ҫ���޸� C
//	tpyePw = tpyePw ? 1: 0 ;
////	/*�ն���Կȡ�������� ����֡���� FrameGW.Len*/
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
//	/*��ȡ����*/
//	pRx = pFrame_Str->pRx  ;				// ָ��������

//	pFrame_Str->App_Frame_HeadStr->Seq ^= arrayPw[1];
//	pFrame_Str->App_Frame_HeadStr->Dt[0] ^= arrayPw[2];
//	pFrame_Str->App_Frame_HeadStr->Dt[1] ^= arrayPw[3];
//	
//	for(countIndex=0 ;countIndex < pFrame_Str->LenRx ; countIndex ++) //ȥ���� seq dt[2];
//	{
//		*pRx ^=arrayPw[1 + countIndex%8] ;	//��ȥ�汾��
//		pRx ++ ;		
//	}
//		
//	/*������ʵ�ʳ���*/
//	pRx = pFrame_Str->pRx  ;				// ָ��������
////	pRx = &pFrame_Str->App_Frame_HeadStr->Seq ;				// ����ǰ������ָ��λ�ã������¸�ֵ
//	/*�������һ��0x80 */
//	len = 0 ;
//	for( countIndex = 0  ;countIndex < pFrame_Str->LenRx ; countIndex++ )
//	{
//		if(0x80 == *pRx )		/*��¼�����һ��λ��*/
//		{
//			len = countIndex ;	
//		}
//		pRx++ ;
//	}
//	if(!len)
//	{
//		return ;
//	}
//	/*�����Ƿ� 00*/
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
//	pFrame_Str->LenRx = countIndex ;							//ʵ�ʳ���

//	/*�������*/
//	pRx = pFrame_Str->pRx  ;									// ����������λ��

//	dataID = TwoByteToShort(pFrame_Str->App_Frame_HeadStr->Dt);
//	pointAmount = *(pRx+0) ;

//	pRx+= 1 ;					  							   /*ָ���һ��������ŵ�λ��*/
//	parestuct.afn = AFN_SETPARA;  
//	parestuct.IDIndex = dataID;

//	offset_frame = 0 ;
//	pointIndex = 0 ;
//	parestuct.pSource = pFrame_Str->pRx+1 ;
//    while((pFrame_Str->LenRx> offset_frame )&&(pointAmount>pointIndex))
//    {
//		pointIndex++ ;
//	    //������� 
//		parestuct.point = *(pRx+1)<<8;
//	    parestuct.point += *pRx;

//		parestuct.dbuff =(pRx +2);

//		len = GWPara_function(&parestuct);
//		if(!len)	 	/*�зǷ��������*/
//		{
//			break;	
//		} 			                    
//		pRx+=len+2 ;	/*��������+�������*/
//		offset_frame += len+2 ;
//		pointIndex ++ ;
//    }//while end  

//    /*ȷ�ϵ���Ϣ���� */ 
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
// ** �������ƣ�void App_Parameter_Read
// ** ����������������ѯ����
// ** ��    ����none
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
///*����֡ ��ز�����ʼ��*/
//	pointAmount = *(pRx) ;					//����
//	pRx+= 1 ;					  			/*ָ���һ��������ŵ�λ��*/

///*�ظ�֡ ��ز�����ʼ��*/
//    framMaxLen= APP_AGR_TX_BUFFAMOUNT-20 ;	//ÿ֡������ֽ���  APP_AGR_BUFFAMOUNT


///*��������ر�־λ����ʼ��*/
//	parestuct.afn = AFN_INQUIREPARA;  
//	dataID = TwoByteToShort(pFrame_Str->App_Frame_HeadStr->Dt);
//	parestuct.IDIndex = dataID;
//	pointIndex = 0 ;
//	pSendBuff = pFrame_Str->pTxData+1 ;		//����������
//    pFrame_Str->LenTx =1;				   //Ԥ�� SEQ DI TA
//	while(pointIndex < pointAmount)
//    {
//		INT8U  buff[50];
//		pointIndex++ ;
//		/*��ȡ�������*/
//		parestuct.point = *(pRx+1)	<< 8 ;
//		parestuct.point |= *(pRx+0);
//		if(parestuct.point > POINTAMOUNT)
//		{
//			pFrame_Str->App_Frame_HeadStr->CMD |= 0xc0 ;
//			break ;
//		}
//		parestuct.dbuff = buff ;
//		/*��ȡ����*/
//		len = GWPara_function(&parestuct);
//		if(!len)
//		{
//			pFrame_Str->App_Frame_HeadStr->CMD |= 0xc0 ;			
//			break;
//		}
//		if(len + pFrame_Str->LenTx > framMaxLen)
//		{
//			*pFrame_Str->pTxData = pointIndex ;	//����
//			/*����һ��֡*/
//			frameTpye++;
//			frameTpye = frameTpye?2:1;  /*0��֡ 1��ʼ֡ 2�м�֡ 3����֡*/
//			pFrame_Str->App_Frame_HeadStr->Seq &=0x9f;
//			pFrame_Str->App_Frame_HeadStr->Seq|=(frameTpye<<5) ;
//			App_Agr_MakeFrame(pFrame_Str);
//			pFrame_Str->App_Frame_HeadStr->Seq ++ ;
//			pSendBuff = pFrame_Str->pTxData ;
//			pFrame_Str->LenTx = 0 ;
//			continue ;
//		}
//		else if(len)  /*û�г���һ֡����*/
//		{
//			pointIndex++;
//			*pSendBuff++ = *(pRx+0) ;	//������
//			*pSendBuff++ = *(pRx+1) ;
//			memcpy(pSendBuff,buff,len);	//��������
//			pSendBuff+=len ;
//			pFrame_Str->LenTx += (len + 2) ;
//			
//			pRx+=2 ;			/*ƫ���·�֡����λ��*/
//		}
//    } 
//	if(len)
//	{
//		*pFrame_Str->pTxData = pointIndex ;	//����		
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
//һ�����ݶ�ȡ				  
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

//	/*�ظ�֡ ��ز�����ʼ��*/
//    framMaxLen= APP_AGR_TX_BUFFAMOUNT-20 ;	//ÿ֡������ֽ���  APP_AGR_BUFFAMOUNT


//	/*��������ر�־λ����ʼ��*/
//	dataStuct.afn = AFN_INQUIREPARA;  
//	dataID_S = TwoByteToShort(pFrame_Str->App_Frame_HeadStr->Dt);
//	dataStuct.IDIndex = dataID_S;
//	pointIndex = 0 ;
//	pSendBuff = pFrame_Str->pTxData ;

//	pRx = pFrame_Str->pRx ;
//	/*����֡ ��ز�����ʼ��*/
//	pointAmount = *(pRx) ;					//����
//	pRx+= 1 ;					  			/*ָ���һ��������ŵ�λ��*/

//	/*��������ر�־λ����ʼ��*/
//	pointIndex = 0 ;
//    frameTpye = 0 ;
//	pointAountFrame = 0 ;
//	
//	dataStuct.afn = AFN_INQUIREPARA_ONE;  
//	dataStuct.IDIndex = dataID_S;
//	
//	dataStuct.pSource = pFrame_Str->pRx+1 ;
//	pSendBuff = pFrame_Str->pTxData ; 			//�ظ���վ��������
//    pFrame_Str->LenTx =1;				   //Ԥ�� SEQ DI TA
//	while(pointIndex < pointAmount)
//    {
//		INT8U tmpArray[75];
////		INT8U  readMeterFlag = 0 ;
//		pointIndex ++ ;
//		/*��ȡ�������*/
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
//			//Э����Ч
//			if(pointAllFlag)
//			{
////				if(!(pointIndex%30))
////				{
////					WDG_clr_flag = 1;
////				}
//			}
//			else if((0xC02 ==dataID_S)||(0xC04 ==dataID_S))
//			{
//				pRx+=5 ;			/*px + ʱ��*/
//				dataStuct.pSource += 5 ;
//			}
//			else if(0xC0A ==dataID_S)
//			{
//				pRx += 6 ;			/*px + ʱ��*/
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
////		if(dataID_S == 0xC01)	  //�㳭����
////		{
////			INT16U dataID ;
////			INT16U timeCount = 0 ;
////			INT16U timeLimit = 	SYS_DELAY_1M/SYS_DELAY_3s;
////			/*���͵㳭��Ϣ*/
////			dataID = 0x1F90; //���������ˮ��CJ188
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
//		/*��ȡ����*/
//		len = GWFirstData_function(&dataStuct);
//		/*�Ƿ��зǷ�ID ���߳��Ȳ���*/
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
//				pRx+=5 ;			/*ʱ��*/
//				dataStuct.pSource += 5 ;
//				continue ;
//			}
//			else if(0xC0A ==dataID_S)
//			{
//				pSendBuff-=2 ;
//				pRx+=6 ;			/*ʱ��*/
//				dataStuct.pSource += 6 ;
//				continue ;
//			}
//			else
//			{
//				break ;				
//			}
//		}
//		pFrame_Str->LenTx +=2 ;  //point
////		if(dataID_S == 0xC01)	  //�㳭����
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
//		/*֡��Ч+DI01*/
////		if(0x0C07!=dataID_S)
////		{
////			app_ReadOneDataFrame.Len+=2 ;
////		}
//		if( len + pFrame_Str->LenTx > framMaxLen)
//		{
//			/*����*/
//			pointIndex -- ;

//			if(0x0C07!=dataID_S)
//			{
//				*pFrame_Str->pTxData = pointAountFrame ;	//����
//				pFrame_Str->LenTx -=2 ;  //point
//				
//			}
////			else
////			{
////				app_ReadOneDataFrame.DataBuffer[3]= eventStart  ;	//��ʼ�¼�
////				app_ReadOneDataFrame.DataBuffer[4]= pointAountFrame ;	//�¼�����

////			    eventStart = pointAountFrame ;
////			}

//			
//			/*����һ��֡*/
//			frameTpye++;
//			frameTpye = frameTpye?2:1;  /*0��֡ 1��ʼ֡ 2�м�֡ 3����֡*/
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
//		pSendBuff+=len ;							//�¸����ݵ���ʼλ��
//		pFrame_Str->LenTx+=len;
//	    
//		if(!pointAllFlag)
//		{
//			if(0x0C0A==dataID_S)
//			{
//				pRx+=6 ;			/*ƫ���·�֡����λ��*/
//			
//			}
//			else if(0x0C07!=dataID_S)
//			{
//				pRx+=2 ;			/*ƫ���·�֡����λ��*/
//			}
//			
//			if((0xC02 ==dataID_S)||(0xC04 ==dataID_S))
//			{
//				pRx+=3 ;			/*ʱ��*/
//				dataStuct.pSource += 5 ;
//			}
//			else if(0xC08 ==dataID_S)
//			{
//				pRx+=3 ;			/*ʱ��*/
//			}			
//		}
//    } 
//	if(len)
//	{
//		*pFrame_Str->pTxData = pointIndex ;	//����				
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








