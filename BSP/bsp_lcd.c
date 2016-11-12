/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : lcd.c
* Author             : lichao
* Version            : V1.00
* Date               : 2015.07.14
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Bsp_lcd.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LCD_CMU_CLK_PRE         cmuClkDiv_64
#define LCD_CMU_CLK_DIV         cmuClkDiv_1
/* Private macro -------------------------------------------------------------*/
uint32_t const segnumTable[11][4] = 
{
	{0x2a9554,0x3ffffc,0x2a9554,0x3ffffc},		//0
	{0x000000,0x2a9554,0x2a9554,0x000000},		//1
	{0x2a9554,0x155554,0x3feaa8,0x2abffc},		//2
	{0x2a9554,0x2a9554,0x3ffffc,0x2a9554},		//3
	{0x000000,0x2abffc,0x3ffffc,0x154000},		//4
	{0x2a9554,0x2aaaa8,0x157ffc,0x3fd554},		//5
	{0x2a9554,0x7feaa8,0x157ffc,0x3ffffc},		//6
	{0x001554,0x2a9554,0x2a9554,0x2a8000},		//7
	{0x2a9554,0x3ffffc,0x3ffffc,0x3ffffc},		//8
	{0x2a9554,0x2abffc,0x3ffffc,0x3fd554},		//9
	{0x000000,0x000000,0x000000,0x000000},		//空
};
 

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************
* Description       : void Bsp_LCD_Init(void)
* Input             : none
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/20
***********************************************************************************************/
void Bsp_LCD_Init(void)
{
	const LCD_Init_TypeDef lcdInit = { 
        true,
        lcdMuxQuadruplex ,//,lcdMuxQuadruplex
        lcdBiasOneThird,//lcdBiasOneFourth,  
		lcdWaveLowPower, //lcdWaveNormal,//
        lcdVLCDSelVDD,
        lcdConConfVLCD 
    }; 
    /* Ensure LE modules are accessible */
    CMU_ClockEnable(cmuClock_CORELE, true); 
    /* Enable LFRCO as LFACLK in CMU (will also enable oscillator if not enabled) */
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);  
    /* LCD Controller Prescaler (divide LFACLK / 64) */
    CMU_ClockDivSet(cmuClock_LCDpre, cmuClkDiv_64);
    /* LFACLK_LCDpre = 512 Hz */
    CMU_LCDClkFDIVSet(cmuClkDiv_2);
    /* Set FDIV=0, means 512/1 = 512 Hz */
    /* With octaplex mode, 512/16 => 32 Hz Frame Rate */   
    /* Enable clock to LCD module */
    CMU_ClockEnable(cmuClock_LCD, true);    
    /* Disable interrupts */
    LCD_IntDisable(0xFFFFFFFF);   
    /* Initialize and enable LCD controller */
    LCD_Init(&lcdInit);
 //   LCD_ContrastSet(26);
    /* Enable all display segments */
    LCD_SegmentRangeEnable(lcdSegment0_3, true);
    LCD_SegmentRangeEnable(lcdSegment4_7, true);
	LCD_SegmentRangeEnable(lcdSegment8_11, true);
	LCD_SegmentRangeEnable(lcdSegment12_15, true);
	LCD_SegmentRangeEnable(lcdSegment16_19, true);
	LCD_SegmentRangeEnable(lcdSegment20_23, true);
	
	
    LCD_SyncBusyDelay(0xFFFFFFFF);
}
/**********************************************************************************************
* Description       : void LCD_Display_NUM(unsigned int num)
* Input             : none
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/20
***********************************************************************************************/
//static void LCD_Updata_Time(void)
//{
//	_BSPRTC_TIME rtcTime;
//	BSP_RTCGetTime(&rtcTime);
//	LCD_Display_Symbol(BSP_LCD_P9,1);
//	LCD_Display_Symbol(BSP_LCD_P7,1);
//	LCD_Display_Symbol(BSP_LCD_P10,0);
//	LCD_NUM7(ByteBcdToHex(rtcTime.Hour)/10);
//	LCD_NUM8(ByteBcdToHex(rtcTime.Hour)%10);
//	LCD_NUM9(ByteBcdToHex(rtcTime.Minute)/10);
//	LCD_NUM10(ByteBcdToHex(rtcTime.Minute)%10);
//}
//static void LCD_Display_Address(void)
//{
//	LCD_Display_Symbol(BSP_LCD_P9,0);
//	LCD_Display_Symbol(BSP_LCD_P7,0);
//	LCD_Display_Symbol(BSP_LCD_P10,0);
//	LCD_NUM7(Detector_Address[5]/10);
//	LCD_NUM8(Detector_Address[5]%10);
//	LCD_NUM9(Detector_Address[4]/10);
//	LCD_NUM10(Detector_Address[4]%10);
//}
//static void LCD_Display_VersionNumber(void)
//{
//	LCD_Display_Symbol(BSP_LCD_P9,0);
//	LCD_Display_Symbol(BSP_LCD_P7,0);
//	LCD_Display_Symbol(BSP_LCD_P10,1);
//	LCD_NUM7(VersionNumber/1000);
//	LCD_NUM8(VersionNumber%1000/100);
//	LCD_NUM9(VersionNumber%100/10);
//	LCD_NUM10(VersionNumber%10);
//}
//void LCD_Display_Num(BSP_LCD_NUM type)
//{
//	switch(type)
//	{
//		case BSP_LCD_NUM_TIME:
//			LCD_Updata_Time();
//			break;
//		case BSP_LCD_NUM_ADDRESS:
//			LCD_Display_Address();
//			break;
//		case BSP_LCD_NUM_VERSIONNUM:
//			LCD_Display_VersionNumber();
//			break;
//		default:
//			break;
//	}
//}

void LCD_Updata_Date(void)
{
	_BSPRTC_TIME rtcTime;
	BSP_RTCGetTime(&rtcTime);
	LCD_Display_Symbol(BSP_LCD_T10,1);		//月
	LCD_Display_Symbol(BSP_LCD_T18,1);		//日
	LCD_NUM4(ByteBcdToHex(rtcTime.Month)/10);	//月
	LCD_NUM5(ByteBcdToHex(rtcTime.Month)%10);	//
	
	LCD_NUM1(ByteBcdToHex(rtcTime.Day)/10);	//日
	LCD_NUM2(ByteBcdToHex(rtcTime.Day)%10);
	
}


/**********************************************************************************************
* Description       : void LCD_Display_Symbol(_Bsp_LCD_Symbol symbol,uint8_t enable)
* Input             : _Bsp_LCD_Symbol symbol	根据枚举_Bsp_LCD_Symbol选择要操作的液晶段
						uint8_t enable			点亮或者熄灭液晶
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/20
***********************************************************************************************/
void LCD_Display_Symbol(_Bsp_LCD_Symbol symbol,uint8_t enable)
{
	switch(symbol)
	{
		case BSP_LCD_P1:LCD_P1(enable);
			break;
		case BSP_LCD_P2:LCD_P2(enable);
			break;
		case BSP_LCD_P3T9:LCD_P3T9(enable);
			break;
		case BSP_LCD_P4P5:LCD_P4P5(enable);
			break;
		case BSP_LCD_P6:LCD_P6(enable);
			break;
		case BSP_LCD_P7:LCD_P7(enable);
			break;
		case BSP_LCD_P9:LCD_P9(enable);
			break;
		case BSP_LCD_P10:LCD_P10(enable);
			break;
		case BSP_LCD_T1:LCD_T1(enable);
			break;
		case BSP_LCD_T2:LCD_T2(enable);
			break;
		case BSP_LCD_T3:LCD_T3(enable);
			break;
		case BSP_LCD_T4:LCD_T4(enable);
			break;
		case BSP_LCD_T5:LCD_T5(enable);
			break;
		case BSP_LCD_T6:LCD_T6(enable);
			break;
		case BSP_LCD_T7:LCD_T7(enable);
			break;
		case BSP_LCD_T8:LCD_T8(enable);
			break;	
		case BSP_LCD_T10:LCD_T10(enable);
			break;
		case BSP_LCD_T11:LCD_T11(enable);
			break;
		case BSP_LCD_T12:LCD_T12(enable);
			break;
		case BSP_LCD_T13:LCD_T13(enable);
			break;
		case BSP_LCD_T14:LCD_T14(enable);
			break;
		case BSP_LCD_T15:LCD_T15(enable);
			break;
		case BSP_LCD_T16:LCD_T16(enable);
			break;
		case BSP_LCD_T17:LCD_T17(enable);
			break;
		case BSP_LCD_T18:LCD_T18(enable);
			break;
		case BSP_LCD_T19:LCD_T19(enable);
			break;		
		default:
			break;
	}
}
/**********************************************************************************************
* Description       : 显示温湿度
* Input             : 
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/20
***********************************************************************************************/
void LCD_Display_Temperature(INT16U BCDtem)		
{
//	INT8U qian;
	INT8U bai;
	INT8U shi;
	INT8U ge;
//	qian = ByteBcdToHex((INT8U)(BCDtem>>8))%100/ 10;
	bai = ByteBcdToHex((INT8U)(BCDtem>>8)) % 10;
	shi = ByteBcdToHex((INT8U)(BCDtem))%100/10;
	ge  = ByteBcdToHex((INT8U)(BCDtem)) % 10;
	
	LCD_Display_Symbol(BSP_LCD_P4P5,1);
	LCD_Display_Symbol(BSP_LCD_P6,1);
	LCD_NUM4(bai);
	LCD_NUM5(shi);
	LCD_NUM6(ge);
}
void LCD_Display_Humidity(INT8U BCDhum)
{
	INT8U shi;
	INT8U ge;
	shi = ByteBcdToHex((INT8U)(BCDhum))%100/10;
	ge  = ByteBcdToHex((INT8U)(BCDhum)) % 10;
	LCD_NUM1(shi);
	LCD_NUM2(ge);
}



/**********************************************************************************************
* Description       : void LCD_Display_Signal(_Bsp_LCD_Signal signal)
* Input             : _Bsp_LCD_Signal signal 根据枚举_Bsp_LCD_Signal 选择信号的强度等级
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/20
***********************************************************************************************/
void LCD_Display_Signal(_Bsp_LCD_Signal signal)
{
	switch(signal)
	{
		case BSP_LCD_SIGNAL_0:
				LCD_Display_Symbol(BSP_LCD_T1,0);
				LCD_Display_Symbol(BSP_LCD_T2,0);
				LCD_Display_Symbol(BSP_LCD_T3,0);
				LCD_Display_Symbol(BSP_LCD_T4,0);
			break;
		case BSP_LCD_SIGNAL_1:
				LCD_Display_Symbol(BSP_LCD_T1,1);
				LCD_Display_Symbol(BSP_LCD_T2,0);
				LCD_Display_Symbol(BSP_LCD_T3,0);
				LCD_Display_Symbol(BSP_LCD_T4,0);
			break;
		case BSP_LCD_SIGNAL_2:
				LCD_Display_Symbol(BSP_LCD_T1,1);
				LCD_Display_Symbol(BSP_LCD_T2,1);
				LCD_Display_Symbol(BSP_LCD_T3,0);
				LCD_Display_Symbol(BSP_LCD_T4,0);
			break;
		case BSP_LCD_SIGNAL_3:
				LCD_Display_Symbol(BSP_LCD_T1,1);
				LCD_Display_Symbol(BSP_LCD_T2,1);
				LCD_Display_Symbol(BSP_LCD_T3,1);
				LCD_Display_Symbol(BSP_LCD_T4,0);
			break;
		case BSP_LCD_SIGNAL_4:
				LCD_Display_Symbol(BSP_LCD_T1,1);
				LCD_Display_Symbol(BSP_LCD_T2,1);
				LCD_Display_Symbol(BSP_LCD_T3,1);
				LCD_Display_Symbol(BSP_LCD_T4,1);
			break;
		default:
			break;
	}
}
/**********************************************************************************************
* Description       : void LCD_Display_Power(_Bsp_LCD_Power power)
* Input             : _Bsp_LCD_Power power		根据枚举_Bsp_LCD_Power选择电量等级
* Output            : none 
* Contributor       : lichao
* Date First Issued : 2015/07/20
***********************************************************************************************/
void LCD_Display_Power(_Bsp_LCD_Power power)
{
	switch(power)
	{
		case BSP_LCD_POWER_0:
				LCD_Display_Symbol(BSP_LCD_T13,1);
				LCD_Display_Symbol(BSP_LCD_T14,0);
				LCD_Display_Symbol(BSP_LCD_T15,0);
				LCD_Display_Symbol(BSP_LCD_T16,0);
			break;
		case BSP_LCD_POWER_1:
				LCD_Display_Symbol(BSP_LCD_T13,1);
				LCD_Display_Symbol(BSP_LCD_T14,1);
				LCD_Display_Symbol(BSP_LCD_T15,0);
				LCD_Display_Symbol(BSP_LCD_T16,0);
			break;
		case BSP_LCD_POWER_2:
				LCD_Display_Symbol(BSP_LCD_T13,1);
				LCD_Display_Symbol(BSP_LCD_T14,1);
				LCD_Display_Symbol(BSP_LCD_T15,1);
				LCD_Display_Symbol(BSP_LCD_T16,0);
			break;
		case BSP_LCD_POWER_3:
				LCD_Display_Symbol(BSP_LCD_T13,1);
				LCD_Display_Symbol(BSP_LCD_T14,1);
				LCD_Display_Symbol(BSP_LCD_T15,1);
				LCD_Display_Symbol(BSP_LCD_T16,1);
			break;
		case BSP_LCD_POWER_4:
				LCD_Display_Symbol(BSP_LCD_T13,0);
				LCD_Display_Symbol(BSP_LCD_T14,0);
				LCD_Display_Symbol(BSP_LCD_T15,0);
				LCD_Display_Symbol(BSP_LCD_T16,0);
			break;
		default:
			break;
	}
}
//void LCD_Updata_Power(void)
//{
//	if(Voltage > 1450)			//	VGPRS>3.9V
//	{
//		LCD_Display_Power((_Bsp_LCD_Power)3);		//3格电
//	}
//	else if(Voltage > 1370)		//	VGPRS>3.7V
//	{
//		LCD_Display_Power((_Bsp_LCD_Power)2);		//2格电
//	}
//	else if(Voltage > 1300)		//	VGPRS>3.5V
//	{
//		LCD_Display_Power((_Bsp_LCD_Power)1);		//1格电
//	}
//	else 						//	VGPRS<3.5V
//	{
//		LCD_Display_Power((_Bsp_LCD_Power)0);		//0格电
//	}
//}
/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
