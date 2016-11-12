/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "App_SC.h"
/* Private typedef -----------------------------------------------------------*/

INT8U SC_RXTXBUFF[100];
/* Private define ------------------------------------------------------------*/
INT8U ADDH;
INT8U ADDL;
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/******************************************************************************
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_SC_ClearBuff(INT8U ComPort)
{
	BSP_UART_ClearRxBuff(ComPort);
	BSP_UART_ClearTxBuff(ComPort);

}
void App_SC_SendArray(INT8U ComPort,INT8U *pArray,INT16U Lenth)			//发送一串数据
{
	App_SC_ClearBuff(ComPort);
	BSP_UART_Write(ComPort,pArray,Lenth);
}
void App_SC_SendFixedFrame(INT8U ComPort,App_SC_FixedFrame_Struct *pApp_SC_FixedFrame)		//发送一个固定帧
{
	INT8U* FrameBuff = SC_RXTXBUFF;

	App_SC_ClearBuff(ComPort);

	*FrameBuff++ = pApp_SC_FixedFrame->FrameStart;			//起始帧0x10		
	*FrameBuff++ = pApp_SC_FixedFrame->ControlCode;     	//控制码
	*FrameBuff++ = pApp_SC_FixedFrame->AidControlCode;  	//辅助控制码
	*FrameBuff++ = pApp_SC_FixedFrame->ADDH;            	//被叫方地址高位
	*FrameBuff++ = pApp_SC_FixedFrame->ADDL;            	//被叫方地址低位
	*FrameBuff++ = pApp_SC_FixedFrame->CS;              	//CS校验位			
	*FrameBuff++ = pApp_SC_FixedFrame->FrameStop;      		 //结束位0x16

	BSP_UART_Write(APP_485_COMM,SC_RXTXBUFF,7);		//发送7个字节
}

void App_SC_SendVariableFrame(INT8U ComPort,App_SC_VariableFrame_Struct *pApp_SC_VariableFrame)	//发送一个可变帧
{
	INT8U* FrameBuff = SC_RXTXBUFF;
	INT8U i = 0;
	INT8U Lenth = (pApp_SC_VariableFrame->FrameLenthH *256) + pApp_SC_VariableFrame->FrameLenthL;		//数据域长度
	App_SC_ClearBuff(ComPort);		//清空收发缓冲区

	*FrameBuff++ = pApp_SC_VariableFrame->FrameStart;
	*FrameBuff++ = pApp_SC_VariableFrame->FrameLenthH;	
	*FrameBuff++ = pApp_SC_VariableFrame->FrameLenthL;	
	*FrameBuff++ = pApp_SC_VariableFrame->FrameFlag;	
	*FrameBuff++ = pApp_SC_VariableFrame->ControlCode;	
	*FrameBuff++ = pApp_SC_VariableFrame->ADDH;			
	*FrameBuff++ = pApp_SC_VariableFrame->ADDL;			

	for(i = 0; i <Lenth-3; i++)
	{
		*FrameBuff++ = *(pApp_SC_VariableFrame->Data+i);
	}
	*FrameBuff++ = pApp_SC_VariableFrame->CS;			
	*FrameBuff++ = pApp_SC_VariableFrame->FrameStop;	
	BSP_UART_Write(APP_485_COMM,SC_RXTXBUFF,6+Lenth);
}
/**********************************************************************************************
* Description       :
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
void App_SC_FixedFrameFillCSCheck(App_SC_FixedFrame_Struct *pApp_SC_FixedFrame)
{
	INT8U CS = 0;
	//从控制码到被叫地址低字节
	CS +=pApp_SC_FixedFrame->ControlCode;     	//控制码	
	CS +=pApp_SC_FixedFrame->AidControlCode;  	//辅助控制
	CS +=pApp_SC_FixedFrame->ADDH;            	//被叫方地址高位
	CS +=pApp_SC_FixedFrame->ADDL;            	//被叫方地址低位

	pApp_SC_FixedFrame->CS = CS;
}
void App_SC_VariableFrameFillCSCheck(App_SC_VariableFrame_Struct *pApp_SC_VariableFrame)
{
	INT8U CS = 0;
	INT8U i;
	INT8U Lenth = (pApp_SC_VariableFrame->FrameLenthH *256)+pApp_SC_VariableFrame->FrameLenthL;		//数据域长度
	CS +=pApp_SC_VariableFrame->ControlCode;     	//控制码	
	CS +=pApp_SC_VariableFrame->ADDH;            	//被叫方地址高位
	CS +=pApp_SC_VariableFrame->ADDL;            	//被叫方地址低位
	for(i = 0;i< Lenth-3;i++)
	{
		CS += pApp_SC_VariableFrame->Data[i];
	}
	pApp_SC_VariableFrame->CS = CS;
}
/******************************************************************************
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_SC_DealFixedFrame(App_SC_FixedFrame_Struct *pApp_SC_FixedFrame_Struct)		//处理固定帧
{
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x05)			//集中器自检结果
	{	
		App_SC_VariableFrame_Struct pApp_SC_VariableFrame;
		INT8U Data[8];
		
		pApp_SC_VariableFrame.FrameStart = 0x68;
		pApp_SC_VariableFrame.FrameLenthH = 0x00;
		pApp_SC_VariableFrame.FrameLenthL = 0x0b;
		pApp_SC_VariableFrame.FrameFlag = 0x68;
		pApp_SC_VariableFrame.ControlCode = 0x85;
		pApp_SC_VariableFrame.ADDH = ADDH;
		pApp_SC_VariableFrame.ADDL = ADDL;
		//TODO:调用自检结果
		Data[0] = 0x01;		//自检的结果
		Data[1] = 0x01;
		Data[2] = 0x01;
		Data[3] = 0x01;
		Data[4] = 0x01;
		Data[5] = 0x01;
		Data[6] = 0x01;
		Data[7] = 0x01;
		pApp_SC_VariableFrame.Data = Data;
		pApp_SC_VariableFrame.CS = 0x00;			
		pApp_SC_VariableFrame.FrameStop = 0x16;
		App_SC_VariableFrameFillCSCheck(&pApp_SC_VariableFrame);
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//上报可变帧
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x07)			//取当前时间.主站向集中器获取当前时间，集中器返回集中器的时间
	{
		_BSPRTC_TIME RTCTime;
		INT8U Data[6];
		App_SC_VariableFrame_Struct pApp_SC_VariableFrame;
		
		BSP_RTCGetTime(&RTCTime);
		pApp_SC_VariableFrame.FrameStart = 0x68;
		pApp_SC_VariableFrame.FrameLenthH = 0x00;
		pApp_SC_VariableFrame.FrameLenthL = 0x09;
		pApp_SC_VariableFrame.FrameFlag = 0x68;
		pApp_SC_VariableFrame.ControlCode = 0x87;
		pApp_SC_VariableFrame.ADDH = ADDH;
		pApp_SC_VariableFrame.ADDL = ADDL;
		Data[0] = RTCTime.Year;
		Data[1] = RTCTime.Month;
		Data[2] = RTCTime.Day;
		Data[3] = RTCTime.Hour;
		Data[4] = RTCTime.Minute;
		Data[5] = RTCTime.Second;
		pApp_SC_VariableFrame.Data = Data;
		pApp_SC_VariableFrame.FrameStop = 0x16;
		App_SC_VariableFrameFillCSCheck(&pApp_SC_VariableFrame);
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//上报可变帧//回复应答帧
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x30)			//查询集中器地址
	{
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;			//上报固定帧
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//被叫应答主叫
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//回复应答帧
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x14)			//查询GPRS参数		
	{
		App_SC_VariableFrame_Struct pApp_SC_VariableFrame;
		__App_Para_InterfaceStr para_data;
	
		INT8U IPBuff[28];
		INT8U DataBuff[20];
		INT8U lenth;
		INT8U *p;
		INT8U dataLenth;
		Bsp_I2CSPM_Init();
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = IPBuff;				//
		para_data.dlen  =  sizeof(IPBuff) ;
		GWPara_function(&para_data);			//把主站IP地址从E2中读出来
		Bsp_I2CSPM_Disable();
		
		pApp_SC_VariableFrame.Data = DataBuff;
		p = pApp_SC_VariableFrame.Data;
		pApp_SC_VariableFrame.FrameStart = 0x68;
		pApp_SC_VariableFrame.FrameLenthH = 0x00;
		pApp_SC_VariableFrame.FrameLenthL = 0x00;
		pApp_SC_VariableFrame.FrameFlag = 0x68;
		pApp_SC_VariableFrame.ControlCode = 0x94;
		pApp_SC_VariableFrame.ADDH = ADDH;
		pApp_SC_VariableFrame.ADDL = ADDL;
		
		*p++ = IPBuff[4];		//port低
		*p++ = IPBuff[5];		//port高
		*p++ = 0x00;			//卡类型
		dataLenth = 3+3;
		lenth = sprintf((char*)p,"%d",IPBuff[0]);		//第一段IP
		p += lenth;
		*p++ = '.';
		dataLenth += lenth+1;
		lenth = sprintf((char*)p,"%d",IPBuff[1]);		//第二段IP
		p += lenth;
		*p++ = '.'; 
		dataLenth += lenth+1;
		lenth = sprintf((char*)p,"%d",IPBuff[2]);		//第三段IP
		p += lenth;
		*p++ = '.';
		dataLenth += lenth+1;
		lenth = sprintf((char*)p,"%d",IPBuff[3]);		//第四段IP
		*p++ = 0;
		
		pApp_SC_VariableFrame.FrameLenthL = dataLenth;
	
		pApp_SC_VariableFrame.FrameStop = 0x16;
		App_SC_VariableFrameFillCSCheck(&pApp_SC_VariableFrame);
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//上报可变帧
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x09)			//查询集中器软件版本号
	{
		App_SC_VariableFrame_Struct pApp_SC_VariableFrame;
		INT8U DATA[2];
		pApp_SC_VariableFrame.FrameStart = 0x68;
		pApp_SC_VariableFrame.FrameLenthH = 0x00;
		pApp_SC_VariableFrame.FrameLenthL = 0x05;
		pApp_SC_VariableFrame.FrameFlag = 0x68;
		pApp_SC_VariableFrame.ControlCode = 0x89;
		pApp_SC_VariableFrame.ADDH = ADDH;
		pApp_SC_VariableFrame.ADDL = ADDL;
		pApp_SC_VariableFrame.Data = DATA;
		pApp_SC_VariableFrame.Data[0] = 0x10;		//TODO:换成版本
		pApp_SC_VariableFrame.Data[1] = 0x10;
		pApp_SC_VariableFrame.CS = 0x00;		
		pApp_SC_VariableFrame.FrameStop = 0x16;
		App_SC_VariableFrameFillCSCheck(&pApp_SC_VariableFrame);
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//上报可变帧
	}
}


void App_SC_DealVariableFrame(App_SC_VariableFrame_Struct *pApp_SC_VariableFrame_Struct)		//处理可变帧
{
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x01)		//校对时间
	{
		_BSPRTC_TIME realTime;
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;
		//把时间写入RTC
		realTime.Year = pApp_SC_VariableFrame_Struct->Data[0];				//年
		realTime.Month =pApp_SC_VariableFrame_Struct->Data[1];				//月
		realTime.Day=pApp_SC_VariableFrame_Struct->Data[2];					//日
		realTime.Hour=pApp_SC_VariableFrame_Struct->Data[3];				//时
		realTime.Minute=pApp_SC_VariableFrame_Struct->Data[4];				//分
		realTime.Second=pApp_SC_VariableFrame_Struct->Data[5];				//秒
		BSP_RTCSetTime(&realTime);						//保存时间
		//应答	
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//被叫应答主叫
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH ;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.CS = 0x00;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//回复应答帧
		
	}
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x12)		//设置GPRS参数
	{
		__App_Para_InterfaceStr para_data;	
		INT8U IPBuff[28];
		INT8U *p;
		INT8U i;
		p = pApp_SC_VariableFrame_Struct->Data;				//p指向数据域
		IPBuff[4] = p[0];
		IPBuff[5] = p[1];
		//pApp_SC_VariableFrame_Struct->Data[2];			//卡类型
		p += 3;
		for(i = 0; i<4; i++)		//4段IP地址
		{
			while(*p++ != '.')		//检测到一个'.'，一共3个'.'
			{
				IPBuff[i] *= 10;
				IPBuff[i] += (*p - '0');
				if(*p == 0x00)			//如果检测到0x00代表
					break;
			}
		}
		//把IP信息写入到E2中
		Bsp_I2CSPM_Init();
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = IPBuff;				//
		para_data.dlen  =  sizeof(IPBuff) ;
		GWPara_function(&para_data);			//把主站IP地址从E2中读出来
		Bsp_I2CSPM_Disable();
	}
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x25)		//设置RS485波特率				此处有疑问，是什么编码，16进制还是BCD码
	{
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;			//上报固定帧
		//更新485波特率
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data);
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data + 1);
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data + 2);
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data + 3);
		//应答	
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//被叫应答主叫
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.CS = 0x00;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//回复应答帧
	}
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x31)		//设置集中器地址
	{
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;			//上报固定帧
		
		ADDH = pApp_SC_VariableFrame_Struct->Data[0];				//接收新地址
		ADDL = pApp_SC_VariableFrame_Struct->Data[1];
		
		//应答	
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//被叫应答主叫
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.CS = 0x00;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//回复应答帧
		
	}
}

/******************************************************************************
* Description: 把接收到的数据传入该函数，该函数自动识别是固定帧还是可变帧，并执行指令
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
INT8U App_SC_Resolve(INT8U *pBuff,INT8U Lenth)
{
	INT8U i;
	volatile INT8U len;
	INT8U *pArray;
	pArray = pBuff;
	
	for(i=0; i<Lenth; i++)
	{
		if(pArray[i] == 0x68)			//检测到可变帧帧头
		{
			if(pArray[i+3] == 0x68)			//检测到可变帧标志位
			{
				len = pArray[i+1]*256+ pArray[i+2];			//帧长
				if(pArray[i+4+len] == 0x16)					//判断帧尾
				{
					//if(CS校验)	       	//可添加CS校验，如果正确则处理，错误则放弃
					{
						//TODO:处理可变帧
						App_SC_VariableFrame_Struct *pApp_SC_VariableFrame_Struct;
						memcpy(pApp_SC_VariableFrame_Struct,pArray+i,7);						
						memcpy(pApp_SC_VariableFrame_Struct->Data,pArray+i+7,len-7);
						pApp_SC_VariableFrame_Struct->CS = pArray[i+3+len];
						pApp_SC_VariableFrame_Struct->FrameStop = pArray[i+4+len];
						
						App_SC_DealVariableFrame(pApp_SC_VariableFrame_Struct);		//处理可变帧
						return 2;				//代表接收到可变帧
					}
				}
			}
		}
		if(pArray[i] == 0x68)			//检测到固定帧帧头
		{
			if(pArray[i+6] == 0x16)			//第7个字节是0x16，表示收到一条完整的固定帧
			{
				//if(CS校验)		//可添加CS校验，如果正确则处理，错误则放弃
				{
					//TODO:处理固定帧
					App_SC_FixedFrame_Struct *pApp_SC_FixedFrame_Struct;
					memcpy(pApp_SC_FixedFrame_Struct,pArray+i,sizeof(App_SC_FixedFrame_Struct));
					App_SC_DealFixedFrame(pApp_SC_FixedFrame_Struct);
					return 1;					//代表接收到固定帧
				}
			}
		}
	}
	return 0;		//代表没有接收到，或者解析错误
}

void App_SC_Report(void)
{
	//--从E2中读取抄表数据
	__App_Para_InterfaceStr para_data;

//	Array = App_Gprs_CMD_Buff.TxBuff;

	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0c0c;
	para_data.point = 1;
//	para_data.dbuff = App_Gprs_CMD_Buff.TxBuff + 14;
	para_data.dbuff = SC_RXTXBUFF + 14;
	para_data.dlen = 147;				//修改
	GWFirstData_function(&para_data);
	
}


/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
