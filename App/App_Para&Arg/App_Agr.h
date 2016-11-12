/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : App_Agr.h
* Author            : fjb
* Date First Issued : 2015/8/03
* Version           : V1.0
* Description       : 此文件专为编写规约表格而定，与规约无关的东西请不要在此定义,参数存取在AT45DB里面
*----------------------------------------历史版本信息-------------------------------------------
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
              ,GROUP_BROADCASE, TERNIMAL_ADDR,CASACDE_ADDR,SET_TER_ADDR}__ADDRTPYE;  /*地址类型*/

/* Private typedef----------------------------------------------------------------------------*/

//typedef struct __App_Para_InterfaceStr__
//{
//    INT16U afn;            /*功能码 */
//    INT16U point;          /*测量点 */
//    INT16U IDIndex;			/*源数据缓冲区,主要是传递ID号用，其他数据传递请勿用 */
//    INT8U  *pSource;       /*主要是参数查询，需要传递的一些数据*/
//	INT16U dlen;           /*目的数据长度 */
//    INT8U  *dbuff;         /*目的数据缓冲区 */
//    
//}__App_Para_InterfaceStr; 


typedef struct
{
//	_BSP_MSGID MsgID;		// 消息ID,具体看_BSP_MSGID定义
	INT16U DivNum;			// 设备号  对于抄表信息，还有测量点的含义 
	INT16U DataLen;			// 数据长度, 操作注意见以上说明	 对于抄表信息，还有数据标识的含义
	INT8U  *pData;			// 数据指针, 操作注意见以上说明
}_BSP_MESSAGE;



/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/
extern INT8U App_Agr_Buff_Rx[APP_AGR_RX_BUFFAMOUNT];
extern INT8U App_Agr_Buff_Tx[APP_AGR_TX_BUFFAMOUNT];

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

#endif  //__App_Head__H__  

