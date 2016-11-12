/*****************************************Copyright(C)******************************************
*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName          : App_Agr.h
* Author            : fjb
* Date First Issued : 2015/8/03
* Version           : V1.0
* Description       : ���ļ�רΪ��д��Լ�����������Լ�޹صĶ����벻Ҫ�ڴ˶���,������ȡ��AT45DB����
*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
* History           :
* //2008            : V
* Description       : 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
#ifndef __App_Agr__H__
#define __App_Agr__H__
/* Includes-----------------------------------------------------------------------------------*/

#include "Bsp_config.h " 
#include "Bsp_Uart.h"
#include "App_Head.h"

/* Private define-----------------------------------------------------------------------------*/
#define APP_AGR_RX_BUFFAMOUNT	250//200 
#define APP_AGR_TX_BUFFAMOUNT	250//500


//#define POINTAMOUNT	 1
/* Private enum----------------------------------------------------------------------------*/
typedef enum {NULL_ADDR = 0,GROUP_ADDR_1,GROUP_ADDR_2,GROUP_ADDR_3,GROUP_ADDR_4,GROUP_ADDR_5,GROUP_ADDR_6,GROUP_ADDR_7,GROUP_ADDR_8\
              ,GROUP_BROADCASE, TERNIMAL_ADDR,CASACDE_ADDR,SET_TER_ADDR}__ADDRTPYE;  /*��ַ����*/

/* Private typedef----------------------------------------------------------------------------*/

//typedef struct __App_Para_InterfaceStr__
//{
//    INT16U afn;            /*������ */
//    INT16U point;          /*������ */
//    INT16U IDIndex;			/*Դ���ݻ�����,��Ҫ�Ǵ���ID���ã��������ݴ��������� */
//    INT8U  *pSource;       /*��Ҫ�ǲ�����ѯ����Ҫ���ݵ�һЩ����*/
//	INT16U dlen;           /*Ŀ�����ݳ��� */
//    INT8U  *dbuff;         /*Ŀ�����ݻ����� */
//    
//}__App_Para_InterfaceStr; 


typedef struct
{
//	_BSP_MSGID MsgID;		// ��ϢID,���忴_BSP_MSGID����
	INT16U DivNum;			// �豸��  ���ڳ�����Ϣ�����в�����ĺ��� 
	INT16U DataLen;			// ���ݳ���, ����ע�������˵��	 ���ڳ�����Ϣ���������ݱ�ʶ�ĺ���
	INT8U  *pData;			// ����ָ��, ����ע�������˵��
}_BSP_MESSAGE;



/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/
extern INT8U App_Agr_Buff_Rx[APP_AGR_RX_BUFFAMOUNT];
extern INT8U App_Agr_Buff_Tx[APP_AGR_TX_BUFFAMOUNT];

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

#endif  //__App_Head__H__  

