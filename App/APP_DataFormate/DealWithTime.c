///*****************************************Copyright(C)******************************************
//*------------------------------------------文件信息---------------------------------------------
//* FileName			: DealWithTime.c
//* Date First Issued	: 2007.05.21
//* Version			: V1.0
//* Description		: 定义各程序包共用的时间处理函数
//*----------------------------------------历史版本信息-------------------------------------------
//* History			:
//* //2008			: 
//* Description		: 
//*-----------------------------------------------------------------------------------------------
//***********************************************************************************************/
///* Includes-----------------------------------------------------------------------------------*/
//#include "app.h"
///* Private define-----------------------------------------------------------------------------*/
//#define SecTime  1   //分
//#define MinTime  2  //分钟
//#define HourTime 3  //小时
//#define DayTime  4  //天
//#define YueTime  5  //月
//#define YearTime 0  //年
///* Private typedef----------------------------------------------------------------------------*/
///* Private macro------------------------------------------------------------------------------*/
///* Private variables--------------------------------------------------------------------------*/
///* Private function prototypes----------------------------------------------------------------*/
//void GetTheTime(_BSPRTC_TIME* Ptr_RtcTim);//获取当前时间
//void AdjustTime(_BSPRTC_TIME* pTime, INT32U numb, BOOLEAN state);//计算某个时间点的倒退或将来的时间
//INT16S CompareTime(_BSPRTC_TIME Time1, _BSPRTC_TIME Time2);//比较两个时间点的先后
//INT32U CmpTheTime(_BSPRTC_TIME *TimeBuff,_BSPRTC_TIME *TimeBuff2,INT8U Type);//计算两个时间点的时间差
//INT8U CmpTeskTime(INT8U *TimeBuff);//与当前时间比大小
///* Private functions--------------------------------------------------------------------------*/
///*****************************************************************   
//函数名称：INT16S CompareTime(RTC_TIME_S Time1, RTC_TIME_S Time2)
//输入参数：Time1，Time2－作比较的两个时间值
//输出参数：0 相等 1 大于 -1 小于
//约束条件：无
//功能描述：时间比较
//全局变量：
//调用模块：
//作    者：王夏娇			日期: 2007.05.27 
//修    改：					日期:  
//版    本：V1.0
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
// 函数:	 CmpTeskTime
// 功能：  与当前时间比大小
// 返回值：1 比较时间小  0 当前时间小
// 作者：  
//*************************************************************************/ 
//INT8U  CmpTeskTime(INT8U *TimeBuff)
//{
//	if((TimeBuff[0]==0x00)&&(gRtcTime.Year==0x99))//跨越世纪的情况
//	{
//		return 0;
//	}
//	if(TimeBuff[0]<gRtcTime.Year)   return 1;
//	if(TimeBuff[0]>gRtcTime.Year)   return 0;//年分要大
//	                        
//	if(TimeBuff[1]<gRtcTime.Month)  return 1;
//	if(TimeBuff[1]>gRtcTime.Month)  return 0;//月分要大
//	                        
//	if(TimeBuff[2]<gRtcTime.Day)    return 1; 
//	if(TimeBuff[2]>gRtcTime.Day)    return 0;//日要大
//	                        
//	if(TimeBuff[3]<gRtcTime.Hour)   return 1;
//	if(TimeBuff[3]>gRtcTime.Hour)   return 0;//小时要大
//	                        
//	if(TimeBuff[4]<gRtcTime.Minute) return 1;
//	if(TimeBuff[4]>gRtcTime.Minute) return 0;
//	return 0;//年、月、天、时、分都相同
//}

// 
///*******************************************************************************
//* Function Name  : void AdjustTime(_BSPRTC_TIME* pTime, INT32U numb, BOOLEAN state)
//* Description    : 时间调整numb秒  天 小时  分 都可以折算成秒
//* Input          : pTime 指向需调整的时间结构
//                    numb 调整的幅度  单位秒
//                    state  TRUE为顺时针   FALSE 为逆时针
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
//* Description    : 获取当前时间
//* Input          : 存放实时时间的结构体指针
//* Output         : None
//* Return         : None
//*******************************************************************************/   
//void GetTheTime(_BSPRTC_TIME* Ptr_RtcTim)
//{
//	memcpy((INT8U*)Ptr_RtcTim,(INT8U*)&gRtcTime,7);
//}

///******************************************************************************************
// 函数 CmpTheTime 
// 功能:比较两个时间的之差
//      TimeBuff2-TimeBuff
// 入参:
//      TimeBuff  减数     BCD码
//                TimeBuff[0]:年
//      			TimeBuff[1]:月
//      			TimeBuff[2]:日
//      			TimeBuff[3]:时
//      			TimeBuff[4]:分
//      TimeBuff2 被减数  BCD码
//     			TimeBuff2[0]:年
//      			TimeBuff2[1]:月
//      			TimeBuff2[2]:日
//      			TimeBuff2[3]:时
//      			TimeBuff2[4]:分
//      Type:     减类型
//                02           分
//                03           时
//                04           日
//                05           月 
// 返回值: =   TimeBuff2-TimeBuff    Hex码    
// 其它说明
//       1:TimeBuff>TimeBuff2 则返回0xffffffff
//       2:TimeBuff2>TimeBuff 时间差不能间隔1年 也返回0xffffffff
//       
//       3:差值是以Type为单位.
//       
//       如:Type=04  日,如果实际时间差为23时59分,返回值也为0
// 作者:       创作时间:07/06/02
// 历史记录
//      07/06/02 没有考虑瑞年
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
//		return  tempTimeSecond;//秒
//	tempTimeSecond/=60;
//	if(Type==2) 
//		return tempTimeSecond;//分
//	tempTimeSecond/=60;
//	if(Type==3) 
//		return  tempTimeSecond;//时
//	tempTimeSecond/=24;
//	if(Type==4) 
//		return  tempTimeSecond;//日
//	CmptempTimeSecond=0;
//	if(Type==5)                        //月  
//	{
//		while(tempTimeSecond)
//		{
//			switch(tmTime.tm_mon)
//			{
//				case 0:	//1月
//				case 2:	//3月
//				case 4:	//5月
//				case 6:	//7月
//				case 7:	//8月
//				case 9:	//10月
//				case 11://12月
//					if(tempTimeSecond>=31)  tempTimeSecond-=31;
//					else             return CmptempTimeSecond;//不足31天 
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
//					else             return CmptempTimeSecond;//不足31天 
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
