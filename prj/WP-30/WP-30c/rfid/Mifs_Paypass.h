/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ� 
*     Mifs_PAYPASS.h
* ��ǰ�汾�� 
*     01-01-01
* ����ժҪ�� 
*     ���ļ������˷���PAYPASSҪ��������������ݽṹ
* ��ʷ��¼�� 
*     �޸���		      ����			    	   �汾��
*                  02-22-2006 	        01-01-01  
******************************************************************/
 // ֻ����һ��
#ifndef _MIFS_PAYPASS_
#define _MIFS_PAYPASS_

#include "wp30_ctrl.h"

//=========================================
typedef struct 
{
    uint8_t RxThresholdTypeA;//RxThresholdReg
    uint8_t RxThresholdTypeB;//RxThresholdReg
    uint8_t Demod;       //DemodReg
	uint8_t GsNOff; //GsNOffReg
    uint8_t RFCfg;  //RFCfgReg
	uint8_t GsNOn;	//GsNOnReg
	uint8_t CWGsP;	//CWGsPReg
	uint8_t ModGsP; //ModGsPReg
//    uint8_t rfu[3];
}MIFS_REG;
#ifdef EM_PN512_Module
extern MIFS_REG gcMifReg;
#endif


/***********************************************************************
                            ��������
************************************************************************/
#define  EM_mifs_NODELAY     (uint)0x0000
#define  EM_mifs_LOOP        (uint)0xFFFF

#define  EM_mifs_FWTMIN       9			// ����WUPA��ANTICOLLISION��SELECT����ĵȴ���Ӧʱ��
									    // ʵ����Ӧ����9.2ETU								
#define  EM_mifs_FWTRATS      560		// ���ڵȴ�RATS֡
#define  EM_mifs_FWTDESELECT  560		// ���ڵȴ�DESELECT��Ӧ֡
#define  EM_mifs_FWTWUPB      60		// ���ڵȴ�WUPB��Ӧ֡
#define  EM_mifs_MAXFWT       8960		// FWT���ʱ��Ϊ32 * 2^8 + 3 * 2^8 = 8960ETU
#define  EM_mifs_DEFAULT      560		// ȱʡ֡�ȴ�ʱ��Ϊ560ETU
#define  EM_mifs_TPDEALY     (uint)500*2	// �л����Ʒ�ʽʱ��ʱ500��ETU
#define  EM_mifs_TRESET      (uint)1000	// ��λPICCʱ����ʱ1000ETU
#define  EM_mifs_FDTMIN      (uint)60	// ���ڱ�֤PCD������һ������֡ǰ��500us��ʱ

#define  EM_mifs_FSD_Default  256		//Ĭ��FSD
#define  EM_mifs_FSC_Default  32		//Ĭ��FSC


// PCD����ʱ��
#define  TIME_WAIT_TReset          8   //8ms  5.1ms~10ms  Poll��λ������
#define  TIME_WAIT_TResetDelay     25  //25ms 0~33ms      Col��λ������
#define  TIME_WAIT_Tp              7   //8ms  5.1ms~10ms  �ز��������ӳ�
#define  TIME_WAIT_Resend          6   //1ms  0~33ms      �ط������ӳ�

#define  PCD_TRYON_TIMES           3   //��poll��remove���������ʱʱ��ೢ��3��

/**********************************************************************

                         �ڲ�����ֵ����
                          
***********************************************************************/

//��ȷ��1ms
#define rfid_delayms(ms)   s_DelayMs(ms)
/**********************************************************************

                         ȫ�ֺ���ԭ��
                          
***********************************************************************/
void EI_paypass_vInit(void);
void EI_paypass_vSetTimer(ulong ulETU);	// ���ó�ʱʱ��
void EI_paypass_vDelay(ulong ulETU);	// ��ʱ
uchar EI_paypass_ucWUPA(uchar * pucResp);	// ����TypeA��
uchar EI_paypass_ucAnticoll(uchar ucSEL, uchar * pucUID);	// TypeA������ͻ
uchar EI_paypass_ucSelect(uchar ucSEL, uchar * pucUID, uchar * pucSAK);	// ѡ��TypeA����ACTIVATE״̬
uchar EI_paypass_ucRATS(uchar * pucOutLen, uchar * pucATSData);	// ����RATS���ʹTypeA������Protocol״̬
void EI_paypass_vHALTA(void);			// ʹTypeA������HALT״̬
void EI_paypass_vResetPICC(void);		// �ر�RC531�ز�����λ���е�PICCs
void EI_paypass_vOptField(uchar mode);
uchar EI_paypass_ucWUPB(uchar * pucOutLen, uchar * pucOut);	// ����TypeB��
uchar EI_paypass_ucAttrib(uchar * pucResp);	// ѡ��TypeB����ʹ֮����ACTIVATE״̬
uchar EI_paypass_ucHALTB(uchar * pucResp);	// ʹTypeB������HALT״̬
uchar EI_paypass_ucGetUID(void);		// ��ȡTypeA��������UID��Ϣ
uchar EI_paypass_ucActPro(uchar * pucOutLen, uchar * pucATSData);	// ����TypeA��
uchar EI_paypass_ucActTypeB(uchar * pucOutLen, uchar * pucOut);	// ����TypeB��
void EI_paypass_vSelectType(uchar ucMifType);	// ѡ���ز����Ʒ���
uchar EI_paypass_ucPOLL(uchar * pucMifType);	// �жϸ�Ӧ�����Ƿ��п�
uchar EI_paypass_ucIfMifExit(void);		// �ж�Mif���Ƿ��ڸ�Ӧ��
uchar EI_paypass_ucExchange(uint uiSendLen, uchar * pucInData, uint * puiRecLen,
	uchar * pucOutData);
uchar EI_paypass_ucDeSelect(void);		// DESELECT����
uchar EI_paypass_ucProcess(uint uiSendLen, uint * puiRecLen);	// �����
uchar EI_paypass_ucMifRBlock(uint * puiRecLen);	// ����R_Block
uchar EI_paypass_ucMifSBlock(uint * puiRecLen);	// ����S_Block
uchar EI_ucAnticoll(uchar ucSEL, uchar * pucUID);
uint EI_rfid_GetFramesize(uchar fsdi);
void s_rfid_init(void);
uint s_rfid_FWI2FWT(uint FWI); 


#endif
