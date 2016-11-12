/*****************************************Copyright(C)******************************************
*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName          : App_Agr_RF.h
* Author            : fjb
* Date First Issued : 2015/8/03
* Version           : V1.0
* Description       : 
*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
* History           :
* //2008            : V
* Description       : 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
#ifndef __App_Agr_RF__H__
#define __App_Agr_RF__H__
/* Includes-----------------------------------------------------------------------------------*/

#include "App_Para_Interface.h"

/* Private define-----------------------------------------------------------------------------*/
 
/* Private enum-------------------------------------------------------------------------------*/
typedef enum 
{   
//	
	APP_RF_ORD_REG_FORCE= 0x01			  //ǿ��ע�� 	
	
//ͨ���ɼ��� 0x10	

	,APP_RF_ORD_NODE_READMETER= 0x10  	
	,APP_RF_ORD_NODE_READMETER_ACK  	
	,APP_RF_ORD_NODE_REG                //�ɼ���ע��
	,APP_RF_ORD_NODE_REG_ACK  	
//	,APP_RF_ORD_NODE_RELAY_REG		    //�ɼ����м�ע��
//	,APP_RF_ORD_NODE_RELAY_REG_ACK  	
	,APP_RF_ORD_NODE_METER_REG		    //�ɼ����м�ע��
	,APP_RF_ORD_NODE_METER_REG_ACK  	
	,APP_RF_ORD_RM_WIRED	= 0x16		//���߱�Ƴ�������
	,APP_RF_ORD_RM_WIRED_ACK  	

	,APP_RF_ORD_NODE_TIME_ADJST			//18
	,APP_RF_ORD_TIME_ADJST_RELY_ACK  	
	,APP_RF_ORD_NODE_VALVE				//������ֱ�ӷ��ſ����������
	,APP_RF_ORD_VALVE_RELAY_ACK  			
	,APP_RF_ORD_RM_RELAY	 =0x1C		//�ɼ��鳭��	
	,APP_RF_ORD_RM_RELAY_ACK	

//��� 0xA0	
	,APP_RF_ORD_READMETER = 0xA0 		
	,APP_RF_ORD_READMETER_ACK  	
	,APP_RF_ORD_REG_NODE
	,APP_RF_ORD_REG_NODE_ACK  	
	,APP_RF_ORD_A4	
	,APP_RF_ORD_A5  	
	,APP_RF_ORD_TIME_ADJST			//a6--��ʱ
	,APP_RF_ORD_A9  	
	,APP_RF_ORD_VALVE=0xA8			//1A���ſ��ؿ���
	,APP_RF_ORD_VALVE_ACK	
	,APP_RF_ORD_VALVE_IS_PERMIT=0xAA	//���ſ���������
	,APP_RF_ORD_VALVE_IS_PERMIT_ACK	
	
//P2P ����
	,APP_RF_ORD_DIR_WORK_W = 0xD0 		//����ʱ��
	,APP_RF_ORD_DIR_WORK_W_ACK       	//Ӧ��	
	,APP_RF_ORD_DIR_WORK_R   	 		//��ȡ
	,APP_RF_ORD_DIR_WORK_R_ACK       	//Ӧ��	
	
	,APP_RF_ORD_DIR_ADDR_W = 0xE0 		//���õ�ַ
	,APP_RF_ORD_DIR_ADDR_W_ACK       	//Ӧ��	
	,APP_RF_ORD_DIR_ADDR_R      		//��ѯ��ַ
	,APP_RF_ORD_DIR_ADDR_R_ACK       	//	
	,APP_RF_ORD_DIR_PARA_W      		//rf �������� FREQ SP,BW,
	,APP_RF_ORD_DIR_PARA_W_ACK       	//	
	,APP_RF_ORD_DIR_PARA_R      		//rf ������ȡ FREQ SP,BW,
	,APP_RF_ORD_DIR_PARA_R_ACK       	//	
 	,APP_RF_ORD_DIR_STATE_W      		//���ñ��״̬����
	,APP_RF_ORD_DIR_STATE_W_ACK       	//	
 	,APP_RF_ORD_DIR_STATE_R      		//��ȡ���״̬����
	,APP_RF_ORD_DIR_STATE_R_ACK       	//	
 	,APP_RF_ORD_DIR_BASEDATA_W      	//���ñ�ƻ���
	,APP_RF_ORD_DIR_BASEDATA_W_ACK      //	
 	,APP_RF_ORD_DIR_BASEDATA_R      	//��ȡ�������
	,APP_RF_ORD_DIR_BASEDATA_R_ACK      //	
 	,APP_RF_ORD_DIR_VALVE_W      	    //���ÿ��Ʒ�����ָ��
	,APP_RF_ORD_DIR_VALVE_W_ACK         //	
 	,APP_RF_ORD_DIR_METERDATA_C      	//�����������ʼ��
	,APP_RF_ORD_DIR_METERDATA_C_ACK     //	
 	,APP_RF_ORD_DIR_METERPARA_C      	//��Ʋ�������ʼ��
	,APP_RF_ORD_DIR_METERPARA_C_ACK     //	
 	,APP_RF_ORD_DIR_TIME_ADJUST      	//��ʱ����
	,APP_RF_ORD_DIR_TIME_ADJUST_ACK     //	
 	,APP_RF_ORD_DIR_TIME_R      	    //��ȡʱ��
	,APP_RF_ORD_DIR_TIME_R_ACK          //	
	,APP_RF_ORD_DIR_REG_R   	 		//��ȡע����Ϣ
	,APP_RF_ORD_DIR_REG_R_ACK       	//Ӧ��	
	
	
}__App_NL_RF_ORDDef ;



/* Private typedef----------------------------------------------------------------------------*/
typedef enum{ ERROR_SEND ,NORMAL_SEND , SEND_DATA,NO_SEND_DATA}__App_Enum_Echo_Type; 

/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

/**********************************************************************************************
* Description       : ·��Э�� ������
* Input             : pBuff�����ݻ��� Len--����
* Output            : none
***********************************************************************************************/
__App_Enum_Echo_Type App_Agr_RF_Process(INT8U  *pBuff,INT8U Len);

__App_Task_State App_Agr_Rf_Main(void );

//LPʱ��ʼ��RF����
void App_SX1276_Free_Config(void);


#endif  // __App_Agr_RF__H__

