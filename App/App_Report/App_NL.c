/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "App_NL.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/******************************************************************************
* Description: 清除收发缓冲区
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_ClearBuff(INT8U ComPort)	//清除缓冲区
{
	BSP_UART_ClearRxBuff(ComPort);
	BSP_UART_ClearTxBuff(ComPort);
}
/******************************************************************************
* Description: 填充CS校验
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_FillCSCheck(App_NL_Struct *pApp_NL_Struct)
{
	INT8U CS = 0;
	INT8U i = 0;
	INT16U Lenth = pApp_NL_Struct->DataLenth[0] + pApp_NL_Struct->DataLenth[1]*256;		//数据域长度
	CS += pApp_NL_Struct->FrameStart;
	CS += pApp_NL_Struct->InfoDomain[0];
	CS += pApp_NL_Struct->InfoDomain[1];
	CS += pApp_NL_Struct->DataLenth[0];
	CS += pApp_NL_Struct->DataLenth[1];
	for(i=0; i<Lenth; i++)
	{
		CS += pApp_NL_Struct->Data[i];
	}
	pApp_NL_Struct->CS = CS;
}
/******************************************************************************
* Description: 从串口发送一条NL的数据结构体
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_SendStruct(INT8U ComPort,App_NL_Struct *pApp_NL_Struct)	
{
	INT8U TXBuff[200];
	INT8U* p;
	p = TXBuff;
	INT8U Lenth;
	INT8U i;
	
	Lenth = pApp_NL_Struct->DataLenth[0] + pApp_NL_Struct->DataLenth[1]*256;		//数据域长度
	*p++ = pApp_NL_Struct->FrameStart;			//起始位
	*p++ = pApp_NL_Struct->InfoDomain[0];		//信息域
	*p++ = pApp_NL_Struct->InfoDomain[1];
	*p++ = pApp_NL_Struct->DataLenth[0];		//数据域长度
	*p++ = pApp_NL_Struct->DataLenth[1];
	for(i=0; i<Lenth; i++)
	{
		*p++ = pApp_NL_Struct->Data[i];			//数据域
	}
	*p++ = pApp_NL_Struct->CS;					//CS校验位
	*p++ = pApp_NL_Struct->FrameStop;			//结束位
	BSP_UART_Write(ComPort,TXBuff,Lenth+7);			//把结构体从ComPort发送出去
}
/******************************************************************************
* Description: 确认帧和否认帧
* Input:  Nothing			//0是确认，1是否认
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_Ask(INT8U ComPort,INT8U ask)				//应答
{
	App_NL_Struct pApp_NL_Struct;
	INT8U DataBuff[20];
	
	pApp_NL_Struct.Data = DataBuff;
	pApp_NL_Struct.FrameStart = 0x68;				//起始位
	pApp_NL_Struct.InfoDomain[0] = 0x00;
	pApp_NL_Struct.InfoDomain[1] = 0x00;
	pApp_NL_Struct.DataLenth[0] = 0x06;				//5字节
	pApp_NL_Struct.DataLenth[1] = 0x00;
	pApp_NL_Struct.Data[0] = 0x01;						//数据标识
	pApp_NL_Struct.Data[1] = 0x00;						//数据标识
	pApp_NL_Struct.Data[2] = 0x01;						//测量点数量
	pApp_NL_Struct.Data[3] = 0x00;						//测量点号
	pApp_NL_Struct.Data[4] = 0x00;						//测量点号
	pApp_NL_Struct.Data[5] = ask;					//确认结果
	
	App_NL_FillCSCheck(&pApp_NL_Struct);
	pApp_NL_Struct.FrameStop = 0x16;				//结束位
	App_NL_SendStruct(ComPort,&pApp_NL_Struct);
}
/******************************************************************************
* Description:处理一条NL的结构体
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_DealStruct(INT8U ComPort,App_NL_Struct *pApp_NL_Struct)
{
	__App_Para_InterfaceStr para_data;
	INT32U Datalenth;
	Datalenth = pApp_NL_Struct->DataLenth[0] + pApp_NL_Struct->DataLenth[1]*256;		//
	if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 6))		//设置上报点数
	{
//		GPRS_Interval = pApp_NL_Struct->Data[6];
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0401;
		para_data.point = 1;
		para_data.dbuff = pApp_NL_Struct->Data+5;			//指向上报点数
		para_data.dlen  =  1 ;
		GWPara_function(&para_data);							//把上报点数写入E2；
		App_NL_Ask(ComPort,0);					//回复确认帧
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 5))		//查询上报点数
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[10] = {0};
		pSend_NL_Struct.Data = DataBuff;

		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0401;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;			//指向上报点数
		para_data.dlen  =  1 ;
		GWPara_function(&para_data);							//把上报点数写入E2；
		
		pSend_NL_Struct.FrameStart = 0x68;				//起始位
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x06;				//5字节
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x01;						//数据标识
		pSend_NL_Struct.Data[1] = 0x05;						//数据标识
		pSend_NL_Struct.Data[2] = 0x01;						//测量点数量
		pSend_NL_Struct.Data[3] = 0x00;						//测量点号
		pSend_NL_Struct.Data[4] = 0x00;						//测量点号
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//结束位
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//返回查询信息
		
	}
	else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x02)&&(Datalenth == 0x21))		//设置主站IP地址和端口号
	{
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = pApp_NL_Struct->Data+5;			//指向IP地址
		para_data.dlen  = 28 ;
		GWPara_function(&para_data);							//把IP地址写入E2；
		App_NL_Ask(ComPort,0);					//回复确认帧		
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x02)&&(Datalenth == 5))		//查询主站IP地址和端口号
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[35] = {0};
		pSend_NL_Struct.Data = DataBuff;
		
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;			//指向IP地址
		para_data.dlen  = 28 ;
		GWPara_function(&para_data);							//把上报点数写入E2；
		
		pSend_NL_Struct.FrameStart = 0x68;				//起始位
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x21;				//33字节
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x02;						//数据标识
		pSend_NL_Struct.Data[1] = 0x05;						//数据标识
		pSend_NL_Struct.Data[2] = 0x01;						//测量点数量
		pSend_NL_Struct.Data[3] = 0x00;						//测量点号
		pSend_NL_Struct.Data[4] = 0x00;						//测量点号
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//结束位
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//返回查询信息
	}
	else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x0D)&&(Datalenth == 0x0b))		//对时
	{
		_BSPRTC_TIME realTime;

		realTime.Second = pApp_NL_Struct->Data[5];				//秒
		realTime.Minute = pApp_NL_Struct->Data[6];				//分
		realTime.Hour = pApp_NL_Struct->Data[7];					//时
		realTime.Day = pApp_NL_Struct->Data[8];				//日
		realTime.Month = pApp_NL_Struct->Data[9];				//月
		realTime.Year = pApp_NL_Struct->Data[10];				//年
		BSP_RTCSetTime(&realTime);	
		App_NL_Ask(ComPort,0);					//回复确认帧
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x0D)&&(Datalenth == 5))		//查询集中器的时间
	{
		_BSPRTC_TIME RTCTime;
		
		BSP_RTCGetTime(&RTCTime);	
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[20];
	
		pSend_NL_Struct.Data = DataBuff;
		pSend_NL_Struct.FrameStart = 0x68;				//起始位
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x0b;				//11字节
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x0D;						//数据标识
		pSend_NL_Struct.Data[1] = 0x05;						//数据标识
		pSend_NL_Struct.Data[2] = 0x01;						//测量点数量
		pSend_NL_Struct.Data[3] = 0x00;						//测量点号
		pSend_NL_Struct.Data[4] = 0x00;						//测量点号
		pSend_NL_Struct.Data[5] = RTCTime.Second;
		pSend_NL_Struct.Data[6] = RTCTime.Minute;
		pSend_NL_Struct.Data[7] = RTCTime.Hour;
		pSend_NL_Struct.Data[8] = RTCTime.Day;
		pSend_NL_Struct.Data[9] = RTCTime.Month;
		pSend_NL_Struct.Data[10] = RTCTime.Year;

		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//结束位
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//返回查询信息

	}
	else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x0E)&&(Datalenth == 0x0c))		//设置集中器逻辑地址
	{
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = pApp_NL_Struct->Data+5;			//指向集中器逻辑地址
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);							//把集中器逻辑地址写入E2
		App_NL_Ask(ComPort,0);					//回复确认帧
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x0E)&&(Datalenth == 5))		//查询集中器逻辑地址
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[20]={0};
		
		pSend_NL_Struct.Data = DataBuff;
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;			//指向地址区
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);							//把地址从E2中读取出来
				
		pSend_NL_Struct.FrameStart = 0x68;				//起始位
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x0c;				//12字节
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x0E;						//数据标识
		pSend_NL_Struct.Data[1] = 0x05;						//数据标识
		pSend_NL_Struct.Data[2] = 0x01;						//测量点数量
		pSend_NL_Struct.Data[3] = 0x00;						//测量点号
		pSend_NL_Struct.Data[4] = 0x00;						//测量点号
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//结束位
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//返回查询信息
	}
	else if((pApp_NL_Struct->Data[1] == 0x08)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 5))		//查询集中器版本信息
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[20];
	
		pSend_NL_Struct.Data = DataBuff;
		pSend_NL_Struct.FrameStart = 0x68;				//起始位
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x13;				//19字节
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x01;						//数据标识
		pSend_NL_Struct.Data[1] = 0x08;						//数据标识
		pSend_NL_Struct.Data[2] = 0x01;						//测量点数量
		pSend_NL_Struct.Data[3] = 0x00;						//测量点号
		pSend_NL_Struct.Data[4] = 0x00;						//测量点号
		pSend_NL_Struct.Data[5] = (INT8U)SW_Version;			//软件版本
		pSend_NL_Struct.Data[6] = (INT8U)(SW_Version>>8);	
		pSend_NL_Struct.Data[7] = (INT8U)(SW_Version>>16);	
		pSend_NL_Struct.Data[8] = (INT8U)(SW_Version>>24);	
		pSend_NL_Struct.Data[9] = SW_ReleaseData[0];			//软件版本发布时间	
		pSend_NL_Struct.Data[10]= SW_ReleaseData[1];	
		pSend_NL_Struct.Data[11] = SW_ReleaseData[2];	
		pSend_NL_Struct.Data[12] = (INT8U)HW_Version;			//硬件版本
		pSend_NL_Struct.Data[13] = (INT8U)(HW_Version>>8);	
		pSend_NL_Struct.Data[14] = (INT8U)(HW_Version>>16);	
		pSend_NL_Struct.Data[15] = (INT8U)(HW_Version>>24);	
		pSend_NL_Struct.Data[16] = HW_ReleaseData[0];			//硬件版本发布时间
		pSend_NL_Struct.Data[17] = HW_ReleaseData[1];	
		pSend_NL_Struct.Data[18]=  HW_ReleaseData[2];	
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//结束位
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//返回查询信息
		
	}else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x06)&&(Datalenth == 0x16))		//设置集中器户表配置参数	
	{
		INT8U Buff[10];
		INT8U JAddress[7];
		memcpy(Buff,pApp_NL_Struct->Data+5,9);					//通讯速率及端口，通讯协议类型，户表地址
		memcpy(JAddress,pApp_NL_Struct->Data+5+9,7);				//集中器地址
		Buff[9] = pApp_NL_Struct->Data[5+16];					//用户表大小类号
		/*集中器地址*/
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = JAddress;						//指向集中器逻辑地址
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);							//把集中器逻辑地址写入E2
		/*其他户表参数*/
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0406;
		para_data.point = 1;
		para_data.dbuff = Buff;						//把集中器户表数据写入E2
		para_data.dlen  =  10 ;
		GWPara_function(&para_data);							//把上报点数写入E2；
		
		App_NL_Ask(ComPort,0);					//回复确认帧
	}else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x06)&&(Datalenth == 0x05))		//查询集中器户表配置参数
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[30];
		/*其他户表参数*/
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0406;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;						//把集中器户表数据读出
		para_data.dlen  =  10 ;
		GWPara_function(&para_data);	
		DataBuff[21] = DataBuff[14];						//户表大小类号
		/*集中器地址*/
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5+9;						//指向集中器逻辑地址
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);
		pSend_NL_Struct.Data = DataBuff;
		pSend_NL_Struct.FrameStart = 0x68;				//起始位
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x16;				//17+5字节
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x06;						//数据标识
		pSend_NL_Struct.Data[1] = 0x05;						//数据标识
		pSend_NL_Struct.Data[2] = 0x01;						//测量点数量
		pSend_NL_Struct.Data[3] = 0x00;						//测量点号
		pSend_NL_Struct.Data[4] = 0x00;						//测量点号
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//结束位
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//返回查询信息
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 5))		//硬件初始化
	{
		Bsp_Wdt_Init();				//打开看门狗，不喂狗，时间到以后会重启
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)0);		//所有灯点亮
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)1);
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)2);
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);
		App_NL_Ask(ComPort,0);					//回复确认帧
		while(1);
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x02)&&(Datalenth == 5))		//数据区初始化
	{
		Reset_ONEDATA_Function();							//清除E2中的抄表数据
		App_NL_Ask(ComPort,0);					//回复确认帧
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x03)&&(Datalenth == 5))		//回复出产配置
	{
		Reset_Para_All();									//清除E2中所有数据
		App_NL_Ask(ComPort,0);					//回复确认帧
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x04)&&(Datalenth == 5))		//除主站信息其余初始化
	{
		Reset_Para_Not_Comunication();						//清除E2中除主站信息外的数据
		App_NL_Ask(ComPort,0);					//回复确认帧
	}
	else	//没有响应该指令的代码，返回应答错误
	{
		App_NL_Ask(ComPort,1);					//回复确认帧
	}
	
}

/******************************************************************************
* Description: 从串口接收一条NL的数据结构体并交给App_NL_DealStruct()处理
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
INT8U App_NL_Receive(INT8U ComPort,INT8U *pBuff,INT8U Lenth)
{
	App_NL_Struct pApp_NL_Struct;
	INT8U i;
	volatile INT8U DataLenth;
	INT8U *pArray;
	INT8U DataBuff[100];
	pArray = pBuff;
	
	for(i=0; i<Lenth; i++)
	{
		if(pArray[i] == 0x68)								//帧起始位	
		{
			DataLenth = pArray[i+3] + pArray[i+4]*256;		//数据域长度
			if(pArray[i+DataLenth+6] == 0x16)				//帧结束位
			{
				//if(CS??)	    				//把数据传入结构体
				{
					pApp_NL_Struct.FrameStart = pArray[i];			
					pApp_NL_Struct.InfoDomain[0] = pArray[i+1];
					pApp_NL_Struct.InfoDomain[1] = pArray[i+2];
					pApp_NL_Struct.DataLenth[0] = pArray[i+3];
					pApp_NL_Struct.DataLenth[1] = pArray[i+4];
					pApp_NL_Struct.Data = DataBuff;
					memcpy(DataBuff,pArray+5+i,DataLenth);
					pApp_NL_Struct.CS = pArray[i+DataLenth+5];
					pApp_NL_Struct.FrameStop = pArray[i+DataLenth+6];
					App_NL_ClearBuff(ComPort);
					App_NL_DealStruct(ComPort,&pApp_NL_Struct);				//处理结构体
					return 1;				//接收到
				}
			}
		}
	}
	return 0;					//未接收到
}

/******************************************************************************
* Description: 上报数据，仅供GPRS模块调用
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_Report(void)
{
	__App_Para_InterfaceStr para_data;		
	App_NL_Struct	pApp_NL_Struct;
	INT8U DataBuff[100];
	INT8U Buff[20] = {0};
	_BSPRTC_TIME RTCTime;
	BSP_RTCGetTime(&RTCTime);		//获取RTC时间
		/*填充帧头*/
	pApp_NL_Struct.Data = DataBuff;
	pApp_NL_Struct.FrameStart= 0x68;
	pApp_NL_Struct.InfoDomain[0] = 0x40;			//表计
	pApp_NL_Struct.InfoDomain[1] = 0x01;			//需要回复
	pApp_NL_Struct.DataLenth[0] =  0x56;				//86字节
	pApp_NL_Struct.DataLenth[1] =  0x00;
		/*填充数据域前部的数据*///未完成
	DataBuff[0] = 0x02;						//0C02	数据标识
	DataBuff[1] = 0x0c;
	memset(DataBuff+2,0xaa,20);				//20字节 ICCID
	memset(DataBuff+22,0xbb,8);				//8字节	地理信息
	DataBuff[30] = 0x01;			//测量点数量
	DataBuff[31] = 0x01;			//上报点数
	DataBuff[32] = RTCTime.Day;				//3字节上报时间
	DataBuff[33] = RTCTime.Month;
	DataBuff[34] = RTCTime.Year;
	/*读取抄表数据*/			//此处返回的是带5个字节的时标，需要往前写5个字节，然后写7字节逻辑地址覆盖掉时标
	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0c01;
	para_data.point = 1;
	para_data.dbuff = DataBuff+43-5;
	para_data.dlen  = 48;
	GWFirstData_function(&para_data);	
		/*读取逻辑地址*/
	para_data.afn = METER_READ;			//7字节地址
	para_data.IDIndex = 0x040e;
	para_data.point = 1;
	para_data.dbuff = DataBuff+35;
	para_data.dlen  = 7;
	GWPara_function(&para_data);			//读取逻辑地址
	/*读取大小类号*/
	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0406;
	para_data.point = 1;
	para_data.dbuff = Buff;						//把集中器户表数据读出
	para_data.dlen  =  10 ;
	GWPara_function(&para_data);	
	DataBuff[42] = Buff[9];					//表记类型(大小类号)				
	/*填充帧尾*/
	App_NL_FillCSCheck(&pApp_NL_Struct);
	pApp_NL_Struct.FrameStop = 0x16;
	/*上报抄表数据*/
	App_NL_SendStruct(APP_GPRS_COMM,&pApp_NL_Struct);		//从GPRS端口发送抄表数据		
	
}









