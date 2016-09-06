#ifndef EMV_H_
#define EMV_H_

//#include <time.h>
//#include "../inc/libapi.h"

#define  BYTE        unsigned char    // ���ֽ�(Byte)
#define  WORD        unsigned short   // ��(Word)
#define  DWORD       unsigned long    // ˫��(Double Word)
#define  UINT        unsigned int
//#ifndef DevHandle
//typedef unsigned long DevHandle;
//#endif

// ����EMV����ֵ����
#define EMV_RET_OK  					0x00		// �ɹ�
#define EMV_RET_ERROR  					0x01		// ����
#define EMV_RET_PARA_ERROR				0x8B		// �����������
#define EMV_RET_OTHER_ERROR             0x8D        // ��������
#define EMV_RET_TAG_ERROR               0x32        // ��ǩ����
#define EMV_RET_TAGLEN_ERROR            0x35        // �޶�Ӧ�ı�ǩ(��ǩ���ǩ���ȴ���)
#define EMV_RET_TAG_NOEXIST             0x34        // ������Ԫ��δ��ֵ
#define EMV_RET_NO_LIST                 0x40        // EMV L2�ں����޴������б���޴�Ӧ��
#define EMV_RET_OVER_NUM                0x33        // ���������������
#define EMV_RET_FORMAT_ERROR            0x30        // ��ʽ����
#define EMV_RET_CARD_BLOCKED            0x43        // ��Ƭ������֧��ѡ������
#define EMV_RET_SELFLAG_ERROR           0x42        // ��ȡ����ѡ���־����
#define EMV_RET_STATUS_ERROR            0xE7        // ��ȡ��ʽ����ʱ����Ƭ���ص�״̬�����
#define EMV_RET_CARDOUT                 0x8F        // ��Ƭ����λ��ϵͳ����
#define EMV_RET_CANDLIST_ERROR          0x52        // ������ѡ�б�ʱ�����˴���,�Һ��������⴦���Ӧ��
#define EMV_RET_APP_BLOCKED             0x44        // ƥ���Ӧ�þ�������
#define EMV_RET_PSE_ERROR               0x74        // PSE��ʽʧ��, ��Ҫʹ��AID�б����ѡ��
#define EMV_RET_TLV_ERROR               0x48        // ��Ƭ���ݵ�TLV�������
#define EMV_RET_GPO_ERROR               0x49        // ��Ƭ��ȡ����ѡ��Ӧ���У����ص�״̬��!=9000,����!= 6985
#define EMV_RET_SW_6985                 0x46        // ��Ƭ��ȡ����ѡ��Ӧ���GAC����Ӧ���У����ء�6985��״̬�룬��Ӧ�ó��������ֹ���ǻ���
#define EMV_RET_DOL_ERROR               0x47        // ���PDOLʱ����(�翨Ƭ���ص�PDOL�б��ʽ����)
#define EMV_RET_READ_ERROR              0x45        // ������ʱ����
#define EMV_RET_DATA_MISSING            0x4A        // �ر����ѻ���֤����ȱʧ
#define EMV_RET_GETPK_FAIL              0x4B        // ��֤���Ĺ�Կ��ȡʧ��
#define EMV_RET_ISSUER_FAIL             0x4C        // �����й�Կ�ָ�����
#define EMV_RET_SDA_FAIL                0x4D        // ǩ����̬������֤ʧ��
#define EMV_RET_INTER_AUTH_ERROR        0x4E        // �ڲ���֤�������
#define EMV_RET_DDA_DATA_ERROR          0x4F        // ��ö�̬��֤���ݳ���
#define EMV_RET_DDA_FAIL                0x50        // ǩ����̬������֤ʧ��
#define EMV_RET_ONLINE_PIN              0x01        // ��Ҫ����PIN����
#define EMV_RET_OFFLINE_PIN             0x02        // ��Ҫ�ѻ�PIN����
#define EMV_RET_CERTIFICATE             0x05        // ��Ҫ�ֿ���֤����֤
#define EMV_RET_CID_ERROR               0x51        // ��ƬӦ���CID���ݴ�����ֹ����
#define EMV_RET_TRANS_CANCEL            0x53        // �ֿ���ȡ������
#define EMV_RET_TRANS_DECLINE           0x54        // ���׾ܾ�
#define EMV_RET_TRANS_TERMINATE         0x55        // ������ֹ
#define EMV_RET_CHANGE_INTERFACE        0x56        // ����ʧ�ܣ��л���������


//	���ýṹ����
//�ն˹�������Ԫ
typedef struct TM_FIX_DATA
{ 
	BYTE   tTmCntrCode[2];      // �ն˹��Ҵ���
	BYTE   tAquirerID[6];       // �յ��б�ʶ��9F01�� (6�ֽ�)
	BYTE   tMerchCatCode[2];    // �̻������롯9F15��(2�ֽ�) 
	BYTE   tTmTransCur[2];      // �ն˽��׻��Ҵ��롯5F2A��(2�ֽ�) 
	BYTE   cTmTransCurExp;      // �ն˽��׻���ָ����5F36��(1�ֽ�)
	BYTE   tTmRefCurCode[2];    // �ն˽��ײο����Ҵ��롯9F3C��(2�ֽ�)
	BYTE   tTmRefCurExp;	    // �ն˽��ײο�����ָ����9F3D��(1�ֽ�)
	BYTE   tTmRefCurConv[5];    // �ն˽��ײο����Ҷһ�(5�ֽ�) ���ַ�����ʽ��ֵ���硱1000������\x00����
	BYTE   tTmID[8];            // �ն˺�
	BYTE   tMerchantID[15];     // �̻���ʶ��9F16��(15�ֽ�)
	BYTE   MCHLOCLen;           // �̻����Ƽ�λ��������ĳ���
	BYTE   tMCHNAMELOC[256];    // �̻����Ƽ�λ�á�9F4E��(1-256 �ֽ�) 
	BYTE   tTmTransCateCode;    // �ն˽��������� 0x22
}TmFixData;

//Ӧ���������Ԫ
typedef struct INIT_EMV_DATA_STRUCT
{ 
	BYTE   tAppVerNo[2];         // Ӧ�ð汾�š�9F09��(2�ֽ�)
	BYTE   cTmRmCheckFlg;        // ���յ���ȷ���Ƿ�����ն˷��չ�����
	// = 0: ��ִ���ն˷��չ���
	// = 1: ǿ��ִ���ն˷��չ���
	// = 2: �ɿ�Ƭ�����Ƿ�ִ���ն˷��չ���(Ĭ��)
	BYTE   tTAC_Denial[5];       // �ն���Ϊ����TAC���ܾ�(5�ֽ�) 
	BYTE   tTAC_Online[5];       // �ն���Ϊ����TAC������(5�ֽ�)
	BYTE   tTAC_Default[5];      // �ն���Ϊ����TAC��Ĭ��(5�ֽ�)  
	BYTE   cLenTDOL;             // TDOL�ĳ���
	BYTE   tTDOL[252];           // TDOL��97��
	BYTE   cLenDDOL;             // DDOL�ĳ���
	BYTE   tDDOL[252];           // DDOL ��9F49��
	BYTE   tFloorLmt[11];        // ����޶�(11�ֽ�), �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE   tLmt[11];		     // ���ѡ����ֵ, �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE   tPercent[3];          // ���ѡ��Ŀ��ٷ���, ֱ�����ַ�����ʾ��ֵ, �硱50������\x00����
	BYTE   tMaxPercent[3];       // ���ѡ�����Ŀ��ٷ���, ֱ�����ַ�����ʾ��ֵ, �硱99������\x00����
	BYTE   tECTransLimit[13];    // �ն˵����ֽ����޶�, �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE   tQPBOCTransLimit[13]; // �ǽӴ������޶�, �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE   tQPBOCFloorLimit[13]; // �ǽӴ��ѻ�����޶�, �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE   tQPBOCCVMLimit[13];   // ִ��CVM�޶�, �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
}AppInitData;

//��ѡ�б����ݽṹ
typedef struct TERM_CAND_LIST_STRUCT
{    
	BYTE   tAID[16];             // AID����
	BYTE   cLenAID;  		     // AID���Ƶĳ���
	BYTE   tAppLabel[16];        // Ӧ�ñ�ǩ
	BYTE   cLenLabel;		     // Ӧ�ñ�ǩ�ĳ���
	BYTE   tAPN[16];             // Ӧ����ѡ��
	BYTE   cLenAPN;              // Ӧ����ѡ���ĳ���
	BYTE   cAPID;                // Ӧ������Ȩ��ʶ��
	BYTE   cFlgAPID;             // Ӧ������Ȩ��ʶ�����ڱ�־(0-������, 1-����)
	BYTE   sLangPref[8];         // ��ѡ����
	BYTE   cLenLangPref;         // ��ѡ���Եĳ���
	BYTE   cIssCTIndex;          // �����д��������
	BYTE   cFlgIssCTIndex;       // �����д�����������ڱ�־(0-������, 1-����) 
}DCandList;

//����ʹ�õ�����Ԫ
typedef struct  _APP_DATA_STRUCT
{
	BYTE   AmntAuth[13];         // ��Ȩ���ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00���������㲿��ǰ����0��
	BYTE   AmntOther[13];        // �������ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00���������㲿��ǰ����0��
	BYTE   cTransType;           // �������͡�9C��	
	BYTE   tDate[3];             // ��������ָ��(3 BYTE)
	BYTE   tTime[3];             // ����ʱ��ָ��(3 BYTE)
	BYTE   tTrace[4];            // �������м�����(4 BYTE)  
}AppDt;

//�����������, ���ڵ��ݴ�ӡ����ˮ��¼
#define  ISS_SCRPT_RSLT_MAX       12*5    // �����нű��������ֽ���(��12���ű�,ÿ��ռ��5�ֽ�)

typedef struct  TRANS_DATA_STRUCT
{    	// �Ƿ���ڱ�־: 0-������, 1-����
	BYTE   tExpDate[4];          // ʧЧ���� 
	BYTE   cFlgCID;              // ������Ϣ���ݴ��ڱ�־
	BYTE   cCID;                 // ������Ϣ����
	BYTE   cFlgAC;               // Ӧ�����Ĵ��ڱ�־
	BYTE   tAC[8];               // Ӧ������
	BYTE   cFlgTVR;              // �ն���֤������ڱ�־
	BYTE   tTVR[5];              // �ն���֤���
	BYTE   cFlgTSI;              // ����״̬��Ϣ���ڱ�־
	BYTE   tTSI[2];              // ����״̬��Ϣ
	BYTE   cTrack2Len;           // ���ŵ��ȼ����ݳ���
	BYTE   tTrack2[19];          // ���ŵ��ȼ�����
	BYTE   cFlgATC;              // Ӧ�ý��׼��������ڱ�־
	BYTE   tATC[2];              // Ӧ�ý��׼�����
	BYTE   cFlgAIP;              // Ӧ�ý����������ڱ�־
	BYTE   tAIP[2];              // Ӧ�ý�������
	BYTE   cFlgAUC;              // Ӧ����;���ƴ��ڱ�־
	BYTE   tAUC[2];              // Ӧ����;����
	BYTE   cFlgCVMR;             // �ֿ�����֤����������ڱ�־
	BYTE   tCVMR[3];             // �ֿ�����֤�������
	BYTE   cFlgIAC_Denial;       // ��������Ϊ���룭�ܾ����ڱ�־
	BYTE   tIAC_Denial[5];       // ��������Ϊ���룭�ܾ�
	BYTE   cFlgIAC_Online;       // ��������Ϊ���룭�������ڱ�־
	BYTE   tIAC_Online[5];       // ��������Ϊ���룭����
	BYTE   cFlgIAC_Default;      // ��������Ϊ���룭Ĭ�ϴ��ڱ�־
	BYTE   tIAC_Default[5];      // ��������Ϊ���룭Ĭ��
	BYTE   tIssSrptRslt[ISS_SCRPT_RSLT_MAX];    // �����нű���� 
	BYTE   cNumIssSrptRslt;      // �����нű��������
	BYTE   tUnpreNumber[4];      // ����Ԥ֪��
	BYTE   tARC[2];              // ARC
	BYTE   cFlgSign;             // ǩ����־
	BYTE   cLenIssAppDt;         // ������Ӧ�����ݳ��� 
	BYTE   tIssAppDt[32];        // ������Ӧ������ 
}TransDt;

//CA��Կ���ݽṹ
#define  MAX_MODULUS_LEN  248
typedef  struct PUBLIC_KEY_FILE
{ 
	BYTE   cRid[5];  		     // RID
	BYTE   cIndex;   		     // CA��Կ����	
	BYTE   cModLen;       	     // CA��Կģ�ĳ���
	BYTE   cMod[MAX_MODULUS_LEN];// CA��Կģ	
	BYTE   cExpLen;              // CA��Կָ���ĳ���(ֻ��Ϊ1��3)
	BYTE   cExp[3];              // CA��Կָ��(ֻ��Ϊ��0x03����\x01\x00\x01��)
	BYTE   cExpDate[4];          // CA��ԿʧЧ��, ��ʽΪ:YYYYMMDD. 
	BYTE   cHashFlg;             // CA��ԿУ��ʹ��ڱ�־(0-������, 1-����)
	BYTE   cHash[32];            // CA��ԿУ���
}PKFILESTRU;

//�����й�Կ֤������б�ṹ
typedef  struct CERTIFICATE_FILE
{
	BYTE   cIndex;   		     // CA��Կ����
	BYTE   cRid[5];  		     // RID
	BYTE   cCertSn[3];           // ���յķ����й�Կ֤�����к�
	BYTE   tRFU[3];              // ����
}CertFile;

//Ӧ�ñ�ʶ�б�ṹ
typedef struct TERM_AID_LIST_STRUCT
{
	BYTE  cLen;                  // AID���Ƶĳ���
	BYTE  tAID[16];              // AID��������
	BYTE  SelFlag;	             // Ӧ��ѡ��ָʾ��(1 ����ƥ��  0 ȫƥ��)
	BYTE  tRFU[2];               // ����
}DAidList;

//��Ƭ��־��Ϣ�ṹ
typedef struct TERM_CL_LIST_STRUCT
{
	BYTE   tAid[16];             // Ӧ�ñ�ǩ
	BYTE   cLenAID;		         // Ӧ�ñ�ǩ����
	BYTE   cLogFSI;		         // ��Ƭ��־SFI
	BYTE   cLogNum;		         // ��Ƭ��־��¼����
	BYTE   cAppIsLock;	         // ��Ӧ���Ƿ�����(0-δ����; 1-����)
}DClList;

//IC���������Խṹ
typedef struct ICCARD_STATUS
{
	char cTP;	         	     // ����Э���ʶ(0: T=0�ַ�, 1: T=1��֡)
	char cCardType;	             // IC����������
	DevHandle hCardHandle;       // IC��������� 
}DCardSt;

//RSA˽Կ���ݽṹ
typedef struct RSA_PRI_KEY_STRUCT
{
	BYTE 	uModLen;		     //ģ����(�ֽ���)
	BYTE 	*pMod;			     //ģ����
	BYTE 	uPkExpLen;		     //��Կָ������(�ֽ���)
	BYTE 	*pPkExp;			 //��Կָ��
	BYTE 	uSkExpLen; 		     //˽Կָ������(�ֽ���)
	BYTE 	*pSkExp;			 //˽Կָ��
	BYTE 	uPrimeLen1;		     //����1����(�ֽ���)
	BYTE 	*pPrime1;		     //����1����
	BYTE 	uPrimeLen2;		     //����2����(�ֽ���)
	BYTE 	*pPrime2;            //����2����
	BYTE 	uPrimeExpLen1;  	 //����1ָ������(�ֽ���)
	BYTE 	*pPrimeExp1;         //����1ָ������
	BYTE 	uPrimeExpLen2;       //����2ָ������(�ֽ���)
	BYTE 	*pPrimeExp2;	 	 //����2ָ������
	BYTE 	uCoefLen;		     //ϵ������(�ֽ���)
	BYTE 	*pCoef;			     //ϵ������
} PriKeyDt;

//����Ӧ���������ݽṹ
typedef struct TRANS_PROCESS_STRUCT
{
	BYTE    ucProcessFlag; //���̱�־
	// = 0 �����̣������Ӵ�ʽ�ͷǽӴ�����ֻ����Ӧ������Ϊֹ
	// = 1 EMV��������
	// = 2 �Ӵ�ʽ�����ֽ�����
	// = 3 qPBOCС���ѻ�����
	// = 4 �ǽ��������̣����������ѣ�Ԥ��Ȩ������ѯ�ķǽ�q������
	// = 5 �����ֽ�����ѯ�����־���̣�����GPO���
	BYTE    ucCmd;          //�����ʶ 0 �� �������� 1 �� ǿ������
	BYTE    ucCardType;     //��Ƭ���� 0x00 - �Ӵ�ʽIC��  0xFF - �ǽӴ�IC��
}TransProDt;

typedef struct EMV_IC_PARA
{
	BYTE cFlag;		   //Ӧ��ѡ��ָʾ�� DF01
	BYTE AID[16];      //AID 9F06
	BYTE cLenAID;      //AID����
	BYTE tTAC_Denial[5];	// �ն���Ϊ����TAC���ܾ�(5�ֽ�) DF13
	BYTE tTAC_Online[5];	// �ն���Ϊ����TAC������(5�ֽ�) DF12
	BYTE tTAC_Default[5];	// �ն���Ϊ����TAC��Ĭ��(5�ֽ�)  DF11
	BYTE tFloorLmt[11];	    // �ն�����޶� 9F1B �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE tLmt[11];          // ƫ�����ѡ�����ֵ DF15 �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE tPercent[3];      // ���ѡ���Ŀ��ٷ��� DF17 ֱ�����ַ�����ʾ��ֵ, �硱50������\x00����
	BYTE tMaxPercent[3];   // ƫ�����ѡ������Ŀ��ٷ��� DF16 ֱ�����ַ�����ʾ��ֵ, �硱99������\x00����
	BYTE tOnline;          // �ն�����PIN֧������ DF18
	BYTE tDDOL[252];       // ȱʡDDOL DF14
	BYTE cLenDDOL;
	BYTE tAppVerNo[2];     //Ӧ�ð汾�� 9F08
	//С��֧��
	BYTE tECTransLimit[13];// �ն˵����ֽ����޶�  9F7B �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE tQPBOCLimit[13];  // �ǽӴ������޶� DF20 �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE tQPBOCFloorLimit[13];  // �ǽӴ��ѻ�����޶� DF19 �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
	BYTE tCVMLimit[13];    // ִ��CVM�޶� DF21 �ַ�����ʽ��ֵ,�Է�Ϊ��λ, �硱50000������\x00����
}EMVPARA;

typedef struct EMV_IC_PINFLAG
{
	//����
	BYTE cType;          //��Կ����
	BYTE cIndex;         //��Կ����
	BYTE cMode;          //����ģʽ
	BYTE cMinLen;        //��С����
	BYTE cMaxLen;        //��󳤶�
	BYTE cTimeout;       //��ʱʱ��
	BYTE cShowlenflag;   //�������������ʾ��  0-����ʾ 1-��ʾ
	BYTE cBeepflag;      //��������  0-�ް��� 1-��������
	BYTE acCardNo[12];      //���ŵ���12λ
	BYTE cInfoLen;          //��ʾ��Ϣ����
	BYTE acInfo[50];        //��ʾ��Ϣ
	//���
	BYTE cPINInputFlag;     //���ܱ�ʶ 1 - �����ܣ� 2 - bypass��  3 - δ����
	BYTE cEncryptPinLen;    //PIN���ĳ���
	BYTE acEncryptPin[16];  //PIN����
	BYTE cTrack2Len;            //���ŵ����ĳ���
	BYTE acEncryptTrack2[40];  //���ŵ�����
}PINSTATUS;


typedef struct
{
	int (*emv_select_candidate_application)(void);   		// �ֿ���ѡ��Ӧ�ã�����Ӧ�ú�
	int (*emv_account_type_select)(void);				// �ʻ�����ѡ��
	int (*emv_get_transaction_amount)(void);  			// ����Ա���뽻�׽����ؽ��׽��    
	int (*emv_get_cardholder_pin)(PINSTATUS *, uchar *);				//�ֿ����������룬�������볤��
	int (*emv_show_pin_verify_result)(const uchar);		// ��ʾpin��֤���
	int (*emv_process_online_pin)(PINSTATUS *);			// ����pin��֤
	int (*emv_process_pboc_certificate)(void);				// ֤����֤
	int (*emv_process_advice)(void);						// Advice����
	int (*emv_process_online)(uchar *,uchar *, int *, uchar *, int *,		// �������״���
		uchar *, int *);
	int (*emv_process_issuer_refer)(void);				// �������вο�
	int (*emv_set_transaction_data)(void);              // ���ú�AID��صĽ���Ӧ������
	int (*emv_ec_account_select)(void);			    	// �����ֽ�������ʻ�����ѡ��
}EMV_CALLBACK_FUNC;

int emv_set_callback_function(EMV_CALLBACK_FUNC *pFunc);

// �����ӿ�
unsigned char ST_EMV_SetData(unsigned char *pDt, unsigned short wLenDt);
unsigned char ST_EMV_GetData(unsigned char *pDol, unsigned short wLenDol, unsigned char *pVal, unsigned short *pwLenVal);
unsigned char ST_EMV_GetAppData(unsigned char *pTag, unsigned char cTagLen, unsigned char *pVal, unsigned short *pwLen);
void ST_EMV_SetKernKind(unsigned char cKernKind);
unsigned char ST_EMV_GetTransDt(TransDt	*pTransDt);
unsigned char ST_EMV_SetKernOpt(unsigned char cOptItem, unsigned char cFlg);
void ST_EMV_DelallTermAIDList(void);
unsigned char ST_EMV_DelAidListAid(unsigned char *pAid, unsigned char cLenAid);
unsigned char ST_EMV_SetAidListItem(unsigned char *pAid, unsigned char cLenAid, unsigned char cFlag);
unsigned char ST_EMV_DelAllCAPKey(void);
unsigned char ST_EMV_DelCAPKeyByRidIndex(unsigned char *pRid, unsigned char cIndex);
unsigned char ST_EMV_SetCAPKey(PKFILESTRU *pCAPKStru);
unsigned char ST_EMV_GetCAPKeyNum(void);
unsigned char ST_EMV_GetCAPKeyByRidIndex(unsigned char *pRid, unsigned char cIndex, PKFILESTRU *cpkeystru);
unsigned char ST_EMV_GetCAPKeyInfo(unsigned char cNum, PKFILESTRU *pCAPKeyInfo);
unsigned char ST_EMV_GetCAPKeyInfoByNo(unsigned char cNo, PKFILESTRU *pCAPKeyInfo);
unsigned char ST_EMV_SetCertSn(CertFile *pCertFile, unsigned char cNum);
void ST_EMV_DelAllCAPKeyCertSn(void);
unsigned char ST_EMV_DelCAPKeyCertSn(unsigned char *pCertSn, unsigned char *pRid, unsigned char cIndex);
unsigned char ST_EMV_CardLogAppSlt(DCardSt *ptCardSt, unsigned char cSelType, unsigned char *pList, unsigned short *pwLenList);
unsigned char ST_EMV_GetCardLogFormat(unsigned char *tBuf, unsigned short *cLen);
unsigned char ST_EMV_GetRecordD(unsigned char cRecNo, unsigned char cSFI, unsigned char *pRecDt, unsigned short *pRecDtLen);
unsigned char ST_EMV_GetEMVDeviceSn(unsigned char *pSn, unsigned short *pwLenSn);
unsigned char ST_SHA1(unsigned int uiInlen, void *pvDataIn, void *pvDataOut);
unsigned char ST_EMV_TranKernLog(int comHandle);
void ST_EMV_SetFirstSelApp(unsigned char ucFlag);
unsigned char ST_EMV_GetFirstSelApp(void);
// ���׽ӿ�
void ST_EMV_Init(void);
unsigned char ST_EMV_InitTmFixData(TmFixData *ptTmFixDt);
unsigned char ST_EMV_InitiateEMV(AppInitData *pInitDt);
unsigned char ST_EMV_AppSelection(DCardSt *ptCardSt, unsigned char cSelType, unsigned char *pList, unsigned short *pwLenList);
unsigned char ST_EMV_FinalSelect(unsigned char cAidNo, unsigned char *pList, unsigned short *pwLenList, unsigned char *pPDOL, unsigned short *pwLenPDOL);
unsigned char ST_EMV_GetProcOption(unsigned char cAidNo, AppDt *pDt, unsigned char *pList, unsigned short *pwLenList);
unsigned char ST_EMV_ReadAppData(unsigned char *pList, unsigned short *pwLenList);
unsigned char ST_EMV_OfflineDataAuth(void);
unsigned char ST_EMV_TermRiskManage(unsigned char cIsBlackCard, unsigned long lPanAmnt, unsigned char *cResult);
unsigned char ST_EMV_ProcessRestrict(unsigned char *cResult);
unsigned char ST_EMV_NewCardHolderValidate(unsigned char cFlg, unsigned char cCmd, unsigned char *pDt, unsigned short *pwLenDt);
unsigned char ST_EMV_ActionAnalysis(unsigned char cCmd, TransDt *pTransDt);
unsigned char ST_EMV_OnlineDataProcess(unsigned char *pIssDt, unsigned short wLenIssDt, TransDt *pTransDt);
unsigned char ST_EMV_GetVersionInfo(char *pVersion, int *pnLen);
unsigned char ST_EMV_ICCommand_GetData(int p1, int p2, unsigned char *pDataOut, int *pOutLen);
unsigned char ST_EMV_GetTLVList(unsigned char *pTagList, int nTagListLen, unsigned char *pOutList, int *pnOutListLen);
unsigned char ST_EMV_BeginTransaction(TransProDt *pTransInfo, AppDt *pDt);
unsigned char ST_EMV_ContinueTransaction(PINSTATUS *pEncryptInfo);
unsigned char ST_EMV_ReadLastRecord(void);
unsigned char ST_EMV_GetCardnoSfi(int *pnSFI, int *pnRecord);
unsigned char ST_EMV_ReadCardnoData(int nSFI, int nRecord, char *pCard, int *pCardLen);

#endif
