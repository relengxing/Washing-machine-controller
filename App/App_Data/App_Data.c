///*****************************************Copyright(C)******************************************
//**-----------------------------------------文件信息---------------------------------------------
//**文    件    名: App_Data.c
//**创    建    人: 
//**创  建  日  期: 100329
//**最  新  版  本: V0.1
//**描          述: 存储管理表文件
//**---------------------------------------历史版本信息-------------------------------------------
//**修    改    人: fangjb
//**日          期: 20150511
//**版          本: V0.2
//**描          述:存储管理表文件
//**----------------------------------------------------------------------------------------------
//***********************************************************************************************/
/* Includes-----------------------------------------------------------------------------------*/
#include <string.h>
#include "App_Head.h " 
#include "App_Para_Interface.h" 
#include "bsp_rtc.h"
#include "bsp_iic.h"
#include "convertDataInterface.h"


///* Private define-----------------------------------------------------------------------------*/
#define	ONEDATA_0C01_LEN  50 

///* Private typedef----------------------------------------------------------------------------*/
typedef enum
{
	 ONEDATA_TYPE_TERMIAL=0					
	,ONEDATA_TYPE_GROUP						
	,ONEDATA_TYPE_POINT		
	,ONEDATA_TPYE_FREEZE			
}_ONEDATA_POINT_NATURE;



typedef enum
{
	ASSI_TYPE_NULL=0u,							// 空
	ASSI_TYPE_CURVE,							// 曲线数据
	ASSI_TYPE_DAY,								// 日数据
	ASSI_TYPE_MONTH,							// 月数据	
}_ASSI_TYPE;


//实时数据
__packed typedef struct
{
	INT8U Time[5];
	INT8U Data_0C01[ONEDATA_0C01_LEN];
}__OneData_0C01;

__packed typedef struct
{
	__OneData_0C01	OneData_0C01[POINTAMOUNT]   ;

}__OneData_Point_Data;

//历史数据
__packed typedef struct
{
	INT8U Data_0C02[sizeof(__OneData_0C01)];
}__TwoData_POINT_0C02;


__packed typedef struct
{
	INT8U FreezTime[3];
	__TwoData_POINT_0C02	TwoData_0C02[POINTAMOUNT][ASSI_CURVE_DAY_POINT];

}__TwoData_Day_All_Point_Data;


__packed typedef struct
{
	__TwoData_Day_All_Point_Data	TwoData_Day_All_Point_Data[ASSI_CURVE_SAVE_DAYS];
}__TwoData_All_Days_Points_Data;

__packed typedef struct
{
	__OneData_Point_Data	  			OneData_Point_Data[POINTAMOUNT] ;
	__TwoData_All_Days_Points_Data      TwoData_All_Days_Points_Data  ;
}__OneData_ALL_Table;

//测量点从0 开始
typedef struct
{
	_ONEDATA_POINT_NATURE OneData_PointNature;  //
	INT16U ID;									// ID,规约定义的Fn,
	INT16U MaxPoint;							// 测量点数量
	INT16U DataLen;								// 数据长度
	INT32U PointOffsetLen;						// 测量点偏移长度 (一个测量点所有数据项的长度) 
	INT32U DataAddrOffset;						// 数据偏移地址
}_OneData_Struct;

/* Private macro------------------------------------------------------------------------------*/
const _OneData_Struct OneData_Sampling_Table[]=
{
	 ONEDATA_TYPE_POINT		,0x0C01		,POINTAMOUNT    ,sizeof(__OneData_0C01)			,sizeof(__OneData_0C01)					,OFFSETOF(__OneData_ALL_Table,OneData_Point_Data[0].OneData_0C01)	
//曲线类
	,ONEDATA_TYPE_POINT		,0x0C0C		,POINTAMOUNT	,sizeof(__TwoData_POINT_0C02)	,sizeof(__TwoData_Day_All_Point_Data)	,OFFSETOF(__OneData_ALL_Table,TwoData_All_Days_Points_Data)	
	,ONEDATA_TYPE_POINT		,0x0C0A		,POINTAMOUNT	,sizeof(__OneData_0C01)	,sizeof(__OneData_0C01)   				,OFFSETOF(__OneData_ALL_Table,TwoData_All_Days_Points_Data)															
};

/* Private variables--------------------------------------------------------------------------*/

//INT8U gOneDataBuff[ (ONEDATA_0C01_LEN+5)*POINTAMOUNT] ;

/* Private function prototypes----------------------------------------------------------------*/
INT16U GWFirstData_function(__App_Para_InterfaceStr *pdata);

/* Private functions--------------------------------------------------------------------------*/



static const _OneData_Struct * OneData_SearchID(INT16U ID)
{
	const _OneData_Struct *pOneDataTable ;
	INT32U left,right  ;
	pOneDataTable = OneData_Sampling_Table ;
	right = sizeof(OneData_Sampling_Table)/sizeof(_OneData_Struct);
	left = 0 ;
	do
	{
		if(pOneDataTable->ID == ID)
			return pOneDataTable ;
		pOneDataTable ++ ;	
		left ++ ;
	}while(left!=right);
	pOneDataTable = (void*)0;
	return 0;	
}



//大小类号对应的数据长度 如果返回值0X00,说明该测量点无效 
INT8U App_BigSammTpyeToLen(INT16U PointIndex)
{
    __App_Para_InterfaceStr pdata;
	INT8U  Dbuff[20];
	INT8U  meterTpye = 0 ;
	/*初始化接口函数参数*/
	pdata.afn = METER_READ; //功能码
	pdata.point = PointIndex;//测量点
    pdata.IDIndex=0x406 ;
	pdata.dlen = sizeof(Dbuff);		//数据长度
	pdata.dbuff=Dbuff; //数据内
    GWPara_function(&pdata);
	meterTpye = Dbuff[16];
	/*判断规约是否有效*/
	if((Dbuff[1] > 10)||(!Dbuff[1]))
	{
		return 0x00 ;
	}
	if((APP_METER_AGR_DEFINE_WATER_ELE==meterTpye)||(APP_METER_AGR_DEFINE_WATER_PHO_ELE==meterTpye))
	{
		return APP_METER_WATER_LEN ;
	}
	if((APP_METER_AGR_DEFINE_HOT==meterTpye)||(APP_METER_AGR_DEFINE_HOT_RUINA==meterTpye)||(APP_METER_AGR_DEFINE_HOT_MAITUO==meterTpye) \
		||(APP_METER_AGR_DEFINE_HOT_TIANGANG==meterTpye)||(APP_METER_AGR_DEFINE_HOT_LICHUANG==meterTpye)||(APP_METER_AGR_DEFINE_HOT_JINSHUI==meterTpye)
			||(APP_METER_AGR_DEFINE_HOT_HUIZHONG==meterTpye) ||(APP_METER_AGR_DEFINE_HOT_WEIAN==meterTpye) )
	{
		return APP_METER_HOT_LEN ;
	}
	if(APP_METER_AGR_DEFINE_TEMP_RF==meterTpye)
	{
		return APP_METER_TEMP_RF_LEN ;
	}	
	return APP_METER_WATER_LEN ;
}


INT8U App_WR_OneData(INT8U Opt,INT32U addr,INT8U *pBuff,INT16U Len)
{
	addr+=1000;
	if(Opt) //W
	{
//			memcpy(gOneDataBuff + addr,pBuff,Len);
		return Bsp_EEPROM_Write(addr,pBuff,Len);
	}
	else
	{
		return Bsp_EEPROM_Read(addr,pBuff,Len);
//			memcpy(pBuff ,gOneDataBuff + addr,Len);
	}
}



/**********************************************************************************************
* Description       : 由于对不同的命令，做数据长度处理 
* Input             : pdata ，参见InterfaceStruct  测量点从1开始 
*					  rateLen:标准标度 为一类数据OneData_Sampling_Table和DLT_OneData_Sampling_Table规定长度
* Output            : none
* Contributor       : fangjb
* Date First Issued : 2010.05.11
* Date Secord Issued :2012.09.05	暂时不考虑曲线问题
***********************************************************************************************/

//volatile INT32U OnedDataLen = 0 ;	 //e005-->57349 //2015.06.16 12406H
INT16U GWFirstData_function(__App_Para_InterfaceStr *pdata)
{
	const _OneData_Struct   *pOneDataTable ; 
	INT16U len = 0 ;
	INT16U pointIndex = 0 ;
//	OnedDataLen = sizeof(__OneData_ALL_Table);
//	if(ONEDATA_LEN >=OnedDataLen)
//	{
//			   NOP();
//	}
//	else
//	{
//		while(1);
//	}
	
	pOneDataTable = OneData_SearchID(pdata->IDIndex) ;	
	if(0 != pOneDataTable)	
	{
		INT32U addr = 0;
		//几个特殊处理的数据 曲线和日冻结数据
		if(0xc02 == pdata->IDIndex)
		{
			if(AFN_INQUIREPARA_ONE == pdata->afn)
			{
//				return Data_Freeze_Read(pdata);
			}
			return 0 ;
		}
		if(ONEDATA_TYPE_TERMIAL == pOneDataTable->OneData_PointNature)
		{
			pointIndex = 0 ;	
		}		
		else if(ONEDATA_TYPE_POINT == pOneDataTable->OneData_PointNature)
		{
			if((pdata->point>(POINTAMOUNT))||(!pdata->point))
			{
				return 0;
			}
			pointIndex = pdata->point-1 ;
				
		}
		addr = pOneDataTable->DataAddrOffset ;	   					/*表格中的起始位1-c01*/
		addr += (pOneDataTable->PointOffsetLen)*(pointIndex);		/*数据偏移位置*/
		if( METER_WRITE == pdata->afn  )
		{
			
			len = pOneDataTable ->DataLen;	
			if((0xF001 == pdata->IDIndex)||(0x0C01 == pdata->IDIndex)||(0x901F == pdata->IDIndex))
			{
				_BSPRTC_TIME pTime;
				INT32U addrTime = 0 ;
				if(BSP_RTCGetTime(&pTime))
				{
					/*写个时间*/
					pOneDataTable = OneData_SearchID(0xC01) ;	
					addrTime = 	pOneDataTable->DataAddrOffset + (pOneDataTable->PointOffsetLen)*(pointIndex) ;
					App_WR_OneData(1,addrTime,(INT8U*)&pTime.Minute,5);
				}
				if(0x0C01 == pdata->IDIndex)
				{
					addr+=5 ;
					len -=5 ;
				}
				if(!App_DataBcdIsOK(pdata->dbuff,4))
				{
					return 0 ;
				}			
				App_WR_OneData(1,addr,pdata->dbuff,len);//1-5

			}
			else if(0x0C0C ==pdata->IDIndex)
			{
				_BSPRTC_TIME tTime;
				if(BSP_RTCGetTime(&tTime))
				{
					INT32U startOffset ;//= ByteBcdToHex(tTime.Day);
					INT32U dayOffset ;
					dayOffset   = ByteBcdToHex(tTime.Day);
					if(dayOffset)
					{
						dayOffset-- ;
					}
					startOffset = OFFSETOF(__OneData_ALL_Table,TwoData_All_Days_Points_Data.TwoData_Day_All_Point_Data[dayOffset]); //基地址 
					
					if((!tTime.Hour)&&(tTime.Minute < 0x5))
					{
						//写时间
						App_WR_OneData(1,addr,(INT8U*)&tTime.Day,3);
						startOffset+=3 ;
						
					}
					//这天的哪个时间 小时为单位
					startOffset+=  ByteBcdToHex(tTime.Hour)*pOneDataTable ->DataLen ;
					App_WR_OneData(1,startOffset,pdata->dbuff,pOneDataTable ->DataLen);//
				}			
			}
		}
		else if(( AFN_INQUIREPARA_ONE == pdata->afn ) ||( METER_READ == pdata->afn  ))
		{
			len = pdata->dlen ;	
			
			if(AFN_INQUIREPARA_ONE == pdata->afn)
			{
				if((0x0C01 == pdata->IDIndex)||(0x1C01 == pdata->IDIndex))
				{
					len = App_BigSammTpyeToLen(pdata->point);				
				}
				else
				{
					len = pOneDataTable ->DataLen ; 					
				}

				if(pdata->dlen < len)
				{
					return 0;
				}
			}			
			if((0x0C01 == pdata->IDIndex)||(0x1C01 == pdata->IDIndex))
			{
				len = pOneDataTable ->DataLen ;//App_BigSammTpyeToLen(pdata->point);				
				App_WR_OneData(0,addr,pdata->dbuff,len);
			}
			else if(0x0C0C ==pdata->IDIndex)
			{
				_BSPRTC_TIME tTime;
				if(BSP_RTCGetTime(&tTime))
				{
					INT32U startOffset ;//= ByteBcdToHex(tTime.Day);
					INT32U dayOffset ;
//					startOffset = OFFSETOF(__OneData_ALL_Table,TwoData_All_Days_Points_Data); //基地址
					dayOffset   = ByteBcdToHex(tTime.Day);
					if(dayOffset)
					{
						dayOffset-- ;
					}
					startOffset = OFFSETOF(__OneData_ALL_Table,TwoData_All_Days_Points_Data.TwoData_Day_All_Point_Data[dayOffset]); //基地址 				
					App_WR_OneData(0,startOffset,pdata->dbuff,sizeof(__TwoData_Day_All_Point_Data));
				}			
			}
		}
	}
	return  len ;
}

/**********************************************************************************************
* Description       : 对批量数据初始化
* Input             : addr：起始地址 pData：缓冲区 Len：缓冲长度 totalLen：需要批量处理的数据长度
* Output            : 1
* Contributor       : fangjb
* Date First Issued : 2010.05.31
***********************************************************************************************/
INT16U Operate_BigData_OneData(INT32U startAddr ,INT8U *pData,INT16U Len,INT16U totalLen )
{
	INT32U countIndex ;
	INT32U countAmount = 0 ;
	countAmount = totalLen /Len  ;
	countAmount += (totalLen %Len)? 1:0 ;
	countIndex = 0 ;
	do
	{
		INT32U writeLen ;
		INT32U addr ;
		addr = startAddr + countIndex * Len ;
		countIndex ++ ;	
		if(countIndex * Len < totalLen)	
			writeLen = Len ;
		else
			writeLen = totalLen -(countIndex-1)*Len;
		App_WR_OneData(1,addr,pData,writeLen); 
		
	}while(countAmount!=countIndex);
	return 1 ;
}



INT16U Reset_ONEDATA_Function(void)
{
	INT8U arry[64*2];
	INT16U len ;
	len = sizeof(arry);
	memset(arry,0,len);
	Operate_BigData_OneData(0,arry,len,(INT16U)sizeof(__OneData_ALL_Table) );
	return TRUE ;
}





