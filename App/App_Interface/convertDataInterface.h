/*****************************************Copyright(C)******************************************

*------------------------------------------文件信息---------------------------------------------
* FileName			: convertDataInterface.h
* Author			: 
* Date First Issued	: 12/12/2008
* Version			: V2.1
* Description		: 包含主程序需要使用的常规函数，包括字符结构更改，时间计算函数
*----------------------------------------历史版本信息-------------------------------------------
* History			:
* //2008		: V
* Description		: 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
#ifndef __App_convertDataInterface__H__
#define __App_convertDataInterface__H__


/* Includes-----------------------------------------------------------------------------------*/

#include "Bsp_config.h"

/* Private define-----------------------------------------------------------------------------*/
/* Private typedef----------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables--------------------------------------------------------------------------*/
/* Private function prototypes----------------------------------------------------------------*/

extern INT16U TwoByteToShort(const INT8U *Point);
extern INT32U ThreeByteToInt(INT8U *Point);
extern INT32U FourByteToInt(INT8U *Point);
extern INT64U FiveByteToLong(INT8U *Point);

//整型拆分成单字节数据
extern void ShortToByte(INT16U Source, INT8U *Target);
extern void IntToFourByte(INT32U Source, INT8U *Target);
extern void LongToFiveByte(INT64U Source, INT8U *Target);

//BCD码转化成Hex
extern INT8U ByteBcdToHex(INT8U Source);
extern INT16U TwoByteBcdToHex(INT8U *Point);
extern INT32U ThreeByteBcdToHex(INT8U *Point);
extern INT32U FourByteBcdToHex(INT8U *Point);
extern INT64U FiveByteBcdToHex(INT8U *Point);

//Hex转化成BCD码
extern INT8U ByteHexToBcd(INT8U Source);
extern INT16U ShortHexToBCD(INT16U Source);
extern INT32U IntHexToBcd(INT32U Source);
extern INT64U LongHexToBcd(INT64U Source);

//BCD与Hex相加或相减
extern INT8U BcdAddHex(INT8U YuBcd,INT8U MuHex);
extern INT8U BcdDccHex(INT8U YuBcd,INT8U MuHex);

//国网专用数据格式转换
extern INT64S Dada02ToINT64S(INT16U Source); 
extern INT16U INT64SToDada02(INT64S Source);
INT32S Dada03ToINT32S(INT32U Souce);
INT64S Dada03ToINT64S(INT32U Souce);
extern INT32U INT64SToDada03(INT64S Source);
extern INT32U INT32UToDada03(INT32U Source);
extern INT16U AberranceRate(INT8U *p,INT8U n);
//任务索引数据校验
extern INT8U TeskCS(INT8U*Buff,INT16U len);

//取绝对值
extern INT32U Labs(long v);

//extern void GetTheTime(_BSPRTC_TIME *Ptr_RtcTim);//获取当前时间
//extern void AdjustTime(_BSPRTC_TIME *pTime, INT32U numb, BOOLEAN state);//计算某个时间点的倒退或将来的时间
//extern INT16S CompareTime(_BSPRTC_TIME Time1, _BSPRTC_TIME Time2);//比较两个时间点的先后
//extern INT32U CmpTheTime(_BSPRTC_TIME *TimeBuff,_BSPRTC_TIME *TimeBuff2,INT8U Type);//计算两个时间点的时间差
//extern INT8U CmpTeskTime(INT8U *TimeBuff);//与当前时间比大小 

INT8U App_DataBcdIsOK(const INT8U *pData,INT16U Len);
//数据与给定的数据是否一样
INT8U App_DataMatchDataIsSame(const INT8U *pData,INT16U Len,INT8U MatchData);


/* Private functions--------------------------------------------------------------------------*/



#endif


