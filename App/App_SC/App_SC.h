/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/
#ifndef _APP_SC_H
#define _APP_SC_H
/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stdio.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "bsp_rtc.h"
#include "Bsp_Uart.h"
#include "App_Head.h"
#include "App_Para_Interface.h"
#include "App_Data_interface.h "
#include "bsp_iic.h"

/**--------------------------------------------------------------------------
					上行报文：单水表集中上报一天数据      命令码     0x80
					抄表序号  BCD   2BETY
					手表状态  HEX   1BETY
					采集条数  HEX   1BETY		0X01-0X18
					。。。。。。
					
					下行报文：系统平台应答				  命令码     0x20
					。。。。。。
--------------------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct __App_SC_FixedFrame_Struct
{
	INT8U FrameStart;			//起始帧0x10																
	INT8U ControlCode;			//控制码
	INT8U AidControlCode;		//辅助控制码
	INT8U ADDH;					//被叫方地址高位
	INT8U ADDL;					//被叫方地址低位
	INT8U CS;					//CS校验位				
	INT8U FrameStop;			//结束位0x16
}App_SC_FixedFrame_Struct;


typedef struct __App_SC_VariableFrame_Struct
{
	INT8U FrameStart;			//起始位0x68
	INT8U FrameLenthH;			//帧长高字节
	INT8U FrameLenthL;			//帧长低字节
	INT8U FrameFlag;			//可变帧标志位
	INT8U ControlCode;			//控制码
	INT8U ADDH;					//被叫方地址高位
	INT8U ADDL;					//被叫方地址低位
	INT8U *Data;				//数据域
	INT8U CS;					//CS校验
	INT8U FrameStop;			//结束位0x16
}App_SC_VariableFrame_Struct;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
extern INT8U ADDH;
extern INT8U ADDL;
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_SC_Report(void);
INT8U App_SC_Resolve(INT8U *pBuff,INT8U Lenth);
void App_SC_DealVariableFrame(App_SC_VariableFrame_Struct *pApp_SC_VariableFrame_Struct);		//处理可变帧
void App_SC_DealFixedFrame(App_SC_FixedFrame_Struct *pApp_SC_FixedFrame_Struct);		//处理固定帧
/******************************************************************************
* Description: 
* Input: 
* Output: Nothing
* Return: Nothing
******************************************************************************/
#endif
/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
