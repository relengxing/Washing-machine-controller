/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName			: DealWithChar.c
* Author			: 
* Date First Issued	: 2007.05.21
* Version			: V1.0
* Description		: 数据格式转化
*----------------------------------------历史版本信息-------------------------------------------
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
//若干个单字节数据合成多字节的数据类型
INT16U TwoByteToShort(const INT8U *Point);
INT32U ThreeByteToInt(INT8U *Point);
INT32U FourByteToInt(INT8U *Point);
INT64U FiveByteToLong(INT8U *Point);
//整型拆分成单字节数据
void ShortToByte(INT16U Source, INT8U *Target);
void IntToFourByte(INT32U Source, INT8U *Target);
void LongToFiveByte(INT64U Source, INT8U *Target);
//BCD码转化成Hex
INT8U ByteBcdToHex(INT8U Source);
INT16U TwoByteBcdToHex(INT8U *Point);
INT32U ThreeByteBcdToHex(INT8U *Point);
INT32U FourByteBcdToHex(INT8U *Point);
INT64U FiveByteBcdToHex(INT8U *Point);
//Hex转化成BCD码
INT8U ByteHexToBcd(INT8U Source);
INT16U ShortHexToBCD(INT16U Source);
INT32U IntHexToBcd(INT32U Source);
INT64U LongHexToBcd(INT64U Source);
//BCD与Hex相加或相减
INT8U BcdAddHex(INT8U YuBcd,INT8U MuHex);
INT8U BcdDccHex(INT8U YuBcd,INT8U MuHex);
//国网专用数据格式转换
INT64S Dada02ToINT64S(INT16U Source); 
INT16U INT64SToDada02(INT64S Source);
INT32S Dada03ToINT32S(INT32U Source);
INT32U INT64SToDada03(INT64S Source);
//任务索引数据校验
INT8U TeskCS(INT8U*Buff,INT16U len);
//取绝对值
INT32U Labs(long v);
/* Private functions--------------------------------------------------------------------------*/
/////////////////////////////////////////////////////
//若干个单字节数据合成多字节的数据类型////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
函数名称：INT16U TwoByteToShort(INT8U *Point)
输入参数：Point--2字节数组首地址
输出参数：RetVal--转化完成的整形数据
约束条件：无
功能描述：将2字节8位数据转化成16位的短整形数据子程序
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：  
*****************************************************************/ 
INT16U TwoByteToShort(const INT8U *Point)
{
	INT16U RetVal;
	
	RetVal = *Point + (*(Point+1)<<8);
	
	return RetVal;
}

/*****************************************************************   
函数名称：INT32U ThreeByteToInt(INT8U *Point)
输入参数：Point--3字节数组首地址
输出参数：RetVal--转化完成的整形数据
约束条件：无
功能描述：将3字节数组转化成32位整形数据子程序
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：    
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
函数名称：INT32U FourByteToInt(INT8U *Point)
输入参数：Point--4字节数组首地址
输出参数：RetVal--转化完成的整形数据
约束条件：无
功能描述：将4字节数组转化成32位整形数据子程序
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：  
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
函数名称：INT64U FiveByteToLong(INT8U *Point)
输入参数：Point--5字节数组首地址
输出参数：RetVal--转化完成的整形数据
约束条件：无
功能描述：将5字节数组转化成64位整形数据子程序
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：  
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
//整型拆分成单字节数据///////////////////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
函数名称：void ShortToByte(INT16U Source, INT8U *Target)
输入参数：Source-待转化的短整形数据   Target--2字节数组首地址
输出参数：无
约束条件：无
功能描述：将16位短整形数据转化成2字节的8位数据
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：  
*****************************************************************/ 
void ShortToByte(INT16U Source, INT8U *Target)
{
	Target[0] = Source & 0xff;
	Target[1] = (Source >> 8)&0xff;
}	
/*****************************************************************   
函数名称：void IntToFourByte(INT32U Source, INT8U *Target)
输入参数：Source-待转化的整形数据   Target--4字节数组首地址
输出参数：无
约束条件：无
功能描述：将32位整形数据转化成4字节的8位数据
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：  
*****************************************************************/ 
void IntToFourByte(INT32U Source, INT8U *Target)
{
	Target[0] = (INT8U)(Source & 0xff);
	Target[1] = (INT8U)((Source >> 8)&0xff);
	Target[2] = (INT8U)((Source >> 16)&0xff);
	Target[3] = (INT8U)((Source >> 24)&0xff);
}	
/*****************************************************************   
函数名称：void LongToFiveByte(INT64U Source, INT8U *Target)
输入参数：Source-待转化的整形数据   Target--5字节数组首地址
输出参数：无
约束条件：无
功能描述：将64位类型数据转化成5字节的8位数据
作    者：	                日期: 2007.09.28 
修    改：                     日期:  
版    本：  
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
//BCD码转化成Hex///////////////////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
函数名称：INT8U ByteBcdToHex(INT8U Source)
输入参数：Source-待转化的BCD码数据   
输出参数：转化后的Hex数据
约束条件：无
功能描述：将单字节BCD码转换成十六进制
作    者：                     日期: 
修    改：                     日期:  
版    本：  
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
函数名称：INT16U TwoByteBcdToHex(INT8U *Point)
输入参数：Point-待转化的BCD码数据存储首地址   
输出参数：转化后的短整型Hex数据
约束条件：无
功能描述：将2字节BCD码转换成十六进制
作    者：                     日期:  
修    改：                     日期:  
版    本：  
*****************************************************************/ 
INT16U TwoByteBcdToHex(INT8U *Point)
{
	INT16U RetVal;
	
	RetVal =  ByteBcdToHex(*Point);
	RetVal += ByteBcdToHex(*(Point+1)) * 100;
	return RetVal;
}

/*****************************************************************   
函数名称：INT32U ThreeByteBcdToHex(INT8U *Point)
输入参数：Point-待转化的BCD码数据存储首地址   
输出参数：转化后的整型Hex数据
约束条件：无
功能描述：将3字节BCD码转换成十六进制
作    者：                     日期:  
修    改：                     日期:  
版    本：  
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
函数名称：INT32U FourByteBcdToHex(INT8U *Point)
输入参数：Point-待转化的BCD码数据存储首地址   
输出参数：转化后的整型Hex数据
约束条件：无
功能描述：将4字节BCD码转换成十六进制
作    者：                     日期:  
修    改：                     日期:  
版    本：  
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
函数名称：INT64U FiveByteBcdToHex(INT8U *Point)
输入参数：Point-待转化的BCD码数据存储首地址   
输出参数：转化后的整型Hex数据
约束条件：无
功能描述：将5字节BCD码转换成十六进制
作    者：                     日期:  
修    改：                     日期:  
版    本：  
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
//Hex转化成BCD码///////////////////////////////
/////////////////////////////////////////////////////
/*****************************************************************   
函数名称：INT8U ByteHexToBcd(INT8U Source)
输入参数：Source-待转化的Hex数据   
输出参数：转化后的BCD值
约束条件：无
功能描述：单字节十六进制转换成BCD码
作    者：                     日期:  
修    改：                     日期:  	 有点问题的，溢出
版    本：  
*****************************************************************/
INT8U ByteHexToBcd(INT8U Source)
{
	INT8U Temp1, Temp2;
	Temp1 = (Source %100)/ 10;		// 修改，舍去百位数字的值
	Temp2 = Source % 10;
	Temp1 = (Temp1 << 4) + Temp2;
	return Temp1;
}

/*****************************************************************   
函数名称：INT16U ShortHexToBCD(INT16U Source) 
输入参数：Source--待转化的short型Hex数据
输出参数：Result--转化完成的BCD数据
约束条件：无
功能描述：16位16进制数据转化成16位BCD码数据	  
作    者：				日期: 2007.11.14 
修    改：				日期:  
版    本：  
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
函数名称：INT32U IntHexToBcd(INT32U Source)
输入参数：Source-待转化的Hex数据   
输出参数：转化后的整型BCD码数据
约束条件：无
功能描述：将整型的十六进制数转换成BCD码
作    者：                     日期:  
修    改：                     日期:  		 会溢出
版    本：  
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
//函数名称：INT32U IntHexToBcd(INT32U Source)
//输入参数：Source-待转化的Hex数据   
//输出参数：转化后的整型BCD码数据
//约束条件：无
//功能描述：将整型的十六进制数转换成BCD码
//作    者：                     日期:  
//修    改：                     日期:  	  不会溢出 正解
//版    本：  
//*****************************************************************/
//INT64U IntHexToBcd(INT32U Source)
//{
//	INT64U result = 0 ;
//	INT64U j;
//	for(j =0 ; j < 10 ; j ++)//转换后最大有10位数字
//	{
//		INT64U tmp ;
//		tmp = Source%10 ;
//		Source /= 10 ;
//		result |= tmp<<(j*4) ;
//	}
//	return result ;
//}







/*****************************************************************   
函数名称：INT64U LongHexToBcd(INT64U Source)
输入参数：Source-待转化的BCD码数据   
输出参数：转化后的长整型Hex数据
约束条件：无
功能描述：将长整型的十六进制数转换成BCD码
作    者：                     日期:  
修    改：                     日期:  
版    本：  
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

//返回值 BCD
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
BCD-Hex=BCD码
*********************************************************************/
//返回值 BCD
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

//国网专用数据格式转换
/*****************************************************************   
函数名称：INT64S Dada02ToINT64S(INT16U Source)
输入参数：Source--待转化的数据
输出参数：Val--转化完成的数据
约束条件：无
功能描述：将数据格式02的数据转化成64位的16进制数据  
作    者：				日期: 2007.09.12 
修    改：				日期:  
版    本：  
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
函数名称：INT16U INT64SToDada02 (INT64S Source)
输入参数：Source--待转化的数据
输出参数：Val--转化完成的数据
约束条件：无
功能描述：将INT64S类型的Hex值Source转换成INT16U类型的02格式数据。输入值为实际要转换的值的1000倍。 
作    者：				日期: 2007.11.14 
修    改：				日期:  
版    本：  
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
函数名称：INT32S Dada03ToINT32S(INT32U Souce) 
输入参数：Source--待转化的数据
输出参数：DataTemp--转化完成的数据
约束条件：无
功能描述：将03数据格式的数据转化成16进制有符号数据  
作    者：				日期: 2007.09.12 
修    改：				日期:  
版    本：  
*****************************************************************/
INT32S Dada03ToINT32S(INT32U Souce)
{
	INT32U Temp;
	INT32S DataTemp;
	
	Temp=Souce&0x0fffffff;
	Temp=FourByteBcdToHex((INT8U*)&Temp);
	DataTemp=(INT32S)Temp;

	if(Souce&0x10000000)//负值
	{
		DataTemp=-DataTemp;
	}

	return DataTemp;
}

/*****************************************************************   
函数名称：INT64S Dada03ToINT64S(INT32U Souce)
输入参数：Source--待转化的数据
输出参数：转化完成的数据
约束条件：无
功能描述：将03数据格式的数据转化成16进制有符号数据  
作    者：fangjb		日期: 2010.05.31 
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
	if(Souce&0x10000000)//负值
		dataTemp=-dataTemp;
	return dataTemp;
}


/*****************************************************************   
函数名称：INT32U INT64SToDada03(INT64S Souce) 
输入参数：Source--待转化的数据
输出参数：Temp--转化完成的数据
约束条件：无
功能描述：将16进制有符号数据转化成03数据格式  
作    者：				日期: 2007.09.12 
修    改：	GQ			日期: 2008.12.18 
版    本：  
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
//	if(Source&0x8000000000000000)// Source为负值
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
	if(Source&0x80000000)// Source为负值
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
 函数： TeskCS 
 功能： 任务索引数据校验
 计算原理：被校验数据和校验码累加和为0x00
 Buff:输入数据指针
 len :输入数据长度
 返回值：校验码
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
函数名称：INT32U Labs(long v)
输入参数：有符号类型值
输出参数：绝对值
约束条件：无
功能描述：求取绝对值
全局变量：
调用模块：
作    者：王夏娇			日期: 2007.05.27 
修    改：					日期:  
版    本：V1.0
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
 ** 功能描述：计算平方根
 ** 输    入：number为float数
 ** 输    出：平方根为INT32U 
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
 ** 功能描述：计算畸变率
 ** 输    入：P指向U8的数组
 			  n要处理的个数
 ** 输    出：畸变率
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
	rate = (sum )	;/// data;  //保留3位小数
	rate = ShortHexToBCD(rate);
	return rate;
}
/********************************************************************
* Function Name :INT32U powfun(INT16U data,INT16U pow) 
* Description   :  求数据幂数
* Input         :  data-数据 ,pow-幂数
* Output        :  幂值
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

//数据与给定的数据是否一样
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
