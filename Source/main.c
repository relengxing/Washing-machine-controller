/**************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for EFM32TG_STK3300
 * @version 3.20.12
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "sysTick.h"
#include "bsp_Test.h"
#include "bsp_Led.h"
#include "bsp_key.h"
#include "App_485_Interface.h"
#include "App_M6310.h"

/*
	系统的一些参数初始化赋值
*/

void Device_Info_Init(void)
{
	/*系统初始时间*/
	_BSPRTC_TIME realTime;
	__App_Para_InterfaceStr para_data;	
	INT8U Buff[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	realTime.Year = 0x16;	//年
	realTime.Month = 0x02; 			//月
	realTime.Day = 0x25;		//日
	realTime.Hour = 0x00;			//时
	realTime.Minute = 0x59;		//分
	realTime.Second =0x45;			//秒
	BSP_RTCSetTime(&realTime);	
	
	/*版本参数*/
	SW_Version = 0xaaaaaaaa;		//软件版本号
	HW_Version = 0xaaaaaaaa;		//硬件版本号
	SW_ReleaseData[0] = 0x26;		//软件发布日期
	SW_ReleaseData[1] = 0x02;		
	SW_ReleaseData[2] = 0x16;		
	HW_ReleaseData[0] = 0x26;		//硬件发布日期
	HW_ReleaseData[1] = 0x02;
	HW_ReleaseData[2] = 0x16;
	/*地址参数*/
//	para_data.afn = METER_WRITE;			//7字节地址
//	para_data.IDIndex = 0x040e;
//	para_data.point = 1;
//	para_data.dbuff = Buff;
//	para_data.dlen  = 7;
//	GWPara_function(&para_data);			//读取逻辑地址
	/*抄表参数*/
	GPRS_Interval = 1;
	para_data.afn = METER_WRITE;	
	para_data.IDIndex = 0x0406;
	para_data.point = 1;
	para_data.dbuff = Buff;						//把集中器户表数据读出
	para_data.dlen  =  10 ;
	GWPara_function(&para_data);	
	/*GPRS参数*/
//	App_Gprs_SetConentIP(101,69,242,138,7001);
}		
/*
	一段LED流水灯，代表单片机重启了或者重新上电，方便测试
**/
void rePower_TEST(void)
{
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)0);		//所有灯点亮
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)0);		//所有灯点亮
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)0);		//所有灯点亮
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)0);		//所有灯点亮
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
}	
/****************************************************************************
 * @brief  Main function  系统任务时间不能超过8S
 *****************************************************************************/
int main(void)
{
	/* Chip errata */
	CHIP_Init();
//	Bsp_I2CSPM_Init();

	BSP_RTC_Init();
	Bsp_Led_Init();
	Bsp_Key_PinConfig();
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / SYS_TICK_COUNT))
	{
		while (1) ;
	}	 
	
	rePower_TEST();			//模块重启的时候会有一次流水灯
//	Device_Info_Init();	
//	App_Gprs_SetConentIP(101,69,242,138,7001);
	while(1)
	{
		Bsp_Test();
		
		/*****************************************
			操控器任务
		按键短按一下开启1分钟，长按关闭
		******************************************/
		App_ManipulatorMain();		
		/*****************************************
			抄表任务
		每天抄表一次，抄表时间在sysTick中
		*******************************************/
		App_ReadMeterMain();	
		/******************************************
			上报任务
		每天上报一次，上报时间在sysTick中
		*******************************************/		
		App_Gprs_Main();	
//		Bsp_Led_Toggle(BSP_ENUM_LED_RUN);
		
		if( (App_485_Get_WorkState() < 2) && (App_Gprs_Get_WorkState() < 2) )	//如果不在执行任务
		{
			EMU_EnterEM2(true);		//进入低功耗
		}
		else		
		{
			Delay(100);
		}
	}

}

