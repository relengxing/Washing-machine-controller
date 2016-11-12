/*****************************************Copyright(C)******************************************

*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName			: convertDataInterface.h
* Author			: 
* Date First Issued	: 12/12/2008
* Version			: V2.1
* Description		: ������������Ҫʹ�õĳ��溯���������ַ��ṹ���ģ�ʱ����㺯��
*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
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

//���Ͳ�ֳɵ��ֽ�����
extern void ShortToByte(INT16U Source, INT8U *Target);
extern void IntToFourByte(INT32U Source, INT8U *Target);
extern void LongToFiveByte(INT64U Source, INT8U *Target);

//BCD��ת����Hex
extern INT8U ByteBcdToHex(INT8U Source);
extern INT16U TwoByteBcdToHex(INT8U *Point);
extern INT32U ThreeByteBcdToHex(INT8U *Point);
extern INT32U FourByteBcdToHex(INT8U *Point);
extern INT64U FiveByteBcdToHex(INT8U *Point);

//Hexת����BCD��
extern INT8U ByteHexToBcd(INT8U Source);
extern INT16U ShortHexToBCD(INT16U Source);
extern INT32U IntHexToBcd(INT32U Source);
extern INT64U LongHexToBcd(INT64U Source);

//BCD��Hex��ӻ����
extern INT8U BcdAddHex(INT8U YuBcd,INT8U MuHex);
extern INT8U BcdDccHex(INT8U YuBcd,INT8U MuHex);

//����ר�����ݸ�ʽת��
extern INT64S Dada02ToINT64S(INT16U Source); 
extern INT16U INT64SToDada02(INT64S Source);
INT32S Dada03ToINT32S(INT32U Souce);
INT64S Dada03ToINT64S(INT32U Souce);
extern INT32U INT64SToDada03(INT64S Source);
extern INT32U INT32UToDada03(INT32U Source);
extern INT16U AberranceRate(INT8U *p,INT8U n);
//������������У��
extern INT8U TeskCS(INT8U*Buff,INT16U len);

//ȡ����ֵ
extern INT32U Labs(long v);

//extern void GetTheTime(_BSPRTC_TIME *Ptr_RtcTim);//��ȡ��ǰʱ��
//extern void AdjustTime(_BSPRTC_TIME *pTime, INT32U numb, BOOLEAN state);//����ĳ��ʱ���ĵ��˻�����ʱ��
//extern INT16S CompareTime(_BSPRTC_TIME Time1, _BSPRTC_TIME Time2);//�Ƚ�����ʱ�����Ⱥ�
//extern INT32U CmpTheTime(_BSPRTC_TIME *TimeBuff,_BSPRTC_TIME *TimeBuff2,INT8U Type);//��������ʱ����ʱ���
//extern INT8U CmpTeskTime(INT8U *TimeBuff);//�뵱ǰʱ��ȴ�С 

INT8U App_DataBcdIsOK(const INT8U *pData,INT16U Len);
//����������������Ƿ�һ��
INT8U App_DataMatchDataIsSame(const INT8U *pData,INT16U Len,INT8U MatchData);


/* Private functions--------------------------------------------------------------------------*/



#endif


