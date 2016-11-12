/******************** (C) COPYRIGHT 2014 NLKJ ********************
* File Name          : Bsp_Uart.h
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
* Description        : This file contains all the functions prototypes for the
*                      ADC firmware library.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Bsp_Uart_H
#define __Bsp_Uart_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"

#include "em_usart.h"


/* Exported types ------------------------------------------------------------*/



/* Exported constants --------------------------------------------------------*/
#define UART_DEFAULT_MODE  0x00  /* 普通UART口使用    */
#define UART_HALFDUP_MODE  0x01	 /* 半双工模式, (IRDA), 扩展串口有20MS的转换延时 */
#define UART_RS485_MODE    0x33	 /* 半双工模式, RS485, 扩展串口有20MS的转换延时  */




/* Exported macro ------------------------------------------------------------*/
//// 串口初始化参数
//// 串口设置,不要修改以下定义
typedef enum
{
	BSPUART_PARITY_NO  = USART_FRAME_PARITY_NONE,					// 无校验(默认)
	BSPUART_PARITY_ODD = USART_FRAME_PARITY_ODD,					// 奇校验
	BSPUART_PARITY_EVEN= USART_FRAME_PARITY_EVEN,					// 偶校验
}_BSPUART_PARITY;
// 停止位
typedef enum
{
//	BSPUART_STOPBITS_0_5=0x05,					// 0.5个停止位
	BSPUART_STOPBITS_1  =USART_FRAME_STOPBITS_ONE,					// 1个停止位(默认)
//	BSPUART_STOPBITS_1_5=0x15,					// 1.5个停止位
	BSPUART_STOPBITS_2  =USART_FRAME_STOPBITS_TWO,					// 2个停止位
}_BSPUART_STOPBITS;

//数据长度
typedef enum
{
	BSPUART_WORDLENGTH_5=USART_FRAME_DATABITS_FIVE,				// 5位数据+校验位
	BSPUART_WORDLENGTH_6=USART_FRAME_DATABITS_SIX,				// 6位数据
	BSPUART_WORDLENGTH_7=USART_FRAME_DATABITS_SEVEN,				// 7位数据+唤醒位?
	BSPUART_WORDLENGTH_8=USART_FRAME_DATABITS_EIGHT,				// 8位数据+校验位(默认)
}_BSPUART_WORDLENGTH;



typedef struct STRUCT_UARTX_SET{
INT32U BaudRate;	            /* 波特率 300 ~ 57600 */
_BSPUART_WORDLENGTH  DataBits;	/* 串口1..5支持7 - 8数据位，串口6,7支持5 - 8数据位        */
_BSPUART_PARITY  	 Parity;	    /* 0: No parity, 1: Odd parity, 2: Even parity          */
_BSPUART_STOPBITS    StopBits;	/* 1 - 2                                                */
INT8U  *RxBuf;		/* 接收缓冲区                                            */
INT16U RxBufLen;	/* 接收缓冲区长度                                        */
INT8U  *TxBuf;		/* 发送缓冲区                                            */
INT16U TxBufLen; 	/* 发送缓冲区长度                                        */
INT8U  Mode;		/* UART_DEFAULT_MODE, UART_HALFDUP_MODE, UART_RS485_MODE*/
                    /* 串口1..5不支持7位、无校验模式                          */
}S_UARTx_Set;


typedef struct STRUCT_BSP_UARTX{
volatile      INT16U     RxNum;
volatile      INT16U     TxBusy;
volatile      INT16U     TxComplete;
volatile      INT8U      *pSendBuf;
volatile      INT8U      *pRecvBuf;
volatile      INT8U      FrameRxFlag; //接收到一帧

INT16U        MaxTxBufLen;
INT16U        MaxRxBufLen;
INT16U        RxOffset;
INT16U        RxPointer;
INT16U        FrameRxIntv;
INT16U        FrameIntv;
INT16U        TxIntv;
INT16U        TISet;
S_UARTx_Set   Setting;
USART_TypeDef *UARTx;
}S_BSP_UARTx;

/* Exported constants --------------------------------------------------------*/
#define UART_DEFAULT_MODE  0x00  /* 普通UART口使用    */
#define UART_HALFDUP_MODE  0x01	 /* 半双工模式, (IRDA), 扩展串口有20MS的转换延时 */
#define UART_RS485_MODE    0x33	 /* 半双工模式, RS485, 扩展串口有20MS的转换延时  */


extern S_BSP_UARTx   *BSP_UART1, *BSP_UART2, *BSP_UART3, *BSP_UART4, *BSP_UART5, *BSP_UART6, *BSP_UART7;


/* Exported functions ------------------------------------------------------- */
void SYS_UART_ISR(S_BSP_UARTx *BSP_UARTx);

/*******************************************************************************
* Function Name  : BSP_UART_FSR
* Description    : 串口字节流组帧服务程序，按超时位(时间)区分和字节个数区分 
* Input          : ComPort: 串口号1...7
* Output         : None
* Return         : None
*******************************************************************************/ 
void SYS_UART_FSR(INT8U ComPort);

INT8U BSP_UART_ClearRxBuff(INT8U ComPort);

/*******************************************************************************
* Description    : 是否有数据收到
* Input          : ComPort : 串口号1...5
* Return         : 1 / 0
*******************************************************************************/ 
INT16U BSP_UART_IsRxFrame(INT8U ComPort );

/*******************************************************************************
* Description    : 处理完接收的数据后，清楚一下端口，重新接收
* Input          : ComPort : 串口号1...5
* Return         : 1 
*******************************************************************************/ 
INT8U BSP_UART_ClearRxBuff(INT8U ComPort);
INT8U BSP_UART_ClearTxBuff(INT8U ComPort);
/*******************************************************************************
* Function Name  : BSP_UART_Init
* Description    : 串口初始化
* Input          : ComPort:    1 ... 5
*                  Settings:   串口配置
*              	   Mail_Queue：邮箱或队列指针
* Output         : None
* Return         : 1 成功 / 0 失败 
*******************************************************************************/
INT8U BSP_UART_Init(INT8U ComPort, S_UARTx_Set *UartSet);

/*******************************************************************************
* Function Name  : BSP_UART_Write
* Description    : 往串口发送一帧数据。若前一帧未发完，附加到前一帧数据末尾发。
*                  若缓冲去长度不够，本帧数据不发送返回失败。帧与帧之间默认有
*                  一定的时间间隔。时间间隔可以BSP_UART_TxIntvSet设置，设0为无
*                  间隔。
* Input          : ComPort : 串口号1...7
*				   pData: 帧缓冲首地址
*				   Number: 帧长度
* Output         : None
* Return         : TRUE / FALSE
* Note : BSP_UART_Write可以有两种工作方式
*        1) 帧与帧之间默认有一定的发送时间间隔。若前一帧未发送完毕，调用
*           BSP_UART_Write发下一帧将不能发送，返回FALSE。为使数据发送成功，
*           返回FALSE时，应该OSTimeDly或DelayUS，等待一段时间后再数据重发。
*        2) 可以调用函数BSP_UART_TxIntvSet设置发送时间间隔，设0为无间隔。若前
*           一帧未发送完毕，调用BSP_UART_Write发下一帧，新的数据将附加到前一
*           帧数据末尾发。若缓冲去长度不够，本帧数据不发送返回失败。
*******************************************************************************/ 
INT8U BSP_UART_Write(INT8U ComPort, INT8U *FrameBuf, INT16U FrameLen);



#endif /*__xxx_xx_H */

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/




