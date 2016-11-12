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
	ϵͳ��һЩ������ʼ����ֵ
*/

void Device_Info_Init(void)
{
	/*ϵͳ��ʼʱ��*/
	_BSPRTC_TIME realTime;
	__App_Para_InterfaceStr para_data;	
	INT8U Buff[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	realTime.Year = 0x16;	//��
	realTime.Month = 0x02; 			//��
	realTime.Day = 0x25;		//��
	realTime.Hour = 0x00;			//ʱ
	realTime.Minute = 0x59;		//��
	realTime.Second =0x45;			//��
	BSP_RTCSetTime(&realTime);	
	
	/*�汾����*/
	SW_Version = 0xaaaaaaaa;		//����汾��
	HW_Version = 0xaaaaaaaa;		//Ӳ���汾��
	SW_ReleaseData[0] = 0x26;		//�����������
	SW_ReleaseData[1] = 0x02;		
	SW_ReleaseData[2] = 0x16;		
	HW_ReleaseData[0] = 0x26;		//Ӳ����������
	HW_ReleaseData[1] = 0x02;
	HW_ReleaseData[2] = 0x16;
	/*��ַ����*/
//	para_data.afn = METER_WRITE;			//7�ֽڵ�ַ
//	para_data.IDIndex = 0x040e;
//	para_data.point = 1;
//	para_data.dbuff = Buff;
//	para_data.dlen  = 7;
//	GWPara_function(&para_data);			//��ȡ�߼���ַ
	/*�������*/
	GPRS_Interval = 1;
	para_data.afn = METER_WRITE;	
	para_data.IDIndex = 0x0406;
	para_data.point = 1;
	para_data.dbuff = Buff;						//�Ѽ������������ݶ���
	para_data.dlen  =  10 ;
	GWPara_function(&para_data);	
	/*GPRS����*/
//	App_Gprs_SetConentIP(101,69,242,138,7001);
}		
/*
	һ��LED��ˮ�ƣ�����Ƭ�������˻��������ϵ磬�������
**/
void rePower_TEST(void)
{
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)0);		//���еƵ���
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)0);		//���еƵ���
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)0);		//���еƵ���
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)0);		//���еƵ���
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)1);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)2);
	Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);
	Delay(500);
	Bsp_Led_SetL((__Bsp_Enum_Led_Name)3);
}	
/****************************************************************************
 * @brief  Main function  ϵͳ����ʱ�䲻�ܳ���8S
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
	
	rePower_TEST();			//ģ��������ʱ�����һ����ˮ��
//	Device_Info_Init();	
//	App_Gprs_SetConentIP(101,69,242,138,7001);
	while(1)
	{
		Bsp_Test();
		
		/*****************************************
			�ٿ�������
		�����̰�һ�¿���1���ӣ������ر�
		******************************************/
		App_ManipulatorMain();		
		/*****************************************
			��������
		ÿ�쳭��һ�Σ�����ʱ����sysTick��
		*******************************************/
		App_ReadMeterMain();	
		/******************************************
			�ϱ�����
		ÿ���ϱ�һ�Σ��ϱ�ʱ����sysTick��
		*******************************************/		
		App_Gprs_Main();	
//		Bsp_Led_Toggle(BSP_ENUM_LED_RUN);
		
		if( (App_485_Get_WorkState() < 2) && (App_Gprs_Get_WorkState() < 2) )	//�������ִ������
		{
			EMU_EnterEM2(true);		//����͹���
		}
		else		
		{
			Delay(100);
		}
	}

}

