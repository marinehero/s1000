  /*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ� 
*   Mifs.h
* ��ǰ�汾�� 
*   01-01-01
* ����ժҪ�� 
*   ���ļ�ʵ��Mifs.c��ͷ�ļ�,�����ṩAPI�ӿ�
* ��ʷ��¼��
*   �޸���      ����		    	�汾��      �޸ļ�¼
******************************************************************/
 // ֻ����һ��
#ifndef _MIFS_H_
#define _MIFS_H_

#include "wp30_ctrl.h"
/**********************************************************************

                         ��������
                          
***********************************************************************/
// ISO14443 
#define EM_mifs_TYPEA				0x00
#define EM_mifs_TYPEB				0x08
#define EM_mifs_NFCIP				0x01
#define EM_mifs_NULL				0xFF

// Mifare��Requestʱ�Ĳ���
#define EM_mifs_IDLE    0				// ֻ��һ�ſ���λ
#define EM_mifs_ALL     1				// ����Ƶ���ߵ���Ч��Χ�ڵ����п���λ

#define EM_mifs_HALT    1				// ������HALT״̬�Ŀ���λ

// Mifare����֤ʱ����
#define EM_mifs_KEYA	0				// A������
#define EM_mifs_KEYB	1				// B������

// TYPE B���Ĳ���
#define EM_mifs_TYPEB_REQALL  0			// �����ڵĿ�Ƭ����Ӧ

#define EM_mifs_TIMESLOT_MAXINT  4		// ���ʱ϶����Ӧ��ϵ��
#define EM_mifs_TIMESLOT_MAXNUM  16		// ���ʱ϶��

#define EM_mifs_IDLE 0					// ���Ƭʱ��Ƭ����IDLE״̬
#define EM_mifs_HALT 1					// ���Ƭʱ��Ƭ����HALT״̬

#define EM_mifs_SUPPORTB    1			// ֧��ISO14443-4
#define EM_mifs_NOTSUPPORTB 0			// ��֧��ISO14443-4

#define EM_mifs_MAXPID   14				// TYPE B�������PID

#define EM_mifs_S50          0
#define EM_mifs_S70          1
#define EM_mifs_PROCARD      2
#define EM_mifs_PRO_S50      3
#define EM_mifs_PRO_S70      4
#define EM_mifs_TYPEBCARD    5

/**********************************************************************

                         ����ֵ
                          
***********************************************************************/
// ICODE1 Error   Codes                        
#define EM_mifs_NOTAGERR		     2	// ������Χ���޿�
#define EM_mifs_CRCERR      	     3	// ��CRCУ�����
#define EM_mifs_AUTHERR              4	// ��������֤
#define EM_mifs_PARITYERR	         5	// �ӿ����յ������PARITY
#define EM_mifs_CODEERR     	     6	// �������������ݴ���
#define EM_mifs_SERNRERR    	     7	// ��Anticoll��ͨѶ����
#define EM_mifs_NOTAUTHERR           8	// δ��֤
#define EM_mifs_BITCOUNTERR          9	// ��������������λλ������
#define EM_mifs_BYTECOUNTERR         10	// ���������������ֽ�������
#define EM_mifs_OVFLERR              17	// �������
#define EM_mifs_FRAMINGERR           18	// ֡����
#define EM_mifs_UNKNOWN_COMMAND      19	// ��Ч����
#define EM_mifs_COLLERR              20	// ��Ƭ��ͻ
#define EM_mifs_RESETERR             21	// ��λʧ��
#define EM_mifs_INTERFACEERR         22	// �ӿڴ���
#define EM_mifs_RECBUF_OVERFLOW      23	// ���ջ��������
#define EM_mifs_VALERR               24	// ��ֵ����
#define EM_mifs_ERRTYPE              25	// �����ʹ���
#define EM_mifs_FDTERR               26 // FDT����
#define EM_mifs_SWDIFF               0xE7	// SW1!=0X90����SW2!=0X00

// ��������ֵ 
#define EM_mifs_SUCCESS              EM_SUCCESS
#define EM_mifs_TRANSERR             0xA2	// ��Ƭ�������֮��ͨ�Ŵ���
#define EM_mifs_PROTERR              0xA3	// ��Ƭ�������ݲ�����ISO14443����PAYPASS�淶
#define EM_mifs_MULTIERR             0xA4	// ��Ӧ���࿨����
#define EM_mifs_NOCARD               0xA5	// �ڹ涨ʱ����û���жϵ��������Ӧ��
#define EM_mifs_CARDEXIST            0xA6	// �ڹ涨ʱ����û�н��������Ӧ��
#define EM_mifs_TIMEOUT              0xA7	// ���ݽ��������У���Ƭ����Ӧ
#define EM_mifs_NOACT                0xB3	// ��Ƭδ����
#define EM_mifs_NOISE                0xA8   // ����

//������api�ķ���ֵ
// Ѱ��ģʽ
// D0: Ѱ����ʽ 0-ISO 1-EMV   
// D1: �Ƿ��жϿ���֡�� 1-���ж�
// D2: �Ƿ�֧��CID 
// D3: �Ƿ�֧��NAD 
// D8: ֧��TYPEA��
// D9: ֧��TYPEB��
#define RFID_MODE_ISO                (0<<0)   //ISO��ʽѰ�������ԴӶ��ſ���Ѱ��1�ſ�
#define RFID_MODE_EMV                (1<<0)   //EMV��ʽѰ����ֻ����1�ſ�
#define RFID_MASK_OPT                (1<<0)
#define RFID_MODE_NOFRAME            (1<<1)   //ISO��ʽѰ�������жϿ��˷���֡��
#define RFID_MASK_FRAME              (1<<1)
#define RFID_MODE_CID                (1<<2)   //֧��CID
#define RFID_MASK_CID                (1<<2)
#define RFID_MODE_NAD                (1<<3)   //֧��NAD
#define RFID_MASK_NAD                (1<<3)   

#define RFID_MODE_TYPEA              (1<<8)   //Ѱ��TypeA��
#define RFID_MODE_TYPEB              (1<<9)   //Ѱ��TypeB��
//#define RFID_MODE_ALL                (RFID_MODE_TYPEA | RFID_MODE_TYPEB)  //Ѱ��2�ֿ�
//#define RFID_MODE_MASK               (RFID_MODE_ALL | RFID_MODE_EMV)

// poll���ؿ�����ģʽ
#define RFID_TYPEA                   0   //A��
#define RFID_TYPEB                   5   //B��

// powerup���1���ֽڷ��ؿ�������
#define RFID_CARD_S50                0
#define RFID_CARD_S70                1
#define RFID_CARD_PRO                2
#define RFID_CARD_PROS50             3
#define RFID_CARD_PROS70             4
#define RFID_CARD_B                  5


// ����ֵ
enum RFID_RET{
	RFID_SUCCESS = 0,
	RFID_ERROR = 4601,   //1��ģ��ʧ��
	RFID_ERRPARAM,	     //2�������� 
	RFID_TIMEOUT,        //3��ʱ
	RFID_NOCARD,         //4�޿�
	RFID_TRANSERR,       //5��Ƭ�������֮��ͨ�Ŵ���
	RFID_PROTERR,        //6��Ƭ�������ݲ�����ISO14443����PAYPASS�淶
	RFID_MULTIERR,	     //7��Ӧ���࿨����	
	RFID_NOACT,          //8��δ�ϵ�
	RFID_CARDEXIST,      //9��δ�뿪
	RFID_NOAUTH,         //10δ��֤
	RFID_AUTHERR,        //11��֤ʧ��
	RFID_UNCHANGE,		 //12�����޸�
	RFID_KEYNOTPOWER,	 //13��Կ��Ȩ��

	RFID_DEVICEUSED	= 4630,  	   //�豸��ռ��
	RFID_NODEVICE	= 4631,      // �豸δ��
};

#ifdef __ICCARM__						// IAR
#pragma pack(1)            // IAR
#define __attribute__(...) // IAR
#endif	
typedef union 
{
	uchar byte;
	struct
	{
		uchar bit0:1;
		uchar bit1:1;
		uchar bit2:1;
		uchar bit3:1;
		uchar bit4:1;
		uchar bit5:1;
		uchar bit6:1;
		uchar bit7:1;		
	};
}__attribute__ ((packed)) BYTE2BIT;
typedef struct
{
	uchar sector;
	uchar blk;
}__attribute__ ((packed)) MIFARE_MEM;

typedef struct 
{
	uchar pwred;    //�ϵ��־ 0-δ�ϵ� 1-�ϵ� 2-��֤
	uchar sector;
	uchar type; 	//0-S50 1-S70
	uchar key;		//0-����Կ 1-KeyA 2-KeyB
	uchar keybuf[6];
}__attribute__ ((packed)) MIFARECARD;

typedef struct _KEY
{
	uchar key0;
	uchar key1;
	uchar key2;
	uchar key3;
	uchar key4;
	uchar key5;
}__attribute__ ((packed)) KEY;
typedef union _ACCESSBIT
{
	u32 w;
	struct
	{
		uchar versac1:4;
		uchar versac2:4;
		uchar versac3:4;
		uchar c1:4;			
		uchar c2:4;
		uchar c3:4;
		uchar rfu:8;
	};
}__attribute__ ((packed)) ACCESSBIT;
typedef struct _TRAILER
{
	KEY keyA;
	ACCESSBIT access;
	KEY keyB;
}__attribute__ ((packed)) TRAILER;

struct RECHARGE{
	uchar mode;
	ulong value;
}__attribute__ ((packed)) ;

#ifdef __ICCARM__          // IAR
#pragma pack()             // IAR
#endif  
extern MIFARECARD gMifareData;

typedef struct _BLKALTER
{
	uchar mode;
	uchar access;
	uchar keyA[6];
	uchar keyB[6];
	uchar rfu[2];
}BLKALTER;

typedef struct
{
    int     index;
    int     pwrfield;
}s_rfidpara_info;
#define S50BLK_SIZE			4
#define S70BLK_SIZE_PRE32	4
#define S70BLK_SIZE_LAST8	16
//#define S70BLK_SIZE			(S70BLK_SIZE_PRE32+S70BLK_SIZE_LAST8)

#define S50SECTOR_SIZE			16
#define S70SECTOR_SIZE_PRE32	32
#define S70SECTOR_SIZE_LAST8	8
#define S70SECTOR_SIZE			(S70SECTOR_SIZE_PRE32+S70SECTOR_SIZE_LAST8)


/**********************************************************************

                         �ӿڶ���
                          
***********************************************************************/

typedef struct
{
	// Mifare��
	uchar(*EA_ucMIFRequest) (DevHandle hDevHandle, uchar ucMode, ushort * pusTagType);
	uchar(*EA_ucMIFAntiColl) (DevHandle hDevHandle, ulong * pulSnr);
	uchar(*EA_ucMIFSelect) (DevHandle hDevHandle, ulong ulSnr, uint * puiSize);
	uchar(*EA_ucMIFAuthentication) (DevHandle hDevHandle, uchar ucMode, uchar ucSecNr);
	uchar(*EA_ucMIFAuthToRam) (DevHandle hDevHandle, uchar ucSecNr, uchar ucKeyType,
		uchar * pucKey);
	uchar(*EA_ucMIFLoadKey) (DevHandle hDevHandle, uchar ucMode, uchar * pucNKey);
	uchar(*EA_ucMIFHalt) (DevHandle hDevHandle);
	uchar(*EA_ucMIFRead) (DevHandle hDevHandle, uchar ucAddr, uchar * pucData);
	uchar(*EA_ucMIFWrite) (DevHandle hDevHandle, uchar ucAddr, uchar * pucData);
	uchar(*EA_ucMIFIncrement) (DevHandle hDevHandle, uchar ucAddr, ulong ulValue);
	uchar(*EA_ucMIFDecrement) (DevHandle hDevHandle, uchar ucAddr, ulong ulValue);
	uchar(*EA_ucMIFRestore) (DevHandle hDevHandle, uchar ucAddr);
	uchar(*EA_ucMIFTransfer) (DevHandle hDevHandle, uchar ucAddr);

	// MifarePro
	uchar(*EA_ucMIFResetPro) (DevHandle hDevHandle, uchar ucMode, uint * puiRespLen, void *pvResp);
	uchar(*EA_ucMIFExchangePro) (DevHandle hDevHandle, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar(*EA_ucMIFDeselectPro) (DevHandle hDevHandle);

	// TYPE B
	uchar(*EA_ucMIFRequestB) (DevHandle hDevHandle, uchar ucState, uchar ucType,
		uchar ucCount, uint * puiRespLen, void *pvResp);
	uchar(*EA_ucMIFSlotMarkerB) (DevHandle hDevHandle, uchar ucNumber, uint * puiRespLen,
		void *pvResp);
	uchar(*EA_ucMIFAttriB) (DevHandle hDevHandle, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar(*EA_ucMIFExchangeB) (DevHandle hDevHandle, uchar ucCID, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar(*EA_ucMIFDeselectB) (DevHandle hDevHandle, uchar ucCID);
	uchar(*EA_ucMIFHaltB) (DevHandle hDevHandle, ulong ulPUPI);

	uchar(*EA_ucMIFCommandPro) (DevHandle hDevHandle, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar(*EA_ucMIFCommandB) (DevHandle hDevHandle, uchar ucCID, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);

	// ��չ
	uchar(*EA_ucMIFSetTime) (DevHandle hDevHandle, uchar ucTime);
	uchar(*EA_ucMIFAuthExtend) (DevHandle hDevHandle, uchar ucMode, uchar ucAddr);
	uchar(*EA_ucMIFAuthToRamExtend) (DevHandle hDevHandle, uchar ucAddr, uchar ucKeyType,
		uchar * pucKey);
	uchar(*EA_ucMIFSetRegVal) (DevHandle hDevHandle, uchar ucAddr, uchar ucVal);

	// ����API����
	uchar(*EA_ucMIFPollMifCard) (DevHandle hDevHandle, uchar ucAntiFlag, uchar * pucMifCardType);
	uchar(*EA_ucMIFActivate) (DevHandle hDevHandle, uchar ucMifType, uint * puiOutLen, void *pvOut);
	uchar(*EA_ucMIFAPDU) (DevHandle hDevHandle, uint uiSendLen, void *pvInData, uint * puiRecLen,
		void *pvOutData);
	uchar(*EA_ucMIFDeactivate) (DevHandle hDevHandle);

	void *pvMifsExpand;
} ET_MIFS_BASE_EXPORT;

typedef struct
{
	uchar(*EA_ucMifsOpen) (uchar ucOpenMode);
	uchar(*EA_ucMifsClose) (void);
	void *Reserved;
} ET_MIFS_IODRV_EXPORT;

typedef struct
{
	ET_MIFS_BASE_EXPORT *ptBase;
	ET_MIFS_IODRV_EXPORT *ptIODrv;
	void *Reserved;
} ET_MIFS_EXPORT_TABLE;

/**********************************************************************

                         API����ԭ��
                          
***********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

	uchar EA_ucMifsOpen(uchar ucOpenMode);
	uchar EA_ucMifsClose(void);
	uchar EA_ucMIFSetTime(DevHandle hDevHandle, uchar ucTime);

	// Mifare��
	uchar EA_ucMIFRequest(DevHandle hDevHandle, uchar ucMode, ushort * pusTagType);
	uchar EA_ucMIFAntiColl(DevHandle hDevHandle, ulong * pulSnr);
	uchar EA_ucMIFSelect(DevHandle hDevHandle, ulong ulSnr, uint * puiSize);
	uchar EA_ucMIFAuthentication(DevHandle hDevHandle, uchar ucMode, uchar ucSecNr);
	uchar EA_ucMIFAuthToRam(DevHandle hDevHandle, uchar ucSecNr, uchar ucKeyType, uchar * pucKey);
	uchar EA_ucMIFLoadKey(DevHandle hDevHandle, uchar ucMode, uchar * pucNKey);
	uchar EA_ucMIFHalt(DevHandle hDevHandle);
	uchar EA_ucMIFRead(DevHandle hDevHandle, uchar ucAddr, uchar * pucData);
	uchar EA_ucMIFWrite(DevHandle hDevHandle, uchar ucAddr, uchar * pucData);
	uchar EA_ucMIFIncrement(DevHandle hDevHandle, uchar ucAddr, ulong ulValue);
	uchar EA_ucMIFDecrement(DevHandle hDevHandle, uchar ucAddr, ulong ulValue);
	uchar EA_ucMIFRestore(DevHandle hDevHandle, uchar ucAddr);;
	uchar EA_ucMIFTransfer(DevHandle hDevHandle, uchar ucAddr);

	// MifarePro
	uchar EA_ucMIFResetPro(DevHandle hDevHandle, uchar ucMode, uint * puiRespLen, void *pvResp);
	uchar EA_ucMIFExchangePro(DevHandle hDevHandle, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar EA_ucMIFDeselectPro(DevHandle hDevHandle);
	uchar EA_ucMIFCommandPro(DevHandle hDevHandle, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);

	// TYPE B
	uchar EA_ucMIFRequestB(DevHandle hDevHandle, uchar ucState, uchar ucType,
		uchar ucCount, uint * puiRespLen, void *pvResp);
	uchar EA_ucMIFSlotMarkerB(DevHandle hDevHandle, uchar ucNumber, uint * puiRespLen,
		void *pvResp);
	uchar EA_ucMIFAttriB(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint * puiRecLen,
		void *pvRecData);
	uchar EA_ucMIFExchangeB(DevHandle hDevHandle, uchar ucCID, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar EA_ucMIFDeselectB(DevHandle hDevHandle, uchar ucCID);
	uchar EA_ucMIFCommandB(DevHandle hDevHandle, uchar ucCID, uint uiSendLen, void *pvSendData,
		uint * puiRecLen, void *pvRecData);
	uchar EA_ucMIFHaltB(DevHandle hDevHandle, ulong ulPUPI);

	uchar EA_ucMIFAuthExtend(DevHandle hDevHandle, uchar ucMode, uchar ucAddr);
	uchar EA_ucMIFAuthToRamExtend(DevHandle hDevHandle, uchar ucAddr, uchar ucKeyType,
		uchar * pucKey);
	uchar EA_ucMIFSetRegVal(DevHandle hDevHandle, uchar ucAddr, uchar ucVal);

	// ����API���� 
	uchar EA_ucMIFPollMifCard(DevHandle hDevHandle, uchar ucAntiFlag, uchar * pucMifCardType);
	uchar EA_ucMIFActivate(DevHandle hDevHandle, uchar ucMifType, uint * puiOutLen, void *pvOut);
	uchar EA_ucMIFAPDU(DevHandle hDevHandle, uint uiSendLen, void *pvInData, uint * puiRecLen,
		void *pvOutData);
	uchar EA_ucMIFDeactivate(DevHandle hDevHandle);

	// F01��API
	int rfid_open(uint mode);
	int rfid_close(void);
	int rfid_poll(uint mode, uint *cardtype);
	int rfid_powerup(uint cardtype, uint *puiOutLen, uchar *pvOut);
	int rfid_exchangedata(uint sendlen, uchar *psenddata, uint *precvlen, uchar *precvdata);
	int rfid_powerdown(void);
	int rfid_MIFAuth(uint Sec, uint keytype, uchar *pData);
	int rfid_MIFRead(uint block, uchar *out);
	int rfid_MIFWrite(uint block, uchar *in);
	int rfid_MIFIncrement(uint block, ulong value);
	int rfid_MIFDecrement(uint block, ulong value);
	int rfid_MIFRestore(uint block);
	int rfid_MIFTransfer(uint block);
	int rfid_MIFModify(uint block, const BLKALTER *alterVal);
	void rfid_MIFMakevalue(long value,uchar addr, uchar *out);

	uchar rfid_MIFParseStatus(const ACCESSBIT *pAccess, uchar ucBlkNum);
	uchar rfid_MIFAccessIfAlter(uchar access,uchar authKey);
	uchar rfid_MIFKeyIfAlter(uchar access,uchar authKey);
	void rfid_MIFMakeStatus(uchar ucIndata,uchar ucBlkNum, ACCESSBIT *pAccess);
	int rfid_ParaAdjust(int para, int step);
    int rfid_ResetField(int para);
    int adjust_rfid_PwrFieldSetTime(int mode);
    int adjust_rfid_PwrField(int mode);
//    int rfid_type_menu(int mode);
    int ReadRfid_Para_Get(int paramIndex, int *param);
    int WriteRfid_Para_Set(int paramIndex, int param);

	
#ifdef __cplusplus
}
#endif

#endif									//_MIFS_H_
