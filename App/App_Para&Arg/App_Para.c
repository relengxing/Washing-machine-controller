/*****************************************Copyright(C)******************************************

* FileName          : agreementpara.c
* Author            : fjb
* Date First Issued : 2008/12/03
* Version           : V1.0



* History       :
* //2008        : V
* Description       : 
*-----------------------------------------------------------------------------------------------

***********************************************************************************************/
/* Includes-----------------------------------------------------------------------------------*/
#include "App_ParaTable.h" 
#include "App_Para_Interface.h" 
#include "App_MemoryConfig.h"   

#include "bsp_iic.h"
#include "bsp_rtc.h"
#include "bsp_power_ctrl.h"
//#include "App_PlcInterface.h" 

//#include "agreementConfigInformtion.h" 

/* Private define-----------------------------------------------------------------------------*/



/* Private typedef----------------------------------------------------------------------------*/

typedef enum
{
	 PARA_TYPE_TERMIAL=0					
	,PARA_TYPE_PORT						
	,PARA_TYPE_POINT		
	,PARA_TPYE_TASK	
	,PARA_TPYE_PW		
}_Para_Nature;

typedef enum{DEVICETPYE=0,POINTTPYE}__PointTpye;

typedef enum{INIT_ALL_PARA=0,INIT_NOT_COMM_PARA}__InitParaTpye;

typedef INT16U (_ParaFUNCREAD)(AgreementTableStruct*);   

typedef struct _NewParaStruct_
{
    INT16U ID;       				    /*��ʶ�� */
	_ParaFUNCREAD  *ParaFuncRead;		/* ���ݶ�ȡ����*/
}__NewParaStruct;


//�������0 ��ʼ
typedef struct
{
	_Para_Nature ParaNature;  					//
	INT16U ID;									// ID,��Լ�����Fn,
	INT16U MaxPoint;							// �������
	INT16U DataLen;								// ���ݳ���
	INT32U PointOffsetLen;						// ������ƫ�Ƴ���
	INT32U ParaIDStartAddrOffset;				// ID���׸���ַ ƫ����
}_ParaData_Struct;

/* Private macro------------------------------------------------------------------------------*/


const _ParaData_Struct Para_All_Table[ ]=
{

	 PARA_TYPE_TERMIAL	,0x200	,0				,sizeof(__ParaData_0200)	,0							,OFFSETOF(__Para_ALL_Table,Const_Ter_Data.ParaData_0200 )		//�ն˵�ַ
	,PARA_TYPE_TERMIAL	,0x401	,0				,sizeof(__ParaData_0401)	,0							,OFFSETOF(__Para_ALL_Table,Para_Ter_Para_Data.ParaData_0401)	//�������
	,PARA_TYPE_TERMIAL	,0x402	,0				,sizeof(__ParaData_0402)	,0							,OFFSETOF(__Para_ALL_Table,Para_Ter_Comm_Data.ParaData_0402)	//��վip	
	,PARA_TYPE_TERMIAL	,0x403	,0				,sizeof(__ParaData_0403)	,0							,OFFSETOF(__Para_ALL_Table,Para_Ter_Comm_Data.ParaData_0403)	//SMS	
	,PARA_TYPE_TERMIAL	,0x404	,0				,sizeof(__ParaData_0404)	,0							,OFFSETOF(__Para_ALL_Table,Para_Ter_Comm_Data.ParaData_0404)	//������IP
	,PARA_TYPE_POINT	,0x406	,POINTAMOUNT	,sizeof(__ParaData_0406)	,sizeof(__Para_Point_Data)	,OFFSETOF(__Para_ALL_Table,Para_Point_Data[0].ParaData_0406) 	//�����
	,PARA_TYPE_TERMIAL	,0x407	,0      		,sizeof(__ParaData_0407)	,0							,OFFSETOF(__Para_ALL_Table,Para_Ter_Para_Data.ParaData_0407 ) 	
	,PARA_TYPE_TERMIAL	,0x409	,TASKMAXAMOUNT	,sizeof(__ParaData_0409)	,sizeof(__Para_Task_Data)	,OFFSETOF(__Para_ALL_Table,Para_Task_Data[0].ParaData_0409)  	//�ϱ�����ʱ��
	,PARA_TPYE_TASK		,0x40A	,TASKMAXAMOUNT	,sizeof(__ParaData_040A)	,sizeof(__Para_Task_Data)	,OFFSETOF(__Para_ALL_Table,Para_Task_Data[0].ParaData_040A)  	
	,PARA_TYPE_POINT	,0x40B	,POINTAMOUNT	,sizeof(__ParaData_040B)	,sizeof(__Para_Point_Data)	,OFFSETOF(__Para_ALL_Table,Para_Point_Data[0].ParaData_040B) 	
	,PARA_TYPE_TERMIAL	,0x40C	,0				,sizeof(__ParaData_040C)	,0							,OFFSETOF(__Para_ALL_Table,Para_Ter_Para_Data.ParaData_040C)	
	,PARA_TYPE_TERMIAL	,0x40D	,0				,sizeof(__ParaData_040D)	,0							,0															//��ʱ�������					
	,PARA_TYPE_TERMIAL	,0x40E	,0				,sizeof(__ParaData_0200)	,0							,OFFSETOF(__Para_ALL_Table,Const_Ter_Data.ParaData_0200)		//��ַ ��ѯ
	,PARA_TYPE_TERMIAL	,0x40F	,0				,0							,0 							,0                                                          	//�汾�� 
	,PARA_TYPE_TERMIAL	,0x4ff	,0				,0							,0							,0														   //fatcor id					
};




/* Private variables--------------------------------------------------------------------------*/


//INT8U ParaChangeFlag=0; 
//INT8U  AllArrary[DATA_LEN]; 
const  INT8U Factor_ID[4] = "SCBT";
static INT8U Ver_RAM[ ]  = "1.00" ;//
const  INT8U Ver_Data[ ] = {0x11,0x1,0x16}; //DDMMYY
const  INT8U Ver_Hard[ ] = "2.00" ;
const  INT8U Ver_Hard_Day[ ] = {0x15,0x12,0x15}; //DDMMYY
const  INT8U Ver_MbName[] = "NL2100-BATT";		 //��������
//INT8U  AreaCode[3] ; 		//������������ֽ�
INT8U  TerAddress[APP_TER_ADDR_LEN]; 		//�ն˵�ַ


/* Private function prototypes----------------------------------------------------------------*/
INT8U  App_Para_Write(INT32U Addr,INT8U *pData,INT32U Len)
{
	
	
	return Bsp_EEPROM_Write(Addr,pData,Len);
}

INT8U  App_Para_Read(INT32U Addr,INT8U *pData,INT32U Len)
{
	return Bsp_EEPROM_Read(Addr,pData,Len);
}


void OSTimeDly(INT32U DlyTime)
{
	
}
////////////////////////////////////////��Ҫ���Ƶĵط�//////////////////////////////////////////



/* Private functions--------------------------------------------------------------------------*/




INT8U CommunictionParaFlag ;
//static void Write_Communiction_Para_Flag(INT8U flag)
//{
//	assert_param(flag!=0);
//	CommunictionParaFlag = flag;	
//}

//���ͨѶ�������ı�־
void Clear_Communiction_Para_Flag(void)
{
	CommunictionParaFlag = 0 ;
}
//��ȡͨѶ�������ı�־
INT8U Read_Communiction_Para_Flag(void)
{
	return CommunictionParaFlag;
}


///**********************************************************************************************
//* Function          : Deal_Ver_Change
//* Description       : ����汾�仯
//* Input             : none
//* Output            : none 
//* Contributor       : fjb
//* Date First Issued : 2009/04/02
//***********************************************************************************************/
//void Deal_Ver_Change(void)
//{
//	INT8U arraryVer[BACKUP_VER_ADDR_LEN];
//	INT8U flag;
//	BSP_AT45Read(BACKUP_VER_ADDR,arraryVer,4);
//	flag = memcmp(arraryVer,Ver_RAM,4);
//	if(flag)
//	{
//		if((0xff!=*arraryVer)&&(0xff!=*(arraryVer+1)))
//		{
//			ERC1_Warm(arraryVer,Ver_RAM);
//		}
//			
//		BSP_AT45Write(BACKUP_VER_ADDR,Ver_RAM,4);
//	}		
//}

static INT8U SpeacilParaID_040D(__App_Para_InterfaceStr *pData)
{

	_BSPRTC_TIME pTime;
	static INT8U yearHight = 0 ;
	if(AFN_SETPARA == pData->afn  )
	{
		pTime.Year 	 = *pData->dbuff++;			// ��   
		yearHight    = *pData->dbuff++;
		pTime.Month  = *pData->dbuff++;			// ��
		pTime.Day 	 = *pData->dbuff++ ;		// ��
		pTime.Hour 	 = *pData->dbuff++ ;		// ʱ
		pTime.Minute = *pData->dbuff++ ;		// ��
		pTime.Second = *pData->dbuff++	;		// ��
		pTime.Week 	 = *pData->dbuff++;			// �� 
		if(TRUE==BSP_RTCSetTime(&pTime))
	    {
			return 8;    
	    }		
	}
	else if( METER_WRITE == pData->afn  )
	{
		pTime.Second = *pData->dbuff++	;		    // ��
		pTime.Minute = *pData->dbuff++ ;			// ��
		pTime.Hour 	 = *pData->dbuff++ ;			// ʱ
		pTime.Day 	 = *pData->dbuff++ ;			// ��
		pTime.Month  = (*pData->dbuff++)&0x1f;		// ��
		pTime.Year 	 = *pData->dbuff;				// ��   
		pTime.Week 	 = 1;							// �� 
		if(TRUE==BSP_RTCSetTime(&pTime))
	    {
			return 7;    
	    }
		
	}
	else if( AFN_INQUIREPARA == pData->afn )
	{
		if(BSP_RTCGetTime(&pTime))
		{
			*pData->dbuff++= pTime.Year 	;	// ��   
			*pData->dbuff++= yearHight    	;
			*pData->dbuff++= pTime.Month  	;	// ��
			*pData->dbuff++= pTime.Day 	  	;	// ��
			*pData->dbuff++= pTime.Hour 	;	// ʱ
			*pData->dbuff++= pTime.Minute  	;	// ��
			*pData->dbuff++= pTime.Second 	;	// ��
			*pData->dbuff++= pTime.Week 	;	// �� 
			 return 8 ;
		}			
		
	}
	else if( METER_READ == pData->afn  )
	{
		if(BSP_RTCGetTime(&pTime))
		{
			 *pData->dbuff++ = pTime.Second ;	
			 *pData->dbuff++ = pTime.Minute ;		
			 *pData->dbuff++ = pTime.Hour 	 ;		
			 *pData->dbuff++ = pTime.Day 	 ;		
			 *pData->dbuff++ = pTime.Month  ;		
			 *pData->dbuff++ = pTime.Year	 ;	
			 *pData->dbuff++ = 0x20	 		;	
			 return 7 ;
		}			
	}
	return 0 ;
}

static INT8U SpeacilParaID_040F(__App_Para_InterfaceStr *pData)
{
	if(( AFN_INQUIREPARA == pData->afn ) ||( METER_READ == pData->afn  ))
	{
		memcpy(pData->dbuff,Ver_RAM,4);
		memcpy(pData->dbuff+4,Ver_Data,3);
		memcpy(pData->dbuff+7,Ver_Hard,4);
		memcpy(pData->dbuff+11,Ver_Hard_Day,3);
		return 14 ;
	}
	return 0 ;
}


//static INT8U SpeacilParaID_0409(__App_Para_InterfaceStr *pData)
//{
////	if( (AFN_SETPARA == pData->afn  ) ||( METER_WRITE == pData->afn  ))
////	{
////		return ASSIF66Setup(pData->point,pData->dbuff) ;
////		
////	}
////	else if(( AFN_INQUIREPARA == pData->afn ) ||( METER_READ == pData->afn  ))
////	{
////		return ASSIF66Read(pData->point,pData->dbuff)  ;
////	}
//	return 0 ;
//}


//static INT8U SpeacilParaID_040A(__App_Para_InterfaceStr *pData)
//{
////	if( (AFN_SETPARA == pData->afn  ) ||( METER_WRITE == pData->afn  ))
////	{
////		return ASSIF68Setup(pData->point,pData->dbuff) ;
////		
////	}
////	else if(( AFN_INQUIREPARA == pData->afn ) ||( METER_READ == pData->afn  ))
////	{
////		INT8U reData = 0 ;
////		reData = ASSIF68Read(pData->point,pData->dbuff)  ;
////		if(0xff==*pData->dbuff)
////		{
////			*pData->dbuff = 0x55;
////		}
////		return reData ;
////	}
//	return 0 ;
//}


//INT8U MeterRxArry[20 ] ;
//INT8U SendMsgToMeter(_BSP_MESSAGE *Msg)
//{
//	Msg->MsgID = BSP_RAED_POINT_FN;
//	memcpy(MeterRxArry,Msg->pData,Msg->DataLen +2 );
//	OSQPost(ReadDL645_Prevent,Msg);
//	return TRUE ;
//}
//

//static INT8U SpeacilParaID_040B(__App_Para_InterfaceStr *pData)
//{

//	if( AFN_SETPARA == pData->afn )
//	{
//		INT8U array[5] = {0xa0,0x17} ;
//		memcpy(array,(INT8U*)&pData->IDIndex,2);
//		memcpy(array+2 ,pData->dbuff ,PARA_040B_LEN);
////		MeterRxArry[0] = 0xa0 ;
////		MeterRxArry[1] = 0x17 ;
////		MeterRxArry[2] = *pData->dbuff ;
////		gSendMsgToMeter.DivNum = pData->point;
////		gSendMsgToMeter.pData = MeterRxArry ;//pData->dbuff;
////		gSendMsgToMeter.DataLen = PARA_040B_LEN;
////		gSendMsgToMeter.MsgID= BSP_RAED_POINT_FN;
////		OSQPost(ReadDL645_Prevent,&gSendMsgToMeter);
////		SendMsgToMeter(&gSendMsgToMeter);

//		return App_SendRMeterMsg(pData->point,array,PARA_040B_LEN+2);

////		return PARA_040B_LEN ;
//		
//	}
//	return 0 ;
//}


static INT8U SpeacilParaID_04FF(__App_Para_InterfaceStr *pData)
{
	if(( AFN_INQUIREPARA == pData->afn ) ||( METER_READ == pData->afn  ))
	{
		memcpy(pData->dbuff,Factor_ID,4);
		return 4 ;
	}
	return 0 ;
}



static const _ParaData_Struct *ParaData_SearchID(INT16U ID)
{
	const _ParaData_Struct *pDataTable = (void*)0 ;
	INT32U left,right  ;
//	if(ID > 0x5000)
//	{
//		return 0 ;
//	}
	pDataTable = Para_All_Table ;
	right = sizeof(Para_All_Table)/sizeof(_ParaData_Struct);

	left = 0 ;
	do
	{
		if(pDataTable->ID == ID)
			return pDataTable ;
		pDataTable ++ ;	
		left ++ ;
	}while(left!=right);
	pDataTable = (void*)0;
	return 0;	
}



/**********************************************************************************************
* Function          : FirstData_function
* Description       : �����漰�������Ķ�ȡ��д����Ҫ�ǵ�����ݶ�ȡ����Ҫд����Ӧ��λ��
*                     ��վ�ٲ��ù�Լ�Ĺ�����.�����������ܶ��FNͬʱ���������Ծ�ֻ����һ��FN	 
*					  ֻ��Ե����������DI
* Input             : pdata��Ա����
*                            afn--------������                       
*                            point------�������   1~~~      
*                            id_len-----ID�ŵĸ���        
*                            *IDIndex---ID��ʾ���ŵĻ�����
*                            dlen-------���ص����ݳ���           
*                            *dbuff-----���ص����ݻ����� 
*                     
*                     exg: ������������ __App_Para_InterfaceStr variblestruct  
*                     variblestruct.afn    = 0x0a ;
*                     variblestruct.point  = 0;			
*                     variblestruct.id_len = 2; 
*                     variblestruct.IDIndex= ID_ARRARY; (INT16U ID_ARRARY = {0x01,0x02c1}�������һ��Ҫ��������������Ұָ��)
*                     variblestruct.dlen   = 0;            (�˸�ֵ�����Ա�ʡ�ԣ��ڽӿ������Ѿ����˳�ʼ������)
*                     variblestruct.dbuff  = databuff;    (���ݶ�����д�����ݵĴ�����������һ��Ҫ��������������Ұָ��)
*                     FirstData_function(variblestruct);
* Output            : �ɹ�������Ч���ȣ�ʧ�� 0��
* Contributor       : fjb
* Date First Issued : 2010.03.15
***********************************************************************************************/
//volatile INT32U lenXXXXXXXXXX ;		// 4BDC  //

INT16U GWPara_function(__App_Para_InterfaceStr *pdata)
{
	const _ParaData_Struct   *pParaTable ; 
	INT16U tmpID = 0 ;
//	lenXXXXXXXXXX = sizeof(__Para_ALL_Table);
	
	tmpID = pdata->IDIndex ;
//	if(0x0409==tmpID)
//	{
//		return SpeacilParaID_0409(pdata);
//	}
//	if(0x040A==tmpID)
//	{
//		return SpeacilParaID_040A(pdata);
//	}
//	if(0x040B==tmpID)
//	{
//		return SpeacilParaID_040B(pdata);
//	}
	if(0x040D==tmpID)	//��ʱ
	{
		return SpeacilParaID_040D(pdata);	
	}
	if(0x040F==tmpID)
	{
		return SpeacilParaID_040F(pdata);
	}
	if(0x04FF==tmpID)
	{
		return SpeacilParaID_04FF(pdata);
	}
	if((0xf405 == tmpID)&&(AFN_INQUIREPARA == pdata->afn))
	{
		pdata->afn = METER_READ ;
		tmpID = 0x1405 ;
	}
	else if((0xf406 == tmpID)&&(AFN_INQUIREPARA == pdata->afn))
	{
		pdata->afn = METER_READ ;
		*pdata->pSource = 0xff ;
		tmpID = 0x1405 ;
	}

	pParaTable = ParaData_SearchID(tmpID) ;	
	if(0 != pParaTable)	
	{
		INT32U addr = 0 ;
		INT32U len = pdata->dlen ;
		INT16U pointIndex = 0 ;
		addr = 	TERMINAL_PARA_START_ADDR + pParaTable->ParaIDStartAddrOffset ;
		//����������Ƿ���Ч
		if(PARA_TYPE_TERMIAL == pParaTable->ParaNature)
		{
			pointIndex = 0 ;
		}			
		else if(PARA_TYPE_POINT == pParaTable->ParaNature)
		{
			if((pdata->point > POINTAMOUNT)||(!pdata->point)) 
			{
				return 0 ;
			}
			pointIndex = pdata->point -1 ;
		}
		else if(PARA_TPYE_PW == pParaTable->ParaNature)
		{
			if((pdata->point > POINTAMOUNT)) 
			{
				return 0 ;
			}
			pointIndex = pdata->point ;
		}
		else
		{
			while(1);
		}

		addr += pParaTable->PointOffsetLen*pointIndex ;  	   //83432
		//
		if( (AFN_SETPARA == pdata->afn  ) ||( METER_WRITE == pdata->afn  )||(REST_PARA_ORDER == pdata->afn))
		{
			
			len = pParaTable ->DataLen;	
			if(0x0200==tmpID)
			{
//				memcpy(AreaCode,pdata->dbuff,3);
				memcpy(TerAddress,pdata->dbuff,sizeof(TerAddress));
			}
//			else if(((0x0405==tmpID)||(0x1405==tmpID)))
//			{
//				INT8U array[15]={0xa0,0x14};
//				memcpy(array+2,pdata->dbuff ,PARA_0405_0_LEN);
//				if((REST_PARA_ORDER != pdata->afn)&&(0x0405==tmpID))
//				{
//					if(pdata->point)	 /*�������Ĳ��÷�����*/
//					{
//						if(AFN_SETPARA == pdata->afn)
//						{
//							if(0xff==*(pdata->dbuff)) 
//                            {
//                                array[0]=0xa0;
//                                array[1]=0x20;
//                                array[2]= 0XFF;
//                            }
//                            App_SendRMeterMsg(pdata->point,array,PARA_0405_0_LEN+2);
//						}
//					}
//					else
//					{
//						__App_Para_InterfaceStr pDataStrct;
//						INT16U tempID;
//						INT8U  flagPw = 0 ;
//						pDataStrct.afn = METER_WRITE;
//						tempID = 0x0C09;
//						pDataStrct.id_len = 1;
//						pDataStrct.IDIndex = &tempID;
//						pDataStrct.dbuff = pdata->dbuff;
//						pDataStrct.dlen = 1;
//						pDataStrct.point = 0;
//						FirstDataPress(&pDataStrct);	
//						if(0xff==*(pdata->dbuff)) 
//						{
//							flagPw = 1 ;
//						}
//						ERC12_Warm(0,flagPw);
//						
//					}
//				}
//				if(0xff==*(pdata->dbuff)) 
//				{
//					addr+= PARA_0405_0_LEN ;//859ea
//				}
//                else if((0xff==*(pdata->pSource))&&(METER_WRITE == pdata->afn))
//                {
//					addr+= PARA_0405_0_LEN ;
//                }
//                len=PARA_0405_0_LEN ;					                 
//			}
//			else if(0x410 == tmpID)
//			{
//				extern void App_Lcd_Set_Pw(const INT8U*pData,INT8U Len) ;
//				App_Lcd_Set_Pw(pdata->dbuff,len);
//			}
			else if((0x420 == tmpID))
			{
				if(!pointIndex)
				{

					return 0 ;
				}
				if(0xff==*(pdata->dbuff)) 
				{
					addr+= PARA_0405_0_LEN ;
				}
				len = PARA_1405_0_LEN ;


//				else
//				{
//					len = PARA_1405_0_LEN ;
//				}
			}
			App_Para_Write(addr,pdata->dbuff,len);	
			if((0x0402==tmpID)||(0x0403==tmpID)||(0x0404==tmpID))
			{

//				App_GprsParaChargePostMsg( ) ;
				CommunictionParaFlag =1 ;
			}
//			else if((0x0406==tmpID)||(0x0421==tmpID))
//			{
//				AppSet_PlcParaChangeFlag();
//			}	
		}
		else if(( AFN_INQUIREPARA == pdata->afn ) ||( METER_READ == pdata->afn  ))
		{
			if(AFN_INQUIREPARA == pdata->afn)
			{
				len = pParaTable ->DataLen	;
				if(((0x0405==tmpID)||(0x1405==tmpID)))	/*ֻ������ͨ��Կ*/
				{
//					addr+= 8 ;//���볤��8 +Ver 
					len = 1 ;		
				}
				else if((0x410 == tmpID)||(0x420 == tmpID))
				{
					return 0 ;	
				}
			}
			else 
			{
				if(((0x0405==tmpID)||(0x1405==tmpID)))
				{
					if(0xff==*pdata->pSource)	/*��������Կ*/
					{
						addr+= PARA_0405_0_LEN ;//���볤��8 +Ver 
					}
					len = PARA_0405_0_LEN ; 
				}
			}
			App_Para_Read(addr,pdata->dbuff,len);
			// Ϊ�˳�ʼ�������޶�
			if(((0x0405==tmpID)||(0x1405==tmpID)))
			{
				if(0x88 == *pdata->dbuff)	/*��������Կ*/
				{
					if(AFN_INQUIREPARA != pdata->afn)
					{
						if(0xff==*pdata->pSource)	/*��������Կ*/
						{
							*pdata->dbuff = 0xff ;
						}
						else
						{
							*pdata->dbuff = 0x00;
						}
					}
					else
					{
						*pdata->dbuff = 0x00;
					}
				}
			}				
		    if((0x40E==tmpID)&&(AFN_INQUIREPARA == pdata->afn))
			{
				len+= 1 ;
				memmove(pdata->dbuff+4,pdata->dbuff+3,2);
				*(pdata->dbuff+3) = 0 ;	
			}
			
		}
		return len ;			
	}
    return 0;    
}

//void Reset_Para_Function_PassWord(INT8U *Buff,INT32U Len)
//{

//	const _ParaData_Struct   *pParaTable ; 
//	INT32U totalLen = 0 ;
//	INT32U addr ;
//	INT32U surplusLen ;
//	INT16U clrCountAmount ;

//	INT16U tmpID;
//	//����������
//	tmpID = 0x0405 ;
//	pParaTable = ParaData_SearchID(tmpID) ;	
//	totalLen = pParaTable->PointOffsetLen * pParaTable->MaxPoint ;
//	memset(Buff,0x88,Len);
//	addr = TERMINAL_PARA_START_ADDR + pParaTable->ParaIDStartAddrOffset;	
//	clrCountAmount = totalLen/Len ;		
//	while(clrCountAmount--)
//	{
//		BSP_AT45Write(addr,Buff,Len);  
//		addr+=Len;
//		if(!(clrCountAmount%5))
//		{
//			OSTimeDly(SYS_DELAY_10ms);
//		}
//	}
//	surplusLen = totalLen/Len  ;
//	if(surplusLen)
//	{
//		BSP_AT45Write(addr,Buff,surplusLen);  
//    }

//	//���ݲ���������
//	tmpID = 0x1405 ;
//	pParaTable = ParaData_SearchID(tmpID) ;	
//	totalLen = pParaTable->PointOffsetLen * pParaTable->MaxPoint ;
//	memset(Buff,0x88,Len);
//	addr = TERMINAL_PARA_START_ADDR + pParaTable->ParaIDStartAddrOffset;	
//	clrCountAmount = totalLen/Len ;		
//	while(clrCountAmount--)
//	{
//		BSP_AT45Write(addr,Buff,Len);  
//		addr+=Len;
//		if(!(clrCountAmount%5))
//		{
//			OSTimeDly(SYS_DELAY_10ms);
//		}
//	}
//	surplusLen = totalLen/Len  ;
//	if(surplusLen)
//	{
//		BSP_AT45Write(addr,Buff,surplusLen);  
//    }


//}


//��Ҫ�߳��ն˵�ַ,�˴��д��󣬵�Ϊ���ݣ����ݵ�ַ����ȥ��
INT16U Reset_Para_Function(__InitParaTpye InitFlag)
{
	__App_Para_InterfaceStr pData ;
	INT32U addr ;
	INT32U clrTotalLen ;
	INT32U terParaLen = 0 ;
	INT32U surplusLen ;
	INT8U array[150];
	INT16U tmpID ;
	INT16U clrCountAmount ;
	INT16U buffLen ;
	buffLen = 150 ;
	memset(array,0,buffLen);
	//���ն�Ϊ�Ĳ���ȫ������
	if(INIT_NOT_COMM_PARA == InitFlag)
	{
		terParaLen = sizeof(__Para_Ter_Comm_Data) ; 
	
	}
	addr = TERMINAL_PARA_START_ADDR + terParaLen + sizeof(__Const_Ter_Data);

	clrTotalLen = sizeof(__Para_ALL_Table) - terParaLen-sizeof(__Const_Ter_Data);

	clrCountAmount = clrTotalLen/buffLen ;
		
	while(clrCountAmount--)
	{
		App_Para_Write(addr,array,buffLen);  
		addr+=buffLen;
		if(!(clrCountAmount%5))
		{
			OSTimeDly(SYS_DELAY_10ms);
		}
	}
	surplusLen = clrTotalLen/buffLen  ;
	if(surplusLen)
	{
		App_Para_Write(addr,array,surplusLen);  
    }
	
	//�����ն˲�����ͨѶ�޹ص�����
    pData.afn  = REST_PARA_ORDER;   /*������ */
    pData.point = 0 ;          		/*������ */
	tmpID = 0x401 ;
    pData.IDIndex = tmpID; 		/*Դ���ݻ�����,��Ҫ�Ǵ���ID���ã��������ݴ��������� */
	pData.dlen    = 7 ;            /*Ŀ�����ݳ��� */
	array[1] = 0x30 ;
	array[2] = 5 ;
    pData.dbuff   = array;          /*Ŀ�����ݻ����� */
	GWPara_function(&pData);

//	Reset_Para_Function_PassWord(array,sizeof(array));
//	/*�������*/ 
//	ASSICDClearPara();
	return 1 ;
}


void Reset_Para_Not_Comunication(void)
{
	
	Reset_Para_Function(INIT_NOT_COMM_PARA);		
}

void Reset_Para_All(void)
{
	Reset_Para_Function	(INIT_ALL_PARA);

}



