
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
    INT16U afn;            /*功能码 */
    INT16U point;          /*测量点 */
    INT16U IDIndex;			/*源数据缓冲区,主要是传递ID号用，其他数据传递请勿用 x0c01 xc0c*/
    INT8U  *pSource;       /*主要是参数查询，需要传递的一些数据*/
	INT16U dlen;           /*目的数据长度 */
    INT8U  *dbuff;         /*目的数据缓冲区 */
    
}__App_Para_InterfaceStr; 


//__packed typedef struct __App_Frame_RouteStr__
//{
//	INT8U  StartChar ;
//	INT8U  Type		;
//	INT8U  AreaCode[3];
//	INT8U  Maddress;
//	INT8U  TerAddress[2];
//	
//	INT8U  CMD;			// 控制域命令字节
//	INT16U LenData;     
//	INT8U  Seq ;
//	INT8U  Dt[2] ;
//	INT8U  *pData ;     //数据域
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
	INT8U  *pData ;     //数据域
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
* Function          : 规约中的写参数功能
* Output            : none
***********************************************************************************************/
void App_Parameter_Write(__App_Frame_Str *pFrame_Str);

/************************************************************************
 ** 函数名称：void App_Parameter_Read
 ** 功能描述：参数查询处理
 ** 输    出：none
*************************************************************************/
void App_Parameter_Read(__App_Frame_Str *pFrame_Str);

//规约包相关信息初始化
void App_Agr_Init(void);

//规约任务包
void TaskGuiYue(void);

//上电时需要初始化，从能量角度采用这种方式
void App_RF_GlobeInit(void);


//需要每1S 调用一次
void App_Agr_Cad_On_Cycle(void);

bool App_Get_PowerOn_Rf_State(void);

void App_Agr_Time_msCall(void);



#endif  //__App_Head__H__  

