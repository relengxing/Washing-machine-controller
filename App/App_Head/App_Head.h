/*****************************************Copyright(C)******************************************
*------------------------------------------�ļ���Ϣ---------------------------------------------
* FileName          : App_Head.h
* Author            : fjb
* Date First Issued : 2015/8/03
* Version           : V1.0
* Description       : ���ļ�רΪ��д��Լ�����������Լ�޹صĶ����벻Ҫ�ڴ˶���,������ȡ��AT45DB����
*----------------------------------------��ʷ�汾��Ϣ-------------------------------------------
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

// �������ݵı������
#define	ASSI_CURVE_SAVE_DAYS			31uL		// �������ݱ�������
//#define	ASSI_DAY_HOURS      		24uL		// �������ݱ�������
#define	ASSI_CURVE_DAY_POINT			1uL			// һ�챣��ĵ���


#define APP_AGR_START_CHAR		0x66
#define APP_AGR_END_CHAR		0x68
#define APP_TER_ADDR_LEN		8

/*extern agr ��Լ���� */
#define MY_ORD_WRITE_BROAD_ADDR	0x06  	/*���õ�ַ */
#define MY_ORD_READ_BROAD_ADDR	0x08  	/*�㲥��ѯ��ַ*/

#define MY_ORD_WRITE_METER_PARA 0x09  	/*���ñ��ַ */

#define AFN_W_TIME				0x10    /*����ʱ��*/
#define AFN_R_TIME				0x11    /*��ȡʱ��*/

#define AFN_W_REP_TIME			0x12    /*�����ϱ�ʱ��*/
#define AFN_R_REP_TIME			0x13    /*��ȡ�ϱ�ʱ��*/

#define AFN_W_REMOTE_IP			0x14    /*���÷�������ַ*/
#define AFN_R_REMOTE_IP			0x15    

#define AFN_W_HEART_TIME		0x16    /*�����������*/
#define AFN_R_HEART_TIME		0x17    


#define AFN_W_METER_PARA		0x2E    /*���ñ����*/
#define AFN_R_METER_PARA		0x1E      

#define AFN_REP_METER_DATA		0x9A    /*�ϴ�������*/

#define AFN_W_TIME				0x10    /*����ʱ��*/
#define AFN_R_TIME				0x11    /*��ȡʱ��*/

#define APP_TASK_REP_OPEN		0x1
#define APP_TASK_REP_CLOSE		0x2

#define APP_TASK_CYCLE_MONTH	0xA1
#define APP_TASK_CYCLE_WEEK		0xA2
#define APP_TASK_CYCLE_DAY		0xA3
#define APP_TASK_CYCLE_HOUR		0xA4

#define APP_FREAM_MAXPOINTAMOUNT	20ul

//local agr
#define AFN_ACK_OK              0x02    /*ȷ�� */
#define AFN_RESET				0x06	/*��λ����*/
#define AFN_SETPARA             0x04    /*���ò��� */
#define AFN_CONGTRAL            0x05    /*��������  */
#define AFN_ID                  0x07    /*��������  */
#define AFN_CONFIG              0x09    /*�������ü���Ϣ*/
#define AFN_INQUIREPARA         0x0A    /*��ѯ���� */
#define AFN_INQUIRETASK         0x0B    /*�������� */
#define AFN_INQUIREPARA_ONE     0x0C    /*����һ������ */
#define AFN_INQUIREPARA_TWO     0x0D    /*����������� */
#define AFN_INQUIREPARA_THREE   0x0E    /*������������ */
#define AFN_TRANS_FILE          0x0F    /*�ļ����� */
#define AFN_TRANS_DATA          0x10    /*����ת�� */

/*�Զ������� */

#define DATA_RESET          0x0602ul    /*���ݸ�λ����*/

#define ININ_HARD           0x0601ul    /*Ӳ����λ���� */
#define DATA_RESET          0x0602ul    /*���ݸ�λ����*/
#define PARA_RESET_SELF     0x0604ul    /*��������*/
#define ALL_PARA_RESET      0x0603ul    /*������λ����*/

#define REST_PARA_ORDER     0xBB    /*������ʼ������ */
#define METER_WRITE         0xA1    /*����д���Լλ������ */
#define METER_READ          0xA2    /*��ȡ��Լ�������е����� */


#define AFN_REST_F3         0xcc  


#define	APP_METER_AGR_DEFINE_WATER_ELE			0		//����ʽˮ�� ����
#define	APP_METER_AGR_DEFINE_WATER_PHO_ELE	  	1		//���ʽˮ�� ����
#define APP_METER_AGR_DEFINE_HOT				0x22	//�������ȱ� 
#define APP_METER_AGR_DEFINE_HOT_HUIZHONG		0x25	//�������ȱ� ����
#define APP_METER_AGR_DEFINE_HOT_RUINA			0x26	//�������ȱ� ����
#define APP_METER_AGR_DEFINE_HOT_MAITUO			0x27	//�������ȱ� ����
#define APP_METER_AGR_DEFINE_HOT_TIANGANG		0x28	//�������ȱ� ���
#define APP_METER_AGR_DEFINE_HOT_LICHUANG		0x29	//�������ȱ� ����
#define APP_METER_AGR_DEFINE_HOT_WEIAN			0x2A	//�������ȱ� ΰ�� 
#define APP_METER_AGR_DEFINE_HOT_JINSHUI		0x2B	//�������ȱ� ��ˮ
#define	APP_METER_AGR_DEFINE_WATER_RF	  		0x0f	//����RFģ��ˮ��
#define	APP_METER_AGR_DEFINE_TEMP_RF	  		0x50	//����RFģ���¶����


#define APP_METER_TEMP_RF_MODE_1_LEN	57 		//����RFģ���¶���� �������ݳ���
#define APP_METER_WATER_LEN				13	//
#define APP_METER_HOT_LEN				48  //43+5
#define APP_METER_TEMP_RF_LEN			62  //57+5


//��ԼЭ�鶨��

#define AGR_METER_CT188_2004	1
#define AGR_METER_DLT645_2007	2
#define AGR_METER_LYRB_2013		3
#define AGR_METER_DLT645_1997	4
#define AGR_METER_WECAN_2015	5

//����˿�
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
    INT16U afn;      /*������ */
    INT16U id;       /*��ʶ�� */
    INT16U point;    /*������ */
    INT16U len;      /*���ݳ��� */
	const INT8U  *pSource; /*��Ҫ�ǲ�����ѯ����Ҫ���ݵ�һЩ����*/
    INT8U  *ptmp;    /*���ݻ����� */

    
}AgreementTableStruct;
/* Private macro------------------------------------------------------------------------------*/

/* Private variables--------------------------------------------------------------------------*/

/* Private function prototypes----------------------------------------------------------------*/

/* Private functions--------------------------------------------------------------------------*/

#endif  //__App_Head__H__  

