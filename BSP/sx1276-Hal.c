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
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */

#include "sx1276-Hal.h"
#include "Bsp_SX127X.h"
#include "sysTick.h"
#include "bsp_spi.h"
#include <string.h>


void DelayUS(INT32U time)
{	
//	volatile INT32U i ;
//	for(i= 0 ; i<time ;i ++);
	time/=1000;
	if(!time)
	{
		time =1 ;
	}
	DelayMs(time);
}

uint8_t RFSendData[0x70],RFRcvData[0x70];
void SX1276InitIo( void )
{   
	
}

void SX1276SetReset(void)
{
   RST_RF_Pin_Control(FALSE);                                      //SX1278复位
   DelayUS(200);
   RST_RF_Pin_Control(TRUE);                                     	//SX1278复位 需要5ms后操作SPI
   DelayUS(5000);
}

void SX1276Write( unsigned char addr, unsigned char setdata )
{
    SX1276WriteBuffer( addr, &setdata, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *readdata )
{
    SX1276ReadBuffer( addr, readdata, 1 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    SX127XCS();

    SPI2_SendRcvByte(addr | 0x80);
    for( i = 0; i < size; i++ )
    {
        SPI2_SendRcvByte(buffer[i]);
    }

	SX127XDisCS();
	
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    SX127XCS();

    SPI2_SendRcvByte(addr & 0x7F);

    for( i = 0; i < size; i++ )
    {
        buffer[i] = SPI2_SendRcvByte(0);
    }

	SX127XDisCS();
	
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
	SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
	SX1276ReadBuffer( 0, buffer, size );
}

//uint8_t SX1276ReadDio0( void )
//{
//    return (P0&0X01);
//}

//uint8_t SX1276ReadDio1( void )
//{
//    return (P0&0X02);
//}

//uint8_t SX1276ReadDio2( void )
//{
//    return (P2&0X04);
//}

//uint8_t SX1276ReadDio3( void )
//{
//    return (P1&0X04);
//}

//uint8_t SX1276ReadDio4( void )
//{
//    return (P1&0X08);
//}

//uint8_t SX1276ReadDio5( void )
//{
//    return (P1&0X02);
//}

void SX1276WriteRxTx( uint8_t txEnable )
{
	if( txEnable != 0 )
	{

	}
	else
	{

	} 
}
