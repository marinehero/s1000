/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ�
*     Mifs_PayPass.c
* ��ǰ�汾��
*     01-01-01
* ����ժҪ��
*     ���ļ�����ʵ�ַ���ISO14443��PAYPASS�淶�ķǽӴ��������ĳ���
* �����ĺ�����
*     EI_paypass_vInit;         // ��ر�����ʼ��
*     EI_paypass_vSetTimer;     // ���ó�ʱʱ��
*     EI_paypass_vDelay;        // ��ʱ
*     EI_paypass_ucWUPA;        // ����TypeA��
*     EI_paypass_ucAnticoll;    // TypeA������ͻ
*     EI_paypass_ucSelect;      // ѡ��TypeA����ACTIVATE״̬
*     EI_paypass_ucRATS;        // ����RATS���ʹTypeA������Protocol״̬
*     EI_paypass_ucHALTA;       // ʹTypeA������HALT״̬
*     EI_paypass_vResetPICC;    // �ر�RC531�ز�����λ���е�PICCs
*     EI_paypass_ucWUPB;        // ����TypeB��
*     EI_paypass_ucAttrib;      // ѡ��TypeB����ʹ֮����ACTIVATE״̬
*     EI_paypass_ucHALTB;       // ʹTypeB������HALT״̬
*     EI_paypass_ucGetUID;      // ��ȡTypeA��������UID��Ϣ
*     EI_paypass_ucActPro;      // ����TypeA��
*     EI_paypass_ucActTypeB;    // ����TypeB��
*     EI_paypass_ucSelectType;  // ѡ���ز����Ʒ���
*     EI_paypass_ucPOLL;        // �жϸ�Ӧ�����Ƿ��п�
*     EI_paypass_ucIfMifExit;   // �ж�Mif���Ƿ��ڸ�Ӧ��
*     EI_paypass_ucExchange;    // APDU����
*     EI_paypass_ucDeSelect;    // DESELECT����
*     EI_paypass_ucProcess;     // �����
*     EI_paypass_ucMifRBlock;   // ����R_Block
*     EI_paypass_ucMifSBlock;   // ����S_Block
* ��ʷ��¼��
*     �޸���           ����              �汾��
*                   04-04-2006          01-01-01
******************************************************************/
#include "wp30_ctrl.h"

#ifdef CFG_RFID
/*****************************************************************

                        �ⲿ����˵��

******************************************************************/

/***************************************************************
                        ȫ�ֱ���˵��

****************************************************************/
#define EM_paypass_TRYON_TIMES     3  //��������������

#define EM_paypass_protocol_TIMES  3  //Э������ط�����

#define EM_paypass_polltime        5000 //type a/b�� ��ѯ���ʱ��
#define EM_paypass_minFDTadtime    250  //pcd����֡����ʱ��FDTmin

_t_RfidDebugInfo_ gtRfidDebugInfo;
#ifdef RFID_ENABLE_REG
_t_RfidReg_Info_ gtRfidRegInfo = {
	0xFF,0x73,0x06,0x3F,0x04,
    0x64,0x73,0x03,0x3F,0x03
};
#endif

#ifdef EM_PN512_Module
MIFS_REG gcMifReg = {
//#if Product_Type == Product_3
//	#if PCD_TYPE == PCD_F11
//	0xF4,0x0F,0x04
//	#elif PCD_TYPE == PCD_F10_V20
//	0xF4,0x0F,0x0b
//	#else
//	0xF4,0x0F,0x04
//	#endif
//#elif Product_Type == Product_F05_2
//	0xF4,0x0F,0x04
//#elif Product_Type == Product_F14
//	0xF4,0x0F,0x04
//#else
//	0xF4,0x0F,0x04
//#endif
    0x7B,0x6B,0x5D,0x38,0x78,0x82,0x38,0x19
};
#endif
/*****************************************************************

                          ��������

******************************************************************/
extern int s_rfid_getPara(int module, int para, int index, s_rfidpara_info *rfdata);
/*****************************************************************
* �������ƣ�
*        s_rfid_init
* ����������
*
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
****************************************************************/
void s_rfid_init(void)
{
    extern const s_rfidpara_info rfidpara_FM17550_V1EX[4];
    int  module_type;
    int  index,ret;
    s_rfidpara_info rfdata;

	memset(&gtRfidProInfo,0,sizeof(gtRfidProInfo));
	memset(&EG_mifs_tWorkInfo, 0, sizeof(EG_mifs_tWorkInfo));
//	memset(&gtPN512Info, 0, sizeof(gtPN512Info));

    module_type = sys_get_module_type(MODULE_RF_TYPE);
//    module_type = MODULE_RF_FM17550;

#ifdef EM_RC531_Module
    if ( module_type == MODULE_RF_RC531 ) {
        EG_mifs_tWorkInfo.RFIDModule = RFID_Module_RC531;  //0-RC531   1-PN512
        EG_mifs_tWorkInfo.FIFOSIZE = 64;
    }
#endif

#ifdef EM_PN512_Module
    if ( (module_type == MODULE_RF_PN512_BASE) || (module_type == MODULE_RF_FM17550) ) {
        EG_mifs_tWorkInfo.RFIDModule = RFID_Module_PN512;  //0-RC531   1-PN512
        EG_mifs_tWorkInfo.FIFOSIZE = 64; //max 96
    }
#endif

#ifdef EM_RC663_Module
    if ( module_type == MODULE_RF_RC663 ) {
        EG_mifs_tWorkInfo.RFIDModule = RFID_Module_RC663;  //0-RC531   1-PN512
    }
#endif

#ifdef EM_AS3911_Module
    if ( module_type == MODULE_RF_AS3911 ) {
        EG_mifs_tWorkInfo.RFIDModule = RFID_Module_AS3911;  //0-RC531   1-PN512
        EG_mifs_tWorkInfo.FIFOSIZE = 64; //max 96
    }
#endif

	s_Rfid_vHalInit();

	// ��ʼ��ȫ�ֱ���
	EG_mifs_tWorkInfo.ulMaxLen = 32;
	EG_mifs_tWorkInfo.ucCIDSupport = 1;
	EG_mifs_tWorkInfo.ucFWI = EM_mifs_FWI_MAXVALUE;

	EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEA;
	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_NONE;
	EG_mifs_tWorkInfo.FSC = EM_mifs_FSC_Default;
	EG_mifs_tWorkInfo.FSD = EM_mifs_FSD_Default;

    //�Ȼ�ȡĬ�ϲ���
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
        s_rfid_getPara(EG_mifs_tWorkInfo.RFIDModule, RFID_PARA_PWR, RFID_DEFAULT_PWR, &rfdata);
    }
#ifdef EM_PN512_Module
    else if (  EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512  ) {
        s_rfid_getPara(EG_mifs_tWorkInfo.RFIDModule, RFID_PARA_PWR, RFID_DEFAULT_PWR, &rfdata);
        gcMifReg.GsNOn = (uchar)rfdata.pwrfield;
        gcMifReg.CWGsP = (uchar)rfidpara_FM17550_V1EX[RFID_DEFAULT_PWR].pwrfield;
    }
#endif
    else{
        //531��ʼ����ǿ��ǿ
        s_rfid_getPara(EG_mifs_tWorkInfo.RFIDModule, RFID_PARA_PWR, RFID_PWR_LEVEL3, &rfdata);
    }
    gtRfidDebugInfo.CW_A = rfdata.pwrfield;
    gtRfidDebugInfo.CW_B = gtRfidDebugInfo.CW_A;

//    Dprintk("\r\n befor pwr:%x ",gtRfidDebugInfo.CW_A);

    //��ȡflash����
	ret = ReadRfid_Para_Get(RFID_PARA_PWR, &index);
    if ( ret || (index > RFID_MAX_PWR)) {
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
            index = RFID_DEFAULT_PWR;
        }else if (  EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512 ) {
            index = RFID_DEFAULT_PWR;
        }else
            index = RFID_PWR_LEVEL3; //531��ʼ����ǿ��ǿ
		WriteRfid_Para_Set(RFID_PARA_PWR, index);
    }else{
        ret = s_rfid_getPara(EG_mifs_tWorkInfo.RFIDModule, RFID_PARA_PWR, index, &rfdata);
        if ( !ret ) {
            gtRfidDebugInfo.CW_A = rfdata.pwrfield;
            gtRfidDebugInfo.CW_B = gtRfidDebugInfo.CW_A;
#ifdef EM_PN512_Module
            if (  EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512  ){
                gcMifReg.GsNOn = (uchar)rfdata.pwrfield;
                gcMifReg.CWGsP = (uchar)rfidpara_FM17550_V1EX[index].pwrfield;
            }
#endif
        }
    }

//    Dprintk("\r\n after pwr:%x ",gtRfidDebugInfo.CW_A);

	gtRfidDebugInfo.ModWidth_A = 0x13;

	gtRfidDebugInfo.ModConduct_B = 0x06;

	ReadRfid_Para_Get(RFID_PARA_PWRSETTIME, &EG_mifs_tWorkInfo.PwrAdTime);
	if((EG_mifs_tWorkInfo.PwrAdTime<RFID_DEFAULT_PWRSETTIME)
		|| (EG_mifs_tWorkInfo.PwrAdTime>RFID_MAX_PWRSETTIME))
	{
		EG_mifs_tWorkInfo.PwrAdTime = RFID_DEFAULT_PWRSETTIME;  //cf02282012
		WriteRfid_Para_Set(RFID_PARA_PWRSETTIME, EG_mifs_tWorkInfo.PwrAdTime);
	}
}
/*****************************************************************
* �������ƣ�
*        EI_rfid_GetFramesize
* ����������
*        ����FSDI��FSCI��ȡ֡��С
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
uint  s_rfid_FWI2FWT(uint FWI)
{
	uint uiFWITemp,FWT;
	uiFWITemp = (uint) 0x01 << FWI;
	if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEA)
	{
		FWT = (uint) (35) * uiFWITemp;
		if(FWI != 14 && FWI != 13 && FWI != 12)
			FWT += 1;
	}
	else
	{
		FWT = (uint) (35) * uiFWITemp;
		if(FWI <= 2)
		{
			FWT += 2;
		}
		else
		{
			FWT += 1;
		}
	}
	return FWT;
}
/*****************************************************************
* �������ƣ�
*        EI_rfid_GetFramesize
* ����������
*        ����FSDI��FSCI��ȡ֡��С
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
uint EI_rfid_GetFramesize(uchar fsdi)
{
	switch(fsdi)
	{
		case 0:
			return 16;
		case 1:
			return 24;
		case 2:
			return 32;
		case 3:
			return 40;
		case 4:
			return 48;
		case 5:
			return 64;
		case 6:
			return 96;
		case 7:
			return 128;
		case 8:
		default:
			return 256;
	}
}

/*****************************************************************
* �������ƣ�
*
* ����������
*        ��ǰ�������
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
#define INC_PCB_BLOCKNO(PCB)    do{PCB ^= 0x01;}while(0)

#define GET_PCB_BLOCKNO(PCB)    (PCB&0x01)  //��ÿ��
/*****************************************************************
* �������ƣ�
*
* ����������
*        �жϿ�����
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
#if 1
#define IF_IBLOCK(PCB)      ((PCB & 0xC0) == 0x00)   //�ж��Ƿ���I��b8b7=00
uint IF_IBLOCK_ERR(uchar PCB)
{
	if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV)
	{
		//b6=0 b4=0(CID)b3=0(NAD)b2=1
		return ((PCB & 0x2E) != 0x02);
	}
	else
	{
		// 2011-12-30 I�鲻Ҫ���ж�CID��NAD b4~b3λ
		return ((PCB & 0x22) != 0x02);
	}
}


#define IF_RBLOCK(PCB)      ((PCB & 0xC0) == 0x80)   //�ж�R��b8b7=10
uint IF_RBLOCK_ERR(uchar PCB)
{
	if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV)
	{
		//b6=0 b4=0(CID)b3=0(NAD)b2=1
		return ((PCB & 0x3E) != 0x22);
	}
	else
	{
		// 2011-12-30 R�鲻Ҫ���ж�CID  b4λ
		return ((PCB & 0x36) != 0x22);
	}
}
#define IF_R_ACK(PCB)   ((PCB & 0xF6) == 0xA2)       //�ж�ACK��
#define IF_R_NAK(PCB)   ((PCB & 0xF6) == 0xB2)       //�ж�NAK��

#define IF_SBLOCK(PCB)      ((PCB & 0xC0) == 0xC0)   //�ж��Ƿ���S�� b8b7=11
uint IF_SBLOCK_ERR(uchar PCB)
{
	if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV)
	{
		//�ж� b6b5=11(WTX) b4=0 b3=0 b2=1 b1=0
		return ((PCB & 0x3F) != 0x32);
	}
	else
	{
		// 2011-12-30 S�鲻Ҫ���ж�CID  b4λ
		return ((PCB & 0x37) != 0x32);
	}
}
#define IF_S_WTX(PCB)   ((PCB & 0xF7) == 0xF2)       //�ж�WTX��
#else
#define IF_IBLOCK(PCB)      ((PCB & 0xC0) == 0x00)   //�ж��Ƿ���I��b8b7=00
#define IF_IBLOCK_ERR(PCB)  ((PCB & 0x2E) != 0x02)   //b6=0 b4=0(CID)b3=0(NAD)b2=1

#define IF_RBLOCK(PCB)      ((PCB & 0xC0) == 0x80)   //�ж�R��b8b7=10
#define IF_RBLOCK_ERR(PCB)  ((PCB & 0x3E) != 0x22)   //b6=1 b5=0(������ACK) b4=0(CID=0) b3=0 b2=1
	#define IF_R_ACK(PCB)   ((PCB & 0xF6) == 0xA2)       //�ж�ACK��
	#define IF_R_NAK(PCB)   ((PCB & 0xF6) == 0xB2)       //�ж�NAK��

#define IF_SBLOCK(PCB)      ((PCB & 0xC0) == 0xC0)   //�ж��Ƿ���S�� b8b7=11
#define IF_SBLOCK_ERR(PCB)  ((PCB & 0x3F) != 0x32)   //�ж� b6b5=11(WTX) b4=0 b3=0 b2=1 b1=0
	#define IF_S_WTX(PCB)   ((PCB & 0xF7) == 0xF2)       //�ж�WTX��
#endif
/*****************************************************************
* �������ƣ�
*        EI_paypass_vInit(void)
* ����������
*        ��ʼ��MIF�ṹ
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
void EI_paypass_vInit(void)
{
	EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEA;
	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_NONE;
	EG_mifs_tWorkInfo.FSC = EM_mifs_FSC_Default;
	EG_mifs_tWorkInfo.FSD = EM_mifs_FSD_Default;
	EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S50;
}

/*****************************************************************
* �������ƣ�
*         EI_paypass_vSetTimer(ulong ulETU)
* ����������
*         ���ò���ʱ��
* �����º������ã�
*         ��
* �������º�����
*         ��
* ���������
*        ʵ����Ҫ��ʱ��ETU��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
void EI_paypass_vSetTimer(ulong ulETU)
{
	uchar ucTempData = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        //ע��:ulFWT����3911���Ϊ1/fc  �������Ǳ�ʾetu
        //�������ΪETU����(128/fc)  �����1/fc
        EG_mifs_tWorkInfo.ulTimeout = ulETU*128;
#endif

    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
    {

#ifdef EM_RC663_Module
        if ( ulETU <= 65535 ) {
            ucTempData = 0x80;  //1/13.56MHZ*128 == 9.43us(1/106kbit)
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_T0RELOADLO, &ucTempData);
            ucTempData = 0;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_T0RELOADHI, &ucTempData);

            ulETU--;//΢��1��etu
            ucTempData = (uchar)(ulETU&0xFF);
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_T1RELOADLO, &ucTempData);
            ucTempData = (uchar)((ulETU>>8)&0xFF);
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_T1RELOADHI, &ucTempData);
        }else{
            //����65535
        }
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x40;      //64/6.78 = 9.43us
		EI_mifs_vWriteReg(1, TPrescalerReg, &ucTempData);
		ucTempData = 0x0F;
		EI_mifs_vClrBitMask(TModeReg, ucTempData);

		ucTempData = (uchar)(ulETU&0xFF);
		EI_mifs_vWriteReg(1, TReloadVal_Lo, &ucTempData);
		ucTempData = (uchar)((ulETU>>8)&0xFF);
		EI_mifs_vWriteReg(1, TReloadVal_Hi, &ucTempData);

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		if (ulETU == EM_mifs_FWTMIN)
		{
			// ��Ҫ���ڶ�ʱ9.2ETU
			// TAutoRestart = 0, TPrescale = 8
			ucTempData = 0x03;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			// 9.2ETU = 1172CLK / 8 = 147 = 0x93
			// ucTempData = 0x99; // TReloadValue,��ʱ����λ��ʱ8Clk
			ucTempData = 0xFF;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 256)
		{
			// TAutoRestart = 0, TPrescale = 128
			ucTempData = 0x07;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) ulETU;		// TReloadValue,��ʱ����λ��ʱ1ETU
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 511)
		{
			// TAutoRestart = 0, TPrescale = 256
			ucTempData = 0x08;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 2);	// TReloadValue,��ʱ����λ��ʱ2ETU
			if ((ulETU % 2) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 1021)
		{
			// TAutoRestart = 0, TPrescale = 512
			ucTempData = 0x09;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 4);	// TReloadValue,��ʱ����λ��ʱ4ETU
			if ((ulETU % 4) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 2041)
		{
			// TAutoRestart = 0, TPrescale = 1024
			ucTempData = 0x0A;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 8);	// TReloadValue,��ʱ����λ��ʱ8ETU
			if ((ulETU % 8) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 4081)
		{
			// TAutoRestart = 0, TPrescale = 2048
			ucTempData = 0x0B;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 16);	// TReloadValue,��ʱ����λ��ʱ16ETU
			if ((ulETU % 16) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 8161)
		{
			// TAutoRestart = 0, TPrescale = 4096
			ucTempData = 0x0C;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 32);	// TReloadValue,��ʱ����λ��ʱ32ETU
			if ((ulETU % 32) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 16321)
		{
			// TAutoRestart = 0, TPrescale = 8192
			ucTempData = 0x0D;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 64);	// TReloadValue,��ʱ����λ��ʱ64ETU
			if ((ulETU % 64) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 32641)
		{
			// TAutoRestart = 0, TPrescale = 16384
			ucTempData = 0x0E;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 128);	// TReloadValue,��ʱ����λ��ʱ128ETU
			if ((ulETU % 128) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 65281)
		{
			// TAutoRestart = 0, TPrescale = 32768
			ucTempData = 0x0F;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 256);	// TReloadValue,��ʱ����λ��ʱ256ETU
			if ((ulETU % 256) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 130561)
		{
			// TAutoRestart = 0, TPrescale = 65536
			ucTempData = 0x10;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 512);	// TReloadValue,��ʱ����λ��ʱ512ETU
			if ((ulETU % 512) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 261121)
		{
			// TAutoRestart = 0, TPrescale = 131072
			ucTempData = 0x11;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 1024);	// TReloadValue,��ʱ����λ��ʱ1024ETU
			if ((ulETU % 1024) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else if (ulETU < 522241)
		{
			// TAutoRestart = 0, TPrescale = 262144
			ucTempData = 0x12;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 2048);	// TReloadValue,��ʱ����λ��ʱ2048ETU
			if ((ulETU % 2048) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}
		else
		{
			// TAutoRestart = 0, TPrescale = 524288
			ucTempData = 0x13;
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

			ucTempData = (uchar) (ulETU / 4096);	// TReloadValue,��ʱ����λ��ʱ4096ETU
			if ((ulETU % 4096) != 0)
			{
				ucTempData = ucTempData + 1;
			}
			EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
		}

#endif

	}

}

/*****************************************************************
* �������ƣ�
*         EI_paypass_vDelay(ulong ulETU)
* ����������
*         ��ʱ����
* �����º������ã�
*         ��
* �������º�����
*         ��
* ���������
*        ulETU  ��ʱ��ETU����
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
void EI_paypass_vDelay(ulong ulETU)
{
	ulong DelayTime;
	volatile uint j;
	DelayTime = ulETU * 10/100;  //1etu=9.43us

	for (j = 0; j < DelayTime; j++)
	{
		s_DelayUs(85);		// 100us
	}
}

/*****************************************************************
* �������ƣ�
*        EI_paypass_ucWUPA(uchar* pucResp)
* ����������
*        ��Pro������WUPA�����������Ӧ
* �������º�����
*        ��
* �����º������ã�
*        ��
* ���������
*        ��
* ���������
*        pucResp   ��Ƭ���ص�ATQA����
* �� �� ֵ��
*        EM_mifs_SUCCESS   �ɹ�
*        EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*        EM_mifs_TRANSERR  ͨ�Ŵ���
*        EM_mifs_TIMEOUT   ������Ӧ
*        EM_mifs_MULTIERR  �࿨��ͻ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
uchar EI_paypass_ucWUPA(uchar * pucResp)
{
	uchar ucRet = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucCMD = 0;

	// �Ƚ��ز�����ΪTypeA��
	EI_paypass_vSelectType(EM_mifs_TYPEA);
	EI_paypass_vSetTimer(EM_mifs_DEFAULT);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WUPA;
        EG_mifs_tWorkInfo.expectMaxRec = 2;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = ucTempData;
		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);
        //��ͻ��Ҫ����?
//		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, 0x80);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);

		// set TxLastBits to 7, ��֡��ʽ
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXDATANUM, 0x07);

		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(Status2Reg, 0x08);
		//cf_pn512 ��ͻλ
		ucTempData = 0x80;
		EI_mifs_vSetBitMask(CollReg, ucTempData);
		EI_mifs_vClrBitMask(TxModeReg, 0xF0);
		EI_mifs_vClrBitMask(RxModeReg, 0xF0);

		// set TxLastBits to 7, ��֡��ʽ
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, BitFramingReg, &ucTempData);

		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

        // ��ֹ RxCRC �� TxCRC , ������żУ��
        ucTempData = 0x03;
        EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

        // ��ֹcrypto1��Ԫ
        EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);

        // set TxLastBits to 7, ��֡��ʽ
        ucTempData = 0x07;
        EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);

        ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = 1;	// how many bytes to send
	if (EG_mifs_tWorkInfo.ucAnticollFlag == 1)
	{
		EG_mifs_tWorkInfo.ucDisableDF = 1;
	}
	else
	{
		EG_mifs_tWorkInfo.ucDisableDF = 0;
	}

	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_REQALL;	// WUPA����
	ucRet = EI_mifs_ucHandleCmd(ucCMD);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
	EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXDATANUM, 0x07); //������Ϻ��������ֽ�
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module
	EI_mifs_vClrBitMask(BitFramingReg, 0x07); //������Ϻ��������ֽ�
#endif

	}

	if (ucRet == EM_mifs_NOTAGERR)
	{
		return EM_mifs_TIMEOUT;
	}

	// �����������
	// �������յ����������ֽ�: ������ʶ���(2�ֽ�)
	if ((ucRet == EM_mifs_SUCCESS) && (EG_mifs_tWorkInfo.lBitsReceived != 16))
	{
		ucRet = EM_mifs_TRANSERR;
	}

	// 1. ATQA��һ���ֽڵĵ�B8��B7λ����Ϊ00 01 10֮һ������Э�������
	// 2. ATQA��һ���ֽڵĵ�B6Ϊ����Ϊ0������Э�������
	// 3. ATQA��һ���ֽڵĵ�B5��B1λ����Ϊ10000 01000 00100 00010 00001֮һ������Э�������
	// 4. ATQA�ڶ����ֽڵĵ�B8��B5λ����Ϊ0000������Э�������
	if (ucRet != EM_mifs_SUCCESS)
	{
		//�������A�� ATQA��ͬ�����ֿ�Ƭ��ͻ
		if (ucRet == EM_mifs_COLLERR)
		{
			ucRet = EM_mifs_MULTIERR;
		}
		else if(ucRet == EM_mifs_FDTERR)
		{
			//FDT������ΪЭ�����
			ucRet = EM_mifs_PROTERR;
		}
		else
		{
			// ͨ�Ŵ���
			ucRet = EM_mifs_TRANSERR;
		}
	}
	else
	{
		//�ж�D4~D0λֻ��1λ��1
		if (((EG_mifs_tWorkInfo.aucBuffer[0] & 0x1f) != 0x10)
			&& ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x1f) != 0x08)
			&& ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x1f) != 0x04)
			&& ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x1f) != 0x02)
			&& ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x1f) != 0x01))
		{
			ucRet = EM_mifs_PROTERR;	// Э�����
		}
		//D0����Ϊ0
		//QPBOC ���԰���Ҫ��
		/*
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x20) != 0x00)
		{
			ucRet = EM_mifs_PROTERR;	// Э�����
		}
		*/
		//D8D7=11
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC0) == 0xC0)
		{
			ucRet = EM_mifs_PROTERR;	// Э�����
		}

		//QPBOC ���԰���Ҫ���2�ֽ�������ֵ
		/*
		//�жϵ�2�ֽڸ�4λ�������0000
		else if ((EG_mifs_tWorkInfo.aucBuffer[1] & 0xf0) != 0x00)
		{
			ucRet = EM_mifs_PROTERR;	// Э�����
		}
		*/
		else
		{
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC0) == 0x00)
			{
				EG_mifs_tWorkInfo.ucUIDLen = 4;
			}
			else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC0) == 0x40)
			{
				EG_mifs_tWorkInfo.ucUIDLen = 7;
			}
			else
			{
				EG_mifs_tWorkInfo.ucUIDLen = 10;
			}
			memcpy(pucResp, EG_mifs_tWorkInfo.aucBuffer, 2);
		}
	}
    if ( EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911 ) {
//        EI_paypass_vDelay(EM_mifs_FDTMIN);	// ��ʱ500us�ٷ�����һ������
        s_DelayUs(EM_paypass_minFDTadtime);	// emvҪ��(���RC531) ����ʱ��պ���ʱ500us
    }
	return ucRet;
}

/**********************************************************************
* �������ƣ�
*       EI_paypass_ucAnticoll(uchar ucSEL, uchar* pucUID)
* ����������
*       Ӧ�÷���ͻ���ƻ�ȡUID��Ϣ
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ucSEL   ��ͻ�ȼ� ȡֵ���£�
*              0x93   ��һ����ͻ
*              0x95   �ڶ�����ͻ
*              0x97   ��������ͻ
* ���������
*       pucUID  ��ȡ��UID��Ϣ������BCC�ֽ�
* �� �� ֵ��
*       EM_mifs_SUCCESS    ��ȡ�ɹ�
*       EM_mifs_TRANSERR   ͨ�Ŵ���
*       EM_mifs_PROTERR    ���صĲ��������Ϲ淶
*       EM_mifs_TIMEOUT    ������Ӧ
*       EM_mifs_MULTIERR   �࿨��ͻ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
uchar EI_paypass_ucAnticoll(uchar ucSEL, uchar * pucUID)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucTempVal = 0x00;
	uchar i = 0,ucCMD = 0;

	if (EG_mifs_tWorkInfo.ucAnticollFlag == 1)
	{
		// ���÷���ͻ���ƻ�ȡUID
		result = EI_ucAnticoll(ucSEL, pucUID);
		return result;
	}
	// ���ö�ʱ��
	EI_paypass_vSetTimer(EM_mifs_DEFAULT);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITHOUT_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 5;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
		ucTempData = 0x80;
		//cf_RC663   ��ͻ��־
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, ucTempData);
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);

		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		//cf_pn512   ��ͻ��־
		EI_mifs_vSetBitMask(CollReg, ucTempData);   //cf_pn512 �Ƿ�Ҫ����
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(Status2Reg, 0x08);
		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

        // ��ͻ�����ݶ���0, Rx֡��ʽΪISO14443A
        ucTempData = 0x28;
        EI_mifs_vWriteReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);	// Zero After Coll activate
        // ��ֹcrypto1��Ԫ
        EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);
        // ÿ���ֽڴ���У��
        ucTempData = 0x03;
        EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}

	EG_mifs_tWorkInfo.aucBuffer[0] = ucSEL;	// SEL
	EG_mifs_tWorkInfo.aucBuffer[1] = 0x20;
	EG_mifs_tWorkInfo.ulSendBytes = 2;	// how many bytes to send
	// ִ������
	result = EI_mifs_ucHandleCmd(ucCMD);

/*
	if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
#ifdef EM_PN512_Module
	EI_mifs_vSetBitMask(CollReg, 0x80);  //cf_pn512 ��ͻ������ԭֵ
#endif
	}
*/

	if (result == EM_mifs_NOTAGERR)		// ��ʱ����Ӧ��ֱ�ӷ���
	{
		return EM_mifs_TIMEOUT;
	}

	if (result == EM_mifs_SUCCESS)
	{
		if (EG_mifs_tWorkInfo.lBitsReceived != 40)	// not 5 bytes answered
		{
			result = EM_mifs_TRANSERR;
		}
		else
		{
			if (EG_mifs_tWorkInfo.ucUIDLen == 4)
			{
				if (ucSEL == EM_mifs_PICC_ANTICOLL1)
				{
					if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x88)
					{
						result = EM_mifs_PROTERR;	// Э�����
					}
				}
			}
			else if (EG_mifs_tWorkInfo.ucUIDLen == 7)
			{
				if (ucSEL == EM_mifs_PICC_ANTICOLL1)
				{
					if (EG_mifs_tWorkInfo.aucBuffer[0] != 0x88)
					{
						result = EM_mifs_PROTERR;	// Э�����
					}
				}

				if (ucSEL == EM_mifs_PICC_ANTICOLL2)
				{
					if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x88)
					{
						result = EM_mifs_PROTERR;	// Э�����
					}
				}
			}
			else
			{
				if (ucSEL == EM_mifs_PICC_ANTICOLL1)
				{
					if (EG_mifs_tWorkInfo.aucBuffer[0] != 0x88)
					{
						result = EM_mifs_PROTERR;	// Э�����
					}
				}

				if (ucSEL == EM_mifs_PICC_ANTICOLL2)
				{
					if (EG_mifs_tWorkInfo.aucBuffer[0] != 0x88)
					{
						result = EM_mifs_PROTERR;	// Э�����
					}
				}
			}

			// У��BCC
			ucTempVal = 0;
			for (i = 0; i < 4; i++)
			{
				ucTempVal ^= EG_mifs_tWorkInfo.aucBuffer[i];
			}
			if (ucTempVal != EG_mifs_tWorkInfo.aucBuffer[4])
			{
				result = EM_mifs_TRANSERR;	// ͨ�Ŵ���
			}
		}

		memcpy(pucUID, EG_mifs_tWorkInfo.aucBuffer, 5);
	}
	else if (result == EM_mifs_COLLERR)
	{
		result = EM_mifs_MULTIERR;
	}
	else
	{
		result = EM_mifs_TRANSERR;
	}

    if ( EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911 ) {
//        EI_paypass_vDelay(EM_mifs_FDTMIN);	// ��ʱ500us�ٷ�����һ������
        s_DelayUs(EM_paypass_minFDTadtime);	// emvҪ��(���RC531) ����ʱ��պ���ʱ500us
    }
	return result;
}

/*******************************************************************
* �������ƣ�
*        EI_paypass_ucSelect(uchar ucSEL, uchar* pucUID, uchar* pucSAK)
* ����������
*        ����UID��Ϣѡ��Pro��
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*       ucSEL     UID�ȼ���ȡֵ���£�
*                 0x93        ��һ��
*                 0x95        �ڶ���
*                 0x97        ������
*       pucUID   UID������5���ֽ�
* ���������
*		pucSAK     Pro�����͵�SAK��Ϣ
* �� �� ֵ��
*       EM_mifs_SUCCESS   ��ȡ�ɹ�
*       EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*       EM_mifs_TRANSERR  ͨ�Ŵ���
*       EM_mifs_TIMEOUT   ������Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
uchar EI_paypass_ucSelect(uchar ucSEL, uchar * pucUID, uchar * pucSAK)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucCMD = 0;

	// ���ö�ʱ��
	EI_paypass_vSetTimer(EM_mifs_DEFAULT);
	// RxCRC,TxCRC, Odd Parity enable

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 1+2;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = ucTempData;
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vSetBitMask(RxModeReg, ucTempData);

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(Status2Reg, 0x08);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

        ucTempData = 0x0F;
        EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

        // ��ֹcrypto1��Ԫ
        EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);	// disable crypto 1 unit
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}

	EG_mifs_tWorkInfo.ulSendBytes = 7;
	EG_mifs_tWorkInfo.ucDisableDF = 0;

	// ����UID����
	memcpy(EG_mifs_tWorkInfo.aucBuffer + 2, pucUID, 5);

	EG_mifs_tWorkInfo.aucBuffer[0] = ucSEL;
	EG_mifs_tWorkInfo.aucBuffer[1] = 0x70;	// number of bytes send

	// �������������Ӧ
	result = EI_mifs_ucHandleCmd(ucCMD);
	if (result == EM_mifs_NOTAGERR)		// ��ʱ����Ӧ��ֱ�ӷ���
	{
		return EM_mifs_TIMEOUT;
	}
	else if(result == EM_mifs_FDTERR)
	{
		//FDT������ΪЭ�����
		result = EM_mifs_PROTERR;
	}
	if (result != EM_mifs_SUCCESS)
	{
		result = EM_mifs_TRANSERR;		// ����ͨ�Ŵ���
	}
	else
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module
            ;
#endif
        }
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {
#ifdef EM_RC663_Module
#endif
        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{
#ifdef EM_PN512_Module
#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{
#ifdef EM_RC531_Module
			EI_mifs_vReadReg(1, EM_mifs_REG_TIMERVALUE, &ucTempData);
#endif
		}
		if (EG_mifs_tWorkInfo.lBitsReceived != 8)
		{
			// һ���ֽ�û������
			result = EM_mifs_TRANSERR;
		}
		else
		{
			// B3λ�ж�UID�Ƿ�����
			if (EG_mifs_tWorkInfo.ucUIDLen == 4)
			{
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x04) == 0x04)
				{
					result = EM_mifs_PROTERR;
				}
				EG_mifs_tWorkInfo.ucSAK1 = EG_mifs_tWorkInfo.aucBuffer[0];
			}
			else if (EG_mifs_tWorkInfo.ucUIDLen == 7)
			{
				if (ucSEL == EM_mifs_PICC_ANTICOLL1)
				{
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x04) != 0x04)
					{
						result = EM_mifs_PROTERR;
					}
					EG_mifs_tWorkInfo.ucSAK1 = EG_mifs_tWorkInfo.aucBuffer[0];
				}

				if (ucSEL == EM_mifs_PICC_ANTICOLL2)
				{
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x04) == 0x04)
					{
						result = EM_mifs_PROTERR;
					}
					EG_mifs_tWorkInfo.ucSAK2 = EG_mifs_tWorkInfo.aucBuffer[0];
				}
			}
			else						// EG_mifs_tWorkInfo.ucUIDLen == 10
			{
				if (ucSEL == EM_mifs_PICC_ANTICOLL1)
				{
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x04) != 0x04)
					{
						result = EM_mifs_PROTERR;
					}
					EG_mifs_tWorkInfo.ucSAK1 = EG_mifs_tWorkInfo.aucBuffer[0];
				}

				if (ucSEL == EM_mifs_PICC_ANTICOLL2)
				{
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x04) != 0x04)
					{
						result = EM_mifs_PROTERR;
					}
					EG_mifs_tWorkInfo.ucSAK2 = EG_mifs_tWorkInfo.aucBuffer[0];
				}

				if (ucSEL == EM_mifs_PICC_ANTICOLL3)
				{
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x04) == 0x04)
					{
						result = EM_mifs_PROTERR;
					}
					EG_mifs_tWorkInfo.ucSAK3 = EG_mifs_tWorkInfo.aucBuffer[0];
				}
			}
		}
	}
	*pucSAK = EG_mifs_tWorkInfo.aucBuffer[0];
    if ( EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911 ) {
//        EI_paypass_vDelay(EM_mifs_FDTMIN);	// ��ʱ500us�ٷ�����һ������
        s_DelayUs(EM_paypass_minFDTadtime);	// emvҪ��(���RC531) ����ʱ��պ���ʱ500us
    }
	return result;
}

/***************************************************************
* �������ƣ�
*        EI_paypass_ucRATS(uchar* pucOutLen, uchar* pucATSData)
* ����������
*        ��Pro������RATS�������ATS��Ӧ
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*       ��
* ���������
*       pucOutLen   Pro�����ص�ATS����
*       pucATSData  Pro�����ص�ATS����
* �� �� ֵ��
*       EM_mifs_SUCCESS   ��ȡ�ɹ�
*       EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*       EM_mifs_COMMERR   ͨ�Ŵ���
*       EM_mifs_TIMEOUT   ������Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
uchar EI_paypass_ucRATS(uchar * pucOutLen, uchar * pucATSData)
{
	uchar ucTempData = 0;
	uchar result;
	uchar tucTemp[2];
	uchar i = 0;
	uchar ucCMD = 0;
RATSSTART:
	result = EM_SUCCESS;
	*pucOutLen = 0;
	EG_mifs_tWorkInfo.FSC = EM_mifs_FSC_Default;
	EG_mifs_tWorkInfo.FSD = EM_mifs_FSD_Default;
	EG_mifs_tWorkInfo.ucFWI = 4;
	EG_mifs_tWorkInfo.ulFWT = s_rfid_FWI2FWT(EG_mifs_tWorkInfo.ucFWI);
	EG_mifs_tWorkInfo.ucSFGI = 0;
	// PayPassҪ�󣬵�SFGI=0��û��ʱ��Ҫ�����ӳ�(6780/128)etu=500us
	EG_mifs_tWorkInfo.ulSFGT = 60;
	// Ĭ�ϲ�����CID��NAD
	EG_mifs_tWorkInfo.ucCIDFlag = 0;
	EG_mifs_tWorkInfo.ucNADFlag = 0;

	tucTemp[0] = 0xE0;
	//yehf 2010-09-03 Level-1Ҫ���޸�,FSD = 256Bytes ��֧��CID
	tucTemp[1] = 0x80;					//0x70;

	// ���ö�ʱ��
	EI_paypass_vSetTimer(EM_mifs_FWTRATS);	// ��ʱ560ETU
	// RxCRC,TxCRC, Parity enable

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        //emv�淶20+2(crc)�ֽ�
        EG_mifs_tWorkInfo.expectMaxRec = sizeof(EG_mifs_tWorkInfo.aucBuffer);//20+2;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vSetBitMask(RxModeReg, ucTempData);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		ucTempData = 0x0F;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = 2;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	memcpy(EG_mifs_tWorkInfo.aucBuffer, tucTemp, EG_mifs_tWorkInfo.ulSendBytes);

	// �������������Ӧ
	result = EI_mifs_ucHandleCmd(ucCMD);

	if (result == EM_mifs_NOTAGERR)		// ��ʱ����Ӧ��ֱ�ӷ���
	{
		return EM_mifs_TIMEOUT;
	}
	else if(result == EM_mifs_FDTERR)
	{
		//FDT������ΪЭ�����
		result = EM_mifs_PROTERR;
	}
	if (result == EM_mifs_SUCCESS)
	{
		i = 0;
		if (EG_mifs_tWorkInfo.ulBytesReceived != EG_mifs_tWorkInfo.aucBuffer[0])	// TL�ֽڴ���
		{
			result = EM_mifs_TRANSERR;	// ͨ�Ŵ���
			goto RATSEND;
		}

		// ����ATS����
		*pucOutLen = EG_mifs_tWorkInfo.aucBuffer[0];
		memcpy(pucATSData, EG_mifs_tWorkInfo.aucBuffer, EG_mifs_tWorkInfo.ulBytesReceived);
//		if(EG_mifs_tWorkInfo.aucBuffer[0] > 20 || EG_mifs_tWorkInfo.aucBuffer[0] == 0)
		if(EG_mifs_tWorkInfo.aucBuffer[0] == 0)//��ʱ���ж�TL����12162015 chenf
		{
			//PBOC-Level1Э��涨TL������20,�Ҵ���0
			result = EM_mifs_PROTERR;	// Э�����
			goto RATSEND;
		}
		else if (EG_mifs_tWorkInfo.aucBuffer[0] == 1)
		{
			//ֻ����TL�ֽ�,������T0��TA1��TB1����TC1����Ҫ���ط�
			goto RATSEND;
		}

		i = 1;							// ȡT0�ֽ�
		// T0�ֽڶ���FSC��TA1��TB1��TC1�Ĵ������
		if ((EG_mifs_tWorkInfo.aucBuffer[1] & 0x80) != 0x00)	// Bit8����Ϊ0
		{
			result = EM_mifs_PROTERR;	// Э�����
			goto RATSEND;
		}
		else
		{
			//��ȡFSC
			EG_mifs_tWorkInfo.FSC = EI_rfid_GetFramesize(EG_mifs_tWorkInfo.aucBuffer[1] & 0x0F);
			EG_mifs_tWorkInfo.ulMaxLen = EG_mifs_tWorkInfo.FSC;
		}
		// TA1�ֽڶ����豸�뿨֮���ͨ�����ʣ�Ĭ��Ϊ106KBits/s
		// ����TA1�����κ�ֵ������оƬ�뿨Ƭ֮���԰�106KBits/s����ͨ��
		if ((EG_mifs_tWorkInfo.aucBuffer[1] & 0x10) == 0x10)	// TA1�ֽ�
		{
			i++;
			//QPBOC ���԰���Ҫ��88��08����ͨ��
			/*
			if ((EG_mifs_tWorkInfo.aucBuffer[i] & 0x08) != 0x00)
			{
				result = EM_mifs_PROTERR;	// Э�����
				goto RATSEND;
			}
			*/
		}
		// TB1�ֽڶ���FWT��SFGT
		if ((EG_mifs_tWorkInfo.aucBuffer[1] & 0x20) == 0x20)	// TB1�ֽ�
		{
			i++;
			EG_mifs_tWorkInfo.ucFWI = (EG_mifs_tWorkInfo.aucBuffer[i] & 0x0F0) >> 4;
			EG_mifs_tWorkInfo.ucSFGI = EG_mifs_tWorkInfo.aucBuffer[i] & 0x0F;
			if(EG_mifs_tWorkInfo.ucFWI == 0x0F || EG_mifs_tWorkInfo.ucSFGI == 0x0F)
			{
				//FWI��SFGI��Χ0~14,15Ԥ��
				result = EM_mifs_PROTERR;	// Э�����
				goto RATSEND;
			}
			EG_mifs_tWorkInfo.ulFWT = s_rfid_FWI2FWT(EG_mifs_tWorkInfo.ucFWI);
 			if(EG_mifs_tWorkInfo.ucSFGI != 0)
			{
				//SFGT=(32 + 3)*2^SFGI  etu
				EG_mifs_tWorkInfo.ulSFGT = s_rfid_FWI2FWT(EG_mifs_tWorkInfo.ucSFGI);
			}
		}
		else
		{
			// TB1û�� ����ȱʡFWI=4  SFGI=0
			EG_mifs_tWorkInfo.ucFWI = 4;
			EG_mifs_tWorkInfo.ulFWT = s_rfid_FWI2FWT(EG_mifs_tWorkInfo.ucFWI);
			EG_mifs_tWorkInfo.ucSFGI = 60;
		}
		if ((EG_mifs_tWorkInfo.aucBuffer[1] & 0x40) == 0x40)	// TC1
		{
			i++;
			// QPBOC ���԰���TA103.0��TA103.2��Ҫ�����
			/*
			// TC1�ĸ�6λ����Ϊ000000
			if ((EG_mifs_tWorkInfo.aucBuffer[i] & 0xFC) != 0x00)
			{
				result = EM_mifs_PROTERR;	// Э�����
				goto RATSEND;
			}
			*/
			// �ж�CID
			if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_CID)
			{
				if (EG_mifs_tWorkInfo.aucBuffer[i] & 0x02)
				{
					EG_mifs_tWorkInfo.ucCIDFlag = 1;
				}
			}
			if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_NAD)
            {
				// �ж�NAD
				if (EG_mifs_tWorkInfo.aucBuffer[i] & 0x01)
				{
					EG_mifs_tWorkInfo.ucNADFlag = 1;
				}
            }
			result = EM_mifs_SUCCESS;
		}
	}
	else if(result != EM_mifs_NOTAGERR)
	{
		if(EG_mifs_tWorkInfo.lBitsReceived/8<4 || EG_mifs_tWorkInfo.lBitsReceived%8 != 0)
		{
			EI_paypass_vDelay(EG_mifs_tWorkInfo.ulSFGT*5);
			goto RATSSTART;
		}
	}
  RATSEND:
//	EI_paypass_vDelay(EG_mifs_tWorkInfo.ulSFGT+100);	// ��ʱSFGT�ٷ�����һ������
	EI_paypass_vDelay(EG_mifs_tWorkInfo.ulSFGT+10);	// emvҪ��(���RC531) ����ʱ��
	return result;
}

/**********************************************************************
* �������ƣ�
*       EI_paypass_vHALTA(void)
* ����������
*       ��Pro����ΪHALT
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       ��
* �� �� ֵ��
*       ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
void EI_paypass_vHALTA(void)
{
	uchar ucTempData;
	uchar ucCMD = 0;
	uchar ucRegAddr = 0;;

	// ��ʱ��ʱ������
	EI_paypass_vSetTimer(60);			// ��ʱ560ETU

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 0;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
		ucRegAddr = PHHAL_HW_RC663_REG_COMMAND;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, 0x80);
		EI_mifs_vSetBitMask(RxModeReg, 0x80);
		ucCMD = PN512CMD_TRANSCEIVE;
		ucRegAddr = CommandReg;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// TxCRC, Parity enable
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
		ucRegAddr = EM_mifs_REG_COMMAND;

#endif

	}


	// ׼����������: ֹͣ������, 0
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_HALT;	// Halt command code
	EG_mifs_tWorkInfo.aucBuffer[1] = 0x00;
	// dummy address
	// ִ������
    gtRfidProInfo.Cmd = EM_mifs_PICC_HALT;
	EI_mifs_ucHandleCmd(ucCMD);
    gtRfidProInfo.Cmd = 0;

	// ��PCD������HLTA��ʼ����ΪPICC�ѳɹ����ղ���ȷִ����HLTA���������ȥ���
	// PICC������κ���Ӧ��

	// ��λ����Ĵ���
	if(EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911)
    {
        ucTempData = 0x00;
        EI_mifs_vWriteReg(1, ucRegAddr, &ucTempData);
    }
}

/**********************************************************************
* �������ƣ�
*       EI_paypass_vResetPICC(void)
* ����������
*       �ر�RC531�ز�����λ��Ӧ�������е�PICC
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       ��
* �� �� ֵ��
*       ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
void EI_paypass_vResetPICC(void)
{
	uchar data = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        data = data;
        as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xD0);
//        emvHalActivateField(OFF);
		s_DelayMs(10);
        emvHalActivateField(ON);
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        data = (uchar)PHHAL_HW_RC663_BIT_TXEN;
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_DRVMODE, data);
		s_DelayMs(10);
        EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_DRVMODE, data);
		s_DelayMs(10);
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		data = 0x03;
		// ʹRC531����̶���ƽ
		EI_mifs_vClrBitMask(TxControlReg, data);
		s_DelayMs(10);
		EI_mifs_vSetBitMask(TxControlReg, data);
		s_DelayMs(10);  //PN512 ���ز���Ҫ��ʱ �ȶ�ϵͳ

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

	//S980֧�ֵ͹���ģʽ
#if (defined(PRODUCT_F16_2))
	data = 0x00;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CONTROL, &data);	//��������ģʽ
#endif
	// ʹRC531����̶���ƽ
	EI_mifs_vClrBitMask(EM_mifs_REG_TXCONTROL, 0x03);
	s_DelayMs(10);	// TIME_WAIT_TReset �ر��ز�1000ETU Treset=5.1~10ms
	// ��Ҫ����ԭ����
	EI_mifs_vSetBitMask(EM_mifs_REG_TXCONTROL, 0x03);

#endif

    }
	if((EG_mifs_tWorkInfo.ucAnticollFlag == 0)
        || (EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))  //EMVģʽ
		s_DelayMs(6);
	else
		s_DelayMs(EG_mifs_tWorkInfo.PwrAdTime); //ISO�ز�������ʱ Ĭ��15ms (qpboc 5-10ms) cf02282012
}

/**********************************************************************
* �������ƣ�
*       EI_paypass_vOptField(uchar mode)
* ����������
*       ����ģ���ز�����λ��Ӧ�������е�PICC
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       ��
* �� �� ֵ��
*       ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
void EI_paypass_vOptField(uchar mode)
{
    uchar data = 0;

    EI_paypass_vSelectType(EM_mifs_TYPEA);
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        emvHalActivateField(mode);
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        data = (uchar)PHHAL_HW_RC663_BIT_TXEN;
        if ( mode == ON ) {
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_DRVMODE, data);
        }else
            EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_DRVMODE, data);
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		data = 0x03;
		// ʹRC531����̶���ƽ
        if ( mode == ON ) {
            EI_mifs_vSetBitMask(TxControlReg, data);
        }else
            EI_mifs_vClrBitMask(TxControlReg, data);
		s_DelayMs(10);  //PN512 ���ز���Ҫ��ʱ �ȶ�ϵͳ

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

	//S980֧�ֵ͹���ģʽ
#if (defined(PRODUCT_F16_2))
	data = 0x00;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CONTROL, &data);	//��������ģʽ
#endif
	// ʹRC531����̶���ƽ
    if ( mode == ON ) {
        EI_mifs_vSetBitMask(EM_mifs_REG_TXCONTROL, 0x03);
    }else
        EI_mifs_vClrBitMask(EM_mifs_REG_TXCONTROL, 0x03);

#endif

    }
    s_DelayUs(EM_paypass_polltime);
}
/*****************************************************************
* �������ƣ�
*     EI_paypass_ucWUPB(uchar* pucOutLen, uchar* pcuOut)
* ����������
*     ����TypeB��
* �����º������ã�
*     ��
* �������º�����
*     ��
* ���������
*     ��
* ���������
*     ��
* �� �� ֵ��
*        EM_mifs_SUCCESS   �ɹ�
*        EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*        EM_mifs_TRANSERR  ͨ�Ŵ���
*        EM_mifs_TIMEOUT   ������Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
uchar EI_paypass_ucWUPB(uchar * pucOutLen, uchar * pucOut)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0;
	uchar ucCMD = 0;
	EI_mifs_vFlushFIFO();				//empty FIFO
	// ���ز�����ΪTypeB��
	EI_paypass_vSelectType(EM_mifs_TYPEB);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 13+2;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetBitMask(ManualRCVReg, 0x10);  //��ֹ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);  //CRC
		EI_mifs_vSetBitMask(RxModeReg, ucTempData);  //CRC
		EI_mifs_vClrBitMask(Status2Reg, 0x08);

		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// RxCRC and TxCRC enable, parity
		ucTempData = 0x2C;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

		EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);	// disable crypto 1 unit

		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}

//	EI_paypass_vSetTimer(3 * EM_mifs_FWTWUPB);	// ��ʱ�ȴ�180ETU
    EI_paypass_vSetTimer(65);	// ��ʱ�ȴ�65*128/fc //emv chenf 12132013

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = 3;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_REQB;	// ��������
	EG_mifs_tWorkInfo.aucBuffer[1] = 0x00;	// AFI = 0x00
	// EMVҪ��PCD��֧����չATQB b5=0  0x08
	EG_mifs_tWorkInfo.aucBuffer[2] = 0x08;// TimeSlot = 0, WUPB

	// ִ������
	result = EI_mifs_ucHandleCmd(ucCMD);

	if (result == EM_mifs_NOTAGERR)		// ��ʱ����Ӧ
	{
		return EM_mifs_TIMEOUT;
	}
	if (result != EM_mifs_SUCCESS)
	{
		result = EM_mifs_TRANSERR;
	}
	else
	{
		// ����12��13���ֽ�
		if (EG_mifs_tWorkInfo.ulBytesReceived != 12 && EG_mifs_tWorkInfo.ulBytesReceived != 13)
		{
			result = EM_mifs_TRANSERR;
			goto WUPBEND;
		}
		if (EG_mifs_tWorkInfo.aucBuffer[0] != 0x50)
		{
			result = EM_mifs_PROTERR;
			goto WUPBEND;
		}

		// ��6~9�ֽ�ΪApplication Data,һ��Ϊ0000���ն˿��Բ������

		// ��10�ֽ�ΪBitRate��PCD��PICCһ��ֻ֧��106KBit
		// ���ܸ��ֽڻ����κ�ֵ������106KBits/s����ͨ��
		// QPBOC����Ҫ��Ҫ�ж�
		/*
		if ((EG_mifs_tWorkInfo.aucBuffer[9] & 0x08) != 0x00)
		{
			result = EM_mifs_PROTERR;
			goto WUPBEND;
		}
		*/
		// ��11�ֽ�(MaxFrameSize Protocol Type)����֡��С��Э�� ��4λΪ֡��С����4λΪЭ��ֵ
		EG_mifs_tWorkInfo.FSC = EI_rfid_GetFramesize(EG_mifs_tWorkInfo.aucBuffer[10]>>4);
		EG_mifs_tWorkInfo.ulMaxLen = EG_mifs_tWorkInfo.FSC;
		// TR2���� ��4λ��ʾ�Ƿ�֧��ISO14443Э�飬Ĭ��֧��
		//if ((EG_mifs_tWorkInfo.aucBuffer[10] & 0x09) != 0x01)
		// QPBOC ����Ҫ��Protocol_type��D3λ��Ҫ�ж�
		if ((EG_mifs_tWorkInfo.aucBuffer[10] & 0x01) != 0x01)
		{
			result = EM_mifs_PROTERR;
			goto WUPBEND;
		}

		// ��12�ֽڰ���FWI��CID��NAD����Ϣ
		// ��4λ����FWI
		EG_mifs_tWorkInfo.ucFWI = (EG_mifs_tWorkInfo.aucBuffer[11] & 0x0F0) >> 4;
		if(EG_mifs_tWorkInfo.ucFWI == 0x0F)
		{
			//���ܵ���0F
			result = EM_mifs_PROTERR;
			goto WUPBEND;
		}
		EG_mifs_tWorkInfo.ulFWT = s_rfid_FWI2FWT(EG_mifs_tWorkInfo.ucFWI);


		// ��3~4λ��ʾADC�����迼��
		// ��1~2λ��ʾCID��NAD���ƣ�����PICC����ʲô��������֧�ִ���
		// �ж�NAD
		/*
		if (EG_mifs_tWorkInfo.aucBuffer[11] & 0x01)
		{
			EG_mifs_tWorkInfo.ucCIDFlag = 1;
		}
		else
		{
			EG_mifs_tWorkInfo.ucCIDFlag = 0;
		}
		// �ж�CID
		if (EG_mifs_tWorkInfo.aucBuffer[11] & 0x02)
		{
			EG_mifs_tWorkInfo.ucNADFlag = 1;
		}
		else
		{
			EG_mifs_tWorkInfo.ucNADFlag = 0;
		}
		*/
		*pucOutLen = (uchar) EG_mifs_tWorkInfo.ulBytesReceived;
		memcpy(pucOut, EG_mifs_tWorkInfo.aucBuffer, EG_mifs_tWorkInfo.ulBytesReceived);

		EG_mifs_tWorkInfo.ucUIDLen = 4;
		// ����TypeB��UID���к� ��2~5�ֽ�
		memcpy(EG_mifs_tWorkInfo.ucUIDB, EG_mifs_tWorkInfo.aucBuffer + 1, 4);
	}

  WUPBEND:

    if ( EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911 ) {
//        EI_paypass_vDelay(EM_mifs_FDTMIN);	// ��ʱ500us�ٷ�����һ������
        s_DelayUs(EM_paypass_minFDTadtime);	// emvҪ��(���RC531) ����ʱ��պ���ʱ500us
    }

	return result;
}

/***********************************************************
* �������ƣ�
*        EI_paypass_ucAttrib(uchar* pucResp)
* ����������
*        ѡ��TypeB��
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        pucResp   TypeB�����ص�����
* �� �� ֵ��
*        EM_mifs_SUCCESS   �ɹ�
*        EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*        EM_mifs_TRANSERR  ͨ�Ŵ���
*        EM_mifs_TIMEOUT   ������Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
uchar EI_paypass_ucAttrib(uchar * pucResp)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucCMD = 0;
EI_paypass_ucAttrib_start:
	EI_mifs_vFlushFIFO();				//empty FIFO

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 1+2;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = ucTempData;
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);//disable parity
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vSetBitMask(RxModeReg, ucTempData);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif
	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
#ifdef EM_RC531_Module

		// RxCRC and TxCRC enable, parity
		ucTempData = 0x2C;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif
	}

	EI_paypass_vSetTimer(EG_mifs_tWorkInfo.ulFWT);	// ���ó�ʱʱ��

	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_ATTRIB;	// Attrib������
	// ��2~5�ֽ�ΪPUPI
	EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucUIDB[0];
	EG_mifs_tWorkInfo.aucBuffer[2] = EG_mifs_tWorkInfo.ucUIDB[1];
	EG_mifs_tWorkInfo.aucBuffer[3] = EG_mifs_tWorkInfo.ucUIDB[2];
	EG_mifs_tWorkInfo.aucBuffer[4] = EG_mifs_tWorkInfo.ucUIDB[3];
	// ��6�ֽ�Ϊparam1������TR0��TR1����SOF��EOF
	// PAYPASSҪ�󣺲���Ĭ��TR0��TR1��֧��SOF��EOF
	EG_mifs_tWorkInfo.aucBuffer[5] = 0x00;
	// ��7�ֽ�Ϊparam2������BitRate��Frame Size
	// PAYPASSҪ��BitRateΪ106KBits��Frame Size = 256Bytes
	EG_mifs_tWorkInfo.aucBuffer[6] = 0x08;
	// ��8�ֽ�Ϊparam3������PCD�Ƿ�֧��ISO14443-4
	// PAYPASSҪ���ܹ�֧��ISO14443-4
	EG_mifs_tWorkInfo.aucBuffer[7] = 0x01;
	// ��9�ֽ�Ϊparam4��������CID
	// PAYPASSҪ��֧��CID
	EG_mifs_tWorkInfo.aucBuffer[8] = 0x00;

	EG_mifs_tWorkInfo.ulSendBytes = 9;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;

	result = EI_mifs_ucHandleCmd(ucCMD);

	if (result == EM_mifs_NOTAGERR)
	{
		// ��ʱ����Ӧ��ֱ�ӷ���
		return EM_mifs_TIMEOUT;
	}
	// 2010-12-19  QPBOC TB305���CRC����֡����У��������ط����3��
	// QPBOC TB306_12 00��CRC_B
	else if(result == EM_mifs_PARITYERR || result == EM_mifs_CRCERR || result == EM_mifs_FRAMINGERR)
	{
		if(EG_mifs_tWorkInfo.ulBytesReceived == 1 && result == EM_mifs_CRCERR)
		{
			result = EM_mifs_TRANSERR;
			EI_paypass_vDelay(EM_mifs_FDTMIN);
			return result;
		}
		EI_paypass_vDelay(EM_mifs_FDTMIN);
		goto EI_paypass_ucAttrib_start;
	}
	else if (result != EM_mifs_SUCCESS)
	{
		// ���������ͨ�Ŵ�����
		result = EM_mifs_TRANSERR;
	}
	else
	{
		//��Ӧ��С1B����
		if (EG_mifs_tWorkInfo.ulBytesReceived != 1)
		{
			result = EM_mifs_TRANSERR;
		}
		else
		{
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x0F) != 0x00)
			{
				// CID����Ϊ0
				result = EM_mifs_PROTERR;
			}
		}

		*pucResp = EG_mifs_tWorkInfo.aucBuffer[0];
	}

	EI_paypass_vDelay(EM_mifs_FDTMIN);	// ��ʱ500us�ٷ�����һ������
	return result;
}

/******************************************************
* �������ƣ�
*      EI_paypass_ucHALTB(uchar* pucResp)
* ����������
*      ��TypeB����ΪHALT̬
* �����º������ã�
*      ��
* �������º�����
*      ��
* ���������
*      ��
* ���������
*      pucResp   TypeB���ص�����
* �� �� ֵ
*        EM_mifs_SUCCESS   �ɹ�
*        EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*        EM_mifs_TRANSERR  ͨ�Ŵ���
*        EM_mifs_TIMEOUT   ������Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
*******************************************************************/
uchar EI_paypass_ucHALTB(uchar * pucResp)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucCMD = 0;

	EI_mifs_vFlushFIFO();				// empty FIFO

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 1+2; //00 + CRC
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = ucTempData;
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);//disable parity
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vSetBitMask(RxModeReg, ucTempData);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif
	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
#ifdef EM_RC531_Module

		// RxCRC and TxCRC enable, parity
		ucTempData = 0x2C;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif
	}

	EI_paypass_vSetTimer(EG_mifs_tWorkInfo.ulFWT);	// ���ó�ʱʱ��

	// ׼����������: �����룫PUPI
	EG_mifs_tWorkInfo.ulSendBytes = 5;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_HALT;	// HALT������
	// ��2~5�ֽ�ΪPUPI
	EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucUIDB[0];
	EG_mifs_tWorkInfo.aucBuffer[2] = EG_mifs_tWorkInfo.ucUIDB[1];
	EG_mifs_tWorkInfo.aucBuffer[3] = EG_mifs_tWorkInfo.ucUIDB[2];
	EG_mifs_tWorkInfo.aucBuffer[4] = EG_mifs_tWorkInfo.ucUIDB[3];

	result = EI_mifs_ucHandleCmd(ucCMD);

	if (result == EM_mifs_SUCCESS)
	{
		if (EG_mifs_tWorkInfo.ulBytesReceived != 1)
		{
			result = EM_mifs_TRANSERR;
		}
		else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0)
		{
			*pucResp = EG_mifs_tWorkInfo.aucBuffer[0];
		}
		else
		{
			result = EM_mifs_PROTERR;
		}
	}
	else if (result == EM_mifs_NOTAGERR)
	{
		result = EM_mifs_TIMEOUT;
	}
	else
	{
		result = EM_mifs_TRANSERR;
	}

	EI_paypass_vDelay(EM_mifs_FDTMIN);	// ��ʱ500us�ٷ�����һ������
	return result;
}

/***********************************************************
* �������ƣ�
*       EI_paypass_ucGetUID(void)
* ����������
*       TypeA�����÷���ͻ���ƣ���ȡUID
* �����º������ã�
*       ��
* �������º�����
*       EI_paypass_ucAnticoll
*       EI_paypass_ucSelect
* ���������
*       ��
* ���������
*       ��
* �� �� ֵ��
*       EM_mifs_SUCCESS   ��ȡ�ɹ�
*       EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*       EM_mifs_TRANSERR  ͨ�Ŵ���
*       EM_mifs_TIMEOUT   ������Ӧ
*       EM_mifs_MULTIERR  �࿨��ͻ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
* ˵    ����
*
*******************************************************************/
uchar EI_paypass_ucGetUID(void)
{
#if 0
	uint i,j,ucReSendNo;
	uchar cmd;
	uchar result = EM_mifs_SUCCESS;
	uchar ucSAK = 0x00;
	uchar ucUIDTemp[6];

	if(EG_mifs_tWorkInfo.ucUIDLen == 4)
	{
		j = 1;
	}
	else if(EG_mifs_tWorkInfo.ucUIDLen == 7)
	{
		j = 2;
	}
	else
	{
		j = 3;
	}
    for(i=0;i<j;i++)
    {
    	if(i == 0)
    	{
    		cmd = EM_mifs_PICC_ANTICOLL1;
    	}
		else if(i == 1)
		{
			cmd = EM_mifs_PICC_ANTICOLL2;
		}
		else
		{
			cmd = EM_mifs_PICC_ANTICOLL3;
		}
    	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			result = EI_paypass_ucAnticoll(cmd, ucUIDTemp);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
		}
		if (result != EM_mifs_SUCCESS)
		{
			//Anticoll�����˳�
			return result;
		}
		if(i == 0)
    	{
    		// �����1��UID����
			memcpy(EG_mifs_tWorkInfo.ucUIDCL1, ucUIDTemp, 5);
    	}
		else if(i == 1)
		{
			// �����2��UID����
			memcpy(EG_mifs_tWorkInfo.ucUIDCL2, ucUIDTemp, 5);
		}
		else
		{
			// �����3��UID����
			memcpy(EG_mifs_tWorkInfo.ucUIDCL3, ucUIDTemp, 5);
		}

		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			result = EI_paypass_ucSelect(cmd, ucUIDTemp, &ucSAK);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
		}

		if (result != EM_mifs_SUCCESS)
		{
			//Select�����˳�
			return result;
		}
    }
	//�ɹ���ȡSAK
	//b5=1ʱ����ISO14443-3Э��
	if ((ucSAK & 0x20) != 0x20)
	{
		if ((ucSAK & 0x18) == 0x08)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S50;
		}
		else if ((ucSAK & 0x18) == 0x18)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S70;
		}
		else
		{
			return EM_mifs_TRANSERR;
		}
	}
	else
	{
		if ((ucSAK & 0x18) == 0x08)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S50;
		}
		else if ((ucSAK & 0x18) == 0x18)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S70;
		}
		else
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PROCARD;
		}
	}
	return EM_mifs_SUCCESS;
#else

	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucUIDTemp[5];
	uchar ucSAK = 0x00;
	uchar ucReSendNo = 0x00;

	memset(ucUIDTemp, 0x00, sizeof(ucUIDTemp));

	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		ucTempData = EM_mifs_PICC_ANTICOLL1;	// ��һ����ͻ
		result = EI_paypass_ucAnticoll(ucTempData, ucUIDTemp);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		result = EI_paypass_ucSelect(ucTempData, ucUIDTemp, &ucSAK);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	// �����һ��UID����
	memcpy(EG_mifs_tWorkInfo.ucUIDCL1, ucUIDTemp, 5);
	if (EG_mifs_tWorkInfo.ucUIDLen == 4)
	{
		memcpy((uchar *)&EG_mifs_tWorkInfo.ulCardID,ucUIDTemp,4);  //MIFARE��/CPU+M1����֤ʹ��
		if ((ucSAK & 0x20) != 0x20)
		{
			//b5=0 ������ISO14443-3Э�� M1������Ҫ�ж��Ƿ�ΪPro��
			// 2011-12-31  yehf �޸�
			if ((ucSAK & 0x18) == 0x18)
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S70;
				//memcpy((uchar *)&EG_mifs_tWorkInfo.ulCardID,ucUIDTemp,4);
			}
			else
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S50;
				//memcpy((uchar *)&EG_mifs_tWorkInfo.ulCardID,ucUIDTemp,4);
			}
		}

		else
		{
			// b5=1 ����ISO14443-3�淶ΪPro��
			if ((ucSAK & 0x18) == 0x08)
			{
				// S50+Pro
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S50;

			}
			else if ((ucSAK & 0x18) == 0x18)
			{
				// S70+Pro
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S70;

			}
			else
			{
				// Pro
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PROCARD;
			}
		}
		return EM_mifs_SUCCESS;
	}

	// ��ȡ�ڶ���UID����
	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		ucTempData = EM_mifs_PICC_ANTICOLL2;	// �ڶ�����ͻ
		result = EI_paypass_ucAnticoll(ucTempData, ucUIDTemp);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		result = EI_paypass_ucSelect(ucTempData, ucUIDTemp, &ucSAK);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}
	// ����ڶ���UID����
	memcpy(EG_mifs_tWorkInfo.ucUIDCL2, ucUIDTemp, 5);

	if (EG_mifs_tWorkInfo.ucUIDLen == 7)
	{
		if ((ucSAK & 0x20) != 0x20)
		{
			//b5=0 ������ISO14443-3Э�� M1������Ҫ�ж��Ƿ�ΪPro��
			// 2011-12-31  yehf �޸�
			if ((ucSAK & 0x18) == 0x18)
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S70;
			}
			else
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S50;
			}
		}
		else
		{
			if ((ucSAK & 0x18) == 0x08)
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S50;
			}
			else if ((ucSAK & 0x18) == 0x18)
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S70;
			}
			else
			{
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PROCARD;
			}
		}
		return EM_mifs_SUCCESS;
	}

	// ��ȡ������UID����
	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		ucTempData = EM_mifs_PICC_ANTICOLL3;	// ��������ͻ
		result = EI_paypass_ucAnticoll(ucTempData, ucUIDTemp);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		result = EI_paypass_ucSelect(ucTempData, ucUIDTemp, &ucSAK);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	// ���������UID����
	memcpy(EG_mifs_tWorkInfo.ucUIDCL3, ucUIDTemp, 5);

	if ((ucSAK & 0x20) != 0x20)
	{
		//b5=0 ������ISO14443-3Э�� M1������Ҫ�ж��Ƿ�ΪPro��
		// 2011-12-31  yehf �޸�
		if ((ucSAK & 0x18) == 0x18)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S70;
		}
		else
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S50;
		}
	}
	else
	{
		if ((ucSAK & 0x18) == 0x08)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S50;
		}
		else if ((ucSAK & 0x18) == 0x18)
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PRO_S70;
		}
		else
		{
			EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_PROCARD;
		}
	}
	return EM_mifs_SUCCESS;
#endif
}

/****************************************************************
* �������ƣ�
*       EI_paypass_ucActPro(uchar* pucOutLen, uchar* pucATSData)
* ����������
*       ����Pro��
* �����º������ã�
*       ��
* �������º�����
*       EI_paypass_ucWUPA
*       EI_paypass_ucSelect
*       EI_paypass_ucRATS
* ���������
*       ��
* ���������
*       pucOutLen   Pro�����ص�ATS����
*       pucATSData  Pro�����ص�ATS����
* �� �� ֵ��
*       EM_mifs_SUCCESS   ��ȡ�ɹ�
*       EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*       EM_mifs_TRANSERR  ͨ�Ŵ���
*       EM_mifs_TIMEOUT   ������Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
* ˵    ����
*        1���ڼ�������У�������ֳ�ʱ��Ӧ��������ն�Ӧ�ط�������ܷ��ʹ����ɴ�3��
*        2���������ͨ�Ŵ�����ֱ�ӷ���
*        3���������Э�������ֱ�ӷ���
*******************************************************************/
uchar EI_paypass_ucActPro(uchar * pucOutLen, uchar * pucATSData)
{
	uchar result = EM_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucSAK = 0x00;
	uchar ucResp[2];
	uchar ucReSendNo = 0;

	// ��ʱ����£���෢������ͬһ������
	EI_paypass_vSelectType(EM_mifs_TYPEA);
//	s_DelayMs(6);
	s_DelayUs(EM_paypass_polltime);
	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		// �Ƚ���WUPA����
		result = EI_paypass_ucWUPA(ucResp);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	// ��������������ط���ֱ�ӷ��ظ��ն�
	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}
	else
	{
		// ���жϻ�ȡ��ATQA�����Ƿ��ѱ����ATQA����һ��
		if (memcmp(ucResp, EG_mifs_tWorkInfo.ucATQA, 2))
		{
			return EM_mifs_PROTERR;
		}
	}

	if(!(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))
	{
		//ISO ͨ��ģʽ
		// ���е�һ��SELECT
		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			ucTempData = EM_mifs_PICC_ANTICOLL1;
			result = EI_paypass_ucSelect(ucTempData, EG_mifs_tWorkInfo.ucUIDCL1, &ucSAK);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
		}

		if (result != EM_mifs_SUCCESS)
		{
			return result;
		}
		// UIDΪ4���ֽڣ�һ��Select��������ʹPICC����ACTIVATE״̬
		if (EG_mifs_tWorkInfo.ucUIDLen == 4)
		{
			goto ACTEND;
		}

		// ���еڶ���SELECT
		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			ucTempData = EM_mifs_PICC_ANTICOLL2;
			result = EI_paypass_ucSelect(ucTempData, EG_mifs_tWorkInfo.ucUIDCL2, &ucSAK);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
		}

		if (result != EM_mifs_SUCCESS)
		{
			return result;
		}
		// UIDΪ7���ֽڣ�����Select��������ʹPICC����ACTIVATE״̬
		if (EG_mifs_tWorkInfo.ucUIDLen == 7)
		{
			goto ACTEND;
		}

		// ���е�����SELECT

		// UIDΪ10���ֽڣ�����Select��������ʹPICC����ACTIVATE״̬
		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			ucTempData = EM_mifs_PICC_ANTICOLL3;
			result = EI_paypass_ucSelect(ucTempData, EG_mifs_tWorkInfo.ucUIDCL3, &ucSAK);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
		}

		if (result != EM_mifs_SUCCESS)
		{
			return result;
		}
	}
	else if((EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))
	{
		//EMV ��֤ģʽ
		//EMVģʽpoll�޷���ȡID ����EI_paypass_ucGetUID���»�ȡ������
		result = EI_paypass_ucGetUID();

		if(result != EM_mifs_SUCCESS)
		{
			return result;
		}
		else
		{
			goto ACTEND;
		}
	}

  ACTEND:
	if ((EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
		|| (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S70))
	{
		*pucOutLen = 0;
		return EM_mifs_SUCCESS;
	}
	// SELECT����ʹPICC����ACTIVATE״̬��Ӧ�ٷ���RATS��ȡATS����
	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		result = EI_paypass_ucRATS(pucOutLen, pucATSData);

		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_OK;
	EG_mifs_tWorkInfo.ucCurPCB = 0;

	return EM_mifs_SUCCESS;
}

/****************************************************************
* �������ƣ�
*       EI_paypass_ucActTypeB(uchar* pucOutLen, uchar* pucOut)
* ����������
*       ����TypeB��
* �����º������ã�
*       ��
* �������º�����
*       EI_paypass_ucWUPB
*       EI_paypass_ucAttrib
* ���������
*       ��
* ���������
*       pucOutLen   TypeB�����س���
*       pucOut      TypeB����������
* �� �� ֵ��
*       EM_mifs_SUCCESS   ��ȡ�ɹ�
*       EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*       EM_mifs_TRANSRR   ͨ�Ŵ���
*       EM_mifs_TIMEOUT   ������Ӧ
*       EM_mifs_NOSUP4    ��Ƭ��֧��ISO14443-4Э��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
* ˵    ����
*        1���ڼ�������У�������ֳ�ʱ��Ӧ��������ն�Ӧ�ط�������ܷ��ʹ����ɴ�3��
*        2���������ͨ�Ŵ�����ֱ�ӷ���
*        3���������Э�������ֱ�ӷ���
*******************************************************************/
uchar EI_paypass_ucActTypeB(uchar * pucOutLen, uchar * pucOut)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucReSendNo = 0;

	// ��ִ��WUPB����
	EI_paypass_vSelectType(EM_mifs_TYPEB);
	s_DelayMs(6);
	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		result = EI_paypass_ucWUPB(pucOutLen, pucOut);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	// �Ƚ�ATQB������ԭ�ȱ����ATQB����
	if (memcmp(pucOut, EG_mifs_tWorkInfo.ucATQB, 12))
	{
		return EM_mifs_PROTERR;
	}

	// ������ִ��ATTRIB����ʹTypeB������ACTIVATE״̬
	for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
	{
		result = EI_paypass_ucAttrib(&ucTempData);
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_OK;
	EG_mifs_tWorkInfo.ucCurPCB = 0;

	return EM_mifs_SUCCESS;
}

/***********************************************************
* �������ƣ�
*        EI_paypass_vSelectType(uchar ucMifType)
* ����������
*        ѡ���ز���������
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*       ucMifType     �ز��������ͣ�ȡֵ���£�
*�������������������� EM_mifs_TYPEA  TypeA�͵���
*                     EM_mifs_TYPEB  TypeB�͵���
* ���������
*       ��
* �� �� ֵ��
*       ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
************************************************************/
//extern uchar gCWGSNON;
//extern uint EG_Param[20];
void EI_paypass_vSelectType(uchar ucMifType)
{
	uchar ucTempData = 0;

	if (ucMifType == EM_mifs_TYPEA)
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ucTempData = ucTempData;
            emvHalSetStandard(EMV_HAL_TYPE_A);
            emvHalSetErrorHandling(EMV_HAL_PREACTIVATION_ERROR_HANDLING);
#endif

        }
        else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
            ucTempData = 0x8E;//EG_Param[0];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_DRVMODE, &ucTempData);
            ucTempData = 0x08;//EG_Param[1];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXAMP, &ucTempData);
            ucTempData = 0x21;//EG_Param[2];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_DRVCON, &ucTempData);
            ucTempData = 0x1A;//EG_Param[3];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXI, &ucTempData);
            ucTempData = 0x18;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXCRCCON, &ucTempData);
            ucTempData = 0x18;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCRCCON, &ucTempData);
            ucTempData = 0x27;//EG_Param[4];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXMODWIDTH, &ucTempData);
            ucTempData = 0xC0;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXWAITCTRL, &ucTempData);
            ucTempData = 0x12;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXWAITLO, &ucTempData);
            ucTempData = 0xCF;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FRAMECON, &ucTempData);
            ucTempData = 0x04;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCTRL, &ucTempData);
            ucTempData = 0x32;//EG_Param[5];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXTHRESHOLD, &ucTempData);
            ucTempData = 0x12;//EG_Param[6];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RCV, &ucTempData);
            ucTempData = 0x0A;//EG_Param[7];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXANA, &ucTempData);
//            ucTempData = 0x12;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RCV, &ucTempData);
//            ucTempData = 0x0A;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXANA, &ucTempData);

            ucTempData = 0x20;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXBITMOD, &ucTempData);
//            ucTempData = 0x04;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATACON, &ucTempData);
            ucTempData = 0x50;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATAMOD, &ucTempData);
            ucTempData = 0x40;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYMFREG, &ucTempData);
            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM0L, &ucTempData);

            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM1L, &ucTempData);
//            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10LEN, &ucTempData);
//            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10MOD, &ucTempData);
            ucTempData = 0x50;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM32MOD, &ucTempData);
            ucTempData = 0x02;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXBITMOD, &ucTempData);
            ucTempData = 0x01;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXSYNCVALL, &ucTempData);
            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXSYNCMOD, &ucTempData);
            ucTempData = 0x08;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXMOD, &ucTempData);
//            ucTempData = 0x80;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCORR, &ucTempData);
//            ucTempData = 0xF0;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FABCALI, &ucTempData);
            s_DelayUs(5000); //RC663 ���ز���Ҫ��ʱ �ȶ�ϵͳ
#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{

#ifdef EM_PN512_Module

			// TYPE A��
			ucTempData = 0x10;
			EI_mifs_vWriteReg(1, ControlReg, &ucTempData);

			ucTempData = 0x03;
			EI_mifs_vSetBitMask(TxControlReg, ucTempData);	// TX1 & TX2 enable

			ucTempData = 0x40;
			EI_mifs_vWriteReg(1, TxAutoReg, &ucTempData);	  //Force100ASK,
			ucTempData = 0x00;
			EI_mifs_vWriteReg(1, TxModeReg, &ucTempData);
			ucTempData = 0x08;
			EI_mifs_vWriteReg(1, RxModeReg, &ucTempData);
//			ucTempData = 0x4D;
			ucTempData = gcMifReg.Demod;
			EI_mifs_vWriteReg(1, DemodReg, &ucTempData);
			//ucTempData = 0x0F;//0x3F (2012-4-20)
			ucTempData = gcMifReg.CWGsP;
			EI_mifs_vWriteReg(1, CWGsPReg, &ucTempData);

			//ucTempData = 0xF4;//gCWGSNON:0x64(2012-4-20)
			ucTempData = gcMifReg.GsNOn;
			EI_mifs_vWriteReg(1, GsNOnReg, &ucTempData);

//			ucTempData = 0x84;
			ucTempData = gcMifReg.RxThresholdTypeA;
			EI_mifs_vWriteReg(1, RxThresholdReg, &ucTempData);
			ucTempData = gcMifReg.RFCfg;//0x48;
			EI_mifs_vWriteReg(1, RFCfgReg, &ucTempData);

			ucTempData = 0x40;
			EI_mifs_vClrBitMask(ModeReg, ucTempData);

			ucTempData = 0x10;
			EI_mifs_vClrBitMask(ModeReg,ucTempData);

#endif

		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{

#ifdef EM_RC531_Module

            // TYPE A��
            ucTempData = 0x5b;
            EI_mifs_vWriteReg(1, EM_mifs_REG_TXCONTROL, &ucTempData);	// Force100ASK, TX1 & TX2 enable
            ucTempData = 0x19;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CODERCONTROL, &ucTempData);	// Miller coding, 106kbps
            ucTempData = 0x08;
            EI_mifs_vWriteReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);	// Manchester Coding
            ucTempData = 0x63;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CRCPRESETLSB, &ucTempData);	// set CRC preset to 0x6363
            ucTempData = 0x63;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CRCPRESETMSB, &ucTempData);
		#ifndef RFID_ENABLE_REG
            ucTempData = 0xFF;
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXTHRESHOLD, &ucTempData);	// set max MinLevel & ColLevel.
            ucTempData = 0x73;
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXCONTROL1, &ucTempData);	// Manchester Coding
            ucTempData = 0x06;
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXWAIT, &ucTempData);	// set Guard Time.
            ucTempData = 0x3F;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CWCONDUCTANCE, &gtRfidDebugInfo.CW_A);
        #else
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXTHRESHOLD, &gtRfidRegInfo.ARxThreadhold);	// set max MinLevel & ColLevel.
			EI_mifs_vWriteReg(1, EM_mifs_REG_RXCONTROL1, &gtRfidRegInfo.ARxControl_1);	// Manchester Coding
			EI_mifs_vWriteReg(1, EM_mifs_REG_RXWAIT, &gtRfidRegInfo.ARxWait);	// set Guard Time.
			EI_mifs_vWriteReg(1, EM_mifs_REG_CWCONDUCTANCE, &gtRfidRegInfo.ATxCW);
			EI_mifs_vWriteReg(1, EM_mifs_REG_MFOUTSELECT, &gtRfidRegInfo.AMFout);
        #endif

#endif

		}

		EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEA;
	}
	else if(ucMifType == EM_mifs_TYPEB)
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ucTempData = ucTempData;
            emvHalSetStandard(EMV_HAL_TYPE_B);
            emvHalSetErrorHandling(EMV_HAL_PREACTIVATION_ERROR_HANDLING);
#endif

        }
        else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
            ucTempData = 0x8F;//EG_Param[8];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_DRVMODE, &ucTempData);
            ucTempData = 0x4F;//EG_Param[9];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXAMP, &ucTempData);
            ucTempData = 0x01;//EG_Param[10];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_DRVCON, &ucTempData);
            ucTempData = 0x06;//EG_Param[11];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXI, &ucTempData);
            ucTempData = 0x7B;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXCRCCON, &ucTempData);
//            ucTempData = 0x79;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCRCCON, &ucTempData);
            ucTempData = 0x08;//0x0F
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATANUM, &ucTempData);
            ucTempData = 0x00;//EG_Param[12];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXMODWIDTH, &ucTempData);
            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10BURSTLEN, &ucTempData);
            ucTempData = 0x01;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXWAITCTRL, &ucTempData);
            ucTempData = 0x00;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXWAITLO, &ucTempData);
            ucTempData = 0x05;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FRAMECON, &ucTempData);
            ucTempData = 0x34;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCTRL, &ucTempData);
//            ucTempData = 0x90;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXWAIT, &ucTempData);
            ucTempData = 0x3F;//EG_Param[13];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXTHRESHOLD, &ucTempData);
            ucTempData = 0x12;//EG_Param[14];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RCV, &ucTempData);
            ucTempData = 0x0A;//EG_Param[15];
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXANA, &ucTempData);
//            ucTempData = 0x12;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RCV, &ucTempData);
//            ucTempData = 0x0A;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXANA, &ucTempData);

            ucTempData = 0x09;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXBITMOD, &ucTempData);
//            ucTempData = 0x04;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATACON, &ucTempData);
            ucTempData = 0x08;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATAMOD, &ucTempData);
            ucTempData = 0x04;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYMFREG, &ucTempData);
            ucTempData = 0x03;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM0L, &ucTempData);

            ucTempData = 0x01;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM1L, &ucTempData);
            ucTempData = 0xAB;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10LEN, &ucTempData);
            ucTempData = 0x08;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10MOD, &ucTempData);
//            ucTempData = 0x04;
//            EI_mifs_vWriteReg(1, 0x57, &ucTempData);
            ucTempData = 0x04;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXBITMOD, &ucTempData);
//            ucTempData = 0x04;
//            EI_mifs_vWriteReg(1, 0x5B, &ucTempData);
            ucTempData = 0x02;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXSYNCMOD, &ucTempData);
            ucTempData = 0x0D;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXMOD, &ucTempData);
//            ucTempData = 0x80;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCORR, &ucTempData);
//            ucTempData = 0xF0;
//            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FABCALI, &ucTempData);

            ucTempData = 0;
//            EI_mifs_vWriteReg(1, 0x4D, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x4F, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x51, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x52, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x54, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x55, &ucTempData);
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM32MOD, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x59, &ucTempData);
//            EI_mifs_vWriteReg(1, 0x5A, &ucTempData);
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXSYNCVALL, &ucTempData);  //һ��Ҫ����
            s_DelayUs(5000); //RC663 ���ز���Ҫ��ʱ �ȶ�ϵͳ
#endif
        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{

#ifdef EM_PN512_Module
			// TYPE-B��
			ucTempData = 0x03;
			EI_mifs_vSetBitMask(TxControlReg, ucTempData);	// TX1 & TX2 enable

			ucTempData = 0x00;
			EI_mifs_vWriteReg(1, TxAutoReg, &ucTempData);
			ucTempData = 0x10;
			EI_mifs_vWriteReg(1, ControlReg, &ucTempData);
			ucTempData = 0x03;
			EI_mifs_vWriteReg(1, TxModeReg, &ucTempData);
			ucTempData = 0x0B;
			EI_mifs_vWriteReg(1, RxModeReg, &ucTempData);
			ucTempData = 0x00; //0x03
			EI_mifs_vWriteReg(1, TypeBReg, &ucTempData);

//			ucTempData = 0x4D;
			ucTempData = gcMifReg.Demod;
			EI_mifs_vWriteReg(1, DemodReg, &ucTempData);
			//ucTempData = 0xF4;//gCWGSNON:0x64(2012-4-20)
			ucTempData = gcMifReg.GsNOn;
			EI_mifs_vWriteReg(1, GsNOnReg, &ucTempData);
			//ucTempData = 0x0F;//0x3F(2012-4-20)
			ucTempData = gcMifReg.CWGsP;
			EI_mifs_vWriteReg(1, CWGsPReg, &ucTempData);
			//ucTempData = 0x04;  //0x0B(2012-4-20)
			ucTempData = gcMifReg.ModGsP;
			EI_mifs_vWriteReg(1, ModGsPReg, &ucTempData);
			ucTempData = gcMifReg.GsNOff;
			EI_mifs_vWriteReg(1, GsNOffReg, &ucTempData);
//			ucTempData = 0x55;
			ucTempData = gcMifReg.RxThresholdTypeB;
			EI_mifs_vWriteReg(1, RxThresholdReg, &ucTempData);
			ucTempData = gcMifReg.RFCfg;//0x68;
			EI_mifs_vWriteReg(1, RFCfgReg, &ucTempData);

			ucTempData = 0x40;
			EI_mifs_vClrBitMask(ModeReg, ucTempData);

			ucTempData = 0x10;
			EI_mifs_vClrBitMask(ModeReg,ucTempData);

#endif

		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{

#ifdef EM_RC531_Module

            // TYPE-B��
            ucTempData = 0x4b;
            EI_mifs_vWriteReg(1, EM_mifs_REG_TXCONTROL, &ucTempData);	// disable Force100ASk
            EI_mifs_vWriteReg(1, EM_mifs_REG_MODCONDUCTANCE, &gtRfidDebugInfo.ModConduct_B);	// set modulation index at 12% ����ʵ��������е����Ľ��
            ucTempData = 0x20;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CODERCONTROL, &ucTempData);	// NRZ-L, TypeB baud 106kbps
            ucTempData = 0x23;
            EI_mifs_vWriteReg(1, EM_mifs_REG_TYPEBFRAMING, &ucTempData);	// EGT=0
            ucTempData = 0x19;
            EI_mifs_vWriteReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);	// Manchester Coding
            ucTempData = 0xff;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CRCPRESETLSB, &ucTempData);	// set CRC preset to 0xFFFF
            ucTempData = 0xff;
            EI_mifs_vWriteReg(1, EM_mifs_REG_CRCPRESETMSB, &ucTempData);
            ucTempData = 0x5E;
            EI_mifs_vWriteReg(1, EM_mifs_REG_BPSKDEMCONTROL, &ucTempData);	//
		#ifndef RFID_ENABLE_REG
            ucTempData = 0x64;
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXTHRESHOLD, &ucTempData);	// set max MinLevel & ColLevel.
            ucTempData = 0x73;
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXCONTROL1, &ucTempData);
            ucTempData = 0x03;
            EI_mifs_vWriteReg(1, EM_mifs_REG_RXWAIT, &ucTempData);	// set Guard Time.
			ucTempData = 0x3F;							// TR0min + TR1min
//            EI_mifs_vWriteReg(1, EM_mifs_REG_CWCONDUCTANCE, &gtRfidDebugInfo.CW_B);
            EI_mifs_vWriteReg(1, EM_mifs_REG_CWCONDUCTANCE, &ucTempData);
		#else
			EI_mifs_vWriteReg(1, EM_mifs_REG_RXTHRESHOLD, &gtRfidRegInfo.BRxThreadhold);	// set max MinLevel & ColLevel.
			EI_mifs_vWriteReg(1, EM_mifs_REG_RXCONTROL1, &gtRfidRegInfo.BRxControl_1);	// Manchester Coding
			EI_mifs_vWriteReg(1, EM_mifs_REG_RXWAIT, &gtRfidRegInfo.BRxWait);	// set Guard Time.
			EI_mifs_vWriteReg(1, EM_mifs_REG_CWCONDUCTANCE, &gtRfidRegInfo.BTxCW);
			EI_mifs_vWriteReg(1, EM_mifs_REG_MFOUTSELECT, &gtRfidRegInfo.BMFout);
		#endif

#endif

		}

		EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEB;
	}
	else if(ucMifType == EM_mifs_NFCIP)
	{
		if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{

#ifdef EM_PN512_Module

#endif
		}
	}
#ifdef PRODUCT_F12_USB
	else
	{
        if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911)
        {

#ifdef EM_AS3911_Module

            //power down mode
//            emvHalActivateField(OFF);
            as3911WriteRegister(AS3911_REG_IO_CONF1, 0x0F);
            as3911WriteRegister(AS3911_REG_IO_CONF2, 0x80);
            /* Enable Oscillator, Transmitter and receiver. */
            as3911WriteRegister(AS3911_REG_OP_CONTROL, 0x00);
            as3911WriteRegister(AS3911_REG_MODE, 0x08);

#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{

#ifdef EM_RC531_Module
            EI_mifs_vClrBitMask(EM_mifs_REG_TXCONTROL, 0x03);
            EI_mifs_vSetBitMask(EM_mifs_REG_CONTROL, 1<<4);  //����powerdownģʽ
#endif
        }
	}
#endif
	//s_DelayMs(20);
}


/***********************************************************
* �������ƣ�
*       EI_paypass_ucPOLL(uchar* pucMifType)
* ����������
*       ��ѯ��Ӧ�����ж��Ƿ��п������Ӧ��,���ؿ�����
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       pucMifType   ��Ӧ���ڵ���Ƶ������
* �� �� ֵ��
*       EM_mifs_SUCCESS   ��ȡ�ɹ�
*       EM_mifs_PROTERR   ���صĲ��������Ϲ淶
*       EM_mifs_MULTIERR  �࿨��ͻ
*       EM_mifs_TIMEOUT   ������Ӧ
*       EM_mifs_TRANSERR  ͨ�Ŵ���
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
* ˵    ����
*        1�������Ѱ�������г���ͨ�Ŵ����򰴶࿨��ͻ����
*        2�������Ѱ�������г���Э�������ֱ�ӷ���
*******************************************************************/
uchar EI_paypass_ucPOLL(uchar * pucMifType)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucATQA[2];					// ���ڱ���ATQA����
	uchar ucATQB[12];					// ���ڱ���ATQB����
	uchar i,j;
	memset(ucATQA, 0x00, sizeof(ucATQA));
	memset(ucATQB, 0x00, sizeof(ucATQB));

	// ����WUPA����жϸ�Ӧ���Ƿ���TypeA��
	result = EI_paypass_ucWUPA(ucATQA);
	if (result == EM_mifs_TRANSERR)
	{
		for (i = 0; i < 10; i++)
		{
			result = EI_paypass_ucWUPA(ucATQA);
			if (result != EM_mifs_TRANSERR)
			{
				break;
			}
		}
	}

    if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV){
		//EMV ��֤ģʽ
        if(result == EM_mifs_MULTIERR){
            //���A��Ѱ����ͻ�� ֱ�ӱ��࿨��ͻ ����EMVģʽ���޸�
            EI_paypass_vHALTA();
			result = EI_paypass_ucWUPA(ucATQA);
//            EI_paypass_vHALTA();
            if(result == EM_mifs_MULTIERR){
//                lcd_cls();
//                lcd_display(0, DISP_FONT_LINE0,DISP_CLRLINE, "ret:%x",result);
//                while ( 1 ) {
//                    sys_beep_pro(4000,300,YES);
//                    if ( kb_getkey(500) == KEY1 ) {
//                        break;
//                    }
//                }
                return result;
            }
        }
    }

	if(result == EM_mifs_TRANSERR || result == EM_mifs_PROTERR || result == EM_mifs_MULTIERR)
	{
		//yehf ���� WUPA��Ӧ���󣬽�����ΪIdle״̬
		EI_paypass_vHALTA();
		goto EI_paypass_ucPOLLB;
	}
	else if (result == EM_mifs_SUCCESS)
	{
		// �����ȡ��ATQA����
		EG_mifs_tWorkInfo.ucATQA[0] = ucATQA[0];
		EG_mifs_tWorkInfo.ucATQA[1] = ucATQA[1];

		if(!(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))
		{
			//ISO ͨ��ģʽ
			// ��ȡUID��Ϣ�����ж��Ƿ��ж࿨����
			result = EI_paypass_ucGetUID();
			if(result == EM_mifs_SUCCESS)
			{
				if(EG_mifs_tWorkInfo.ucMifCardType <= EM_mifs_S70)
				{
					// 2011-09-05  ���������ŵ��� �߼�����CPU��˫����Ѱ�������߼�����������Ȼ���԰�CPU���ܼ�������
					j = 1;
				}
				else
				{
					j = EM_paypass_TRYON_TIMES;
				}
				EG_mifs_tWorkInfo.ATS[0] = 0;
				for (i = 0; i < j; i++)
				{
					result = EI_paypass_ucRATS(&ucTempData, EG_mifs_tWorkInfo.ATS);
					if (result != EM_mifs_TIMEOUT)
					{
						break;
					}
				}
				if(EG_mifs_tWorkInfo.ucMifCardType <= EM_mifs_S70)
				{
					if(result == EM_mifs_SUCCESS)
						EG_mifs_tWorkInfo.ucMifCardType += 3; //��ΪCPU+M1
					else
					{
						result = EM_mifs_SUCCESS; //��M1�߼���ֱ�ӷ��سɹ�
					}
				}
				
                //ISOģʽRAT����ʧ��
				if (result != EM_mifs_SUCCESS)
				{
					return result;
				}
				
				EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_OK;
				EG_mifs_tWorkInfo.ucCurPCB = 0;
			}
			else
			{
				return result;
			}

			//�����ISOģʽ TYPE A���˽���
			if (EG_mifs_tWorkInfo.ucAnticollFlag == 1)
			{
				//ucAnticollFlag == 1 �Ѿ���ISOͨ��ģʽ
                if(!(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))
				{
					//ISO ͨ��ģʽ
					// �������࿨���ڣ������ж��Ƿ���B��
					*pucMifType = EG_mifs_tWorkInfo.ucMifCardType;
				}
				EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEA;
				return EM_mifs_SUCCESS;
			}

		}

		//if (result == EM_mifs_SUCCESS)
		//EMV ��֤ģʽ
		if((EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))
		{
			//EMV ��֤ģʽ
			//if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
			{
				//EMV ��֤ģʽ
				// ���ÿ���ΪHALT̬
				EI_paypass_vHALTA();
			}

			EI_paypass_vSelectType(EM_mifs_TYPEB);
//			EI_paypass_vDelay(EM_mifs_TPDEALY);	// ��ʱ500ETU
            s_DelayUs(EM_paypass_polltime); //emvҪ��(���RC531) ����ʱ��
			// �������ж��Ƿ���TypeB������
			result = EI_paypass_ucWUPB(&ucTempData, ucATQB);
			if (result != EM_mifs_TIMEOUT)
			{
				/*
				if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
				{
					//EMV ��֤ģʽ
					s_DelayMs(8);
				}
				*/
				// ������Ӧ��˵����Ӧ���ڳ���TypeA�⣬����TypeB��
				// Ӧ�÷��ض࿨��ͻ
                // ��type b���سɹ�ʱ�ŷ��ض࿨��ͻ
                // �޸�ԭ��:s980����ģ������ʱ wupb����ͺ����������ж�
                // ��FIFO��û������  2013-04-26 chenf
                if (result == EM_mifs_SUCCESS)
                    return EM_mifs_MULTIERR;
			}
			// ����˵����Ӧ����ֻ��һ��TypeA��
			//EI_paypass_vDelay(EM_mifs_TPDEALY);	// �ɹ���ʱ500ETU ��ActPro/ActTypeBʱ��ʱ

			/*
			if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
			{
				//EMV ��֤ģʽ
				//EMVģʽ��ǿ�б�ΪEM_mifs_PROCARD�� ������EI_paypass_ucGetUID�����´���
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_TYPEA;
			}
			*/

			*pucMifType = EG_mifs_tWorkInfo.ucMifCardType;
			EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEA;
			return EM_mifs_SUCCESS;
		}
		else
		{
			/*
			if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
			{
				//EMV ��֤ģʽ
				s_DelayMs(8);
			}
			return result;
			*/
		}
	}
	else if (result == EM_mifs_TIMEOUT)
	{
	EI_paypass_ucPOLLB:
		// ��Ӧ��û��TypeA����Ӧ��˵����Ӧ����û��TypeA������
		// ������Ӧ���ж��Ƿ�TypeB������
		EI_paypass_vSelectType(EM_mifs_TYPEB);
		EI_paypass_vDelay(EM_mifs_TPDEALY);	// ��ʱ500ETU
		//s_DelayMs(50);
		// �������ж��Ƿ���TypeB������
		result = EI_paypass_ucWUPB(&ucTempData, ucATQB);
		if (result == EM_mifs_TRANSERR)
		{
			for (i = 0; i < 10; i++)
			{
				result = EI_paypass_ucWUPB(&ucTempData, ucATQB);
				if (result != EM_mifs_TRANSERR)
				{
					break;
				}
			}
		}
		if (result == EM_mifs_SUCCESS)
		{
			// ����ATQB����
			memcpy(EG_mifs_tWorkInfo.ucATQB, ucATQB, ucTempData);

            if(!(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV))
			{
				//ISO ͨ��ģʽ
				// ��������TypeB����ΪHALT״̬
				for (i = 0; i < EM_paypass_TRYON_TIMES; i++)
				{
					result = EI_paypass_ucHALTB(&ucTempData);
					if (result != EM_mifs_TIMEOUT)
					{
						break;
					}
				}
			}

			if (result == EM_mifs_SUCCESS)
			{
				if (EG_mifs_tWorkInfo.ucAnticollFlag == 1)
				{
					//ucAnticollFlag == 1 �Ѿ���ISOͨ��ģʽ
					// �������࿨���ڣ������ж��Ƿ���A��
					EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_TYPEBCARD;
					*pucMifType = EG_mifs_tWorkInfo.ucMifCardType;
					EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEB;
					return EM_mifs_SUCCESS;
				}

				EI_paypass_vSelectType(EM_mifs_TYPEA);
//				EI_paypass_vDelay(EM_mifs_TPDEALY);	// ��ʱ500ETU
                s_DelayUs(EM_paypass_polltime); //emvҪ��(���RC531) ����ʱ��

				result = EI_paypass_ucWUPA(ucATQA);
				if (result != EM_mifs_TIMEOUT)
				{
					/*
					if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
					{
						//EMV ��֤ģʽ
						s_DelayMs(8);
					}
					*/
					// ��TypeA����Ӧ����˵������TypeA���࿨��ͻ
                    // ��TYPE A���سɹ�ʱ�ŷ��ض࿨��ͻ(ͬ��)
                    // �޸�ԭ��:wp70 wupa����ͺ�Ҳ����������ж� ԭ����
                    // ��FIFO��û������  2014-09-11 chenf
                    if (result == EM_mifs_SUCCESS)
                        return EM_mifs_MULTIERR;
				}

				//EI_paypass_vDelay(EM_mifs_TPDEALY);	// �ɹ���ʱ500ETU ��ActPro/ActTypeBʱ��ʱ
				// ��Ӧ����ֻ��һ��TypeB��
				EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_TYPEBCARD;
				*pucMifType = EG_mifs_tWorkInfo.ucMifCardType;
				EG_mifs_tWorkInfo.ucCurType = EM_mifs_TYPEB;
				return EM_mifs_SUCCESS;
			}
			else
			{
				/*
				if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
				{
					//EMV ��֤ģʽ
					s_DelayMs(8);
				}
				*/
				return result;
			}
		}
		else if (result == EM_mifs_TIMEOUT)
		{
			//EI_paypass_vDelay(EM_mifs_TPDEALY);	// ��ʱ500ETU
		}
		else
		{
			/*
			if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
			{
				//EMV ��֤ģʽ
				s_DelayMs(8);
			}
			*/
		}
		return result;
	}
	/*
	if(EG_mifs_tWorkInfo.ucOptMode == RFID_MODE_EMV)
	{
		//EMV ��֤ģʽ
		s_DelayMs(8);
	}
	*/
	// �������������ֱ�ӷ���
	return result;

}

/*****************************************************************
* �������ƣ�
*        EI_paypass_ucIfMifExit(void)
* ����������
*        �ж��Ƿ��Ѿ���Mif���뿪��Ӧ��
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        EM_mifs_SUCCESS   ��Ƭ���뿪��Ӧ��
*        EM_mifs_CARDEXIST ��Ƭ���ڸ�Ӧ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
**********************************************************/
uchar EI_paypass_ucIfMifExit(void)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData[12];
	uchar ucTemp = 0x00;
	uchar ucReSendNo = 0;

	if (EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEA)
	{
		// �ȷ���WUPAѰ��������п���Ӧ����˵����δ�뿪��Ӧ��
		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			result = EI_paypass_ucWUPA(ucTempData);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}

			if ((result == EM_mifs_TIMEOUT) && (ucReSendNo == 2))
			{
				// �޿���Ӧ����˵�������뿪��Ӧ��
				return EM_mifs_SUCCESS;
			}
		}
		// ������Ӧ��˵����δ�뿪��Ӧ��
		// �Ƚ���Ƭ��ΪHALT̬
		EI_paypass_vHALTA();
		return EM_mifs_CARDEXIST;
	}
	else
	{
		// �ȷ���WUPBѰ��������п���Ӧ����˵����δ�뿪��Ӧ��
		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			result = EI_paypass_ucWUPB(&ucTemp, ucTempData);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
			if ((result == EM_mifs_TIMEOUT) && (ucReSendNo == 2))
			{
				// �޿���Ӧ����˵�������뿪��Ӧ��
				return EM_mifs_SUCCESS;
			}
		}
		// ������Ӧ��˵����δ�뿪��Ӧ��
		// �Ƚ���Ƭ��ΪHALT̬
		for (ucReSendNo = 0; ucReSendNo < EM_paypass_TRYON_TIMES; ucReSendNo++)
		{
			result = EI_paypass_ucHALTB(&ucTemp);
			if (result != EM_mifs_TIMEOUT)
			{
				break;
			}
			if ((result == EM_mifs_TIMEOUT) && (ucReSendNo == 2))
			{
				// �޿���Ӧ����˵�������뿪��Ӧ��
				return EM_mifs_SUCCESS;
			}
		}
	}

	return EM_mifs_CARDEXIST;
}

/**********************************************************************
* �������ƣ�
*       EI_paypass_ucProcess(uint uiSendLen, uint* puiRecLen)
* ��������:
*       ��PCD��PICC֮�䷢�ͺͽ�������
* �����º�������:
*       �ϲ㺯��
* �������º���:
*       ��
* �������:
*       uiSendLen   Ҫ���͵����ݳ���
* ���������
*       puiRecLen   ���յ������ݳ���
* �� �� ֵ��
*       EM_mifs_SUCCESS    ���ճɹ�
*       EM_mifs_TIMEOUT    ��ʱ����Ӧ
*       EM_mifs_TRANSERR   ͨ�Ŵ���
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EI_paypass_ucProcess(uint uiSendLen, uint *puiRecLen)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar ucCMD = 0;
	// ���ö�ʱ��
	EI_paypass_vSetTimer(EG_mifs_tWorkInfo.ulFWT);

	if (EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEA)
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ucTempData = ucTempData;
            EI_paypass_vSelectType(EM_mifs_TYPEA);
            ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
            EG_mifs_tWorkInfo.expectMaxRec = sizeof(EG_mifs_tWorkInfo.aucBuffer);
#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
		{

#ifdef EM_RC663_Module
            ucTempData = 0x01;
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);
            ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{

#ifdef EM_PN512_Module

			ucTempData = 0x80;
			EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
			EI_mifs_vSetBitMask(TxModeReg, ucTempData);
			EI_mifs_vSetBitMask(RxModeReg, ucTempData);
			ucCMD = PN512CMD_TRANSCEIVE;

#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{

#ifdef EM_RC531_Module

			ucTempData = 0x0F;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
			ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif
		}
	}
	else
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ucTempData = ucTempData;
            EI_paypass_vSelectType(EM_mifs_TYPEB);
            ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
            EG_mifs_tWorkInfo.expectMaxRec = sizeof(EG_mifs_tWorkInfo.aucBuffer);
#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
		{

#ifdef EM_RC663_Module

            ucTempData = 0x01;
            EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);
            ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;

#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{

#ifdef EM_PN512_Module

			EI_mifs_vSetBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
			EI_mifs_vSetBitMask(TxModeReg, 0x80);
			EI_mifs_vSetBitMask(RxModeReg, 0x80);
			ucCMD = PN512CMD_TRANSCEIVE;

#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{

#ifdef EM_RC531_Module

			ucTempData = 0x2C;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
			ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif
		}
	}

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = uiSendLen;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	result = EI_mifs_ucHandleCmd(ucCMD);

	if (result == EM_mifs_NOTAGERR)
	{
		result = EM_mifs_TIMEOUT;
	}
	else if (result != EM_mifs_SUCCESS)
	{
		result = EM_mifs_TRANSERR;
	}

	if (result == EM_mifs_SUCCESS)
	{
		*puiRecLen = EG_mifs_tWorkInfo.ulBytesReceived;
		if (EG_mifs_tWorkInfo.ulBytesReceived == 0)
		{
			// ��������ȷ������ʵ�ʽ��ճ���Ϊ0ʱ�������������
			// 2006.08.24

			result = EM_mifs_TRANSERR;
		}
		// ����TypeB������PICC����TR0Min + TR1Min��ʼ�������ݵĻ�
		// ����PAYPASSҪ�󣬿����϶�Ϊ�������Ҳ�����������գ�
		// �ڱ���������ж����������
	}

    if ( EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911 ) {
//        EI_paypass_vDelay(EM_mifs_FDTMIN);	// ����֮����ʱ500us���Ա�֤��С���򱣻�ʱ��
        s_DelayUs(EM_paypass_minFDTadtime);	// emvҪ��(���RC531) ����ʱ��պ���ʱ500us
    }

	return (result);
}
/**********************************************************************
* �������ƣ�
*       ����I��
* ����������
*       ����R_Block
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       puiRecLen  �յ������ݳ���
* �� �� ֵ��
*       EM_mifs_SUCCESS    ���ճɹ�
*       EM_mifs_TIMEOUT    ��ʱ����Ӧ
*       EM_mifs_TRANSERR   ͨ�Ŵ���
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uint make_IBlock(uchar PCB, uint inlen, uchar *in)
{
	uint i=0;
	EG_mifs_tWorkInfo.aucBuffer[i++] = PCB;
	if(EG_mifs_tWorkInfo.ucCIDFlag)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] |= 0x08;  //��CID
		EG_mifs_tWorkInfo.aucBuffer[i++] = 0x00;
	}
	if(EG_mifs_tWorkInfo.ucNADFlag)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] |= 0x04;  //��NAD
		EG_mifs_tWorkInfo.aucBuffer[i++] = 0x00;
	}
	memcpy(&EG_mifs_tWorkInfo.aucBuffer[i],in,inlen);
	i += inlen;
	return i;
}

uint make_RBlock(uchar PCB)
{
	uint i=0;
	EG_mifs_tWorkInfo.aucBuffer[i++] = PCB;
	if(EG_mifs_tWorkInfo.ucCIDFlag)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] |= 0x08;  //��CID
		EG_mifs_tWorkInfo.aucBuffer[i++] = 0x00;
	}
	return i;
}
/**********************************************************************
* �������ƣ�
*       EI_paypass_ucMifRBlock(uint* puiRecLen)
* ����������
*       ����R_Block
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       puiRecLen  �յ������ݳ���
* �� �� ֵ��
*       EM_mifs_SUCCESS    ���ճɹ�
*       EM_mifs_TIMEOUT    ��ʱ����Ӧ
*       EM_mifs_TRANSERR   ͨ�Ŵ���
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EI_paypass_ucMifRBlock(uint * puiRecLen)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucReSendNo = 0x00;
	uint sendlen;
	// ��෢��NAK����
	for (ucReSendNo = 0; ucReSendNo < 2; ucReSendNo++)
	{
		sendlen = make_RBlock(EG_mifs_tWorkInfo.ucCurPCB|0xB2);
		result = EI_paypass_ucProcess(sendlen, puiRecLen);
		if ((result != EM_mifs_TIMEOUT) && (result != EM_mifs_TRANSERR))
		{
			// ������ֳ�ʱ���ߴ��������Ӧ�ط�NAK
			break;
		}
	}
	return result;
}
/**********************************************************************
* �������ƣ�
*       EI_paypass_ucMifSBlock(uint* puiRecLen)
* ����������
*       ����S_Block
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ��
* ���������
*       puiRecLen  �յ������ݳ���
* �� �� ֵ��
*       EM_mifs_SUCCESS     ���ճɹ�
*       EM_mifs_TIMEOUT     ��ʱ����Ӧ
*       EM_mifs_TRANSERR    ͨ�Ŵ���
*       EM_mifs_PROTERR     Э�����
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EI_paypass_ucMifSBlock(uint * puiRecLen)
{
	uchar ucTempData = 0;
	uchar result = EM_mifs_SUCCESS;
	ulong tmp,i;
	uint sendlen;
	if (IF_SBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
	{
		// PICC��Ӧ��CID�ֽ�
		return EM_mifs_PROTERR;
	}
	else
	{
		ucTempData = EG_mifs_tWorkInfo.aucBuffer[1];
		if ((ucTempData & 0xC0) != 0x00)
		{
			// ��֧��Power Level
			return EM_mifs_PROTERR;
		}
		// ��ȡWTXM����
		EG_mifs_tWorkInfo.ucWTX = ucTempData & 0x3F;
		if (EG_mifs_tWorkInfo.ucWTX == 0x00)
		{
			return EM_mifs_PROTERR;
		}
		if (EG_mifs_tWorkInfo.ucWTX > 59)
		{
			EG_mifs_tWorkInfo.ucWTX = 59;
		}

		// ������ʱFWTʱ��
		// TB412_12~14��Ҫ����1
		if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEB
			&& (EG_mifs_tWorkInfo.ucFWI >=12 && EG_mifs_tWorkInfo.ucFWI <=14))
		{
			i = EG_mifs_tWorkInfo.ulFWT - 1;
		}
		else
		{
			i = EG_mifs_tWorkInfo.ulFWT;
		}
		EG_mifs_tWorkInfo.ulFWTTemp = EG_mifs_tWorkInfo.ucWTX * i;
	}
	//����SӦ��
	sendlen = 0;
	EG_mifs_tWorkInfo.aucBuffer[sendlen++] = 0xf2;
	if(EG_mifs_tWorkInfo.ucCIDFlag)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] |= 0x08;
		EG_mifs_tWorkInfo.aucBuffer[sendlen++] = 0x00;
	}
	EG_mifs_tWorkInfo.aucBuffer[sendlen++] = ucTempData & 0x3F;
	if(EG_mifs_tWorkInfo.ulFWTTemp > (1146880))  //EM_mifs_MAXFWT 2^15*35
	{
	    // ���ö�ʱ��,��󲻳���FWT_MAX
	    //return EM_mifs_PROTERR;
	    EG_mifs_tWorkInfo.ulFWTTemp = s_rfid_FWI2FWT(14);
		 if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEB
			&& (EG_mifs_tWorkInfo.ucFWI >=12 && EG_mifs_tWorkInfo.ucFWI <=14))
		{
			EG_mifs_tWorkInfo.ulFWTTemp  -= 1;
		}
	}
	//D2(TRACE("\r\n FWT:d",EG_mifs_tWorkInfo.ulFWTTemp););
	tmp = EG_mifs_tWorkInfo.ulFWT;
	EG_mifs_tWorkInfo.ulFWT = EG_mifs_tWorkInfo.ulFWTTemp;
	EG_mifs_tWorkInfo.aucBuffer[sendlen-1] = ucTempData & 0x3F;
	result = EI_paypass_ucProcess(sendlen,puiRecLen);
	EG_mifs_tWorkInfo.ulFWT = tmp;
	/*
	if(++gtRfidProInfo.SBlock>=3)
	{
		//TA421����S�鲻�ܳ���3��
		return EM_mifs_PROTERR;
	}
	*/
	return (result);
}

/**********************************************************************
* �������ƣ�
*       EI_paypass_ucExchange(uint uiSendLen, uchar* pucInData,
*                             uint* puiRecLen, uchar* pucOutData )
* ����������
*      ʵ�ֶ������뿨֮���ISO14443-4Э�����ݽ���
* �����º������ã�
*      API
* �������º�����
*      ��
* ���������
*      uiSendLen    �������ݳ���
*      pucInData    ��������
* ���������
*      puiRecLen    ������ݳ���
*      pucOutData   �������
* �� �� ֵ:
*        EM_mifs_SUCCESS  ���ݽ����ɹ�����Ƭ����״̬�ֽ�Ϊ0x9000��
*        EM_mifs_SWDIFF   ��Ƭ����״̬�ֽ�Ϊ�������� 0x9000��
*        EM_mifs_TIMEOUT  ��Ƭ�޷���
*        EM_mifs_TRANSERR ͨ�Ŵ���
*        EM_mifs_PROTERR  Э�����
*        EM_mifs_SWDIFF   SW1SW2 != 0x9000
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EI_paypass_ucExchange(uint uiSendLen, uchar * pucInData, uint * puiRecLen, uchar * pucOutData)
{
	uint uiSendLeftLen = 0, uiTempLen = 0, uiSendCurrPos = 0, uiCurrRecvLen = 0;
	uchar result = EM_mifs_SUCCESS;
	uchar tempResult = 0x00;
	uchar i = 0;
	uchar ucResendNo = 0x00;
	uint blocklen;
	*puiRecLen = 0;
	// ����Ҫ���͵����ݳ���
	uiSendLeftLen = uiSendLen;
	// �����֡���Ȱ�������CRC�ֽں�PCB�ֽ�
	// PAYPASSҪ�󲻰���CID�ֽں�NAD�ֽ� ���ӿ鷢��
	while (uiSendLeftLen > (EG_mifs_tWorkInfo.FSC - 3))
	{
		//ÿ�ο��Դ����������ݳ���
		uiTempLen = EG_mifs_tWorkInfo.FSC - 3;
		// ����I��
		blocklen = make_IBlock(EG_mifs_tWorkInfo.ucCurPCB|0x12,uiTempLen,pucInData+uiSendCurrPos);
		result = EI_paypass_ucProcess(blocklen, &uiCurrRecvLen);

		// �����ʱû�н��յ�����,����NAKҪ��PICC��������
		// ������չ����г��ִ���Ҳ����NAKҪ��PICC��������
		if ((result == EM_mifs_TIMEOUT) || (result == EM_mifs_TRANSERR))
		{
			tempResult = result;
        exchange_noise0:
			result = EI_paypass_ucMifRBlock(&uiCurrRecvLen);
			if (result != EM_mifs_SUCCESS)
			{
				// ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
				return tempResult;
			}
		}

		if (result != EM_mifs_SUCCESS)
		{
			return result;
		}
		// ȥ��2B��CRCУ����
		if (uiCurrRecvLen > (EG_mifs_tWorkInfo.FSD - 2))
		{
			// ���ճ��ȳ�����FSD
			return EM_mifs_PROTERR;
		}

		if (uiCurrRecvLen == 0)
		{
			// ���ݽ���ʧ��
			return EM_mifs_TRANSERR;
		}
		if(IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			// ���յ�I_Block, ��Э�������
			return EM_mifs_PROTERR;
		}
		else if (IF_RBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			// ���յ�R_Block
			if (IF_RBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
			{
				// PICC��Ӧ����NAK ��Ӧ����CID
				return EM_mifs_PROTERR;
			}

			if(GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) == GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
			{
				// �յ�R_block(ACK)Ӧ�������ȷ, ׼��������һ����Ϣ��
				uiSendCurrPos += uiTempLen;
				uiSendLeftLen -= uiTempLen;
				INC_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB);
				// �ط���������
				ucResendNo = 0;
			}
			else
			{
				// �����Ų���ȷ, ���ط�
				if (ucResendNo++ > EM_paypass_protocol_TIMES)
				{
					// PCD�ط�һ��I_Block��������
					return EM_mifs_PROTERR;
				}
			}
		}
		else if (IF_SBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			// ���յ�S_Block
			gtRfidProInfo.SBlock = 0;
			while (1)
			{
				result = EI_paypass_ucMifSBlock(&uiCurrRecvLen);
				if ((result == EM_mifs_TIMEOUT) || (result == EM_mifs_TRANSERR))
				{
					tempResult = result;
				exchange_noise1:
					// ������Ӧ�����ͨ�Ŵ�������NAK
					result = EI_paypass_ucMifRBlock(&uiCurrRecvLen);
					if (result != EM_mifs_SUCCESS)
					{
						// ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
						return tempResult;
					}
				}

				if (result != EM_mifs_SUCCESS)
				{
					return result;
				}

				if (uiCurrRecvLen > (EG_mifs_tWorkInfo.FSD - 2))
				{
					// ���ճ��ȳ�����FSD
					return EM_mifs_PROTERR;
				}

				if (uiCurrRecvLen == 0)
				{
					// ���ݽ���ʧ��
					return EM_mifs_TRANSERR;
				}

				if(IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// ���յ�I_Block, ��Э�������
					return EM_mifs_PROTERR;
				}
				else if(IF_RBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// ���յ�R_Block
					if (IF_RBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
					{
						// PICC��Ӧ����CID ��Ӧ����NAK
						return EM_mifs_PROTERR;
					}

					if(GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) == GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
					{
						// �յ�R_block(ACK)Ӧ�������ȷ, ׼��������һ����Ϣ��
						uiSendCurrPos += uiTempLen;
						uiSendLeftLen -= uiTempLen;
						INC_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB);
						// �ط���������
						ucResendNo = 0;
					}
					else
					{
						// �����Ų���ȷ, �ط��ϸ�I_BLOCk
						if (ucResendNo++ > EM_paypass_protocol_TIMES)
						{
							// PCD�ط�һ��I_Block��������
							return EM_mifs_PROTERR;
						}
					}
					break;
				}
				else if (!IF_SBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// ���յ���������Block
					if(uiCurrRecvLen < 4)
					{
						//���ճ���С��4�ֽڣ���Ϊ����������NAK
						tempResult = EM_mifs_TIMEOUT;
						goto exchange_noise1;
					}
					else
					{
						return EM_mifs_PROTERR;
					}
				}
			}
		}
		else
		{
			// PCBֵ����ȷ
			if(uiCurrRecvLen < 4)
			{
				//���ճ���С��4�ֽڣ���Ϊ����������NAK
				tempResult = EM_mifs_TIMEOUT;
				goto exchange_noise0;
			}
			else
			{
				return EM_mifs_PROTERR;
			}
		}
	}

	ucResendNo = 0;						// �ط���������
	// �������һ֡����
	while (1)
	{
		blocklen = make_IBlock(EG_mifs_tWorkInfo.ucCurPCB|0x02,uiSendLeftLen,pucInData+uiSendCurrPos);
		result = EI_paypass_ucProcess(blocklen, &uiCurrRecvLen);
		// �����ʱû�н��յ�����,����NAKҪ��PICC��������
		// ������չ����г��ִ���Ҳ����NAKҪ��PICC��������
		if ((result == EM_mifs_TIMEOUT) || (result == EM_mifs_TRANSERR))
		{
			tempResult = result;
//            if(EG_mifs_tWorkInfo.ucCIDPollFlg)
//            {
//                return tempResult;
//            }
		exchange_noise2:
			result = EI_paypass_ucMifRBlock(&uiCurrRecvLen);
			if (result != EM_mifs_SUCCESS)
			{
				// ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
				return tempResult;
			}
		}

		if (result != EM_mifs_SUCCESS)
		{
			return result;
		}
		if (uiCurrRecvLen > (EG_mifs_tWorkInfo.FSD - 2))
		{
			// ���ճ��ȳ�����FSD
			return EM_mifs_PROTERR;
		}

		if (uiCurrRecvLen == 0)
		{
			// ���ݽ���ʧ��
			return EM_mifs_TRANSERR;
		}
		if (IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			// ���յ�I_Block ׼�����ܿ�Ӧ������
			break;
		}
		else if (IF_RBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			// ���յ�R_Block
			if (IF_RBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
			{
				// PICC��Ӧ����CID PICC��Ӧ����NAK
				return EM_mifs_PROTERR;
			}

			if(GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) == GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
			{
				// PICC���ʹ�����
				return EM_mifs_PROTERR;
			}
			else
			{
				//�ط�����,Ҳ����ѡ��ֱ���˳�
				if (++ucResendNo >= EM_paypass_protocol_TIMES)
				{
					// PCD�ط�һ��I_Block��������
					return EM_mifs_PROTERR;
				}
			}
		}
		else if (IF_SBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			// ���յ�S_Block
			gtRfidProInfo.SBlock = 0;
			while (1)
			{
				result = EI_paypass_ucMifSBlock(&uiCurrRecvLen);
				if ((result == EM_mifs_TIMEOUT) || (result == EM_mifs_TRANSERR))
				{
					tempResult = result;
				exchange_noise3:
					// ������Ӧ�����ͨ�Ŵ�������NAK
					result = EI_paypass_ucMifRBlock(&uiCurrRecvLen);
					if (result != EM_mifs_SUCCESS)
					{
						// ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
						return tempResult;
					}
				}

				if (result != EM_mifs_SUCCESS)
				{
					return result;
				}

				if (uiCurrRecvLen == 0)
				{
					// ���ݽ���ʧ��
					return EM_mifs_TRANSERR;
				}

				if (uiCurrRecvLen > (EG_mifs_tWorkInfo.FSD - 2))
				{
					// ���ճ��ȳ�����FSD
					return EM_mifs_PROTERR;
				}

				if (IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// ���յ�I_Block
					break;
				}
				else if (IF_RBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// ���յ�R_Block
					if (IF_RBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
					{
						// PICC��Ӧ����CID  PICC��Ӧ����NAK
						return EM_mifs_PROTERR;
					}
					if(GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) == GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
					{
						// PICC���ʹ�����
						return EM_mifs_PROTERR;
					}
					else
					{
						// �����ط�
						if (ucResendNo++ > EM_paypass_protocol_TIMES)
						{
							// PCD�ط�һ��I_Block��������
							return EM_mifs_PROTERR;
						}
					}
					break;
				}
				else if (!IF_SBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// PCBֵ����ȷ
					if(uiCurrRecvLen < 4)
					{
						//���ճ���С��4�ֽڣ���Ϊ����������NAK
						tempResult = EM_mifs_TIMEOUT;
						goto exchange_noise3;
					}
					else
					{
						return EM_mifs_PROTERR;
					}
				}
			}

			if (IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
			{
				// ���յ�I_Block
				break;
			}
		}
		else
		{
			// ���յ�������֧�ֵ�PCB
			// PCBֵ����ȷ
			if(uiCurrRecvLen < 4)
			{
				//���ճ���С��4�ֽڣ���Ϊ����������NAK
				tempResult = EM_mifs_TIMEOUT;
				goto exchange_noise2;
			}
			else
			{
				return EM_mifs_PROTERR;
			}
		}
	}
	// ����PICC���͵�I_Block
	while (1)
	{
		if(!(EG_mifs_tWorkInfo.OptMode&RFID_MODE_NOFRAME))
		{
			if (GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) != GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
			{
				// PICC���ʹ�����
				return EM_mifs_PROTERR;
			}
		}
		if (IF_IBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
		{
			return EM_mifs_PROTERR;
		}
		// ���ӳ���Խ���жϣ� 2006-10-09
		if (uiCurrRecvLen < 1)
		{
			return EM_mifs_TRANSERR;
		}
		//   ������յ���I_Block����
		// �ж���Ч����ƫ�� PCB+CID+NAD+DATA
		blocklen = 1;
		if(EG_mifs_tWorkInfo.ucCIDFlag)
		{
			++blocklen;
		}
		if(EG_mifs_tWorkInfo.ucNADFlag)
		{
			++blocklen;
		}
		memcpy(&pucOutData[*puiRecLen], &EG_mifs_tWorkInfo.aucBuffer[blocklen], uiCurrRecvLen - blocklen);
		*puiRecLen += (uiCurrRecvLen - blocklen);
		INC_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB);
		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x0)
		{
			// ���յ����һ��I_Block���򷵻�
			break;
		}

		// PICC��������I_Block
		i = 0;
		while (1)
		{
			// ����ACK��ȡ����I_Block
			blocklen = make_RBlock(EG_mifs_tWorkInfo.ucCurPCB|0xA2);
			result = EI_paypass_ucProcess(blocklen, &uiCurrRecvLen);

			if ((result == EM_mifs_TIMEOUT) || (result == EM_mifs_TRANSERR))
			{
				i++;
				if (i >= 3)
				{
					// ACK����������������� ��1������Ӧ�� ��2���ط�
					return result;
				}
				// û����Ӧ����ͨ�Ŵ������ط�ACK
				continue;
			}

			if (result != EM_mifs_SUCCESS)
			{
				return result;
			}

			if (uiCurrRecvLen == 0)
			{
				// ���ݽ���ʧ��
				return EM_mifs_TRANSERR;
			}

			if (uiCurrRecvLen > (EG_mifs_tWorkInfo.FSD - 2))
			{
				// ���ճ��ȳ�����FSD
				return EM_mifs_PROTERR;
			}

			if (IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
			{
				// I_Block
				break;
			}
			else if (IF_RBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
			{
				// ���յ�R_Block
				if (IF_RBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// PICC��Ӧ����CID PICC��Ӧ����NAK
					return EM_mifs_PROTERR;
				}
				if (GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) == GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
				{
					// PICC���ʹ�����
					return EM_mifs_PROTERR;
				}
				if (++i >= EM_paypass_protocol_TIMES)
				{
					// ACK�����������������
					return EM_mifs_PROTERR;
				}
				continue;
			}
			else if (IF_SBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
			{
				// ���յ�S_Block
				gtRfidProInfo.SBlock = 0;
				while (1)
				{
					result = EI_paypass_ucMifSBlock(&uiCurrRecvLen);
					if ((result == EM_mifs_TIMEOUT) || (result == EM_mifs_TRANSERR))
					{
						tempResult = result;
						// ������Ӧ�����ͨ�Ŵ�������NAK
						result = EI_paypass_ucMifRBlock(&uiCurrRecvLen);
						if (result != EM_mifs_SUCCESS)
						{
							// ��Ȼû�н��յ���Ч��Ӧ���ݣ���֪ͨ�豸
							return tempResult;
						}
					}

					if (result != EM_mifs_SUCCESS)
					{
						return result;
					}

					if (uiCurrRecvLen == 0)
					{
						// ���ݽ���ʧ��
						return EM_mifs_TRANSERR;
					}

					if (uiCurrRecvLen > (EG_mifs_tWorkInfo.FSD - 2))
					{
						// ���ճ��ȳ�����FSD
						return EM_mifs_PROTERR;
					}

					if (IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
					{
						// ���յ�I_Block
						break;
					}
					else if (IF_RBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
					{
						// ���յ�R_Block
						if (IF_RBLOCK_ERR(EG_mifs_tWorkInfo.aucBuffer[0]))
						{
							// PICC��Ӧ����CID PICC��Ӧ����NAK
							return EM_mifs_PROTERR;
						}
						if (GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.aucBuffer[0]) != GET_PCB_BLOCKNO(EG_mifs_tWorkInfo.ucCurPCB))
						{
							// PICC���ʹ�����
							return EM_mifs_PROTERR;
						}
						if (++i >= EM_paypass_protocol_TIMES)
						{
							// PCD�ط�һ��I_Block��������
							return EM_mifs_PROTERR;
						}
						break;
					}
					else if (!IF_SBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
					{
						// ���յ���������Block
						return EM_mifs_PROTERR;
					}
				}

				if (IF_IBLOCK(EG_mifs_tWorkInfo.aucBuffer[0]))
				{
					// ���յ�I_Block
					break;
				}
			}
			else
			{
				// ���յ���������Block
				return EM_mifs_PROTERR;

			}
		}
		// ���յ�I_Block, ��������н���
		i = 0;
	}

	// ���մ���
	if ((*puiRecLen > 1) && (result == EM_mifs_SUCCESS))
	{
		if ((pucOutData[*puiRecLen - 2] != 0x90) || (pucOutData[*puiRecLen - 1] != 0x00))
		{
			result = EM_mifs_SWDIFF;
		}
		else
		{
			result = EM_mifs_SUCCESS;
		}
	}
	else
	{
		*puiRecLen = 0;
	}

	return result;
}

/**************************************************************************
* �������ƣ�
*        EI_paypass_ucDeSelect(void)
* ����������
*        ����DESELECT��ʹMif������HALT״̬
* �����º������ã�
*        ��
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        EM_mifs_SUCCESS   �ɹ�
*        EM_mifs_TRANSERR  ͨ�Ŵ���
*        EM_mifs_PROTERR   ������PayPassЭ��
*        EM_mifs_TIMEOUT   ��ʱ����Ӧ
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EI_paypass_ucDeSelect(void)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucTempData = 0x00;
	uchar i = 0;
	uchar ucCMD = 0;

	if (EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEA)
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ucTempData = ucTempData;
            ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
            EG_mifs_tWorkInfo.expectMaxRec = 1+2; //DESELECT + CRC
#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
		{

#ifdef EM_RC663_Module
            ucTempData = 0x01;
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);
            ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{
#ifdef EM_PN512_Module

			ucTempData = 0x80;
			EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
			EI_mifs_vSetBitMask(TxModeReg, ucTempData);
			EI_mifs_vSetBitMask(RxModeReg, ucTempData);
			ucCMD = PN512CMD_TRANSCEIVE;

#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{
#ifdef EM_RC531_Module

			ucTempData = 0x0F;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
			ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif
		}
	}
	else
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ucTempData = ucTempData;
            ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
            EG_mifs_tWorkInfo.expectMaxRec = 1+2; //DESELECT + CRC
#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
		{

#ifdef EM_RC663_Module

            ucTempData = 0x01;
            EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);
            ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;

#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{
#ifdef EM_PN512_Module

			EI_mifs_vSetBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
			EI_mifs_vSetBitMask(TxModeReg, 0x80);
			EI_mifs_vSetBitMask(RxModeReg, 0x80);
			ucCMD = PN512CMD_TRANSCEIVE;

#endif
		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{
#ifdef EM_RC531_Module

			ucTempData = 0x2C;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
			ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif
		}
	}

	while (1)
	{
		// ���ö�ʱ��
		EI_paypass_vSetTimer(EM_mifs_FWTDESELECT);

		EG_mifs_tWorkInfo.aucBuffer[0] = 0xC2;	// 1100 0010

		// ׼����������: ������
		EG_mifs_tWorkInfo.ulSendBytes = 1;	// how many bytes to send
		EG_mifs_tWorkInfo.ucDisableDF = 0;
		result = EI_mifs_ucHandleCmd(ucCMD);

		if ((result != EM_mifs_SUCCESS) && (result != EM_mifs_NOTAGERR))
		{
			result = EM_mifs_TRANSERR;
		}

		if (result == EM_mifs_NOTAGERR)
		{
			result = EM_mifs_TIMEOUT;
		}

		if ((result == EM_mifs_TRANSERR) || (result == EM_mifs_TIMEOUT))
		{

			i++;
			if (i > 2)
			{
				break;
			}
			// �ط�DESELECT����
			continue;
		}

		if (result == EM_mifs_SUCCESS)
		{
			if (EG_mifs_tWorkInfo.ulBytesReceived != 1)
			{
				result = EM_mifs_TRANSERR;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] != 0xC2)
			{
				result = EM_mifs_PROTERR;
			}
			break;
		}
	}

	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_NONE;

	return result;
}

/**********************************************************************
* �������ƣ�
*       EI_ucAnticoll(uchar ucSEL, uchar* pucUID)
* ����������
*       Ӧ�÷���ͻ���ƻ�ȡUID��Ϣ
* �����º������ã�
*       ��
* �������º�����
*       ��
* ���������
*       ucSEL   ��ͻ�ȼ� ȡֵ���£�
*              0x93   ��һ����ͻ
*              0x95   �ڶ�����ͻ
*              0x97   ��������ͻ
* ���������
*       pucUID  ��ȡ��UID��Ϣ������BCC�ֽ�
* �� �� ֵ��
*       EM_mifs_SUCCESS    ��ȡ�ɹ�
*       EM_mifs_TRANSERR   ͨ�Ŵ���
*       EM_mifs_TIMEOUT    ������Ӧ
*
*       ˵��
*       //��ͻλ�ò�����mod 8  ����4���ֽھ����λ�� ��ڶ��ֽ�bit4 ��Ϊ0x0D(PN512)����0x0C(CLRC663)
*       //RC531��ͻ�� ��ͻλ�Ժ�����(������ͻλ)
*       //PN512��CLR663��ͻ�� ��ͻλ�Ժ���1��1 (�������Ϊ0 �򲻻ᷢ��5���ֽ� ֻ���� ����ͻ���ڵ��ֽ���)
*       //PN512 bit0��ͻ��λ��Ϊ0(CollPosΪ0)  CLRC663Ϊ1
*       //for example
*       //card 1 uid:  59 4D E6 0C FE
*       //card 2 uid:  59 FD 08 21 8D
*       //RC531 ��ͻ����0     �յ� 59 1D 00 00 00 ��ͻλ�ڶ����ֽ�bit4 collposΪ0x0D(bit0��1)
*       //PN512 ��ͻ�����Ϊ1 �յ� 59 FD EE 2D FF ��ͻλ�ڶ����ֽ�bit4 collposΪ0x0D(bit0��1)
*       //RC663 ��ͻ�����Ϊ1 �յ� 59 FD EE 2D FF ��ͻλ�ڶ����ֽ�bit4 collposΪ0x0C(bit0��0)
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-22           ����
****************************************************************/
uchar EI_ucAnticoll(uchar ucSEL, uchar * pucUID)
{
#if 11
	uchar ucTempData;
	ulong ulBCnt;						// ��ǰҪ�������ݵĺϷ�λ��
	ulong ulBytes;						// ��ǰҪ�������ݵĺϷ��ֽ���
	uchar ucCompleted;					// ���������Ƿ����
	uchar ucTempVal,ucCMD = 0;
	uint i;
	uchar ucResult = 0;
    uchar select = 0,getcol = 0;
	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
//        ucCMD = EMV_HAL_TRANSCEIVE_WITHOUT_CRC;
//        EG_mifs_tWorkInfo.expectMaxRec = 5;
//		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
        ucTempData = 0;
        ucTempData = ucTempData;
        //�࿨ѡ��3911��api����
        ucResult = as3911AntiCollision(1, ucSEL, pucUID, EM_mifs_DEFAULT*128);
        EG_mifs_tWorkInfo.ucAnticol = 0;
        return ucResult;
#endif

    }
	else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
		//cf_RC663   ��ͻ��־
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
		ucTempData = 0x80;
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, ucTempData);
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);

		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXCRCCON, 0x01);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, 0x01);
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
		EI_mifs_vClrBitMask(TxModeReg, 0x80);
		EI_mifs_vClrBitMask(RxModeReg, 0x80);
		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(CollReg, 0x80);   //cf_pn512 �Ƿ�Ҫ��0 ������1
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(Status2Reg, 0x08);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		select = select;
		getcol = getcol;
		// ���ö�ʱ��
		EI_mifs_vSetTimer(2);
		// ��ͻ�����ݶ���0, Rx֡��ʽΪISO1443A
		ucTempData = 0x28;
		EI_mifs_vWriteReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);	// ZeroAfterColl aktivieren

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}


	memset(EG_mifs_tWorkInfo.aucCmdBuf, 0, 5);

	ucCompleted = 0;
	ulBCnt = 0;

	// ��������AntiColl���ֱ������ͻ�ɹ�
	while ((ucCompleted == 0) && (ucResult == EM_SUCCESS))
	{
		EG_mifs_tWorkInfo.ucDisableDF = 1;

        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
            ;
#endif

        }
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
		{

#ifdef EM_RC663_Module
			;
#endif

		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
		{

#ifdef EM_PN512_Module
			;
#endif

		}
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{
#ifdef EM_RC531_Module
			// ÿ���ֽڴ���У��
			ucTempData = 0x03;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
#endif
		}


		EG_mifs_tWorkInfo.ucnBits = (uchar) (ulBCnt % 8);
//        TRACE("\r\n1111------%x---%d",EG_mifs_tWorkInfo.ucnBits, ulBCnt);
		if (EG_mifs_tWorkInfo.ucnBits != 0)
		{
			// �յ��в���һ���ֽڵ�����
			// ���ݶ���Ĳ���һ���ֽڵ�λ�����ý���������ʼλ��ʹ������������������һ���ֽڣ�
			// �����һ������λ��,
			ucTempData = (EG_mifs_tWorkInfo.ucnBits % 0x10 * 0x10) | EG_mifs_tWorkInfo.ucnBits;

            if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
                ;
#endif

            }
			else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
			{

#ifdef EM_RC663_Module
                EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, 0x70);
                EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXDATANUM, 0x07);
                EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXDATANUM, (ucTempData&0x07));
//                ucTempData &= 0x70;
                EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, (ucTempData&0x70));
#endif

			}
			else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
			{

#ifdef EM_PN512_Module
				ucTempData &= 0x7F;
				EI_mifs_vWriteReg(1, BitFramingReg, &ucTempData);
#endif

			}
			else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
			{
#ifdef EM_RC531_Module
				EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);
#endif
			}

			ulBytes = ulBCnt / 8 + 1;	//������Ҫ���͵��ֽ���
            /*
			if (EG_mifs_tWorkInfo.ucnBits == 7) //yehf �ⲿ�ֿ��Բ�Ҫ
			{
				// ���ж��н��д���
				EG_mifs_tWorkInfo.lBitsReceived = 7;
				// ��һ���ֽڽ�����ʼλ�ø�Ϊ0
				EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, (uchar *) & EG_mifs_tWorkInfo.ucnBits);
			}
			*/
		}
		else
		{
			ulBytes = ulBCnt / 8;
		}

//        TRACE("\r\n2222---------%d",ulBytes);
		// ׼����������: SEL+NVB+�ϴγɹ����յ�����������
		EG_mifs_tWorkInfo.ucAnticol = ucSEL;
		EG_mifs_tWorkInfo.aucBuffer[0] = ucSEL;	// SEL
		// NVB: ��4λ = ��������кϷ�����λ��/8(����SEL��NVB,����������λ��)
		// ��4λ=��������кϷ�����λ�� mod 8
		EG_mifs_tWorkInfo.aucBuffer[1] =
			(uchar) (0x20 + ((ulBCnt / 8) * 0x10) + EG_mifs_tWorkInfo.ucnBits);

		for (i = 0; i < ulBytes; i++)
		{
			EG_mifs_tWorkInfo.aucBuffer[i + 2] = EG_mifs_tWorkInfo.aucCmdBuf[i];
		}
		EG_mifs_tWorkInfo.ulSendBytes = 2 + ulBytes;	// how many bytes to send

		// ִ������
		ucResult = EI_mifs_ucHandleCmd(ucCMD);
		/*
		if (EG_mifs_tWorkInfo.ucnBits == 7)  //yehf �ⲿ�ֿ��Բ�Ҫ
		{
			// �������������ͻ����, ����Ȼ�洢
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucBuffer[0] << 7;
			EG_mifs_tWorkInfo.ucCollPos = 8;
			ucResult = EM_mifs_COLLERR;
			EG_mifs_tWorkInfo.lBitsReceived = 40 - ulBCnt;
		}
		*/
		if ((ucResult == EM_SUCCESS) || (ucResult == EM_mifs_COLLERR))
		{
			// �������ݳ��ȴ���
			if (EG_mifs_tWorkInfo.lBitsReceived != (long)(40 - ulBCnt))	// not 5 bytes answered
			{
				ucResult = EM_mifs_BITCOUNTERR;
			}
			else
			{
                if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module
                    if ( ucResult == EM_mifs_COLLERR ) {
                        //����ײ�ݲ�ʵ��
                        ucCompleted = 1;
                    }

					// �½��յ����ݴӱ��η������ݲ�����ͻ��λ��ʼ���
					ucTempVal = 0;
					if (EG_mifs_tWorkInfo.ucnBits != 0)	// ���һ���ֽڲ�����
					{
						// ��������, ��FIFO�н��յ�ֻ�е�һ���ֽڲ�����
						// ���Լ����ϴν��յ����һ���ֽ���
						EG_mifs_tWorkInfo.aucCmdBuf[ulBytes - 1] |= EG_mifs_tWorkInfo.aucBuffer[0];
						ucTempVal = 1;
					}
					for (i = 0; i < (4 - ulBytes); i++)
					{
						EG_mifs_tWorkInfo.aucCmdBuf[i + ulBytes] =
							EG_mifs_tWorkInfo.aucBuffer[ucTempVal + i];
					}

                    if (ucResult == EM_SUCCESS)
                    {
                        // �ɹ����У���
                        ucTempVal = 0;
                        for (i = 0; i < 4; i++)
                        {
                            ucTempVal ^= EG_mifs_tWorkInfo.aucCmdBuf[i];
                        }
//                        DISPBUF(EG_mifs_tWorkInfo.aucCmdBuf,5,0);
                        if (ucTempVal !=
                            EG_mifs_tWorkInfo.aucBuffer[EG_mifs_tWorkInfo.ulBytesReceived - 1])
                        {
                            // У�����
                            ucResult = EM_mifs_SERNRERR;
                        }
                        else
                        {
                            //����ײ���
                            ucCompleted = 1;
                            memcpy(pucUID, EG_mifs_tWorkInfo.aucCmdBuf, 4);
                            pucUID[4] = ucTempVal;
                        }
                    }
					if (ucResult == EM_mifs_COLLERR) //����ǳ�ͻ ��Ϊ�ɹ�
						ucResult = EM_mifs_SUCCESS;
#endif
                }
				else if((EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
                   || (EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663))
				{

//#ifdef EM_PN512_Module
#if (defined(EM_PN512_Module) || defined(EM_RC663_Module))

					if (ucResult == EM_mifs_COLLERR) // ��ͻ
					{
                        if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
                        {

#ifdef EM_RC663_Module
                            ulBCnt = ulBCnt + EG_mifs_tWorkInfo.ucCollPos+1 - EG_mifs_tWorkInfo.ucnBits;
                            select = (EG_mifs_tWorkInfo.ucCollPos)/8;  //collposΪ0����bit0
                            getcol = (0x01<<((EG_mifs_tWorkInfo.ucCollPos%8)+1)) - 1;
                            EG_mifs_tWorkInfo.aucBuffer[select] = EG_mifs_tWorkInfo.aucBuffer[select]&getcol;

#endif

                        }
                        else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
                        {

#ifdef EM_PN512_Module
                            ulBCnt = ulBCnt + EG_mifs_tWorkInfo.ucCollPos - EG_mifs_tWorkInfo.ucnBits;
                            select = (EG_mifs_tWorkInfo.ucCollPos-1)/8;
                            if(!(EG_mifs_tWorkInfo.ucCollPos%8))   //collposΪ1����bit0  Ϊ8����bit0~bit7  Ϊ9����bit0
                                getcol = (0x01<<8)-1;
                            else
                                getcol = (0x01<<(EG_mifs_tWorkInfo.ucCollPos%8)) - 1;

                            EG_mifs_tWorkInfo.aucBuffer[select] = EG_mifs_tWorkInfo.aucBuffer[select]&getcol;
#endif

                        }
//                        TRACE("\r\n--%x---%x----%x---%x",(0x01<<8)-1, select, getcol,EG_mifs_tWorkInfo.aucBuffer[select]);
						//ucResult = EM_mifs_SUCCESS;
					}

					// �½��յ����ݴӱ��η������ݲ�����ͻ��λ��ʼ���
					ucTempVal = 0;
					if (EG_mifs_tWorkInfo.ucnBits != 0)	// ���һ���ֽڲ�����
					{
						// ��������, ��FIFO�н��յ�ֻ�е�һ���ֽڲ�����
						// ���Լ����ϴν��յ����һ���ֽ���
						EG_mifs_tWorkInfo.aucCmdBuf[ulBytes - 1] |= EG_mifs_tWorkInfo.aucBuffer[0];
						ucTempVal = 1;
					}
					for (i = 0; i < (4 - ulBytes); i++)
					{
						EG_mifs_tWorkInfo.aucCmdBuf[i + ulBytes] =
							EG_mifs_tWorkInfo.aucBuffer[ucTempVal + i];
					}

					if (ucResult == EM_SUCCESS)
					{
						// �ɹ����У���
						ucTempVal = 0;
						for (i = 0; i < 4; i++)
						{
							ucTempVal ^= EG_mifs_tWorkInfo.aucCmdBuf[i];
						}
						if (ucTempVal !=
							EG_mifs_tWorkInfo.aucBuffer[EG_mifs_tWorkInfo.ulBytesReceived - 1])
						{
							// У�����
							ucResult = EM_mifs_SERNRERR;
						}
						else
						{
							//����ײ���
							ucCompleted = 1;
							memcpy(pucUID, EG_mifs_tWorkInfo.aucCmdBuf, 4);
							pucUID[4] = ucTempVal;
						}
					}

//                    TRACE("\r\n3333--%x---%x----%d",ucResult, EG_mifs_tWorkInfo.ucCollPos, ulBCnt);
//                    DISPBUF(EG_mifs_tWorkInfo.aucCmdBuf, 5, 0);
					if (ucResult == EM_mifs_COLLERR) //����ǳ�ͻ ��Ϊ�ɹ�
						ucResult = EM_mifs_SUCCESS;
#endif

				}
				else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
				{

#ifdef EM_RC531_Module

                    // �½��յ����ݴӱ��η������ݲ�����ͻ��λ��ʼ���
                    ucTempVal = 0;
                    if (EG_mifs_tWorkInfo.ucnBits != 0)	// ���һ���ֽڲ�����
                    {
                        // ��������, ��FIFO�н��յ�ֻ�е�һ���ֽڲ�����
                        // ���Լ����ϴν��յ����һ���ֽ���
                        EG_mifs_tWorkInfo.aucCmdBuf[ulBytes - 1] |= EG_mifs_tWorkInfo.aucBuffer[0];
                        ucTempVal = 1;
                    }
                    for (i = 0; i < (4 - ulBytes); i++)
                    {
                        EG_mifs_tWorkInfo.aucCmdBuf[i + ulBytes] =
                            EG_mifs_tWorkInfo.aucBuffer[ucTempVal + i];
                    }
                    if (ucResult == EM_SUCCESS)
                    {
                        // �ɹ����У���
                        ucTempVal = 0;
                        for (i = 0; i < 4; i++)
                        {
                            ucTempVal ^= EG_mifs_tWorkInfo.aucCmdBuf[i];
                        }
                        if (ucTempVal !=
                            EG_mifs_tWorkInfo.aucBuffer[EG_mifs_tWorkInfo.ulBytesReceived - 1])
                        {
                            // У�����
                            ucResult = EM_mifs_SERNRERR;
                        }
                        else
                        {
                            //����ײ���
                            ucCompleted = 1;
                            memcpy(pucUID, EG_mifs_tWorkInfo.aucCmdBuf, 4);
                            pucUID[4] = ucTempVal;
                        }
                    }
                    else					// ��ͻ
                    {
                        // �������÷�������: ����һ����ͻλ��λ��֮ǰ�����ݷ��͸���
                        // ������ʹӵ�һ����ͻλ�Ժ������,��ý��յ�����ȷ��λ��
                        ulBCnt = ulBCnt + EG_mifs_tWorkInfo.ucCollPos - EG_mifs_tWorkInfo.ucnBits;
                        ucResult = EM_mifs_SUCCESS;
                    }

#endif

				}
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != (40 - ulBCnt))
		}								// if(ucResult == EM_SUCCESS || ucResult == EM_mifs_COLLERR)
	}									// while(ucCompleted == 0 && ucResult == EM_SUCCESS)

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module
        ;
#endif
    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, 0x70);
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXDATANUM, 0x07);
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module
		//EI_mifs_vSetBitMask(CollReg, 0x80);	//cf_pn512 �Ƿ�Ҫ��1
        EI_mifs_vClrBitMask(BitFramingReg, 0x07); //������Ϻ��������ֽ� chenf 20121204
        EI_mifs_vClrBitMask(BitFramingReg, 0x70); //������Ϻ���������ֽ� chenf 20121204
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// ��������
		EI_mifs_vClrBitMask(EM_mifs_REG_DECODERCONTROL, 0x20);	// ZeroAfterColl disable
#endif

	}

//    TRACE("\r\n4455-------------%x---",ucResult);
	EG_mifs_tWorkInfo.ucAnticol = 0;

	if (ucResult == EM_mifs_NOTAGERR)
	{
		ucResult = EM_mifs_TIMEOUT;
	}
	else if (ucResult != EM_mifs_SUCCESS)
	{
		ucResult = EM_mifs_TRANSERR;
	}

//    TRACE("\r\n4444-------------%x---",ucResult);
//    DISPBUF(pucUID, 5, 0);
	return ucResult;
#else
	uchar ucTempData;
	uchar ucResult = 0;
	ulong ulBCnt;			// ��ǰҪ�������ݵĺϷ�λ��
	ulong ulBytes,ulByteplus;			// ��ǰҪ�������ݵĺϷ��ֽ���
	uint i;

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	// ���ö�ʱ��
	EI_mifs_vSetTimer(2);

	// ��ͻ�����ݶ���0, Rx֡��ʽΪISO1443A
	ucTempData = 0x28;
	EI_mifs_vWriteReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);	// ZeroAfterColl aktivieren

	// ��ֹcrypto1��Ԫ
	EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);

	CLRBUF(EG_mifs_tWorkInfo.aucCmdBuf);
	EG_mifs_tWorkInfo.ucAnticol = ucSEL;
	ulBCnt = 0;
	//����ײ���̺��Դ���
	EG_mifs_tWorkInfo.ucDisableDF = 1;
	while(1)
	{
		// ÿ���ֽڴ���У��
		ucTempData = 0x03;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		EG_mifs_tWorkInfo.ucnBits = (uchar) (ulBCnt % 8);
		//������Ҫ���͵��ֽ���
		ulBytes = ulBCnt/8;
		if (EG_mifs_tWorkInfo.ucnBits != 0)
		{
			// �յ��в���һ���ֽڵ�����
			// ���ݶ���Ĳ���һ���ֽڵ�λ�����ý���������ʼλ�ͷ�����ʼλ
			ucTempData = (EG_mifs_tWorkInfo.ucnBits << 4) | EG_mifs_tWorkInfo.ucnBits;
			EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);
			//����һ���ֽڣ������ֽ�����1
			ulByteplus = 1;
		}
		else
		{
			ulByteplus = 0;
		}
		EG_mifs_tWorkInfo.aucBuffer[0] = ucSEL;
		//ȷ��Ҫ�������ݳ��Ⱥ�λ��
		EG_mifs_tWorkInfo.aucBuffer[1] = 0x20 + (uchar)(ulBytes<<4) + EG_mifs_tWorkInfo.ucnBits;
		memcpy(&EG_mifs_tWorkInfo.aucBuffer[2],EG_mifs_tWorkInfo.aucCmdBuf,ulBytes+ulByteplus);
		EG_mifs_tWorkInfo.ulSendBytes = 2+ulBytes+ulByteplus;
		// ִ������
		ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);

		if ((ucResult == EM_SUCCESS) || (ucResult == EM_mifs_COLLERR))
		{
			// �������ݳ��ȴ���
			if (EG_mifs_tWorkInfo.lBitsReceived != (long)(40 - ulBCnt))
			{
				ucResult = EM_mifs_BITCOUNTERR;
				break;
			}
			else
			{
				// �½��յ����ݴӱ��η������ݲ�����ͻ��λ��ʼ���
				// �г�ͻ�����ϴ��ֽڽ��ղ�����
				if(EG_mifs_tWorkInfo.ucnBits)
				{
					EG_mifs_tWorkInfo.aucCmdBuf[ulBytes] |= EG_mifs_tWorkInfo.aucBuffer[0];
					//ʣ��λ��
					EG_mifs_tWorkInfo.lBitsReceived -= (8-EG_mifs_tWorkInfo.ucnBits);
					memcpy(&EG_mifs_tWorkInfo.aucCmdBuf[ulBytes+ulByteplus],&EG_mifs_tWorkInfo.aucBuffer[1],
						EG_mifs_tWorkInfo.lBitsReceived/8);
				}
				else
				{
					memcpy(&EG_mifs_tWorkInfo.aucCmdBuf[ulBytes+ulByteplus],&EG_mifs_tWorkInfo.aucBuffer[0],
						EG_mifs_tWorkInfo.lBitsReceived/8);
				}

				//DATAIN(0x11);
				//DATAIN(EG_mifs_tWorkInfo.ucnBits);
				//BUFFIN(7,EG_mifs_tWorkInfo.aucCmdBuf);
				if (ucResult == EM_SUCCESS)
				{
					// û�г�ͻ����˵��UID�������
					//DATAIN(0x33);
					//BUFFIN(7,EG_mifs_tWorkInfo.aucCmdBuf);
					// �ɹ����У���
					ucTempData = 0;
					for (i=0;i<5;i++)
					{
						ucTempData ^= EG_mifs_tWorkInfo.aucCmdBuf[i];
					}
					if (ucTempData != 0)
					{
						// UIDУ�����
						ucResult = EM_mifs_SERNRERR;
					}
					else
					{
						memcpy(pucUID,&EG_mifs_tWorkInfo.aucCmdBuf[0],5);
					}
					break;
				}
				else
				{
					// ��ͻ
					// �������÷�������: ����һ����ͻλ��λ��֮ǰ�����ݷ��͸���
					// ������ʹӵ�һ����ͻλ�Ժ������,��ý��յ�����ȷ��λ��
					ulBCnt = ulBCnt + EG_mifs_tWorkInfo.ucCollPos - EG_mifs_tWorkInfo.ucnBits;
					//DATAIN(0x22);
					//DATAIN(LBYTE(ulBCnt));
				}
			}
		}
		else
		{
			break;
		}
	}
	// ��������
	EI_mifs_vClrBitMask(EM_mifs_REG_DECODERCONTROL, 0x20);	// ZeroAfterColl disable
	EG_mifs_tWorkInfo.ucAnticol = 0;

	if (ucResult == EM_mifs_NOTAGERR)
	{
		ucResult = EM_mifs_TIMEOUT;
	}
	else if (ucResult != EM_mifs_SUCCESS)
	{
		ucResult = EM_mifs_TRANSERR;
	}
	return ucResult;
#endif
}
#endif


