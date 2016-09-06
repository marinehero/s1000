#ifndef LIB_EMV_H_
#define LIB_EMV_H_

//#include <time.h>
#include "wp30_ctrl.h"
#include "emv.h"
//#include "emv_public_func.h"
// #include "debug.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

extern int lcd_EmvDisp(s_lcd_emv * lcd_emvdis, int timeout);


// ��������ֵ
#define ONLINE_APPROVE    		0x00       	// ��̨��׼����
#define ONLINE_REFER      		0x02       	// ��̨Ҫ��ο�
#define ONLINE_DENIAL     		0x03       	// ��̨�ܾ�����
#define ONLINE_TIMEOUT	  		0x04		// ���Ӻ�̨��ʱ
#define ONLINE_OTHER_ERROR  	0x05		// �Ӻ�̨���յ��������ݣ�У����		

//	����ֵ����	
#define EMV_RET_ERROR_OK  					0		// ��׼���׻��߲����ɹ�
#define EMV_RET_ERROR_PARA				-1		// ��������
#define EMV_RET_ERROR_CHECKSUM			-2		// У��ʹ�
#define EMV_RET_ERROR_DATA				-3		// ���������
#define EMV_RET_ERROR_ICCARD				-4		//��IC ͨ��ʧ��
#define EMV_RET_ERROR_SW					-5		// IC�����ش���״ֵ̬
#define EMV_RET_ERROR_CARD_BLOCKED		-6		// IC��������
#define EMV_RET_ERROR_APP_BLOCKED		-7		// ����Ӧ�ñ�����
#define EMV_RET_ERROR_OVERFLOW			-8		// �ڴ����
#define EMV_RET_ERROR_NO_APP				-9		// �����޸��ļ�
#define EMV_RET_ERROR_NOT_ACCEPTED		-10		// Ӧ�ò�������
#define EMV_RET_ERROR_USER_CANCEL		-11		// ����Աȡ������
#define EMV_RET_ERROR_NO_LOG				-12		// �����޼�¼
#define EMV_RET_ERROR_DATA_MISSING		-13		// ��Ҫ����ȱʧ
#define EMV_RET_ERROR_EXPIRED			-14		// ��Կ���ڻ���Ӧ�ù���
#define EMV_RET_ERROR_BYPASSPIN       		-15     	// ȡ��PIN����
#define EMV_RET_ERROR_FORCE_ACCEPT		-16     	// ������ǿ����׼����
#define EMV_RET_ERROR_PIN_BLOCKED		-17		// pin��֤������
#define EMV_RET_ERROR_DECLINE				-18		// IC���ܾ�����
#define EMV_RET_ERROR_TERMINATE				-19		// ��ֹ����

//	���ýṹ����
typedef struct				// �ն˹�������Ԫ�ṹ
{
	uchar	TermID[8];          		// Terminal identifier
	uchar	MerchantID[15];     	// Merchant identifier
	uchar	MerchCatCode[2];    	// Merchant category code
	uchar	RFU;				// RFU
	ushort	MCHLOCLen;          	// the length of Merchant Name and Location
	uchar  	MCHNameLoc[256];    	// Merchant Name and Location    
	uchar  	TermCntrCode[2];    	// Terminal country code
	uchar  	TransCurCode[2];    	// Transaction currency code
	uchar  	ReferCurCode[2];    	// Transaction Reference Currency Code
	uchar  	TransCurExp;        	// Transation Currency Exponent
	uchar  	ReferCurExp;        	// Transation Reference Currency Exponent
	ushort 	ReferCurrCon;       	// Transaction Reference Currency Conversion
	uchar	tRFU[2];
}EMV_TERM_FIX_DATA;

int emv_set_terminal_data(EMV_TERM_FIX_DATA *pData);
int emv_get_terminal_data(EMV_TERM_FIX_DATA *pData);

typedef struct
{
	uint		TargetPer;           	// Ŀ��ٷֱ���
	uint		MaxTargetPer;        	// ���Ŀ��ٷֱ���
	uint		FloorLimit;          		// ����޶�
	uint		Threshold;           	// ��ֵ
	uint        ECTransLimit;           // �ն˵����ֽ����޶�
	uint        QPBOCTransLimit;        // �ǽӴ������޶�
	uint        QPBOCFloorLimit;        // �ǽӴ��ѻ�����޶�
	uint        QPBOCCVMLimit;          // ִ��CVM�޶�
	uchar	TACDenial[5];        	// �ն���Ϊ����(�ܾ�)
	uchar	TACOnline[5];        	// �ն���Ϊ����(����)
	uchar	TACDefault[5];       	// �ն���Ϊ����(ȱʡ)
	uchar	AcquierId[6];        	// �յ��б�־
	uchar	LenOfdDOL;           	// �ն�ȱʡDDOL����
	uchar	dDOL[252];           	// �ն�ȱʡDDOL
	uchar	LenOftDOL;           	// �ն�ȱʡTDOL����
	uchar	tDOL[252];           	// �ն�ȱʡTDOL
	uchar	Version[2];          		// Ӧ�ð汾
	uchar	RiskManDataLen;      	// ���չ������ݳ���
	uchar	RiskManData[8];      	// ���չ�������
	uchar	tRFU[2];
}EMV_APP_INIT_DATA;

int emv_set_app_init_data(EMV_APP_INIT_DATA *pData);
int emv_get_app_init_data(EMV_APP_INIT_DATA *pData);

// MCK ���ýṹ 
typedef struct
{ // ��ICS�г��ֵ��Ⱥ�˳������
	uchar   cTmType;          	// �ն�����     (Ĭ��Ϊ0x22)
	uchar   tTmCap[3];       	// �ն�����     (Ĭ��Ϊ 0xE0,0xF8,0xC8)
	uchar   tTmCapAd[5];     	// �����ն����� (Ĭ��Ϊ 0x6F,0x00,0xF0,0xF0,0x01)	
	uchar   cSupPSESelect;    	// PSEѡ��ʽ  (Ĭ��Ϊ ֧��)  

	uchar   cSupAppConfirm;   	// �Ƿ�֧�ֳֿ���ȷ�ϼ�ѡ��Ӧ�� (Ĭ��Ϊ ֧��)
	uchar   cSupAppDispOrder; 	// ֧�ְ�ָ��˳����ʾ��ѡӦ��   (Ĭ��Ϊ ֧��)
	uchar   cSupMulLang;      	// ֧�ֶ�����    (Ĭ��Ϊ ��֧��)
	uchar   cSupIssCertCheck ;	// IssCert ��� (Ĭ��Ϊ ֧��) 
	uchar   cSupDDOLFlg ;      	// ֧��Ĭ��DDOL (Ĭ��Ϊ ֧��)      

	uchar   cSupBypassPIN;     	// Bypass PIN, ���������� (Ĭ��Ϊ ֧��)    
	uchar   cSupFlrLmtCheck ;  	// ����޶��� (Ĭ��Ϊ ֧��)   
	uchar   cSupRndOnlineSlt;  	// �������ѡ�� (Ĭ��Ϊ ֧��)
	uchar   cSupVeloCheck;     	// Ƶ�ȼ��     (Ĭ��Ϊ ֧��) 
	uchar   cSupTransLog;      	// ������ˮ     (Ĭ��Ϊ ֧��)

	uchar   cSupExcptCheck ;   	// �쳣�ļ���� (Ĭ��Ϊ ֧��)
	uchar   cTmRmCheckFlg ;    	// �ն˷��չ����ִ���Ƿ���ڿ�ƬAIP  (Ĭ��Ϊ ��)               
	uchar   cSupTmActionCode;  	// �Ƿ�֧���ն���Ϊ���� (Ĭ��Ϊ ֧��)
	uchar   cSupForceOnline;    // ����ǿ���������� (Ĭ��Ϊ ֧��)
	uchar   cSupForceAccept;    // ����ǿ�ƽ������� (Ĭ��Ϊ ֧��)

	uchar   cSupAdvice;         // �ն��Ƿ�֧��֪ͨ (Ĭ��Ϊ ֧��)
	uchar   cSupIssVoiceRef;    // ֧�ַ����з���Ĳο� (Ĭ��Ϊ ֧��)
	uchar   cSupCardVoiceRef;   // ֧�ֿ�Ƭ����Ĳο� (Ĭ��Ϊ ֧��)
	uchar   cSupBatchCollect;   // ֧�������ݲɼ�  (Ĭ��Ϊ ֧��)
	uchar   cSupOnlineCollect;  // ֧���������ݲɼ�(Ĭ��Ϊ ��֧��)             

	uchar   cSupTDOLFlg ;       // ֧��Ĭ��TDOL     (Ĭ��Ϊ ֧��)                
	uchar   cPOSEntryMode;  	// POS��Entry Code  (Ĭ��Ϊ0x25)
	uchar   tHashVal[20];       // ������ö�Ӧ��hashֵ	
}TermMCK;  // 1-> ֧��

int emv_set_mck(TermMCK *pMCK);	// �����ն�MCK
int emv_get_mck(TermMCK *pMCK);	// ��ȡ�ն˵�ǰ��MCK

// typedef struct
// {
// 	uchar   cLen;            // AID���Ƶĳ���
// 	uchar   tAID[16];	     // AID����
// 	uchar   SelFlag;         // ѡ���־(1 ����ƥ��  0 ȫƥ��)	
// 	uchar   tRFU[2];
// }DAidList;

void  emv_init_aidlist(void);								// ��ʼ��AID�б�ɾ��ԭ�е�����AID��Ϣ
int emv_set_aidlist(int Num, DAidList *pAidList); 	// ����AID�б�һ�����������е�AID��Ϣ
int emv_add_aid_item(uchar AidLen, uchar* pAid, uchar SelFlag); // ����һ��AID��Ϣ
int emv_get_aid_num(void);							// ���ص�ǰ�����AID��Ŀ
int emv_del_aid_item(uchar AidLen, uchar* pAid);		// ɾ��һ��AID
int emv_get_aid_by_index(int index, DAidList *pAidList); // ��ȡ��index��AID��Ϣ
int emv_check_aid(uchar AidLen, uchar* pAid);			// ����AID�Ƿ����

// typedef  struct
// { 
// 	uchar   cRid[5];  		// RID
// 	uchar   cIndex;   		// ��Կ����	
// 	uchar   cModLen;       	// ��Կģ�ĳ���
// 	uchar   cMod[248]; 		// ��Կģ	
// 	uchar   cExpLen;        // ��Կָ���ĳ���
// 	uchar   cExp[3];        // ��Կָ��
// 	uchar   cExpDate[4];    // ��ԿʧЧ��, YYYYMMDD
// 	uchar   cHashFlg;       // ��ԿУ����Ƿ����
// 	uchar   cHash[20];      // ��ԿУ��� - ���ڶ��ڼ��, �ɲ���ֵ
// }PKFILESTRU;

void emv_init_capklist(void);								// ��ʼ��CAPK�б�ɾ��ԭ�е�����CAPK
int emv_del_capk(uchar *pRid, uchar cIndex);			// ɾ��һ��CAPK
int emv_add_capk_item(PKFILESTRU  *pCAPKStru);		// ����һ��CAPK
int emv_change_capk_expdate(uchar *pRid, uchar cIndex, uchar *pExpDate); // ����ָ��CAPK�Ĺ�������
int emv_get_capk_num(void);							// ��ȡCAPK����Ŀ
int emv_get_capk_item(uchar *pRid, uchar cIndex, PKFILESTRU *cpkeystru);	// ����RID��Index��ȡCAPK
int emv_get_capk_by_index(uchar cNo, PKFILESTRU *pCAPKeyInfo);			// ��ȡ��cNo��CAPK

typedef struct
{
    uchar PANLen;
    uchar Pan[10];
    uchar PANSn;	
}ExceptFile;

void emv_init_except_files(void);	// ������еĺ�����
int emv_add_except_file_item(uchar PANLen, uchar *Pan, uchar PANSn);	// ����һ��������
int emv_del_except_file_item(uchar PANLen, uchar *Pan, uchar PANSn);		// ɾ��һ��������
int emv_get_except_file_num(void);	// ��ȡ����������
int emv_check_except_file(uchar PANLen, uchar *Pan, uchar PANSn);	// ����Pan�Ƿ��ں�������
int emv_set_except_file_list(int Num, ExceptFile *pExceptFile);		// һ�����������еĺ�����
int emv_get_except_file_list(int Num, ExceptFile *pExceptFile);		// һ���Ի�ȡ���������

typedef  struct 
{
	uchar   cIndex;   		        // ��Կ����
	uchar   cRid[5];  		        // RID
	uchar   cCertSn[3];             // ֤�����к�
	uchar   tRFU[3];                // RFU,���ṹ�ֽ������Ϊ4��������
}IssCRL;

void emv_init_isscrl(void);		// ������еķ����л���֤���б�
int emv_add_isscrl_item(uchar cIndex, uchar *pRid, uchar *pCertSn);	// ����һ������֤��
int emv_del_isscrl_item(uchar cIndex, uchar *pRid, uchar *pCertSn);	// ɾ��һ������֤��
int emv_get_isscrl_num(void);	// ��ȡ����֤�����
int emv_check_isscrl(uchar cIndex, uchar *pRid, uchar *pCertSn); // ���ù�Կ�Ƿ��ѱ�����
int emv_set_isscrl_list(int Num, IssCRL *pCert);	// һ�����������еĻ���֤��
int emv_get_isscrl_list(int Num, IssCRL *pCert);	// һ���Ի�ȡ��λ���֤��

// ���׼�¼����
typedef  struct
{
	uchar    PanLen;            // Pan�ų���
	uchar    Pan[10];   		// ��Pan��
	uchar    PANSn;             // Pan���к�
	uint     TransAmount;  	    // ���׽��
	uchar    TransTime[6];      // ����ʱ��YYMMDDHHMMSS
	uchar    tRFU[2];			// Ԥ��
}TransLog;

void emv_init_translogs(void);
int emv_add_translog_item(TransLog *pLog);
int emv_get_translogs(int LogNum, TransLog *pLog);
int emv_get_translog_item(TransLog *pLog);
int emv_get_translogs_num(void);
int emv_get_transerr_log(char *pData, int *pnDataLen);

void emv_init_all_data_elements(void);		// ��ʼ�����е�����Ԫ�������ն�����Ԫ��IC������Ԫ
void emv_init_icc_data_elements(void);	// ��ʼ�����е�IC������Ԫ
int emv_set_data_element(ushort Tag, int pLen, uchar *pVal);	// ����һ������Ԫ
int emv_get_data_element(ushort Tag, int *pLen, uchar *pVal);	// ��ȡһ������Ԫ
int  emv_check_data_element(ushort Tag);	// ��������Ԫ�Ƿ�������
int emv_del_data_element(ushort Tag);	// ɾ��һ������Ԫ

typedef struct
{
	uchar   tCandAppName[33];
	uchar   cFlgAPID;
	uchar   cAPID;
	uchar   tRFU;
}EMV_CANDAPP_LIST;

/* ����AID�б�Ľṹ */
typedef struct
{
	uchar   tAID[16];               // AID
	uchar   cLenAID;
	uchar   tAppLabel[16];          // Ӧ�ñ�ǩ
	uchar   cLenLabel;
	uchar   tAPN[16];  	            // Ӧ����ѡ��
	uchar   cLenAPN;
	uchar   cAPID;                  // Ӧ������Ȩ��ʶ��
	uchar   cFlgAPID;               // Ӧ������Ȩ��ʶ����־
	uchar   sLangPref[8];           // ��ѡ����
	uchar   cLenLangPref;
	uchar   cIssCTIndex;            // �����д��������
	uchar   cFlgIssCTIndex;
	uchar   cFlgPreferNameFlg;      // �Ƿ�ʹ��Ӧ����ѡ��
	uchar   cLenPDOL;
	uchar   tPDOL[128];             // ԭ����256
}DCandList_ST;

void emv_Get_All_CandAppInfo(uint *CandAppNum, EMV_CANDAPP_LIST *pCandApp);
int emv_Get_CandApp(int cIndex, DCandList_ST *pCandList);

// EMV���׽ӿ�
int emv_cardslot_init(long CardSlot);
void emv_set_pse_type(int nFlag);
int emv_get_pse_type(void);
int emv_application_select(uint TransNo);
int emv_read_app_data(void);
int emv_offline_data_auth(void);
int emv_cardholder_verify(void);
void emv_process_restriction(void);
int emv_term_risk_manage(void);
int emv_complete_transaction(void);

int emv_application_select_read_log(uint TransNo);
int ReadLogRecord(int RecordNo);
int GetLogItem(uchar *Tag, uchar TagLen, uchar *Value, int *ValueLen);
int ReadLoadLogRecord(int RecordNo);
int GetLoadLogItem(char *Tag, uchar TagLen, uchar *Value, int *ValueLen);
int GetLoadLogHead(uchar *Value);
int ReadLoadLogAll(uchar *pLoadLog, int *pLogLen);

int emv_check_force_accept(void);
int emv_get_script_result(uchar *Result, int *RetLen);
int emv_check_print_signature(void);
int emv_del_script_result(void);
int emv_preonline_transaction(uchar *pucFlag);
int emv_onlineback_transaction(uchar ucProcFlag, int nOnlineResult, uchar *pRspCode,uchar *pAuthCode, int *pAuthCodeLen,
	uchar  *pIAuthData, int *pIAuthDataLen, uchar *pScript1, int *pScriptLen1, uchar *pScript2, int *pScriptLen2);

//Ϊ�˴�����������
int emv_read_last_record(void);
int emv_Init_App_Process(void);
int emv_read_app_data_QA(void);
int emv_get_cardno_sfi(int *pnSFI, int *pnRecord);
int emv_read_cardno_data(int nSFI, int nRecord, char *pCard, int *pCardLen);
int emv_check_QA_occur(void);
void emv_clear_QA_flag(void);
void emv_set_QA_flag(void);
int  emv_Get_OdaList_QA(uchar *pDt, int *pLen, int *pNum);
int  emv_Set_OdaList_QA(uchar *pDt, int nLen, int nNum);

//qPBOC���׽ӿ�
int qPBOC_offline_data_auth(void);
int qPBOC_term_risk_manage(void);
int qPBOC_complete_transaction(void);
int qPBOC_trans_preprocess(void);
int qPBOC_check_except_file(uchar PANLen, uchar *Pan);  // ����Pan�Ƿ��ں�������
int qPBOC_preonline_transaction(uchar *pucFlag);
int qPBOC_onlineback_transaction(int nOnlineResult, uchar *pRspCode);

//��Ƭָ��
int emv_icc_get_data(int p1, int p2, uchar *DataOut, int *OutLen);
ushort emv_get_last_iccard_sw(void);


typedef struct EMV_ICAdapterStru
{
	long  (*glICAdapterSelectCard)(long lIndex);
	long  (*glICAdapterCardIn)(long);
	long  (*glICAdapterPowerUp)(long,long *, char *);
	long  (*glICAdapterPowerDown)(long);
	long  (*glICAdapterExchangeAPDU)(long, long , char *, long *, char *);
}EMV_ICAdapterStruDef;
long emv_ICAdapterSet(EMV_ICAdapterStruDef tEMVICAdapter);
#endif

