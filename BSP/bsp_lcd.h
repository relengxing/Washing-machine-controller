/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_led.h
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Bsp_lcd_H
#define __Bsp_lcd_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_lcd.h"
#include "convertDataInterface.h"
#include "bsp_rtc.h"
/* 数字段码 			------------------------------------------------------*/
extern uint32_t const segnumTable[11][4];
typedef enum {
	BSP_LCD_P1,
	BSP_LCD_P2,
	BSP_LCD_P3T9,
	BSP_LCD_P4P5,
	BSP_LCD_P6,
	BSP_LCD_P7,
	BSP_LCD_P9,
	BSP_LCD_P10,
	BSP_LCD_T1,
	BSP_LCD_T2,
	BSP_LCD_T3,
	BSP_LCD_T4,
	BSP_LCD_T5,
	BSP_LCD_T6,
	BSP_LCD_T7,
	BSP_LCD_T8,
	BSP_LCD_T10,
	BSP_LCD_T11,
	BSP_LCD_T12,
	BSP_LCD_T13,
	BSP_LCD_T14,
	BSP_LCD_T15,
	BSP_LCD_T16,
	BSP_LCD_T17,
	BSP_LCD_T18,
	BSP_LCD_T19,
}_Bsp_LCD_Symbol;
typedef enum{
	BSP_LCD_SIGNAL_0 = 0,		//0格
	BSP_LCD_SIGNAL_1 = 1,		//1格
	BSP_LCD_SIGNAL_2 = 2,       //2格
	BSP_LCD_SIGNAL_3 = 3,       //3格
	BSP_LCD_SIGNAL_4 = 4		//4格
}_Bsp_LCD_Signal;
typedef enum{
	BSP_LCD_POWER_0 = 0,		//0格电
	BSP_LCD_POWER_1 = 1,		//1格电
	BSP_LCD_POWER_2 = 2,		//2格电
	BSP_LCD_POWER_3 = 3,		//3格电
	BSP_LCD_POWER_4 = 4			//空
}_Bsp_LCD_Power;

typedef enum{
	BSP_LCD_NUM_TIME = 0,
	BSP_LCD_NUM_ADDRESS = 1,
	BSP_LCD_NUM_VERSIONNUM = 2
}BSP_LCD_NUM;
/* Exported types ------------------------------------------------------------*/
#define LCD_ALL_ON()	do{\
							LCD_SegmentSetLow(0, 0xFFFFFFFF, 0xFFFFFFFF);\
							LCD_SegmentSetLow(1, 0xFFFFFFFF, 0xFFFFFFFF);\
							LCD_SegmentSetLow(2, 0xFFFFFFFF, 0xFFFFFFFF);\
							LCD_SegmentSetLow(3, 0xFFFFFFFF, 0xFFFFFFFF);\
						}while(0)
#define LCD_ALL_OFF()	do{\
							LCD_SegmentSetLow(0, 0xFFFFFFFF, 0x00000000);\
							LCD_SegmentSetLow(1, 0xFFFFFFFF, 0x00000000);\
							LCD_SegmentSetLow(2, 0xFFFFFFFF, 0x00000000);\
							LCD_SegmentSetLow(3, 0xFFFFFFFF, 0x00000000);\
						}while(0)
/*--中文											-----------------------*/

/*--特殊符号---------------------------*///对的
#define LCD_P1(a)		if(a)\
							LCD_SegmentSet(0,23,true);\
						else\
							LCD_SegmentSet(0,23,false);
#define LCD_P2(a)		if(a)\
							LCD_SegmentSet(1,23,true);\
						else\
							LCD_SegmentSet(1,23,false);
#define LCD_P3T9(a)		if(a)\
							LCD_SegmentSet(0,7,true);\
						else\
							LCD_SegmentSet(0,7,false);
#define LCD_P4P5(a)		if(a)\
							LCD_SegmentSet(0,13,true);\
						else\
							LCD_SegmentSet(0,13,false);



#define LCD_P6(a)		if(a)\
							LCD_SegmentSet(0,11,true);\
						else\
							LCD_SegmentSet(0,11,false);
#define LCD_P7(a)		if(a)\
							LCD_SegmentSet(0,14,true);\
						else\
							LCD_SegmentSet(0,14,false);

#define LCD_P9(a)		if(a)\
							LCD_SegmentSet(0,16,true);\
						else\
							LCD_SegmentSet(0,16,false);
#define LCD_P10(a)		if(a)\
							LCD_SegmentSet(0,18,true);\
						else\
							LCD_SegmentSet(0,18,false);


/*--信号强度--*/		//对的
#define LCD_T1(a)		if(a)\
							LCD_SegmentSet(0,0,true);\
						else\
							LCD_SegmentSet(0,0,false);
#define LCD_T2(a)		if(a)\
							LCD_SegmentSet(1,0,true);\
						else\
							LCD_SegmentSet(1,0,false);
#define LCD_T3(a)		if(a)\
							LCD_SegmentSet(2,0,true);\
						else\
							LCD_SegmentSet(2,0,false);
#define LCD_T4(a)		if(a)\
							LCD_SegmentSet(3,0,true);\
						else\
							LCD_SegmentSet(3,0,false);

/*--手动自动--*/		//对的

#define LCD_T5(a)		if(a)\
							LCD_SegmentSet(3,1,true);\
						else\
							LCD_SegmentSet(3,1,false);
#define LCD_T6(a)		if(a)\
							LCD_SegmentSet(2,1,true);\
						else\
							LCD_SegmentSet(2,1,false);

/*--M1M2--*/			//对的
#define LCD_T7(a)		if(a)\
							LCD_SegmentSet(1,1,true);\
						else\
							LCD_SegmentSet(1,1,false);
#define LCD_T8(a)		if(a)\
							LCD_SegmentSet(0,1,true);\
						else\
							LCD_SegmentSet(0,1,false);
/*--设定温度，月，L/H,H--*///对的
//#define LCD_T9(a)		if(a)\
//							LCD_SegmentSet(3,3,true);\
//						else\
//							LCD_SegmentSet(3,3,false);
#define LCD_T10(a)		if(a)\
							LCD_SegmentSet(0,9,true);\
						else\
							LCD_SegmentSet(0,9,false);
#define LCD_T11(a)		if(a)\
							LCD_SegmentSet(0,20,true);\
						else\
							LCD_SegmentSet(0,20,false);
#define LCD_T12(a)		if(a)\
							LCD_SegmentSet(0,22,true);\
						else\
							LCD_SegmentSet(0,22,false);

#define LCD_T13(a)		if(a)\
							LCD_SegmentSet(2,23,true);\
						else\
							LCD_SegmentSet(2,23,false);
#define LCD_T14(a)		if(a)\
							LCD_SegmentSet(3,23,true);\
						else\
							LCD_SegmentSet(3,23,false);
#define LCD_T15(a)		if(a)\
							LCD_SegmentSet(3,22,true);\
						else\
							LCD_SegmentSet(3,22,false);
#define LCD_T16(a)		if(a)\
							LCD_SegmentSet(2,22,true);\
						else\
							LCD_SegmentSet(2,22,false);
#define LCD_T17(a)		if(a)\
							LCD_SegmentSet(1,22,true);\
						else\
							LCD_SegmentSet(1,22,false);
/*--日--*/
#define LCD_T18(a)		if(a)\
							LCD_SegmentSet(0,3,true);\
						else\
							LCD_SegmentSet(0,3,false);
#define LCD_T19(a)		if(a)\
							LCD_SegmentSet(0,5,true);\
						else\
							LCD_SegmentSet(0,5,false);
/*数字					----------------------------------*/

#define LCD_NUM1(num)		do{\
								LCD_SegmentSetLow(0,0x000040,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x0000c0,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x0000c0,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x0000c0,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM2(num)		do{\
								LCD_SegmentSetLow(0,0x000010,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x000030,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x000030,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x000030,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM3(num)		do{\
								LCD_SegmentSetLow(0,0x000004,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x00000c,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x00000c,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x00000c,segnumTable[num][3]);\
							}while(0)	                                      
#define LCD_NUM4(num)		do{\
								LCD_SegmentSetLow(0,0x001000,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x003000,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x003000,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x003000,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM5(num)		do{\
								LCD_SegmentSetLow(0,0x000400,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x000c00,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x000c00,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x000c00,segnumTable[num][3]);\
							}while(0)	                                      
#define LCD_NUM6(num)		do{\
								LCD_SegmentSetLow(0,0x000100,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x000300,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x000300,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x000300,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM7(num)		do{\
								LCD_SegmentSetLow(0,0x008000,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x00c000,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x00c000,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x00c000,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM8(num)		do{\
								LCD_SegmentSetLow(0,0x020000,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x030000,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x030000,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x030000,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM9(num)		do{\
								LCD_SegmentSetLow(0,0x080000,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x0c0000,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x0c0000,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x0c0000,segnumTable[num][3]);\
							}while(0)	
#define LCD_NUM10(num)		do{\
								LCD_SegmentSetLow(0,0x200000,segnumTable[num][0]);\
								LCD_SegmentSetLow(1,0x300000,segnumTable[num][1]);\
								LCD_SegmentSetLow(2,0x300000,segnumTable[num][2]);\
								LCD_SegmentSetLow(3,0x300000,segnumTable[num][3]);\
							}while(0)	
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

void Bsp_LCD_Init(void);
void LCD_Display_NUM(uint32_t num);
void LCD_Display_Symbol(_Bsp_LCD_Symbol symbol,uint8_t enable);
void LCD_Display_Signal(_Bsp_LCD_Signal signal);
void LCD_Display_Power(_Bsp_LCD_Power power);
//void LCD_Updata_Power(void);
/******************************************************************************
* Function Name: 
* Description: 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void LCD_Display_Temperature(INT16U BCDtem);
void LCD_Display_Humidity(INT8U BCDhum);
//void LCD_Updata_Date(void);
void LCD_Display_Num(BSP_LCD_NUM type);



#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




