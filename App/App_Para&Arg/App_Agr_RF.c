/*****************************************Copyright(C)******************************************
* FileName			: App_Agr_RF.c
* Author			: 
* Date First Issued	: 
* Version		: V1.0
* History		: 
* //2008		: V
* Description		: 
*-----------------------------------------------------------------------------------------------

***********************************************************************************************/
/* Includes-----------------------------------------------------------------------------------*/
#include "bsp_iic.h"
//#include "LoRa.h"
#include "sx1276-LoRa.h"
#include "sx1276.h"
#include "bsp_rtc.h"
#include "Bsp_SX127X.h"
#include "bsp_SPI.h"

#include "App_Agr_RF.h"
#include "App_Agr.h"
#include "App_Agr_MakeFrame.h"
#include "App_485_Interface.h"
#include "App_MemoryConfig.h"
#include "convertDataInterface.h"
#include "bsp_led.h"
#include "em_emu.h"
#include "sysTick.h"



/* Private define-----------------------------------------------------------------------------*/
#define APP_ROUTE_LEVES		4//最大路由级数

#define APP_NODE_ADDR_LEN		7//地址长度
#define APP_NODE_REGID_LEN		1//注册码ID长度
#define APP_SOURCE_REGID_LEN	1//注册集中器ID长度

#define APP_AGR_CAD_CYCELE_TIME		3  //uint second  CAD侦测周期

//#define RF_SEND_TYPE_CELL_TO_MASTER   1
//#define RF_SEND_TYPE_CELL_TO_METER    2
//#define RF_SEND_TYPE_CELL_TO_NODE     3


#define APP_RF_DEFAULT_TIME			  15


/* Private typedef----------------------------------------------------------------------------*/
typedef enum 
{
	RF_SEND_TYPE_CELL_TO_MASTER = 1 ,RF_SEND_TYPE_CELL_TO_METER,RF_SEND_TYPE_CELL_TO_NODE 
}__App_Rf_SendTypeEnum;


/*************采集器帧格式结构体**********/
__packed typedef struct __App_Rf_Frame__
{
	INT8U StartChar;			//帧头28H
	INT8U Cmd;			  	  	//控制码
	INT8U DataLen;				//数据域长度
	INT8U Step;				    //无线路由等级
	INT8U SouceAddrID;		    //集中器地址
	INT8U *pRx;			      	//数据区
	__App_Rf_SendTypeEnum  SendEnum ;
}__App_Rf_Frame;





__packed typedef struct __App_Rf_Para_Table__
{
	INT8U  SouceAddrID ; 				//主模块的ID号
	INT8U  DesAddrID	;   			//注册的ID号
	INT8U  NodeAddr[APP_NODE_ADDR_LEN]; //采集器地址
	INT8U  WorkStartTime ;				//启动时间
	INT8U  WorkEndTime ;
	
	INT8U RF_Rx_Channal  ;	//收的信道号 
	INT8U RF_Tx_Channal  ;	//发的信道号 
	INT8U RF_Rx_SF  ;		//收的扩频因子
	INT8U RF_Tx_SF  ;		//发的扩频因子 
	INT8U  RF_BW     ;      //带宽
}__App_Rf_Para_Table ;



__packed typedef struct __App_RF_Table__
{
	INT16U ID        ;  //ID表号 
	INT8U  DataLen	 ;  //数据长度 
	INT16U OffsetAddr;  //采集器地址
}__App_RF_Table ;


//上电时，RF默认的参数
__packed typedef struct __App_RF_PowerOn_Para__
{
	INT8U 	Channel		;
	INT16U 	Preamable	;
	INT8U  	Bw;
	INT8U  	Sf ;
	INT8U 	WindowsTime	;//有效时间窗口
	bool    FlagBool    ;
}__App_RF_PowerOn_Para ;



/* Private macro------------------------------------------------------------------------------*/

//typedef enum{ ERROR_SEND ,NORMAL_SEND , SEND_DATA,NO_SEND_DATA}__App_Enum_Echo_Type; 
typedef enum{ APP_ADDR_NULL ,APP_ADDR_VALID }__App_Enum_Addr_Type; 



const __App_RF_Table App_Rf_Para_All_Table[ ]=
{
	{0x1001	,APP_NODE_ADDR_LEN		,OFFSETOF(__App_Rf_Para_Table,NodeAddr)} //地址长度
	,{0x1002,APP_NODE_REGID_LEN		,OFFSETOF(__App_Rf_Para_Table,DesAddrID)}	
	,{0x1003,APP_SOURCE_REGID_LEN	,OFFSETOF(__App_Rf_Para_Table,SouceAddrID)}
	,{0x1004,1						,OFFSETOF(__App_Rf_Para_Table,RF_Rx_Channal)} 
	,{0x1005,1						,OFFSETOF(__App_Rf_Para_Table,RF_Tx_Channal)}	
	,{0x1006,1						,OFFSETOF(__App_Rf_Para_Table,RF_Rx_SF)}
	,{0x1007,1						,OFFSETOF(__App_Rf_Para_Table,RF_Tx_SF)}  
	,{0x1008,1						,OFFSETOF(__App_Rf_Para_Table,RF_BW)}	
	,{0x1009,1						,OFFSETOF(__App_Rf_Para_Table,RF_BW)}	
	,{0x100A,5						,OFFSETOF(__App_Rf_Para_Table,RF_Rx_Channal) } 
	,{0xffff,0						,0xffff										 }
};



/* Private variables--------------------------------------------------------------------------*/
//上电时RF参数
__App_RF_PowerOn_Para App_RF_PowerOnConfig=
{
	14
	,8
	,7
	,10  
	,APP_RF_DEFAULT_TIME   
    ,TRUE	

};

__App_Rf_Para_Table App_Node_RF_Inform;

volatile INT8U App_Cad_Flag = 0 ;	//周期时间到了，值1
volatile INT8U	App_Led_Rf_Time = 0 ; //LED显示时间
INT8U App_Agr_Rf_WeekFlag = 0 ;       //唤醒标志
tRFProcessReturnCodes App_Agr_Rf_State  ; //规约中的RF状态



/* Private function prototypes----------------------------------------------------------------*/
void SX1276CADCyclePro(void);
void LoRaSetStartSend(const void *buff, uint16_t len, uint8_t Type);
INT8U OnMaster( void );
void App_SX1276_PowerOn_ParaInit(void);

//主循环调用 同时不能进入低功耗
void App_Agr_Rf_Led(void);

/* Private functions--------------------------------------------------------------------------*/


bool App_Get_PowerOn_Rf_State(void)
{
	return App_RF_PowerOnConfig.FlagBool ;
}

//上电时需要初始化，从能量角度采用这种方式
void App_RF_GlobeInit(void)
{
	INT8U workInter ;
	Bsp_EEPROM_Read(APP_NODE_PARA_ADDR,(INT8U*)&App_Node_RF_Inform,sizeof(App_Node_RF_Inform));
	if(((!App_Node_RF_Inform.WorkStartTime)&&(!App_Node_RF_Inform.WorkEndTime))	\
		||(App_Node_RF_Inform.WorkStartTime > 0x23) ||(App_Node_RF_Inform.WorkEndTime > 0x23)	\
			||(App_Node_RF_Inform.WorkStartTime > 0x23) ||(App_Node_RF_Inform.WorkEndTime > 0x23))
	{
		App_Node_RF_Inform.WorkStartTime = 0x9 ;
		App_Node_RF_Inform.WorkEndTime   = 0x12;
	}
	workInter = ByteBcdToHex(App_Node_RF_Inform.WorkEndTime);
	workInter -= ByteBcdToHex(App_Node_RF_Inform.WorkStartTime);
	if(workInter > 3)
	{
		App_Node_RF_Inform.WorkEndTime = ByteBcdToHex(App_Node_RF_Inform.WorkEndTime)+2;
		if(App_Node_RF_Inform.WorkEndTime > 23)
		{
			App_Node_RF_Inform.WorkEndTime = 23;
		}
		App_Node_RF_Inform.WorkEndTime = ByteHexToBcd(App_Node_RF_Inform.WorkEndTime);		
	}
	if(App_Node_RF_Inform.RF_Rx_Channal > 30)
	{
		App_Node_RF_Inform.RF_Rx_Channal = 0 ;
	}
	if(App_Node_RF_Inform.RF_Tx_Channal > 30)
	{
		App_Node_RF_Inform.RF_Tx_Channal = 0 ;
	}
////  3
//	App_Node_RF_Inform.RF_Rx_Channal = 6 ;
//	App_Node_RF_Inform.RF_Tx_Channal = 10 ;

////1
//	App_Node_RF_Inform.RF_Rx_Channal = 8 ;
//	App_Node_RF_Inform.RF_Tx_Channal = 7 ;//
	
	App_SX1276_PowerOn_ParaInit();
		
}


static INT8U  App_RF_Para_WR_Routine(INT8U Cmd,INT16U offsetAddr,INT8U*pData,INT16U Len)
{
	INT16U addr = offsetAddr + APP_NODE_PARA_ADDR ;
	if(addr > APP_NODE_PARA_ADDR + APP_NODE_PARA_LEN)
	{
		return 0;
	}
	if(NULL==pData)
	{
		return 0;
	}
	if(Cmd)
	{
		Bsp_EEPROM_Write(addr,pData,Len);//2
	}
	else
	{
		Bsp_EEPROM_Read(addr,pData,Len);
	}
	return 1 ;
}

//参数读写接口 Cmd-1-w /0-r
static INT8U  App_RF_Para_WR_Func(INT8U Cmd,INT16U ID,INT8U*pData,INT16U Len)
{
	INT8U i,amount;
	amount = sizeof(App_Rf_Para_All_Table)/sizeof(__App_RF_Table);
	for(i = 0 ; i < amount ;i++)
	{
		if((ID==App_Rf_Para_All_Table[i].ID)&&(0xffff!=App_Rf_Para_All_Table[i].ID))
		{
			const __App_RF_Table *pApp_RF_Table_Index ;
			pApp_RF_Table_Index = &App_Rf_Para_All_Table[i];
			return App_RF_Para_WR_Routine(Cmd,pApp_RF_Table_Index->OffsetAddr,pData,Len);
		}
	}
	return 0 ;
}

INT8U InitiativeRegisterFlag = 0x00;        //主动注册标志 

//转化为0.1的倍数
extern INT8U App_Batt_BinData(void);

/*************帧组装**********/
__App_Enum_Echo_Type App_Rf_MakeFramePackage(__App_Rf_Frame *pTxFrame)
{
	INT8U *pSend = App_Agr_Buff_Tx;
	INT16U crc16;
	if((pTxFrame->DataLen > sizeof(App_Agr_Buff_Tx))||(pTxFrame->DataLen < 7))
	{
		return NO_SEND_DATA ;
	}
	*pSend++ = pTxFrame->StartChar ;     	
	*pSend++ = pTxFrame->Cmd     ;     	
	*pSend++ = pTxFrame->DataLen   ;//这里长度怎么弄好呢？？？	
	*pSend++ = pTxFrame->Step      ;     	
	*pSend++ = pTxFrame->SouceAddrID ;  
	memcpy(pSend,pTxFrame->pRx,pTxFrame->DataLen-7);//
	pSend+= pTxFrame->DataLen-7 ; 	
	//crc
	pSend = App_Agr_Buff_Tx ;
	crc16 = Crc16_IBM(pSend,pTxFrame->DataLen-2);
	App_Agr_Buff_Tx[pTxFrame->DataLen-2] = crc16&0xff;
	App_Agr_Buff_Tx[pTxFrame->DataLen-1] = crc16>>8;
	
	LoRaSetStartSend(App_Agr_Buff_Tx,pTxFrame->DataLen,pTxFrame->SendEnum);//发送数据
	return SEND_DATA;
}

///**********************************************************************************************
//* Description       : App_IsCollectorAddID
//* Input             : pApp_Rf_Frame 参见 __App_Rf_Frame 
//* Output            : NO_SEND_DATA  /SEND_DATA
//* Contributor       : pentral
//* Date First Issued : 2015/09/20
//***********************************************************************************************/
//__App_Enum_Addr_Type App_IsCollectorAddID(__App_Rf_Frame *pApp_Rf_Frame )//INT8U Temp,INT8U Temp1,INT8U *Temp2,INT8U TempS0,INT8U Controlcode)
//{
//	INT8U stepAmount,stepIndex;
//	INT8U desAddrIndex = 0 ;
////	INT8U ComAddrBuf[];
//	if(App_Node_RF_Inform.SouceAddrID != pApp_Rf_Frame->SouceAddrID) //集中器地址判断
//	{
//		return APP_ADDR_NULL; 	   
//	}
//	stepAmount = ((pApp_Rf_Frame->Step)>> 0x03) & 0x07;       //路由等级数
//	stepIndex = pApp_Rf_Frame->Step & 0x07;                 //所在等级数

////	memcpy(Tempcommdata,Temp2,sizeof(Tempcommdata));
//	if(stepAmount < stepIndex)
//	{
//		return APP_ADDR_NULL; 
//    }
//	// 计算出目标地址位置
//	desAddrIndex = stepAmount - stepIndex ;
//    if((APP_RF_ORD_NODE_REG_ACK == pApp_Rf_Frame->Cmd)||(APP_RF_ORD_NODE_REG ==  pApp_Rf_Frame->Cmd)||(APP_RF_ORD_RM_WIRED_ACK ==  pApp_Rf_Frame->Cmd)||(0x31 ==  pApp_Rf_Frame->Cmd))   
//    {
//		if(stepAmount == stepIndex)
//		{
//			return APP_ADDR_NULL; 	
//		}
//		desAddrIndex -= 0x01;
//	}	
//	if(App_Node_RF_Inform.DesAddrID != pApp_Rf_Frame->pRx[desAddrIndex])               //采集器目标ID
//	{	
//		return APP_ADDR_NULL; 	
//	}
//	return APP_ADDR_VALID;	
//}

/**********************************************************************************************
* Description       : App_IsCollectorAddID
* Input             : pApp_Rf_Frame 参见 __App_Rf_Frame 
* Output            : NO_SEND_DATA  /SEND_DATA
* Contributor       : pentral
* Date First Issued : 2015/09/20
***********************************************************************************************/
__App_Enum_Addr_Type App_IsCollectorAddReg(INT8U *pRegAddr,INT8U Len )
{
	if(memcmp(App_Node_RF_Inform.NodeAddr,pRegAddr,sizeof(App_Node_RF_Inform.NodeAddr)))
	{
		return APP_ADDR_NULL; 			
	}
	return APP_ADDR_VALID;	
}


INT32U xxxTimeRf = 0xfff;
__App_Enum_Echo_Type  App_RF_PraseBuff(__App_Rf_Frame *pApp_Rf_Frame)
{			
	INT8U stepAmount = 0x00;       //路由等级数
	INT8U stepIndex = 0x00;        //所在等级数
	INT8U tRf_Rssi,tRf_Snr ;
	__App_Enum_Addr_Type addrTpye ;
	INT8U rx_SourceAddrID;
	INT8U rx_DesAddrID ;
	INT16U tmpID = 0;

//	/*获取RSSI & SNR 值 */
//	tRf_Rssi = SX1276LoRaGetPacketRssi();
//	tRf_Snr  = SX1276LoRaGetPacketSnr();

	//设置和查询部分 P-P
	if(pApp_Rf_Frame->Cmd >= APP_RF_ORD_DIR_WORK_W)
	{
		INT8U  cmd = 0 ;
		INT8U  *pData;
		INT8U  len = 0;
		INT8U  boardAddr[APP_NODE_ADDR_LEN];
		/*判断SO CO DO 是否为 0xff 0xff 0xff*/
		//28 E0 L FF FF FF 08 00 00 00 00 00 00 CRC 第一个0XFF在step
		if((0xff!=pApp_Rf_Frame->SouceAddrID)||(0xff!=*pApp_Rf_Frame->pRx))
		{
			return NO_SEND_DATA;
				
		}
		pData = pApp_Rf_Frame->pRx+1; //地址
		if(APP_RF_ORD_DIR_ADDR_R ==pApp_Rf_Frame->Cmd)
		{
			/*这里最好能有硬件来配合 还有待于确认 0926*/
			memset(boardAddr,0xaa,sizeof(boardAddr));
			if(memcmp(boardAddr,pData,sizeof(boardAddr)))
			{
				return NO_SEND_DATA;
			}		
			memcpy(pData,App_Node_RF_Inform.NodeAddr,sizeof(App_Node_RF_Inform.NodeAddr));
			tmpID = 0x1001 ;
			len   = APP_NODE_ADDR_LEN ;
		}
		else if(APP_RF_ORD_DIR_ADDR_W ==pApp_Rf_Frame->Cmd)
		{
			memcpy(App_Node_RF_Inform.NodeAddr,pData,sizeof(App_Node_RF_Inform.NodeAddr));	
			tmpID = 0x1001 ;
			len   = APP_NODE_ADDR_LEN ;
			cmd   = 1 ;
		}		
		else 
		{
			//是否本采集器
			if(memcmp(pData,App_Node_RF_Inform.NodeAddr,sizeof(App_Node_RF_Inform.NodeAddr)))
			{
				return NO_SEND_DATA;
			}
			pData+=sizeof(App_Node_RF_Inform.NodeAddr) ;//数据内容
			if(APP_RF_ORD_DIR_PARA_W ==pApp_Rf_Frame->Cmd)
			{
//收频率（1字节）      无符号HEX
//发频率（1字节）      无符号HEX
//收扩频因子（1字节）  无符号HEX 
//发扩频因子（1字节）  无符号HEX 
//带宽 （1字节）       无符号HEX
				App_Node_RF_Inform.RF_Rx_Channal = *(pData+0);
				App_Node_RF_Inform.RF_Tx_Channal = *(pData+1);
				App_Node_RF_Inform.RF_Rx_SF      = *(pData+2);
				App_Node_RF_Inform.RF_Tx_SF      = *(pData+3);
				App_Node_RF_Inform.RF_BW         = *(pData+4);
				tmpID = 0x100A ;
				len   = 5 ;

				cmd   = 1 ;

			}	
			else if(APP_RF_ORD_DIR_PARA_R ==pApp_Rf_Frame->Cmd)
			{
				*pData++ = App_Node_RF_Inform.RF_Rx_Channal ;
				*pData++ = App_Node_RF_Inform.RF_Tx_Channal ;
				*pData++ = App_Node_RF_Inform.RF_Rx_SF      ;
				*pData++ = App_Node_RF_Inform.RF_Tx_SF      ;
				*pData++ = App_Node_RF_Inform.RF_BW         ;
				pApp_Rf_Frame->DataLen+=5 ;
				tmpID = 0x100A ;
				len   = 5 ;
			}
			//时间
			else if(APP_RF_ORD_DIR_TIME_ADJUST ==pApp_Rf_Frame->Cmd)
			{
				BSP_RTCSetTime((_BSPRTC_TIME*)pData);
				pApp_Rf_Frame->DataLen-=6 ;  //回复时去掉时间
			}	
			else if(APP_RF_ORD_DIR_TIME_R ==pApp_Rf_Frame->Cmd)
			{
				BSP_RTCGetTime((_BSPRTC_TIME*)pData);
				pApp_Rf_Frame->DataLen+=6 ;  //回复时去掉时间
			}		
			//查询注册编码
			else if(APP_RF_ORD_DIR_REG_R ==pApp_Rf_Frame->Cmd)
			{
				*pData++ = App_Node_RF_Inform.SouceAddrID ;
				*pData++ = App_Node_RF_Inform.DesAddrID ;	
				memset(pData,0,10);
				pApp_Rf_Frame->DataLen+=12 ;  //回复时去掉时间
			}	
			
			//para init
			else if(APP_RF_ORD_DIR_METERPARA_C ==pApp_Rf_Frame->Cmd)
			{

			}	
			//data init
			else if(APP_RF_ORD_DIR_METERDATA_C ==pApp_Rf_Frame->Cmd)
			{

			}	
			//work time set 
			else if(APP_RF_ORD_DIR_WORK_W ==pApp_Rf_Frame->Cmd)
			{
				INT8U workInter = 0 ;
				App_Node_RF_Inform.WorkStartTime = *pData++;
				App_Node_RF_Inform.WorkEndTime   = *pData++;
				if(((!App_Node_RF_Inform.WorkStartTime)&&(!App_Node_RF_Inform.WorkEndTime))	\
					||(App_Node_RF_Inform.WorkStartTime > 0x23) ||(App_Node_RF_Inform.WorkEndTime > 0x23)	\
						||(App_Node_RF_Inform.WorkStartTime > 0x23) ||(App_Node_RF_Inform.WorkEndTime > 0x23))
				{
					App_Node_RF_Inform.WorkStartTime = 0x9 ;
					App_Node_RF_Inform.WorkEndTime   = 0x12;
				}
				workInter = ByteBcdToHex(App_Node_RF_Inform.WorkEndTime);
				workInter -= ByteBcdToHex(App_Node_RF_Inform.WorkStartTime);
				if(workInter > 3)
				{
					App_Node_RF_Inform.WorkEndTime = ByteBcdToHex(App_Node_RF_Inform.WorkEndTime)+2;
					if(App_Node_RF_Inform.WorkEndTime > 23)
					{
						App_Node_RF_Inform.WorkEndTime = 23;
					}
					App_Node_RF_Inform.WorkEndTime = ByteHexToBcd(App_Node_RF_Inform.WorkEndTime);		
				}			
			}	
			else if(APP_RF_ORD_DIR_WORK_R ==pApp_Rf_Frame->Cmd)
			{
				*pData++ = App_Node_RF_Inform.WorkStartTime ;
				*pData++ = App_Node_RF_Inform.WorkEndTime   ;
				
				pApp_Rf_Frame->DataLen+=2 ;  //
			}	
			else
			{
				return NO_SEND_DATA;				
			}
		}
//		cmd = pApp_Rf_Frame->Cmd&0x03 ;
		//从功耗的角度，从RAM中读取比较合适
		if(cmd)//
		{
			App_RF_Para_WR_Func(1,tmpID,pData,len);
		}
//		else
//		{
//			App_RF_Para_WR_Func(0,tmpID,pData,len);
//			
//		}
		if(App_RF_PowerOnConfig.FlagBool)
		{
			App_RF_PowerOnConfig.WindowsTime = APP_RF_DEFAULT_TIME ;	
		}

		//封装发送
		pApp_Rf_Frame->Cmd++ ;
		pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_MASTER ;
		App_Rf_MakeFramePackage(pApp_Rf_Frame);
		return NORMAL_SEND; 	
	}
	
	//路由部分
	stepAmount = ((pApp_Rf_Frame->Step)>> 0x03) & 0x07;       //路由等级数
	stepIndex = pApp_Rf_Frame->Step & 0x07;                   //所在等级数
	if((stepAmount > 0x04) || (!stepAmount)||(stepIndex > 0x04) || (!stepIndex)||(stepAmount <stepIndex))           //级数最大为4级
	{
		return NO_SEND_DATA; 	
	}
	rx_SourceAddrID = pApp_Rf_Frame->SouceAddrID ;    //集中器ID
	rx_DesAddrID = pApp_Rf_Frame->pRx[stepAmount-stepIndex];
	//首先处理转发的
	if(stepIndex >1)
	{
		if((rx_DesAddrID!=App_Node_RF_Inform.DesAddrID) ||(rx_SourceAddrID != App_Node_RF_Inform.SouceAddrID))
		{
			return NO_SEND_DATA; 	   					
		}
		if(pApp_Rf_Frame->Cmd&1)
		{
			pApp_Rf_Frame->Step++;				//路由级数减1				
		}
		else
		{
			pApp_Rf_Frame->Step--;				//路由级数减1				
		}
		pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_NODE ;//RF_SEND_TYPE_CELL_TO_MASTER ;

	}
	else //本采集器的处理 
	{
		if((APP_RF_ORD_NODE_REG!=pApp_Rf_Frame->Cmd)&&(APP_RF_ORD_REG_FORCE!=pApp_Rf_Frame->Cmd))
		{
			if(pApp_Rf_Frame->Cmd&1)
			{
				rx_DesAddrID = pApp_Rf_Frame->pRx[stepIndex-1];
				pApp_Rf_Frame->Step++ ;
			}
			if((rx_DesAddrID!=App_Node_RF_Inform.DesAddrID) ||(rx_SourceAddrID != App_Node_RF_Inform.SouceAddrID))
			{
				return NO_SEND_DATA; 	   					
			}						
		}
		switch(pApp_Rf_Frame->Cmd)	
		{
			case APP_RF_ORD_REG_FORCE:        //手持器触发表计主动注册解析
				
				break;
			case APP_RF_ORD_NODE_READMETER:        //集中器直接抄表命令解析//集中器通过采集器抄表解析 RF
//			case APP_RF_ORD_NODE_REG:              //采集器中继注册 
			case APP_RF_ORD_NODE_TIME_ADJST:       //集中器直接对时命令解析 
			case APP_RF_ORD_NODE_VALVE:		       //集中器直接阀门控制命令解析//集中器通过采集器阀门控制解析   

				break ;
			case APP_RF_ORD_RM_WIRED :			   //抄表有线
				if(1)
				{
					INT8U *pRmeter;
					INT8U portCh ;
					INT8U rmLen ,headLen;
					portCh =  *(pApp_Rf_Frame->pRx+stepAmount-stepIndex+1) ;  //ch
					pRmeter = pApp_Rf_Frame->pRx+stepAmount-stepIndex+1 + 2 ; //2000002a8 + ch(2)
					headLen = OFFSETOF(__App_Rf_Frame,pRx) ;
					headLen += stepAmount-stepIndex+2+2+1 ;                   //desID+ch+crc
					rmLen  = pApp_Rf_Frame->DataLen - headLen;                //ch-2    //stepAmount-2
					

					if(!App_ReadMeter_Main(pRmeter,&rmLen,portCh))
					{
						return NO_SEND_DATA ;
					}
					//去掉 ch
					memmove(pApp_Rf_Frame->pRx+stepAmount-stepIndex+1,pRmeter,rmLen);
					pApp_Rf_Frame->DataLen = OFFSETOF(__App_Rf_Frame,pRx)+stepAmount-stepIndex+rmLen+2+2-1 ; //add buff crc len -ch
					pApp_Rf_Frame->Cmd++ ;	
				}			
				break;
			case APP_RF_ORD_NODE_REG:        //采集器注册
				/*---------------judge meter address-------------------------*/		
				if(0x01 == stepIndex)	//对本采集器注册
				{				
					INT8U *pNodeAddr ;
					INT8U *pDataAddBuffData; 
					if(0x01 == stepAmount)	 	
					{					
						pNodeAddr = pApp_Rf_Frame->pRx+1 ;
						rx_DesAddrID    = *pApp_Rf_Frame->pRx		    ; //注册的采集器ID
						
					}	
					else  //通过路由来的
					{
						rx_DesAddrID    = pApp_Rf_Frame->pRx[stepAmount-stepIndex]; //注册的采集器ID
						pNodeAddr = pApp_Rf_Frame->pRx+stepAmount-stepIndex+1 ;
					}					
					
					addrTpye = App_IsCollectorAddReg(pNodeAddr,7) ;  //是否对本采集器注册
					if(APP_ADDR_NULL == addrTpye)   //判断链路上的采集器ID号
					{
						return NO_SEND_DATA; 	   
					}
					App_Node_RF_Inform.SouceAddrID = rx_SourceAddrID;		//绑定的集中器ID 
					App_Node_RF_Inform.DesAddrID = rx_DesAddrID     ;		//注册的采集器ID  
					pDataAddBuffData= pNodeAddr + 7 ;     
					*pDataAddBuffData++ = 0;        //ST
					*pDataAddBuffData++ = tRf_Rssi; //信号强度
					*pDataAddBuffData++ = tRf_Snr;  //噪声比
					*pDataAddBuffData++ = 0x00;    	//最佳采集器
					pApp_Rf_Frame->DataLen =OFFSETOF(__App_Rf_Frame,pRx)+1+7+4+2+(stepAmount-stepIndex);    //regid+REALY+ add +inform+crc
					pApp_Rf_Frame->Cmd++ ;
					/*保存参数*/
					tmpID = 0x1002 ;
					App_RF_Para_WR_Func(1,tmpID,&App_Node_RF_Inform.DesAddrID,1);
					tmpID = 0x1003 ;
					App_RF_Para_WR_Func(1,tmpID,&App_Node_RF_Inform.SouceAddrID,1);
					
					
				}
				break;	
			case APP_RF_ORD_NODE_REG_ACK:      //采集器监听到的帧
			case APP_RF_ORD_NODE_METER_REG: 
			case APP_RF_ORD_RM_WIRED_ACK:				
				break ;
//			case 0x31:
	//		case 0x17:			
	//		    if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))  //判断链路上的采集器ID号
	//			{
	//				return NO_SEND_DATA; 
	//			}
	//            (buf->stepn) += 0x01;                //回复的路由级数
	//            AutomaticTransferFlag = 0xAA;            //不是最终一级 回复帧特殊处理			
	//		    break;	
	//        case 0x16:                                        //有线表计操作
	//			//集中器地址判断
	//			if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))   //判断链路上的采集器ID号
	//            {
	//				return NO_SEND_DATA; 	   
	//            }
	//			if(0x01 == Tempstepn1)	
	//            {				
	//                //有线抄表
	//				memset(WaitRFSendDataBuff,0x00,sizeof(WaitRFSendDataBuff));
	//				memcpy(WaitRFSendDataBuff,(struct CommFormat *)buf,sizeof(WaitRFSendDataBuff));  //整条帧赋值	
	//				/**************UART通信*******************/
	//			     BSP_UART_Write(UART2_PORT,WaitRFSendDataBuff,buf->datalength);			
	//				/**************UART通信*******************/
	//				return NO_SEND_DATA;
	//            }
	//            else
	//            {
	//				(buf->stepn)--;                          //路由级数减1
	//				AutomaticTransferFlag = 0xAA;            //不是最终一级 回复帧特殊处理
	//            }	
	//            break;			
	//		case 0x30:
	//			//集中器地址判断
	//			if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))   //判断链路上的采集器ID号
	//            {
	//				return NO_SEND_DATA; 	   
	//            }
	//            if(0x01 == Tempstepn1)	
	//            {				
	//				INT8U countIndex = 0 ;
	//				INT8U tArry[0x04];
	//				INT8U rLen = 0 ;
	//				INT8U countIndex1 = 0 ;
	//				INT8U countIndex2 = 0 ;
	////				INT8U TimeOut = 0;
	//				
	//				countIndex1 = buf->commdata[Tempstepn];
	//				countIndex2 = buf->commdata[Tempstepn + 0x01];
	////			    if(countIndex1 >= countIndex2)       //起始点大于等于结束点
	////				{
	////					return NO_SEND_DATA; 	   
	////				}
	//				for(countIndex = 0x00 ; countIndex < countIndex2 ;countIndex++)
	//				{
	//						BSP_A24LC64Read(AssociatedCollectorAdd + ((countIndex1 + countIndex) * 0x04),tArry,sizeof(tArry));
	//					    rLen = sizeof(tArry) - 0x01;
	//					    memcpy(&buf->commdata[Tempstepn + 0x02 + (rLen * countIndex)],tArry,rLen);
	//						buf->datalength += rLen ;//原来的帧长度+内容长度
	//				}	
	//				buf->datalength -=1;				// -TIMEOUT						        				
	//            }
	//            else
	//            {
	//				if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))   //判断链路上的采集器ID号
	//				{
	//					return NO_SEND_DATA; 	   
	//				}
	//				(buf->stepn)--;                          //路由级数减1
	//				AutomaticTransferFlag = 0xAA;            //不是最终一级 回复帧特殊处理
	//            }

	//			break;
//			case 0xA1:                                         //接收到表计端发来的抄表数据
	//		case 0xA3: 		                                   //接收到表计端发来的注册数据
	//		    memcpy(&MeterReadApp,buf,buf->datalength);     //表计回复帧格式赋值
	//		    memcpy(&CollectorReadApp,OneWaterDataBuff,OneWaterDataBuff[0x02]);   //表计回复帧格式赋值		    
	//		    WriteAndReadMeterFlash(0x00,&MeterReadApp.meteraddS[0x02]);  //读取数据
	//		    Tempstepn = ((CollectorReadApp.stepn)>> 0x03) & 0x07;       //路由等级数
	//	        Tempstepn1 = (CollectorReadApp.stepn) & 0x07;               //所在等级数
	//		    if(!CompareMeterAddID(Tempstepn,Tempstepn1,&MeterReadApp.meteraddS[0x00],MeterReadApp.controlcode))  //判断链路上的采集器ID号
	//			{
	//				return NO_SEND_DATA; 
	//			}
	//			buf->controlcode = CollectorReadApp.controlcode;             			
	//			buf->stepn = CollectorReadApp.stepn; 	
	//			buf->meteraddS0 = CollectorReadApp.meteraddS0;
	//		    memcpy(buf->commdata, CollectorReadApp.commdata, Tempstepn + 0x01);	//采集器ID赋值表计ID	
	//			memmove((buf->commdata + Tempstepn + 0x01), MeterReadApp.commdata, (MeterReadApp.datalength - 0x08));//数据赋值
	//			buf->datalength = MeterReadApp.datalength + Tempstepn;
	//            break;
			default: 
				return NO_SEND_DATA; 	 
		}		
	
	}	
	if(1==stepAmount)
	{
		pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_MASTER ;
	}
	else
	{
		pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_NODE ;//RF_SEND_TYPE_CELL_TO_MASTER ;
	}
	if(pApp_Rf_Frame->Cmd&1)
	{
		stepAmount = ((pApp_Rf_Frame->Step)>> 0x03) & 0x07;       //路由等级数
		stepIndex = pApp_Rf_Frame->Step & 0x07;                   //所在等级数
		if(stepAmount==stepIndex)
		{
			pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_MASTER ;
		}
	}
	xxxTimeRf = 0x2000 ;
	while(xxxTimeRf>5)
	{
		xxxTimeRf--;
	}
	//封装发送
	App_Rf_MakeFramePackage(pApp_Rf_Frame);
	return NORMAL_SEND; 	
}



/**********************************************************************************************
* Description       : 路由协议 主程序
* Input             : pBuff：数据缓冲 Len--长度
* Output            : none
***********************************************************************************************/
__App_Enum_Echo_Type App_Agr_RF_Process(INT8U  *pBuff,INT8U Len)	
{	
	while(Len > 5)
	{
		if(0x28 ==*pBuff)
		{
			INT16U crc16 = 0 ;
			crc16=Crc16_IBM(pBuff,Len);
			if(!crc16)
			{
				__App_Rf_Frame tRxBuff  ;
				tRxBuff.StartChar = *pBuff++;
				tRxBuff.Cmd       = *pBuff++;
				tRxBuff.DataLen   = *pBuff++;
				tRxBuff.Step  	     = *pBuff++;
				tRxBuff.SouceAddrID  = *pBuff++;
				
				tRxBuff.pRx = pBuff ;	
			    return App_RF_PraseBuff(&tRxBuff);	
			}		
		}
		pBuff++;
		Len--;
	}
	return NO_SEND_DATA ;
}

INT8U xxxxxtIMESDFI = 0 ;
__App_Task_State App_Agr_Rf_Main(void )
{
	INT8U ret = 0;
	INT16U datasize = 0;	
	__App_Enum_Echo_Type tEcho_Type = NO_SEND_DATA ;
//	volatile _BSPRTC_TIME tTime ;
	
//	BSP_RTCGetTime(&tTime);
//	if((tTime.Hour < App_Node_RF_Inform.WorkStartTime)||(tTime.Hour >App_Node_RF_Inform.WorkEndTime))
//	{
//		return ;
//	}
	if(SysGetKeyIntFlag())
	{
		App_SX1276_PowerOn_ParaInit();
	}
	ret = OnMaster();
	if(ret == 1)
	{
		INT8U *pRec = App_Agr_Buff_Rx ;
		SX1276GetRxPacket(pRec,&datasize);		
		tEcho_Type = App_Agr_RF_Process(pRec,datasize);
//		Bsp_Led_Toggle(BSP_ENUM_LED_RUN);
		xxxxxtIMESDFI++;
	}
	ret = Bsp_Led_Is_OnState() ;
	if(NO_SEND_DATA == tEcho_Type)
	{
		
		if(!ret)
		{			
			return APP_TASK_FREE ;
		}
	}
	
	return APP_TASK_BUSY ;
}
//需要每1S 调用一次
void App_Agr_Cad_On_Cycle(void)
{
	static INT8U cadOnCycle = 0 ;
	if(TRUE ==App_RF_PowerOnConfig.FlagBool)
	{
		if(App_RF_PowerOnConfig.WindowsTime)
		{
			App_RF_PowerOnConfig.WindowsTime--;
		}
		else
		{
			App_RF_PowerOnConfig.FlagBool = FALSE ;

		}		
		return ;
	}
	if(!(cadOnCycle%APP_AGR_CAD_CYCELE_TIME)) //cad turn on cycle 
	{
		/*check state isn't cad*/
		App_Cad_Flag =1 ;

	}
	cadOnCycle++ ;
}

/////////////////////////////原先的lora.c文件 因为也是APP,所以整合过来////////////////////////
//#include "sx1276-LoRaMisc.h"
//#include "Bsp_SX127X.h"
//#include "bsp_SPI.h"
//#include "LoRa.h"
//#include "sx1276-LoRa.h"

/*! OnMaster
@brief LoRa的主节点任务
@param void
@return ret: 1 接收到完整的数据
             2 发送数据完成
             3 接收超时
             4 发送超时
             0 其它情况
*/

void App_Agr_Rf_Led_Set(tRFProcessReturnCodes Rf_State)
{
	if((RF_RX_DONE == Rf_State)||(RF_TX_DONE == Rf_State))
	{
		Bsp_Led_On(BSP_ENUM_LED_UP_LINK,5);
	}	
}


INT8U OnMaster( void )
{
	INT8U ret = 0;
	tRFProcessReturnCodes sxRet = RF_IDLE ;
	SX1276CADCyclePro( );
	sxRet = (tRFProcessReturnCodes)SX1276Process()  ;
	App_Agr_Rf_State = (tRFProcessReturnCodes)sxRet ;
	/*接收完需要等待发送*/
	if((RF_TX_TIMEOUT == sxRet)||(RF_RX_TIMEOUT == sxRet)||(RF_CHANNEL_EMPTY == sxRet)||(RF_TX_DONE == sxRet)||(RF_RX_DONE == sxRet)||(RF_IDLE == sxRet))
	{
		//LED
		App_Agr_Rf_Led_Set((tRFProcessReturnCodes)sxRet);
		
		//lp
		if(RF_IDLE == sxRet)
		{
			App_Agr_Rf_WeekFlag =  0 ;
		}
		else
		{
			App_Agr_Rf_WeekFlag = 1 ;
		}
		if((App_Agr_Rf_WeekFlag)&&(!App_RF_PowerOnConfig.FlagBool))
		{
//			INT8U rf_State ;
//			SX1276_RelatePIN_Open();
//			SPI_Initial();
			SX1276LoRaSetRFState( RFLR_STATE_IDLE );
			SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );	
			Bsp_Rf_PowerOffPinConfig();
		} 
		if(App_RF_PowerOnConfig.FlagBool)
		{
			if((RF_RX_TIMEOUT == sxRet) ||(RF_RX_DONE == sxRet)||(RF_TX_TIMEOUT == sxRet) ||(RF_TX_DONE == sxRet))
			{
				SX1276LoRaSetRFState(RFLR_STATE_RX_INIT);
			}
		}		
		if(RF_RX_DONE == sxRet)
		{
			ret = 1 ;
		}
	}
  return ret;
}


//上电时初始化RF参数
void App_SX1276_PowerOn_ParaInit(void)
{
//	uint32_t freq = BSP_1278_FREQ_BASE +  App_RF_PowerOnConfig.Channel *BSP_1278_FREQ_OFFSET ;	
	SX1276_RelatePIN_Open();
	SX1276Init();		
//	SX1276LoRaSetRFFrequency(freq);		
	App_LoraSetFreq(App_RF_PowerOnConfig.Channel);	
	SX1276LoRaSetPreambleLength(App_RF_PowerOnConfig.Preamable); 
	SX1276LoRaSetSignalBandwidth(App_RF_PowerOnConfig.Bw);
	SX1276LoRaSetSpreadingFactor(App_RF_PowerOnConfig.Sf);

	SX1276LoRaSetRFState(RFLR_STATE_RX_INIT);
	App_RF_PowerOnConfig.WindowsTime = APP_RF_DEFAULT_TIME ;
	App_RF_PowerOnConfig.FlagBool    = TRUE ;

}

//volatile INT16U App_Led_RunTime = 5;

void SX1276CADCyclePro(void)
{
	if(App_Cad_Flag)
	{
//		if(RF_IDLE == App_Agr_Rf_State)
		if(RFLR_STATE_IDLE == SX1276GetRFState())
		{
//			uint32_t freq = BSP_1278_FREQ_BASE + App_Node_RF_Inform.RF_Rx_Channal *BSP_1278_FREQ_OFFSET ;	
			SX1276_RelatePIN_Open();
			SPI_Initial();
//			SX1276LoRaSetRFFrequency(freq);		
			App_LoraSetFreq(App_Node_RF_Inform.RF_Rx_Channal);	

			SX1276LoRaSetPreambleLength(LoRaSettings.RF_Rx_Preamable_Len);  
			SX1276LoRaSetRFState(RFLR_STATE_CAD_INIT );	//RFLR_STATE_RX_INIT RFLR_STATE_CAD_INIT
			Bsp_Led_On(BSP_ENUM_LED_RUN,5);
			
		}
		App_Cad_Flag =0 ;
	}
}


/*! LoRaSetStartRec
@brief 设置开始发送
@param buff: 发送缓存
       len: 数据长度
       Type: 操作类型, 
@return void 
*/

void LoRaSetStartSend(const void *buff, uint16_t len, uint8_t Type)
{
	SX1276_RelatePIN_Open();
	SPI_Initial();
	if(!App_RF_PowerOnConfig.FlagBool)
	{
		uint8_t freqChannel = 0 ; //LoRaSettings.Channel_RF *BSP_1278_FREQ_OFFSET ;
		if(RF_SEND_TYPE_CELL_TO_MASTER!=Type)
		{
			freqChannel = App_Node_RF_Inform.RF_Rx_Channal;
			SX1276LoRaSetPreambleLength(LoRaSettings.RF_Rx_Preamable_Len);  //RFTX_LONG_PREAMBLE_MASTTOCOLL_LENGTH
		}
		else
		{
			freqChannel = App_Node_RF_Inform.RF_Tx_Channal;
			SX1276LoRaSetPreambleLength(LoRaSettings.RF_Tx_S_Master_Preamable_Len);  //RFTX_LONG_PREAMBLE_MASTTOCOLL_LENGTH
			
		}
		App_LoraSetFreq(freqChannel);	
		
	}
	SX1276SetTxPacket(buff, len );
	
}



/************************(C)COPYRIGHT 2014 能联科技*****END OF FILE****************************/
