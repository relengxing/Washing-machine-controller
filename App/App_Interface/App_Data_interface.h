//App_Data_interface.h
#ifndef __App_Data_interface__H__
#define __App_Data_interface__H__
/* Includes-----------------------------------------------------------------------------------*/

#include "Bsp_config.h " 
#include "App_Para_Interface.h"



/* Private define-----------------------------------------------------------------------------*/
/* Private enum----------------------------------------------------------------------------*/

/*Private typedef----------------------------------------------------------------------------*/

/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/
/**********************************************************************************************
* Description       : 由于对不同的命令，做数据长度处理 
* Input             : pdata ，参见InterfaceStruct  测量点从1开始 
*					  rateLen:标准标度 为一类数据OneData_Sampling_Table和DLT_OneData_Sampling_Table规定长度
* Output            : none
* Contributor       : fangjb
* Date First Issued : 2010.05.11
* Date Secord Issued :2012.09.05	暂时不考虑曲线问题
***********************************************************************************************/
INT16U GWFirstData_function(__App_Para_InterfaceStr *pdata);
INT16U Reset_ONEDATA_Function(void);

#endif  //  

