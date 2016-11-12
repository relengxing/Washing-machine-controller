//#include "App_485_Interface.h"
#ifndef __App_485_interface__H__
#define __App_485_interface__H__


void App_485_Task_TimeCall(void);
void App_ReadMeterMain(void);

void App_485_Set_WorkState(INT8U flag);
INT8U App_485_Get_WorkState(void);

void App_ManipulatorMain(void);

#endif


