//ParseCmd.h
//  Command parse module, which is used to communicate with MCU
#ifndef __PARSECMD_H__
#define __PARSECMD_H__ 

#include "common.h"
#define PCK_CHK_CRC16 //�Ƿ����CRC16˫�ֽ�У�飬�������LRC���ֽ�У��

#ifdef PCK_CHK_CRC16
#define PCK_CHK_LEN 2//crc16ռ��2�ֽ�
#else
#define PCK_CHK_LEN 1//lrcռ��2�ֽ�
#endif

#define CTC_STX 0x55
#define CTC_ETX 0xAA
#define ESC 0xA5 //ת���ַ�

#define CTC_STX_T 0xE7 //CTC_STXת���ַ�
#define CTC_ETX_T 0xE8 //CTC_ETXת���ַ�
#define ESC_T 0xE9 //ESCת���ַ�

//ͨ�ô�����
#define RET_OK                          0x00  //�ɹ�
#define RET_UNKNOWN               0x01//ָ�������
#define RET_PARAM                    0x02//��������
#define RET_TIMEOUT                 0x03//������ʱ
#define RET_CANCEL                   0x04//������ȡ��
#define RET_MEM                       0x05//�ڴ治��
#define RET_OPENED                  0x06//�Ѿ����������̴�
#define RET_PERMISSION            0x07//Ȩ�޲���
#define RET_NOT_OPEN              0x08//�豸δ��
#define RET_OCCUPIED               0x09//�豸��ռ�ã�����ʹ����
#define RET_ILLEAGLE_SERVICE  0x00A//�Ƿ�����ID
#define RET_PARAM_LEN             0x0B//�������ȴ���
#define RET_LRC                         0x0C//LRC����
#define RET_TRANSLATE              0x0D//ת���ַ�������
#define RET_NOCTC_STX		           0x0E//û��CTC_STX
#define RET_NOCTC_ETX			     0x0F//û��CTC_ETX
#define RET_UNPACK_LEN		     0x10//�����������
#define RET_CHECK_LEN		     0x11//����У������
#define RET_EMPTY     		     0x12//�����ݴ���
#define RET_NOT_FOUND     	     0x13//δ�ҵ���Ӧ����
#define RET_PACKET_INTEGRITY    0x14//�������Դ���
#define RET_WRITE_DEVICE  	     0x15//д�豸ʧ��
#define RET_READ_DEVICE  	     0x16//���豸ʧ��
#define RET_ICCARD_SELECT         0x17
#define RET_RFCARD_SELECT        0x18
#define RET_ICCARD_RESET          0x19
#define RET_ICCARD_APDU           0x1A
#define RET_RFCARD_POLL           0x1B
#define RET_RFCARD_APDU          0x1C

//״̬��
#define CMDST_OK					0x0000//����ɹ�
#define CMDST_CMD_ER				0x0001//ָ���벻֧��
#define CMDST_PARAM_ER				0x0002//��������
#define CMDST_DATA_LEN_ER			0x0003//�ɱ������򳤶ȴ���
#define CMDST_FRAME_FORMAT_ER	    0x0004//֡��ʽ����
#define CMDST_LRC_ER				0x0005//LRCУ��ʧ��
#define CMDST_OTHER_ER				0x0006//����
#define CMDST_TIMEOUT				0x0007//��ʱ
#define CMDST_PUBKEY_FORMAT_ER      0x0008//��Կ��ʽ����
#define CMDST_PUBKEY_ZERO           0x0009//���ڹ�Կ����Ϊ0���޷���ǩ
#define CMDST_SIGN_FORMAT_ER		0x000A//ǩ�����ݸ�ʽ����
#define CMDST_HASE_ERR  			0x000B//Hashֵ��ƥ��
#define CMDST_SIGN_ER               0x000C//��ǩʧ��
#define CMDST_STORE_FULL			0x000D//�洢������
#define CMDST_IC_SELECT_ER		    0x000E//IC������ѡ�����
#define CMDST_RF_SELECT_ER			0x000F//RF������ѡ�����
#define CMDST_IC_RESET_ER			0x0010//IC����λ����
#define CMDST_IC_APDU_ER			0x0011//IC��APDU����
#define CMDST_RF_POLL_ER			0x0012//RF��Ѱ��ʧ��
#define CMDST_RF_APDU_ER			0x0013//RF��APDU����
#define CMDST_OPEN_DEV_ER			0x0014//���豸����
#define CMDST_CLOSE_DEV_ER		0x0015//�ر��豸����
#define CMDST_IC_EXIST_ER		0x0016// ICCARD����λ
#define CMDST_UNPACK_LEN_ER		0x0017//���������ȳ���
#define CMDST_CANCEL_ER		    0x0018//ȡ���˳�
#define CMDST_RF_TRANS_ER       0x0019//�������
#define CMDST_RF_PROT_ER        0x001A//Э�����
#define CMDST_RF_MULTI_ER       0x001B//�࿨����
#define CMDST_RF_NOACT_ER       0x001C//��δ�ϵ����
#define CMDST_RF_NOAUTH_ER      0x001D//��δ��֤����
#define CMDST_RF_AUTH_ER        0x001E//����֤ʧ�ܴ���
#define CMDST_RF_UNCHANGE_ER    0x001F//�����޸Ĵ���
#define CMDST_RF_KEYNOAUTH_ER   0x0020//��Կ��Ȩ��
#define CMDST_IC_INVALID_ER     0X0021//��Ƭ���ڣ���Ч��
#define CMDST_IC_CODE_ER        0X0022//���������
#define CMDST_IC_CODECOUNT_ER   0X0023//����Ч�����þ�
#define CMDST_IC_LOCK_ER        0X0024//��Ƭ������
#define CMDST_RF_MULCARD_ER			0x0025//RF���࿨����
/* ������*/
/* �ſ�*/
#define CMD_MAGCARD			      0xA3
#define CMD_MAGCARD_OPEN			0x01
#define CMD_MAGCARD_CLOSE			0x02
#define CMD_MAGCARD_READ			0x03
#define CMD_MAGCARD_REPORT	       0x04
#define CMD_MAGCARD_CANCEL	       0x05

/* ��ʽ��ӡ��*/
#define CMD_NEEDLE                           0xE3
#define CMD_NEEDLE_OPEN			0x01
#define CMD_NEEDLE_CLOSE			0x02
#define CMD_NEEDLE_STATUS			0x03
#define CMD_NEEDLE_WRITE			0x04

/* ICCARD */
#define CMD_ICCARD                           0xB3
#define CMD_ICCARD_SLOT_OPEN			0x01
#define CMD_ICCARD_SLOT_CLOSE			0x02
#define CMD_ICCARD_TYPE     			0x03
#define CMD_ICCARD_CARD_OPEN			0x04
#define CMD_ICCARD_CARD_CLOSE			0x05
#define CMD_ICCARD_APDU			0x06
#define CMD_ICCARD_CHECK        0x07
/* RFCARD */
#define CMD_RFCARD                           0xC3
#define CMD_RFCARD_OPEN			 0x01
#define CMD_RFCARD_CLOSE		 0x02
#define CMD_RFCARD_TYPE			 0x03
#define CMD_RFCARD_MODULE_OPEN	 0x04
#define CMD_RFCARD_MODULE_CLOSE	 0x05
#define CMD_RFCARD_POLL_POWER_UP 0x06
#define CMD_RFCARD_APDU			 0x07
#define CMD_RFCARD_MIFARE        0x08
#define CMD_RFCARD_POLL			 0x09
#define CMD_RFCARD_CARD_OPEN	 0x0A
#define CMD_RFCARD_CARD_CLOSE	 0x0B

/* TPRINTER */
#define CMD_TPRINTER                         0xC4
#define CMD_TPRINTER_OPEN			0x01
#define CMD_TPRINTER_CLOSE			0x02
#define CMD_TPRINTER_LINESET		0x03
#define CMD_TPRINTER_DESITY		0x04
#define CMD_TPRINTER_FEEDPAPER	0x05
#define CMD_TPRINTER_BITMAP       	0x06
#define CMD_TPRINTER_STATUS      	0x07
#define CMD_TPRINTER_BLACKMARK      0x08

/* SIGN */
#define CMD_SIGN                       0xB1
#define CMD_SIGN_DOWN_PUBKEY           0x01//��װ��Կ
#define CMD_SIGN_GET_PUBKEY_NUM        0x02//��ȡ��Կ����
#define CMD_SIGN_CLEAR_PUBKEY          0x03//������й�Կ
#define CMD_SIGN_VERIFY                0x04//��ǩ
#define CMD_SIGN_UPDATE_HASH           0x05//����HASH����
#define CMD_SIGN_VERIFY_IMG            0x06//�����Ȩ
#define CMD_SIGN_PUBKEY_DRENCRY        0x07//��Կ����
#define CMD_SIGN_SET_PROJECT_ID        0x08// ������Ŀ�̼�ID
#define CMD_SIGN_GET_PROJECT_ID        0x09// ��ȡ��Ŀ�̼�ID
#define CMD_SIGN_GET_GAPKHASH_REQ      0x0A// ��ȡGAPK HASH����
#define CMD_SIGN_GET_GAPKHASH_NUM      0x0B// ��ȡGAPK ������

/* PINPAD */

#define CMD_PED                         0xD3 // ���������
#define CMD_PED_OPEN                    0x01 // ���������
#define CMD_PED_CLOSE                   0x02 // �ر�������� 
#define CMD_PED_DL_MK                   0x03 // ��������Կ����֤��Կ��������Կ�͹�����Կ����ɢ�������
#define CMD_PED_DL_WK                   0x04 // ���ع�����Կ
#define CMD_PED_GET_RANDOM              0x05 // ��ȡ�����
#define CMD_PED_ONLINE_PIN              0x06 // ��ȡ����PIN
#define CMD_PED_OFFLINE_PIN             0x07 // ��ȡ�ѻ�PIN
#define CMD_PED_ENCRYPT                 0x08 // ���ݼӽ���
#define CMD_PED_GET_MAC                 0x09 // ��ȡMAC
#define CMD_PED_SELE_KEY                0x0A // ѡ��ǰ��Կ
#define CMD_PED_AUTHENT                 0x0B // ����֤�����������֤
#define CMD_PED_HAND                    0x0D // �������������
#define CMD_PED_GET_PINSTR              0x0E // ֪ͨ׼����ȡPIN��Կ����λ����ʾ������̴���
#define CMD_PED_EXTERN                  0x0F // ��չ 
#define CMD_PED_NOTIFY_KEY              0x10 // pin��ֵָ֪ͨ��
#define CMD_PED_DIS_KEYNUM              0x11 // ��λ����ʾAPP��ʾ����*�ĸ���
#define CMD_PED_PINSTR_END              0x12 // PIN�����������
#define CMD_PED_ONLINEPINSTR_RESULT     0x13 // ����PIN�������
#define CMD_PED_OFFLINEPINSTR_RESULT    0x14 // �ѻ�PIN�������

                                             
/* EMV */                                    
#define CMD_EMV                       0xC5   
#define CMD_EMV_OPEN               0x01     
#define CMD_EMV_CLOSE              0x02
#define CMD_EMV_SET_DATA           0x03  //���ù̶�����Դ  
#define CMD_EMV_BEGIN_TRANSACTION  0x04  //��ʼ����
#define CMD_EMV_ONLINE_DATA_PROCESS  0x05  //��������  
#define CMD_EMV_APP_SET_DATA       0x06  //����Ӧ����ع̶�����Դ 
#define CMD_EMV_APP_SELECT         0x07  //Ӧ��ѡ��  
#define CMD_EMV_APP_FINAL_SELECT   0x08  //Ӧ��ѡ��ȷ��
#define CMD_EMV_APP_INIT           0x09  //Ӧ�ó�ʼ��
#define CMD_EMV_APP_READ           0x0A  //Ӧ�����ݶ�ȡ 
#define CMD_EMV_OFFLINE_DATA_AUTH      0x0B  //�ѻ�������֤ 
#define CMD_EMV_TREM_RIS_MANAGE        0x0C  //�ն˷��չ���
#define CMD_EMV_PROCESS_RESTRICT       0x0D  //�������ƹ���
#define CMD_EMV_CARD_HOLDER_VALIDATE   0x0E  //�ֿ�����֤ 
#define CMD_EMA_ACTION_ANALYSIS        0x0F  //�ն���Ϊ�����Ϳ�Ƭ��Ϊ����
#define CMD_EMV_UPDATE_DATA            0x10  //����EMV L2 �ں�ģ���е�����Ԫ 
#define CMD_EMV_GET_KERNEL_DATA        0x11  //��ȡEMV L2 �ں�ģ������Ԫ 
#define CMD_EMV_GET_APP_DATA           0x12  //��ȡӦ������  
#define CMD_EMV_SET_KERN_OPT           0x13  //�����ں�ѡ����� 
#define CMD_EMV_AID_INIT_LIST          0x14  //��ʼ��AID�б�    
#define CMD_EMV_AID_DEL_LIST_AID       0x15  //ɾ��EMV L2 �ں����� AID�� 
#define CMD_EMV_AID_SET_LIST_AID       0x16  //����AID�б�����  
#define CMD_EMV_CAPKEY_SET            0x17  //�����ں�ģ���е���֤���Ĺ�Կ��
#define CMD_EMV_CAPKEY_DEL            0x18  //ɾ�����Ĺ�Կ
#define CMD_EMV_CAPKEY_GET_INFO       0x19  //��ȡ��Կ��Ϣ
#define CMD_EMV_ICCARD_GET_LOG        0x1A    //��ȡIC��������־
#define CMD_EMV_ICCARD_GET_LOAD_LOG   0x1B  //��ȡIC��Ȧ����־
#define CMD_EMV_GET_KERN_VERSION      0x1C  //��ȡEMV�ں˰汾��Ϣ 
#define CMD_EMV_GET_DATA              0x1D  //ȡ����ָ��
#define CMD_EMV_READ_LAST_RECORD      0x1E  //��ȡ���һ����¼   
#define CMD_EMV_GET_CARD_NO_SFI       0x1F  //��ȡ���Ż���ŵ��������ڵ��ļ����ͼ�¼��   
#define CMD_EMV_READ_CARD_NO_DATA     0x20  //��ȡ���Ż���ŵ��������ڵļ�¼����
#define CMD_EMV_EXPAND                0x21  //������չָ��   
#define CMD_EMV_DIS                   0x22  //EMV��ʾ��ָ��  
#define CMD_EMV_DIS_RESPONSE          0x23  // EMV��ʾ����ָ֪ͨ��   

#define CMD_EMV_DELETE_ALL_CAPKEY   0x24          //ɾ��EMV�ڵ����й�Կ
#define CMD_EMV_SET_IC_PARAMETER    0x25          //����EMV��IC������
#define CMD_EMV_CONTINUE_TRANSACTION  0x26         //��������
#define CMD_EMV_CONTINUE_TRANSACTION_RESULT  0x27  //�������׽��
#define CMD_EMV_BEGIN_TRANSACTION_RESULT     0x28  //��ʼ���׽��
#define CMD_EMV_EXTEND_SET_ATTRIBUTE         0x29  // ��չָ�����emv�������
#define CMD_EMV_EXTEND_GET_ATTRIBUTE         0x2A  // ��չָ���ȡemv�������

/* currency detect */
#define CMD_CURRENCYDETE                      0xE5   
#define CMD_CURRENCYDETE_OPEN                 0x01   
#define CMD_CURRENCYDETE_CLOSE                0x02   

/*ϵͳ����*/
#define CMD_SYS_MANAGE         0xA1     //ϵͳ����
#define CMD_SYS_MANAGE_LED     0x01     //LED ����
#define CMD_SYS_MANAGE_BEEP    0x02     //������ ����
#define CMD_SYS_MANAGE_GET_RTC 0x03     // RTCʱ�ӻ�ȡ
#define CMD_SYS_MANAGE_SET_RTC 0x04     // RTCʱ������
#define CMD_SYS_MANAGE_MDF_STATUS       0x05  // ϵͳ�۸�״̬��ѯ
#define CMD_SYS_MANAGE_MDF_UNLOCK       0x06  // ϵͳ�۸Ľ���
#define CMD_SYS_MANAGE_READ_EXFLASH     0x07  // ϵͳ�ⲿflash������
#define CMD_SYS_MANAGE_WRITE_EXFLASH    0x08  // ϵͳ�ⲿflashд����
#define CMD_SYS_MANAGE_DL_INFORM        0x09  // ֪ͨ�����ļ�
#define CMD_SYS_MANAGE_DL_DATA          0x0A  // ��������
#define CMD_SYS_MANAGE_DL_FINISH        0x0B  // �������
#define CMD_SYS_MANAGE_INFO_MANAGE      0x0C  // ֪ͨ��ȫCPU�������ģʽ
#define CMD_SYS_MANAGE_GET_CUPSTATUS    0x0D  // ��ȡ��ȫCPU״̬ 
#define CMD_SYS_MANAGE_SET_CUPSTATUS    0x0E  // ���ð�ȫCPU״̬
#define CMD_SYS_MANAGE_LOWPOWER    0x0F  // ϵͳ����͹���
#define CMD_SYS_MANAGE_STATE_CHANGE    0x10  // ϵͳ״̬�ı�

/*��������*/
#define CMD_FAC             0xA2
#define CMD_FAC_ATTACK      0x01   // 

#define CMD_FAC_TEST_UART      0x02 
#define CMD_FAC_TEST_GPIO      0x03 
#define CMD_FAC_TEST_CHARGING  0x04 
#define CMD_FAC_TEST_TRUMPET   0x05 

/*�ն���Ϣ*/
#define CMD_TERMINAL_INFO       0xD1   //�ն���Ϣ
#define CMD_TERMINAL_INFO_SOFT  0x01   //CPU����汾    
#define CMD_TERMINAL_INFO_HARD  0x02   //CPUӲ���汾
#define CMD_TERMINAL_INFO_GETANDROID_VER  0x03   //��ȡandroid�м���İ汾
#define CMD_TERMINAL_INFO_PM    0x04   //ANDROID֪ͨ������������ģʽ��ͨ������ָ����������������ƣ������������͹���ģʽ

///*һά��*/
//#define CMD_BARCODE             0xF3
//#define CMD_BARCODE_OPEN        0x01   //��һάɨ��ͷ
//#define CMD_BARCODE_CLOSE       0x02   //�ر�һάɨ��ͷ
//#define CMD_BARCODE_OPERATE     0x03   //ɨ��ͷ����
//
//
///*��ά��*/
//#define CMD_QRCODE              0xA4
//#define CMD_QRCODE_OPEN               0x01  //�򿪶�άɨ��ͷ
//#define CMD_QRCODE_CLOSE              0x02  //�رն�ά��ɨ��ͷ
//#define CMD_QRCODE_OPERATE            0x03  //ɨ��ͷ����

/*һά��*/
#define CMD_SCAN             0xF3
#define CMD_SCAN_OPEN        0x01   //��һάɨ��ͷ
#define CMD_SCAN_CLOSE       0x02   //�ر�һάɨ��ͷ
#define CMD_SCAN_OPERATE     0x03   //ɨ��ͷ����
#define CMD_SCAN_RESULT_REPORT  0x04   //ɨ��������
#define CMD_SCAN_CANCEL         0x05   //ɨ��ȡ��

/*������չ*/
#define CMD_UART_EXP       0xB4
#define CMD_UART_EXP_OPEN        0x01 
#define CMD_UART_EXP_CLOSE       0x02
#define CMD_UART_EXP_READ        0x03
#define CMD_UART_EXP_WRITE       0x04

/*IDCARD_CARD*/
#define CMD_IDCARD               0xD5 //���֤
#define CMD_IDCARD_OPEN          0x01 //��
#define CMD_IDCARD_CLOSE         0X02 //�ر�
#define CMD_IDCARD_SEARCH        0x03 //Ѱ��
#define CMD_IDCARD_READ_WORD_PHOTO 0x04 //����
#define CMD_IDCARD_ACTIVE        0x05 //���
#define CMD_IDCARD_APDU_GETTHROUTH  0x06 // ͸��
#define CMD_IDCARD_FINGER_PRINT  0x07 //��ָ��


typedef struct packet_mcu
{
	uint32_t len;
	uint8_t  cmdcls;
	uint8_t  cmdop;
	uint8_t  sno1;
	uint8_t  sno2;
}MCUPCK,*PMCUPCK;

#define PACK_MINSIZE			(2+sizeof(MCUPCK)+PCK_CHK_LEN)//���������С����

#define MAX_CMD_TEMP_SIZE		2048

#define MIN_CMD_RESPONSE		6
#define FIX_LENGTH               		4



//��һ����ͷ
MCUPCK CmdPackReq(uint32_t len,uint8_t  cmdcls,uint8_t  cmdop,uint8_t  sno1,uint8_t  sno2);

//�����ַ���ת���ĳ���
int CmdTranslateLen(const uint8_t *inBuf,int inLen);

//�����ַ���ת��ǰ�ĳ���
int CmdRestoreLen(const uint8_t *inBuf,int inLen);

//ת���ַ�������ĩβ����lrcֵ
int CmdTranslate(const uint8_t *inBuf,uint32_t inLen,uint8_t *outBuf,uint32_t *outLen);

//ת������ַ�����ԭ,�д�LRCУ��
int CmdRestore(const uint8_t *inBuf,uint32_t inLen,uint8_t *outBuf,uint32_t *outLen);

//���
int packCmd(MCUPCK req,const uint8_t *inBuf,uint8_t *outBuf,uint32_t *outLen);

//���
int unpackCmd(const uint8_t *inBuf,uint32_t inLen,MCUPCK *res,uint8_t *outBuf,uint32_t *outLen);

//ָ��������У��
int frame_integrity_check(uint8_t *data, uint32_t size, int *s_pos, int *e_pos);

//���Զ����������Ľ������
int unpackCmdEx(uint8_t *data, uint32_t size, MCUPCK *res,uint8_t *outBuf,uint32_t *outLen, uint32_t *restLen);

void dumpByteArray(uint8_t *data, uint32_t len);

#endif
