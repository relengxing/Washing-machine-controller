/******************** (C) COPYRIGHT2014 NLKJ ********************
* File Name          : Bsp_Uart.c
* Author             : pentral
* Version            : V1.00
* Date               : 2014.11.13
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_Uart.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_leuart.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
S_BSP_UARTx   StructUART[7];
S_BSP_UARTx   *BSP_UART1 = &StructUART[0];
S_BSP_UARTx   *BSP_UART2 = &StructUART[1];
S_BSP_UARTx   *BSP_UART3 = &StructUART[2];
S_BSP_UARTx   *BSP_UART4 = &StructUART[3];


/* Private function prototypes -----------------------------------------------*/
void  UART_RxEnable(S_BSP_UARTx *BSP_UARTx,INT8U Enable);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Function Name: UARTx_PIN_CFG
* Description: Define UARTx Rx and Tx Pins 配置的UART1 对应efm32UART0 
* Input:  Nothing
* Output: Nothing
* Return: Nothing
******************************************************************************/
void UART1_PIN_CFG(void)
{

	/* Configure GPIO pin for UART TX */
	/* To avoid false start, configure output as high. */
	GPIO_PinModeSet( BSP_UART_0_TXPORT, BSP_UART_0_TXPIN, gpioModePushPull, 1 );

	/* Configure GPIO pin for UART RX */
	GPIO_PinModeSet( BSP_UART_0_RXPORT, BSP_UART_0_RXPIN, gpioModeInput, 1 );

	LEUART0->ROUTE |= LEUART_ROUTE_TXPEN | LEUART_ROUTE_RXPEN | BSP_UART_0_LOCATION;
	
}

void UART2_PIN_CFG(void)
{
	/* Enable High Frequency Peripherals */
	CMU_ClockEnable(cmuClock_HFPER, true);

	/* Enable clocks to GPIO */
	CMU_ClockEnable(cmuClock_GPIO, true);
	/* Configure GPIO pin for UART TX */
	/* To avoid false start, configure output as high. */
	GPIO_PinModeSet( BSP_UART_1_TXPORT, BSP_UART_1_TXPIN, gpioModePushPull, 1 );

	/* Configure GPIO pin for UART RX */
	GPIO_PinModeSet( BSP_UART_1_RXPORT, BSP_UART_1_RXPIN, gpioModeInput, 1 );

/* Module USART0 is configured to location 1 */
	USART0->ROUTE = (USART0->ROUTE & ~_USART_ROUTE_LOCATION_MASK) ;
	USART0->ROUTE |= USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | BSP_UART_1_LOCATION;

}


void UART3_PIN_CFG(void)
{
	/* Enable High Frequency Peripherals */
	CMU_ClockEnable(cmuClock_HFPER, true);

	/* Enable clocks to GPIO */
	CMU_ClockEnable(cmuClock_GPIO, true);
	/* Configure GPIO pin for UART TX */
	/* To avoid false start, configure output as high. */
	GPIO_PinModeSet( BSP_UART_2_TXPORT, BSP_UART_2_TXPIN, gpioModePushPull, 1 );

	/* Configure GPIO pin for UART RX */
	GPIO_PinModeSet( BSP_UART_2_RXPORT, BSP_UART_2_RXPIN, gpioModeInput, 1 );


	USART1->ROUTE |= USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | BSP_UART_2_LOCATION;

}


/*******************************************************************************
* Function Name  : GetCommHandle
* Description    : 串口号到结构体的映射 
* Input          : 串口号 1...7
* Output         : None
* Return         : 结构体
*******************************************************************************/
S_BSP_UARTx *GetCommHandle(INT8U ComPort)
{
	S_BSP_UARTx *BSP_UARTx = 0;

	switch(ComPort)
	{
		case 1:
		     BSP_UARTx = BSP_UART1;
			 break;

		case 2:
		     BSP_UARTx = BSP_UART2;
			 break;

		case 3:
		     BSP_UARTx = BSP_UART3;
			 break;


		default:
			 ;
	}

	return BSP_UARTx;
}

INT8U GetCommPort(S_BSP_UARTx *BSP_UARTx)
{
	if(BSP_UARTx == BSP_UART1)
		return 1;
	if(BSP_UARTx == BSP_UART2)
		return 2;
	if(BSP_UARTx == BSP_UART3)
		return 3;	
	return 1;
}


/*******************************************************************************
* Function Name  : BSP_UART_Init
* Description    : 串口初始化
* Input          : ComPort:    1 ... 7
*                  Settings:   串口配置
*              	   Mail_Queue：邮箱或队列指针
* Output         : None
* Return         : 1 成功 / 0 失败 
*******************************************************************************/
INT8U BSP_UART_Init(INT8U ComPort, S_UARTx_Set *UartSet)
{
//	CMU_Clock_TypeDef  bsp_uart_clock ;
//	USART_TypeDef  *bsp_usart_com ;
	S_BSP_UARTx * BSP_UARTx ;

	
	BSP_UARTx =	GetCommHandle (ComPort);
	BSP_UARTx->pSendBuf = UartSet->TxBuf;
	BSP_UARTx->pRecvBuf = UartSet->RxBuf;
	BSP_UARTx->MaxTxBufLen = UartSet->TxBufLen;
	BSP_UARTx->MaxRxBufLen = UartSet->RxBufLen;
	BSP_UARTx->Setting.Mode = UartSet->Mode;
	//判断帧时间
	switch(UartSet->BaudRate)
	{
		case 300:
			 BSP_UARTx->FrameIntv = (OS_TICKS_PER_SEC *4)/5;/* 800ms, 240位时间 */
			 break;
		case 600:
			 BSP_UARTx->FrameIntv = (OS_TICKS_PER_SEC *2)/5;/* 400ms, 240位时间 */
			 break;
		case 1200:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 5;   /* 200ms, 240位时间 */
			 break;
		case 2400:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 10;  /* 100ms, 240位时间 */
			 break;
		case 4800:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 20;  /* 50ms,  240位时间 */
			 break;
		case 9600:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 50;  /* 20ms,  192位时间 */
			 break;
		case 19200:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 50;  /* 20ms,  384位时间 */
			 break;
		case 38400:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 100; /* 10ms,  384位时间 */
			 break;
		case 57600:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 100; /* 10ms,  576位时间 */
			 break;
		case 115200:
			 BSP_UARTx->FrameIntv = OS_TICKS_PER_SEC / 5; /* 200ms,   */
			 break;
		default:
			 BSP_UARTx->FrameIntv = 9;
	}	

	BSP_UARTx->RxNum = 0;
    BSP_UARTx->TxBusy = 0;
	BSP_UARTx->TxComplete = 0;
	BSP_UARTx->RxOffset = 0;
	BSP_UARTx->RxPointer = 0;
	BSP_UARTx->FrameRxIntv = 0;
	BSP_UARTx->TxIntv = 0;
	BSP_UARTx->TISet = BSP_UARTx->FrameIntv + 1;	

	/* Enable High Frequency Peripherals */
	CMU_ClockEnable(cmuClock_HFPER, true);

	/* Enable clocks to GPIO */
	CMU_ClockEnable(cmuClock_GPIO, true);
	
	if(1==ComPort)
	{
		LEUART_Init_TypeDef leUsartInit = LEUART_INIT_DEFAULT;
		//clock config
		CMU_ClockEnable(cmuClock_CORELE, true);
		CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
		CMU_ClockEnable( cmuClock_LEUART0, true );
		
		//rate config
		leUsartInit.baudrate = UartSet->BaudRate ;
		leUsartInit.databits = leuartDatabits8 ;//UartSet->DataBits ;
//		LINK_REF_FREQUENCY   CMU_ClockFreqGet(cmuClock_LEUART0)
		leUsartInit.refFreq = CMU_ClockFreqGet(cmuClock_LEUART0) ;
		leUsartInit.parity   = leuartNoParity   ;
		if(usartEvenParity == UartSet->Parity)
		{
			leUsartInit.parity   = leuartEvenParity ;		
		}
		else if(usartOddParity == UartSet->Parity)
		{
			leUsartInit.parity   = leuartOddParity   ;		
		}		
		leUsartInit.stopbits = leuartStopbits1 ;
		LEUART_Reset(LEUART0);	
		LEUART_Init( LEUART0, &leUsartInit );
		//interrupt config
		LEUART0->IFC = _USART_IFC_MASK;
		NVIC_ClearPendingIRQ(LEUART0_IRQn);
		NVIC_EnableIRQ(LEUART0_IRQn);
		LEUART0->IEN = LEUART_IEN_RXDATAV |LEUART_IEN_TXC;
		//pin config
		UART1_PIN_CFG();
		LEUART_TxDmaInEM2Enable(LEUART0, 0);
		LEUART_RxDmaInEM2Enable(LEUART0, 0);

	}
	else if(2==ComPort)
	{
		USART_InitAsync_TypeDef usartInit = USART_INITASYNC_DEFAULT;
		CMU_ClockEnable( cmuClock_USART0, true );
		usartInit.baudrate = UartSet->BaudRate ;
		usartInit.databits = (USART_Databits_TypeDef)UartSet->DataBits ;
		usartInit.parity   = (USART_Parity_TypeDef)UartSet->Parity   ;
		usartInit.stopbits = (USART_Stopbits_TypeDef)UartSet->StopBits ;		
//		usartInit.prsRxCh  = usartPrsRxCh1 ;		
		USART_InitAsync(USART0 ,&usartInit);
		
		//interrupt config
		USART0->IFC = _USART_IFC_MASK;
		
		NVIC_ClearPendingIRQ(USART0_RX_IRQn);
		NVIC_EnableIRQ(USART0_RX_IRQn);

		NVIC_ClearPendingIRQ(USART0_TX_IRQn);
		NVIC_EnableIRQ(USART0_TX_IRQn);
		UART2_PIN_CFG();		
		USART0->IEN  |= USART_IEN_RXDATAV | USART_IEN_TXC;


	}	
	else if(3==ComPort)
	{
		USART_InitAsync_TypeDef usartInit = USART_INITASYNC_DEFAULT;
		CMU_ClockEnable( cmuClock_USART1, true );
		usartInit.baudrate = UartSet->BaudRate ;
		usartInit.databits = (USART_Databits_TypeDef)UartSet->DataBits ;
		usartInit.parity   = (USART_Parity_TypeDef)UartSet->Parity   ;
		usartInit.stopbits = (USART_Stopbits_TypeDef)UartSet->StopBits ;
//		usartInit.prsRxCh  = usartPrsRxCh2 ;		
		USART_InitAsync(USART1 ,&usartInit);
		
		//interrupt config
		USART1->IFC = _USART_IFC_MASK;
		NVIC_ClearPendingIRQ(USART1_RX_IRQn);
		NVIC_EnableIRQ(USART1_RX_IRQn);
//		USART1->IEN |= USART_IEN_RXDATAV;

		NVIC_ClearPendingIRQ(USART1_TX_IRQn);
		NVIC_EnableIRQ(USART1_TX_IRQn);
		UART3_PIN_CFG();
		USART1->IEN  |= USART_IEN_RXDATAV | USART_IEN_TXC;


	}	

	return TRUE ;
}


//////////////////////////中断处理/////////////////////////////////////
void LEUART0_IRQHandler(void)
{	
	SYS_UART_ISR(BSP_UART1);
}

void USART0_RX_IRQHandler(void)
{  
	SYS_UART_ISR(BSP_UART2);	
}

void USART0_TX_IRQHandler(void)
{  
	SYS_UART_ISR(BSP_UART2);
}


void USART1_RX_IRQHandler(void)
{  
	SYS_UART_ISR(BSP_UART3);	
}

void USART1_TX_IRQHandler(void)
{  
	SYS_UART_ISR(BSP_UART3);
}

/*****************************************************************************
* Function Name: UART_RxEnable
* Description:   接收数据使能
* Input : BSP_UARTx : BSP_UART1...BSP_UART5
* Output: NONE
* Return: NONE
*****************************************************************************/
void  UART_RxEnable(S_BSP_UARTx *BSP_UARTx,INT8U Enable)
{
	volatile INT16U tmpreg = 0x00;
	
    if(BSP_UARTx < BSP_UART4)  
	{
		if(BSP_UART1 ==BSP_UARTx)
		{
			if(Enable)
			{				
				LEUART0->CMD |= 0x01 ;
			}
			else
			{
				LEUART0->CMD &= ~0x01 ;
				
			}		
		}
		else if(BSP_UART2 ==BSP_UARTx)
		{
			if(Enable)
			{				
				USART0->CMD |= 0x01 ;
			}
			else
			{
				USART0->CMD &= ~0x01 ;
				
			}		
		}
		else if(BSP_UART3 ==BSP_UARTx)
		{
			if(Enable)
			{				
				USART1->CMD |= 0x01 ;
			}
			else
			{
				USART1->CMD &= ~0x01 ;				
			}				
		}
	}
}

/*******************************************************************************
* Description    : 485 
* Input          : 485Tx/RX引脚使能
* Output         : None
* Return         : None
*******************************************************************************/
void MODE485_TxEnable(S_BSP_UARTx *BSP_UARTx)
{
	GPIO_PinModeSet( BSP_485_PORT, BSP_485_PIN, gpioModePushPull, 1 );

}
void MODE485_RxEnable(S_BSP_UARTx *BSP_UARTx)
{
	GPIO_PinModeSet( BSP_485_PORT, BSP_485_PIN, gpioModePushPull, 0 );
}

/*******************************************************************************
* Description    : 中断服务程序 
* Input          : 串口号对应的结构体
* Output         : None
* Return         : None
*******************************************************************************/
void SYS_UART_ISR(S_BSP_UARTx *BSP_UARTx)
{
//	INT8U TxStatus;
	volatile   INT8U rcvData;
	INT32U txrxFlag ;
    /* 半双工模式, 最后发送字节发\收中断顺序到来 */
	if(BSP_UART1 == BSP_UARTx)
	{
		txrxFlag = LEUART_IntGet(LEUART0);
		if ( txrxFlag & LEUART_IF_RXDATAV) //是否有数据接收到
		{
			rcvData = LEUART0->RXDATA; 
			if(BSP_UARTx->RxNum < BSP_UARTx->MaxRxBufLen)
			{
				BSP_UARTx->pRecvBuf[BSP_UARTx->RxNum] = rcvData; 
				BSP_UARTx->RxNum++; 							
			}
		}
		LEUART_IntClear(LEUART0, ~_LEUART_IFC_RESETVALUE);
		if(txrxFlag & LEUART_IF_TXC)
		{
			BSP_UARTx->TxComplete++;
			
			if(BSP_UARTx->TxComplete < BSP_UARTx->TxBusy)	/* 检查发送是否完成 */
			{
				LEUART_Tx(LEUART0,BSP_UARTx->pSendBuf[BSP_UARTx->TxComplete]);
			}
			else
			{
			
				// send all data ok
		     if(BSP_UARTx->Setting.Mode == UART_RS485_MODE)
			 {
			 	MODE485_RxEnable(BSP_UARTx);
//				UART_RxEnable(BSP_UARTx,1);
			 }
//			 if(BSP_UARTx->Setting.Mode == UART_HALFDUP_MODE)
//			 {
//				UART_RxEnable(BSP_UARTx,1);
//			 }
				 if(BSP_UARTx->TxBusy)
				 {
					BSP_UARTx->TxIntv= BSP_UARTx->TISet; /* 帧间隔设置 */				 
				 }
				 BSP_UARTx->TxComplete = 0;
				 BSP_UARTx->TxBusy= 0; 			
			}
		}		
	}
	else
	{
		USART_TypeDef * pBSP_UARTx ;
		if(BSP_UART2==BSP_UARTx)
		{
			pBSP_UARTx = USART0 ;  
		}
		else if(BSP_UART3==BSP_UARTx)
		{
			pBSP_UARTx = USART1 ; 
		}
		else
		{
			return ;
		}
		if(pBSP_UARTx->IF &USART_IF_RXDATAV)
		{
			rcvData = USART_Rx(pBSP_UARTx);
			if(BSP_UARTx->RxNum < BSP_UARTx->MaxRxBufLen)
			{
				BSP_UARTx->pRecvBuf[BSP_UARTx->RxNum] = rcvData; 
				BSP_UARTx->RxNum++; 							
			}
			
		}
		if(pBSP_UARTx->IF &USART_IF_TXC)
		{
			//
			pBSP_UARTx->CMD&=~
			BSP_UARTx->TxComplete++;
			pBSP_UARTx->IFC |=USART_IFC_TXC;
			if(BSP_UARTx->TxComplete < BSP_UARTx->TxBusy)	/* 检查发送是否完成 */
			{
				USART_Tx(pBSP_UARTx,BSP_UARTx->pSendBuf[BSP_UARTx->TxComplete]);
			}
			else
			{			
				 if(BSP_UARTx->TxBusy)
				 {
					BSP_UARTx->TxIntv= BSP_UARTx->TISet; /* 帧间隔设置 */				 
				 }
				 BSP_UARTx->TxComplete = 0;
				 BSP_UARTx->TxBusy= 0; 
				 if(BSP_UARTx->Setting.Mode == UART_HALFDUP_MODE)
				 {
					UART_RxEnable(BSP_UARTx,1);
				 }					
			}
		}		
		
	}
	return;
}


/*******************************************************************************
* Function Name  : UartPostRcvMsg
* Description    : post receive msg
* Input 1        : ComPort:  1 - 7
* Output         : None
* Return         : None
*******************************************************************************/
void SendReceiveMsg(S_BSP_UARTx *BSP_UARTx)
{
	if((BSP_UARTx->RxNum != 0xFFFF)&&(!BSP_UARTx->RxNum))
	{
		return ;
	}
	/* 填 充 消 息 */			
	BSP_UARTx->FrameRxFlag  = 1;
	BSP_UARTx->RxNum= BSP_UARTx->RxPointer - BSP_UARTx->RxOffset;

	/* 下一帧起始地址 */
	BSP_UARTx->RxOffset = 0;

}



/*******************************************************************************
* Function Name  : BSP_UART_FSR
* Description    : 串口字节流组帧服务程序，按超时位(时间)区分和字节个数区分 
* Input          : ComPort: 串口号1...7
* Output         : None
* Return         : None
*******************************************************************************/ 
void SYS_UART_FSR(INT8U ComPort)
{
	S_BSP_UARTx *BSP_UARTx;
	BSP_UARTx = GetCommHandle(ComPort);

	/* 无接收缓冲区，返回 */
	if(BSP_UARTx->MaxRxBufLen == 0)
		return;

	/* 发送祯间隔   */
	if(BSP_UARTx->MaxTxBufLen && BSP_UARTx->TxIntv)
	{
		BSP_UARTx->TxIntv--;
	}
	if(BSP_UARTx->FrameRxFlag )
	{
		return ;
	}
	
	/* 数据接收分帧处理 */
    if((BSP_UARTx->FrameIntv & 0x8000) == 0)
	{	
		/* 时 间 分 帧 */
		if(BSP_UARTx->RxPointer == BSP_UARTx->RxNum)
		{
			/* 未收到最新数据   */
			BSP_UARTx->FrameRxIntv++;
			if(BSP_UARTx->FrameRxIntv >= BSP_UARTx->FrameIntv)
			{
				if(BSP_UARTx->RxOffset != BSP_UARTx->RxPointer)
				{/* 不 是 空 帧 */
				    SendReceiveMsg(BSP_UARTx);
					BSP_UARTx->FrameRxIntv = 0;
				}
			}
		}
		else  
		{/* 收到最新数据   */
		    BSP_UARTx->RxPointer = BSP_UARTx->RxNum;
			BSP_UARTx->FrameRxIntv = 0;
		}
			

	} // if((BSP_UARTx->FrameIntv & 0x8000) == 0)
	else
	{/* 字节数分帧 */
		if(BSP_UARTx->RxNum != BSP_UARTx->RxOffset)
		{
			if((BSP_UARTx->RxNum - BSP_UARTx->RxOffset >= (BSP_UARTx->FrameIntv & 0x3fff)) || 
			(BSP_UARTx->RxNum >= BSP_UARTx->MaxRxBufLen))
			{
				BSP_UARTx->RxPointer = BSP_UARTx->RxOffset + (BSP_UARTx->FrameIntv & 0x3fff);
				if(BSP_UARTx->RxPointer >= BSP_UARTx->MaxRxBufLen)
				{/* 缓冲区不够的情况 */
					BSP_UARTx->RxPointer = BSP_UARTx->MaxRxBufLen;
					BSP_UARTx->RxNum = BSP_UARTx->MaxRxBufLen;
				}
				SendReceiveMsg(BSP_UARTx);
			}
		}
	}

	return;
}


/******************************************************************************
* Function Name: BSP_UART_TxState
* Description: 获取串口发送状态
* Input:  ComPort : 串口号1...7        
* Output: Nothing
* Return: 1 send busy / 0 send free
******************************************************************************/
INT8U BSP_UART_TxState(INT8U ComPort)
{
	S_BSP_UARTx *BSP_UARTx;

	BSP_UARTx = GetCommHandle(ComPort);

	if(BSP_UARTx->TxComplete/*FIFO未空*/|| BSP_UARTx->TxBusy || BSP_UARTx->TxIntv)
		return 1;
	else
		return 0;
}

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
INT8U BSP_UART_Write(INT8U ComPort, INT8U *FrameBuf, INT16U FrameLen)
{
	S_BSP_UARTx *BSP_UARTx;

	BSP_UARTx = GetCommHandle(ComPort);	

	if(FrameLen > BSP_UARTx->MaxTxBufLen - BSP_UARTx->TxBusy || FrameLen == 0 || (BSP_UART_TxState(ComPort)&&BSP_UARTx->TISet))
	{
		return FALSE;
	}
	if(BSP_UARTx->Setting.Mode == UART_RS485_MODE)
	{
		MODE485_TxEnable(BSP_UARTx);
//	    UART_RxEnable(BSP_UARTx,0);
	}
	if(BSP_UARTx->Setting.Mode == UART_HALFDUP_MODE)
	{
		UART_RxEnable(BSP_UARTx,0);
	}
	
	if(BSP_UARTx->TxBusy == 0)
	{/* 空闲状态 */		
		BSP_UARTx->TxBusy = FrameLen;
		BSP_UARTx->TxComplete = 0;
		memcpy((void*)BSP_UARTx->pSendBuf, FrameBuf, FrameLen);
		
		/* Send the first data with UARTx, and the next data send
	       by ISR automatic */
	    if(ComPort < 6)
		{
			if(1==ComPort)
			{
				LEUART_Tx(LEUART0,BSP_UARTx->pSendBuf[0]);
			}
			else
			{

				if(BSP_UART2==BSP_UARTx)
				{
					BSP_UARTx->UARTx = USART0 ;
				}
				else if(BSP_UART3==BSP_UARTx)
				{
					BSP_UARTx->UARTx = USART1 ;
				}
				else
				{
					return FALSE;
				}
				USART_Tx(BSP_UARTx->UARTx, BSP_UARTx->pSendBuf[0]);
				
			}
		}
	}
	else
	{
		/* UART数 据 正 在 发 送  */
		memcpy((void *)(BSP_UARTx->pSendBuf + BSP_UARTx->TxBusy), FrameBuf, FrameLen);
		BSP_UARTx->TxBusy += FrameLen;
USART0->IEN  &= ~USART_IEN_RXDATAV ;

	}

	return TRUE;
}

/*******************************************************************************
* Description    : 是否有数据收到,有返回接收的长度，否则为0 表示无接收
* Input          : ComPort : 串口号1...5
* Return         : 1 / 0
*******************************************************************************/ 
INT16U BSP_UART_IsRxFrame(INT8U ComPort )
{
	S_BSP_UARTx *BSP_UARTx;
	BSP_UARTx = GetCommHandle(ComPort);	
	if(BSP_UARTx->FrameRxFlag)
	{
		return BSP_UARTx->RxNum ;
	}
	return 0 ;
}

/*******************************************************************************
* Description    : 处理完接收的数据后，清除一下端口，重新接收
* Input          : ComPort : 串口号1...5
* Return         : 1 
*******************************************************************************/ 
INT8U BSP_UART_ClearRxBuff(INT8U ComPort)
{
	S_BSP_UARTx *BSP_UARTx;
	BSP_UARTx = GetCommHandle(ComPort);	
	BSP_UARTx->FrameRxFlag = 0 ;
	BSP_UARTx->RxOffset = 0;
	BSP_UARTx->RxNum = 0 ;
	memset((void*)BSP_UARTx->pRecvBuf,0x00,BSP_UARTx->MaxRxBufLen);
	
	return 1 ;
}
/*********************************************************************************
* Description    : 清空发送缓冲区
* Input          : ComPort : 串口号1...5
* Return         : 1 
**********************************************************************************/
INT8U BSP_UART_ClearTxBuff(INT8U ComPort)
{
	S_BSP_UARTx *BSP_UARTx;
	BSP_UARTx = GetCommHandle(ComPort);	
	BSP_UARTx->TxBusy = 0;
	memset((void*)BSP_UARTx->pSendBuf,0x00,BSP_UARTx->MaxTxBufLen);
	return 1 ;
}

/******************* (C) COPYRIGHT 2014 NLKJ *****END OF FILE******************/
