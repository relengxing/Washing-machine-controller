/*****************************************Copyright(C)******************************************
*------------------------------------------文件信息---------------------------------------------
* FileName          : App_Head.h
* Author            : fjb
* Date First Issued : 2015/8/03
* Version           : V1.0
* Description       : 此文件专为编写规约表格而定，与规约无关的东西请不要在此定义,参数存取在AT45DB里面
*----------------------------------------历史版本信息-------------------------------------------
* History           :
* //2008            : V
* Description       : 
*-----------------------------------------------------------------------------------------------
***********************************************************************************************/
#ifndef __App_Head__H__
#define __App_Head__H__
/* Includes-----------------------------------------------------------------------------------*/

#include	"Bsp_config.h " 

/* Private define-----------------------------------------------------------------------------*/


#define POINTAMOUNT	 	96
#define TASKMAXAMOUNT	1

// 各类数据的保存点数
#define	ASSI_CURVE_SAVE_DAYS			31uL		// 曲线数据保存天数
//#define	ASSI_DAY_HOURS      		24uL		// 曲线数据保存天数
#define	ASSI_CURVE_DAY_POINT			1uL			// 一天保存的点数


#define APP_AGR_START_CHAR		0x66
#define APP_AGR_END_CHAR		0x68
#define APP_TER_ADDR_LEN		8

/*extern agr 规约命令 */
#define MY_ORD_WRITE_BROAD_ADDR	0x06  	/*设置地址 */
#define MY_ORD_READ_BROAD_ADDR	0x08  	/*广播查询地址*/

#define MY_ORD_WRITE_METER_PARA 0x09  	/*设置表地址 */

#define AFN_W_TIME				0x10    /*设置时间*/
#define AFN_R_TIME				0x11    /*读取时间*/

#define AFN_W_REP_TIME			0x12    /*设置上报时间*/
#define AFN_R_REP_TIME			0x13    /*读取上报时间*/

#define AFN_W_REMOTE_IP			0x14    /*设置服务器地址*/
#define AFN_R_REMOTE_IP			0x15    

#define AFN_W_HEART_TIME		0x16    /*设置心跳间隔*/
#define AFN_R_HEART_TIME		0x17    


#define AFN_W_METER_PARA		0x2E    /*设置表参数*/
#define AFN_R_METER_PARA		0x1E      

#define AFN_REP_METER_DATA		0x9A    /*上传表数据*/

#define AFN_W_TIME				0x10    /*设置时间*/
#define AFN_R_TIME				0x11    /*读取时间*/

#define APP_TASK_REP_OPEN		0x1
#define APP_TASK_REP_CLOSE		0x2

#define APP_TASK_CYCLE_MONTH	0xA1
#define APP_TASK_CYCLE_WEEK		0xA2
#define APP_TASK_CYCLE_DAY		0xA3
#define APP_TASK_CYCLE_HOUR		0xA4

#define APP_FREAM_MAXPOINTAMOUNT	20ul

//local agr
#define AFN_ACK_OK              0x02    /*确认 */
#define AFN_RESET				0x06	/*复位命令*/
#define AFN_SETPARA             0x04    /*设置参数 */
#define AFN_CONGTRAL            0x05    /*控制命令  */
#define AFN_ID                  0x07    /*控制命令  */
#define AFN_CONFIG              0x09    /*请求配置及信息*/
#define AFN_INQUIREPARA         0x0A    /*查询参数 */
#define AFN_INQUIRETASK         0x0B    /*请求任务 */
#define AFN_INQUIREPARA_ONE     0x0C    /*请求一类数据 */
#define AFN_INQUIREPARA_TWO     0x0D    /*请求二类数据 */
#define AFN_INQUIREPARA_THREE   0x0E    /*请求三类数据 */
#define AFN_TRANS_FILE          0x0F    /*文件传输 */
#define AFN_TRANS_DATA          0x10    /*数据转发 */

/*自定义命令 */

#define DATA_RESET          0x0602ul    /*数据复位命令*/

#define ININ_HARD           0x0601ul    /*硬件复位命令 */
#define DATA_RESET          0x0602ul    /*数据复位命令*/
#define PARA_RESET_SELF     0x0604ul    /*出厂设置*/
#define ALL_PARA_RESET      0x0603ul    /*参数复位命令*/

#define REST_PARA_ORDER     0xBB    /*参数初始化命令 */
#define METER_WRITE         0xA1    /*数据写入规约位置命令 */
#define METER_READ          0xA2    /*读取规约集合项中的命令 */


#define AFN_REST_F3         0xcc  


#define	APP_METER_AGR_DEFINE_WATER_ELE			0		//电子式水表 有线
#define	APP_METER_AGR_DEFINE_WATER_PHO_ELE	  	1		//光电式水表 有线
#define APP_METER_AGR_DEFINE_HOT				0x22	//超声波热表 
#define APP_METER_AGR_DEFINE_HOT_HUIZHONG		0x25	//超声波热表 汇中
#define APP_METER_AGR_DEFINE_HOT_RUINA			0x26	//超声波热表 瑞纳
#define APP_METER_AGR_DEFINE_HOT_MAITUO			0x27	//超声波热表 迈拓
#define APP_METER_AGR_DEFINE_HOT_TIANGANG		0x28	//超声波热表 天罡
#define APP_METER_AGR_DEFINE_HOT_LICHUANG		0x29	//超声波热表 力创
#define APP_METER_AGR_DEFINE_HOT_WEIAN			0x2A	//超声波热表 伟岸 
#define APP_METER_AGR_DEFINE_HOT_JINSHUI		0x2B	//超声波热表 金水
#define	APP_METER_AGR_DEFINE_WATER_RF	  		0x0f	//能联RF模块水表
#define	APP_METER_AGR_DEFINE_TEMP_RF	  		0x50	//能联RF模块温度面板


#define APP_METER_TEMP_RF_MODE_1_LEN	57 		//能联RF模块温度面板 数据内容长度
#define APP_METER_WATER_LEN				13	//
#define APP_METER_HOT_LEN				48  //43+5
#define APP_METER_TEMP_RF_LEN			62  //57+5


//规约协议定义

#define AGR_METER_CT188_2004	1
#define AGR_METER_DLT645_2007	2
#define AGR_METER_LYRB_2013		3
#define AGR_METER_DLT645_1997	4
#define AGR_METER_WECAN_2015	5

//抄表端口
#define APP_METER_PORT_MBUS_1	1
#define APP_METER_PORT_MBUS_2	2
#define APP_METER_PORT_MBUS_3	3
#define APP_METER_PORT_WIRE		4
#define APP_METER_PORT_485_1	5
#define APP_METER_PORT_485_2	6

#define APP_RS232_COMM	1
#define APP_GPRS_COMM	3

#define APP_485_COMM	1


/* Private enum----------------------------------------------------------------------------*/

/* Private typedef----------------------------------------------------------------------------*/
typedef struct AgreementTableStruct_
{
    INT16U afn;      /*功能码 */
    INT16U id;       /*标识码 */
    INT16U point;    /*测量点 */
    INT16U len;      /*数据长度 */
	const INT8U  *pSource; /*主要是参数查询，需要传递的一些数据*/
    INT8U  *ptmp;    /*数据缓冲区 */

    
}AgreementTableStruct;
/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

#endif  //__App_Head__H__  

