/******************** (C) COPYRIGHT2015 NLKJ ********************
* File Name          : bsp_rtc.c
* Author             : 
* Version            : V1.00
* Date               : 2015.07.15
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_rtc.h"
#include "convertDataInterface.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define RTC_FREQ    32768
/* Private macro -------------------------------------------------------------*/
const INT8U BSP_Month_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

/* Private variables ---------------------------------------------------------*/
volatile INT32U BSP_Rtc_TimeCount ;
//volatile _BSPRTC_TIME gRtcTimeDefault=
//{
//	0x00
//	,0x15
//	,0x13
//	,0x11
//	,0x09
//	,0x15
//	,5
//	
//};


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/***********************************************************************************************
* Description	: 判断是否闰年 
* Input			: 年  hex
* Output		: 1/0
* Contributor	: 150617	
***********************************************************************************************/
static INT8U BSP_IsLeap_Year(INT8U Year)
{
	if(((Year%4 == 0)&&(Year%100 != 0)) || (Year%400 == 0))
	{
		return 1;
	}
	return 0 ;

}


/***********************************************************************************************
* Function		: Bsp_Get_Week
* Description	: 计算星期几
* Input			: *pTime:要设的时间数据指针 
* Output		: 1~7
* Contributor	: 150617	
***********************************************************************************************/
INT8U Bsp_Get_Week(volatile _BSPRTC_TIME *pTime)
{
	INT16U temp ; //7得倍数 
	INT16U  year = ByteBcdToHex(pTime->Year);
	INT16U  month = ByteBcdToHex(pTime->Month);
	INT8U   week ;
	INT8U   century ;
	if(pTime->Month<3)
	{
		year--;
		month+=12;
	}
	//w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1 
	temp = year%100 + (year%100)/4 ;
	century = 20+year/100;	//从20实际开始 
	week = (70+temp + century/4-(2*century)+(26*(month+1))/10+ByteBcdToHex(pTime->Day)-1)%7;
	return week ;
}

/***********************************************************************************************
* Function		: BSP_RTCSetTime
* Description	: 重新设置RTC时间
* Input			: *pTime:要设的时间数据指针 BCD码输入
* Output		: 1/0
* Contributor	: 150617	
***********************************************************************************************/
INT8U BSP_RTCSetTime(volatile _BSPRTC_TIME *pTime)
{
	INT16U temp ;
	INT32U secondCout = 0 ;
	INT8U curTemp ;
	INT16U curYear = ByteBcdToHex(pTime->Year);
	curYear+=2000;
	for( temp = 2000 ;temp < curYear ;temp++)
	{
		if(BSP_IsLeap_Year(temp)) 
		{
			secondCout+=31622400;
		}
		else
		{
			secondCout+=31536000;
		}
	}
	if(!pTime->Month)
	{
		return 0 ;
	}
	curTemp = ByteBcdToHex(pTime->Month&0x1f)-1;
	for(temp = 0 ; temp <curTemp ;temp++ )
	{
		secondCout+=(INT32U)BSP_Month_table[temp]*86400;
		if(BSP_IsLeap_Year(curYear)&&(1==temp)) 
		{
			secondCout+=86400 ;
		}

	}
	temp = ByteBcdToHex(pTime->Day)-1;
	secondCout+= temp*86400;
	
	temp = ByteBcdToHex(pTime->Hour);
	secondCout+= temp*3600;
	
	temp = ByteBcdToHex(pTime->Minute);
	secondCout+= temp*60;

	secondCout+= ByteBcdToHex(pTime->Second);

	BSP_Rtc_TimeCount = secondCout ;//1d144c96  15:14:10
	return 1 ;	
	
}


/***********************************************************************************************
* Function		: BSP_RTCGetTime
* Description	: 获取RTC时间
* Input			: *pTime:要设的时间数据指针 BCD码输出
* Output		: 1/0
* Contributor	: 150617	
***********************************************************************************************/
INT8U BSP_RTCGetTime(volatile _BSPRTC_TIME *pTime)
{
	INT32U temp ;
	INT16U dayCnt = 0 ;
	INT32U secondCout = BSP_Rtc_TimeCount  ;// RTC_GetCounter();
	temp=secondCout/86400 ;//得到天数
	if(dayCnt!=temp)//超过一天
	{
		INT16U year = 2000 ;// 起始年份
		INT16U curTemp = 0 ;
		dayCnt =temp ;
		//计算年
		while(temp>=365)
		{
			if(BSP_IsLeap_Year(year)) //闰年
			{
				if(temp>=366)
				{
					temp-=366 ;
				}
				else
				{
					year++;
					break;
				}
			}
			else
			{
				temp-=365;
			}
			year++;

		}
		year-=2000;
		pTime->Year = ByteHexToBcd(year);//得到年份
		//计算月
		curTemp = 0 ;
		while(temp>=28) 
		{
			if(BSP_IsLeap_Year(year)&&curTemp==1) //闰年是不是闰年 2月
			{
				if(temp>=29)
				{
					temp-=29 ;//
				}
				else
				{
					break ;
				}
			}
			else
			{
				if(temp>=BSP_Month_table[curTemp])
				{
					temp-=BSP_Month_table[curTemp];
				}
				else
				{
					break ;
				}				
			}	
			curTemp++ ;			
		}
		pTime->Month = ByteHexToBcd(curTemp+1);	//得到月份
		pTime->Day = ByteHexToBcd(temp+1);		//得到日期
		
		//time
		temp = secondCout%86400 ;
		pTime->Hour = ByteHexToBcd(temp/3600) ;			//时
		pTime->Minute = ByteHexToBcd((temp%3600)/60);	//分
		pTime->Second = ByteHexToBcd((temp%3600)%60 );	//秒
		pTime->Week = Bsp_Get_Week(pTime);				
	}
	return 1 ;	
	
}


///**************************************************************************//**
// * @brief RTC Interrupt Handler.
// *        Updates minutes and hours.
// *****************************************************************************/
//void RTC_IRQHandler(void)
//{
//	/* Clear interrupt source */
//	RTC_IntClear(RTC_IFC_COMP0);
//	BSP_Rtc_TimeCount++ ;
////	BSP_RTCGetTime(&gRtcTimeDefault);
//	App_Agr_Cad_On_Cycle();    

//}


/**************************************************************************//**
 * @brief Enables LFACLK and selects LFXO as clock source for RTC
 *        Sets up the RTC to generate an interrupt every minute.
 *****************************************************************************/
void BSP_RTC_Init(void)
{
	volatile _BSPRTC_TIME tRtcTimeDefault=
	{
		0x00
		,0x15
		,0x09
		,0x11
		,0x09
		,0x15
		,5
		
	};

	RTC_Init_TypeDef rtcInit = RTC_INIT_DEFAULT;

	/* Enable LE domain registers */
	CMU_ClockEnable(cmuClock_CORELE, true);

	/* Enable LFXO as LFACLK in CMU. This will also start LFXO */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	/* Set a clock divisor of 32 to reduce power conumption. */
	CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_32);

	/* Enable RTC clock */
	CMU_ClockEnable(cmuClock_RTC, true);

	/* Initialize RTC */
	rtcInit.enable   = false;  /* Do not start RTC after initialization is complete. */
	rtcInit.debugRun = false;  /* Halt RTC when debugging. */
	rtcInit.comp0Top = true ;   /* Wrap around on COMP0 match. */
	RTC_Init(&rtcInit);

	/* Interrupt every second */
	RTC_CompareSet(0, ((RTC_FREQ / 32) ) - 1 );

//	/* Interrupt every 10 second */
//	RTC_CompareSet(0, ((RTC_FREQ / 32)*10 ) - 1 );



	/* Enable interrupt */
	NVIC_EnableIRQ(RTC_IRQn);
	RTC_IntEnable(RTC_IEN_COMP0);

	/* Start Counter */
	RTC_Enable(true);
	BSP_RTCSetTime(&tRtcTimeDefault);
}



/******************* (C) COPYRIGHT 2015 NLKJ *****END OF FILE******************/

