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
* Description       : ���ڶԲ�ͬ����������ݳ��ȴ��� 
* Input             : pdata ���μ�InterfaceStruct  �������1��ʼ 
*					  rateLen:��׼��� Ϊһ������OneData_Sampling_Table��DLT_OneData_Sampling_Table�涨����
* Output            : none
* Contributor       : fangjb
* Date First Issued : 2010.05.11
* Date Secord Issued :2012.09.05	��ʱ��������������
***********************************************************************************************/
INT16U GWFirstData_function(__App_Para_InterfaceStr *pdata);
INT16U Reset_ONEDATA_Function(void);

#endif  //  

