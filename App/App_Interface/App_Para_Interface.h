
#ifndef __App_Para_Interface__H__
#define __App_Para_Interface__H__
/* Includes-----------------------------------------------------------------------------------*/

#include	"Bsp_config.h " 
#include	"App_Head.h " 

/* Private define-----------------------------------------------------------------------------*/

/* Private enum----------------------------------------------------------------------------*/

/*Private typedef----------------------------------------------------------------------------*/

typedef struct __App_Para_InterfaceStr__
{
    INT16U afn;            /*������ */
    INT16U point;          /*������ */
    INT16U IDIndex;			/*Դ���ݻ�����,��Ҫ�Ǵ���ID���ã��������ݴ��������� x0c01 xc0c*/
    INT8U  *pSource;       /*��Ҫ�ǲ�����ѯ����Ҫ���ݵ�һЩ����*/
	INT16U dlen;           /*Ŀ�����ݳ��� */
    INT8U  *dbuff;         /*Ŀ�����ݻ����� */
    
}__App_Para_InterfaceStr; 


//__packed typedef struct __App_Frame_RouteStr__
//{
//	INT8U  StartChar ;
//	INT8U  Type		;
//	INT8U  AreaCode[3];
//	INT8U  Maddress;
//	INT8U  TerAddress[2];
//	
//	INT8U  CMD;			// �����������ֽ�
//	INT16U LenData;     
//	INT8U  Seq ;
//	INT8U  Dt[2] ;
//	INT8U  *pData ;     //������
//	INT16U Crc ;
//	INT8U  EndChar ;	
//	INT8U  Comm ;
//}__App_Frame_RouteStr;

__packed typedef struct __App_Frame_HeadStr__
{
	INT8U  StartChar ;
	INT16U Seq		;
	INT8U  Cmd;
	INT8U  TerAddr[8];
}__App_Frame_HeadStr;



__packed typedef struct __App_Frame_DataBuffStr__
{
	INT8U  LenData;     
	INT8U  *pData ;     //������
}__App_Frame_DataBuffStr;

__packed typedef struct __App_Frame_Str__
{
	INT8U Comm ;
	INT8U  StartChar ;
	INT16U Seq		;
	INT8U  Cmd;
	INT8U  TerAddr[APP_TER_ADDR_LEN];
	INT16U Len ;
	INT8U *pBuff ;
	INT16U CrcData;
	INT8U EndChar ;
}__App_Frame_Str;
/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/
INT16U GWPara_function(__App_Para_InterfaceStr *pdata);
void Reset_Para_All(void);
void Reset_Para_Not_Comunication(void);
/**********************************************************************************************
* Function          : ��Լ�е�д��������
* Output            : none
***********************************************************************************************/
void App_Parameter_Write(__App_Frame_Str *pFrame_Str);

/************************************************************************
 ** �������ƣ�void App_Parameter_Read
 ** ����������������ѯ����
 ** ��    ����none
*************************************************************************/
void App_Parameter_Read(__App_Frame_Str *pFrame_Str);

//��Լ�������Ϣ��ʼ��
void App_Agr_Init(void);

//��Լ�����
void TaskGuiYue(void);

//�ϵ�ʱ��Ҫ��ʼ�����������ǶȲ������ַ�ʽ
void App_RF_GlobeInit(void);


//��Ҫÿ1S ����һ��
void App_Agr_Cad_On_Cycle(void);

bool App_Get_PowerOn_Rf_State(void);

void App_Agr_Time_msCall(void);



#endif  //__App_Head__H__  

