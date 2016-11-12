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
* Description: ����շ�������
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_ClearBuff(INT8U ComPort)	//���������
{
	BSP_UART_ClearRxBuff(ComPort);
	BSP_UART_ClearTxBuff(ComPort);
}
/******************************************************************************
* Description: ���CSУ��
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_FillCSCheck(App_NL_Struct *pApp_NL_Struct)
{
	INT8U CS = 0;
	INT8U i = 0;
	INT16U Lenth = pApp_NL_Struct->DataLenth[0] + pApp_NL_Struct->DataLenth[1]*256;		//�����򳤶�
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
* Description: �Ӵ��ڷ���һ��NL�����ݽṹ��
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
	
	Lenth = pApp_NL_Struct->DataLenth[0] + pApp_NL_Struct->DataLenth[1]*256;		//�����򳤶�
	*p++ = pApp_NL_Struct->FrameStart;			//��ʼλ
	*p++ = pApp_NL_Struct->InfoDomain[0];		//��Ϣ��
	*p++ = pApp_NL_Struct->InfoDomain[1];
	*p++ = pApp_NL_Struct->DataLenth[0];		//�����򳤶�
	*p++ = pApp_NL_Struct->DataLenth[1];
	for(i=0; i<Lenth; i++)
	{
		*p++ = pApp_NL_Struct->Data[i];			//������
	}
	*p++ = pApp_NL_Struct->CS;					//CSУ��λ
	*p++ = pApp_NL_Struct->FrameStop;			//����λ
	BSP_UART_Write(ComPort,TXBuff,Lenth+7);			//�ѽṹ���ComPort���ͳ�ȥ
}
/******************************************************************************
* Description: ȷ��֡�ͷ���֡
* Input:  Nothing			//0��ȷ�ϣ�1�Ƿ���
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_Ask(INT8U ComPort,INT8U ask)				//Ӧ��
{
	App_NL_Struct pApp_NL_Struct;
	INT8U DataBuff[20];
	
	pApp_NL_Struct.Data = DataBuff;
	pApp_NL_Struct.FrameStart = 0x68;				//��ʼλ
	pApp_NL_Struct.InfoDomain[0] = 0x00;
	pApp_NL_Struct.InfoDomain[1] = 0x00;
	pApp_NL_Struct.DataLenth[0] = 0x06;				//5�ֽ�
	pApp_NL_Struct.DataLenth[1] = 0x00;
	pApp_NL_Struct.Data[0] = 0x01;						//���ݱ�ʶ
	pApp_NL_Struct.Data[1] = 0x00;						//���ݱ�ʶ
	pApp_NL_Struct.Data[2] = 0x01;						//����������
	pApp_NL_Struct.Data[3] = 0x00;						//�������
	pApp_NL_Struct.Data[4] = 0x00;						//�������
	pApp_NL_Struct.Data[5] = ask;					//ȷ�Ͻ��
	
	App_NL_FillCSCheck(&pApp_NL_Struct);
	pApp_NL_Struct.FrameStop = 0x16;				//����λ
	App_NL_SendStruct(ComPort,&pApp_NL_Struct);
}
/******************************************************************************
* Description:����һ��NL�Ľṹ��
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void App_NL_DealStruct(INT8U ComPort,App_NL_Struct *pApp_NL_Struct)
{
	__App_Para_InterfaceStr para_data;
	INT32U Datalenth;
	Datalenth = pApp_NL_Struct->DataLenth[0] + pApp_NL_Struct->DataLenth[1]*256;		//
	if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 6))		//�����ϱ�����
	{
//		GPRS_Interval = pApp_NL_Struct->Data[6];
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0401;
		para_data.point = 1;
		para_data.dbuff = pApp_NL_Struct->Data+5;			//ָ���ϱ�����
		para_data.dlen  =  1 ;
		GWPara_function(&para_data);							//���ϱ�����д��E2��
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 5))		//��ѯ�ϱ�����
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[10] = {0};
		pSend_NL_Struct.Data = DataBuff;

		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0401;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;			//ָ���ϱ�����
		para_data.dlen  =  1 ;
		GWPara_function(&para_data);							//���ϱ�����д��E2��
		
		pSend_NL_Struct.FrameStart = 0x68;				//��ʼλ
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x06;				//5�ֽ�
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x01;						//���ݱ�ʶ
		pSend_NL_Struct.Data[1] = 0x05;						//���ݱ�ʶ
		pSend_NL_Struct.Data[2] = 0x01;						//����������
		pSend_NL_Struct.Data[3] = 0x00;						//�������
		pSend_NL_Struct.Data[4] = 0x00;						//�������
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//����λ
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//���ز�ѯ��Ϣ
		
	}
	else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x02)&&(Datalenth == 0x21))		//������վIP��ַ�Ͷ˿ں�
	{
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = pApp_NL_Struct->Data+5;			//ָ��IP��ַ
		para_data.dlen  = 28 ;
		GWPara_function(&para_data);							//��IP��ַд��E2��
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡		
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x02)&&(Datalenth == 5))		//��ѯ��վIP��ַ�Ͷ˿ں�
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[35] = {0};
		pSend_NL_Struct.Data = DataBuff;
		
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0402;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;			//ָ��IP��ַ
		para_data.dlen  = 28 ;
		GWPara_function(&para_data);							//���ϱ�����д��E2��
		
		pSend_NL_Struct.FrameStart = 0x68;				//��ʼλ
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x21;				//33�ֽ�
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x02;						//���ݱ�ʶ
		pSend_NL_Struct.Data[1] = 0x05;						//���ݱ�ʶ
		pSend_NL_Struct.Data[2] = 0x01;						//����������
		pSend_NL_Struct.Data[3] = 0x00;						//�������
		pSend_NL_Struct.Data[4] = 0x00;						//�������
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//����λ
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//���ز�ѯ��Ϣ
	}
	else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x0D)&&(Datalenth == 0x0b))		//��ʱ
	{
		_BSPRTC_TIME realTime;

		realTime.Second = pApp_NL_Struct->Data[5];				//��
		realTime.Minute = pApp_NL_Struct->Data[6];				//��
		realTime.Hour = pApp_NL_Struct->Data[7];					//ʱ
		realTime.Day = pApp_NL_Struct->Data[8];				//��
		realTime.Month = pApp_NL_Struct->Data[9];				//��
		realTime.Year = pApp_NL_Struct->Data[10];				//��
		BSP_RTCSetTime(&realTime);	
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x0D)&&(Datalenth == 5))		//��ѯ��������ʱ��
	{
		_BSPRTC_TIME RTCTime;
		
		BSP_RTCGetTime(&RTCTime);	
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[20];
	
		pSend_NL_Struct.Data = DataBuff;
		pSend_NL_Struct.FrameStart = 0x68;				//��ʼλ
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x0b;				//11�ֽ�
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x0D;						//���ݱ�ʶ
		pSend_NL_Struct.Data[1] = 0x05;						//���ݱ�ʶ
		pSend_NL_Struct.Data[2] = 0x01;						//����������
		pSend_NL_Struct.Data[3] = 0x00;						//�������
		pSend_NL_Struct.Data[4] = 0x00;						//�������
		pSend_NL_Struct.Data[5] = RTCTime.Second;
		pSend_NL_Struct.Data[6] = RTCTime.Minute;
		pSend_NL_Struct.Data[7] = RTCTime.Hour;
		pSend_NL_Struct.Data[8] = RTCTime.Day;
		pSend_NL_Struct.Data[9] = RTCTime.Month;
		pSend_NL_Struct.Data[10] = RTCTime.Year;

		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//����λ
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//���ز�ѯ��Ϣ

	}
	else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x0E)&&(Datalenth == 0x0c))		//���ü������߼���ַ
	{
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = pApp_NL_Struct->Data+5;			//ָ�������߼���ַ
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);							//�Ѽ������߼���ַд��E2
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}
	else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x0E)&&(Datalenth == 5))		//��ѯ�������߼���ַ
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[20]={0};
		
		pSend_NL_Struct.Data = DataBuff;
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;			//ָ���ַ��
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);							//�ѵ�ַ��E2�ж�ȡ����
				
		pSend_NL_Struct.FrameStart = 0x68;				//��ʼλ
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x0c;				//12�ֽ�
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x0E;						//���ݱ�ʶ
		pSend_NL_Struct.Data[1] = 0x05;						//���ݱ�ʶ
		pSend_NL_Struct.Data[2] = 0x01;						//����������
		pSend_NL_Struct.Data[3] = 0x00;						//�������
		pSend_NL_Struct.Data[4] = 0x00;						//�������
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//����λ
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//���ز�ѯ��Ϣ
	}
	else if((pApp_NL_Struct->Data[1] == 0x08)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 5))		//��ѯ�������汾��Ϣ
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[20];
	
		pSend_NL_Struct.Data = DataBuff;
		pSend_NL_Struct.FrameStart = 0x68;				//��ʼλ
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x13;				//19�ֽ�
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x01;						//���ݱ�ʶ
		pSend_NL_Struct.Data[1] = 0x08;						//���ݱ�ʶ
		pSend_NL_Struct.Data[2] = 0x01;						//����������
		pSend_NL_Struct.Data[3] = 0x00;						//�������
		pSend_NL_Struct.Data[4] = 0x00;						//�������
		pSend_NL_Struct.Data[5] = (INT8U)SW_Version;			//����汾
		pSend_NL_Struct.Data[6] = (INT8U)(SW_Version>>8);	
		pSend_NL_Struct.Data[7] = (INT8U)(SW_Version>>16);	
		pSend_NL_Struct.Data[8] = (INT8U)(SW_Version>>24);	
		pSend_NL_Struct.Data[9] = SW_ReleaseData[0];			//����汾����ʱ��	
		pSend_NL_Struct.Data[10]= SW_ReleaseData[1];	
		pSend_NL_Struct.Data[11] = SW_ReleaseData[2];	
		pSend_NL_Struct.Data[12] = (INT8U)HW_Version;			//Ӳ���汾
		pSend_NL_Struct.Data[13] = (INT8U)(HW_Version>>8);	
		pSend_NL_Struct.Data[14] = (INT8U)(HW_Version>>16);	
		pSend_NL_Struct.Data[15] = (INT8U)(HW_Version>>24);	
		pSend_NL_Struct.Data[16] = HW_ReleaseData[0];			//Ӳ���汾����ʱ��
		pSend_NL_Struct.Data[17] = HW_ReleaseData[1];	
		pSend_NL_Struct.Data[18]=  HW_ReleaseData[2];	
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//����λ
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//���ز�ѯ��Ϣ
		
	}else if((pApp_NL_Struct->Data[1] == 0x04)&&(pApp_NL_Struct->Data[0] == 0x06)&&(Datalenth == 0x16))		//���ü������������ò���	
	{
		INT8U Buff[10];
		INT8U JAddress[7];
		memcpy(Buff,pApp_NL_Struct->Data+5,9);					//ͨѶ���ʼ��˿ڣ�ͨѶЭ�����ͣ������ַ
		memcpy(JAddress,pApp_NL_Struct->Data+5+9,7);				//��������ַ
		Buff[9] = pApp_NL_Struct->Data[5+16];					//�û����С���
		/*��������ַ*/
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = JAddress;						//ָ�������߼���ַ
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);							//�Ѽ������߼���ַд��E2
		/*�����������*/
		para_data.afn = METER_WRITE;	
		para_data.IDIndex = 0x0406;
		para_data.point = 1;
		para_data.dbuff = Buff;						//�Ѽ�������������д��E2
		para_data.dlen  =  10 ;
		GWPara_function(&para_data);							//���ϱ�����д��E2��
		
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}else if((pApp_NL_Struct->Data[1] == 0x05)&&(pApp_NL_Struct->Data[0] == 0x06)&&(Datalenth == 0x05))		//��ѯ�������������ò���
	{
		App_NL_Struct pSend_NL_Struct;
		INT8U DataBuff[30];
		/*�����������*/
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x0406;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5;						//�Ѽ������������ݶ���
		para_data.dlen  =  10 ;
		GWPara_function(&para_data);	
		DataBuff[21] = DataBuff[14];						//�����С���
		/*��������ַ*/
		para_data.afn = METER_READ;	
		para_data.IDIndex = 0x040e;
		para_data.point = 1;
		para_data.dbuff = DataBuff+5+9;						//ָ�������߼���ַ
		para_data.dlen  = 7 ;
		GWPara_function(&para_data);
		pSend_NL_Struct.Data = DataBuff;
		pSend_NL_Struct.FrameStart = 0x68;				//��ʼλ
		pSend_NL_Struct.InfoDomain[0] = 0x00;
		pSend_NL_Struct.InfoDomain[1] = 0x00;
		pSend_NL_Struct.DataLenth[0] = 0x16;				//17+5�ֽ�
		pSend_NL_Struct.DataLenth[1] = 0x00;
		pSend_NL_Struct.Data[0] = 0x06;						//���ݱ�ʶ
		pSend_NL_Struct.Data[1] = 0x05;						//���ݱ�ʶ
		pSend_NL_Struct.Data[2] = 0x01;						//����������
		pSend_NL_Struct.Data[3] = 0x00;						//�������
		pSend_NL_Struct.Data[4] = 0x00;						//�������
		App_NL_FillCSCheck(&pSend_NL_Struct);
		pSend_NL_Struct.FrameStop = 0x16;				//����λ
		App_NL_SendStruct(ComPort,&pSend_NL_Struct);			//���ز�ѯ��Ϣ
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x01)&&(Datalenth == 5))		//Ӳ����ʼ��
	{
		Bsp_Wdt_Init();				//�򿪿��Ź�����ι����ʱ�䵽�Ժ������
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)0);		//���еƵ���
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)1);
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)2);
		Bsp_Led_SetH((__Bsp_Enum_Led_Name)3);
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
		while(1);
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x02)&&(Datalenth == 5))		//��������ʼ��
	{
		Reset_ONEDATA_Function();							//���E2�еĳ�������
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x03)&&(Datalenth == 5))		//�ظ���������
	{
		Reset_Para_All();									//���E2����������
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}else if((pApp_NL_Struct->Data[1] == 0x06)&&(pApp_NL_Struct->Data[0] == 0x04)&&(Datalenth == 5))		//����վ��Ϣ�����ʼ��
	{
		Reset_Para_Not_Comunication();						//���E2�г���վ��Ϣ�������
		App_NL_Ask(ComPort,0);					//�ظ�ȷ��֡
	}
	else	//û����Ӧ��ָ��Ĵ��룬����Ӧ�����
	{
		App_NL_Ask(ComPort,1);					//�ظ�ȷ��֡
	}
	
}

/******************************************************************************
* Description: �Ӵ��ڽ���һ��NL�����ݽṹ�岢����App_NL_DealStruct()����
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
		if(pArray[i] == 0x68)								//֡��ʼλ	
		{
			DataLenth = pArray[i+3] + pArray[i+4]*256;		//�����򳤶�
			if(pArray[i+DataLenth+6] == 0x16)				//֡����λ
			{
				//if(CS??)	    				//�����ݴ���ṹ��
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
					App_NL_DealStruct(ComPort,&pApp_NL_Struct);				//����ṹ��
					return 1;				//���յ�
				}
			}
		}
	}
	return 0;					//δ���յ�
}

/******************************************************************************
* Description: �ϱ����ݣ�����GPRSģ�����
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
	BSP_RTCGetTime(&RTCTime);		//��ȡRTCʱ��
		/*���֡ͷ*/
	pApp_NL_Struct.Data = DataBuff;
	pApp_NL_Struct.FrameStart= 0x68;
	pApp_NL_Struct.InfoDomain[0] = 0x40;			//���
	pApp_NL_Struct.InfoDomain[1] = 0x01;			//��Ҫ�ظ�
	pApp_NL_Struct.DataLenth[0] =  0x56;				//86�ֽ�
	pApp_NL_Struct.DataLenth[1] =  0x00;
		/*���������ǰ��������*///δ���
	DataBuff[0] = 0x02;						//0C02	���ݱ�ʶ
	DataBuff[1] = 0x0c;
	memset(DataBuff+2,0xaa,20);				//20�ֽ� ICCID
	memset(DataBuff+22,0xbb,8);				//8�ֽ�	������Ϣ
	DataBuff[30] = 0x01;			//����������
	DataBuff[31] = 0x01;			//�ϱ�����
	DataBuff[32] = RTCTime.Day;				//3�ֽ��ϱ�ʱ��
	DataBuff[33] = RTCTime.Month;
	DataBuff[34] = RTCTime.Year;
	/*��ȡ��������*/			//�˴����ص��Ǵ�5���ֽڵ�ʱ�꣬��Ҫ��ǰд5���ֽڣ�Ȼ��д7�ֽ��߼���ַ���ǵ�ʱ��
	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0c01;
	para_data.point = 1;
	para_data.dbuff = DataBuff+43-5;
	para_data.dlen  = 48;
	GWFirstData_function(&para_data);	
		/*��ȡ�߼���ַ*/
	para_data.afn = METER_READ;			//7�ֽڵ�ַ
	para_data.IDIndex = 0x040e;
	para_data.point = 1;
	para_data.dbuff = DataBuff+35;
	para_data.dlen  = 7;
	GWPara_function(&para_data);			//��ȡ�߼���ַ
	/*��ȡ��С���*/
	para_data.afn = METER_READ;	
	para_data.IDIndex = 0x0406;
	para_data.point = 1;
	para_data.dbuff = Buff;						//�Ѽ������������ݶ���
	para_data.dlen  =  10 ;
	GWPara_function(&para_data);	
	DataBuff[42] = Buff[9];					//�������(��С���)				
	/*���֡β*/
	App_NL_FillCSCheck(&pApp_NL_Struct);
	pApp_NL_Struct.FrameStop = 0x16;
	/*�ϱ���������*/
	App_NL_SendStruct(APP_GPRS_COMM,&pApp_NL_Struct);		//��GPRS�˿ڷ��ͳ�������		
	
}









