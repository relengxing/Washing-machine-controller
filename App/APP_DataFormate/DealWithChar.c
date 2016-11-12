/*****************************************Copyright(C)******************************************
*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName			: DealWithChar.c
* Author			: 
* Date First Issued	: 2007.05.21
* Version			: V1.0
* Description		: ���ݸ�ʽת��
*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
* History			:
* //2008			: 
* Description		: 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
/* Includes-----------------------------------------------------------------------------------*/
//#include "app.h"
#include "Bsp_config.h"		   
/* Private define-----------------------------------------------------------------------------*/
/* Private typedef----------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables--------------------------------------------------------------------------*/
const INT32U Base[10] = 
{
   	1000000000,
	100000000,
	10000000,
	1000000,
	100000,
	10000,
	1000,
	100,
	10,
	1
};
/* Private function prototypes----------------------------------------------------------------*/
//���ɸ����ֽ����ݺϳɶ��ֽڵ���������
INT16U TwoByteToShort(const INT8U *Point);
INT32U ThreeByteToInt(INT8U *Point);
INT32U FourByteToInt(INT8U *Point);
INT64U FiveByteToLong(INT8U *Point);
//���Ͳ�ֳɵ��ֽ�����
void ShortToByte(INT16U Source, INT8U *Target);
void IntToFourByte(INT32U Source, INT8U *Target);
void LongToFiveByte(INT64U Source, INT8U *Target);
//BCD��ת����Hex
INT8U ByteBcdToHex(INT8U Source);
INT16U TwoByteBcdToHex(INT8U *Point);
INT32U ThreeByteBcdToHex(INT8U *Point);
INT32U FourByteBcdToHex(INT8U *Point);
INT64U FiveByteBcdToHex(INT8U *Point);
//Hexת����BCD��
INT8U ByteHexToBcd(INT8U Source);
INT16U ShortHexToBCD(INT16U Source);
INT32U IntHexToBcd(INT32U Source);
INT64U LongHexToBcd(INT64U Source);
//BCD��Hex��ӻ����
INT8U BcdAddHex(INT8U YuBcd,INT8U MuHex);
INT8U BcdDccHex(INT8U YuBcd,INT8U MuHex);
//����ר�����ݸ�ʽת��
INT64S Dada02ToINT64S(INT16U Source); 
INT16U INT64SToDada02(INT64S Source);
INT32S Dada03ToINT32S(INT32U Source);
INT32U INT64SToDada03(INT64S Source);
//������������У��
INT8U TeskCS(INT8U*Buff,INT16U len);
//ȡ����ֵ
INT32U Labs(long v);
/* Private functions--------------------------------------------------------------------------*/
/////////////////////////////////////////////////////
//���ɸ����ֽ����ݺϳɶ��ֽڵ���������////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
�������ƣ�INT16U TwoByteToShort(INT8U *Point)
���������Point--2�ֽ������׵�ַ
���������RetVal--ת����ɵ���������
Լ����������
������������2�ֽ�8λ����ת����16λ�Ķ����������ӳ���
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT16U TwoByteToShort(const INT8U *Point)
{
	INT16U RetVal;
	
	RetVal = *Point + (*(Point+1)<<8);
	
	return RetVal;
}

/*****************************************************************   
�������ƣ�INT32U ThreeByteToInt(INT8U *Point)
���������Point--3�ֽ������׵�ַ
���������RetVal--ת����ɵ���������
Լ����������
������������3�ֽ�����ת����32λ���������ӳ���
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����    
*****************************************************************/ 
INT32U ThreeByteToInt(INT8U *Point)
{
	INT32U RetVal, Temp;
	INT16U i;	
	RetVal = 0;
	for(i = 0; i < 3; i++)
	{
		Temp = *(Point+i);
		
		RetVal += Temp << (8 * i);
	}	
	return RetVal;
}

/*****************************************************************   
�������ƣ�INT32U FourByteToInt(INT8U *Point)
���������Point--4�ֽ������׵�ַ
���������RetVal--ת����ɵ���������
Լ����������
������������4�ֽ�����ת����32λ���������ӳ���
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT32U FourByteToInt(INT8U *Point)
{
	INT32U RetVal, Temp;
	INT16U i;
	
	RetVal = 0;
	for(i = 0; i < 4; i++)
	{
		Temp = *(Point+i) ;
		
		RetVal += Temp << (8 * i);
	}
	
	return RetVal;
}

/*****************************************************************   
�������ƣ�INT64U FiveByteToLong(INT8U *Point)
���������Point--5�ֽ������׵�ַ
���������RetVal--ת����ɵ���������
Լ����������
������������5�ֽ�����ת����64λ���������ӳ���
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT64U FiveByteToLong(INT8U *Point)
{
	INT8U i;	
	INT64U RetVal, Temp;	
	RetVal = 0;
	for(i = 0; i < 5; i++)
	{
		Temp = *(Point+i) & 0x000000FF;		
		RetVal += Temp << (8 * i);
	}	
	return RetVal;
}

/////////////////////////////////////////////////////
//���Ͳ�ֳɵ��ֽ�����///////////////////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
�������ƣ�void ShortToByte(INT16U Source, INT8U *Target)
���������Source-��ת���Ķ���������   Target--2�ֽ������׵�ַ
�����������
Լ����������
������������16λ����������ת����2�ֽڵ�8λ����
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
void ShortToByte(INT16U Source, INT8U *Target)
{
	Target[0] = Source & 0xff;
	Target[1] = (Source >> 8)&0xff;
}	
/*****************************************************************   
�������ƣ�void IntToFourByte(INT32U Source, INT8U *Target)
���������Source-��ת������������   Target--4�ֽ������׵�ַ
�����������
Լ����������
������������32λ��������ת����4�ֽڵ�8λ����
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
void IntToFourByte(INT32U Source, INT8U *Target)
{
	Target[0] = (INT8U)(Source & 0xff);
	Target[1] = (INT8U)((Source >> 8)&0xff);
	Target[2] = (INT8U)((Source >> 16)&0xff);
	Target[3] = (INT8U)((Source >> 24)&0xff);
}	
/*****************************************************************   
�������ƣ�void LongToFiveByte(INT64U Source, INT8U *Target)
���������Source-��ת������������   Target--5�ֽ������׵�ַ
�����������
Լ����������
������������64λ��������ת����5�ֽڵ�8λ����
��    �ߣ�	                ����: 2007.09.28 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
void LongToFiveByte(INT64U Source, INT8U *Target)
{
	Target[0] = (INT8U)(Source & 0xff);
	Target[1] = (INT8U)((Source >> 8) & 0xff);
	Target[2] = (INT8U)((Source >> 16) & 0xff);
	Target[3] = (INT8U)((Source >> 24) & 0xff);
	Target[4] = (INT8U)((Source >> 32) & 0xff);
}

/////////////////////////////////////////////////////
//BCD��ת����Hex///////////////////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
�������ƣ�INT8U ByteBcdToHex(INT8U Source)
���������Source-��ת����BCD������   
���������ת�����Hex����
Լ����������
���������������ֽ�BCD��ת����ʮ������
��    �ߣ�                     ����: 
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT8U ByteBcdToHex(INT8U Source)
{
	INT8U Temp1, Temp2;
	
	Temp1 = (Source & 0xF0) >> 4;
	Temp2 = Source & 0x0F;
	Temp1 = Temp1 * 10 + Temp2;
	return Temp1;
}

/*****************************************************************   
�������ƣ�INT16U TwoByteBcdToHex(INT8U *Point)
���������Point-��ת����BCD�����ݴ洢�׵�ַ   
���������ת����Ķ�����Hex����
Լ����������
������������2�ֽ�BCD��ת����ʮ������
��    �ߣ�                     ����:  
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT16U TwoByteBcdToHex(INT8U *Point)
{
	INT16U RetVal;
	
	RetVal =  ByteBcdToHex(*Point);
	RetVal += ByteBcdToHex(*(Point+1)) * 100;
	return RetVal;
}

/*****************************************************************   
�������ƣ�INT32U ThreeByteBcdToHex(INT8U *Point)
���������Point-��ת����BCD�����ݴ洢�׵�ַ   
���������ת���������Hex����
Լ����������
������������3�ֽ�BCD��ת����ʮ������
��    �ߣ�                     ����:  
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT32U ThreeByteBcdToHex(INT8U *Point)
{
	INT32U RetVal;
	
	RetVal =  ByteBcdToHex(*Point);
	RetVal += ByteBcdToHex(*(Point+1)) * 100;
	RetVal += (INT32U)(ByteBcdToHex(*(Point+2))) * 10000;
	return RetVal;
}

/*****************************************************************   
�������ƣ�INT32U FourByteBcdToHex(INT8U *Point)
���������Point-��ת����BCD�����ݴ洢�׵�ַ   
���������ת���������Hex����
Լ����������
������������4�ֽ�BCD��ת����ʮ������
��    �ߣ�                     ����:  
��    �ģ�                     ����:  
��    ����  
*****************************************************************/ 
INT32U FourByteBcdToHex(INT8U *Point)
{
	INT32U RetVal, Temp1;
	
	RetVal = ByteBcdToHex(*Point);
	RetVal += ByteBcdToHex(*(Point+1)) * 100;
	Temp1 = (INT32U)(ByteBcdToHex(*(Point+2)));	
	RetVal += Temp1 * 10000;
	Temp1 = (INT32U)(ByteBcdToHex(*(Point+3)));
	RetVal += Temp1 * 1000000;
		          		
	return RetVal;
}

/*****************************************************************   
�������ƣ�INT64U FiveByteBcdToHex(INT8U *Point)
���������Point-��ת����BCD�����ݴ洢�׵�ַ   
���������ת���������Hex����
Լ����������
������������5�ֽ�BCD��ת����ʮ������
��    �ߣ�                     ����:  
��    �ģ�                     ����:  
��    ����  
*****************************************************************/
INT64U FiveByteBcdToHex(INT8U *Point)
{
	INT64U RetVal, Temp1;
	RetVal = ByteBcdToHex(*Point);
	RetVal += ByteBcdToHex(*(Point+1)) * 100;
	Temp1 = (INT32U)(ByteBcdToHex(*(Point+2)));	
	RetVal += Temp1 * 10000;
	Temp1 = (INT32U)(ByteBcdToHex(*(Point+3)));
	RetVal += Temp1 * 1000000;
	Temp1 = (INT64U)(ByteBcdToHex(*(Point+4)));
	RetVal += Temp1 * 100000000;	          		
	return RetVal;
}


/////////////////////////////////////////////////////
//Hexת����BCD��///////////////////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
�������ƣ�INT8U ByteHexToBcd(INT8U Source)
���������Source-��ת����Hex����   
���������ת�����BCDֵ
Լ����������
�������������ֽ�ʮ������ת����BCD��
��    �ߣ�                     ����:  
��    �ģ�                     ����:  	 �е�����ģ����
��    ����  
*****************************************************************/
INT8U ByteHexToBcd(INT8U Source)
{
	INT8U Temp1, Temp2;
	Temp1 = (Source %100)/ 10;		// �޸ģ���ȥ��λ���ֵ�ֵ
	Temp2 = Source % 10;
	Temp1 = (Temp1 << 4) + Temp2;
	return Temp1;
}

/*****************************************************************   
�������ƣ�INT16U ShortHexToBCD(INT16U Source) 
���������Source--��ת����short��Hex����
���������Result--ת����ɵ�BCD����
Լ����������
����������16λ16��������ת����16λBCD������	  
��    �ߣ�				����: 2007.11.14 
��    �ģ�				����:  
��    ����  
*****************************************************************/
INT16U ShortHexToBCD(INT16U Source)
{	
	INT16U Result,Temp1;
	Result=0;
	Temp1=Source/10000;
	if(Temp1)
	{
		Result=0x9999;
	}
	else
	{
		Temp1=Source/1000;
		Result+=(Temp1<<12);
		Temp1=Source%1000/100;
		Result+=(Temp1<<8);
		Temp1=Source%1000%100/10;
		Result+=(Temp1<<4);
		Temp1=Source%1000%100%10;
		Result+=(Temp1);
	}
	return Result;
}

/*****************************************************************   
�������ƣ�INT32U IntHexToBcd(INT32U Source)
���������Source-��ת����Hex����   
���������ת���������BCD������
Լ����������
���������������͵�ʮ��������ת����BCD��
��    �ߣ�                     ����:  
��    �ģ�                     ����:  		 �����
��    ����  
*****************************************************************/
INT32U IntHexToBcd(INT32U Source)
{
	INT32U Result, tBase[8], Temp1, Temp2;
	int i, j;

	tBase[0] = 10000000;
	tBase[1] = 1000000;
	tBase[2] = 100000;
	tBase[3] = 10000;
	tBase[4] = 1000;
	tBase[5] = 100;
	tBase[6] = 10;
	tBase[7] = 1;

	Result = 0;		
	for(i = 0; i < 8; i++)
	{
		Temp1 = 0;  
		Temp2 = 0;
		for(j = 0; j < 10; j++)
		{
			Temp1 += tBase[i];
			if(Source < Temp1)
			{
				Result = (Result << 4);
				Result = (Result | j);
				Source -= Temp2;
				break;
			}
			Temp2 = Temp1;			
		}
	}		
	return Result;
}

///*****************************************************************   
//�������ƣ�INT32U IntHexToBcd(INT32U Source)
//���������Source-��ת����Hex����   
//���������ת���������BCD������
//Լ����������
//���������������͵�ʮ��������ת����BCD��
//��    �ߣ�                     ����:  
//��    �ģ�                     ����:  	  ������� ����
//��    ����  
//*****************************************************************/
//INT64U IntHexToBcd(INT32U Source)
//{
//	INT64U result = 0 ;
//	INT64U j;
//	for(j =0 ; j < 10 ; j ++)//ת���������10λ����
//	{
//		INT64U tmp ;
//		tmp = Source%10 ;
//		Source /= 10 ;
//		result |= tmp<<(j*4) ;
//	}
//	return result ;
//}







/*****************************************************************   
�������ƣ�INT64U LongHexToBcd(INT64U Source)
���������Source-��ת����BCD������   
���������ת����ĳ�����Hex����
Լ����������
�����������������͵�ʮ��������ת����BCD��
��    �ߣ�                     ����:  
��    �ģ�                     ����:  
��    ����  
*****************************************************************/
INT64U LongHexToBcd(INT64U Source)
{
	INT64U  Result, Temp1, Temp2;
	INT8U i, j;
		
	Result = 0;		
	for(i = 0; i < 10; i++)
	{
		Temp1 = 0;  
		Temp2 = 0;
		for(j = 0; j < 10; j++)
		{
			Temp1 += Base[i];
			if(Source < Temp1)
			{
				Result = (Result << 4);
				Result = (Result | j);
				Source -= Temp2;
				break;
			}
			Temp2 = Temp1;			
		}
	}		
	return Result;
}

//����ֵ BCD
//*******************************************************************/   
INT8U BcdAddHex(INT8U YuBcd,INT8U MuHex)
{
	YuBcd=ByteBcdToHex(YuBcd);
	YuBcd+=MuHex;
	if(YuBcd>99)
	{	
		YuBcd-=100;
	}
	return ByteHexToBcd(YuBcd);
}  
/********************************************************************
BCD-Hex=BCD��
*********************************************************************/
//����ֵ BCD
//*******************************************************************/   
INT8U BcdDccHex(INT8U YuBcd,INT8U MuHex)
{
	INT16U DD;
	DD=ByteBcdToHex(YuBcd);
	if(DD<MuHex)DD+=100;
	if(DD<MuHex)DD+=100;
	if(DD<MuHex)DD+=100;
	DD-=MuHex;
	YuBcd=DD&0xff;
	return ByteHexToBcd(YuBcd);
}

//����ר�����ݸ�ʽת��
/*****************************************************************   
�������ƣ�INT64S Dada02ToINT64S(INT16U Source)
���������Source--��ת��������
���������Val--ת����ɵ�����
Լ����������
���������������ݸ�ʽ02������ת����64λ��16��������  
��    �ߣ�				����: 2007.09.12 
��    �ģ�				����:  
��    ����  
*****************************************************************/
INT64S Dada02ToINT64S(INT16U Source)
{
	INT8U Expo;
	INT16U Temp;
	INT64S Val;
	INT64U DataTemp;
	
	Temp=Source&0x0fff;
	Temp=TwoByteBcdToHex((INT8U*)&Temp);
	
	DataTemp=(INT64U)(Temp);	
	Expo=(INT8U)((Source&0xe000)>>13);
	switch(Expo)
	{
		case 0:
			DataTemp=DataTemp*10000000;
			break;
		case 1:
			DataTemp=DataTemp*1000000;
			break;
		case 2:
			DataTemp=(DataTemp*100000);
			break;
		case 3:
			DataTemp=(DataTemp*10000);
			break;
		case 4:
			DataTemp=(DataTemp*1000);
			break;
		case 5:
			DataTemp=(DataTemp*100);
			break;
		case 6:
			DataTemp=(DataTemp*10);
			break;
		case 7:
			DataTemp=(DataTemp*1);
			break;		
		default:
			break;
	}
	if(Source&0x1000)
		Val=-(INT64S)(DataTemp);
	else
		Val=(INT64S)(DataTemp);
	return Val;
}


/*****************************************************************   
�������ƣ�INT16U INT64SToDada02 (INT64S Source)
���������Source--��ת��������
���������Val--ת����ɵ�����
Լ����������
������������INT64S���͵�HexֵSourceת����INT16U���͵�02��ʽ���ݡ�����ֵΪʵ��Ҫת����ֵ��1000���� 
��    �ߣ�				����: 2007.11.14 
��    �ģ�				����:  
��    ����  
*****************************************************************/
INT16U INT64SToDada02 (INT64S Source)
{
	INT8U SignFlag;
	INT16U Expo;
	INT16U Val;
	INT64U DataTemp;
	
    Expo=0;
	SignFlag=0;
	if(Source&0x8000000000000000)
	{
		SignFlag=1;
		DataTemp=( INT64U)(-Source);
	}
	else
	{
		DataTemp=( INT64U)(Source);
	}
	if(DataTemp/1000000000)
	{
		Val=(INT16U)(DataTemp/10000000);
		Expo=0;
	}
	else if(DataTemp/100000000)
	{
		Val=(INT16U)(DataTemp/1000000);
		Expo=1;
	}
	else if(DataTemp/10000000)
	{
		Val=(INT16U)(DataTemp/100000);
		Expo=2;
	}
	else if(DataTemp/1000000)
	{
		Val=(INT16U)(DataTemp/10000);
		Expo=3;
	}	
	else if(DataTemp/100000)
	{
		Val=(INT16U)(DataTemp/1000);
		Expo=4;
	}
	else if(DataTemp/10000)
	{
		Val=(INT16U)(DataTemp/100);
		Expo=5;
	}
	else if(DataTemp/1000)
	{
		Val=(INT16U)(DataTemp/10);
		Expo=6;
	}
	else
	{
		Val=(INT16U)(DataTemp);
		Expo=7;
	}
	Val= ShortHexToBCD (Val);
	if(SignFlag)
	{
		Val|=0x1000;
	}
	Val|=(Expo<<13);	
	return Val;
}


/*****************************************************************   
�������ƣ�INT32S Dada03ToINT32S(INT32U Souce) 
���������Source--��ת��������
���������DataTemp--ת����ɵ�����
Լ����������
������������03���ݸ�ʽ������ת����16�����з�������  
��    �ߣ�				����: 2007.09.12 
��    �ģ�				����:  
��    ����  
*****************************************************************/
INT32S Dada03ToINT32S(INT32U Souce)
{
	INT32U Temp;
	INT32S DataTemp;
	
	Temp=Souce&0x0fffffff;
	Temp=FourByteBcdToHex((INT8U*)&Temp);
	DataTemp=(INT32S)Temp;

	if(Souce&0x10000000)//��ֵ
	{
		DataTemp=-DataTemp;
	}

	return DataTemp;
}

/*****************************************************************   
�������ƣ�INT64S Dada03ToINT64S(INT32U Souce)
���������Source--��ת��������
���������ת����ɵ�����
Լ����������
������������03���ݸ�ʽ������ת����16�����з�������  
��    �ߣ�fangjb		����: 2010.05.31 
*****************************************************************/
INT64S Dada03ToINT64S(INT32U Souce)
{
	INT32U temp;
	INT64S dataTemp;
	
	temp=Souce&0x0fffffff;
	temp=FourByteBcdToHex((INT8U*)&temp);
	dataTemp=(INT64S)temp;
	if(Souce&0x40000000)
		dataTemp *= 1000 ;	
	if(Souce&0x10000000)//��ֵ
		dataTemp=-dataTemp;
	return dataTemp;
}


/*****************************************************************   
�������ƣ�INT32U INT64SToDada03(INT64S Souce) 
���������Source--��ת��������
���������Temp--ת����ɵ�����
Լ����������
������������16�����з�������ת����03���ݸ�ʽ  
��    �ߣ�				����: 2007.09.12 
��    �ģ�	GQ			����: 2008.12.18 
��    ����  
*****************************************************************/
INT32U INT64SToDada03(INT64S Source)
{
	INT8U Singal;
	INT8U Danwei;
	INT32U Temp;
	INT64U DataTemp;

	Singal = 0;
	Danwei = 0;
	Temp = 0;
//	if(Source&0x8000000000000000)// SourceΪ��ֵ
	if(Source<0)
	{
		DataTemp = Source*(-1);
		Singal = 1;
	}
	else
		DataTemp = Source;
    if(DataTemp > 9999999)
	{
		DataTemp /= 1000;
		Danwei = 1;
	}
	Temp = IntHexToBcd(DataTemp);
	if(Singal)
		Temp |= 0x10000000;
	if(Danwei)
		Temp |= 0x40000000;

	return Temp;
}


INT32U INT32UToDada03(INT32U Source)
{
	INT8U signFlag;
	INT8U unitFlag;
	INT32U temp;
	signFlag = 0;
	unitFlag = 0;
	temp = Source;
	if(Source&0x80000000)// SourceΪ��ֵ
	{
		temp &=0x7fffffff;
		signFlag = 1;
	}
    if(temp > 9999999)
	{
		temp /= 1000;
		unitFlag = 1;
	}
	temp = IntHexToBcd(temp);	
	if(signFlag)
		temp |= 0x10000000;
	if(unitFlag)
		temp |= 0x40000000;
	return temp;
}

/*******************************************************
 ������ TeskCS 
 ���ܣ� ������������У��
 ����ԭ����У�����ݺ�У�����ۼӺ�Ϊ0x00
 Buff:��������ָ��
 len :�������ݳ���
 ����ֵ��У����
********************************************************/
INT8U TeskCS(INT8U*Buff,INT16U len)
{
     INT8U reval=0;
     INT16U i;
     for(i=0;i<len;i++)
     {
        reval+=*Buff++;
     }
     reval=0xff-reval;
     return reval+1;
}

/*****************************************************************   
�������ƣ�INT32U Labs(long v)
����������з�������ֵ
�������������ֵ
Լ����������
������������ȡ����ֵ
ȫ�ֱ�����
����ģ�飺
��    �ߣ����Ľ�			����: 2007.05.27 
��    �ģ�					����:  
��    ����V1.0
*****************************************************************/ 
INT32U Labs(long v)
{
 	INT32U rt;
 	
 	if(v >= 0) rt = (INT32U)v;
 	else       rt = (INT32U)(-1 * v);
 	return rt;
}
/**/
INT8U FineMaxDataInverse(INT8U *pSData,INT8U *pDData,INT8U len)
{
	while(len--)
	{
		if((*pSData+len)>(*pDData+len))
			return TRUE;
	}
	return FALSE;
}


/*************************************************
 ** ��������������ƽ����
 ** ��    �룺numberΪfloat��
 ** ��    ����ƽ����ΪINT32U 
**************************************************/
INT32U InvSqrt(float number) { 
	
    INT32S i; 

    float y; 

    y  = number; 

    i  = * ( INT32S * ) &y; 

    i  = 0x5f3759df - ( i >> 1 ); 

    y  = * ( float * ) &i; 
	
	y = 0.5F / y + 0.5F * number * y;

	return((INT32U)y);
} 

/*************************************************
 ** �������������������
 ** ��    �룺Pָ��U8������
 			  nҪ����ĸ���
 ** ��    ����������
**************************************************/
INT16U AberranceRate(INT8U *p,INT8U n)
{
	INT8U i;
	INT16U data,rate;
	INT32U sum;
	sum = 0;
	for(i = 0;i < (n-2)/2;i ++)
	{
		data = TwoByteBcdToHex(p+2*(i+1));
		sum = sum + data*data;
	}
	data = TwoByteBcdToHex(p);
	sum = InvSqrt((float)sum);
	rate = (sum )	;/// data;  //����3λС��
	rate = ShortHexToBCD(rate);
	return rate;
}
/********************************************************************
* Function Name :INT32U powfun(INT16U data,INT16U pow) 
* Description   :  ����������
* Input         :  data-���� ,pow-����
* Output        :  ��ֵ
* Return        :
********************************************************************/
INT32U powfun(INT16U data,INT16U pow)
{
    INT32U temp=0,i=0;
	temp=data;
    if(0==pow)
        temp=1;
    else
        for(i=0;i<pow-1;i++)
	    	temp*=data;
   return temp;
}

INT8U App_DataBcdIsOK(const INT8U *pData,INT16U Len)
{
	INT8U i = 0;
	for(i = 0 ; i < Len ;i++)
	{
		INT8U byteL = *(pData+i)&0x0f;
		INT8U byteH = *(pData+i)>>4;
		if((byteL > 0x09) ||(byteH > 0x09))
		{
			return 0 ;
		}
	}
	return 1 ;
}

//����������������Ƿ�һ��
INT8U App_DataMatchDataIsSame(const INT8U *pData,INT16U Len,INT8U MatchData)
{
	INT8U i = 0;
	
	for(i = 0 ; i < Len ;i++)
	{
		if(*pData!=MatchData)
		{
			pData++ ;
			return 0 ;
		}
	}
	return 1 ;
}
