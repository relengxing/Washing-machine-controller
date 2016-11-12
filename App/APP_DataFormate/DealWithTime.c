///*****************************************Copyright(C)******************************************
//*------------------------------------------�ļ���Ϣ---------------------------------------------
//* FileName			: DealWithTime.c
//* Date First Issued	: 2007.05.21
//* Version			: V1.0
//* Description		: �������������õ�ʱ�䴦����
//*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
//* History			:
//* //2008			: 
//* Description		: 
//*-----------------------------------------------------------------------------------------------
//***********************************************************************************************/
///* Includes-----------------------------------------------------------------------------------*/
//#include "app.h"
///* Private define-----------------------------------------------------------------------------*/
//#define SecTime  1   //��
//#define MinTime  2  //����
//#define HourTime 3  //Сʱ
//#define DayTime  4  //��
//#define YueTime  5  //��
//#define YearTime 0  //��
///* Private typedef----------------------------------------------------------------------------*/
///* Private macro------------------------------------------------------------------------------*/
///* Private variables--------------------------------------------------------------------------*/
///* Private function prototypes----------------------------------------------------------------*/
//void GetTheTime(_BSPRTC_TIME* Ptr_RtcTim);//��ȡ��ǰʱ��
//void AdjustTime(_BSPRTC_TIME* pTime, INT32U numb, BOOLEAN state);//����ĳ��ʱ���ĵ��˻�����ʱ��
//INT16S CompareTime(_BSPRTC_TIME Time1, _BSPRTC_TIME Time2);//�Ƚ�����ʱ�����Ⱥ�
//INT32U CmpTheTime(_BSPRTC_TIME *TimeBuff,_BSPRTC_TIME *TimeBuff2,INT8U Type);//��������ʱ����ʱ���
//INT8U CmpTeskTime(INT8U *TimeBuff);//�뵱ǰʱ��ȴ�С
///* Private functions--------------------------------------------------------------------------*/
///*****************************************************************   
//�������ƣ�INT16S CompareTime(RTC_TIME_S Time1, RTC_TIME_S Time2)
//���������Time1��Time2�����Ƚϵ�����ʱ��ֵ
//���������0 ��� 1 ���� -1 С��
//Լ����������
//����������ʱ��Ƚ�
//ȫ�ֱ�����
//����ģ�飺
//��    �ߣ����Ľ�			����: 2007.05.27 
//��    �ģ�					����:  
//��    ����V1.0
//*****************************************************************/ 
//INT16S CompareTime(_BSPRTC_TIME Time1, _BSPRTC_TIME Time2)
//{
// 
//	if(Time1.Year   > Time2.Year)    return  1;
//	if(Time1.Year   < Time2.Year)    return -1;
//             
//	if(Time1.Month  > Time2.Month)   return  1;
//	if(Time1.Month  < Time2.Month)   return -1;
//             
//	if(Time1.Day    > Time2.Day)	 return  1;
//	if(Time1.Day    < Time2.Day)	 return -1;
//             
//	if(Time1.Hour   > Time2.Hour)	 return  1;		
//	if(Time1.Hour   < Time2.Hour)    return -1;
//             
//	if(Time1.Minute > Time2.Minute)  return  1;
//	if(Time1.Minute < Time2.Minute)  return -1;
//	return 0;			
//}

///************************************************************************
// ����:	 CmpTeskTime
// ���ܣ�  �뵱ǰʱ��ȴ�С
// ����ֵ��1 �Ƚ�ʱ��С  0 ��ǰʱ��С
// ���ߣ�  
//*************************************************************************/ 
//INT8U  CmpTeskTime(INT8U *TimeBuff)
//{
//	if((TimeBuff[0]==0x00)&&(gRtcTime.Year==0x99))//��Խ���͵����
//	{
//		return 0;
//	}
//	if(TimeBuff[0]<gRtcTime.Year)   return 1;
//	if(TimeBuff[0]>gRtcTime.Year)   return 0;//���Ҫ��
//	                        
//	if(TimeBuff[1]<gRtcTime.Month)  return 1;
//	if(TimeBuff[1]>gRtcTime.Month)  return 0;//�·�Ҫ��
//	                        
//	if(TimeBuff[2]<gRtcTime.Day)    return 1; 
//	if(TimeBuff[2]>gRtcTime.Day)    return 0;//��Ҫ��
//	                        
//	if(TimeBuff[3]<gRtcTime.Hour)   return 1;
//	if(TimeBuff[3]>gRtcTime.Hour)   return 0;//СʱҪ��
//	                        
//	if(TimeBuff[4]<gRtcTime.Minute) return 1;
//	if(TimeBuff[4]>gRtcTime.Minute) return 0;
//	return 0;//�ꡢ�¡��졢ʱ���ֶ���ͬ
//}

// 
///*******************************************************************************
//* Function Name  : void AdjustTime(_BSPRTC_TIME* pTime, INT32U numb, BOOLEAN state)
//* Description    : ʱ�����numb��  �� Сʱ  �� �������������
//* Input          : pTime ָ���������ʱ��ṹ
//                    numb �����ķ���  ��λ��
//                    state  TRUEΪ˳ʱ��   FALSE Ϊ��ʱ��
//* Output         : None
//* Return         : None
//*******************************************************************************/    
//void AdjustTime(_BSPRTC_TIME* pTime, INT32U numb, BOOLEAN state)
//{
//    time_t tempTimeSecond;
//    struct tm* ptmTime;
//    struct tm tmTime;
//    
//    tmTime.tm_sec  = ByteBcdToHex(pTime->Second); 
//    tmTime.tm_min  = ByteBcdToHex(pTime->Minute);
//    tmTime.tm_hour = ByteBcdToHex(pTime->Hour);
//    tmTime.tm_mday = ByteBcdToHex(pTime->Day);
//    tmTime.tm_mon  = ByteBcdToHex(pTime->Month) -1;
//    tmTime.tm_year = ByteBcdToHex(pTime->Year) +100;
//    
//    tempTimeSecond = mktime(&tmTime);
//    if(state == TRUE)
//        tempTimeSecond += numb;
//    else
//        tempTimeSecond -= numb;
//    ptmTime = localtime(&tempTimeSecond); 
// 
//    pTime->Second = ByteHexToBcd(ptmTime->tm_sec);
//    pTime->Minute = ByteHexToBcd(ptmTime->tm_min);
//    pTime->Hour = ByteHexToBcd(ptmTime->tm_hour);
//    pTime->Day = ByteHexToBcd(ptmTime->tm_mday);
//    pTime->Month = ByteHexToBcd(ptmTime->tm_mon +1);
//    pTime->Year = ByteHexToBcd(ptmTime->tm_year -100);

//}


///*******************************************************************************
//* Function Name  : void GetTheTime(RTC_TIME_S* Ptr_RtcTim)
//* Description    : ��ȡ��ǰʱ��
//* Input          : ���ʵʱʱ��Ľṹ��ָ��
//* Output         : None
//* Return         : None
//*******************************************************************************/   
//void GetTheTime(_BSPRTC_TIME* Ptr_RtcTim)
//{
//	memcpy((INT8U*)Ptr_RtcTim,(INT8U*)&gRtcTime,7);
//}

///******************************************************************************************
// ���� CmpTheTime 
// ����:�Ƚ�����ʱ���֮��
//      TimeBuff2-TimeBuff
// ���:
//      TimeBuff  ����     BCD��
//                TimeBuff[0]:��
//      			TimeBuff[1]:��
//      			TimeBuff[2]:��
//      			TimeBuff[3]:ʱ
//      			TimeBuff[4]:��
//      TimeBuff2 ������  BCD��
//     			TimeBuff2[0]:��
//      			TimeBuff2[1]:��
//      			TimeBuff2[2]:��
//      			TimeBuff2[3]:ʱ
//      			TimeBuff2[4]:��
//      Type:     ������
//                02           ��
//                03           ʱ
//                04           ��
//                05           �� 
// ����ֵ: =   TimeBuff2-TimeBuff    Hex��    
// ����˵��
//       1:TimeBuff>TimeBuff2 �򷵻�0xffffffff
//       2:TimeBuff2>TimeBuff ʱ���ܼ��1�� Ҳ����0xffffffff
//       
//       3:��ֵ����TypeΪ��λ.
//       
//       ��:Type=04  ��,���ʵ��ʱ���Ϊ23ʱ59��,����ֵҲΪ0
// ����:       ����ʱ��:07/06/02
// ��ʷ��¼
//      07/06/02 û�п�������
//      
//*******************************************************************************************/
//INT32U CmpTheTime(_BSPRTC_TIME *TimeBuff,_BSPRTC_TIME *TimeBuff2,INT8U Type)
//{	
//	time_t tempTimeSecond;
//	time_t CmptempTimeSecond;
//	struct tm tmTime;
//	struct tm CmpTime;
//	if(Type==1)
//	{
//		tmTime.tm_sec =  ByteBcdToHex(TimeBuff->Second);
//		CmpTime.tm_sec=  ByteBcdToHex(TimeBuff2->Second);
//	}
//	else   
//    {
//		tmTime.tm_sec=0;
//		CmpTime.tm_sec=0;
//	}
//	tmTime.tm_min =  ByteBcdToHex(TimeBuff->Minute);
//	tmTime.tm_hour = ByteBcdToHex(TimeBuff->Hour);
//	tmTime.tm_mday = ByteBcdToHex(TimeBuff->Day);
//	tmTime.tm_mon =  ByteBcdToHex(TimeBuff->Month) -1;
//	tmTime.tm_year = ByteBcdToHex(TimeBuff->Year) +100;
//	
//	CmpTime.tm_min =  ByteBcdToHex(TimeBuff2->Minute);
//	CmpTime.tm_hour = ByteBcdToHex(TimeBuff2->Hour);
//	CmpTime.tm_mday = ByteBcdToHex(TimeBuff2->Day);
//	CmpTime.tm_mon =  ByteBcdToHex(TimeBuff2->Month) -1;
//	CmpTime.tm_year = ByteBcdToHex(TimeBuff2->Year) +100;
//	
//	tempTimeSecond = mktime(&tmTime);
//	CmptempTimeSecond=mktime(&CmpTime);
//	if(CmptempTimeSecond<tempTimeSecond)
//		return 0xffffffff;
//	tempTimeSecond= CmptempTimeSecond-tempTimeSecond;
//	if(tempTimeSecond==0)
//		return 0;
//	
//	if(Type==1) 
//		return  tempTimeSecond;//��
//	tempTimeSecond/=60;
//	if(Type==2) 
//		return tempTimeSecond;//��
//	tempTimeSecond/=60;
//	if(Type==3) 
//		return  tempTimeSecond;//ʱ
//	tempTimeSecond/=24;
//	if(Type==4) 
//		return  tempTimeSecond;//��
//	CmptempTimeSecond=0;
//	if(Type==5)                        //��  
//	{
//		while(tempTimeSecond)
//		{
//			switch(tmTime.tm_mon)
//			{
//				case 0:	//1��
//				case 2:	//3��
//				case 4:	//5��
//				case 6:	//7��
//				case 7:	//8��
//				case 9:	//10��
//				case 11://12��
//					if(tempTimeSecond>=31)  tempTimeSecond-=31;
//					else             return CmptempTimeSecond;//����31�� 
//					break;
//				case 1:
//					if(tmTime.tm_year&0x03)
//					{
//					if(tempTimeSecond>=28) tempTimeSecond-=28;
//					else             return CmptempTimeSecond;	
//					}
//					else
//					{
//					if(tempTimeSecond>=29) tempTimeSecond-=29;
//					else             return CmptempTimeSecond;	
//					}
//					break;
//				default:
//					if(tempTimeSecond>=30) tempTimeSecond-=30;
//					else             return CmptempTimeSecond;//����31�� 
//					break;
//			}
//			CmptempTimeSecond++;
//			tmTime.tm_mon++;
//			if(tmTime.tm_mon>11)
//				tmTime.tm_mon=0;
//		}
//		return CmptempTimeSecond;
//	}
//	return 0xffffffff; 
//}
