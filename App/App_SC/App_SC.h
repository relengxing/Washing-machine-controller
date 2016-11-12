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
					���б��ģ���ˮ�����ϱ�һ������      ������     0x80
					�������  BCD   2BETY
					�ֱ�״̬  HEX   1BETY
					�ɼ�����  HEX   1BETY		0X01-0X18
					������������
					
					���б��ģ�ϵͳƽ̨Ӧ��				  ������     0x20
					������������
--------------------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct __App_SC_FixedFrame_Struct
{
	INT8U FrameStart;			//��ʼ֡0x10																
	INT8U ControlCode;			//������
	INT8U AidControlCode;		//����������
	INT8U ADDH;					//���з���ַ��λ
	INT8U ADDL;					//���з���ַ��λ
	INT8U CS;					//CSУ��λ				
	INT8U FrameStop;			//����λ0x16
}App_SC_FixedFrame_Struct;


typedef struct __App_SC_VariableFrame_Struct
{
	INT8U FrameStart;			//��ʼλ0x68
	INT8U FrameLenthH;			//֡�����ֽ�
	INT8U FrameLenthL;			//֡�����ֽ�
	INT8U FrameFlag;			//�ɱ�֡��־λ
	INT8U ControlCode;			//������
	INT8U ADDH;					//���з���ַ��λ
	INT8U ADDL;					//���з���ַ��λ
	INT8U *Data;				//������
	INT8U CS;					//CSУ��
	INT8U FrameStop;			//����λ0x16
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
void App_SC_DealVariableFrame(App_SC_VariableFrame_Struct *pApp_SC_VariableFrame_Struct);		//����ɱ�֡
void App_SC_DealFixedFrame(App_SC_FixedFrame_Struct *pApp_SC_FixedFrame_Struct);		//����̶�֡
/******************************************************************************
* Description: 
* Input: 
* Output: Nothing
* Return: Nothing
******************************************************************************/
#endif
/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
