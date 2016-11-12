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
void App_SC_SendArray(INT8U ComPort,INT8U *pArray,INT16U Lenth)			//����һ������
{
	App_SC_ClearBuff(ComPort);
	BSP_UART_Write(ComPort,pArray,Lenth);
}
void App_SC_SendFixedFrame(INT8U ComPort,App_SC_FixedFrame_Struct *pApp_SC_FixedFrame)		//����һ���̶�֡
{
	INT8U* FrameBuff = SC_RXTXBUFF;

	App_SC_ClearBuff(ComPort);

	*FrameBuff++ = pApp_SC_FixedFrame->FrameStart;			//��ʼ֡0x10		
	*FrameBuff++ = pApp_SC_FixedFrame->ControlCode;     	//������
	*FrameBuff++ = pApp_SC_FixedFrame->AidControlCode;  	//����������
	*FrameBuff++ = pApp_SC_FixedFrame->ADDH;            	//���з���ַ��λ
	*FrameBuff++ = pApp_SC_FixedFrame->ADDL;            	//���з���ַ��λ
	*FrameBuff++ = pApp_SC_FixedFrame->CS;              	//CSУ��λ			
	*FrameBuff++ = pApp_SC_FixedFrame->FrameStop;      		 //����λ0x16

	BSP_UART_Write(APP_485_COMM,SC_RXTXBUFF,7);		//����7���ֽ�
}

void App_SC_SendVariableFrame(INT8U ComPort,App_SC_VariableFrame_Struct *pApp_SC_VariableFrame)	//����һ���ɱ�֡
{
	INT8U* FrameBuff = SC_RXTXBUFF;
	INT8U i = 0;
	INT8U Lenth = (pApp_SC_VariableFrame->FrameLenthH *256) + pApp_SC_VariableFrame->FrameLenthL;		//�����򳤶�
	App_SC_ClearBuff(ComPort);		//����շ�������

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
	//�ӿ����뵽���е�ַ���ֽ�
	CS +=pApp_SC_FixedFrame->ControlCode;     	//������	
	CS +=pApp_SC_FixedFrame->AidControlCode;  	//��������
	CS +=pApp_SC_FixedFrame->ADDH;            	//���з���ַ��λ
	CS +=pApp_SC_FixedFrame->ADDL;            	//���з���ַ��λ

	pApp_SC_FixedFrame->CS = CS;
}
void App_SC_VariableFrameFillCSCheck(App_SC_VariableFrame_Struct *pApp_SC_VariableFrame)
{
	INT8U CS = 0;
	INT8U i;
	INT8U Lenth = (pApp_SC_VariableFrame->FrameLenthH *256)+pApp_SC_VariableFrame->FrameLenthL;		//�����򳤶�
	CS +=pApp_SC_VariableFrame->ControlCode;     	//������	
	CS +=pApp_SC_VariableFrame->ADDH;            	//���з���ַ��λ
	CS +=pApp_SC_VariableFrame->ADDL;            	//���з���ַ��λ
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
void App_SC_DealFixedFrame(App_SC_FixedFrame_Struct *pApp_SC_FixedFrame_Struct)		//����̶�֡
{
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x05)			//�������Լ���
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
		//TODO:�����Լ���
		Data[0] = 0x01;		//�Լ�Ľ��
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
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//�ϱ��ɱ�֡
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x07)			//ȡ��ǰʱ��.��վ��������ȡ��ǰʱ�䣬���������ؼ�������ʱ��
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
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//�ϱ��ɱ�֡//�ظ�Ӧ��֡
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x30)			//��ѯ��������ַ
	{
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;			//�ϱ��̶�֡
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//����Ӧ������
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//�ظ�Ӧ��֡
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x14)			//��ѯGPRS����		
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
		GWPara_function(&para_data);			//����վIP��ַ��E2�ж�����
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
		
		*p++ = IPBuff[4];		//port��
		*p++ = IPBuff[5];		//port��
		*p++ = 0x00;			//������
		dataLenth = 3+3;
		lenth = sprintf((char*)p,"%d",IPBuff[0]);		//��һ��IP
		p += lenth;
		*p++ = '.';
		dataLenth += lenth+1;
		lenth = sprintf((char*)p,"%d",IPBuff[1]);		//�ڶ���IP
		p += lenth;
		*p++ = '.'; 
		dataLenth += lenth+1;
		lenth = sprintf((char*)p,"%d",IPBuff[2]);		//������IP
		p += lenth;
		*p++ = '.';
		dataLenth += lenth+1;
		lenth = sprintf((char*)p,"%d",IPBuff[3]);		//���Ķ�IP
		*p++ = 0;
		
		pApp_SC_VariableFrame.FrameLenthL = dataLenth;
	
		pApp_SC_VariableFrame.FrameStop = 0x16;
		App_SC_VariableFrameFillCSCheck(&pApp_SC_VariableFrame);
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//�ϱ��ɱ�֡
	}
	if(pApp_SC_FixedFrame_Struct->ControlCode == 0x09)			//��ѯ����������汾��
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
		pApp_SC_VariableFrame.Data[0] = 0x10;		//TODO:���ɰ汾
		pApp_SC_VariableFrame.Data[1] = 0x10;
		pApp_SC_VariableFrame.CS = 0x00;		
		pApp_SC_VariableFrame.FrameStop = 0x16;
		App_SC_VariableFrameFillCSCheck(&pApp_SC_VariableFrame);
		App_SC_SendVariableFrame(APP_GPRS_COMM,&pApp_SC_VariableFrame);		//�ϱ��ɱ�֡
	}
}


void App_SC_DealVariableFrame(App_SC_VariableFrame_Struct *pApp_SC_VariableFrame_Struct)		//����ɱ�֡
{
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x01)		//У��ʱ��
	{
		_BSPRTC_TIME realTime;
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;
		//��ʱ��д��RTC
		realTime.Year = pApp_SC_VariableFrame_Struct->Data[0];				//��
		realTime.Month =pApp_SC_VariableFrame_Struct->Data[1];				//��
		realTime.Day=pApp_SC_VariableFrame_Struct->Data[2];					//��
		realTime.Hour=pApp_SC_VariableFrame_Struct->Data[3];				//ʱ
		realTime.Minute=pApp_SC_VariableFrame_Struct->Data[4];				//��
		realTime.Second=pApp_SC_VariableFrame_Struct->Data[5];				//��
		BSP_RTCSetTime(&realTime);						//����ʱ��
		//Ӧ��	
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//����Ӧ������
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH ;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.CS = 0x00;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//�ظ�Ӧ��֡
		
	}
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x12)		//����GPRS����
	{
		__App_Para_InterfaceStr para_data;	
		INT8U IPBuff[28];
		INT8U *p;
		INT8U i;
		p = pApp_SC_VariableFrame_Struct->Data;				//pָ��������
		IPBuff[4] = p[0];
		IPBuff[5] = p[1];
		//pApp_SC_VariableFrame_Struct->Data[2];			//������
		p += 3;
		for(i = 0; i<4; i++)		//4��IP��ַ
		{
			while(*p++ != '.')		//��⵽һ��'.'��һ��3��'.'
			{
				IPBuff[i] *= 10;
				IPBuff[i] += (*p - '0');
				if(*p == 0x00)			//�����⵽0x00����
					break;
			}
		}
		//��IP��Ϣд�뵽E2��
		Bsp_I2CSPM_Init();
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = IPBuff;				//
		para_data.dlen  =  sizeof(IPBuff) ;
		GWPara_function(&para_data);			//����վIP��ַ��E2�ж�����
		Bsp_I2CSPM_Disable();
	}
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x25)		//����RS485������				�˴������ʣ���ʲô���룬16���ƻ���BCD��
	{
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;			//�ϱ��̶�֡
		//����485������
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data);
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data + 1);
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data + 2);
		// = (INT8U)(pApp_SC_VariableFrame_Struct->Data + 3);
		//Ӧ��	
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//����Ӧ������
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.CS = 0x00;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//�ظ�Ӧ��֡
	}
	if(pApp_SC_VariableFrame_Struct->ControlCode == 0x31)		//���ü�������ַ
	{
		App_SC_FixedFrame_Struct pApp_SC_FixedFrame;			//�ϱ��̶�֡
		
		ADDH = pApp_SC_VariableFrame_Struct->Data[0];				//�����µ�ַ
		ADDL = pApp_SC_VariableFrame_Struct->Data[1];
		
		//Ӧ��	
		pApp_SC_FixedFrame.FrameStart = 0x10;
		pApp_SC_FixedFrame.ControlCode = 0xa0;		//����Ӧ������
		pApp_SC_FixedFrame.AidControlCode = 0x00;
		pApp_SC_FixedFrame.ADDH = ADDH;
		pApp_SC_FixedFrame.ADDL = ADDL;
		pApp_SC_FixedFrame.CS = 0x00;
		pApp_SC_FixedFrame.FrameStop = 0x16;
		
		App_SC_FixedFrameFillCSCheck(&pApp_SC_FixedFrame);
		App_SC_SendFixedFrame(APP_GPRS_COMM,&pApp_SC_FixedFrame);		//�ظ�Ӧ��֡
		
	}
}

/******************************************************************************
* Description: �ѽ��յ������ݴ���ú������ú����Զ�ʶ���ǹ̶�֡���ǿɱ�֡����ִ��ָ��
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
		if(pArray[i] == 0x68)			//��⵽�ɱ�֡֡ͷ
		{
			if(pArray[i+3] == 0x68)			//��⵽�ɱ�֡��־λ
			{
				len = pArray[i+1]*256+ pArray[i+2];			//֡��
				if(pArray[i+4+len] == 0x16)					//�ж�֡β
				{
					//if(CSУ��)	       	//�����CSУ�飬�����ȷ�������������
					{
						//TODO:����ɱ�֡
						App_SC_VariableFrame_Struct *pApp_SC_VariableFrame_Struct;
						memcpy(pApp_SC_VariableFrame_Struct,pArray+i,7);						
						memcpy(pApp_SC_VariableFrame_Struct->Data,pArray+i+7,len-7);
						pApp_SC_VariableFrame_Struct->CS = pArray[i+3+len];
						pApp_SC_VariableFrame_Struct->FrameStop = pArray[i+4+len];
						
						App_SC_DealVariableFrame(pApp_SC_VariableFrame_Struct);		//����ɱ�֡
						return 2;				//������յ��ɱ�֡
					}
				}
			}
		}
		if(pArray[i] == 0x68)			//��⵽�̶�֡֡ͷ
		{
			if(pArray[i+6] == 0x16)			//��7���ֽ���0x16����ʾ�յ�һ�������Ĺ̶�֡
			{
				//if(CSУ��)		//�����CSУ�飬�����ȷ�������������
				{
					//TODO:����̶�֡
					App_SC_FixedFrame_Struct *pApp_SC_FixedFrame_Struct;
					memcpy(pApp_SC_FixedFrame_Struct,pArray+i,sizeof(App_SC_FixedFrame_Struct));
					App_SC_DealFixedFrame(pApp_SC_FixedFrame_Struct);
					return 1;					//������յ��̶�֡
				}
			}
		}
	}
	return 0;		//����û�н��յ������߽�������
}

void App_SC_Report(void)
{
	//--��E2�ж�ȡ��������
	__App_Para_InterfaceStr para_data;

//	Array = App_Gprs_CMD_Buff.TxBuff;

	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0c0c;
	para_data.point = 1;
//	para_data.dbuff = App_Gprs_CMD_Buff.TxBuff + 14;
	para_data.dbuff = SC_RXTXBUFF + 14;
	para_data.dlen = 147;				//�޸�
	GWFirstData_function(&para_data);
	
}


/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
