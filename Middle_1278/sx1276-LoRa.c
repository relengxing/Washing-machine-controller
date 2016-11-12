/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-LoRa.c
 * \brief      SX1276 RF chip driver mode LoRa
 *
 * \version    2.0.0 
 * \date       May 6 2013
 * \author     Gregory Cristian
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */

#include "bsp_config.h"
#include "sx1276-LoRa.h"
#include "sx1276-Fsk.h"
#include "Bsp_SX127X.h"
#include "convertDataInterface.h"
#include "sysTick.h"
//#include "Bsp_SX127X.h"
//#include "bsp_SPI.h"

//#include "FlashAddrConfig.h"
#define Get_Tick_Count	SysTick_Get_Second_Counts   //need 32 bit 

/*!
 * Constant values need to compute the RSSI value
 */
#define RSSI_OFFSET_LF                              -155.0
#define RSSI_OFFSET_HF                              -150.0

#define NOISE_ABSOLUTE_ZERO                         -174.0

#define NOISE_FIGURE_LF                                4.0
#define NOISE_FIGURE_HF                                6.0 

#define RF_RECEIVE_TEST                                0x01
uint8_t SX1276Regs[0x70];

/*!
 * Precomputed signal bandwidth log values
 * Used to compute the Packet RSSI value.
 */
const double SignalBwLog[] =
{
    3.8927900303521316335038277369285,  // 7.8 kHz
    4.0177301567005500940384239336392,  // 10.4 kHz
    4.193820026016112828717566631653,   // 15.6 kHz
    4.31875866931372901183597627752391, // 20.8 kHz
    4.4948500216800940239313055263775,  // 31.2 kHz
    4.6197891057238405255051280399961,  // 41.6 kHz
    4.795880017344075219145044421102,   // 62.5 kHz
    5.0969100130080564143587833158265,  // 125 kHz
    5.397940008672037609572522210551,   // 250 kHz
    5.6989700043360188047862611052755   // 500 kHz
};

const double RssiOffsetLF[] =
{   // These values need to be specify in the Lab
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
};

const double RssiOffsetHF[] =
{   // These values need to be specify in the Lab
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
};

/*!
 * Frequency hopping frequencies table
 */
const double HoppingFrequencies[] =
{
    916500000,
    923500000,
    906500000,
    917500000,
    917500000,
    909000000,
    903000000,
    916000000,
    912500000,
    926000000,
    925000000,
    909500000,
    913000000,
    918500000,
    918500000,
    902500000,
    911500000,
    926500000,
    902500000,
    922000000,
    924000000,
    903500000,
    913000000,
    922000000,
    926000000,
    910000000,
    920000000,
    922500000,
    911000000,
    922000000,
    909500000,
    926000000,
    922000000,
    918000000,
    925500000,
    908000000,
    917500000,
    926500000,
    908500000,
    916000000,
    905500000,
    916000000,
    903000000,
    905000000,
    915000000,
    913000000,
    907000000,
    910000000,
    926500000,
    925500000,
    911000000,
};

// Default settings
tLoRaSettings LoRaSettings =
{
	455000000,        		// RFFrequency 432000000  
	14,						//信道号	
    20,               		// Power
    7,                		// SignalBw [0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
							// 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
    10,               		// SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
	490,				    // Rx Preamable Len   3S-490 6S-870
	8,						// coil preamable len
	8,					    // meter preamable len
    1,                		// ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    TRUE,             		// CrcOn [0: OFF, 1: ON]
    FALSE,             		// ImplicitHeaderOn [0: OFF, 1: ON]
    FALSE,//0,               // RxSingleOn [0: Continuous, 1 Single]
    FALSE,//0,              // FreqHopOn [0: OFF, 1: ON]
    255,              		// HopPeriod Hops every frequency hopping period symbols
    4,//100,//2,            // TxPacketTimeout
    6,//250,//5,            // RxPacketTimeout
    128,              		// PayloadLength (used for implicit header mode)
};

/*!
 * SX1276 LoRa registers variable
 */
tSX1276LR* SX1276LR;
/*!
 * Local RF buffer for communication support
 */
static uint8_t RFBuffer[RF_BUFFER_SIZE];

/*!
 * RF state machine variable
 */
static uint8_t RFLRState = RFLR_STATE_IDLE;

/*!
 * Rx management support variables
 */
static uint16_t RxPacketSize = 0;
static int8_t 	RxPacketSnrEstimate;
static double 	RxPacketRssiValue;
static uint8_t  RxGain = 1;
//static uint32_t RxTimeoutTimer = 0;
//static uint32_t TxTimeoutTimer = 0;
//static uint32_t TimeInterval = 0;
static uint32_t CADTimeoutTimer = 0;
static uint32_t PacketRxTimeout;
static uint32_t PacketTxTimeout;
/*!
 * PacketTimeout Stores the Rx window time value for packet reception
 */


//INT32U Get_Tick_Count(void)
//{
//	INT32U tmp = Sys_Get_RTC_Count( ) ;
//	return tmp ;

//}	

void SX1276_Second_Call(void)
{
	if(PacketRxTimeout)
	{
		PacketRxTimeout -- ;
	}
	if(PacketTxTimeout)
	{
		PacketTxTimeout -- ;
	}
	if(CADTimeoutTimer)
	{
		CADTimeoutTimer -- ;
	}
}

//结构体
void SX1276_WR_LoraInfor( uint8_t ord,tLoRaSettings *pLoRaSettings)
{
//	uint8_t len = sizeof(tLoRaSettings);
//	uint8_t *p = (uint8_t*)pLoRaSettings ;
//	tLoRaSettings tmpLoRaSettings ;
//	if(ord)
//	{
////		BSP_WriteDataToAT(APP_RF_ADDR ,p,len);   		
//	}
//	else
//	{
////		BSP_ReadDataFromAT(APP_RF_ADDR ,(uint8_t*)&tmpLoRaSettings,len);  
//		if((0xff!=tmpLoRaSettings.Channel_RF) &&(0xff!=tmpLoRaSettings.Power))
//		{
//			//memcpy((uint8_t*)&LoRaSettings,(uint8_t*)&LoRaSettings,len);
//			LoRaSettings.Channel_RF = tmpLoRaSettings.Channel_RF ;
//			LoRaSettings.RF_Rx_Preamable_Len = tmpLoRaSettings.RF_Rx_Preamable_Len ;
//			LoRaSettings.RF_Tx_M_C_Preamable_Len = tmpLoRaSettings.RF_Tx_M_C_Preamable_Len ;
//			LoRaSettings.RF_Tx_M_M_Preamable_Len = tmpLoRaSettings.RF_Tx_M_M_Preamable_Len ;
//			LoRaSettings.SignalBw				 = tmpLoRaSettings.SignalBw ;
//			LoRaSettings.SpreadingFactor		 = tmpLoRaSettings.SpreadingFactor ;
//		}
//	}
	
}

//上电的时候重新对结构体赋值
void SX1276_LoraInforInit(void)
{
//	SX1276_WR_LoraInfor(0,&LoRaSettings);
}
//设置信道号
//ord：0 读取 1：写 channel 信道号 
uint8_t  SX1276_WR_Channel(uint8_t ord,uint8_t *channel)
{
	if(ord)
	{
		if(*channel > 150)
		{
			return 0 ;
		}
		LoRaSettings.Channel_RF = *channel ;
		SX1276_WR_LoraInfor(1,&LoRaSettings);		
	}
	else
	{
		*channel = LoRaSettings.Channel_RF;
		
	}
	return 1 ;
}

//设置SP BW
//ord：0 读取 1：写 
uint8_t  SX1276_WR_Sp_and_Bw(uint8_t ord,uint8_t *p,uint8_t len )
{
	if(ord)
	{
		if((*p < 13)&&(*p>5))
		{
			LoRaSettings.SpreadingFactor = *p++ ;
		}
		if(*p < 10)
		{
			LoRaSettings.SignalBw = *p++ ;
		}
		SX1276_WR_LoraInfor(1,&LoRaSettings);		
	}
	else
	{
		*p++ = LoRaSettings.SpreadingFactor ;
		*p++ = LoRaSettings.SignalBw ;
	}
	return 1 ;
}

//Preamble
//uint8_t  SX1276_WR_Preamble(uint8_t ord,uint8_t *pPreamable,uint8_t len)
//{
//	if(ord)
//	{
//		LoRaSettings.RF_Rx_Preamable_Len = 8 ;
//		pPreamable++ ;
//		LoRaSettings.RF_Tx_M_C_Preamable_Len = TwoByteToShort(pPreamable) ;
//		pPreamable+=2 ;
//		LoRaSettings.RF_Tx_M_M_Preamable_Len = TwoByteToShort(pPreamable) ;
//		SX1276_WR_LoraInfor(1,&LoRaSettings);		
//	}
//	else
//	{
//		*pPreamable++ = 8 ;
//		memcpy(pPreamable,(uint8_t*)&LoRaSettings.RF_Tx_M_C_Preamable_Len,2);
//		pPreamable+=2 ;
//		memcpy(pPreamable,(uint8_t*)&LoRaSettings.RF_Tx_M_M_Preamable_Len,2);
//	}  
//	return 5 ;
//}

/*!
 * Tx management support variables
 */
static uint16_t TxPacketSize = 0;

void SX1276LoRaInit( void )
{
    RFLRState = RFLR_STATE_IDLE;

    SX1276LoRaSetDefaults();
    
    SX1276Write(REG_LR_LNA,RFLR_LNA_GAIN_G1);
	
    // set the RF settings 
    SX1276LoRaSetRFFrequency( LoRaSettings.RFFrequency );
    SX1276LoRaSetSpreadingFactor( LoRaSettings.SpreadingFactor ); // SF6 only operates in implicit header mode.
    SX1276LoRaSetErrorCoding( LoRaSettings.ErrorCoding );
    SX1276LoRaSetPacketCrcOn( LoRaSettings.CrcOn );
    SX1276LoRaSetSignalBandwidth( LoRaSettings.SignalBw );

    SX1276LoRaSetImplicitHeaderOn( LoRaSettings.ImplicitHeaderOn );
    SX1276LoRaSetSymbTimeout( 0x3FF );
    SX1276LoRaSetPayloadLength( LoRaSettings.PayloadLength );
    SX1276LoRaSetLowDatarateOptimize( TRUE );
	if(TRUE == LoRaSettings.CrcOn)
	{
		SX1276LoRaSetOptimizedReceiverResponse();		
	}
    if( LoRaSettings.RFFrequency > 400000000 )
    {
        SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST );
        SX1276LoRaSetPa20dBm( TRUE );
        SX1276LoRaSetRFPower( LoRaSettings.Power );
    }
    else
    {
        SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_RFO );
        SX1276LoRaSetPa20dBm( FALSE );
        SX1276LoRaSetRFPower( LoRaSettings.Power );
    } 
    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY);
}

void SX1276LoRaSetDefaults( void )
{
    // REMARK: See SX1276 datasheet for modified default values.
    // Initialize FSK and LoRa registers structure
    SX1276LR = ( tSX1276LR* )SX1276Regs;

    SX1276Read( REG_LR_VERSION, &SX1276LR->RegVersion );  //0x12--0x20  REG_LR_VERSION--0x12
}

void SX1276LoRaSetOpMode( uint8_t opMode )
{
    static uint8_t opModePrev = RFLR_OPMODE_STANDBY;
    static bool antennaSwitchTxOnPrev = TRUE;
    bool antennaSwitchTxOn = FALSE;

    opModePrev = SX1276LR->RegOpMode & ~RFLR_OPMODE_MASK;

    if( opMode != opModePrev )
    {
        if( opMode == RFLR_OPMODE_TRANSMITTER )
        {
            antennaSwitchTxOn = TRUE;
        }
        else
        {
            antennaSwitchTxOn = FALSE;
        }
        if( antennaSwitchTxOn != antennaSwitchTxOnPrev )
        {
            antennaSwitchTxOnPrev = antennaSwitchTxOn;
            RXTX((bool) antennaSwitchTxOn ); // Antenna switch control
        }
        SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_MASK ) | opMode;

        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode |RFLR_OPMODE_FREQMODE_ACCESS_LF);      //设置为低频模式        
    }
}

uint8_t SX1276LoRaGetOpMode( void )
{
    SX1276Read( REG_LR_OPMODE, &SX1276LR->RegOpMode );
    
    return SX1276LR->RegOpMode & ~RFLR_OPMODE_MASK;
}

uint8_t SX1276LoRaReadRxGain( void )
{
    SX1276Read( REG_LR_LNA, &SX1276LR->RegLna );
    return( SX1276LR->RegLna >> 5 ) & 0x07;
}

double SX1276LoRaReadRssi( void )
{
    // Reads the RSSI value
    SX1276Read( REG_LR_RSSIVALUE, &SX1276LR->RegRssiValue );

    if( LoRaSettings.RFFrequency < 860000000 )  // LF
    {
        return RssiOffsetLF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegRssiValue;
    }
    else
    {
        return RssiOffsetHF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegRssiValue;
    }
}

uint8_t SX1276LoRaGetPacketRxGain( void )
{
    return RxGain;
}

int8_t SX1276LoRaGetPacketSnr( void )
{
    return RxPacketSnrEstimate;
}

double SX1276LoRaGetPacketRssi( void )
{
    return RxPacketRssiValue;
}

void SX1276LoRaStartRx( void )
{
    SX1276LoRaSetRFState( RFLR_STATE_RX_INIT );
}

void SX1276LoRaGetRxPacket( void *buffer, uint16_t *datasize )
{
	  if(RxPacketSize < RF_BUFFER_SIZE)
		{
				*datasize = RxPacketSize;
				RxPacketSize = 0;
				memcpy( ( void * )buffer, ( void * )RFBuffer, ( uint16_t )*datasize );
		}
		else
		{
			  RxPacketSize = 0;
//			  memset( ( void * )buffer, 0, 20);
		}
}	 

void SX1276LoRaSetTxPacket( const void *buffer, uint16_t datasize )
{
    if( LoRaSettings.FreqHopOn == FALSE )
    {
        TxPacketSize = datasize;
    }
    else
    {
        TxPacketSize = 255;
    }
    memcpy( ( void * )RFBuffer, buffer, ( size_t )TxPacketSize ); 

    RFLRState = RFLR_STATE_TX_INIT;
}

uint8_t SX1276LoRaGetRFState( void )
{
    return RFLRState;
}

void SX1276LoRaSetRFState( uint8_t state )
{
    RFLRState = state;
}

void SX1276ParaModify(uint8_t SignalBw, uint8_t SF)
{
    LoRaSettings.SignalBw = SignalBw;
	LoRaSettings.SpreadingFactor = SF;
}
/*!
 * \brief Process the LoRa modem Rx and Tx state machines depending on the
 *        SX1276 operating mode.
 *
 * \retval rfState Current RF state [RF_IDLE, RF_BUSY, 
 *                                   RF_RX_DONE, RF_RX_TIMEOUT,
 *                                   RF_TX_DONE, RF_TX_TIMEOUT]
 */
tRFProcessReturnCodes SX1276LoRaProcess( void )
{
    tRFProcessReturnCodes result = RF_IDLE;
	uint8_t rxSnrEstimate;
    switch( RFLRState )
    {
		case RFLR_STATE_IDLE:
			result = RF_IDLE;
			break;
		case RFLR_STATE_RX_INIT:
			
			RX_TX_CTRLPin_Control(TRUE);  //发送接收控制  至高表示接收
		
			SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
			SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
										//RFLR_IRQFLAGS_RXDONE |
										//RFLR_IRQFLAGS_PAYLOADCRCERROR |
										RFLR_IRQFLAGS_VALIDHEADER |
										RFLR_IRQFLAGS_TXDONE |
										RFLR_IRQFLAGS_CADDONE |
										//RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
										RFLR_IRQFLAGS_CADDETECTED;
			SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );	
			if( LoRaSettings.FreqHopOn == TRUE )
			{
				SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
				SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
				SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
			}
			else
			{
				SX1276LR->RegHopPeriod = 255;
			}       
			SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );         
										// RxDone                    RxTimeout                   FhssChangeChannel           CadDone
			SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_00;
										// CadDetected               ModeReady
			SX1276Write( REG_LR_DIOMAPPING1, SX1276LR->RegDioMapping1 );
		
			if( LoRaSettings.RxSingleOn == TRUE ) // Rx single mode
			{
				SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
			}
			else // Rx continuous mode
			{
				SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
				SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr ); 
				SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER );
			}
			
			memset( RFBuffer, 0, ( size_t )RF_BUFFER_SIZE );
			PacketRxTimeout = LoRaSettings.RxPacketTimeout;
			RFLRState = RFLR_STATE_RX_RUNNING;
			result = RF_RX_INIT;
			break;
		case RFLR_STATE_RX_RUNNING:
			result = RF_BUSY;			
			if(D0Pin_Control()) // RxDone
			{
				if( LoRaSettings.FreqHopOn == TRUE )
				{
					SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
					SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
				}
				// Clear Irq
				SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE  );
				RFLRState = RFLR_STATE_RX_DONE;
				
				result = RF_RX_RUNNING;
				break ;
			}
			if(! PacketRxTimeout )
			{
					RFLRState = RFLR_STATE_RX_TIMEOUT;
					result = RF_RX_TIMEOUT;
			}
			break;
		case RFLR_STATE_RX_DONE:
			if(TRUE == LoRaSettings.CrcOn)
			{
				SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
				if((SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_PAYLOAD_CRC16_ON) != RFLR_HOPCHANNEL_PAYLOAD_CRC16_ON)
				{
					if( LoRaSettings.RxSingleOn == TRUE ) // Rx single mode
					{
						RFLRState = RFLR_STATE_RX_INIT;
					}
					else
					{
						RFLRState = RFLR_STATE_RX_RUNNING; //if cad mode  may be is RFLR_STATE_CAD_INIT
					}
					result = RF_CHANNEL_EMPTY;
					break;				
				}
			}

			SX1276Read( REG_LR_IRQFLAGS, &SX1276LR->RegIrqFlags );
			if( ( SX1276LR->RegIrqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
			{
				// Clear Irq
				SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR  );
				
				if( LoRaSettings.RxSingleOn == TRUE ) // Rx single mode
				{
					RFLRState = RFLR_STATE_RX_INIT ;//RFLR_STATE_RX_INIT;
				}
				else
				{
					RFLRState = RFLR_STATE_RX_RUNNING ;//RFLR_STATE_RX_RUNNING;
				}
				result = RF_CHANNEL_EMPTY;
				break;
			}
			
			SX1276Read( REG_LR_PKTSNRVALUE, &rxSnrEstimate );
			if( rxSnrEstimate & 0x80 ) // The SNR sign bit is 1
			{
					// Invert and divide by 4
					RxPacketSnrEstimate = ( ( ~rxSnrEstimate + 1 ) & 0xFF ) >> 2;
					RxPacketSnrEstimate = -RxPacketSnrEstimate;
			}
			else
			{
					// Divide by 4
					RxPacketSnrEstimate = ( rxSnrEstimate & 0xFF ) >> 2;
			}			
			
			if( LoRaSettings.RFFrequency < 860000000 )  // LF
			{    
				if( RxPacketSnrEstimate < 0 )
				{
					RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_LF + ( double )RxPacketSnrEstimate;
				}
				else
				{    
					SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
					RxPacketRssiValue = RssiOffsetLF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
				}
			}
			else                                        // HF
			{    
				if( RxPacketSnrEstimate < 0 )
				{
					RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_HF + ( double )RxPacketSnrEstimate;
				}
				else
				{    
					SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
					RxPacketRssiValue = RssiOffsetHF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
				}
			}

//			if( LoRaSettings.RFFrequency < 860000000 )  // LF
//			{
//					SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
//					RxPacketRssiValue = -161.0 + ( double )SX1276LR->RegPktRssiValue;
//					if( RxPacketSnrEstimate < 0 && RxPacketRssiValue < -117 )
//					{
//						RxPacketRssiValue = -119 + RxPacketSnrEstimate;
//					}
//			}
//			else                                        // HF
//			{
//					SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
//					RxPacketRssiValue = -161.0 + ( double )SX1276LR->RegPktRssiValue;
//					if( RxPacketSnrEstimate < 0 && RxPacketRssiValue < -117 )
//					{
//							RxPacketRssiValue = -119 + RxPacketSnrEstimate;
//					}
//			}			
			
			if( LoRaSettings.RxSingleOn == TRUE ) // Rx single mode
			{
				SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
				SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );

				if( LoRaSettings.ImplicitHeaderOn == TRUE )
				{
					RxPacketSize = SX1276LR->RegPayloadLength;
					SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
				}
				else
				{
					SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
					RxPacketSize = SX1276LR->RegNbRxBytes;
					SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
				}
			}
			else // Rx continuous mode
			{
				SX1276Read( REG_LR_FIFORXCURRENTADDR, &SX1276LR->RegFifoRxCurrentAddr );

				if( LoRaSettings.ImplicitHeaderOn == TRUE )
				{
					RxPacketSize = SX1276LR->RegPayloadLength;
					SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxCurrentAddr;
					SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
					SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
				}
				else
				{
					SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
					if((SX1276LR->RegNbRxBytes) > RF_BUFFER_SIZE)
					{
						RFLRState = RFLR_STATE_RX_TIMEOUT;
						result = RF_RX_TIMEOUT;
						break;
					}
					RxPacketSize = SX1276LR->RegNbRxBytes;
					SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxCurrentAddr;
					SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
					SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
				}
			}
			
			if( LoRaSettings.RxSingleOn == TRUE ) // Rx single mode
			{
				RFLRState = RFLR_STATE_RX_INIT;
			}
			else // Rx continuous mode
			{
				RFLRState = RFLR_STATE_RX_RUNNING;
			}
			result = RF_RX_DONE;

			break;
		case RFLR_STATE_RX_TIMEOUT:
			RFLRState = RFLR_STATE_IDLE ;//RFLR_STATE_CAD_INIT; //还有可能处于接受状态，不能马上转入idle. 
			result = RF_RX_TIMEOUT;
			break;
		case RFLR_STATE_TX_INIT:
			RX_TX_CTRLPin_Control(FALSE);//CTRLPin_Control(FALSE);  //发送接收控制 拉低表示发送
		
			SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );

			if( LoRaSettings.FreqHopOn == TRUE )
			{
				SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
											RFLR_IRQFLAGS_RXDONE |
											RFLR_IRQFLAGS_PAYLOADCRCERROR |
											RFLR_IRQFLAGS_VALIDHEADER |
											//RFLR_IRQFLAGS_TXDONE |
											RFLR_IRQFLAGS_CADDONE |
											//RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
											RFLR_IRQFLAGS_CADDETECTED;


				SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;

				SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
				SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
			}
			else
			{
				SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
											RFLR_IRQFLAGS_RXDONE |
											RFLR_IRQFLAGS_PAYLOADCRCERROR |
											RFLR_IRQFLAGS_VALIDHEADER |
											//RFLR_IRQFLAGS_TXDONE |
											RFLR_IRQFLAGS_CADDONE |
											RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
											RFLR_IRQFLAGS_CADDETECTED;
				SX1276LR->RegHopPeriod = 0;
			}
			SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
			SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );

			// Initializes the payload size
			SX1276LR->RegPayloadLength = TxPacketSize;
			SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
			
			SX1276LR->RegFifoTxBaseAddr = 0x00; // Full buffer used for Tx
			SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR->RegFifoTxBaseAddr );

			SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoTxBaseAddr;
			SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );

			SX1276Read( REG_LR_PAYLOADLENGTH, &SX1276LR->RegPayloadLength );
			
			// Write payload buffer to LORA modem
			SX1276WriteFifo( RFBuffer, SX1276LR->RegPayloadLength );
											// TxDone               RxTimeout                   FhssChangeChannel          ValidHeader         
			SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01;
											// PllLock              Mode Ready
			SX1276Write( REG_LR_DIOMAPPING1, SX1276LR->RegDioMapping1 );

			SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );
			PacketTxTimeout = LoRaSettings.TxPacketTimeout;
			RFLRState = RFLR_STATE_TX_RUNNING;
			result = RF_TX_INIT;
			break;
		case RFLR_STATE_TX_RUNNING:
			result = RF_BUSY;			
			if(D0Pin_Control()) // TxDone
			{
				// Clear Irq
				SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE  );
				RFLRState = RFLR_STATE_TX_DONE;  
                result = RF_TX_RUNNING;				
        	}
			if( !PacketTxTimeout )
			{
					RFLRState = RFLR_STATE_TX_TIMEOUT;
					result    = RF_TX_TIMEOUT;
			}						
			break;
		case RFLR_STATE_TX_DONE:
			// optimize the power consumption by switching off the transmitter as soon as the packet has been sent
			SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );

			RFLRState = RFLR_STATE_IDLE;// RFLR_STATE_RX_INIT ;//RFLR_STATE_IDLE; 上电时，需要保持持续接收状态。
			result = RF_TX_DONE;
			break;

		case RFLR_STATE_TX_TIMEOUT:
			RFLRState = RFLR_STATE_IDLE;
			result = RF_TX_TIMEOUT;
		  break;
				
		case RFLR_STATE_CAD_INIT:
			SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );				
			SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |
										RFLR_IRQFLAGS_RXDONE |
										RFLR_IRQFLAGS_PAYLOADCRCERROR |
										RFLR_IRQFLAGS_VALIDHEADER |
										RFLR_IRQFLAGS_TXDONE |
										//RFLR_IRQFLAGS_CADDONE |
										RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL; // |
										//RFLR_IRQFLAGS_CADDETECTED;
			SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );

//										//       RxDone                   RxTimeout              FhssChangeChannel               CadDone
//			SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_00;
										//      CadDone                   CAD Detected		     FhssChangeChannel              PayloadCrcError
			SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_10 | RFLR_DIOMAPPING1_DIO1_10 | RFLR_DIOMAPPING1_DIO2_10 | RFLR_DIOMAPPING1_DIO3_10;
			SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 1 );

			SX1276LoRaSetOpMode( RFLR_OPMODE_CAD );
			CADTimeoutTimer = 1;  //UNIT microSECOND 2000MS  100/6000=1/60 
			RFLRState = RFLR_STATE_CAD_RUNNING;
			result = RF_CHANNEL_CAD_RUNNING;
			break;
			
		case RFLR_STATE_CAD_RUNNING:
			result = RF_BUSY ;
			if(D0Pin_Control())// CAD Done interrupt
			{
				if(D1Pin_Control())// CAD Detected interrupt
				{
					// Clear CAD done Irq CAD detected Irq
					SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDONE | RFLR_IRQFLAGS_CADDETECTED);
					RFLRState = RFLR_STATE_RX_INIT;
					result = RF_CHANNEL_ACTIVITY_DETECTED;	
					break ;
				}
				else
				{
					// Clear CAD done Irq
					SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDONE );
					RFLRState = RFLR_STATE_IDLE;//RFLR_STATE_CAD_RUNNING;//RFLR_STATE_IDLE;
					result = RF_CHANNEL_EMPTY;				
				}
			}
			if(!CADTimeoutTimer)
			{
				RFLRState = RFLR_STATE_IDLE;//RFLR_STATE_IDLE;
				result = RF_IDLE;
			}						
			break;
		default:
				break;
	} 
    return result;
}
