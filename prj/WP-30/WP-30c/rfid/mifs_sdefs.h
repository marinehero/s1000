/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ� 
*   Mifs_SDEFS.h
* ��ǰ�汾�� 
*   01-01-01
* ����ժҪ�� 
*   ���ļ�ʵ�ַǽӴ���ģ��һЩ���ݽṹ�Ķ���
* ��ʷ��¼��
*   �޸���          ����                    �汾��      �޸�˵��
*                06-27-2003              01-01-01
******************************************************************/
 // ֻ����һ��
#ifndef _MIFS_SDEFS_H_
#define _MIFS_SDEFS_H_

#include "wp30_ctrl.h"

/**********************************************************************

                         ��������
                          
***********************************************************************/
#define EM_mifs_BUFFER_LEN                  (uint)(272)
#define EM_mifs_KEYNUM                      16	// EEPROM�������Դ�ŵ�ÿ����Կ����

#define EM_mifs_MIFARE_SECTORNUM            16	// Mifare��������
#define EM_mifs_MIFARE_BLOCKNUMPERSECTOR    4	// Mifare��ÿ����������
#define EM_mifs_FWI_MAXVALUE                14	// ���FWIֵ

/**********************************************************************

                         PCD�����
                          
***********************************************************************/
#define EM_mifs_PCD_IDLE           0x00	// No action: cancel current command
								// or home state, respectively
#define EM_mifs_PCD_WRITEE2        0x01	// Get data from FIFO and write it to the E2PROM
#define EM_mifs_PCD_READE2         0x03	// Read data from E2PROM and put it into the
								// FIFO
#define EM_mifs_PCD_LOADCONFIG     0x07	// Read data from E2PROM and initialise the
								// registers
#define EM_mifs_PCD_LOADKEYE2      0x0B	// Read a master key from the E2PROM and put
								// it into the master key buffer
#define EM_mifs_PCD_AUTHENT1       0x0C	// Perform the first part of the card
								// authentication using the Crypto1 algorithm.
								// Remark: The master key is automatically taken 
								// from 
								// the master key buffer. this implies, that the
								// command LoadKeyE2 has to be executed before 
								// to use a certain key for card authentication
#define EM_mifs_PCD_CALCCRC        0x12	// Activate the CRC-Coprocessor
								// Remark: The result of the CRC calculation can
								// be read from the register CRCResultXXX
#define EM_mifs_PCD_AUTHENT2       0x14	// Perform the second part of the card
								// authentication using the Crypto1 algorithm.
#define EM_mifs_PCD_RECEIVE        0x16	// Activate Receiver Circuitry. Before the
								// receiver actually starts, the state machine 
								// waits until the time 
								// configured in the register RcvWait has 
								// passed.
								// Remark: It is possible to read data from the 
								// FIFO although Receive is active. Thus it is 
								// possible to receive any number of bytes by 
								// reading them from the FIFO in time.
#define EM_mifs_PCD_LOADKEY        0x19	// Read a master key from the FIFO and put it
								// into the master key buffer
								// Remark: The master key has to be prepared in
								// a certain format. Thus, 12 byte have to be 
								// passed to load a 6 byte master key
#define EM_mifs_PCD_TRANSMIT       0x1A	// Transmit data from FIFO to the card
								// Remark: If data is already in the FIFO when 
								// the command is activated, this data is 
								// transmitted immediately. It is possible to 
								// write data to the FIFO although Transmit 
								// is active. Thus it is possible to transmitt 
								// any number of bytes by writting them to the 
								// FIFO in time. 
#define EM_mifs_PCD_TRANSCEIVE     0x1E	// Transmit data from FIFO to the card and after
								// that automatically activates the receiver. 
								// Before the receiver actually starts, the 
								// STATE MACHINE waits until the time configured
								//  in the register RcvWait has passed.
								// Remark: This command is the combination of 
								// Transmit and Receive
#define EM_mifs_PCD_RESETPHASE     0x3F	// Runs the Reset- and Initialisation Phase
								// Remark: This command can not be activated by 
								// software, but only by a Power-On or 
								// Hard Reset 

/**********************************************************************

                         PICC�����
                          
***********************************************************************/
#define EM_mifs_PICC_REQSTD         0x26	// request idle
#define EM_mifs_PICC_REQALL         0x52	// request all
#define EM_mifs_PICC_ANTICOLL1      0x93	// anticollision level 1
#define EM_mifs_PICC_ANTICOLL2      0x95	// anticollision level 2
#define EM_mifs_PICC_ANTICOLL3      0x97	// anticollision level 3
#define EM_mifs_PICC_AUTHENT1A      0x60	// authentication step 1
#define EM_mifs_PICC_AUTHENT1B      0x61	// authentication step 2
#define EM_mifs_PICC_READ           0x30	// read block
#define EM_mifs_PICC_WRITE          0xA0	// write block
#define EM_mifs_PICC_DECREMENT      0xC0	// decrement value
#define EM_mifs_PICC_INCREMENT      0xC1	// increment value
#define EM_mifs_PICC_RESTORE        0xC2	// restore command code
#define EM_mifs_PICC_TRANSFER       0xB0	// transfer command code
#define EM_mifs_PICC_HALT           0x50	// halt
#define EM_mifs_PICC_DESELECT       0xCA	// DESELECT command code
#define EM_mifs_PICC_RATS           0xE0
// T Y P E  B  -  C O M M A N D S 
// commands which are handled by the tag
// Each tag command is written to the reader IC and transfered via RF
#define EM_mifs_PICC_REQB           0x05	// request
#define EM_mifs_PICC_REQBSTD        0x00	// request normal
#define EM_mifs_PICC_REQBWUP        0x08	// request wakeup
#define EM_mifs_PICC_ATTRIB         0x1D	// Attrib������
#define EM_mifs_PICC_SLOTMARKER     0x05	// Slot-Marker�������4λ



#define RFID_ACTIVE_NONE    0   //δpoll
#define RFID_ACTIVE_POLLOK  1   //δ����
#define RFID_ACTIVE_OK      2   //����

#define RFID_PARA_PWRSETTIME      0        //������ǿ����ʱ��
	#define RFID_DEFAULT_PWRSETTIME   15   //Ĭ�ϳ�ǿ����ʱ��
	#define RFID_MAX_PWRSETTIME		  30   //���ʱ��

#define RFID_PARA_PWR      1        //������ǿ��С�ȼ�
	#define RFID_PWR_LEVEL0   0   //��ǿ�ȼ�0 ����
	#define RFID_PWR_LEVEL1   1   //��ǿ�ȼ�1 
	#define RFID_PWR_LEVEL2   2   //��ǿ�ȼ�2 
	#define RFID_PWR_LEVEL3   3   //��ǿ�ȼ�3 ��ǿ
	#define RFID_DEFAULT_PWR  RFID_PWR_LEVEL1   //Ĭ�ϳ�ǿ��С
	#define RFID_MAX_PWR	  RFID_PWR_LEVEL3   //���ȼ�

#define RFID_PARA_CHIP         2        //оƬ����
/**********************************************************************

                         ���ݽṹ����
                          
***********************************************************************/
typedef struct
{
	char acDeviceName[20];				// ���豸������
	uint uiOpenMode;					// ���豸�ķ�ʽ
} ET_IO_DEVICE;

typedef struct
{
    uint  expectMaxRec;                 // Ԥ�ƽ���������󳤶�
	ulong ulSendBytes;					// Ԥ�Ʒ������ݳ���
	ulong ulBytesSent;					// ��ǰ�ѷ����ֽ���
	ulong ulBytesReceived;				// ��ǰ�Ѿ������ֽ���
	long lBitsReceived;					// ��ǰ�Ѿ�����λ��
	ulong ulCardID;						// ��ǰѡ��Ŀ���
	ulong ulMaxLen;						// MifarePro����type B��ͨ����������

	uchar ucCollPos;					// ������ͻ��λ��
	uchar ucAnticol;					// ����ͻ������־
	uchar ucnBits;						// �ڷ���ͻʱ, ���ڱ�ʾ��Ҫ���͵ĺϷ�����λ��ģ8��ֵ, ȡֵ��Χ��0��7
	uchar ucDisableDF;					// �Ƿ���Խ�����Ӧ���̲����Ĵ���, 1-���� 0-������ ��ѡ�������ǲ���1

	uchar ucCurResult;					// �����������еĽ��
	uchar ucINTSource;					// ���������������з������ж�
	uchar ucErrFlags;					// ������ִ���в��������־
	uchar ucSaveErrState;				// ����������ִ���в��������־

	uchar ucEnableTransfer;				// ��ʾ������߽�ֹʹ��Transfer����, 1- ���� 0����ֹ
	uchar ucFirstOpenFlag;				// ��һ�ε���EA_ucMIFOpen�ı�־, 0-��һ�ε���
	uchar ucCurType;					// ��ǰʹ�õĿ����ͺ�: EM_mifs_TYPEA-TYPE A, EM_mifs_TYPEB-TYPE B
	uchar ucCurPCB;						// ֡��

	vuchar TimeSource;					// CLRC663��ʱ���жϼ�¼
	uchar ucPwrLevel;					// ��Դ����
	uchar ucCIDSupport;					// �Ƿ�֧����CID: 1-֧��, 0-��֧��
	uchar ucFWI;						// ��ʱ�ȴ�ʱ������ 
	uchar aucCmdBuf[10];				// ����ֵ�����ʱ�����Ҫ���͵�����         

	uchar ucSAK1;						// ��һ��SELECT����SAKֵ
	uchar ucSAK2;						// �ڶ���SELECT����SAKֵ
	uchar ucSAK3;						// ������SELECT����SAKֵ

	uchar ucMifCardType;				// ��ǰ��Ƭ���ͣ�0 - S50, 1 - S70, 2 - PRO, 3 - TypeB
	uchar ucMifActivateFlag;			// ��Ƶ�������־ 0-δpoll 1-δ����  2-�Ѽ���
	uchar ucUIDLen;						// ��Ƶ����UID���ȣ�TypeA�Ϳ�������4��7��10�ֽڣ�TypeB��ֻ��4�ֽ�
	uchar ucATQA[2];					// ��Ƶ����WUPA�����з��ص�ATQA����

	uchar ucATQB[12];					// ��Ƶ����WUPB�����з��صĲ���      
	uchar ucUIDCL1[5];					// TypeA��һ�����кţ�����BCC�ֽ�
	uchar ucUIDCL2[5];					// TypeA�ڶ������кţ�����BCC�ֽ�
	uchar ucUIDCL3[5];					// TypeA���������кţ�����BCC�ֽ�
	uchar ucUIDB[4];					// TypeB���к�
	uchar ucSFGI;						// TypeA����Ƶ����RATS�������֡�ȴ�ʱ������
	uchar ATS[20];                      // TypeA��ATS

	ushort FIFOSIZE;                    //64-RC531/PN512   512-CLRC663

	uchar RFIDModule;				    //0-RC531  1-PN512  2-CLRC663

	// ����PICC�Ƿ�֧��CID��NAD���ƣ�
	// ����PAYPASSҪ����PCD��PICC֮�����ʹ��CID��NAD����
	uchar ucCIDFlag;					// PICC�Ƿ�֧��CID���ƣ�0 - ��֧�֣� 1 - ֧��
	uchar ucNADFlag;					// PICC�Ƿ�֧��NAD���ƣ�0 - ��֧�֣� 1 - ֧��
	uchar ucWTX;						// ֡�ȴ�ʱ����չ������������TypeA��TypeB��
	uchar ucAnticollFlag;				// �Ƿ�����Ӷ���TypeA���л�ȡһ�ſ�Ƭ 0 - ������ 1 - ����
//	  uchar Frameflg;						// �Ƿ��ж�����֡��0-���ж� 1-���ж�
//    uchar ucCIDPollFlg;                 // ̽��CID�Ƿ�֧�ֱ�־ 0-CID׼��̽�� 1-CID̽�����	

	ulong ulSFGT;						// TypeA����Ƶ����RATS�������֡�ȴ�ʱ�䣬 ��λ��ETU��128/13.56MHz��
	ulong ulFWT;						// ֡�ȴ�ʱ�䣬������TypeA��TypeB������λ��ETU��128/13.56MHz��
	ulong ulFWTTemp;					// ֡�ȴ�ʱ����չ��������TypeA��TypeB������λ��ETU��128/13.56MHz��
	ulong ulTimeout;					// AS3911��ʱʱ�䵥λ��ETU��1/13.56MHz��

	uint FSC;							// FSC��ֵ,���ܽ��յ�֡��󳤶�
	uint FSD;							// FSD��ֵ���ն��ܽ��յ�֡��󳤶�  

	int PwrAdTime;					// Ѱ���ز������ӳ�ʱ��
	uint OptMode;                    // ����ģʽ 00-ISO 01-EMVģʽ(Ӧ��QPBOC��֤ʹ��)
	uchar aucBuffer[EM_mifs_BUFFER_LEN];	// �շ�������    
} ET_mifs_WORKSTRUCT;

extern ET_mifs_WORKSTRUCT EG_mifs_tWorkInfo;	// ���������ṹ


typedef struct{
	uchar  SBlock;      //����S����
	uchar  Cmd;         //��ǰ���͵�ָ�� 
	uchar  IntEnable;   //�����ж�ʹ�ܼĴ���	
	uchar  TimerReload; //����ʱ�ӳ�ʱ�Ĵ���	
	uchar  FDT;         //FDT Ŀǰֻ֧��WUPA/Select/RATS��FDT�ж�
	uchar  FDTFlg;      //FDT ��ǰ������־ 0-����  1-TypeA��ǰ  2-TypeB ��WUPB���������ǰ
	uchar  ReRecvFlg;   //���½��ձ�־  RATS/ATTRIB/Э��������������������4B��Ҫ���¿�ʼ����
	uchar  AttribeTime;  //

    //CLRC663 flag
	uchar  TimerIntEnable;   //RC663�ж�ʹ�ܶ�ʱ������	
//	uchar  Timerflg;         //��ʱ���ж�ʹ�ñ�־ 0x01-time0,0x02-time1,0x04-time2,0x08-time3
	uchar gPN512NFCIP;
	uchar gMifareKey[6];     //Mifare����֤��Կkey a/b
    uchar gTypeArec;    //as3911 type a����������
    uchar gTypeBrec;    //as3911 type b����������
}_t_RfidPro_Info_;

extern _t_RfidPro_Info_ gtRfidProInfo;


typedef struct{
	uchar CW_A;           //
	uchar ModWidth_A;     //
	uchar CW_B;           //
	uchar ModConduct_B;  
}_t_RfidDebugInfo_;
extern _t_RfidDebugInfo_ gtRfidDebugInfo;

typedef struct{
	uchar  ARxThreadhold;
	uchar  ARxControl_1;
	uchar  ARxWait;
	uchar  ATxCW;
	uchar  AMFout;
	
	uchar  BRxThreadhold;
	uchar  BRxControl_1;
	uchar  BRxWait;
	uchar  BTxCW;	
	uchar  BMFout;
}_t_RfidReg_Info_;

extern _t_RfidReg_Info_ gtRfidRegInfo;

//typedef struct{
//	uchar gPN512NFCIP;
//	uchar gPN512Buf[7];
//}_t_RfidPn512_Info_;
//
//extern _t_RfidPn512_Info_ gtPN512Info;

#endif									//_MIFS_SDEFS_H_
