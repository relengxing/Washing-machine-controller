/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : App_Agr_RF.h
* Author            : fjb
* Date First Issued : 2015/8/03
* Version           : V1.0
* Description       : 
*----------------------------------------历史版本信息-------------------------------------------
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
	APP_RF_ORD_REG_FORCE= 0x01			  //强制注册 	
	
//通过采集器 0x10	

	,APP_RF_ORD_NODE_READMETER= 0x10  	
	,APP_RF_ORD_NODE_READMETER_ACK  	
	,APP_RF_ORD_NODE_REG                //采集器注册
	,APP_RF_ORD_NODE_REG_ACK  	
//	,APP_RF_ORD_NODE_RELAY_REG		    //采集器中继注册
//	,APP_RF_ORD_NODE_RELAY_REG_ACK  	
	,APP_RF_ORD_NODE_METER_REG		    //采集器中继注册
	,APP_RF_ORD_NODE_METER_REG_ACK  	
	,APP_RF_ORD_RM_WIRED	= 0x16		//有线表计抄表命令
	,APP_RF_ORD_RM_WIRED_ACK  	

	,APP_RF_ORD_NODE_TIME_ADJST			//18
	,APP_RF_ORD_TIME_ADJST_RELY_ACK  	
	,APP_RF_ORD_NODE_VALVE				//集中器直接阀门控制命令解析
	,APP_RF_ORD_VALVE_RELAY_ACK  			
	,APP_RF_ORD_RM_RELAY	 =0x1C		//采集块抄表	
	,APP_RF_ORD_RM_RELAY_ACK	

//表计 0xA0	
	,APP_RF_ORD_READMETER = 0xA0 		
	,APP_RF_ORD_READMETER_ACK  	
	,APP_RF_ORD_REG_NODE
	,APP_RF_ORD_REG_NODE_ACK  	
	,APP_RF_ORD_A4	
	,APP_RF_ORD_A5  	
	,APP_RF_ORD_TIME_ADJST			//a6--对时
	,APP_RF_ORD_A9  	
	,APP_RF_ORD_VALVE=0xA8			//1A阀门开关控制
	,APP_RF_ORD_VALVE_ACK	
	,APP_RF_ORD_VALVE_IS_PERMIT=0xAA	//阀门控制字命令
	,APP_RF_ORD_VALVE_IS_PERMIT_ACK	
	
//P2P 操作
	,APP_RF_ORD_DIR_WORK_W = 0xD0 		//工作时间
	,APP_RF_ORD_DIR_WORK_W_ACK       	//应答	
	,APP_RF_ORD_DIR_WORK_R   	 		//读取
	,APP_RF_ORD_DIR_WORK_R_ACK       	//应答	
	
	,APP_RF_ORD_DIR_ADDR_W = 0xE0 		//设置地址
	,APP_RF_ORD_DIR_ADDR_W_ACK       	//应答	
	,APP_RF_ORD_DIR_ADDR_R      		//查询地址
	,APP_RF_ORD_DIR_ADDR_R_ACK       	//	
	,APP_RF_ORD_DIR_PARA_W      		//rf 参数设置 FREQ SP,BW,
	,APP_RF_ORD_DIR_PARA_W_ACK       	//	
	,APP_RF_ORD_DIR_PARA_R      		//rf 参数读取 FREQ SP,BW,
	,APP_RF_ORD_DIR_PARA_R_ACK       	//	
 	,APP_RF_ORD_DIR_STATE_W      		//设置表计状态参数
	,APP_RF_ORD_DIR_STATE_W_ACK       	//	
 	,APP_RF_ORD_DIR_STATE_R      		//读取表计状态参数
	,APP_RF_ORD_DIR_STATE_R_ACK       	//	
 	,APP_RF_ORD_DIR_BASEDATA_W      	//设置表计基数
	,APP_RF_ORD_DIR_BASEDATA_W_ACK      //	
 	,APP_RF_ORD_DIR_BASEDATA_R      	//读取表计数据
	,APP_RF_ORD_DIR_BASEDATA_R_ACK      //	
 	,APP_RF_ORD_DIR_VALVE_W      	    //设置控制阀动作指令
	,APP_RF_ORD_DIR_VALVE_W_ACK         //	
 	,APP_RF_ORD_DIR_METERDATA_C      	//表计数据区初始化
	,APP_RF_ORD_DIR_METERDATA_C_ACK     //	
 	,APP_RF_ORD_DIR_METERPARA_C      	//表计参数区初始化
	,APP_RF_ORD_DIR_METERPARA_C_ACK     //	
 	,APP_RF_ORD_DIR_TIME_ADJUST      	//对时命令
	,APP_RF_ORD_DIR_TIME_ADJUST_ACK     //	
 	,APP_RF_ORD_DIR_TIME_R      	    //读取时间
	,APP_RF_ORD_DIR_TIME_R_ACK          //	
	,APP_RF_ORD_DIR_REG_R   	 		//读取注册信息
	,APP_RF_ORD_DIR_REG_R_ACK       	//应答	
	
	
}__App_NL_RF_ORDDef ;



/* Private typedef----------------------------------------------------------------------------*/
typedef enum{ ERROR_SEND ,NORMAL_SEND , SEND_DATA,NO_SEND_DATA}__App_Enum_Echo_Type; 

/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

/**********************************************************************************************
* Description       : 路由协议 主程序
* Input             : pBuff：数据缓冲 Len--长度
* Output            : none
***********************************************************************************************/
__App_Enum_Echo_Type App_Agr_RF_Process(INT8U  *pBuff,INT8U Len);

__App_Task_State App_Agr_Rf_Main(void );

//LP时初始化RF参数
void App_SX1276_Free_Config(void);


#endif  // __App_Agr_RF__H__

