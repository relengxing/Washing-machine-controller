/*****************************************Copyright(C)******************************************
*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName          : App_ParaTable.h
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
#ifndef __App_ParaTable__H__
#define __App_ParaTable__H__
/* Includes-----------------------------------------------------------------------------------*/

#include	"App_Head.h " 


/* Private define-----------------------------------------------------------------------------*/

//�ն˵�ַ �Զ���
#define PARA_0200_LEN	8


#define PARA_0401_LEN	1
#define PARA_0402_LEN	40	//28
#define PARA_0403_LEN	16
#define PARA_0404_LEN	15
//�·�����������
#define PARA_0405_0_LEN	9	//�漰������	����
#define PARA_0405_1_LEN	9	//�漰������	��ͨ
#define PARA_0405_LEN	(PARA_0405_0_LEN + PARA_0405_1_LEN)	//�漰������
//��ʵ�ı�������
#define PARA_1405_0_LEN	9	//�漰������	����
#define PARA_1405_1_LEN	9	//�漰������	��ͨ
#define PARA_1405_LEN	(PARA_1405_0_LEN + PARA_1405_1_LEN)	//�漰������



//#define PARA_0406_LEN	9	//ԭ��
#define PARA_0406_LEN	10	//��޸ģ��������
#define PARA_0407_LEN	4	//�˿�����
#define PARA_0408_LEN	4
#define PARA_0409_LEN	6	//�漰�����


#define PARA_040A_LEN	1	//�漰�����
#define PARA_040B_LEN	1	//�漰������
#define PARA_040C_LEN	1
#define PARA_040D_LEN	6	//��ʱ

#define PARA_0411_LEN	3	//����ʱ��
#define PARA_0422_LEN	1	//������

#define PARA_8801_LEN	6	//MAC��ַ

/* Private enum----------------------------------------------------------------------------*/


/* Private typedef----------------------------------------------------------------------------*/


//�Զ�����
__packed typedef struct
{
	INT8U ParaData_1405_0[PARA_1405_0_LEN];
	INT8U ParaData_1405_1[PARA_1405_1_LEN];
}__ParaData_1405;



//��׼��Լ
__packed typedef struct
{
	INT8U ParaData_0200[PARA_0200_LEN];
}__ParaData_0200;

//LCD ��������
__packed typedef struct
{
	INT8U ParaData_0410[4];
}__ParaData_0410;   //lcd����



__packed typedef struct
{
	INT8U ParaData_0401[PARA_0401_LEN];
}__ParaData_0401;

__packed typedef struct
{
	INT8U ParaData_0402[PARA_0402_LEN];
}__ParaData_0402;

__packed typedef struct
{
	INT8U ParaData_0403[PARA_0403_LEN];
}__ParaData_0403;

__packed typedef struct
{
	INT8U ParaData_0404[PARA_0404_LEN];
}__ParaData_0404;

__packed typedef struct
{
	INT8U ParaData_0405_0[PARA_0405_0_LEN];
	INT8U ParaData_0405_1[PARA_0405_1_LEN];
}__ParaData_0405;

__packed typedef struct
{
	INT8U ParaData_0406[PARA_0406_LEN];
}__ParaData_0406;

__packed typedef struct
{
	INT8U ParaData_0407[PARA_0407_LEN];
}__ParaData_0407;

__packed typedef struct
{
	INT8U ParaData_0408[PARA_0408_LEN];
}__ParaData_0408;

__packed typedef struct
{
	INT8U ParaData_0409[PARA_0409_LEN];
}__ParaData_0409;

__packed typedef struct
{
	INT8U ParaData_040A[PARA_040A_LEN];
}__ParaData_040A;

__packed typedef struct
{
	INT8U ParaData_040B[PARA_040B_LEN];
}__ParaData_040B;

__packed typedef struct
{
	INT8U ParaData_040C[PARA_040C_LEN];
}__ParaData_040C;

__packed typedef struct
{
	INT8U ParaData_040D[PARA_040D_LEN];
}__ParaData_040D;



__packed typedef struct
{
	INT8U ParaData_0411[PARA_0411_LEN];
}__ParaData_0411;



__packed typedef struct
{
	INT8U ParaData_8801[PARA_8801_LEN];
}__ParaData_8801;


__packed typedef struct
{
	__ParaData_0200  ParaData_0200;
	__ParaData_0410	 ParaData_0410;
	__ParaData_8801	 ParaData_8801;

}__Const_Ter_Data;



__packed typedef struct
{
	__ParaData_0402  ParaData_0402;
	__ParaData_0403  ParaData_0403;
	__ParaData_0404  ParaData_0404;	  
}__Para_Ter_Comm_Data;

__packed typedef struct
{
	__ParaData_0401  ParaData_0401;
	__ParaData_0408  ParaData_0408;
	__ParaData_040C  ParaData_040C;	
	__ParaData_0407  ParaData_0407;
	__ParaData_0411  ParaData_0411;
}__Para_Ter_Para_Data;

//__packed typedef struct
//{
//	__Para_Ter_Comm_Data  Para_Ter_Comm_Data;
//	__Para_Ter_Para_Data  Para_Ter_Para_Data;
//	
//}__Para_Ter_Data;

__packed typedef struct
{
	__ParaData_0409  ParaData_0409;
	__ParaData_040A  ParaData_040A;

}__Para_Task_Data;

__packed typedef struct
{
//	__ParaData_0405  ParaData_0405;
	__ParaData_0406  ParaData_0406;
	__ParaData_040B  ParaData_040B;
}__Para_Point_Data;


//__packed typedef struct
//{
//	__ParaData_0407  ParaData_0407;
//}__Para_Port_Data;


__packed typedef struct
{
	__ParaData_0405  ParaData_0405;
}__Para_Pw_Point;


__packed typedef struct
{
	__ParaData_1405  ParaData_1405;
}__BackUp_Para_Point;


//#define PARA_PW_POINT  (POINTAMOUNT+1)

__packed typedef struct
{
	__Const_Ter_Data	Const_Ter_Data ;
//	__Para_Ter_Data  	Para_Ter_Data  ;
	__Para_Ter_Comm_Data  Para_Ter_Comm_Data;
	__Para_Ter_Para_Data  Para_Ter_Para_Data;
	__Para_Task_Data	Para_Task_Data[TASKMAXAMOUNT]  ;
	__Para_Point_Data	Para_Point_Data[POINTAMOUNT]   ;
//	__Para_Port_Data	Para_Port_Data           	   ;
//   __Para_Pw_Point      Para_Pw_Point[PARA_PW_POINT];
//   __BackUp_Para_Point  BackUp_Para_Point[PARA_PW_POINT]; 
}__Para_ALL_Table;



/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/
//extern INT8U AreaCode[3] ; //������������ֽ�
extern INT8U TerAddress[APP_TER_ADDR_LEN]; //�ն˵�ַ

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

#endif  
