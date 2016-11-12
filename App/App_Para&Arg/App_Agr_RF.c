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
#define APP_ROUTE_LEVES		4//���·�ɼ���

#define APP_NODE_ADDR_LEN		7//��ַ����
#define APP_NODE_REGID_LEN		1//ע����ID����
#define APP_SOURCE_REGID_LEN	1//ע�Ἧ����ID����

#define APP_AGR_CAD_CYCELE_TIME		3  //uint second  CAD�������

//#define RF_SEND_TYPE_CELL_TO_MASTER   1
//#define RF_SEND_TYPE_CELL_TO_METER    2
//#define RF_SEND_TYPE_CELL_TO_NODE     3


#define APP_RF_DEFAULT_TIME			  15


/* Private typedef----------------------------------------------------------------------------*/
typedef enum 
{
	RF_SEND_TYPE_CELL_TO_MASTER = 1 ,RF_SEND_TYPE_CELL_TO_METER,RF_SEND_TYPE_CELL_TO_NODE 
}__App_Rf_SendTypeEnum;


/*************�ɼ���֡��ʽ�ṹ��**********/
__packed typedef struct __App_Rf_Frame__
{
	INT8U StartChar;			//֡ͷ28H
	INT8U Cmd;			  	  	//������
	INT8U DataLen;				//�����򳤶�
	INT8U Step;				    //����·�ɵȼ�
	INT8U SouceAddrID;		    //��������ַ
	INT8U *pRx;			      	//������
	__App_Rf_SendTypeEnum  SendEnum ;
}__App_Rf_Frame;





__packed typedef struct __App_Rf_Para_Table__
{
	INT8U  SouceAddrID ; 				//��ģ���ID��
	INT8U  DesAddrID	;   			//ע���ID��
	INT8U  NodeAddr[APP_NODE_ADDR_LEN]; //�ɼ�����ַ
	INT8U  WorkStartTime ;				//����ʱ��
	INT8U  WorkEndTime ;
	
	INT8U RF_Rx_Channal  ;	//�յ��ŵ��� 
	INT8U RF_Tx_Channal  ;	//�����ŵ��� 
	INT8U RF_Rx_SF  ;		//�յ���Ƶ����
	INT8U RF_Tx_SF  ;		//������Ƶ���� 
	INT8U  RF_BW     ;      //����
}__App_Rf_Para_Table ;



__packed typedef struct __App_RF_Table__
{
	INT16U ID        ;  //ID��� 
	INT8U  DataLen	 ;  //���ݳ��� 
	INT16U OffsetAddr;  //�ɼ�����ַ
}__App_RF_Table ;


//�ϵ�ʱ��RFĬ�ϵĲ���
__packed typedef struct __App_RF_PowerOn_Para__
{
	INT8U 	Channel		;
	INT16U 	Preamable	;
	INT8U  	Bw;
	INT8U  	Sf ;
	INT8U 	WindowsTime	;//��Чʱ�䴰��
	bool    FlagBool    ;
}__App_RF_PowerOn_Para ;



/* Private macro------------------------------------------------------------------------------*/

//typedef enum{ ERROR_SEND ,NORMAL_SEND , SEND_DATA,NO_SEND_DATA}__App_Enum_Echo_Type; 
typedef enum{ APP_ADDR_NULL ,APP_ADDR_VALID }__App_Enum_Addr_Type; 



const __App_RF_Table App_Rf_Para_All_Table[ ]=
{
	{0x1001	,APP_NODE_ADDR_LEN		,OFFSETOF(__App_Rf_Para_Table,NodeAddr)} //��ַ����
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
//�ϵ�ʱRF����
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

volatile INT8U App_Cad_Flag = 0 ;	//����ʱ�䵽�ˣ�ֵ1
volatile INT8U	App_Led_Rf_Time = 0 ; //LED��ʾʱ��
INT8U App_Agr_Rf_WeekFlag = 0 ;       //���ѱ�־
tRFProcessReturnCodes App_Agr_Rf_State  ; //��Լ�е�RF״̬



/* Private function prototypes----------------------------------------------------------------*/
void SX1276CADCyclePro(void);
void LoRaSetStartSend(const void *buff, uint16_t len, uint8_t Type);
INT8U OnMaster( void );
void App_SX1276_PowerOn_ParaInit(void);

//��ѭ������ ͬʱ���ܽ���͹���
void App_Agr_Rf_Led(void);

/* Private functions--------------------------------------------------------------------------*/


bool App_Get_PowerOn_Rf_State(void)
{
	return App_RF_PowerOnConfig.FlagBool ;
}

//�ϵ�ʱ��Ҫ��ʼ�����������ǶȲ������ַ�ʽ
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

//������д�ӿ� Cmd-1-w /0-r
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

INT8U InitiativeRegisterFlag = 0x00;        //����ע���־ 

//ת��Ϊ0.1�ı���
extern INT8U App_Batt_BinData(void);

/*************֡��װ**********/
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
	*pSend++ = pTxFrame->DataLen   ;//���ﳤ����ôŪ���أ�����	
	*pSend++ = pTxFrame->Step      ;     	
	*pSend++ = pTxFrame->SouceAddrID ;  
	memcpy(pSend,pTxFrame->pRx,pTxFrame->DataLen-7);//
	pSend+= pTxFrame->DataLen-7 ; 	
	//crc
	pSend = App_Agr_Buff_Tx ;
	crc16 = Crc16_IBM(pSend,pTxFrame->DataLen-2);
	App_Agr_Buff_Tx[pTxFrame->DataLen-2] = crc16&0xff;
	App_Agr_Buff_Tx[pTxFrame->DataLen-1] = crc16>>8;
	
	LoRaSetStartSend(App_Agr_Buff_Tx,pTxFrame->DataLen,pTxFrame->SendEnum);//��������
	return SEND_DATA;
}

///**********************************************************************************************
//* Description       : App_IsCollectorAddID
//* Input             : pApp_Rf_Frame �μ� __App_Rf_Frame 
//* Output            : NO_SEND_DATA  /SEND_DATA
//* Contributor       : pentral
//* Date First Issued : 2015/09/20
//***********************************************************************************************/
//__App_Enum_Addr_Type App_IsCollectorAddID(__App_Rf_Frame *pApp_Rf_Frame )//INT8U Temp,INT8U Temp1,INT8U *Temp2,INT8U TempS0,INT8U Controlcode)
//{
//	INT8U stepAmount,stepIndex;
//	INT8U desAddrIndex = 0 ;
////	INT8U ComAddrBuf[];
//	if(App_Node_RF_Inform.SouceAddrID != pApp_Rf_Frame->SouceAddrID) //��������ַ�ж�
//	{
//		return APP_ADDR_NULL; 	   
//	}
//	stepAmount = ((pApp_Rf_Frame->Step)>> 0x03) & 0x07;       //·�ɵȼ���
//	stepIndex = pApp_Rf_Frame->Step & 0x07;                 //���ڵȼ���

////	memcpy(Tempcommdata,Temp2,sizeof(Tempcommdata));
//	if(stepAmount < stepIndex)
//	{
//		return APP_ADDR_NULL; 
//    }
//	// �����Ŀ���ַλ��
//	desAddrIndex = stepAmount - stepIndex ;
//    if((APP_RF_ORD_NODE_REG_ACK == pApp_Rf_Frame->Cmd)||(APP_RF_ORD_NODE_REG ==  pApp_Rf_Frame->Cmd)||(APP_RF_ORD_RM_WIRED_ACK ==  pApp_Rf_Frame->Cmd)||(0x31 ==  pApp_Rf_Frame->Cmd))   
//    {
//		if(stepAmount == stepIndex)
//		{
//			return APP_ADDR_NULL; 	
//		}
//		desAddrIndex -= 0x01;
//	}	
//	if(App_Node_RF_Inform.DesAddrID != pApp_Rf_Frame->pRx[desAddrIndex])               //�ɼ���Ŀ��ID
//	{	
//		return APP_ADDR_NULL; 	
//	}
//	return APP_ADDR_VALID;	
//}

/**********************************************************************************************
* Description       : App_IsCollectorAddID
* Input             : pApp_Rf_Frame �μ� __App_Rf_Frame 
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
	INT8U stepAmount = 0x00;       //·�ɵȼ���
	INT8U stepIndex = 0x00;        //���ڵȼ���
	INT8U tRf_Rssi,tRf_Snr ;
	__App_Enum_Addr_Type addrTpye ;
	INT8U rx_SourceAddrID;
	INT8U rx_DesAddrID ;
	INT16U tmpID = 0;

//	/*��ȡRSSI & SNR ֵ */
//	tRf_Rssi = SX1276LoRaGetPacketRssi();
//	tRf_Snr  = SX1276LoRaGetPacketSnr();

	//���úͲ�ѯ���� P-P
	if(pApp_Rf_Frame->Cmd >= APP_RF_ORD_DIR_WORK_W)
	{
		INT8U  cmd = 0 ;
		INT8U  *pData;
		INT8U  len = 0;
		INT8U  boardAddr[APP_NODE_ADDR_LEN];
		/*�ж�SO CO DO �Ƿ�Ϊ 0xff 0xff 0xff*/
		//28 E0 L FF FF FF 08 00 00 00 00 00 00 CRC ��һ��0XFF��step
		if((0xff!=pApp_Rf_Frame->SouceAddrID)||(0xff!=*pApp_Rf_Frame->pRx))
		{
			return NO_SEND_DATA;
				
		}
		pData = pApp_Rf_Frame->pRx+1; //��ַ
		if(APP_RF_ORD_DIR_ADDR_R ==pApp_Rf_Frame->Cmd)
		{
			/*�����������Ӳ������� ���д���ȷ�� 0926*/
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
			//�Ƿ񱾲ɼ���
			if(memcmp(pData,App_Node_RF_Inform.NodeAddr,sizeof(App_Node_RF_Inform.NodeAddr)))
			{
				return NO_SEND_DATA;
			}
			pData+=sizeof(App_Node_RF_Inform.NodeAddr) ;//��������
			if(APP_RF_ORD_DIR_PARA_W ==pApp_Rf_Frame->Cmd)
			{
//��Ƶ�ʣ�1�ֽڣ�      �޷���HEX
//��Ƶ�ʣ�1�ֽڣ�      �޷���HEX
//����Ƶ���ӣ�1�ֽڣ�  �޷���HEX 
//����Ƶ���ӣ�1�ֽڣ�  �޷���HEX 
//���� ��1�ֽڣ�       �޷���HEX
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
			//ʱ��
			else if(APP_RF_ORD_DIR_TIME_ADJUST ==pApp_Rf_Frame->Cmd)
			{
				BSP_RTCSetTime((_BSPRTC_TIME*)pData);
				pApp_Rf_Frame->DataLen-=6 ;  //�ظ�ʱȥ��ʱ��
			}	
			else if(APP_RF_ORD_DIR_TIME_R ==pApp_Rf_Frame->Cmd)
			{
				BSP_RTCGetTime((_BSPRTC_TIME*)pData);
				pApp_Rf_Frame->DataLen+=6 ;  //�ظ�ʱȥ��ʱ��
			}		
			//��ѯע�����
			else if(APP_RF_ORD_DIR_REG_R ==pApp_Rf_Frame->Cmd)
			{
				*pData++ = App_Node_RF_Inform.SouceAddrID ;
				*pData++ = App_Node_RF_Inform.DesAddrID ;	
				memset(pData,0,10);
				pApp_Rf_Frame->DataLen+=12 ;  //�ظ�ʱȥ��ʱ��
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
		//�ӹ��ĵĽǶȣ���RAM�ж�ȡ�ȽϺ���
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

		//��װ����
		pApp_Rf_Frame->Cmd++ ;
		pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_MASTER ;
		App_Rf_MakeFramePackage(pApp_Rf_Frame);
		return NORMAL_SEND; 	
	}
	
	//·�ɲ���
	stepAmount = ((pApp_Rf_Frame->Step)>> 0x03) & 0x07;       //·�ɵȼ���
	stepIndex = pApp_Rf_Frame->Step & 0x07;                   //���ڵȼ���
	if((stepAmount > 0x04) || (!stepAmount)||(stepIndex > 0x04) || (!stepIndex)||(stepAmount <stepIndex))           //�������Ϊ4��
	{
		return NO_SEND_DATA; 	
	}
	rx_SourceAddrID = pApp_Rf_Frame->SouceAddrID ;    //������ID
	rx_DesAddrID = pApp_Rf_Frame->pRx[stepAmount-stepIndex];
	//���ȴ���ת����
	if(stepIndex >1)
	{
		if((rx_DesAddrID!=App_Node_RF_Inform.DesAddrID) ||(rx_SourceAddrID != App_Node_RF_Inform.SouceAddrID))
		{
			return NO_SEND_DATA; 	   					
		}
		if(pApp_Rf_Frame->Cmd&1)
		{
			pApp_Rf_Frame->Step++;				//·�ɼ�����1				
		}
		else
		{
			pApp_Rf_Frame->Step--;				//·�ɼ�����1				
		}
		pApp_Rf_Frame->SendEnum = RF_SEND_TYPE_CELL_TO_NODE ;//RF_SEND_TYPE_CELL_TO_MASTER ;

	}
	else //���ɼ����Ĵ��� 
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
			case APP_RF_ORD_REG_FORCE:        //�ֳ��������������ע�����
				
				break;
			case APP_RF_ORD_NODE_READMETER:        //������ֱ�ӳ����������//������ͨ���ɼ���������� RF
//			case APP_RF_ORD_NODE_REG:              //�ɼ����м�ע�� 
			case APP_RF_ORD_NODE_TIME_ADJST:       //������ֱ�Ӷ�ʱ������� 
			case APP_RF_ORD_NODE_VALVE:		       //������ֱ�ӷ��ſ����������//������ͨ���ɼ������ſ��ƽ���   

				break ;
			case APP_RF_ORD_RM_WIRED :			   //��������
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
					//ȥ�� ch
					memmove(pApp_Rf_Frame->pRx+stepAmount-stepIndex+1,pRmeter,rmLen);
					pApp_Rf_Frame->DataLen = OFFSETOF(__App_Rf_Frame,pRx)+stepAmount-stepIndex+rmLen+2+2-1 ; //add buff crc len -ch
					pApp_Rf_Frame->Cmd++ ;	
				}			
				break;
			case APP_RF_ORD_NODE_REG:        //�ɼ���ע��
				/*---------------judge meter address-------------------------*/		
				if(0x01 == stepIndex)	//�Ա��ɼ���ע��
				{				
					INT8U *pNodeAddr ;
					INT8U *pDataAddBuffData; 
					if(0x01 == stepAmount)	 	
					{					
						pNodeAddr = pApp_Rf_Frame->pRx+1 ;
						rx_DesAddrID    = *pApp_Rf_Frame->pRx		    ; //ע��Ĳɼ���ID
						
					}	
					else  //ͨ��·������
					{
						rx_DesAddrID    = pApp_Rf_Frame->pRx[stepAmount-stepIndex]; //ע��Ĳɼ���ID
						pNodeAddr = pApp_Rf_Frame->pRx+stepAmount-stepIndex+1 ;
					}					
					
					addrTpye = App_IsCollectorAddReg(pNodeAddr,7) ;  //�Ƿ�Ա��ɼ���ע��
					if(APP_ADDR_NULL == addrTpye)   //�ж���·�ϵĲɼ���ID��
					{
						return NO_SEND_DATA; 	   
					}
					App_Node_RF_Inform.SouceAddrID = rx_SourceAddrID;		//�󶨵ļ�����ID 
					App_Node_RF_Inform.DesAddrID = rx_DesAddrID     ;		//ע��Ĳɼ���ID  
					pDataAddBuffData= pNodeAddr + 7 ;     
					*pDataAddBuffData++ = 0;        //ST
					*pDataAddBuffData++ = tRf_Rssi; //�ź�ǿ��
					*pDataAddBuffData++ = tRf_Snr;  //������
					*pDataAddBuffData++ = 0x00;    	//��Ѳɼ���
					pApp_Rf_Frame->DataLen =OFFSETOF(__App_Rf_Frame,pRx)+1+7+4+2+(stepAmount-stepIndex);    //regid+REALY+ add +inform+crc
					pApp_Rf_Frame->Cmd++ ;
					/*�������*/
					tmpID = 0x1002 ;
					App_RF_Para_WR_Func(1,tmpID,&App_Node_RF_Inform.DesAddrID,1);
					tmpID = 0x1003 ;
					App_RF_Para_WR_Func(1,tmpID,&App_Node_RF_Inform.SouceAddrID,1);
					
					
				}
				break;	
			case APP_RF_ORD_NODE_REG_ACK:      //�ɼ�����������֡
			case APP_RF_ORD_NODE_METER_REG: 
			case APP_RF_ORD_RM_WIRED_ACK:				
				break ;
//			case 0x31:
	//		case 0x17:			
	//		    if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))  //�ж���·�ϵĲɼ���ID��
	//			{
	//				return NO_SEND_DATA; 
	//			}
	//            (buf->stepn) += 0x01;                //�ظ���·�ɼ���
	//            AutomaticTransferFlag = 0xAA;            //��������һ�� �ظ�֡���⴦��			
	//		    break;	
	//        case 0x16:                                        //���߱�Ʋ���
	//			//��������ַ�ж�
	//			if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))   //�ж���·�ϵĲɼ���ID��
	//            {
	//				return NO_SEND_DATA; 	   
	//            }
	//			if(0x01 == Tempstepn1)	
	//            {				
	//                //���߳���
	//				memset(WaitRFSendDataBuff,0x00,sizeof(WaitRFSendDataBuff));
	//				memcpy(WaitRFSendDataBuff,(struct CommFormat *)buf,sizeof(WaitRFSendDataBuff));  //����֡��ֵ	
	//				/**************UARTͨ��*******************/
	//			     BSP_UART_Write(UART2_PORT,WaitRFSendDataBuff,buf->datalength);			
	//				/**************UARTͨ��*******************/
	//				return NO_SEND_DATA;
	//            }
	//            else
	//            {
	//				(buf->stepn)--;                          //·�ɼ�����1
	//				AutomaticTransferFlag = 0xAA;            //��������һ�� �ظ�֡���⴦��
	//            }	
	//            break;			
	//		case 0x30:
	//			//��������ַ�ж�
	//			if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))   //�ж���·�ϵĲɼ���ID��
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
	////			    if(countIndex1 >= countIndex2)       //��ʼ����ڵ��ڽ�����
	////				{
	////					return NO_SEND_DATA; 	   
	////				}
	//				for(countIndex = 0x00 ; countIndex < countIndex2 ;countIndex++)
	//				{
	//						BSP_A24LC64Read(AssociatedCollectorAdd + ((countIndex1 + countIndex) * 0x04),tArry,sizeof(tArry));
	//					    rLen = sizeof(tArry) - 0x01;
	//					    memcpy(&buf->commdata[Tempstepn + 0x02 + (rLen * countIndex)],tArry,rLen);
	//						buf->datalength += rLen ;//ԭ����֡����+���ݳ���
	//				}	
	//				buf->datalength -=1;				// -TIMEOUT						        				
	//            }
	//            else
	//            {
	//				if(!CompareCollectorAddID(Tempstepn,Tempstepn1,buf->commdata,buf->meteraddS0,buf->controlcode))   //�ж���·�ϵĲɼ���ID��
	//				{
	//					return NO_SEND_DATA; 	   
	//				}
	//				(buf->stepn)--;                          //·�ɼ�����1
	//				AutomaticTransferFlag = 0xAA;            //��������һ�� �ظ�֡���⴦��
	//            }

	//			break;
//			case 0xA1:                                         //���յ���ƶ˷����ĳ�������
	//		case 0xA3: 		                                   //���յ���ƶ˷�����ע������
	//		    memcpy(&MeterReadApp,buf,buf->datalength);     //��ƻظ�֡��ʽ��ֵ
	//		    memcpy(&CollectorReadApp,OneWaterDataBuff,OneWaterDataBuff[0x02]);   //��ƻظ�֡��ʽ��ֵ		    
	//		    WriteAndReadMeterFlash(0x00,&MeterReadApp.meteraddS[0x02]);  //��ȡ����
	//		    Tempstepn = ((CollectorReadApp.stepn)>> 0x03) & 0x07;       //·�ɵȼ���
	//	        Tempstepn1 = (CollectorReadApp.stepn) & 0x07;               //���ڵȼ���
	//		    if(!CompareMeterAddID(Tempstepn,Tempstepn1,&MeterReadApp.meteraddS[0x00],MeterReadApp.controlcode))  //�ж���·�ϵĲɼ���ID��
	//			{
	//				return NO_SEND_DATA; 
	//			}
	//			buf->controlcode = CollectorReadApp.controlcode;             			
	//			buf->stepn = CollectorReadApp.stepn; 	
	//			buf->meteraddS0 = CollectorReadApp.meteraddS0;
	//		    memcpy(buf->commdata, CollectorReadApp.commdata, Tempstepn + 0x01);	//�ɼ���ID��ֵ���ID	
	//			memmove((buf->commdata + Tempstepn + 0x01), MeterReadApp.commdata, (MeterReadApp.datalength - 0x08));//���ݸ�ֵ
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
		stepAmount = ((pApp_Rf_Frame->Step)>> 0x03) & 0x07;       //·�ɵȼ���
		stepIndex = pApp_Rf_Frame->Step & 0x07;                   //���ڵȼ���
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
	//��װ����
	App_Rf_MakeFramePackage(pApp_Rf_Frame);
	return NORMAL_SEND; 	
}



/**********************************************************************************************
* Description       : ·��Э�� ������
* Input             : pBuff�����ݻ��� Len--����
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
//��Ҫÿ1S ����һ��
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

/////////////////////////////ԭ�ȵ�lora.c�ļ� ��ΪҲ��APP,�������Ϲ���////////////////////////
//#include "sx1276-LoRaMisc.h"
//#include "Bsp_SX127X.h"
//#include "bsp_SPI.h"
//#include "LoRa.h"
//#include "sx1276-LoRa.h"

/*! OnMaster
@brief LoRa�����ڵ�����
@param void
@return ret: 1 ���յ�����������
             2 �����������
             3 ���ճ�ʱ
             4 ���ͳ�ʱ
             0 �������
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
	/*��������Ҫ�ȴ�����*/
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


//�ϵ�ʱ��ʼ��RF����
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
@brief ���ÿ�ʼ����
@param buff: ���ͻ���
       len: ���ݳ���
       Type: ��������, 
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



/************************(C)COPYRIGHT 2014 �����Ƽ�*****END OF FILE****************************/
