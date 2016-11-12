#ifndef __App_Gprs_Interface__H__
#define __App_Gprs_Interface__H__

void App_Gprs_Task_TimeCall(void);

//上点后，rst 等会1s 
void App_Gprs_Power_On(void);

void App_Gprs_Connet_ProgramFunc(void);

//GPRS 通讯参数初始化 
void App_GprsCommInit(void);

//void App_Gprs_DelayTime(void);

//void App_Gprs_SendDelay(void);

/**********************************************************************************************
* Description       :GPRS主程序
* Input             : 
* Output            :  
* Contributor       : lichao
* Date First Issued : 2015/09/16
***********************************************************************************************/
void App_Gprs_Main(void);

#endif 



