/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ�
*   Mifs.c
* ����ժҪ��
*   ���ļ�����ʵ�ַǽӴ����ӿڲ�ĳ���
* ��ʷ��¼��
*   �޸���      ����                �汾��      �޸ļ�¼
*   Ҷ����     2010-04-08                     ģ������ʱ���10s���ٵ�3s
******************************************************************/
#include "wp30_ctrl.h"

#ifdef CFG_RFID
/**********************************************************************

                          ȫ�ֱ�������

***********************************************************************/
const ulong EG_tulMaxLenList[16] =
	{ 16, 24, 32, 40, 48, 64, 96, 128, 256, 16, 16, 16, 16, 16, 16, 16 };

//const s_rfidpara_info rfidpara_rc531[4] =
//{{0,0x09}, {1,0x0B}, {2,0x34}, {3,0x3F}};
//
////�Ĵ���27h
//const s_rfidpara_info rfidpara_FM17550_V1[4] =
//{{0,0x82}, {1,0x82}, {2,0x82}, {3,0x82}};
////�Ĵ���28h
//const s_rfidpara_info rfidpara_FM17550_V1EX[4] =
//{{0,0x38}, {1,0x38}, {2,0x38}, {3,0x38}};
//
//#ifdef PRODUCT_F12
//const s_rfidpara_info rfidpara_as3911[4] =
//{{0,0xE2}, {1,0xCF}, {2,0xC0}, {3,0x00}};
//#else
//const s_rfidpara_info rfidpara_as3911[4] =
////{{0,0x8F}, {1,0x80}, {2,0x6F}, {3,0x00}};
////{{0,0xA0}, {1,0x90}, {2,0x80}, {3,0x70}};
//{{0,0xE0}, {1,0xDE}, {2,0xA0}, {3,0x90}};
//#endif

#define RFID_READY(x) {if(!x) return (-RFID_NODEVICE);}
/**********************************************************************

                          �ڲ���������

***********************************************************************/
uchar EI_mifs_ucBeginProtect(DevHandle * phDevHandle, uchar ucMode);
void EI_mifs_vEndProtect(void);
uchar EI_mifs_ucCommand(DevHandle hDevHandle, uchar ucType, uchar ucCID, uint uiSendLen,
	void *pvSendData, uint * puiRecLen, void *pvRecData);
uchar EI_mifs_ucProcess(uchar ucType, uint uiSendLen, uint * puiRecLen);
uchar EI_mifs_ucExchange(DevHandle hDevHandle, uchar ucType, uchar ucCID, uint uiSendLen,
	void *pvSendData, uint * puiRecLen, void *pvRecData);
uchar EI_mifs_ucDeselect(uchar ucType, uchar ucCID);
uchar EI_mifs_ucSendSBlock(uchar ucType, uchar ucCID, uint * puiRecLen);
uchar EI_mifs_ucSendRBlock(uchar ucType, uchar ucCID, uint * puiRecLen);
uchar EI_mifs_ucHandleATQA(uint uiLength);
uchar EI_mifs_ucHandleATQB(uint uiLength);
uchar EI_mifs_ucAuthentication(uchar ucKeyType, uchar ucSecNr);
extern int s_rfid_getPara(int module, int para, int index, s_rfidpara_info *rfdata);
int if_rfid_module(void);
/**********************************************************************

                          API��������

***********************************************************************/


/**********************************************************************
* �������ƣ�
*     EA_ucMifsOpen(uchar ucOpenMode)
* ����������
*   �򿪷ǽӴ����豸��������豸ʹ��Ȩ
* ���������
*   ucOpenMode          Mifsģ��Ĵ򿪷�ʽ
*           EM_mifs_TYPEA           ��TYPEA��ʽ��
*           EM_mifs_TYPEB           ��TYPEB��ʽ��
* ���������
*   ��
* ����ֵ��
*   EM_SUCCESS      �ɹ�
*   EM_ERRPARAM     ��������
*   EM_DEVICE_USED  �ǽӴ���ģ������ʹ���У��Ѿ����ú���EA_ucMifsOpen�����豸��
*   EM_TIMEOUT      ��ʼ��ʱ��ʱ
*   EM_FAILED       ��������ϵͳ����ȣ�
* ��ʷ��¼��
*   �޸���          ����            �汾��      �޸ļ�¼
***********************************************************************/
#define  OpenRFID_TIMEOUT  2100
uchar EA_ucMifsOpen(uchar ucOpenMode)
{
	uint timer0 = 0;
	uchar data;
	uchar ucResult;
	DevHandle *phDevHandle = EM_NULL;
	ucResult = EI_mifs_ucBeginProtect(phDevHandle, 0);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

//    if ( EG_mifs_tWorkInfo.ucFirstOpenFlag == 1 ) {
//		EI_paypass_vSelectType(ucOpenMode);
//        return EM_SUCCESS;
//    }
    if ( if_rfid_module() ) {
        //û����Ƶģ��
        return 1;
    }
    s_rfid_init();

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module  //AS3911
        data = 0;
        data = data;
//        memset(&gemvcardinfo, 0, sizeof(RFID_EMV_CARDINFO));

//        s_as3911_SetInt(OFF);
        /* Reset the AS3911 */
        as3911ExecuteCommand(AS3911_CMD_SET_DEFAULT);

        /* MCU_CLK and LF MCU_CLK off, 27MHz XTAL */
        as3911WriteRegister(AS3911_REG_IO_CONF1, 0x0F);
        as3911WriteRegister(AS3911_REG_IO_CONF2, 0x80);
        /* Enable Oscillator, Transmitter and receiver. */
        //��ʹ��bit 7����ͼĴ�������type b�������
        as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xD0);
        as3911WriteRegister(AS3911_REG_MODE, 0x08);

        s_DelayMs(10);

        //clear interrupt
        s_as3911ReadRegister(3, AS3911_REG_IRQ_MAIN, (uchar*)&timer0);

        //type b ���õ�����Ȼָ�Ĭ�� �������Ա�ָ�Ĭ��
        as3911SetModulationLevelMode(AS3911_MODULATION_LEVEL_FIXED, NULL);
        emvHalSetAs3911TypeBModulationMode(AS3911_MODULATION_LEVEL_FIXED, NULL);
        as3911SetGainMode(AS3911_GAIN_FIXED, NULL);

//        as3911WriteRegister(AS3911_REG_RFO_AM_OFF_LEVEL, 0x00); //����Ӳ������0x9F
        as3911WriteRegister(AS3911_REG_RFO_AM_OFF_LEVEL, gtRfidDebugInfo.CW_A); //���ݲ�������ȡ

        as3911WriteRegister(AS3911_REG_ANT_CAL_CONTROL, 0x80); //0x21 �������
        as3911ModifyRegister(AS3911_REG_RX_CONF3, 0xE0, 0xC0);
        gas3911Reg.gTypeArec = 0xD8;
        gas3911Reg.gTypeBrec = 0xD8;

//        as3911ModifyRegister(AS3911_REG_RX_CONF3, 0xE0, 0xE0);//���C���Ի� �����յ������
//        as3911WriteRegister(AS3911_REG_RX_CONF1, 0x40); //�޸�0AH bit6 ���C���Ի�

//        s_as3911_SetInt(ON);

        //type b �����������Ӧ
        as3911ModifyRegister(AS3911_REG_AM_MOD_DEPTH_CONTROL, 0x7E, 0x1E);
        as3911ClearInterrupts(AS3911_IRQ_MASK_DCT);
        as3911EnableInterrupts(AS3911_IRQ_MASK_DCT);
        as3911ExecuteCommand(AS3911_CMD_CALIBRATE_MODULATION);
        timer0 = 0;
        as3911WaitForInterruptTimed(AS3911_IRQ_MASK_DCT, 200, (ulong *)&timer0);
//        Dprintk("\r\n\r\n interrupt flag  :%x",timer0);
        if ( timer0 == AS3911_IRQ_MASK_DCT) {
            EG_mifs_tWorkInfo.ucFirstOpenFlag = 1;
            if ( s_rfid_mainVersion(0) == 1 ) {
                as3911WriteRegister(AS3911_REG_AM_MOD_DEPTH_CONTROL, 0x80);//�̶�type b�������
                as3911WriteRegister(AS3911_REG_RFO_AM_ON_LEVEL, gas3911Reg.gTypeBmodule);
            }
            if ( ucOpenMode ==  EM_mifs_NULL ) {
                //�ر��ز�
                as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xD0);
            }else{
                //���ز��ͽ��յ�·
                as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xD8);
            }
//            s_Rfid_SetInt(OFF); //port B�жϺ�����ģ�鸴�� ���ܹر�
        }else{
            //��ʼ��ʧ��
            as3911WriteRegister(AS3911_REG_OP_CONTROL, 0x00);
            EG_mifs_tWorkInfo.ucFirstOpenFlag = 0;
//            s_Rfid_SetInt(OFF); //port B�жϺ�����ģ�鸴�� ���ܹر�
            return 1;
        }
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module  //RC663
		timer0 = timer0;
		s_Rfid_SetInt(OFF);
		rfid_delayms(10);
		s_Rfid_SetRST(0);
		rfid_delayms(10);

		EG_mifs_tWorkInfo.ucFirstOpenFlag = 1;

        EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_IRQ0EN,0x80);
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_IRQ1EN,0x80);
        data = 0;
        EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_COMMAND, &data);
        data = 0x80;
        EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FIFOCONTROL, &data);
        EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOCONTROL, &data);
        if ( (data&0x80) == 0) {
            EG_mifs_tWorkInfo.FIFOSIZE = 512;
        }else{
            EG_mifs_tWorkInfo.FIFOSIZE = 255;
        }
		data = 0x86;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_DRVMODE, &data);
//		data = 0x15;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXAMP, &data);
//		data = 0x21;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_DRVCON, &data);
//		data = 0x0A;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXI, &data);
//		data = 0x18;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXCRCCON, &data);
//		data = 0x18;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCRCCON, &data);
		data = 0x08;//0x0F
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATANUM, &data);
//		data = 0x27;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXMODWIDTH, &data);
		data = 0x00;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10BURSTLEN, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXSOFD, &data);
//		data = 0xC0;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXWAITCTRL, &data);
//		data = 0xCF;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FRAMECON, &data);
//		data = 0x04;
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCTRL, &data);
        data = 0x90;
        EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXWAIT, &data);
//		data = 0x3F;//0x66;//0x32
//		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXTHRESHOLD, &data);
		data = 0x12;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RCV, &data);
		data = 0x0A;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXANA, &data);
		data = 0x04;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXDATACON, &data);
		data = 0x00;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM0H, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM2, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM3, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM32LEN, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TXSYM10BURSTCTRL, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXEOFSYM, &data);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXSYNCVALH, &data);
        data = 0x80;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_RXCORR, &data);
        data = 0xF0;
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_FABCALI, &data);
		s_Rfid_SetInt(ON);
		EI_paypass_vSelectType(ucOpenMode);//2012-05-08:EM_mifs_TYPEA
        EI_mifs_vEndProtect();
#endif

    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		timer0 = timer0;
//		s_rfid_init();
		s_Rfid_SetInt(OFF);
		s_Rfid_SetRST(0);
		rfid_delayms(1);
		s_Rfid_SetRST(1);
		rfid_delayms(10);

		EG_mifs_tWorkInfo.ucFirstOpenFlag = 1;

		data = BIT4;
		EI_mifs_vSetBitMask(ControlReg, data);	//initiator ���ͺ����

		data = gcMifReg.GsNOn;//0xF4;
		EI_mifs_vWriteReg(1, GsNOnReg, &data);
		rfid_delayms(1);
		ucResult = 0;
		EI_mifs_vReadReg(1, GsNOnReg, &ucResult);

		if(ucResult != data){
            s_Rfid_SetInt(ON);
			return ERROR;
        }

		data = 0x84;
		EI_mifs_vWriteReg(1, RxThresholdReg, &data);

		//data = 0x3B;
		//EI_mifs_vWriteReg(1, ModeReg, &data);

		data = 0x04;
		EI_mifs_vWriteReg(1, WaterLevelReg, &data);

		data = BIT7;
		EI_mifs_vSetBitMask(CommIEnReg, data);
		EI_mifs_vSetBitMask(DivIEnReg, data);

		data = 0x26;
		EI_mifs_vWriteReg(1, ModWidthReg, &data);

		// ��ֹ����������ж�Դ
		data = 0x7F;
		EI_mifs_vWriteReg(1, ComIrqReg, &data);
		EI_mifs_vWriteReg(1, DivIrqReg, &data);

		data = 0x00;
		EI_mifs_vWriteReg(1, CommandReg, &data);

		data = 0x15;
		EI_mifs_vWriteReg(1, TxSelReg, &data);

		data = 0x38;
		EI_mifs_vWriteReg(1, AnalogTestReg, &data);

		s_Rfid_SetInt(ON);
		EI_paypass_vSelectType(ucOpenMode);//2012-05-08:EM_mifs_TYPEA
		EI_mifs_vEndProtect();

#endif

	}
#ifdef EM_RC531_Module  //RC531
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
        s_Rfid_SetRST(0);
        rfid_delayms(1);
        s_Rfid_SetRST(1);
        rfid_delayms(1);
        s_Rfid_SetRST(0);

		// �����һ�ε��øú�������Ҫ��ʼ��RC531оƬ
		/*
		if (EG_mifs_tWorkInfo.ucFirstOpenFlag != 0)
		{
			goto mifsPowerOn;
		}
		*/
        // �ȴ���ʼ����ɣ���ʱ2.1��
        timer0 = sys_get_counter();
        rfid_delayms(2);
		// �ȴ���ʼ�׶ν���
		while (1)
		{
			EI_mifs_vReadReg(1, EM_mifs_REG_COMMAND, &data);
			if ((data & 0x3F) == 0x00)
			{
				break;
			}
			if(if_timerout(timer0,OpenRFID_TIMEOUT))
			{
				break;
			}
		}
        data = 0x80;
        EI_mifs_vWriteReg(1, EM_mifs_REG_PAGE, &data);	//init RC531 mcu
        while (1)
        {
            EI_mifs_vReadReg(1, EM_mifs_REG_COMMAND, &data);
            if (data == 0x00)
            {
                break;
            }
            if(if_timerout(timer0,OpenRFID_TIMEOUT))
            {
                return ERROR;
            }
        }
        EG_mifs_tWorkInfo.ucFirstOpenFlag = 1;
        //mifsPowerOn:

        data = 0x00;
        EI_mifs_vWriteReg(1, EM_mifs_REG_PAGE, &data);	//SPIģʽ

        // Q-Clock
        EI_mifs_vWriteReg(1, EM_mifs_REG_CLOCKQCONTROL, &data);
        data = 0x40;
        EI_mifs_vWriteReg(1, EM_mifs_REG_CLOCKQCONTROL, &data);
        rfid_delayms(1);  //100us
        EI_mifs_vClrBitMask(EM_mifs_REG_CLOCKQCONTROL, 0x40);

        data = 0xAD;
        EI_mifs_vWriteReg(1, EM_mifs_REG_BITPHASE, &data);  // ���ͺͽ���ʱ�ӵ���λ
        data = 0xFF;
        EI_mifs_vWriteReg(1, EM_mifs_REG_RXTHRESHOLD, &data);
        data = 0x06;
        EI_mifs_vWriteReg(1, EM_mifs_REG_RXWAIT, &data);

        // Rx���� Q-Clock
        data = 0x01;
        EI_mifs_vWriteReg(1, EM_mifs_REG_RXCONTROL2, &data);

        // FIFO����ȼ�: 4
        data = 0x04;
        EI_mifs_vWriteReg(1, EM_mifs_REG_FIFOLEVEL, &data);

        // �͵�ƽ�ж�, CMOS��׼
        data = 0x3;
        EI_mifs_vWriteReg(1, EM_mifs_REG_IRQPINCONFIG, &data);

        // Miller����������
        //data = 0x13;
        EI_mifs_vWriteReg(1, EM_mifs_REG_MODWIDTH, &gtRfidDebugInfo.ModWidth_A);

        // Tx2RF-En, Tx1RF-En�̶������ƽ
        data = 0x58;
        EI_mifs_vWriteReg(1,EM_mifs_REG_TXCONTROL, &data);

		// ��ֹ����������ж�Դ
		data = 0x7F;
		EI_mifs_vWriteReg(1, EM_mifs_REG_INTERRUPTEN, &data);
		EI_mifs_vWriteReg(1, EM_mifs_REG_INTERRUPTRQ, &data);
		// ʹ���ж�
		s_Rfid_SetInt(ON);
		EI_paypass_vSelectType(ucOpenMode);//2012-05-08:EM_mifs_TYPEA
		EI_mifs_vEndProtect();
    }
#endif
    else{
        //û����Ƶģ��
        return 1;
    }
	return 0;
}

/**********************************************************************
* �������ƣ�
*    EA_ucMifsClose(void)
* ����������
*   �򿪷ǽӴ����豸��������豸ʹ��Ȩ
* ���������
*   ��
* ���������
*   ��
* ����ֵ��
*   EM_SUCCESS      �ɹ�
*   EM_FAILED       ��������ϵͳ����ȣ�
* ��ʷ��¼��
*   �޸���          ����            �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMifsClose(void)
{
	EI_mifs_ucBeginProtect(0, 0);
	if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911)
	{

#ifdef EM_AS3911_Module

        //power down mode
//        emvHalActivateField(OFF);
        as3911WriteRegister(AS3911_REG_IO_CONF1, 0x0F);
        as3911WriteRegister(AS3911_REG_IO_CONF2, 0x80);
        /* Enable Oscillator, Transmitter and receiver. */
        as3911WriteRegister(AS3911_REG_OP_CONTROL, 0x00);
        as3911WriteRegister(AS3911_REG_MODE, 0x08);
//		s_Rfid_SetInt(OFF); //port B�жϺ�����ģ�鸴�� ���ܹر�

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module

		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_DRVMODE, PHHAL_HW_RC663_BIT_TXEN);

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		EI_mifs_vClrBitMask(TxControlReg, 0x03);

#endif

	}
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
		EI_mifs_vClrBitMask(EM_mifs_REG_TXCONTROL, 0x03);
        //S980֧�ֵ͹���ģʽ
#if defined(PRODUCT_F16_2)
        EI_mifs_vSetBitMask(EM_mifs_REG_CONTROL, 1<<4);  //����powerdownģʽ
#endif
	}
#endif
	/*
	uchar ucResult;
	uchar ucTempData;
	DevHandle *phDevHandle = EM_NULL;

	ucResult = EI_mifs_ucBeginProtect(phDevHandle, 0);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	// TX1��TX2�̶����
	EI_mifs_vClrBitMask(EM_mifs_REG_TXCONTROL, 0x03);

	// ��ֹ����������ж�Դ
	ucTempData = 0x7F;
	EI_mifs_vWriteReg(1, EM_mifs_REG_INTERRUPTEN, &ucTempData);
	EI_mifs_vWriteReg(1, EM_mifs_REG_INTERRUPTRQ, &ucTempData);
	s_Rfid_SetInt(OFF);
	//��λ��RST�ø�
	s_Rfid_SetRST(1);
	EI_mifs_vEndProtect();
	*/
	EG_mifs_tWorkInfo.ucFirstOpenFlag = 0;
	EI_mifs_vEndProtect();
	return EM_SUCCESS;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFSetTime(DevHandle hDevHandle, uchar ucTime)
* ����������
*     �ı俨�ĳ�ʱ�ȴ�ʱ��
* ���������
*     ucTime: ��ʱʱ�䣬��Χ0~14
* ���������
*     ��
* ����ֵ��
*     EM_SUCCESS    �ɹ�
*     EM_ERRHANDLE  ����ľ��
*     EM_ERRPARAM   ��������(������ucType����)
*     EM_FAILED ��������ϵͳ����ȣ�
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFSetTime(DevHandle hDevHandle, uchar ucTime)
{
	if (ucTime > EM_mifs_FWI_MAXVALUE)
	{
		return EM_ERRPARAM;
	}
	EG_mifs_tWorkInfo.ucFWI = ucTime;
	return EM_SUCCESS;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFSetRegVal(DevHandle hDevHandle , uchar ucAddr , uchar ucVal)
* ����������
*    ����ָ���Ĵ�������ֵ
* ���������
*     hDevHandle     �豸���
*     ucAddr		 �Ĵ�����ַ
*     ucVal		 ����ֵ
* ���������
*     ��
* ����ֵ��
*     EM_SUCCESS    �ɹ�
*     EM_ERRHANDLE  ����ľ��
*     EM_ERRPARAM   ��������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFSetRegVal(DevHandle hDevHandle, uchar ucAddr, uchar ucVal)
{
	uchar ucResult;

	// �жϼĴ�����ַ�Ƿ���Ч
	if ((ucAddr >= 0x11 && ucAddr <= 0x26) || ucAddr == 0x3D)
	{
		if (ucAddr == 0x16 || ucAddr == 0x18 || ucAddr == 0x1D || ucAddr == 0x20 || ucAddr == 0x25)
		{
			return EM_ERRPARAM;
		}
	}
	else
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 3);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}
	EI_mifs_vWriteReg(1, ucAddr, &ucVal);

	EI_mifs_vEndProtect();
	return EM_SUCCESS;
}

/**********************************************************************

                         Mifare����������

***********************************************************************/
/**********************************************************************
* �������ƣ�
*     EA_ucMIFRequest(DevHandle hDevHandle, uchar ucMode, ushort *pusTagType)
* ����������
*     ��ʾ���߸�Ӧ��Χ�ڵĿ�����һ����Ƭ����ʶ���
* ���������
*    hDevHandle     �豸���
*     ucMode
                   EM_mifs_IDLE -- REQUEST IDLEֻ��һ�ſ���λ����ֻ�е��ÿ��뿪��Ƶ���ߵ���
                                   Ч��Χ���ٽ���ʱ�� �ú������ٴ���Ч��(�Ƽ�ʹ�ø�ģʽ)
                   EM_mifs_HALT -- REQUEST ALL����Ƶ���ߵ���Ч��Χ�ڵ����п���λ��
* ���������
      pusTagType������λʱ���صĿ�Ƭ����ʶ���
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���       ����              �汾��
***********************************************************************/
uchar EA_ucMIFRequest(DevHandle hDevHandle, uchar ucMode, ushort * pusTagType)
{
	uchar ucTempData;
	uchar ucResult;
	uchar ucCMD = 0;

	if (pusTagType == EM_NULL)
	{
		return EM_ERRPARAM;
	}
	/*
	 * if ((ucMode != EM_mifs_HALT) && (ucMode != EM_mifs_IDLE))
	 * {
	 * return EM_ERRPARAM;
	 * }
	 */
	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}
	EG_mifs_tWorkInfo.ucMifCardType = EM_mifs_S50;
	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = 0;
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WUPA;
        EG_mifs_tWorkInfo.expectMaxRec = 2;
        EI_paypass_vSelectType(EM_mifs_TYPEA);
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module

//        EI_paypass_vSelectType(EM_mifs_TYPEA); //�˺���ֻ��mifare��ʹ�� type a
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
		// ��ֹ RxCRC �� TxCRC , ������żУ��
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);
        //��ͻ��Ҫ����?
//		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXBITCTRL, 0x80);
        ucTempData = 0x01;
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);

		// set TxLastBits to 7, ��֡��ʽ
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXDATANUM, 0x07);

		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
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
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
		// ���ö�ʱ��
		EI_mifs_vSetTimer(6);

		// ÿ���ֽڴ���У��
		ucTempData = 0x03;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);

		// set TxLastBits to 7
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);

		ucCMD = EM_mifs_PCD_TRANSCEIVE;
	}
#endif

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = 1;
	EG_mifs_tWorkInfo.ucDisableDF = 1;
	if (ucMode == EM_mifs_HALT)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_REQALL;
	}
	else
	{
		EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_REQSTD;
	}

	ucResult = EI_mifs_ucHandleCmd(ucCMD);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

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

	// �����������
	// �������յ����������ֽ�: ������ʶ���(2�ֽ�)
	if ((ucResult == EM_SUCCESS) && (EG_mifs_tWorkInfo.lBitsReceived != 16))
	{
		// ��������λ����
		ucResult = EM_mifs_BITCOUNTERR;
	}

	// λ��ͻ�����Ѿ����յ�16���ֽ�( ���н��յ�����������������)
	if ((ucResult == EM_mifs_COLLERR) && (EG_mifs_tWorkInfo.lBitsReceived == 16))
	{
		ucResult = EM_SUCCESS;
	}

	// �������������û�н��յ����ݣ�����ս���������
	if ((ucResult != EM_SUCCESS) || (EG_mifs_tWorkInfo.ulBytesReceived == 0))
	{
		EG_mifs_tWorkInfo.aucBuffer[0] = 0;
		EG_mifs_tWorkInfo.aucBuffer[1] = 0;
	}
	else if ((EG_mifs_tWorkInfo.ulBytesReceived == 1) && (ucResult == EM_SUCCESS))
	{
		// ֻ�յ�һ���ֽ�
		EG_mifs_tWorkInfo.aucBuffer[1] = 0;
	}

	// ������յ������ͺ�
	memcpy(pusTagType, EG_mifs_tWorkInfo.aucBuffer, 2);

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFAntiColl(DevHandle hDevHandle,ulong *pulSnr)
* ����������
*     �����д������߸�Ӧ��Χ�ڵĿ���ѡ��һ�ţ����������Ŀ��š�(����ͻ����)
* ���������
*    hDevHandle     �豸���
* ���������
*     pulSnr -- �����к�
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFAntiColl(DevHandle hDevHandle, ulong * pulSnr)
{
	uchar ucTempData;
	ulong ulBCnt;						// ��ǰҪ�������ݵĺϷ�λ��
	ulong ulBytes;						// ��ǰҪ�������ݵĺϷ��ֽ���
	uchar ucCompleted;					// ���������Ƿ����
	uchar ucTempVal;
	uint i;
	uchar ucResult;
    uchar select = 0,getcol = 0;
	uchar ucCMD = 0;

	if (pulSnr == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        select = select;getcol = getcol;
        ucTempData = 0;
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITHOUT_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 5;
//        EI_paypass_vSelectType(EM_mifs_TYPEA);
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module

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
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
		select = select;
		getcol = getcol;
		// ���ö�ʱ��
		EI_mifs_vSetTimer(2);

		// ��ͻ�����ݶ���0, Rx֡��ʽΪISO14443A
		ucTempData = 0x28;
		EI_mifs_vWriteReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);	// ZeroAfterColl aktivieren

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
	}
#endif
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
#ifdef EM_RC531_Module
		else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
		{
			// ÿ���ֽڴ���У��
			ucTempData = 0x03;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		}
#endif
		EG_mifs_tWorkInfo.ucnBits = (uchar) (ulBCnt % 8);
		if (EG_mifs_tWorkInfo.ucnBits != 0)
		{
			// �в���һ���ֽڵ�����
			// ���ݶ���Ĳ���һ���ֽڵ�λ�����ý���������ʼλ��ʹ������������������һ���ֽڣ�
			// �����һ������λ��, ����յ�����λ����ʼλ�ͷ��ͽ���λ,��ucnBits=1,��ucTempData=0x11
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
#ifdef EM_RC531_Module
			else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
			{
				// ����Ҫ���͵�ֹͣλ
				EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);
			}
#endif
			ulBytes = ulBCnt / 8 + 1;	//������Ҫ���͵��ֽ���

#ifdef EM_RC531_Module
			if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
			{
				if (EG_mifs_tWorkInfo.ucnBits == 7)
				{
					// ���ж��н��д���
					EG_mifs_tWorkInfo.lBitsReceived = 7;
					// ��һ���ֽڽ�����ʼλ�ø�Ϊ0
					EI_mifs_vWriteReg(1, EM_mifs_REG_BITFRAMING, (uchar *) & EG_mifs_tWorkInfo.ucnBits);
				}
			}
#endif
		}
		else
		{
			ulBytes = ulBCnt / 8;
		}

		// ׼����������: SEL+NVB+�ϴγɹ����յ�����������
		EG_mifs_tWorkInfo.ucAnticol = EM_mifs_PICC_ANTICOLL1;
		EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_ANTICOLL1;	// SEL
		// NVB: ��4λ = ��������кϷ�����λ��/8(����SEL��NVB,����������λ��)
		// ��4λ=��������кϷ�����λ�� mod 8
		// *0x10�൱������4λ��ʾ�ֽ���
		EG_mifs_tWorkInfo.aucBuffer[1] =
			(uchar) (0x20 + ((ulBCnt / 8) * 0x10) + EG_mifs_tWorkInfo.ucnBits);

		for (i = 0; i < ulBytes; i++)
		{
			EG_mifs_tWorkInfo.aucBuffer[i + 2] = EG_mifs_tWorkInfo.aucCmdBuf[i];
		}
		EG_mifs_tWorkInfo.ulSendBytes = 2 + ulBytes;	// how many bytes to send

		// ִ������
		ucResult = EI_mifs_ucHandleCmd(ucCMD);
		//���͵�7λ
		if (EG_mifs_tWorkInfo.ucnBits == 7)
		{
			// �������������ͻ����, ����Ȼ�洢
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucBuffer[0] << 7;
			EG_mifs_tWorkInfo.ucCollPos = 8;
			ucResult = EM_mifs_COLLERR;
			EG_mifs_tWorkInfo.lBitsReceived = 40 - ulBCnt;
		}

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
						}
					}

					if (ucResult == EM_mifs_COLLERR) //����ǳ�ͻ ��Ϊ�ɹ�
						ucResult = EM_mifs_SUCCESS;

#endif

				}
#ifdef EM_RC531_Module
				else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
				{
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
                    if (ucResult != EM_mifs_COLLERR)	// û�д������
                    {
                        // ���У���
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
                            ucCompleted = 1;
                        }
                    }
                    else					// ��ͻ
                    {
                        // �������÷�������: ����һ����ͻλ��λ��֮ǰ�����ݷ��͸���
                        // ������ʹӵ�һ����ͻλ�Ժ������
                        ulBCnt = ulBCnt + EG_mifs_tWorkInfo.ucCollPos - EG_mifs_tWorkInfo.ucnBits;
                        ucResult = EM_SUCCESS;
                    }
				}
#endif
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != (40 - ulBCnt))
		}								// if(ucResult == EM_SUCCESS || ucResult == EM_mifs_COLLERR)
	}									// while(ucCompleted == 0 && ucResult == EM_SUCCESS)

	if (ucResult == EM_SUCCESS)
	{
		// ����ȡ���Ŀ���
		memcpy(pulSnr, EG_mifs_tWorkInfo.aucCmdBuf, 4);
	}

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
#ifdef EM_PN512_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
		//EI_mifs_vSetBitMask(CollReg, 0x80);	//cf_pn512 �Ƿ�Ҫ��1
        EI_mifs_vClrBitMask(BitFramingReg, 0x07); //������Ϻ��������ֽ� chenf 20121204
        EI_mifs_vClrBitMask(BitFramingReg, 0x70); //������Ϻ���������ֽ� chenf 20121204
	}
#endif
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
		// ��������
		EI_mifs_vClrBitMask(EM_mifs_REG_DECODERCONTROL, 0x20);	// ZeroAfterColl disable
	}
#endif
	EG_mifs_tWorkInfo.ucAnticol = 0;

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*      EA_ucMIFSelect(DevHandle hDevHandle,ulong ulSnr, ushort *pusSize)
* ����������
*     ѡ����ܴ�������Ŀ�
* �����º�������
*     API
* �������º�����
*     �²㺯��
* ���������
*     hDevHandle        �豸���
*     ulSnr   -- ����
* ���������
*     puiSize -- ��ѡ��Ŀ��������Ĵ洢���Ĵ�С(kbit)
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EA_ucMIFSelect(DevHandle hDevHandle, ulong ulSnr, uint * puiSize)
{
	uchar ucTempData = 0;
	uint i;
	uchar ucResult;
	uchar ucCMD = 0;

	if (puiSize == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucCMD = EMV_HAL_TRANSCEIVE_WITH_CRC;
        EG_mifs_tWorkInfo.expectMaxRec = 1+2;
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = ucTempData;
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
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

		ucTempData = ucTempData;
		EI_paypass_vSetTimer(EM_mifs_DEFAULT);
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, 0x80);
		EI_mifs_vSetBitMask(RxModeReg, 0x80);

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(Status2Reg, 0x08);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
		EI_mifs_vSetTimer(106);
		// ���ͺͽ���֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x0F;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);	// disable crypto 1 unit
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
	}
#endif
	EG_mifs_tWorkInfo.ulSendBytes = 7;
	EG_mifs_tWorkInfo.ucDisableDF = 1;

	EG_mifs_tWorkInfo.ulCardID = ulSnr;	// ���浱ǰѡ�п���

	// ׼����������: 0x93+0x70+4�ֽڿ���+����У��
	memcpy(EG_mifs_tWorkInfo.aucBuffer + 2, (void *)&ulSnr, 4);
	EG_mifs_tWorkInfo.aucBuffer[6] = 0;
	for (i = 0; i < 4; i++)
	{
		EG_mifs_tWorkInfo.aucBuffer[6] ^= EG_mifs_tWorkInfo.aucBuffer[i + 2];
	}
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_ANTICOLL1;
	EG_mifs_tWorkInfo.aucBuffer[1] = 0x70;

	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	// ��������: ��������С(1�ֽ�)
	if (ucResult == EM_SUCCESS)
	{
		// һ���ֽ�û������
		if (EG_mifs_tWorkInfo.lBitsReceived != 8)
		{
			ucResult = EM_mifs_BITCOUNTERR;
		}
	}
	*puiSize = (uint) EG_mifs_tWorkInfo.aucBuffer[0];

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFAuthExtend(DevHandle hDevHandle,uchar ucMode, uchar ucAddr)
* ����������
*     ʹ�ô���ڷǽӴ���ģ���е���Կ�Կ�Ƭָ������������֤
* ���������
*    hDevHandle     �豸���
*    ucMode�� 0--15     A�׵�1��16������
*             16��31    B�׵�1��16������
*    ucAddr��������Կ���ڿ��
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2005-05-18          01-01-01    ����
* ��    ע��
***********************************************************************/
uchar EA_ucMIFAuthExtend(DevHandle hDevHandle, uchar ucMode, uchar ucAddr)
{
#ifdef EM_RC531_Module
	uchar ucTempData;
	ushort usAddr;
	uchar ucResult;

	usAddr = ucMode;
	if (usAddr < EM_mifs_KEYNUM)
	{
		usAddr = usAddr * 0x18 + 0x80;
	}
	else if (usAddr < EM_mifs_KEYNUM * 2)
	{
		usAddr = (usAddr - EM_mifs_KEYNUM) * 0x18 + 0x8C;
	}
	else
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

/**********************************************************************
 ������Կ����
***********************************************************************/
	// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
	ucTempData = 0x07;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

	// ���ö�ʱ��
	EI_mifs_vSetTimer(2);

	// ׼����������: ��Կ��ַ(2)
	EG_mifs_tWorkInfo.aucBuffer[0] = (uchar) usAddr;
	EG_mifs_tWorkInfo.aucBuffer[1] = (uchar) (usAddr >> 8);
	EG_mifs_tWorkInfo.ulSendBytes = 2;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;

	EI_mifs_vFlushFIFO();				// empty FIFO

	// ִ������
	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_LOADKEYE2);
	if (ucResult != EM_SUCCESS)
	{
		goto mifsAuthorEnd;
	}

	if (ucMode < EM_mifs_KEYNUM)
	{
		ucResult = EI_mifs_ucAuthentication(EM_mifs_KEYA, ucAddr);
	}
	else
	{
		ucResult = EI_mifs_ucAuthentication(EM_mifs_KEYB, ucAddr);
	}

  mifsAuthorEnd:
	EI_mifs_vEndProtect();
	return ucResult;

#else

	//ʹ��EEROM����Կ��֤���� PN512��֧��EEROM��洢��Կ ��API��֧��
	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*      EA_ucMIFAuthentication(DevHandle hDevHandle,uchar ucMode, uchar ucSecNr)
* ����������
*     ʹ�ô���ڷǽӴ���ģ���е���Կ�Կ�Ƭָ������������֤
* ���������
*    hDevHandle     �豸���
*    ucMode�� 0--15     A�׵�1��16������
*             16��31    B�׵�1��16������
*    ucSecnr��������
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
* ��    ע��
***********************************************************************/
uchar EA_ucMIFAuthentication(DevHandle hDevHandle, uchar ucMode, uchar ucSecNr)
{
#ifdef EM_RC531_Module
	return EA_ucMIFAuthExtend(hDevHandle, ucMode,
		(uchar) ((ucSecNr + 1) * EM_mifs_MIFARE_BLOCKNUMPERSECTOR - 1));
#else
	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;
#endif

}

/**********************************************************************
* �������ƣ�
*      EA_ucMIFAuthToRam(DevHandle hDevHandle, uchar ucSecNr,uchar ucKeyType,
*                               uchar *pucKey);
* ����������
*     �����ṩ����Կ��ָ��������������֤
* ���������
*    hDevHandle     �豸���
*    ucKeyType EM_mifs_KEYA A������
*              EM_mifs_KEYB B������
*    ucSecnr������S50�ṹ��Mifare One�����������ţ�0--15��
*             ����S70�ṹ��Mifare One�����������ţ�0--39��
*    pucKey: �����Ҫ������֤����Կ
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFAuthToRam(DevHandle hDevHandle, uchar ucSecNr, uchar ucKeyType, uchar * pucKey)
{
	uchar i = 0, j = 0, k = 0;
	uchar ucResult;

	if (pucKey == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	if ((ucKeyType != EM_mifs_KEYA) && (ucKeyType != EM_mifs_KEYB))
	{
		return EM_ERRPARAM;
	}

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucSecNr > 15)
		{
			return EM_ERRPARAM;
		}
	}
	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S70)
	{
		if (ucSecNr > 39)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        i = i;
        j = j;
        k = k;
        memcpy(gtRfidProInfo.gMifareKey,pucKey,6);//�ݴ���Կ
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module

		i = i;
		j = j;
		k = k;
		memcpy(gtRfidProInfo.gMifareKey,pucKey,6);//�ݴ���Կ
		//ucResult = EI_mifs_ucAuthentication(ucKeyType, (uchar) ((ucSecNr + 1) * EM_mifs_MIFARE_BLOCKNUMPERSECTOR - 1));

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		i = i;
		j = j;
		k = k;
		memcpy(gtRfidProInfo.gMifareKey,pucKey,6);//�ݴ���Կ
		//ucResult = EI_mifs_ucAuthentication(ucKeyType, (uchar) ((ucSecNr + 1) * EM_mifs_MIFARE_BLOCKNUMPERSECTOR - 1));

#endif

	}
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{


/**********************************************************************
 ������Կ
***********************************************************************/
        // ׼����������: ��Կ(12�ֽ�),6�ֽ���Կ��չΪ12�ֽ�
        // ÿ���ֽڽ���ȡ����չ(���ֽ�ȡ��)
        for (i = 0; i < 6; i++)
        {
            k = (pucKey[i] & 0xF0) >> 4;
            j = pucKey[i] & 0x0F;
            EG_mifs_tWorkInfo.aucBuffer[i * 2] = ((k << 4) ^ 0xF0) + k;
            EG_mifs_tWorkInfo.aucBuffer[i * 2 + 1] = ((j << 4) ^ 0xF0) + j;
        }

        EI_mifs_vFlushFIFO();				// empty FIFO

        // ���ö�ʱ��
        EI_mifs_vSetTimer(4);
        EG_mifs_tWorkInfo.ulSendBytes = 12;	// how many bytes to send
        EG_mifs_tWorkInfo.ucDisableDF = 0;

        // ִ������
        ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_LOADKEY);
        if (ucResult != EM_SUCCESS)
        {
            //goto mifsAuthorRamEnd;
            EI_mifs_vEndProtect();
            return ucResult;
        }
	}
#endif
	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		ucResult =
			EI_mifs_ucAuthentication(ucKeyType,
			(uchar) ((ucSecNr + 1) * EM_mifs_MIFARE_BLOCKNUMPERSECTOR - 1));
	}
	else if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S70)
	{
		if (ucSecNr < 32)
		{
			// ǰ���32��������Կ�洢�ڵ�3��Block
			ucResult =
				EI_mifs_ucAuthentication(ucKeyType,
				(uchar) ((ucSecNr + 1) * EM_mifs_MIFARE_BLOCKNUMPERSECTOR - 1));
		}
		else
		{
			// ����8����������Կ�洢�ڵ�15��Block
			ucResult =
				EI_mifs_ucAuthentication(ucKeyType, (uchar) (32 * 4 + (ucSecNr - 32) * 16 + 15));
		}
	}
	else
	{
		//���߼����ܿ�
		ucResult = 0xFF;
	}

  //mifsAuthorRamEnd:
	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     uchar EA_ucMIFAuthToRamExtend(DevHandle hDevHandle, uchar ucAddr,uchar ucKeyType,
*                               uchar *pucKey);
* ����������
*     �����ṩ����Կ��ָ��������������֤
* ���������
*    hDevHandle     �豸���
*    ucKeyType EM_mifs_KEYA A������
*              EM_mifs_KEYB B������
*    ucAddr��������Կ���ڿ��
*    pucKey: �����Ҫ������֤����Կ
* ���������
*     ��
* ����ֵ��
*     ��֤ʧ��������ֵ
*		EM_mifs_NOTAGERR----�޿�
*		EM_mifs_AUTHERR ----��������֤
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFAuthToRamExtend(DevHandle hDevHandle, uchar ucAddr, uchar ucKeyType, uchar * pucKey)
{
	uchar i = 0, j = 0, k = 0;
	uchar ucResult;

	if ((ucKeyType != EM_mifs_KEYA) && (ucKeyType != EM_mifs_KEYB))
	{
		return EM_ERRPARAM;
	}

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        i = i;
        j = j;
        k = k;
		memcpy(gtRfidProInfo.gMifareKey,pucKey,6);//�ݴ���Կ
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module

		i = i;
		j = j;
		k = k;
		memcpy(gtRfidProInfo.gMifareKey,pucKey,6);//�ݴ���Կ

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		i = i;
		j = j;
		k = k;
		memcpy(gtRfidProInfo.gMifareKey,pucKey,6);//�ݴ���Կ

#endif

	}
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
/**********************************************************************
  ������Կ
 ***********************************************************************/
        // ׼����������: ��Կ(12�ֽ�)
        for (i = 0; i < 6; i++)
        {
            k = (pucKey[i] & 0xF0) >> 4;
            j = pucKey[i] & 0x0F;
            EG_mifs_tWorkInfo.aucBuffer[i * 2] = ((k << 4) ^ 0xF0) + k;
            EG_mifs_tWorkInfo.aucBuffer[i * 2 + 1] = ((j << 4) ^ 0xF0) + j;
        }

        EI_mifs_vFlushFIFO();				// empty FIFO

        // ���ö�ʱ��
        EI_mifs_vSetTimer(4);
        EG_mifs_tWorkInfo.ulSendBytes = 12;	// how many bytes to send
        EG_mifs_tWorkInfo.ucDisableDF = 0;

        // ִ������
        ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_LOADKEY);
        if (ucResult != EM_SUCCESS)
        {
            //goto mifsAuthorRamEnd;
            EI_mifs_vEndProtect();
            return ucResult;
        }
	}
#endif
	ucResult = EI_mifs_ucAuthentication(ucKeyType, ucAddr);

  //mifsAuthorRamEnd:
	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     uchar EA_ucMIFLoadKey(DevHandle hDevHandle,uchar ucMode, uchar *pucNKey);
* ����������
*     ��ָ�����������봫���ǽӴ���ģ��
* ���������
*    hDevHandle     �豸���
*    ucMode�� 0--15     A�׵�1��16������
*             16��31    B�׵�1��16������
*    pucNKey  Ҫ���͵����뼴ʵ���뿨������ѯ�����루6�ֽڣ�
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
* ��    ע��
***********************************************************************/
uchar EA_ucMIFLoadKey(DevHandle hDevHandle, uchar ucMode, uchar * pucNKey)
{
#ifdef EM_RC531_Module
	uchar ucTemp1, ucTemp2;
	uint i;
	uchar ucResult;
	ushort usAddr;

	if (pucNKey == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	usAddr = ucMode;
	if (usAddr < EM_mifs_KEYNUM)
	{
		usAddr = usAddr * 0x18 + 0x80;
	}
	else if (usAddr < EM_mifs_KEYNUM * 2)
	{
		usAddr = (usAddr - EM_mifs_KEYNUM) * 0x18 + 0x8C;
	}
	else
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	// ׼����������: EEPROM��ַ(2)+��Կ��12��
	EG_mifs_tWorkInfo.ulSendBytes = 14;

	// ���EEPROM��ַ
	EG_mifs_tWorkInfo.aucBuffer[0] = (uchar) usAddr;
	EG_mifs_tWorkInfo.aucBuffer[1] = (uchar) (usAddr >> 8);

	// ����Կ�����EEPROM�涨�ĸ�ʽ
	for (i = 0; i < 6; i++)
	{
		ucTemp1 = pucNKey[i] >> 4;
		ucTemp2 = pucNKey[i] & 0x0F;
		EG_mifs_tWorkInfo.aucBuffer[i * 2 + 2] = (~ucTemp1 << 4) | ucTemp1;
		EG_mifs_tWorkInfo.aucBuffer[i * 2 + 3] = (~ucTemp2 << 4) | ucTemp2;
	}
	EG_mifs_tWorkInfo.ucDisableDF = 0;

	// long timeout
	EI_mifs_vSetTimer(9);

	// ��������
	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_WRITEE2);

	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*     uchar EA_ucMIFHalt(DevHandle hDevHandle);
* ����������
*     ��������˯��ģʽ���Ӷ�ʹ���˳�ͨѶ����
* ���������
*    hDevHandle     �豸���
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFHalt(DevHandle hDevHandle)
{
	uchar ucResult;
	uchar ucTempData;
	uchar ucCMD = 0,ucRegAddr = 0;

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	// ��ʱ��ʱ������
	EI_mifs_vSetTimer(106);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucRegAddr = 0;
        ucTempData = 0;
        ucTempData = ucTempData;
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
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{
		// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
		ucRegAddr = EM_mifs_REG_COMMAND;
	}
#endif
	// ׼����������: ֹͣ������, 0
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_HALT;	// Halt command code
	EG_mifs_tWorkInfo.aucBuffer[1] = 0x00;	// dummy address

	// ִ������
    gtRfidProInfo.Cmd = EM_mifs_PICC_HALT;
	ucResult = EI_mifs_ucHandleCmd(ucCMD);
    gtRfidProInfo.Cmd = 0;

	// ��ʱ����ִ�гɹ�, ����ɹ�
	if ((ucResult == EM_mifs_NOTAGERR) || (ucResult == EM_TIMEOUT))
	{
		ucResult = EM_SUCCESS;
	}

	// ��λ����Ĵ���
	if(EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911)
    {
        ucTempData = EM_mifs_PCD_IDLE;
        EI_mifs_vWriteReg(1, ucRegAddr, &ucTempData);
    }

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*      s_rfid_UpdataCrc  s_rfid_CrcAB
* ����������
*       ����CRC_A��CRC_BУ����
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
* ���������
*     pucData: ���ݻ�������16�ֽڣ�
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
#define RFID_CRC_A   0
#define RFID_CRC_B   1
ushort s_rfid_UpdataCrc(uchar ch, ushort *lpwCrc)       //���ֽ�Ϊ��λ���м���   
{
    ch = (ch ^ (uchar)((*lpwCrc) & 0x00ff));
    ch = (ch ^ (ch << 4));
    *lpwCrc = (*lpwCrc >> 8) ^ ((ushort)ch << 8)
        ^ ((ushort)ch << 3) ^ ((ushort)ch >> 4);
    return(*lpwCrc);
}

ushort s_rfid_CrcAB(uchar Crctype, uchar *Data, int Sum, ushort* wCrc)
{
    uchar chBlock;
    //ushort wCrc = 0X6B7D;//0x6363;         //�Ĵ�����ʼ��   

    if ( Crctype == RFID_CRC_A ) {
        *wCrc = 0x6363;
    }else{
        *wCrc = 0xFFFF;
    }
    do{
        chBlock = *Data++;
        s_rfid_UpdataCrc(chBlock, wCrc);
    } while (--Sum);

    if ( Crctype == RFID_CRC_B ) {
        *wCrc = ~(*wCrc);
    }
    return *wCrc;
}
/**********************************************************************
* �������ƣ�
*      EA_ucMIFRead(DevHandle hDevHandle,uchar ucAddr, uchar *pucData)
* ����������
*     ��ȡһ��16�ֽڵ����ݿ�
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
* ���������
*     pucData: ���ݻ�������16�ֽڣ�
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFRead(DevHandle hDevHandle, uchar ucAddr, uchar * pucData)
{
    uchar ucTempData = 0;
	uchar ucResult;
	uchar ucCMD = 0;

	if (pucData == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	// ��ջ�����
	EI_mifs_vFlushFIFO();				// empty FIFO

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;

        u16 miflen = 0;
        u16 usCrc = 0;

        memset(EG_mifs_tWorkInfo.aucBuffer, 0, 18);
        EG_mifs_tWorkInfo.ulSendBytes = 2;
        EG_mifs_tWorkInfo.ucDisableDF = 0;
        EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
        EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_READ;	// read command code

        ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                   (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                   EG_mifs_tWorkInfo.aucBuffer,
                                   (16+2),
                                   &miflen,
                                   MIFARE_TRANSCEIVE_DEFTIME,
                                   FALSE);
        if ( (ucResult == EM_SUCCESS) && (miflen >= 16) ) {
            memcpy(pucData, EG_mifs_tWorkInfo.aucBuffer, 16);
            s_rfid_CrcAB(0, EG_mifs_tWorkInfo.aucBuffer, 16, &usCrc);
//            TRACE("\r\n --------crc %x--:%x %x",usCrc,EG_mifs_tWorkInfo.aucBuffer[17],EG_mifs_tWorkInfo.aucBuffer[16]);
            if ( (((uchar)(usCrc&0xFF)) == EG_mifs_tWorkInfo.aucBuffer[16]) && 
                 (((uchar)((usCrc>>8)&0xFF)) == EG_mifs_tWorkInfo.aucBuffer[17]) )
            {
                //crc_a succeed
                ucResult = EM_SUCCESS;
            }else{
                ucResult = EM_mifs_CODEERR;
            }
        }else{
            ucResult = EM_mifs_CODEERR;
        }
        EI_mifs_vEndProtect();
        return ucResult;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT);				// long timeout
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
		EI_mifs_vSetTimer(EM_mifs_DEFAULT);				// long timeout
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vSetBitMask(RxModeReg, ucTempData);
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
#ifdef EM_RC531_Module
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

		EI_mifs_vSetTimer(4);				// long timeout
		// ���ͺͽ���֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x0F;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
	}
#endif
	// ׼����������: ������, ��ַ
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_READ;	// read command code

	// ִ������
	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	// �жϽ��
	if ((ucResult != EM_SUCCESS) || (EG_mifs_tWorkInfo.lBitsReceived != 0x80))
	{
		// ���ղ��ɹ�������û������16���ֽ�
		if (ucResult != EM_mifs_NOTAGERR)
		{

			if ((ucResult == EM_mifs_CRCERR)	// ��֤�ɲ���ʱ���϶��ᷢ��CRC������Ϊ��������CRC�ֽ�
				&& ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x0a) == 0)	// ���յ���δ��֤��Ϣ
				&& (EG_mifs_tWorkInfo.lBitsReceived == 0))	// ���ڵȴ���ʱ�ж�ʱ�����Զ�����Ĵ�����RxLastBits������lBitsReceived = 0;
			{
				ucResult = EM_mifs_NOTAUTHERR;
			}
			// �����޿�����, ��Ҫ���յ�4λ����8λ����������
			else if ((EG_mifs_tWorkInfo.lBitsReceived != 4)
				&& (EG_mifs_tWorkInfo.lBitsReceived != 8))
			{
				ucResult = EM_mifs_BITCOUNTERR;
			}
			else
			{
				// ��������Ϊ������
				EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x0a) == 0)
				{
					ucResult = EM_mifs_NOTAUTHERR;
				}
				else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
				{
					ucResult = EM_SUCCESS;
				}
				else
				{
					ucResult = EM_mifs_CODEERR;
				}
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
		}								// if(ucResult != EM_mifs_NOTAGERR)
	}									// if(reuslt != EM_SUCCESS || EG_mifs_tWorkInfo.lBitsReceived != 0x80)
	else
	{
		// ��������
		memcpy(pucData, EG_mifs_tWorkInfo.aucBuffer, 16);
	}

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFWrite(DevHandle hDevHandle,uchar ucAddr, uchar *pucData)
* ����������
*     ��ȡһ��16�ֽڵ����ݿ�
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
*     pucData: ���ݻ�������16�ֽڣ�
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFWrite(DevHandle hDevHandle, uchar ucAddr, uchar * pucData)
{
    uchar ucTempData = 0;
	uchar ucResult;
	uchar ucCMD = 0;
	uint time = 0;

	if (pucData == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	// ��ջ�����
	EI_mifs_vFlushFIFO();				// empty FIFO

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;

        u16 miflen = 0;

        EG_mifs_tWorkInfo.ulSendBytes = 2;
        EG_mifs_tWorkInfo.ucDisableDF = 0;
        EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
        EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_WRITE;	// read command code

        ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                   (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                   EG_mifs_tWorkInfo.aucBuffer,
                                   1,
                                   &miflen,
                                   MIFARE_TRANSCEIVE_DEFTIME,
                                   TRUE);
//        TRACE("\r\n11--ret:%d  len:%d",ucResult,miflen);
//        DISPBUF(EG_mifs_tWorkInfo.aucBuffer, miflen, 0);
        if ( (ucResult == EM_SUCCESS) && (miflen >= 1)) {
            EG_mifs_tWorkInfo.ulSendBytes = 16;
            memcpy(EG_mifs_tWorkInfo.aucBuffer, pucData, 16);
            EG_mifs_tWorkInfo.ucDisableDF = 0;
            ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                         (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                         EG_mifs_tWorkInfo.aucBuffer,
                                         1,
                                         &miflen,
                                         MIFARE_TRANSCEIVE_DEFTIME,
                                         TRUE);
            if ( (ucResult == EM_SUCCESS) && (miflen >= 1)) {
                ucResult = EM_SUCCESS;
            }else
                ucResult = EM_mifs_CODEERR;
        }else{
            ucResult = EM_mifs_CODEERR;
        }
        EI_mifs_vEndProtect();
        return ucResult;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT);				// long timeout
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*5;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT);
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vClrBitMask(RxModeReg, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PN512CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*5;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// ���ö�ʱ��ʱ��
		EI_mifs_vSetTimer(4);

		// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
		time = 6;

#endif

	}

	// ׼����������: ������+��ַ
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_WRITE;
	EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
	EG_mifs_tWorkInfo.ucDisableDF = 0;

	// ִ������
	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	if (ucResult != EM_mifs_NOTAGERR)
	{
		// �����޿�����, ��Ҫ���յ�4λ����8λ����������
		if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
		{
			ucResult = EM_mifs_BITCOUNTERR;
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x0a) == 0)
			{
				ucResult = EM_mifs_NOTAUTHERR;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
			{
				ucResult = EM_SUCCESS;
			}
			else
			{
				ucResult = EM_mifs_CODEERR;
			}
		}								// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
	}

	// �ɹ�������Ҫд�������
	if (ucResult == EM_SUCCESS)
	{
		// long timeout
		EI_mifs_vSetTimer(time);

		// ׼����������: 16���ֽڵ�����
		EG_mifs_tWorkInfo.ulSendBytes = 16;
		memcpy(EG_mifs_tWorkInfo.aucBuffer, pucData, 16);
		EG_mifs_tWorkInfo.ucDisableDF = 0;

		// ִ������
		ucResult = EI_mifs_ucHandleCmd(ucCMD);
		if (ucResult != EM_mifs_NOTAGERR)
		{
			// �����޿�����, ��Ҫ���յ�4λ����8λ����������
			if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
			{
				ucResult = EM_mifs_BITCOUNTERR;
			}
			else
			{
				EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x0a) == 0)
				{
					ucResult = EM_mifs_NOTAUTHERR;
				}
				else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
				{
					ucResult = EM_SUCCESS;
				}
				else
				{
					ucResult = EM_mifs_CODEERR;
				}
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
		}								// if(ucResult != EM_mifs_NOTAGERR)
	}									// if(ucResult == EM_SUCCESS)

	EI_mifs_vEndProtect();
	return (ucResult);
}

/**********************************************************************
* �������ƣ�
*      EA_ucMIFIncrement(DevHandle hDevHandle,uchar ucAddr, ulong ulValue)
* ����������
*     ��ָ����ֵ����һ��ֵ����
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
*     ulValue: ָ������ֵ
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFIncrement(DevHandle hDevHandle, uchar ucAddr, ulong ulValue)
{
	uchar ucResult;
    uchar ucTempData = 0;
	uchar i;
	uchar ucCMD = 0;
	uint time = 0;

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 1;

	// ��д�������
	EG_mifs_tWorkInfo.aucCmdBuf[0] = EM_mifs_PICC_INCREMENT;
	EG_mifs_tWorkInfo.aucCmdBuf[1] = ucAddr;
	memcpy(EG_mifs_tWorkInfo.aucCmdBuf + 2, (void *)&ulValue, 4);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;

        u16 miflen = 0;

        EG_mifs_tWorkInfo.ulSendBytes = 2;
        EG_mifs_tWorkInfo.ucDisableDF = 0;
        EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucCmdBuf[0];
        EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[1];
        ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                     (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                     EG_mifs_tWorkInfo.aucBuffer,
                                     2,
                                     &miflen,
                                     MIFARE_TRANSCEIVE_DEFTIME,
                                     TRUE);
        if ( ucResult == EM_SUCCESS ) {
            EG_mifs_tWorkInfo.ulSendBytes = 4;
            memcpy(EG_mifs_tWorkInfo.aucBuffer, &EG_mifs_tWorkInfo.aucCmdBuf[2], 4);
            EG_mifs_tWorkInfo.ucDisableDF = 0;
            ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                         (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                         EG_mifs_tWorkInfo.aucBuffer,
                                         2,
                                         &miflen,
                                         MIFARE_TRANSCEIVE_DEFTIME,
                                         TRUE);
            ucResult = EM_SUCCESS;
        }else{
            ucResult = EM_mifs_CODEERR;
        }
        EI_mifs_vEndProtect();
        return ucResult;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*4;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vClrBitMask(RxModeReg, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PN512CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*4;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// short timeout
		EI_mifs_vSetTimer(6);

		// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
		time = 6;

#endif

	}

	// ��һ���ȷ���: ������+Դ���
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucCmdBuf[0];
	EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[1];

	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	if (ucResult != EM_mifs_NOTAGERR)
	{
		// �����޿�����, �������4λ����8λ
		if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
		{
			ucResult = EM_mifs_BITCOUNTERR;
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
			if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
			{
				ucResult = EM_mifs_NOTAUTHERR;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
			{
				ucResult = EM_SUCCESS;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
			{
				ucResult = EM_mifs_VALERR;
			}
			else
			{
				ucResult = EM_mifs_CODEERR;
			}
		}								// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
	}									// if(ucResult != EM_mifs_NOTAGERR)

	if (ucResult == EM_SUCCESS)
	{
		// �ڶ�������ִ��

		// short timeout
		EI_mifs_vSetTimer(time);

		// ׼����������: �޸�ֵ��4�ֽڣ�
		EG_mifs_tWorkInfo.ulSendBytes = 4;
		for (i = 0; i < 4; i++)
		{
			EG_mifs_tWorkInfo.aucBuffer[i] = EG_mifs_tWorkInfo.aucCmdBuf[i + 2];
		}
		EG_mifs_tWorkInfo.ucDisableDF = 0;

		// ִ������
		ucResult = EI_mifs_ucHandleCmd(ucCMD);

		if (ucResult == EM_SUCCESS)
		{
			// �޳�ʱ����, �������4λ����8λ
			if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
			{
				ucResult = EM_mifs_BITCOUNTERR;
			}
			else
			{
				EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
				if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
				{
					ucResult = EM_mifs_NOTAUTHERR;
				}
				else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
				{
					ucResult = EM_mifs_VALERR;
				}
				else
				{
					ucResult = EM_mifs_CODEERR;
				}
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
		}								// if(ucResult == EM_SUCCESS)
		else if (ucResult == EM_mifs_NOTAGERR)
		{
			// û��Ӧ��ɹ�
			ucResult = EM_SUCCESS;
		}
	}									// if(ucResult == EM_SUCCESS)

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*      EA_ucMIFDecrement(DevHandle hDevHandle,uchar ucAddr, ulong ulValue)
* ����������
*     ��һ��ֵ���м�ȥָ����ֵ
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
*     ulValue: ָ������ֵ
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFDecrement(DevHandle hDevHandle, uchar ucAddr, ulong ulValue)
{
	uchar ucResult;
    uchar ucTempData = 0;
	uchar i = 0;
	uchar ucCMD = 0;
	uint time = 0;

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 1;

	// ��д�������
	EG_mifs_tWorkInfo.aucCmdBuf[0] = EM_mifs_PICC_DECREMENT;
	EG_mifs_tWorkInfo.aucCmdBuf[1] = ucAddr;
	memcpy(EG_mifs_tWorkInfo.aucCmdBuf + 2, (void *)&ulValue, 4);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;

        u16 miflen = 0;

        EG_mifs_tWorkInfo.ulSendBytes = 2;
        EG_mifs_tWorkInfo.ucDisableDF = 0;
        EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucCmdBuf[0];
        EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[1];
        ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                     (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                     EG_mifs_tWorkInfo.aucBuffer,
                                     2,
                                     &miflen,
                                     MIFARE_TRANSCEIVE_DEFTIME,
                                     TRUE);
        if ( ucResult == EM_SUCCESS ) {
            EG_mifs_tWorkInfo.ulSendBytes = 4;
            memcpy(EG_mifs_tWorkInfo.aucBuffer, &EG_mifs_tWorkInfo.aucCmdBuf[2], 4);
            EG_mifs_tWorkInfo.ucDisableDF = 0;
            ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                         (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                         EG_mifs_tWorkInfo.aucBuffer,
                                         2,
                                         &miflen,
                                         MIFARE_TRANSCEIVE_DEFTIME,
                                         TRUE);
            ucResult = EM_SUCCESS;
        }else{
            ucResult = EM_mifs_CODEERR;
        }
        EI_mifs_vEndProtect();
        return ucResult;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*4;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vClrBitMask(RxModeReg, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PN512CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*4;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// short timeout
		EI_mifs_vSetTimer(6);

		// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
		time = 6;

#endif

	}

	// ��һ���ȷ���: ������+Դ���
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucCmdBuf[0];
	EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[1];

	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	if (ucResult != EM_mifs_NOTAGERR)
	{
		// �����޿�����, �������4λ����8λ
		if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
		{
			ucResult = EM_mifs_BITCOUNTERR;
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
			if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
			{
				ucResult = EM_mifs_NOTAUTHERR;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
			{
				ucResult = EM_SUCCESS;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
			{
				ucResult = EM_mifs_VALERR;
			}
			else
			{
				ucResult = EM_mifs_CODEERR;
			}
		}								// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
	}									// if(ucResult != EM_mifs_NOTAGERR)

	if (ucResult == EM_SUCCESS)
	{
		// �ڶ�������ִ��

		// short timeout
		EI_mifs_vSetTimer(time);

		// ׼����������: �޸�ֵ��4�ֽڣ�
		EG_mifs_tWorkInfo.ulSendBytes = 4;
		for (i = 0; i < 4; i++)
		{
			EG_mifs_tWorkInfo.aucBuffer[i] = EG_mifs_tWorkInfo.aucCmdBuf[i + 2];
		}
		EG_mifs_tWorkInfo.ucDisableDF = 0;

		// ִ������
		ucResult = EI_mifs_ucHandleCmd(ucCMD);

		if (ucResult == EM_SUCCESS)
		{
			// �޳�ʱ����, �������4λ����8λ
			if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
			{
				ucResult = EM_mifs_BITCOUNTERR;
			}
			else
			{
				EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
				if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
				{
					ucResult = EM_mifs_NOTAUTHERR;
				}
				else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
				{
					ucResult = EM_mifs_VALERR;
				}
				else
				{
					ucResult = EM_mifs_CODEERR;
				}
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
		}								// if(ucResult == EM_SUCCESS)
		else if (ucResult == EM_mifs_NOTAGERR)
		{
			// û��Ӧ��ɹ�
			ucResult = EM_SUCCESS;
		}
	}									// if(ucResult == EM_SUCCESS)

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFRestore(DevHandle hDevHandle,uchar ucAddr)
* ����������
*     ����ָ����ֵ������ݵ��ڴ���
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFRestore(DevHandle hDevHandle, uchar ucAddr)
{
	uchar ucResult;
    uchar ucTempData = 0;
	uchar i = 0;
	uchar ucCMD = 0;
	uint time = 0;

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EG_mifs_tWorkInfo.ucEnableTransfer = 1;

	// ��д�������
	EG_mifs_tWorkInfo.aucCmdBuf[0] = EM_mifs_PICC_RESTORE;
	EG_mifs_tWorkInfo.aucCmdBuf[1] = ucAddr;
	memset(EG_mifs_tWorkInfo.aucCmdBuf + 2, 0, 4);

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;

        u16 miflen = 0;

        EG_mifs_tWorkInfo.ulSendBytes = 2;
        EG_mifs_tWorkInfo.ucDisableDF = 0;
        EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucCmdBuf[0];
        EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[1];
        ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                     (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                     EG_mifs_tWorkInfo.aucBuffer,
                                     2,
                                     &miflen,
                                     MIFARE_TRANSCEIVE_DEFTIME,
                                     TRUE);
        if ( ucResult == EM_SUCCESS ) {
            EG_mifs_tWorkInfo.ulSendBytes = 4;
            memcpy(EG_mifs_tWorkInfo.aucBuffer, &EG_mifs_tWorkInfo.aucCmdBuf[2], 4);
            EG_mifs_tWorkInfo.ucDisableDF = 0;
            ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                         (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                         EG_mifs_tWorkInfo.aucBuffer,
                                         2,
                                         &miflen,
                                         MIFARE_TRANSCEIVE_DEFTIME,
                                         TRUE);
            ucResult = EM_SUCCESS;
        }else{
            ucResult = EM_mifs_CODEERR;
        }
        EI_mifs_vEndProtect();
        return ucResult;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*4;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vClrBitMask(RxModeReg, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PN512CMD_TRANSCEIVE;
		time = EM_mifs_DEFAULT*4;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// short timeout
		EI_mifs_vSetTimer(6);

		// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;
		time = 6;

#endif

	}

	// ��һ���ȷ���: ������+Դ���
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.aucCmdBuf[0];
	EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[1];

	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	if (ucResult != EM_mifs_NOTAGERR)
	{
		// �����޿�����, �������4λ����8λ
		if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
		{
			ucResult = EM_mifs_BITCOUNTERR;
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
			if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
			{
				ucResult = EM_mifs_NOTAUTHERR;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
			{
				ucResult = EM_SUCCESS;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
			{
				ucResult = EM_mifs_VALERR;
			}
			else
			{
				ucResult = EM_mifs_CODEERR;
			}
		}								// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
	}									// if(ucResult != EM_mifs_NOTAGERR)

	if (ucResult == EM_SUCCESS)
	{
		// �ڶ�������ִ��

		// short timeout
		EI_mifs_vSetTimer(time);

		// ׼����������: �޸�ֵ��4�ֽڣ�
		EG_mifs_tWorkInfo.ulSendBytes = 4;
		for (i = 0; i < 4; i++)
		{
			EG_mifs_tWorkInfo.aucBuffer[i] = EG_mifs_tWorkInfo.aucCmdBuf[i + 2];
		}
		EG_mifs_tWorkInfo.ucDisableDF = 0;

		// ִ������
		ucResult = EI_mifs_ucHandleCmd(ucCMD);

		if (ucResult == EM_SUCCESS)
		{
			// �޳�ʱ����, �������4λ����8λ
			if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
			{
				ucResult = EM_mifs_BITCOUNTERR;
			}
			else
			{
				EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
				if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
				{
					ucResult = EM_mifs_NOTAUTHERR;
				}
				else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
				{
					ucResult = EM_mifs_VALERR;
				}
				else
				{
					ucResult = EM_mifs_CODEERR;
				}
			}							// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
		}								// if(ucResult == EM_SUCCESS)
		else if (ucResult == EM_mifs_NOTAGERR)
		{
			// û��Ӧ��ɹ�
			ucResult = EM_SUCCESS;
		}
	}									// if(ucResult == EM_SUCCESS)

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFTransfer(DevHandle hDevHandle,uchar ucAddr)
* ����������
*     ������RAM�Ĵ��������ݿ�����ָ����ֵ��
* ���������
*    hDevHandle     �豸���
*    ucAddr:		����S50�ṹMifare One��Ƭ�����ţ�0-63��
*                   ����S70�ṹMifare One��Ƭ������(0 - 255)
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFTransfer(DevHandle hDevHandle, uchar ucAddr)
{
    uchar ucTempData = 0;
	uchar ucResult;
	uchar ucCMD = 0;

	if (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
	{
		if (ucAddr > 63)
		{
			return EM_ERRPARAM;
		}
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	if (EG_mifs_tWorkInfo.ucEnableTransfer == 0)
	{
		// ������ִ��Transfer����
		ucResult = EM_mifs_CODEERR;
		goto mifsTransferEnd;
	}

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;

        u16 miflen = 0;

        EG_mifs_tWorkInfo.ulSendBytes = 2;
        EG_mifs_tWorkInfo.ucDisableDF = 0;
        EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_TRANSFER;
        EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
        ucResult = mifareSendRequest(EG_mifs_tWorkInfo.aucBuffer,
                                     (u16)EG_mifs_tWorkInfo.ulSendBytes,
                                     EG_mifs_tWorkInfo.aucBuffer,
                                     2,
                                     &miflen,
                                     MIFARE_TRANSCEIVE_DEFTIME,
                                     TRUE);
        EG_mifs_tWorkInfo.ucEnableTransfer = 0;
        if ( ucResult != EM_SUCCESS ) {
            ucResult = EM_mifs_CODEERR;
        }
        EI_mifs_vEndProtect();
        return ucResult;
#endif

    }
    else if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {

#ifdef EM_RC663_Module
        ucTempData = 0x01;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FRAMECON, 0xC0);
		EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData);
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PHHAL_HW_RC663_CMD_TRANSCEIVE;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		ucTempData = 0x80;
		EI_mifs_vSetTimer(EM_mifs_DEFAULT*4);
		EI_mifs_vClrBitMask(ManualRCVReg, 0x10);  //clr ��żУ��
		EI_mifs_vSetBitMask(TxModeReg, ucTempData);
		EI_mifs_vClrBitMask(RxModeReg, ucTempData);    //���ղ���ҪCRCУ��
		ucCMD = PN512CMD_TRANSCEIVE;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		// short timeout
		EI_mifs_vSetTimer(6);

		// ����֡���һ���ֽ�ΪCRCУ�飬ÿ���ֽڴ���У��
		ucTempData = 0x07;
		EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
		ucCMD = EM_mifs_PCD_TRANSCEIVE;

#endif

	}

	// ׼����������: ������+Դ���+4�ֽ�����+Ŀ����
	EG_mifs_tWorkInfo.aucCmdBuf[6] = ucAddr;

	// ׼����������: ������+Ŀ����
	EG_mifs_tWorkInfo.ulSendBytes = 2;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_TRANSFER;
	EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.aucCmdBuf[6];
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	ucResult = EI_mifs_ucHandleCmd(ucCMD);

	if (ucResult != EM_mifs_NOTAGERR)
	{
		// �޳�ʱ����, �������4λ����8λ
		if ((EG_mifs_tWorkInfo.lBitsReceived != 4) && (EG_mifs_tWorkInfo.lBitsReceived != 8))
		{
			ucResult = EM_mifs_BITCOUNTERR;
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] &= 0x0f;	// mask out upper nibble
			if (EG_mifs_tWorkInfo.aucBuffer[0] == 0)
			{
				ucResult = EM_mifs_NOTAUTHERR;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x0a)
			{
				ucResult = EM_SUCCESS;
			}
			else if (EG_mifs_tWorkInfo.aucBuffer[0] == 0x01)
			{
				ucResult = EM_mifs_VALERR;
			}
			else
			{
				ucResult = EM_mifs_CODEERR;
			}
		}								// if(EG_mifs_tWorkInfo.lBitsReceived != 4 && EG_mifs_tWorkInfo.lBitsReceived != 8)
	}									// if(ucResult != EM_mifs_NOTAGERR)

  mifsTransferEnd:
	EG_mifs_tWorkInfo.ucEnableTransfer = 0;
	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************

                          MifarePro��(TYPE A)��������

***********************************************************************/
/**********************************************************************
* �������ƣ�
*     EA_ucMIFResetPro(DevHandle hDevHandle, uchar ucMode, uint *puiRespLen, void *pvResp)
* ����������
*     ���mifpro���ļ������
* ���������
*    hDevHandle     �豸���
*    ucMode: ����ģʽ
* ���������
*     puiRespLen      :  ʵ�ʷ������ݳ���
*     pvResp     : ���ص�����,������������
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFResetPro(DevHandle hDevHandle, uchar ucMode, uint * puiRespLen, void *pvResp)
{

#ifdef EM_RC531_Module

	ushort tagType;
	uint cardSize;
	uchar ucResult;
	ulong cardSerial;
	uchar *pucResp = (uchar *) pvResp;

	if ((puiRespLen == EM_NULL) || (pvResp == EM_NULL))
	{
		return EM_ERRPARAM;
	}
	if ((ucMode != EM_mifs_HALT) && (ucMode != EM_mifs_IDLE))
	{
		return EM_ERRPARAM;
	}

	*puiRespLen = 0;

	EI_paypass_vDelay(500);
	// ����Ƭ����ʶ���
	ucResult = EA_ucMIFRequest(hDevHandle, ucMode, &tagType);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	pucResp[0] = (uchar) tagType;
	pucResp[1] = (uchar) (tagType >> 8);

	// ����ͻ, ȡ�ÿ���
	EI_paypass_vDelay(60);
	ucResult = EA_ucMIFAntiColl(hDevHandle, &cardSerial);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}
	EI_paypass_vDelay(60);
	pucResp[2] = (uchar) cardSerial;
	pucResp[3] = (uchar) (cardSerial >> 8);
	pucResp[4] = (uchar) (cardSerial >> 16);
	pucResp[5] = (uchar) (cardSerial >> 24);
	// ?????
	//pucResp[6] = pucResp[4]^pucResp[5]^pucResp[6]^pucResp[7];
	pucResp[6] = pucResp[2] ^ pucResp[3] ^ pucResp[4] ^ pucResp[5];
	// ѡ��, ����MifarePro����ʶ�����߿���С
	ucResult = EA_ucMIFSelect(hDevHandle, cardSerial, &cardSize);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}
	pucResp[7] = (uchar) cardSize;

	// Added by Pan Pingbin 2005.09.27
	// ȷ��Pro�����㹻��ʱ����ӦRATS����
	EI_paypass_vDelay(100);

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	if ((pucResp[7] & 0x20) != 0)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] = 0xE0;
		//yehf 2010-09-03 Level-1Ҫ���޸�
		EG_mifs_tWorkInfo.aucBuffer[1] = 0x80;	//  0x70;    // �ɽ������֡Ϊ128�ֽ� CID=0
		// ��MifarePro���ϵ磬��ȡATRֵ
		ucResult = EI_mifs_ucProcess(EM_mifs_TYPEA, 2, puiRespLen);
		if (ucResult != EM_SUCCESS)
		{
			goto resetProEnd;
		}
		// ���ӳ���Խ���жϣ�2006.10.09
		if (*puiRespLen > 255)
		{
			ucResult = EM_mifs_CODEERR;
			*puiRespLen = 8;
			goto resetProEnd;
		}

		// �����ϵ縴λ����
		ucResult = EI_mifs_ucHandleATQA(*puiRespLen);
		if (ucResult != EM_SUCCESS)
		{
			// �ϵ縴λ���ݴ���
			*puiRespLen = 8;
			goto resetProEnd;
		}
		// �����������
		memcpy(pucResp + 8, EG_mifs_tWorkInfo.aucBuffer, *puiRespLen);
		*puiRespLen += 8;
	}
	else
	{
		// ����MifarePro��
		ucResult = EM_mifs_CODEERR;
		*puiRespLen = 8;
	}

  resetProEnd:
	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFExchangePro(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint*puiRecLen, void *pvRecData)
* ����������
*     ��ISO14443-4��׼��ɶ�д����mifpro�������ݽ���������
* ���������
*    hDevHandle     �豸���
*    uiSendLen: Ҫ�������ݳ���
*    pvSendData:   Ҫ��������
* ���������
*     puiRecLen      :  ʵ�ʷ������ݳ���
*     pvRecData   : ���ص�����,������������
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFExchangePro(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint * puiRecLen,
	void *pvRecData)
{
#ifdef EM_RC531_Module

	return EI_mifs_ucExchange(hDevHandle, EM_mifs_TYPEA, 0, uiSendLen, pvSendData, puiRecLen,
		pvRecData);
#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif
}

/**********************************************************************
* �������ƣ�
*    EA_ucMIFCommandPro(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint*puiRecLen, void *pvRecData)
* ����������
*     ��ɶ�д����mifpro�������ݽ���������
* ���������
*    hDevHandle     �豸���
*    uiSendLen: Ҫ�������ݳ���
*    pvSendData:   Ҫ��������
* ���������
*     puiRecLen      :  ʵ�ʷ������ݳ���
*     pvRecData   : ���ص�����,������������
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFCommandPro(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint * puiRecLen,
	void *pvRecData)
{
#ifdef EM_RC531_Module

	return EI_mifs_ucCommand(hDevHandle, EM_mifs_TYPEA, 0, uiSendLen, pvSendData, puiRecLen,
		pvRecData);
#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif
}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFDeselectPro(DevHandle hDevHandle)
* ����������
*     ��mifpro������Ϊhalt״̬
* ���������
*    hDevHandle     �豸���
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFDeselectPro(DevHandle hDevHandle)
{

#ifdef EM_RC531_Module

	uchar ucResult;

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	// ISO14443-4�淶Ҫ��: �����һ��Deselect�������ʱ����,��Ӧ�ط�һ��Deselect

	ucResult = EI_mifs_ucDeselect(EM_mifs_TYPEA, 0);
	if (ucResult != EM_SUCCESS)
	{
		ucResult = EI_mifs_ucDeselect(EM_mifs_TYPEA, 0);;
	}

	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************

                          TYPE-B����������

***********************************************************************/
/**********************************************************************
* �������ƣ�
*     EA_ucMIFRequestB(DevHandle hDevHandle, uchar ucState, uchar ucType,
*                             uchar ucCount, uint *puiRespLen, void *pvResp)
* ����������
*     ���������߷�Χ�ڵ�TYPE B��
* ���������
*    hDevHandle     �豸���
*     ucState: ѡ�񼤻Ƭʱ�Ŀ�Ƭ״̬,EM_mifs_IDLE-IDLE״̬,EM_mifs_HALT-HALT״̬
*     ucType: Ӧ�����ͱ�ʶ,���ΪEM_mifs_TYPEB_REQALL�����������ڵĿ�Ƭ��Ӧ��Ӧ,����ֵ����������ݹ淶
*     ucCount: ʱ϶��������,ȡֵ��Χ0��4,���ж��ſ�ʱ�ɲ��ô˷�������ͻ,ÿ�ſ��ڵ�һ��ʱ϶��Ӧ�ĸ���Ϊ1/2^n
*             0-1,1-2,2-4,3-8,4-16
* ���������
*     puiRespLen: ���յ����ݳ���
*     pvResp: ���յ�������, ��ʽ����:
*       �ֽ�1��0x50,��ֵ�̶�
*       �ֽ�2��5��αΨһ��PICC��ʾ����������PICC���кŵ�������ʽ��оƬ���кţ�PICC�����ĵ��������
*                 �ñ�ʾ����HLTB��ATRRIB�����л�Ҫ�õ�
*       �ֽ�6��9��Ӧ�ÿ����ֽڣ�������£�
*                 �ֽ�6�� ��PICCΪ��Ӧ��ʱ�ɲ������Ӧ�ñ��ȷ����Ƭ��Ӧ�÷�Χ��
*                         ��Ϊһ������ʱ��Ҫͨ��CRC_B���֧�ֵ�Ӧ�÷�Χ
*                 �ֽ�7~8�� ��������һ������ʱ��PICC֧��Ӧ�÷�Χ��7816-5��
*                 �ֽ�9�� ֧�ֵ�Ӧ������0-15
*       �ֽ�10��12��Э������ֽڣ�������£�
*                 �ֽ�10�� ����PCD��PICC֮���ETU�����ݴ������ʣ���ֵ��Ӧ������Ҫ�ı�
*                 �ֽ�11�� �Ͱ��ֽ�ȷ��PICC�Ƿ�֧��ISO14443-4,1-֧��,0-��֧�֣�
*                          �߰��ֽ�ȷ��PICC�ɽ��յ����֡����
*                 �ֽ�12�� �߰��ֽ�ȷ������֡ʱ�����ȴ�ʱ�䣨����������������
*                          �Ͱ��ֽڵĸ������ֽ�ȷ��Ӧ��Э�������Ȩ
*                                  00-Ӧ��Э����˽�еģ�01-Ӧ��Э�鰴��Ӧ�ÿ����ֽڶ��壻
*                          �Ͱ��ֽڵĵ������ֽ�ȷ��������Ϣ��1X-��Ƭ֧��NAD��X1-��Ƭ֧��CID
*     ��Ƭ�ı�ʾ����ҪӦ�ó��򱣴�,Ӧ�ó�����Ҫ����ʵ��֧�ֵĿ�Ƭ�Կ�Ƭ�ķ��ؽ����ж���ѡ����Ӧ�Ŀ�Ƭ
*     ���⿨Ƭ�ɽ��յ�������ݳ������û����棬��Ƭ�Ƿ�֧��ISP14443-4Э��ҲҪ���档��Ϊʵ��Ӧ���п���ͬʱ�Զ��ſ����в���
*     ����ʱ�̶�Ϊ����NAD
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
***********************************************************************/
uchar EA_ucMIFRequestB(DevHandle hDevHandle, uchar ucState, uchar ucType,
	uchar ucCount, uint * puiRespLen, void *pvResp)
{

#ifdef EM_RC531_Module

	uchar ucResult = EM_SUCCESS;
	uchar ucTempData;

	if ((ucState != EM_mifs_IDLE) && (ucState != EM_mifs_HALT))
	{
		return EM_ERRPARAM;
	}

	if ((ucCount > EM_mifs_TIMESLOT_MAXINT) || (puiRespLen == EM_NULL) || (pvResp == EM_NULL))
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EI_mifs_vFlushFIFO();				//empty FIFO

	// ���ͺͽ���֡���2���ֽ�ΪCRC16У�飬ÿ���ֽڲ���У��λ
	ucTempData = 0x2C;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

	EI_mifs_vClrBitMask(EM_mifs_REG_CONTROL, 0x08);	// ��ֹcrypto 1���ܵ�Ԫ
	EI_mifs_vSetBitMask(EM_mifs_REG_TXCONTROL, 0x03);	// ��TX1��TX2����

	EI_mifs_vSetTimer(7);

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = 3;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_REQB;	//��������
	EG_mifs_tWorkInfo.aucBuffer[1] = ucType;
	EG_mifs_tWorkInfo.aucBuffer[2] = ucCount;
	// ȷ����ǰ���õļ��ʽ����λ����EG_mifs_tWorkInfo.aucBuffer[2]�ĵ�3λ
	if (ucState == EM_mifs_HALT)
	{
		EG_mifs_tWorkInfo.aucBuffer[2] |= 0x08;
	}

	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);
	// �����һ��RequestB����Ӧ,���������ӳ�5ms��,����ڶ���RequestB����
	// Added by panpingbin 2005.10.21
	if (ucResult == EM_mifs_NOTAGERR)
	{
		ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);
	}

	if ((ucResult != EM_SUCCESS) && (ucResult != EM_mifs_NOTAGERR) &&
		((EG_mifs_tWorkInfo.aucBuffer[0] == 0x50) || (EG_mifs_tWorkInfo.aucBuffer[0] == 0x05)))
	{
		// ��Ƭ��ͻ
		ucResult = EM_mifs_COLLERR;
	}

	if (ucResult == EM_SUCCESS)
	{
		// ���ӳ���Խ���ж�
		if (EG_mifs_tWorkInfo.ulBytesReceived != 12)
		{
			ucResult = EM_mifs_CODEERR;
			*puiRespLen = 0;
			goto ReqBEnd;
		}
		ucResult = EI_mifs_ucHandleATQB(EG_mifs_tWorkInfo.ulBytesReceived);
		if (ucResult == EM_SUCCESS)
		{
			*puiRespLen = EG_mifs_tWorkInfo.ulBytesReceived;
			memcpy(pvResp, EG_mifs_tWorkInfo.aucBuffer, *puiRespLen);
		}
		else
		{
			*puiRespLen = 0;
		}
	}
	else
	{
		*puiRespLen = 0;
	}

  ReqBEnd:
	// ��ʱ500us
	EI_paypass_vDelay(60);
	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*    uchar EA_ucMIFSlotMarkerB(DevHandle hDevHandle, uchar ucNumber, uint *puiRespLen,
*                                void *pvResp);
* ����������
*     ����ʱ϶���,����Ƭ���ڵ�һ��ʱ϶��Ӧʱ,�����͸�����ʹ��Ӧʱ϶�Ŀ�ƬӦ��
* �����º�������
*     API
* �������º�����
*     �²㺯��
* ���������
*    hDevHandle     �豸���
*     ucNumber: ʱ϶���,1-15,��Ӧʱ϶2��16
* ���������
*     puiRespLen: ���յ����ݳ���
*     pvResp: ���յ�������, ��ʽ����:
*       �ֽ�1��0x50,��ֵ�̶�
*       �ֽ�2��5��αΨһ��PICC��ʾ����������PICC���кŵ�������ʽ��оƬ���кţ�PICC�����ĵ��������
*                 �ñ�ʾ����HLTB��ATRRIB�����л�Ҫ�õ�
*       �ֽ�6��9��Ӧ�ÿ����ֽڣ�������£�
*                 �ֽ�6�� ��PICCΪ��Ӧ��ʱ�ɲ������Ӧ�ñ��ȷ����Ƭ��Ӧ�÷�Χ��
*                         ��Ϊһ������ʱ��Ҫͨ��CRC_B���֧�ֵ�Ӧ�÷�Χ
*                 �ֽ�7~8�� ��������һ������ʱ��PICC֧��Ӧ�÷�Χ��7816-5��
*                 �ֽ�9�� ֧�ֵ�Ӧ������0-15
*       �ֽ�10��12��Э������ֽڣ�������£�
*                 �ֽ�10�� ����PCD��PICC֮���ETU�����ݴ������ʣ���ֵ��Ӧ������Ҫ�ı�
*                 �ֽ�11�� �Ͱ��ֽ�ȷ��PICC�Ƿ�֧��ISO14443-4,1-֧��,0-��֧�֣�
*                          �߰��ֽ�ȷ��PICC�ɽ��յ����֡����
*                 �ֽ�12�� �߰��ֽ�ȷ������֡ʱ�����ȴ�ʱ�䣨����������������
*                          �Ͱ��ֽڵĸ������ֽ�ȷ��Ӧ��Э�������Ȩ
*                                  00-Ӧ��Э����˽�еģ�01-Ӧ��Э�鰴��Ӧ�ÿ����ֽڶ��壻
*                          �Ͱ��ֽڵĵ������ֽ�ȷ��������Ϣ��1X-��Ƭ֧��NAD��X1-��Ƭ֧��CID
*     ��Ƭ�ı�ʾ����ҪӦ�ó��򱣴�,Ӧ�ó�����Ҫ����ʵ��֧�ֵĿ�Ƭ�Կ�Ƭ�ķ��ؽ����ж���ѡ����Ӧ�Ŀ�Ƭ
*     ���⿨Ƭ�ɽ��յ�������ݳ������û����棬��Ƭ�Ƿ�֧��ISP14443-4Э��ҲҪ���档��Ϊʵ��Ӧ���п���ͬʱ�Զ��ſ����в���
*     ����ʱ�̶�Ϊ����NAD
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
*                 2006-10-09          01-05-02    ���ӳ���Խ���ж�
***********************************************************************/
uchar EA_ucMIFSlotMarkerB(DevHandle hDevHandle, uchar ucNumber, uint * puiRespLen, void *pvResp)
{

#ifdef EM_RC531_Module

	uchar ucResult = EM_SUCCESS;
	uchar ucTempData;

	if ((puiRespLen == NULL) || (pvResp == NULL))
	{
		return EM_ERRPARAM;
	}

	if ((ucNumber < 1) || (ucNumber >= EM_mifs_TIMESLOT_MAXNUM))
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EI_mifs_vFlushFIFO();				//empty FIFO

	// RxCRC and TxCRC enable, parity
	ucTempData = 0x2C;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

	EI_mifs_vSetTimer(5);

	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = 1;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = (ucNumber << 4) + EM_mifs_PICC_SLOTMARKER;	//��������

	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);

	if ((ucResult != EM_SUCCESS) && (ucResult != EM_mifs_NOTAGERR) &&
		((EG_mifs_tWorkInfo.aucBuffer[0] == 0x50) || (EG_mifs_tWorkInfo.aucBuffer[0] == 0x05)))
	{
		// ��Ƭ��ͻ
		ucResult = EM_mifs_COLLERR;
	}

	if (ucResult == EM_SUCCESS)
	{
		// ���ӳ���Խ���жϣ�
		if (EG_mifs_tWorkInfo.ulBytesReceived != 12)
		{
			ucResult = EM_mifs_CODEERR;
			*puiRespLen = 0;
			goto SlotBEnd;
		}
		ucResult = EI_mifs_ucHandleATQB(EG_mifs_tWorkInfo.ulBytesReceived);
		if (ucResult == EM_SUCCESS)
		{
			*puiRespLen = EG_mifs_tWorkInfo.ulBytesReceived;
			memcpy(pvResp, EG_mifs_tWorkInfo.aucBuffer, *puiRespLen);
		}
		else
		{
			*puiRespLen = 0;
		}

	}
	else
	{
		*puiRespLen = 0;
	}

  SlotBEnd:
	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFAttriB(DevHandle hDevHandle, uint uiSendLen, void* pvSendData, uint* puiRecLen,
*                       void *pvRecData)
* ����������
*     ����ͨѶ������������뿨Ƭ��ͨ��
* ���������
*    hDevHandle     �豸���
*     ulPUPI: αΨһ��PICC��ʾ����������PICC���кŵ�������ʽ��оƬ���кţ�PICC�����ĵ��������
*     ucProtFlag: ȷ����Ƭ�Ƿ�֧��ISO14443-4Э��,0-��֧��,1-֧��
*     ucCID: ָ������0-14
*     uiSendLen: Ҫ�������ݳ���
*     pvSendData: ���Ҫ��������
* ���������
*     puiRecLen: ���յ����ݳ���
*     pvRecData: ���յ�������,���һ���ֽڶ�������
*          b8~b5: MBLI,��󻺳�������������PICC�Դ�֪ͨPCD���������֡���ڲ���������С��
*          b4~b1: CID�����������ucCIDһ�¡�
*
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                2004-08-14          01-01-01    ����
*                2005-10-21          01-02-01    ����һ���ط�����
***********************************************************************/
uchar EA_ucMIFAttriB(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint * puiRecLen,
	void *pvRecData)
{

#ifdef EM_RC531_Module

	uchar ucResult = EM_SUCCESS;
	uchar ucTempData;
	uchar *pucSendData = (uchar *) pvSendData;

	if ((pvSendData == EM_NULL) || (puiRecLen == EM_NULL) || (pvRecData == EM_NULL)
		|| (uiSendLen < 8) || (uiSendLen >= EM_mifs_BUFFER_LEN))
	{
		return EM_ERRPARAM;
	}

	if ((pucSendData[6] != EM_mifs_SUPPORTB) && (pucSendData[6] != EM_mifs_NOTSUPPORTB))
	{
		return EM_ERRPARAM;
	}

	if (pucSendData[7] > EM_mifs_MAXPID)
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EI_mifs_vFlushFIFO();				//empty FIFO

	// RxCRC and TxCRC enable, parity
	ucTempData = 0x2C;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

	// ׼����������
	EG_mifs_tWorkInfo.ulSendBytes = uiSendLen + 1;	// Ҫ�������ݻ��ü���0x1D+PUPI+P1+P2+P3+P4
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_ATTRIB;
	memcpy(&EG_mifs_tWorkInfo.aucBuffer[1], pvSendData, uiSendLen);
	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);
	// �����һ��AttriB��������Ӧ,���͵ڶ���Attrib����.
	// Added by panpingbin 2005.10.21
	if (ucResult == EM_mifs_NOTAGERR)
	{
		ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);
	}

	if (ucResult == EM_SUCCESS)
	{
		if (EG_mifs_tWorkInfo.ulBytesReceived == 0)
		{
			*puiRecLen = 0;
			ucResult = EM_mifs_NOTAGERR;
		}
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x0F) == pucSendData[7])
		{
			*puiRecLen = EG_mifs_tWorkInfo.ulBytesReceived;
			memcpy(pvRecData, EG_mifs_tWorkInfo.aucBuffer, EG_mifs_tWorkInfo.ulBytesReceived);
			EG_mifs_tWorkInfo.ucCurPCB = 1;
		}
		else
		{
			*puiRecLen = 0;
			ucResult = EM_mifs_CODEERR;
		}
	}
	else
	{
		*puiRecLen = 0;
	}

	// ��ʱ500us
	EI_paypass_vDelay(60);
	EI_mifs_vEndProtect();

	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFExchangeB(DevHandle hDevHandle,uchar ucCID, uint uiSendLen, void *pvSendData, uint*puiRecLen, void *pvRecData)
* ����������
*     ��ISO14443-4Ҫ����TYPEB����������
* ���������
*    hDevHandle     �豸���
*     ucCID: �����������ʱ����
*     uiSendLen: Ҫ�������ݳ���
*     pvSendData: ���Ҫ��������
* ���������
*     puiRecLen: ���յ����ݳ���
*     pvRecData: ���յ�������
*
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EA_ucMIFExchangeB(DevHandle hDevHandle, uchar ucCID, uint uiSendLen, void *pvSendData,
	uint * puiRecLen, void *pvRecData)
{
#ifdef EM_RC531_Module

	return EI_mifs_ucExchange(hDevHandle, EM_mifs_TYPEB, ucCID, uiSendLen, pvSendData, puiRecLen,
		pvRecData);
#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*    EA_ucMIFCommandB(DevHandle hDevHandle, uint uiSendLen, void *pvSendData, uint*puiRecLen, void *pvRecData)
* ����������
*     ��ɶ�д����TYPEB�������ݽ���������
* ���������
*    hDevHandle     �豸���
*    ucCID: �����������ʱ����
*    uiSendLen: Ҫ�������ݳ���
*    pvSendData:
* ���������Ҫ��������
*     puiRecLen      :  ʵ�ʷ������ݳ���
*     pvRecData   : ���ص�����,������������
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-12-29          01-01-01    ����
***********************************************************************/
uchar EA_ucMIFCommandB(DevHandle hDevHandle, uchar ucCID, uint uiSendLen, void *pvSendData,
	uint * puiRecLen, void *pvRecData)
{
#ifdef EM_RC531_Module

	return EI_mifs_ucCommand(hDevHandle, EM_mifs_TYPEB, ucCID, uiSendLen, pvSendData, puiRecLen,
		pvRecData);

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif
}

/**********************************************************************
* �������ƣ�
*    EA_ucMIFDeselectB(DevHandle hDevHandle, uchar ucCID)
* ����������
*     ʹ�ѽ���ͨ���Ŀ�Ƭ����HALT״̬
* ���������
*    hDevHandle     �豸���
*     ucCID: ָ������0-14
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
*                 2005-10-21          01-02-01    ����һ���ط�����
***********************************************************************/
uchar EA_ucMIFDeselectB(DevHandle hDevHandle, uchar ucCID)
{

#ifdef EM_RC531_Module

	uchar ucResult;

	if (ucCID > EM_mifs_MAXPID)
	{
		return EM_ERRPARAM;
	}

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	// ISO14443-4�淶Ҫ��: �����һ��Deselect�������ʱ����,��Ӧ�ط�һ��Deselect

	ucResult = EI_mifs_ucDeselect(EM_mifs_TYPEB, ucCID);
	if (ucResult != EM_SUCCESS)
	{
		ucResult = EI_mifs_ucDeselect(EM_mifs_TYPEB, ucCID);
	}

	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*     EA_ucMIFHaltB(DevHandle hDevHandle, ulong ulPUPI)
* ����������
*     ʹ��δ����ͨ���Ŀ�Ƭ����ֹͣ״̬
* ���������
*    hDevHandle     �豸���
*    ulPUPI: αΨһ��PICC��ʾ����������PICC���кŵ�������ʽ��оƬ���кţ�PICC�����ĵ��������
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EA_ucMIFHaltB(DevHandle hDevHandle, ulong ulPUPI)
{

#ifdef EM_RC531_Module

	uchar ucResult = EM_SUCCESS;
	uchar ucTempData;

	ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	EI_mifs_vFlushFIFO();				//empty FIFO

	// RxCRC and TxCRC enable, parity
	ucTempData = 0x2C;
	EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

	EI_mifs_vSetTimer(4);

	// ׼����������: �����룫PUPI
	EG_mifs_tWorkInfo.ulSendBytes = 5;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_HALT;
	EG_mifs_tWorkInfo.aucBuffer[1] = ((uchar *) & ulPUPI)[0];
	EG_mifs_tWorkInfo.aucBuffer[2] = ((uchar *) & ulPUPI)[1];
	EG_mifs_tWorkInfo.aucBuffer[3] = ((uchar *) & ulPUPI)[2];
	EG_mifs_tWorkInfo.aucBuffer[4] = ((uchar *) & ulPUPI)[3];

	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);

	if (ucResult == EM_SUCCESS)
	{
		if (EG_mifs_tWorkInfo.ulBytesReceived == 0)
		{
			ucResult = EM_mifs_NOTAGERR;
		}
		else if ((EG_mifs_tWorkInfo.ulBytesReceived != 1)
			|| (EG_mifs_tWorkInfo.aucBuffer[0] != 0x0))
		{
			ucResult = EM_mifs_CODEERR;
		}
	}

	EI_mifs_vEndProtect();
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/*************************************************************************

                  ����API��������

**************************************************************************/

/*******************************************************************************
* ��������:
*        EA_ucMIFPollMifCard(DevHandle hDevHandle, uchar ucAntiFlag, uchar* pucMifCardType)
* ��������:
*        ��ѯ��Ӧ�����ж��Ƿ���MifCard���и�Ӧ��
* �����º�������:
*        ��
* �������º�����
*        EI_paypass_ucPOLL
* ���������
*        hDevHandle    �豸���
*        ucAntiFlag    �Ƿ�����Ӷ���TypeA���л��һ�ſ�Ƭ�����кţ�ȡֵ���£�
*                       0 - �������Ӧ���ж��ſ�
*                       1 - �����Ӧ���ж��ſ�
* �������:
*       pucMifCardType   �����Ӧ���Ŀ�Ƭ����
*                        EM_mifs_S50          S50�ṹ��С����Mifare One��
*                        EM_mifs_S70          S70�ṹ�Ĵ�����Mifare One��
*                        EM_mifs_PROCARD      Pro��
*                        EM_mifs_PRO_S50      ��������ΪS50ʹ�ã�Ҳ������ΪPro��ʹ��
*                        EM_mifs_PRO_S70      ��������ΪS70ʹ�ã�Ҳ������ΪPro��ʹ��
*                        EM_mifs_TYPEBCARD    TypeB��
* �� �� ֵ��
*       EM_ERRHANDLE     �������
*       EM_ERRPARAM      ��������
*       EM_mifs_SUCCESS  ѯ���ɹ�
*       EM_mifs_NOCARD   �ڹ涨ʱ����û���жϵ��κ���Ƶ��
*       EM_mifs_MULTIERR �ж��ſ��ڸ�Ӧ��
*       EM_mifs_PROTERR  Э�����
*       EM_mifs_TRANSERR ͨ�Ŵ���
* ��ʷ��¼��
*        �޸���           �޸�����             �汾��       �޸�����
*                        2006-10-09           01-05-02        ����
***************************************************************/
uchar EA_ucMIFPollMifCard(DevHandle hDevHandle, uchar ucAntiFlag, uchar * pucMifCardType)
{
	uchar result;

	if (pucMifCardType == EM_NULL)
	{
		return EM_ERRPARAM;
	}

	if ((ucAntiFlag != 0) && (ucAntiFlag != 1))
	{
		return EM_ERRPARAM;
	}

	result = EI_mifs_ucBeginProtect(&hDevHandle, 3);
	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	// �ȳ�ʼ����ر���
	EI_paypass_vInit();
	EG_mifs_tWorkInfo.ucAnticollFlag = ucAntiFlag;
	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_NONE;
//    if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV) //emv ģʽ���ܹر��ز�
        EI_paypass_vOptField(ON);
//    else
//        EI_paypass_vResetPICC();
	//s_DelayMs(10);
	// ���жϸ�Ӧ�����Ƿ��п�
	result = EI_paypass_ucPOLL(pucMifCardType);
	if (result == EM_mifs_TIMEOUT)
	{
		result = EM_mifs_NOCARD;
	}
	if(!result)
	{
		if(EG_mifs_tWorkInfo.ucMifActivateFlag == RFID_ACTIVE_NONE)
		{
			//δѰ����ΪѰ���ɹ�������Ѿ�����������
			EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_POLLOK;
		}
	}
	EI_mifs_vEndProtect();
	return result;
}

/******************************************************************************
* �������ƣ�
*       EA_ucMIFActivate(DevHandle hDevHandle, uchar ucMifType, uint* puiOutLen, void* pvOut)
* ����������
*       �����Ӧ���ڵ�MifCard
* �����º������ã�
*       API
* �������º�����
*       EI_paypass_ucActPro
*       EI_paypass_ucActTypeB
*       EI_paypass_vResetPICC
*       EI_paypass_vDelay
* ���������
*       hDevHandle  ��Ƶ���豸���
*       ucMifType   ��Ӧ���ڵĿ�Ƭ����,ȡֵ���£�
*                     EM_mifs_S50          S50�ṹ��С����Mifare One��
*                     EM_mifs_S70          S70�ṹ�Ĵ�����Mifare One��
*                     EM_mifs_PROCARD      Pro��
*                     EM_mifs_TYPEBCARD    TypeB��
* ���������
*       puiOutLen  ��������ݳ���
*       pvOut      ��������ݵ�ַ������Ӧ�ó��򿪱ٴ���256�ֽڵĿռ䣬
*       ATQA(LByte+HByte)+ sn(���ŵ��ֽ���ǰ4B)+sncheck(����У��1B) + SAK(1B) + ATR
* �� �� ֵ��
*       EM_ERRHANDLE     �������
*       EM_ERRPARAM      ��������
*       EM_mifs_SUCCESS  ����ɹ�
*       EM_mifs_TIMEOUT  ���ʱ
*       EM_mifs_TRANSERR ��������г��ִ������
*       EM_mifs_PROTERR  ��Ƭ�������ݲ����Ϲ淶
* ��ʷ��¼��
*        �޸���           �޸�����             �汾��       �޸�����
*                        2006-10-09           01-05-02        ����
***************************************************************/
uchar EA_ucMIFActivate(DevHandle hDevHandle, uchar ucMifType, uint * puiOutLen, void *pvOut)
{
	uchar result = EM_mifs_SUCCESS;
	uchar ucATSLen = 0x00;
	uchar ucATSData[255];
	uchar *pucOut = (uchar *) pvOut;
    uint i;
	memset(ucATSData, 0x00, sizeof(ucATSData));

	if ((puiOutLen == EM_NULL) || (pvOut == EM_NULL))
	{
		return EM_ERRPARAM;
	}

	*puiOutLen = 0;

	result = EI_mifs_ucBeginProtect(&hDevHandle, 3);
	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	EI_mifs_vFlushFIFO();				// empty FIFO
	if(EG_mifs_tWorkInfo.ucMifActivateFlag < RFID_ACTIVE_POLLOK)
	{
		//δpollֱ�ӷ��س�ʱ
		return EM_mifs_TIMEOUT;
	}
	if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV)
	{
		//pollִֻ��WUPA��WUPB
	//ActivateAB:
		if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEB)
		{
			// �����TypeB��
	ActivateB:
			result = EI_paypass_ucActTypeB(&ucATSLen, ucATSData);
		}
		else
		{
			result = EI_paypass_ucActPro(&ucATSLen, ucATSData);
		}
	}
	else
	{
		//ISOͨ��ģʽ
		if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEB)
		{
			//Ѱ��B����δ����
			goto ActivateB;
		}
		else
		{
			//Ѱ��A���Ѿ�����
			//�Ϻ����Ų�����Ƶ�����Է��ֲ���2�μ��
			result = EM_mifs_SUCCESS;
		}
		/*
		if(EG_mifs_tWorkInfo.ucAnticollFlag == 0)
		{
			//����Ѱ1�ſ�,Ѱ��A����B�����л��ز��������Ҫ���¼���
			EI_paypass_vResetPICC();
			goto ActivateAB;
		}
		else
		{
			//Ѱ���ſ�
			if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEB)
			{
				//Ѱ��B����δ����
				goto ActivateB;
			}
			else
			{
				//Ѱ��A���Ѿ�����
				//�Ϻ����Ų�����Ƶ�����Է��ֲ���2�μ��
				result = EM_mifs_SUCCESS;
			}
		}
		*/
	}
	if(result == EM_mifs_SUCCESS)
	{
		i = 0;
		if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEA)
		{
			// ���ؼ�������л�õ�ȫ������
			memcpy(&pucOut[i],EG_mifs_tWorkInfo.ucATQA,2);
			i += 2;
			if (EG_mifs_tWorkInfo.ucUIDLen >= 4)
			{
				memcpy(&pucOut[i], EG_mifs_tWorkInfo.ucUIDCL1, 5);
				i += 5;
				memcpy(&pucOut[i], &EG_mifs_tWorkInfo.ucSAK1, 1);
				i += 1;
			}
			else if (EG_mifs_tWorkInfo.ucUIDLen >= 7)
			{
				memcpy(&pucOut[i], EG_mifs_tWorkInfo.ucUIDCL2, 5);
				i += 5;
				memcpy(&pucOut[i], &EG_mifs_tWorkInfo.ucSAK2, 1);
				i += 1;
			}
			else
			{
				memcpy(&pucOut[i], EG_mifs_tWorkInfo.ucUIDCL3, 5);
				i += 5;
				memcpy(&pucOut[i], &EG_mifs_tWorkInfo.ucSAK3, 1);
				i += 1;
			}
            memcpy(EG_mifs_tWorkInfo.ATS, ucATSData, ucATSLen);
			memcpy(&pucOut[i], EG_mifs_tWorkInfo.ATS, EG_mifs_tWorkInfo.ATS[0]);
			i += EG_mifs_tWorkInfo.ATS[0];
		}
		else
		{
			// ATS����
			memcpy(&pucOut[i], ucATSData, ucATSLen);
			i += ucATSLen;
		}
		//2012-01-04  ���ز�������1���ֽڱ�ʾ������
		memcpy(&pucOut[i], &EG_mifs_tWorkInfo.ucMifCardType, 1);
		i += 1;
		*puiOutLen = i;
	}
	EI_mifs_vEndProtect();
	return result;
}

/**************************************************************************
* �������ƣ�
*        EA_ucMIFAPDU(DevHandle hDevHandle, uint uiSendLen, void* pvInData,
*                     uint* puiRecLen, void* pvOutData)
* ��������:
*        ʵ���豸��Mif��֮�䰴��ISO14443-4Э�����ͨ��
* �����º������ã�
*        API
* �������º�����
*        ��
* ���������
*        hDevHandle   �豸���
*        uiSendLen    �ն˷��͵����ݳ���
*        pvInData     �ն�Ҫ���͵�����
* ���������
*        puiRecLen    �����͵����ݳ���
*        pvOutData    �����͵�����
* �� �� ֵ��
*        EM_ERRHANDLE     �������
*        EM_ERRPARAM      ��������
*        EM_mifs_SUCCESS  ���ݽ����ɹ�����Ƭ����״̬�ֽ�Ϊ0x9000��
*        EM_mifs_NOACT    ��Ƭδ����
*        EM_mifs_SWDIFF   ��Ƭ����״̬�ֽ�Ϊ�������� 0x9000��
*        EM_mifs_TIMEOUT  ��Ƭ�޷���
*        EM_mifs_PROTERR  ��Ƭ���ص�����Υ��Э��
*        EM_mifs_TRANSERR ͨ�Ź����г��ִ���
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EA_ucMIFAPDU(DevHandle hDevHandle, uint uiSendLen, void *pvInData,
	uint * puiRecLen, void *pvOutData)
{
	uchar ucRet = EM_mifs_SUCCESS;
	uchar pucSendData[300];
	uchar pucRecData[300];
	uint uiSendLenTemp = 0;
	uint uiRecLenTemp = 0;

	if ((pvInData == EM_NULL) || (puiRecLen == EM_NULL) || (pvOutData == EM_NULL))
	{
		return EM_ERRPARAM;
	}
	memset(pucSendData, 0x00, sizeof(pucSendData));
	memset(pucRecData, 0x00, sizeof(pucRecData));
	// ��ʼ������
	*puiRecLen = 0;
	memcpy(pucSendData, (uchar *) pvInData, uiSendLen);
	uiSendLenTemp = uiSendLen;

	if (EG_mifs_tWorkInfo.ucMifActivateFlag < RFID_ACTIVE_OK)
	{
		// ��û�н����κμ��Ƭ�Ĳ���
		return EM_mifs_NOACT;
	}
	if(EG_mifs_tWorkInfo.ucMifCardType < RFID_CARD_PRO)
	{
		return EM_mifs_TIMEOUT;
	}
	ucRet = EI_mifs_ucBeginProtect(&hDevHandle, 3);
	if (ucRet != EM_mifs_SUCCESS)
	{
		return ucRet;
	}
	if ((uiSendLen != 4) && (uiSendLen != 5)	// �����ݳ���Ϊ4ʱ����ʾ�Ȳ�����Ҳ����������
		&& (uiSendLen != (uint) (pucSendData[4] + 5)))
	{
		if (uiSendLen == (uint) (pucSendData[4] + 6))
		{
			if (pucSendData[4] == 0)	// Lc = 0
			{
				pucSendData[4] = pucSendData[5];
				uiSendLenTemp = 5;
			}
		}
		else
		{
			return EM_ERRPARAM;
		}
	}

	if (uiSendLen == 4)
	{
		// CASE 1 ����£�P3 = 0
		pucSendData[4] = 0x00;
		uiSendLenTemp = 5;
	}
	ucRet = EI_paypass_ucExchange(uiSendLenTemp, pucSendData, &uiRecLenTemp, pucRecData);
	//TRACE("\r\n ucRet:%x",ucRet);
	if ((ucRet != EM_mifs_SUCCESS) && (ucRet != EM_mifs_SWDIFF))
	{
		// ����ͨ��ʧ�ܣ�����DEACTIVATE���̡�
//        if(!EG_mifs_tWorkInfo.ucCIDPollFlg)
        {
            EI_paypass_ucDeSelect();
        }
		*puiRecLen = 0x00;
	}
	else
	{
		*puiRecLen = uiRecLenTemp;
		memcpy(pvOutData, (void *)pucRecData, uiRecLenTemp);
	}

	EI_mifs_vEndProtect();

	return ucRet;
}

/**********************************************************************
* �������ƣ�
*       EA_ucMIFDeActivate(DevHandle hDevHandle)
* ����������
*       ����Ƭ��ΪHALT�����ȴ���Ƭ�뿪��Ӧ��
* �����º������ã�
*       API
* �������º�����
*       ��
* ���������
*       hDevHanle �豸���
* ���������
*       ��
* �� �� ֵ��
*        EM_ERRHANDLE      �������
*        EM_ERRPARAM       ��������
*        EM_mifs_SUCCESS   �����ɹ�
*        EM_mifs_CARDEXIST �ڹ涨ʱ����û�н����ÿ���Ӧ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
*                        2006-02-23           ����
***************************************************************/
uchar EA_ucMIFDeactivate(DevHandle hDevHandle)
{
	uchar result = EM_mifs_SUCCESS;

	result = EI_mifs_ucBeginProtect(&hDevHandle, 3);
	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}

	EI_mifs_vFlushFIFO();				// empty FIFO

	if ((EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S50)
		|| (EG_mifs_tWorkInfo.ucMifCardType == EM_mifs_S70))
	{
        if (EG_mifs_tWorkInfo.ucMifActivateFlag > RFID_ACTIVE_NONE)
            EI_paypass_vHALTA();
		result = EM_mifs_SUCCESS;
	}
	else
	{
		if (EG_mifs_tWorkInfo.ucMifActivateFlag == RFID_ACTIVE_OK)
		{
			// ����DEACTIVATE����
//			result = EI_paypass_ucDeSelect();
			result = EM_mifs_SUCCESS; //����ֱ����������Ҫ����deselect����04122016
		}
		else
		{
			result = EM_mifs_SUCCESS;
		}

	}

	EG_mifs_tWorkInfo.ucMifActivateFlag = RFID_ACTIVE_NONE;

	EI_mifs_vEndProtect();
	return EM_mifs_SUCCESS;
}

/**********************************************************************

                         Mifare����������

***********************************************************************/
/**********************************************************************
* �������ƣ�
*     EI_mifs_ucHandleATQB(uint uiLength)
* ����������
*     ����RequestB����SlotMarkerB�ķ�������
* ���������
*    uiLength   �������ݳ���
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EI_mifs_ucHandleATQB(uint uiLength)
{
	if (uiLength == 0)
	{
		return EM_mifs_NOTAGERR;
	}
	if ((uiLength < 3) || ((EG_mifs_tWorkInfo.aucBuffer[0] != 0x50)
			&& (EG_mifs_tWorkInfo.aucBuffer[0] != 0x05)))
	{
		return EM_mifs_CODEERR;
	}
	else if (uiLength > 12)
	{
		return EM_mifs_COLLERR;
	}

	if (uiLength == 12)
	{
		if ((EG_mifs_tWorkInfo.aucBuffer[11] & 0x1) != 0)
		{
			// ֧��CID
			EG_mifs_tWorkInfo.ucCIDSupport = 1;
		}
		else
		{
			// ��֧��CID
			EG_mifs_tWorkInfo.ucCIDSupport = 0;
		}

		EG_mifs_tWorkInfo.ulMaxLen = (EG_mifs_tWorkInfo.aucBuffer[10] >> 4) & 0x0F;
		EG_mifs_tWorkInfo.ucFWI = (EG_mifs_tWorkInfo.aucBuffer[11] >> 4) & 0x0F;
	}
	return EM_SUCCESS;
}

/**********************************************************************
* �������ƣ�
*     EI_mifs_ucHandleATQA(uint uiLength)
* ����������
*     ����MifarePro������ʱ�ķ�������
* ���������
*    uiLength   �������ݳ���
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EI_mifs_ucHandleATQA(uint uiLength)
{
	uint uiTemp;
	uchar ucFormat;
	uint uiCurPos = 1;

	// ���ճ����ֽ�
	uiTemp = EG_mifs_tWorkInfo.aucBuffer[0];

	if (uiTemp != uiLength)
	{
		// �������ݳ��ȴ���
		return EM_mifs_CODEERR;
	}

	ucFormat = EG_mifs_tWorkInfo.aucBuffer[1];
	EG_mifs_tWorkInfo.ulMaxLen = 128;	//ucFormat & 0x0F; // FSC����

	if ((ucFormat & 0x10) == 0x10)
	{
		uiCurPos++;
		if (uiCurPos >= uiTemp)
		{
			return EM_mifs_CODEERR;
		}
	}

	if ((ucFormat & 0x20) == 0x20)
	{
		// ����TB1
		uiCurPos++;
		if (uiCurPos >= uiTemp)
		{
			return EM_mifs_CODEERR;
		}
		EG_mifs_tWorkInfo.ucFWI = (EG_mifs_tWorkInfo.aucBuffer[uiCurPos] >> 4) & 0x0F;
	}

	if ((ucFormat & 0x40) == 0x40)
	{
		// ����TC1
		uiCurPos++;
		if (uiCurPos >= uiTemp)
		{
			return EM_mifs_CODEERR;
		}
		if ((EG_mifs_tWorkInfo.aucBuffer[uiCurPos] & 0x02) == 0x02)
		{
			EG_mifs_tWorkInfo.ucCIDSupport = 1;
		}
		else
		{
			EG_mifs_tWorkInfo.ucCIDSupport = 0;
		}
	}

	EG_mifs_tWorkInfo.ucCurPCB = 1;

	return EM_SUCCESS;

}

/**********************************************************************
* �������ƣ�
*     EI_mifs_ucExchange(DevHandle hDevHandle, uchar ucType, uchar ucCID,uint uiSendLen, void *pvSendData, uint*puiRecLen, void *pvRecData)
* ����������
*     ��ɶ�д���뿨�ķ���14443-4Э������ݽ���������
* ���������
*    hDevHandle     �豸���
*    ucType: ������
*    ucCID: ��ʱ����
*    uiSendLen: Ҫ�������ݳ���
*    pvSendData:   Ҫ��������
* ���������
*     puiRecLen      :  ʵ�ʷ������ݳ���
*     pvRecData   : ���ص�����,������������
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                   2004-08-14          01-01-01    ����
*                   2004-10-21          01-02-01    �޸�CASE4ʵ��
*                   2006-10-09          01-05-02    �����˳���Խ����ж�
***********************************************************************/
uchar EI_mifs_ucExchange(DevHandle hDevHandle, uchar ucType, uchar ucCID, uint uiSendLen,
	void *pvSendData, uint * puiRecLen, void *pvRecData)
{
	uchar *pucSendTemp = (uchar *) pvSendData;
	uint uiSendLeftLen = 0, uiTempLen = 0, uiSendCurrPos = 0, uiCurrRecvLen = 0;
	uchar ucResult = EM_SUCCESS;
	uchar *pucRecData = (uchar *) pvRecData;
	uchar ucReSendNo = 0x00;

	if (ucCID > EM_mifs_MAXPID)
	{
		return EM_ERRPARAM;
	}

	if ((uiSendLen < 4) || (pvSendData == EM_NULL) || (puiRecLen == EM_NULL)
		|| (pvRecData == EM_NULL))
	{
		return EM_ERRPARAM;
	}

	if (uiSendLen > 5)
	{
		if ((uiSendLen != (uint) pucSendTemp[4] + 6) && (uiSendLen != (uint) pucSendTemp[4] + 5))
		{
			return EM_ERRPARAM;
		}
	}

	if (uiSendLen == 4)					// ����ISO7816-4��׼������CASE1�����Ӧ����P3 = 0x00��ͬʱ��ͬCLA��INS��P1��P2һͬ����ȥ
	{
		uiSendLen = 5;
		*(pucSendTemp + 4) = 0x00;
	}

	uiSendLeftLen = uiSendLen;
	*puiRecLen = 0;

	if (ucType == EM_mifs_TYPEA)
	{
		ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	}
	else
	{
		ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	}

	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}
	/*
	D1(
		TRACE("\r\n ��������:%d - %d", uiSendLeftLen, EG_mifs_tWorkInfo.ulMaxLen);
		DISPBUF(pvSendData, uiSendLeftLen, 0);
	);
	*/
	//������I_block
	//�����֡���Ȱ�������CRCУ����ֽں�PCB��CID,�����NAD��Ӧ��5
	while (uiSendLeftLen > (EG_mifs_tWorkInfo.ulMaxLen - 4))
	{
		uiTempLen = EG_mifs_tWorkInfo.ulMaxLen - 4;	// �����֡���ȷ���
		EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
		if (EG_mifs_tWorkInfo.ucCIDSupport == 1)
		{
			// ֧��CID
			memcpy(EG_mifs_tWorkInfo.aucBuffer + 2, pucSendTemp + uiSendCurrPos, uiTempLen);
			// ����֡��
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0x1A;	// PCB��ֵ,��ǰ��NAD
			EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucPwrLevel | ucCID;	//CID��ֵ,EG_PliĬ��ֵΪ0,�ڿ�Ƭ��S�����л��޸�
			ucResult = EI_mifs_ucProcess(ucType, uiTempLen + 2, &uiCurrRecvLen);
		}
		else
		{
			// ��֧��CID
			memcpy(EG_mifs_tWorkInfo.aucBuffer + 1, pucSendTemp + uiSendCurrPos, uiTempLen);
			// ����֡��
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0x12;	// PCB��ֵ,��ǰ��NAD
			ucResult = EI_mifs_ucProcess(ucType, uiTempLen + 1, &uiCurrRecvLen);
		}
		// ���ȷ���ǿ�Ƭ��������ʱ���մ��������ԭ��,���ڲ���ȷ���Ƿ�����Ч�ķ���ֵ,��˶������еĴ����ȡRULE 4�Ĺ涨
		if ((ucResult != EM_SUCCESS) || (uiCurrRecvLen == 0))	//�뿨Ƭ��������ʧ��,Ҫ���ط�������R_block(NAK)����3��
		{
			ucResult = EI_mifs_ucSendRBlock(ucType, ucCID, &uiCurrRecvLen);
			if (ucResult != EM_SUCCESS)
			{
				goto mifsExchangeEnd;
			}
		}

		if (uiCurrRecvLen == 0)
		{
			// ���ݽ���ʧ��
			ucResult = EM_mifs_CODEERR;
			goto mifsExchangeErr;
		}

		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
		{
			// �յ�I-Block������
			ucResult = EM_mifs_CODEERR;
			goto mifsExchangeErr;
		}
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
		{
			// R-Block
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
			{
				// ����CID
				if (EG_mifs_tWorkInfo.aucBuffer[1] != (EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
				{
					// �յ�R_blockӦ������,��CID�ֽڵ�CID����
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
			}
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x10)
			{
				// ����Ӧ�÷���NAK
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) == EG_mifs_tWorkInfo.ucCurPCB)
			{
				// �յ�R_block(ACK)Ӧ�������ȷ, ׼��������һ����Ϣ��
				uiSendCurrPos += uiTempLen;
				uiSendLeftLen -= uiTempLen;
			}
			else
			{
				if (ucReSendNo++ > 3)
				{
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
				// �����Ų���ȷ, ���ط�
				EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
			}
		}
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC7) == 0xC2)
		{
			// S-BLock
			while (1)
			{
				// ����S-BlockӦ��
				ucResult = EI_mifs_ucSendSBlock(ucType, ucCID, &uiCurrRecvLen);
				if (ucResult != EM_SUCCESS)
				{
					goto mifsExchangeEnd;
				}

				if (uiCurrRecvLen == 0)
				{
					// ���ݽ���ʧ��
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}

				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
				{
					// �յ�I-Block, ����
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
				else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
				{
					// R-Block
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
					{
						if (EG_mifs_tWorkInfo.aucBuffer[1] !=
							(EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
						{
							// �յ�R_blockӦ������,��CID�ֽڵ�CID����
							ucResult = EM_mifs_CODEERR;
							goto mifsExchangeErr;
						}
					}

					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x10)
					{
						// �յ�NAK, ����
						ucResult = EM_mifs_CODEERR;
						goto mifsExchangeErr;
					}
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) == EG_mifs_tWorkInfo.ucCurPCB)
					{
						// �յ�R_block(ACK)Ӧ�������ȷ, ׼��������һ����Ϣ��
						uiSendCurrPos += uiTempLen;
						uiSendLeftLen -= uiTempLen;
					}
					else
					{
						if (ucReSendNo++ > 3)
						{
							ucResult = EM_mifs_CODEERR;
							goto mifsExchangeErr;
						}
						// �����Ų���ȷ, ���ط�
						EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
					}
					break;
				}
				else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC7) != 0xC2)
				{
					// ���յ���������Block
					return EM_mifs_CODEERR;
				}
			}
		}
		else
		{
			// �յ�����Ŀ�
			ucResult = EM_mifs_CODEERR;
			goto mifsExchangeErr;
		}
	}

	ucReSendNo = 0x00;
	// ���һ֡�����ݴ���
	while (1)
	{
		EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
		if (EG_mifs_tWorkInfo.ucCIDSupport == 1)
		{
			// ֧��CID
			memcpy(EG_mifs_tWorkInfo.aucBuffer + 2, pucSendTemp + uiSendCurrPos, uiSendLeftLen);
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0x0A;	// PCB��ֵ,��ǰ��NAD
			EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucPwrLevel | ucCID;	//CID��ֵ,EG_PliĬ��ֵΪ0,�ڿ�Ƭ��S�����л��޸�
			ucResult = EI_mifs_ucProcess(ucType, uiSendLeftLen + 2, &uiCurrRecvLen);
		}
		else
		{
			// ��֧��CID
			memcpy(EG_mifs_tWorkInfo.aucBuffer + 1, pucSendTemp + uiSendCurrPos, uiSendLeftLen);
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0x02;	// PCB��ֵ,��ǰ��NAD
			ucResult = EI_mifs_ucProcess(ucType, uiSendLeftLen + 1, &uiCurrRecvLen);
		}
		// ���ȷ���ǿ�Ƭ��������ʱ���մ��������ԭ��,���ڲ���ȷ���Ƿ�����Ч�ķ���ֵ,��˶������еĴ����ȡRULE 4�Ĺ涨
		if ((ucResult != EM_SUCCESS) || (uiCurrRecvLen == 0))	// �뿨Ƭ��������ʧ��,Ҫ���ط�������R_block(NAK)����3��
		{
			ucResult = EI_mifs_ucSendRBlock(ucType, ucCID, &uiCurrRecvLen);
			if (ucResult != EM_SUCCESS)
			{
				goto mifsExchangeEnd;
			}
		}

		if (uiCurrRecvLen == 0)
		{
			// ���ݽ���ʧ��
			ucResult = EM_mifs_CODEERR;
			goto mifsExchangeErr;
		}

		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
		{
			// R-Block
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
			{
				if (EG_mifs_tWorkInfo.aucBuffer[1] != (EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
				{
					// �յ�R_blockӦ������,��CID�ֽڵ�CID����
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
			}
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x10)
			{
				// ����Ӧ�÷���NAK
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) == EG_mifs_tWorkInfo.ucCurPCB)
			{
				// �յ�R_block(ACK)Ӧ������
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			else
			{
				if (ucReSendNo++ > 3)
				{
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
				// �����ط�
				EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
			}
		}
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC7) == 0xC2)
		{
			// S-BLock
			while (1)
			{
				// ����S-BlockӦ��
				ucResult = EI_mifs_ucSendSBlock(ucType, ucCID, &uiCurrRecvLen);
				if (ucResult != EM_SUCCESS)
				{
					goto mifsExchangeEnd;
				}

				if (uiCurrRecvLen == 0)
				{
					// ���ݽ���ʧ��
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}

				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
				{
					// �յ�I-Block
					break;
				}
				else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
				{
					// R-Block
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
					{
						if (EG_mifs_tWorkInfo.aucBuffer[1] !=
							(EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
						{
							// �յ�R_blockӦ������,��CID�ֽڵ�CID����
							ucResult = EM_mifs_CODEERR;
							goto mifsExchangeErr;
						}
					}
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x10)
					{
						// ����Ӧ�÷���NAK
						ucResult = EM_mifs_CODEERR;
						goto mifsExchangeErr;
					}
					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) == EG_mifs_tWorkInfo.ucCurPCB)
					{
						// �յ�R_block(ACK)Ӧ������
						ucResult = EM_mifs_CODEERR;
						goto mifsExchangeErr;
					}
					else
					{
						if (ucReSendNo++ > 3)
						{
							ucResult = EM_mifs_CODEERR;
							goto mifsExchangeErr;
						}
						// �����ط�
						EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
					}
					break;
				}
				else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC7) != 0xC2)
				{
					// ���յ���������Block
					return EM_mifs_CODEERR;
				}
			}
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
			{
				// �յ�I-Block
				break;
			}
		}
		else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
		{
			// ׼�����տ����͵�I-Block
			break;
		}
		else
		{
			// �յ�����Ŀ�
			ucResult = EM_mifs_CODEERR;
			goto mifsExchangeErr;
		}
	}

	// ���տ�����I-block
	while (1)
	{
		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) != EG_mifs_tWorkInfo.ucCurPCB)
		{
			// �յ�R_block(ACK)Ӧ������
			ucResult = EM_mifs_CODEERR;
			goto mifsExchangeErr;
		}
		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
		{
			// ���ӳ���Խ���жϣ�
			if (uiCurrRecvLen < 2)
			{
				// �յ�I_block�����,���ǳ��ȴ���
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			if (EG_mifs_tWorkInfo.aucBuffer[1] != (EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
			{
				// �յ�I_block�����,��CID�ֽڵ�CID����
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			// ����CID
			memcpy(&pucRecData[*puiRecLen], &EG_mifs_tWorkInfo.aucBuffer[2], uiCurrRecvLen - 2);
			*puiRecLen += uiCurrRecvLen - 2;
		}
		else
		{
			// ���ӳ���Խ���жϣ�
			if (uiCurrRecvLen < 1)
			{
				// �յ�I_block�����,���ǳ��ȴ���
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			// ������CID
			memcpy(&pucRecData[*puiRecLen], &EG_mifs_tWorkInfo.aucBuffer[1], uiCurrRecvLen - 1);
			*puiRecLen += uiCurrRecvLen - 1;
		}
		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0)
		{
			// ���һ֡
			break;
		}

		EG_mifs_tWorkInfo.ucCurPCB = (EG_mifs_tWorkInfo.ucCurPCB + 1) & 0x01;
		ucReSendNo = 0x00;
		// ISO14443-4�淶�涨����PICC���ӵ�����£�PCD���κ������ֻ�ܷ���R(ACK)
		while (1)
		{
			// ����R-Block
			if (EG_mifs_tWorkInfo.ucCIDSupport == 1)
			{
				EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0xAA;	// PCB��ֵ,��ǰ��NAD
				EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucPwrLevel | ucCID;	//CID��ֵ,EG_PliĬ��ֵΪ0,�ڿ�Ƭ��S�����л��޸�
				ucResult = EI_mifs_ucProcess(ucType, 2, &uiCurrRecvLen);
			}
			else
			{
				EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0xA2;	// PCB��ֵ,��ǰ��NAD
				ucResult = EI_mifs_ucProcess(ucType, 1, &uiCurrRecvLen);
			}

			if ((ucResult != EM_SUCCESS) || (uiCurrRecvLen == 0))
			{
				// ���մ������ط�R_Block
				if (ucReSendNo++ > 3)
				{
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
				continue;
			}

			if (uiCurrRecvLen == 0)
			{
				// ���ݽ���ʧ��
				ucResult = EM_mifs_CODEERR;
				goto mifsExchangeErr;
			}
			if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
			{
				// I_Block
				break;
			}
			else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
			{
				// R-Block
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
				{
					if (EG_mifs_tWorkInfo.aucBuffer[1] != (EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
					{
						// �յ�R_blockӦ������,��CID�ֽڵ�CID����
						ucResult = EM_mifs_CODEERR;
						goto mifsExchangeErr;
					}
				}
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x10)
				{
					// ����Ӧ�÷���NAK
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) == EG_mifs_tWorkInfo.ucCurPCB)
				{
					// �յ�R_block(ACK)Ӧ������
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}

				if (ucReSendNo++ > 3)
				{
					ucResult = EM_mifs_CODEERR;
					goto mifsExchangeErr;
				}
				continue;
			}
			else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC7) == 0xC2)
			{
				while (1)
				{
					// ����S-BlockӦ��
					ucResult = EI_mifs_ucSendSBlock(ucType, ucCID, &uiCurrRecvLen);
					if (ucResult != EM_SUCCESS)
					{
						goto mifsExchangeEnd;
					}

					if (uiCurrRecvLen == 0)
					{
						// ���ݽ���ʧ��
						ucResult = EM_mifs_CODEERR;
						goto mifsExchangeErr;
					}

					if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
					{
						// �յ�I-Block
						break;
					}
					else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0xA2)
					{
						// R-Block
						if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
						{
							if (EG_mifs_tWorkInfo.aucBuffer[1] !=
								(EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
							{
								// �յ�R_blockӦ������,��CID�ֽڵ�CID����
								ucResult = EM_mifs_CODEERR;
								goto mifsExchangeErr;
							}
						}
						if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x10) == 0x10)
						{
							// ����Ӧ�÷���NAK
							ucResult = EM_mifs_CODEERR;
							goto mifsExchangeErr;
						}
						if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x01) == EG_mifs_tWorkInfo.ucCurPCB)
						{
							// �յ�R_block(ACK)Ӧ������
							ucResult = EM_mifs_CODEERR;
							goto mifsExchangeErr;
						}
						else
						{
							if (ucReSendNo++ > 3)
							{
								ucResult = EM_mifs_CODEERR;
								goto mifsExchangeErr;
							}
						}
						break;
					}
					else if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xC7) != 0xC2)
					{
						// ���յ���������Block
						return EM_mifs_CODEERR;
					}
				}
				if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xE2) == 0x02)
				{
					// �յ�I-Block
					break;
				}
			}
			else
			{
				return EM_mifs_CODEERR;
			}
		}
		ucReSendNo = 0;					// ���յ�I_Block, ��������н���
	}

	if (*puiRecLen == 0)
	{
		ucResult = EM_mifs_NOTAGERR;
	}
	else if (*puiRecLen > 1)
	{
		if ((pucRecData[*puiRecLen - 2] != 0x90) || (pucRecData[*puiRecLen - 1] != 0x00))
		{
			ucResult = EM_mifs_SWDIFF;
		}
		goto mifsExchangeEnd;
	}
	else
	{
		ucResult = EM_mifs_CODEERR;
	}
  mifsExchangeErr:
	if ((ucResult != EM_TIMEOUT) && (ucResult != EM_mifs_NOTAGERR))
	{
		// ����Ѿ���ʱ�����߿�û����Ӧ��û��Ҫ�ٵ���deselect��
		EI_mifs_ucDeselect(ucType, ucCID);
	}

  mifsExchangeEnd:

	EI_mifs_vEndProtect();
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EI_mifs_ucCommand(DevHandle hDevHandle, uchar ucType, uchar ucCID,uint uiSendLen, void *pvSendData, uint*puiRecLen, void *pvRecData)
* ����������
*     ��ɶ�д���뿨�����ݽ���������
* ���������
*    hDevHandle     �豸���
*    ucType: ������
*    ucCID: ��ʱ����
*    uiSendLen: Ҫ�������ݳ���
*    pvSendData:   Ҫ��������
* ���������
*     puiRecLen   :  ʵ�ʷ������ݳ���
*     pvRecData   : ���ص�����,������������
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                  2004-08-14          01-01-01    ����
*                  2004-10-21          01-02-01    �޸�CASE4ʵ��
***********************************************************************/
uchar EI_mifs_ucCommand(DevHandle hDevHandle, uchar ucType, uchar ucCID, uint uiSendLen,
	void *pvSendData, uint * puiRecLen, void *pvRecData)
{
	uchar ucResult = EM_SUCCESS;

	if ((ucCID > EM_mifs_MAXPID) || (uiSendLen > EM_mifs_BUFFER_LEN) || (pvSendData == EM_NULL)
		|| (puiRecLen == EM_NULL) || (pvRecData == EM_NULL))
	{
		return EM_ERRPARAM;
	}

	*puiRecLen = 0;

	if (ucType == EM_mifs_TYPEA)
	{
		ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 1);
	}
	else
	{
		ucResult = EI_mifs_ucBeginProtect(&hDevHandle, 2);
	}

	if (ucResult != EM_SUCCESS)
	{
		return ucResult;
	}

	memcpy(EG_mifs_tWorkInfo.aucBuffer, pvSendData, uiSendLen);

	// ����ISO7816-4��׼��CASE1����µ����������P3Ϊ0x00
	if (uiSendLen == 5)
	{
		EG_mifs_tWorkInfo.aucBuffer[uiSendLen] = 0x00;
		uiSendLen = 6;
	}

	ucResult = EI_mifs_ucProcess(ucType, uiSendLen, puiRecLen);
	if (ucResult == EM_SUCCESS)
	{
		memcpy(pvRecData, EG_mifs_tWorkInfo.aucBuffer, *puiRecLen);
	}

	EI_mifs_vEndProtect();
	return ucResult;

}

/**********************************************************************
* �������ƣ�
*    EI_mifs_ucSendRBlock(uchar ucType, uchar ucCID, uint* puiRecLen)
* ����������
*    ����R-BLOCK
* ���������
*    ucType: ������
*    ucCID: ��ʱ����
* ���������
*     puiRecLen      :  ʵ�ʷ������ݳ���
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EI_mifs_ucSendRBlock(uchar ucType, uchar ucCID, uint * puiRecLen)
{
	uint i;
	uchar ucResult;
	for (i = 0; i < 3; i++)
	{
		if (EG_mifs_tWorkInfo.ucCIDSupport == 1)
		{
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0xBA;	// NAK
			// CID��ֵ,PLIĬ��ֵΪ0,�ڿ�Ƭ��S�����л��޸�
			EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucPwrLevel | ucCID;
			ucResult = EI_mifs_ucProcess(ucType, 2, puiRecLen);
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] = EG_mifs_tWorkInfo.ucCurPCB | 0xB2;	// NAK
			ucResult = EI_mifs_ucProcess(ucType, 1, puiRecLen);
		}

		if ((ucResult == EM_SUCCESS) && (*puiRecLen != 0))
		{
			break;
		}
	}
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*    EI_mifs_ucSendSBlock(uchar ucType, uchar ucCID, uint* puiRecLen)
* ����������
*    ����S-BLOCK
* ���������
*    ucType: ������
*    ucCID: ��ʱ����
* ���������
*     puiRecLen      :  ʵ�ʷ������ݳ���
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EI_mifs_ucSendSBlock(uchar ucType, uchar ucCID, uint * puiRecLen)
{
	uchar ucTemp, ucResult;

	if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x30) != 0x30)
	{
		// ����WTX������
		return EM_mifs_CODEERR;
	}
	if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
	{
		if (EG_mifs_tWorkInfo.aucBuffer[1] != (EG_mifs_tWorkInfo.ucPwrLevel | ucCID))
		{
			// �յ�R_blockӦ������,��CID�ֽڵ�CID����
			return EM_mifs_CODEERR;
		}
		// ����CID
		ucTemp = EG_mifs_tWorkInfo.aucBuffer[2];
	}
	else
	{
		ucTemp = EG_mifs_tWorkInfo.aucBuffer[1];
	}

	// ����S_blockӦ���
	if (EG_mifs_tWorkInfo.ucCIDSupport == 1)
	{
		EG_mifs_tWorkInfo.aucBuffer[0] = 0xfa;
		EG_mifs_tWorkInfo.aucBuffer[1] = EG_mifs_tWorkInfo.ucPwrLevel | ucCID;
		EG_mifs_tWorkInfo.aucBuffer[2] = ucTemp & 0x3F;
		ucResult = EI_mifs_ucProcess(ucType, 3, puiRecLen);
	}
	else
	{
		EG_mifs_tWorkInfo.aucBuffer[0] = 0xf2;
		EG_mifs_tWorkInfo.aucBuffer[1] = ucTemp & 0x3F;
		ucResult = EI_mifs_ucProcess(ucType, 2, puiRecLen);
	}
	if ((ucResult != EM_SUCCESS) || (*puiRecLen == 0))
	{
		return (EI_mifs_ucSendRBlock(ucType, ucCID, puiRecLen));
	}

	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EI_mifs_ucAuthentication(uchar ucKeyType,uchar ucAddr)
* ����������
*     ��Type A��������֤
* ���������
*    ucKeyType EM_mifs_KEYA A������
*              EM_mifs_KEYB B������
*    ucAddr��������Կ���ڿ��
* ���������
*     ��
* ����ֵ��
*     EM_mifs_NOTAGERR----�޿�
*	  EM_mifs_AUTHERR ----��������֤
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
*                 2005-05-18          01-01-01    �޸�
***********************************************************************/
uchar EI_mifs_ucAuthentication(uchar ucKeyType, uchar ucAddr)
{
	uchar ucTempData;
	uchar ucResult = EM_SUCCESS;
	uint time = 0;

	time = time;
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        mifareInitialize();
        mifareResetCipher();
		if (ucKeyType == EM_mifs_KEYA)
			ucTempData = EM_mifs_PICC_AUTHENT1A;
		else
			ucTempData = EM_mifs_PICC_AUTHENT1B;
		memcpy(EG_mifs_tWorkInfo.aucBuffer, &EG_mifs_tWorkInfo.ulCardID, 4);
        ucResult = mifareAuthenticate(ucTempData, ucAddr, EG_mifs_tWorkInfo.aucBuffer, 4, gtRfidProInfo.gMifareKey);
        if ( ucResult ) {
            return EM_mifs_AUTHERR;
        }
        return ucResult;
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{

#ifdef EM_RC663_Module

		s_rfid_setRC663Int(0,RC663_IRQ_ALL);
		s_rfid_setRC663IntFlg(0,RC663_IRQ_ALL);

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_STATUS, 0x20);
		// ���FIFO������
		EI_mifs_vFlushFIFO();
		// ������ǰ�������е�����
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F);

        //load key
        EI_mifs_vWriteReg(6, PHHAL_HW_RC663_REG_FIFODATA, gtRfidProInfo.gMifareKey);
		ucTempData = PHHAL_HW_RC663_CMD_LOADKEY;
        EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, ucTempData);
        s_DelayUs(1000);//δ֪ʱ�����
		// ������ǰ�������е�����
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F);
		// ���FIFO������
		EI_mifs_vFlushFIFO();

		if (ucKeyType == EM_mifs_KEYA)
			EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_AUTHENT1A;
		else
			EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_AUTHENT1B;

		EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
//		memcpy(EG_mifs_tWorkInfo.aucBuffer+2,gtPN512Info.gPN512Buf,6); //key
//		memcpy(EG_mifs_tWorkInfo.aucBuffer + 2 + 6, &EG_mifs_tWorkInfo.ulCardID, 4);
		memcpy(EG_mifs_tWorkInfo.aucBuffer + 2, &EG_mifs_tWorkInfo.ulCardID, 4);

//        TRACE("\r\nRC663 API logic card\r\n");
//        DISPBUF(EG_mifs_tWorkInfo.aucBuffer, 6, 0);

        EI_mifs_vWriteReg(6, PHHAL_HW_RC663_REG_FIFODATA, EG_mifs_tWorkInfo.aucBuffer);

		ucTempData = PHHAL_HW_RC663_CMD_MFAUTHENT;
        EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, ucTempData);

		//EI_mifs_vReadReg(1,PHHAL_HW_RC663_REG_ERROR,&ucTempData);
		//TRACE("\r\n ����Ĵ���:%x",ucTempData);

		time = sys_get_counter();
		while(1)
		{
			// ��Crypto1 �����Ƿ��Ѿ����������û��������˵����֤ʧ��
			EI_mifs_vReadReg(1,PHHAL_HW_RC663_REG_STATUS,&ucTempData);
			if(ucTempData&0x20)
				break;
			if(if_timerout(time,100))
			{
				ucResult = EM_mifs_AUTHERR;
				break;
			}
		}

//        EI_mifs_vReadReg(1,PHHAL_HW_RC663_REG_STATUS,&ucTempData);
//        TRACE("\r\n---Crypto1:%x",ucTempData);

		// ������ǰ�������е�����
        s_DelayUs(5000); //һ��Ҫ�� ��Ȼ���COMMAND�Ĵ�����Ӱ���mifare��
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F);

		return ucResult;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{

#ifdef EM_PN512_Module

		s_rfid_setRC531Int(0,PN512_IRQ_ALL);
		s_rfid_setRC531IntFlg(0,PN512_IRQ_ALL);

		// ��ֹcrypto1��Ԫ
		EI_mifs_vClrBitMask(Status2Reg, 0x08);

		//EI_mifs_vReadReg(1,Status2Reg,&ucTempData);
		//TRACE("\r\nCrypto1:%x",ucTempData);

		// ���FIFO������
		EI_mifs_vFlushFIFO();

		// ������ǰ�������е�����
		ucTempData = PN512CMD_IDLE;
		ucTempData &= 0x0F;
		EI_mifs_vClrBitMask(CommandReg, 0x0F);
		EI_mifs_vSetBitMask(CommandReg, ucTempData);

		if (ucKeyType == EM_mifs_KEYA)
		{
			EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_AUTHENT1A;
		}
		else
		{
			EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_AUTHENT1B;
		}

		EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
		memcpy(EG_mifs_tWorkInfo.aucBuffer+2,gtRfidProInfo.gMifareKey,6); //key
		memcpy(EG_mifs_tWorkInfo.aucBuffer + 2 + 6, &EG_mifs_tWorkInfo.ulCardID, 4);

		//TRACE("\r\nAPI logic card\r\n");
		//DISPBUF(EG_mifs_tWorkInfo.aucBuffer, 12, 0);

		EI_mifs_vWriteReg(12, FIFODataReg, EG_mifs_tWorkInfo.aucBuffer);

		ucTempData = PN512CMD_AUTHENT;
		ucTempData &= 0x0F;
		EI_mifs_vClrBitMask(CommandReg, 0x0F);
		EI_mifs_vSetBitMask(CommandReg, ucTempData);

		//EI_mifs_vReadReg(1,ErrorReg,&ucTempData);
		//TRACE("\r\n ����Ĵ���:%x",ucTempData);

		time = sys_get_counter();
		while(1)
		{
			// ��Crypto1 �����Ƿ��Ѿ����������û��������˵����֤ʧ��
			EI_mifs_vReadReg(1,Status2Reg,&ucTempData);
			if(ucTempData&0x08)
				break;
			if(if_timerout(time,100))
			{
				ucResult = EM_mifs_AUTHERR;
				break;
			}
		}

		s_rfid_setRC531Int(0,PN512_IRQ_ALL);
		s_rfid_setRC531IntFlg(0,PN512_IRQ_ALL);
		/*
		ucTempData = PN512CMD_IDLE;
		ucTempData &= 0x0F;
		EI_mifs_vClrBitMask(CommandReg, 0x0F);
		EI_mifs_vSetBitMask(CommandReg, ucTempData);
		*/

		return ucResult;

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module
/**********************************************************************
 ִ����֤����1
***********************************************************************/
        // RxCRC disable,TxCRC, Parity enable
        ucTempData = 0x07;
        EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

        // ���ö�ʱ��ʱ��
        EI_mifs_vSetTimer(4);

        // ׼����������: ��֤����, ���������׿��, ���ţ�4�ֽڣ�
        if (ucKeyType == EM_mifs_KEYA)
        {
            EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_AUTHENT1A;
        }
        else
        {
            EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_AUTHENT1B;
        }

        EG_mifs_tWorkInfo.aucBuffer[1] = ucAddr;
        memcpy(EG_mifs_tWorkInfo.aucBuffer + 2, &EG_mifs_tWorkInfo.ulCardID, 4);
        EG_mifs_tWorkInfo.ulSendBytes = 6;	// how many bytes to send
        EG_mifs_tWorkInfo.ucDisableDF = 0;

        // ִ������
        ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_AUTHENT1);
        if (ucResult != EM_SUCCESS)
        {
            return ucResult;
        }

        EI_mifs_vReadReg(1, EM_mifs_REG_SECONDARYSTATUS, &ucTempData);
        if ((ucTempData & 0x07) != 0)
        {
            return EM_mifs_BITCOUNTERR;
        }

/**********************************************************************
 ִ����֤����2
***********************************************************************/
        // RxCRC,TxCRC disable, Parity enable
        ucTempData = 0x03;
        EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);

        // ����Ҫ׼����������
        EG_mifs_tWorkInfo.ulSendBytes = 0;	// how many bytes to send
        EG_mifs_tWorkInfo.ucDisableDF = 0;

        // ��������
        ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_AUTHENT2);
        //�߼�����֤�������������
        //�������1:(����Կ��) ��ucResult == EM_SUCCESS ��Crypto1 ����û����������4
        //�������2:(����Կ��) ��ucResult == EM_mifs_NOTAGERR ֱ�ӷ���2
        if (ucResult != EM_SUCCESS)
        {
            return ucResult;
        }

        // ��Crypto1 �����Ƿ��Ѿ����������û��������˵����֤ʧ��
        EI_mifs_vReadReg(1, EM_mifs_REG_CONTROL, &ucTempData);
        if ((ucTempData & 0x08) != 0)
        {
            return EM_SUCCESS;
        }
        else
        {
            return EM_mifs_AUTHERR;
        }
#endif

	}

	return 0xFF;
}

/**********************************************************************
* �������ƣ�
*     EI_mifs_ucDeselect(uchar ucType, uchar ucCID)
* ����������
*     ʹ�ѽ���ͨ���Ŀ�Ƭ����HALT״̬
* ���������
*    ucType     ��Ƭ����
*     ucCID: ָ������
* ���������
*     ��
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EI_mifs_ucDeselect(uchar ucType, uchar ucCID)
{
	uint uiLen;
	uchar ucResult;

	EG_mifs_tWorkInfo.aucBuffer[0] = EM_mifs_PICC_DESELECT;
	if (EG_mifs_tWorkInfo.ucCIDSupport == 1)
	{
		EG_mifs_tWorkInfo.aucBuffer[1] = ucCID;
		ucResult = EI_mifs_ucProcess(ucType, 2, &uiLen);
	}
	else
	{
		EG_mifs_tWorkInfo.aucBuffer[0] &= 0xF7;
		ucResult = EI_mifs_ucProcess(ucType, 1, &uiLen);
	}
	if (ucResult == EM_SUCCESS)
	{
		if (uiLen == 0)
		{
			return EM_mifs_NOTAGERR;
		}
		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0xF6) != (EM_mifs_PICC_DESELECT & 0xF6))
		{
			return EM_mifs_CODEERR;
		}
		if ((EG_mifs_tWorkInfo.aucBuffer[0] & 0x08) == 0x08)
		{
			// ����CID
			if (uiLen != 2)
			{
				return EM_mifs_CODEERR;
			}
		}
		else
		{
			// ������CID
			if (uiLen != 1)
			{
				return EM_mifs_CODEERR;
			}
		}
	}
	return ucResult;
}

/**********************************************************************
* �������ƣ�
*     EI_mifs_ucProcess(uchar ucType,uint uiSendLen, uint* puiRecLen)
* ����������
*     ��ɶ�д���뿨�����ݽ���������
* ���������
*    ucType: ������
*    uiSendLen: Ҫ�������ݳ���
* ���������
*    puiRecLen: ���յ����ݳ���
* ����ֵ��
*     ������
* ��ʷ��¼��
*     �޸���        ����                �汾��      �޸ļ�¼
*                 2004-08-14          01-01-01    ����
***********************************************************************/
uchar EI_mifs_ucProcess(uchar ucType, uint uiSendLen, uint * puiRecLen)
{

#ifdef EM_RC531_Module

	uchar ucTempData;
	uchar ucResult;

	EI_mifs_vFlushFIFO();
	EG_mifs_tWorkInfo.ucEnableTransfer = 0;

	EI_mifs_vSetTimer(EG_mifs_tWorkInfo.ucFWI);

	switch (ucType)
	{
	case EM_mifs_TYPEB:
		{
			ucTempData = 0x2C;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
			break;
		}
	case EM_mifs_TYPEA:
	default:
		{
			// RxCRC,TxCRC, Parity enable
			ucTempData = 0x0F;
			EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
			break;
		}
	}

	D2(
		DATAIN(0x55);
		DATAIN(LBYTE(uiSendLen));
		BUFFIN(uiSendLen,EG_mifs_tWorkInfo.aucBuffer);
	);
	// ׼����������: ������
	EG_mifs_tWorkInfo.ulSendBytes = uiSendLen;	// how many bytes to send
	EG_mifs_tWorkInfo.ucDisableDF = 0;
	ucResult = EI_mifs_ucHandleCmd(EM_mifs_PCD_TRANSCEIVE);
	if (ucResult == EM_SUCCESS)
	{
		// ������յ�������
		*puiRecLen = EG_mifs_tWorkInfo.ulBytesReceived;
	}
	else
	{
		*puiRecLen = 0;
	}

	D2(
		DATAIN(0x66);
		DATAIN(LBYTE(*puiRecLen));
		BUFFIN(*puiRecLen,EG_mifs_tWorkInfo.aucBuffer);
	);
	// ��ʱ500us
	EI_paypass_vDelay(70);
	return ucResult;

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	return 0xFF;

#endif

}

/**********************************************************************
* �������ƣ�
*    EI_mifs_ucBeginProtect(DevHandle* phDevHandle,uchar ucMode)
* ����������
*   ��ȡ��Ƶ��ģ�鹫����Դ��spi����
* ���������
* ���������
*   ��
* ����ֵ��
*   EM_SUCCESS: �ɹ�
*   EM_mifs_ERRTYPE: �����ʹ���
*   EM_ERRHANDLE: �Ƿ����
*   EM_ERRPARAM: ��������
*   EM_FAILED: ��������ϵͳ����ȣ�
* ��ʷ��¼��
*   �޸���      ����            �汾��      �޸�����
*             2004-08-14      01-01-01    ����
**********************************************************************/
uchar EI_mifs_ucBeginProtect(DevHandle * phDevHandle, uchar ucMode)
{
//#ifdef EM_AS3911_Module
    hw_rfid_spi_init();
//#endif
	return 0;
}

/**********************************************************************
* �������ƣ�
*      EI_mifs_vEndProtect(void)
* ����������
*     �ͷ���Ƶ��ģ�鹫����Դ��spi����
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
void EI_mifs_vEndProtect(void)
{
#ifdef CFG_W25XFLASH
    hw_w25x_spi_init();
#endif
}

//************************************************************************************
//
//
//         ��������ƵAPI
//
//
//************************************************************************************

/**********************************************************************
* �������ƣ�
*
* ����������
*     //ѰTypeB�� 0-�ɹ��п� 1-�ж���B��  2-�޿�
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_change_ret(uint ret)
{
	switch(ret)
	{
		case 0:
		case EM_mifs_SWDIFF:
			return RFID_SUCCESS;
		case EM_ERRPARAM:
			return -RFID_ERRPARAM;
		case EM_mifs_TIMEOUT:
			return -RFID_TIMEOUT;
		case EM_mifs_NOCARD:
		case EM_mifs_NOTAGERR:
			return -RFID_NOCARD;
		case EM_mifs_TRANSERR:
		case EM_mifs_CODEERR:
			return -RFID_TRANSERR;
		case EM_mifs_PROTERR:
			return -RFID_PROTERR;
		case EM_mifs_MULTIERR:
			return -RFID_MULTIERR;
		case EM_mifs_NOACT:
			return -RFID_NOACT;
		case EM_mifs_CARDEXIST:
			return -RFID_CARDEXIST;
		case EM_mifs_AUTHERR:
		case EM_mifs_NOTAUTHERR:
			return -RFID_NOAUTH;
		default:
			return -RFID_ERROR;
	}
}

/**********************************************************************
* �������ƣ�
*      if_rfid_module
* ����������
*     ���ж��Ƿ�����Ƶģ��
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int if_rfid_module(void)
{
	if(sys_get_module_type(MODULE_RF_TYPE) == MODULE_NOTEXIST)
	{
		return -RFID_ERROR;
	}
	else
    {
		return RFID_SUCCESS;
    }
}
/**********************************************************************
* �������ƣ�
*      rfid_is_open
* ����������
*     ���ж��Ƿ�����Ƶģ��
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_is_open(void)
{
	if(EG_mifs_tWorkInfo.ucFirstOpenFlag)
	{
		return RFID_SUCCESS;
	}
	else
	{
		return -RFID_ERROR;
	}
}
/**********************************************************************
* �������ƣ�
*      rfid_close
* ����������
*     ���
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_open(uint mode)
{
	uchar openmode;
    int iRet;
	Assert_OK(if_rfid_module());
	if(mode == 0)
	{
		openmode = EM_mifs_TYPEA;
	}
	else if(mode == 1)
	{
		openmode = EM_mifs_TYPEB;
	}
	else
	{
		openmode = EM_mifs_NULL;
	}
    iRet = rfid_change_ret((uint)EA_ucMifsOpen(openmode));
    if (iRet == RFID_SUCCESS) {
    }
	return iRet;
}
/**********************************************************************
* �������ƣ�
*      rfid_close
* ����������
*     ���
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_close(void)
{
    int iRet;
	Assert_OK(if_rfid_module());
    iRet = rfid_change_ret((uint)EA_ucMifsClose());
    if (iRet == RFID_SUCCESS) {
    }
	return iRet;
}
/**********************************************************************
* �������ƣ�
*      rfid_activate
* ����������
*     ���
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_poll(uint mode, uint *cardtype)
{
	uchar ret;
	uchar type=0;
	Assert_OK(if_rfid_module());
	Assert_OK(rfid_is_open());
	if((cardtype == EM_NULL)
       || (mode & (~0x0F)))
	{
	    return -RFID_ERRPARAM;
	}
//    if ( mode&RFID_MODE_EMV ) {
//        //EMVģʽ תΪISOģʽ
//        mode &= (~RFID_MODE_EMV);
//    }
    EG_mifs_tWorkInfo.OptMode = mode;
//    D1(
//        TRACE("\r\n poll:");
//        if ( mode&RFID_MODE_EMV ) {
//        	TRACE("EMV");
//        } else {
//        	TRACE("ISO");
//        }
//        TRACE(" FRAME:%d CID:%d NAD:%d",GETBIT(mode,1)
//              ,GETBIT(mode,2),GETBIT(mode,3));
//    );
    if ( EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV) {
		EG_mifs_tWorkInfo.ucAnticollFlag = 0;
    } else {
		EG_mifs_tWorkInfo.ucAnticollFlag = 1;
    }
	CLRBUF(&gMifareData);
	ret = EA_ucMIFPollMifCard(0,EG_mifs_tWorkInfo.ucAnticollFlag,&type);
	if(!ret)
	{
		if(EG_mifs_tWorkInfo.ucCurType == EM_mifs_TYPEA)
		{
			type = RFID_TYPEA;
		}
		else
		{
			type = RFID_TYPEB;
		}
		*cardtype = type;
	}

	return rfid_change_ret((uint)ret);
}

/**********************************************************************
* �������ƣ�
*      rfid_activate
* ����������
*     ���
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_powerup(uint cardtype, uint *puiOutLen, uchar *pvOut)
{
	uint ret;
//    uint recvlen;
//    uchar recv[8];
	Assert_OK(if_rfid_module());
	Assert_OK(rfid_is_open());
	ret = EA_ucMIFActivate(0,cardtype,puiOutLen,pvOut);
	if(!ret)
	{
		if(EG_mifs_tWorkInfo.ucMifCardType <= RFID_CARD_S70 ||
			(EG_mifs_tWorkInfo.ucMifCardType >= RFID_CARD_PROS50
				&& EG_mifs_tWorkInfo.ucMifCardType <= RFID_CARD_PROS70))
		{
			gMifareData.pwred = 1;
			if(EG_mifs_tWorkInfo.ucMifCardType <= RFID_CARD_S70)
				gMifareData.type = EG_mifs_tWorkInfo.ucMifCardType;
			else
				gMifareData.type = EG_mifs_tWorkInfo.ucMifCardType-3;
			//TRACE("\r\n MIFARE:%d-%d",gMifareData.pwred,gMifareData.type);
		}
#if 0
        // ���ݲ��ֿ�����CID����ȴ��֧��CID�����ԣ��������ϵ�ɹ����ȡ���������
        DISPPOS(EG_mifs_tWorkInfo.ucCIDFlag);
        if(EG_mifs_tWorkInfo.ucCIDFlag == 1)
        {
            if(EG_mifs_tWorkInfo.ucCIDPollFlg == 0){
                // ������֧��CID,��Ҫ̽��
                EG_mifs_tWorkInfo.ucCIDPollFlg = 1;
                //�޸�ʱ��
                EG_mifs_tWorkInfo.ulFWT = s_rfid_FWI2FWT(4);
                ret = EA_ucMIFAPDU(0,5,"\x00\x84\x00\x00\x02",&recvlen,recv);
                DISPPOS(ret);
                EG_mifs_tWorkInfo.ucCIDPollFlg = 0;
                EG_mifs_tWorkInfo.ulFWT = s_rfid_FWI2FWT(EG_mifs_tWorkInfo.ucFWI);
                if ( ret != EM_mifs_SWDIFF && ret != 0) {
                    //̽��ʧ��,�����ϵ�
                    ret = EA_ucMIFPollMifCard(0,EG_mifs_tWorkInfo.ucAnticollFlag,&recv);
                    if ( ret==0 ) {
                        ret = EA_ucMIFActivate(0,recv[0],puiOutLen,pvOut);
                        if ( ret==0 ) {
                            //����֧��CID
                            EG_mifs_tWorkInfo.ucCIDFlag = 0;
                            return rfid_change_ret(ret);
                        } else {
                            return rfid_change_ret(ret);
                        }
                    } else {
                        return rfid_change_ret(ret);
                    }
//                    EG_mifs_tWorkInfo.ucCIDFlag = 0;
                    ret = 0;
                }else{
                    //��ȷʵ֧��CID

                }
            }
        }
#endif
	}
	return rfid_change_ret(ret);
}

/**********************************************************************
* �������ƣ�
*      rfid_exchangedata
* ����������
*      �����ݽ���
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_exchangedata(uint sendlen, uchar *psenddata, uint *precvlen, uchar *precvdata)
{
	Assert_OK(if_rfid_module());
	Assert_OK(rfid_is_open());
	return rfid_change_ret((uint)EA_ucMIFAPDU(0,sendlen,psenddata,precvlen,precvdata));
}

/**********************************************************************
* �������ƣ�
*      rfid_deactivate
* ����������
*     ����Ƭ��ΪHALT�����ȴ���Ƭ�뿪��Ӧ��
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���        ����            �汾��      �޸�����
*                 2004-08-14      01-01-01    ����
**********************************************************************/
int rfid_powerdown(void)
{
    int ret;
	Assert_OK(if_rfid_module());
	Assert_OK(rfid_is_open());
    ret = rfid_change_ret((uint)EA_ucMIFDeactivate(0));
//    if(EG_mifs_tWorkInfo.OptMode&RFID_MODE_EMV) //emv ģʽ
//        EI_paypass_vResetPICC();  //������� ��ʧ�� ��λ��Ƶ��(����qpobc level1)
    rfid_ResetField(0);
	return ret;
}


//*************************************************************************
//
//         Mifare��API
//
//
//*************************************************************************
/**********************************************************************
* �������ƣ�
*    mifs_if_block
* ����������
*    �ж���������Ƿ��д���ת���ɾ��Կ��
* ���������
*     mode:  0-����Ϊ���1�� 1-����Ϊ���1��
*     sector:������ (S50:1����4���� S70:1����4����16����)
*     block: �����ڿ��
* ���������
*      pBlk:��
* ����ֵ��
*     -1-ʧ��  >0-��sector���Ŀ���
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int mifs_if_sector(uint sector)
{
	D1(vDispBuf(sizeof(gMifareData), 0, (uchar *)&gMifareData););
	if(EG_mifs_tWorkInfo.ucMifCardType == RFID_CARD_PRO
		|| EG_mifs_tWorkInfo.ucMifCardType == RFID_CARD_B)
	{
		//�����Ͳ���M1��ֱ�ӷ��س�ʱ
		return -RFID_TIMEOUT;
	}
	if(!gMifareData.pwred)
	{
		return -RFID_NOACT;
	}
	if(gMifareData.type == EM_mifs_S50)
	{
		if (sector >= S50SECTOR_SIZE)
		{
			return -RFID_ERRPARAM;
		}
	}
	else
	{
		if (sector >= S70SECTOR_SIZE)
		{
			return -RFID_ERRPARAM;
		}
	}
	return 0;
}

int mifs_TrailerNumInSector(uchar sector,uchar *pBlkNum)
{
	int iRet;

	iRet = 0x00;
	if (sector < S70SECTOR_SIZE_PRE32)
	{
		*pBlkNum = S70BLK_SIZE_PRE32-1;
	} else if (sector < S70SECTOR_SIZE)
	{
		*pBlkNum = S70BLK_SIZE_LAST8-1;
	} else
	{
		*pBlkNum = 0x00;
		iRet = -1;
	}
	return iRet;
}
int mifs_if_block(uint mode, uchar sector, uint block, uint *blk)
{
	uint i;
	uint blkpersec;
	uchar blkNo;
	if(!mode)
	{
		if(gMifareData.pwred == 0)
		{
			return -RFID_NOACT;
		}
		if(gMifareData.pwred == 1)
		{
			return -RFID_NOAUTH;
		}
	}
	mifs_TrailerNumInSector(sector,&blkNo);
	if (block > blkNo)
	{
		return -RFID_ERRPARAM;
	}
	if(gMifareData.type == EM_mifs_S50)
	{
		blkpersec = S50BLK_SIZE;
		i = sector*blkpersec;
	}
	else
	{
		if (sector < S70SECTOR_SIZE_PRE32)
		{
			blkpersec = S70BLK_SIZE_PRE32;
			i = sector*blkpersec;
		}
		else
		{
			blkpersec = S70BLK_SIZE_LAST8;
			i = S70SECTOR_SIZE_PRE32*S70BLK_SIZE_PRE32
				+ (sector - S70SECTOR_SIZE_PRE32)*blkpersec;
		}
	}
	*blk = i+block;
	TRACE("\r\n [Sec:%d block:%d]",sector,*blk);
	return 0;
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      Sec��block:Mifare�����Ϳ� (S50:1����4���� S70:1����4����16����)
*      keytype:0-A 1-B
* ���������
*     pData:��Կ
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_MIFAuth(uint Sec, uint keytype, uchar *pData)
{
	uchar ucRet,ucBlkNumOfSec;
	uchar ucReSendNo,result;
	uchar ucResp[2];
	uint blk;
	uint len;
	uchar buf[64];
	DevHandle hDevHandle;
    if ( pData == NULL) {
        return -RFID_ERRPARAM;
    }
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_sector(Sec));
	mifs_TrailerNumInSector(Sec,&ucBlkNumOfSec);
	Assert_OK(mifs_if_block(1,Sec,ucBlkNumOfSec,&blk));

	result = EI_mifs_ucBeginProtect(&hDevHandle, 3);
	if (result != EM_mifs_SUCCESS)
	{
		return result;
	}
	EI_paypass_vResetPICC();
	EI_paypass_vSelectType(EM_mifs_TYPEA);
	s_DelayMs(6);
	for (ucReSendNo = 0; ucReSendNo < 3; ucReSendNo++)
	{
		// �Ƚ���WUPA����
		result = EI_paypass_ucWUPA(ucResp); //�߼����ٴ�Ѱ��
		if (result != EM_mifs_TIMEOUT)
		{
			break;
		}
	}

	// ��������������ط���ֱ�ӷ��ظ��ն�
	if (result != EM_mifs_SUCCESS)
	{
		return rfid_change_ret((uint)result);
	}

	result = EI_paypass_ucGetUID();

	if(result != EM_mifs_SUCCESS)
	{
		return rfid_change_ret((uint)result);  //�߼����ٴ��ϵ�
	}

	ucRet = EA_ucMIFAuthToRamExtend(0,(uchar)blk,(uchar)keytype,pData);
	if (ucRet == EM_SUCCESS)
	{
		gMifareData.pwred = 2;
		gMifareData.sector = Sec;
		gMifareData.key = keytype+1;
		memcpy(gMifareData.keybuf,pData,6);
	}
	else
	{
		if(EM_mifs_NOTAGERR == ucRet)
		{
			ucRet = EA_ucMIFActivate(0,gMifareData.type,&len,buf);
			if(ucRet == EM_mifs_TIMEOUT)
			{
				//����޿�
				ucRet = EM_mifs_NOTAGERR;
			}
			else
			{
				//��֤ʧ��
				return -RFID_AUTHERR;
			}
		}
	}
	return rfid_change_ret((uint)ucRet);
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      pMem:Mifare�����Ϳ� (S50:1����4���� S70:1����4����16����)
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_MIFRead(uint block, uchar *out)
{
	uint blk;
    if ( out == NULL) {
        return -RFID_ERRPARAM;
    }
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	return rfid_change_ret((uint)EA_ucMIFRead(0,(uint)blk,out));
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      pMem:Mifare�����Ϳ� (S50:1����4���� S70:1����4����16����)
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_MIFWrite(uint block, uchar *in)
{
	uint blk;
    if ( in == NULL) {
        return -RFID_ERRPARAM;
    }
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	return rfid_change_ret((uint)EA_ucMIFWrite(0,(uint)blk,in));
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      pMem:Mifare�����Ϳ� (S50:1����4���� S70:1����4����16����)
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void rfid_MIFMakevalue(long value,uchar addr, uchar *out)
{
	long data1;
	uchar pucBuf[16];
    if ( out == NULL) {
        return;
    }
	data1 = ~value; // ׼��������ֵΪ125
	pucBuf [12] = pucBuf [14] = addr;
	pucBuf [13] = pucBuf [15] = ~addr;
	memcpy((char *)&pucBuf[0],(char *)&value,4);
	memcpy((char *)&pucBuf[4],(char *)&data1,4);
	memcpy((char *)&pucBuf[8],(char *)&value,4);
	memcpy(out,(char *)pucBuf,sizeof(pucBuf));
}


int rfid_MIFIncrement(uint block, ulong value)
{
	uint blk;
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	return rfid_change_ret((uint)EA_ucMIFIncrement(0, (uint)blk, value));
}

int rfid_MIFDecrement(uint block, ulong value)
{
	uint blk;
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	return rfid_change_ret((uint)EA_ucMIFDecrement(0, (uint)blk, value));
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      pMem:Mifare�����Ϳ� (S50:1����4���� S70:1����4����16����)
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_MIFRestore(uint block)
{
	uint blk;
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	return rfid_change_ret((uint)EA_ucMIFRestore(0, (uint)blk));
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      pMem:Mifare�����Ϳ� (S50:1����4���� S70:1����4����16����)
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_MIFTransfer(uint block)
{
	uint blk;
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	return rfid_change_ret((uint)EA_ucMIFTransfer(0, (uint)blk));
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
uchar rfid_MIFParseStatus(const ACCESSBIT *pAccess, uchar ucBlkNum)
{
	uchar ucRet;

	if (ucBlkNum > 0x3)
	{
		return 0xFF;
	}
	ucRet = 0;
	ucBlkNum = (0x01<<ucBlkNum);
	if (pAccess->c1&ucBlkNum)
	{
		ucRet |= 0x04;
	}
	if (pAccess->c2&ucBlkNum)
	{
		ucRet |= 0x02;
	}
	if (pAccess->c3&ucBlkNum)
	{
		ucRet |= 0x01;
	}

	return ucRet;
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      access:��β��������Ľ�������
*	  authKey:��֤������Կ����  1-KeyA  2-KeyB
* ���������
*
* ����ֵ��
*     0x00-�ɹ�  0x10-�����޸�  0x20-��֤��Կ��Ȩ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
uchar rfid_MIFAccessIfAlter(uchar access,uchar authKey)
{
	uchar ucRet;

	ucRet = 0x00;
	switch (access)
	{
		case 0x01:
			if (authKey != 1)//Not KeyA
			{
				ucRet = 0x20;
			}
			break;
		case 0x03:
		case 0x05:
			if (authKey != 2)//Not KeyB
			{
				ucRet = 0x20;
			}
			break;
		default:
			ucRet = 0x10;
			break;
	}
	return ucRet;
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      access:��β��������Ľ�������
*	  authKey:��֤������Կ����  1-KeyA  2-KeyB
* ���������
*
* ����ֵ��
*     0x00-�ɹ�  0x10-�����޸�  0x20-��֤��Կ��Ȩ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
uchar rfid_MIFKeyIfAlter(uchar access,uchar authKey)
{
	uchar ucRet;

	ucRet = 0x00;
	switch (access)
	{
		case 0x00:
		case 0x01:
			if (authKey != 1)//Not KeyA
			{
				ucRet = 0x20;
			}
			break;
		case 0x03:
		case 0x04:
			if (authKey != 2)//Not KeyB
			{
				ucRet = 0x20;
			}
			break;
		default:
			ucRet = 0x10;
			break;
	}
	return ucRet;
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*      ucIndata:(Fig 9. access condition)( tab3 and tab4)
*	   ucBlkNum:���  һ������Ŀ��
*	   pAccess:ָ�����һ������
* ���������
*     pAccess:�޸ĺ������
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void rfid_MIFMakeStatus(uchar ucIndata,uchar ucBlkNum, ACCESSBIT *pAccess)
{
	uchar bit;

	ucBlkNum = 0x01<<ucBlkNum;
	for (bit=0x01; bit<=0x40; bit<<=1)
	{
		switch (bit)
		{
			case 1:
				if (ucIndata&bit)
				{
					pAccess->c3 |= ucBlkNum;
				} else
				{
					pAccess->c3 &= ~ucBlkNum;
				}
				break;
			case 0x02:
				if (ucIndata&bit)
				{
					pAccess->c2 |= ucBlkNum;
				} else
				{
					pAccess->c2 &= ~ucBlkNum;
				}
				break;
			case 0x04:
				if (ucIndata&bit)
				{
					pAccess->c1 |= ucBlkNum;
				} else
				{
					pAccess->c1 &= ~ucBlkNum;
				}
				break;

			default:
				break;
		}
	}
	pAccess->versac1 = ~pAccess->c1;
	pAccess->versac2 = ~pAccess->c2;
	pAccess->versac3 = ~pAccess->c3;
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int rfid_MIFModify(uint block, const BLKALTER *alterVal)
{
	uint blk;
	TRAILER trailer;	//���ڱ����ȡ��β������
	uchar i,ucTmp;
	uchar ucTrailNo,ucTrailBlk;
	uchar ucRet;

	TRACE("\r\nModify-Sect:%d, Blk:%d",gMifareData.sector,block);
	TRACE(" mode:%02x access:%02x",alterVal->mode,alterVal->access);
	Assert_OK(rfid_is_open());
	Assert_OK(mifs_if_block(0,gMifareData.sector,block,&blk));
	if ((alterVal->mode&0xF8) != 0x00
		|| alterVal->access > 0x07)
	{
		return -RFID_ERRPARAM;
	}
	memset(&trailer,0x00,sizeof(trailer));
	mifs_TrailerNumInSector(gMifareData.sector,&ucTrailNo);
	//��������β������
	ucTrailBlk = (uchar)(blk - block);
	ucTrailBlk += ucTrailNo;
	TRACE("\r\nTrailNO:%d",ucTrailBlk);
	ucRet = EA_ucMIFRead(0,ucTrailBlk,(uchar*)&trailer);
	D1(vDispBuf(16,0,(uchar *)&trailer););
	if (ucRet != EM_SUCCESS)
	{
		return rfid_change_ret((uint)ucRet);
	}
	ucTmp = ucTrailNo;
	if (gMifareData.type == EM_mifs_S70
		&& ucTrailNo >= S70BLK_SIZE_PRE32)
	{
			ucTmp = ucTrailNo/5;
	}
	//�鿴�Ƿ����޸�Ȩ��blk=3
//		TRACE("\r\n acces:");
	D1(vDispBuf(sizeof(trailer.access),0,(uchar *)&trailer.access););
	ucRet = rfid_MIFParseStatus(&trailer.access,ucTmp);
	TRACE("\r\nPase:%02x,ucTmp:%02x",ucRet,ucTmp);
	if (ucRet > 0x02)	//��B��Կ���ɶ�ʱ�����޸���Կ����ͬʱ�޸�A/B
	{
		if (ucRet > 0x07)
		{
			return -RFID_PROTERR;
		}
		if ((alterVal->mode&0x06) == 0x02
			|| (alterVal->mode&0x06) == 0x04)
		{
			return -RFID_ERRPARAM;
		}
	}
	if (gMifareData.key == 1)	//KeyA
	{
		memcpy(&trailer.keyA,gMifareData.keybuf,6);
	} else if (gMifareData.key == 2)	//KeyB
	{
		memcpy(&trailer.keyB,gMifareData.keybuf,6);
	} else
	{
		return -RFID_NOAUTH;
	}
	ucTmp = 0x00;
	for (i=1; i<=0x04; i<<=1)
	{
		switch (alterVal->mode & i)
		{
			case 0x01:	//access
				TRACE(" Access%02x",i);
				ucTmp = rfid_MIFAccessIfAlter(ucRet,gMifareData.key);
				rfid_MIFMakeStatus(alterVal->access,(uchar)block,&trailer.access);
				break;
			case 0x02:	//keyA
				TRACE(" keyA%02x",i);
				if (block != ucTrailNo)
				{
					TRACE("\r\nERR-A");
					return -RFID_ERRPARAM;
				}
				ucTmp = rfid_MIFKeyIfAlter(ucRet,gMifareData.key);
				memcpy(&trailer.keyA,alterVal->keyA,6);
				break;
			case 0x04:	//keyB
				TRACE(" keyB%02x",i);
				if (block != ucTrailNo)
				{
					TRACE("\r\nERR-B");
					return -RFID_ERRPARAM;
				}
				ucTmp = rfid_MIFKeyIfAlter(ucRet,gMifareData.key);
				memcpy(&trailer.keyB,alterVal->keyB,6);
				break;
			default:
				TRACE(" None%02x",i);
				break;
		}
		if (ucTmp == 0x10)
		{
			return -RFID_UNCHANGE;
		} else if (ucTmp == 0x20)
		{
			return -RFID_KEYNOTPOWER;
		}
	}
	D1(vDispBuf(16,0,(uchar *)&trailer););
	if (alterVal->mode == 0)
	{
		ucRet = EM_SUCCESS;
	} else
	{
		ucRet = EA_ucMIFWrite(0,ucTrailBlk,(uchar*)&trailer);
	}
	if (ucRet == EM_SUCCESS)
	{
		if (alterVal->mode == 0x02)
		{
			memcpy(gMifareData.keybuf,&trailer.keyA,6);
		} else if (alterVal->mode == 0x04)
		{
			memcpy(gMifareData.keybuf,&trailer.keyB,6);
		}
	}
	return rfid_change_ret((uint)ucRet);
}

//������Ƶ���� para-��������  step-���ڲ��� 1-poll 2-active 3-random/MIFAuth
int rfid_autoParaAdjust(int para, int step)
{
	int ret = OK;
	int i,j,paraback,type;
	uchar buf[64];

	ret = if_rfid_module();
	if(ret)
	{
		return ret;
	}
	ret = rfid_open(0);
	if(ret)
	{
		return ret;
	}

	if(para == RFID_PARA_PWRSETTIME)
	{
		lcd_cls();
		lcdDispMultiLang(0, DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ǰ��ǿʱ��: %d", "Cur Pwr Time: %d",EG_mifs_tWorkInfo.PwrAdTime);
		lcdDispMultiLang(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "�Զ�������...", "Auto Adjust...");
		paraback = EG_mifs_tWorkInfo.PwrAdTime;
		for(i = 0; i < (RFID_DEFAULT_PWRSETTIME+1); i++)
		{
			EG_mifs_tWorkInfo.PwrAdTime = RFID_DEFAULT_PWRSETTIME+i;
			ret = rfid_poll(RFID_MODE_ISO,(uint *)&type);
			if(!ret)
			{
				ret = rfid_powerup((uint)type,(uint *)&j,buf);
				if(!ret)
				{
					type = buf[j-1];
					if(buf[j-1] >= EM_mifs_PROCARD)
                    {
                    	ret = rfid_exchangedata(sizeof(gApduGet4rand), (uchar *)gApduGet4rand,(uint *)&j,buf);
                		//ret = rfid_exchangedata(sizeof(gApduSelMaindir), gApduSelMaindir,&j,buf);
	                    if(ret == EM_mifs_SUCCESS || ret == EM_mifs_SWDIFF)
	                    {
	                        lcdDispMultiLang(0,  DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE,"Ѱ��:%d ����[%02X-%02X]","Type:%d Rd:[%02X-%02X]",type,buf[j-2],buf[j-1]);
							lcdDispMultiLang(0,  DISP_FONT_LINE6, DISP_FONT|DISP_CLRLINE,"�����ɹ� :%dms","Adjust Succeed :%dms",EG_mifs_tWorkInfo.PwrAdTime);
	                        sys_beep_pro(DEFAULT_SOUNDFREQUENCY,30,YES);
							ret = WriteRfid_Para_Set(RFID_PARA_PWRSETTIME, EG_mifs_tWorkInfo.PwrAdTime);
							kb_getkey(3000);
							return ret;
	                    }
                    }
                    else
                    {
                    	//fac_MIFtest();
                    	lcdDispMultiLang(0,  DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE,"Ѱ��:%d Mifare��","Type:%d Mifare card",type);
						lcdDispMultiLang(0,  DISP_FONT_LINE6, DISP_FONT|DISP_CLRLINE,"�����ɹ� :%dms","Adjust Succeed :%dms",EG_mifs_tWorkInfo.PwrAdTime);
                        sys_beep_pro(DEFAULT_SOUNDFREQUENCY,30,YES);
						ret = WriteRfid_Para_Set(RFID_PARA_PWRSETTIME, EG_mifs_tWorkInfo.PwrAdTime);
						kb_getkey(3000);
						return ret;
                    }
				}
			}
		}
		EG_mifs_tWorkInfo.PwrAdTime = paraback;
		lcdDispMultiLang(0,  DISP_FONT_LINE6, DISP_FONT|DISP_CLRLINE,"����ʧ��: %dms","Adjust Failure: %dms",EG_mifs_tWorkInfo.PwrAdTime);
		ret = ERROR;
		kb_getkey(3000);
	}
	else
	{
		ret = ERROR;
	}

	return ret;
}

//��λ��Ƶ��
int rfid_ResetField(int para)
{
	EI_mifs_ucBeginProtect(0, 3);
    EI_paypass_vResetPICC();
    EI_mifs_vEndProtect();
    return 0;
}

//������Ƶ���� para-��������  value -��������
int rfid_ParaAdjust(int para, int value)
{
	int ret = OK;
//	int i,j,paraback,type;
    s_rfidpara_info rfdata;
//	uchar buf[64];

	ret = if_rfid_module();
	if(ret)
	{
		return ret;
	}

	if(para == RFID_PARA_PWRSETTIME){
        if ( (value >= RFID_DEFAULT_PWRSETTIME) && (value <= RFID_MAX_PWRSETTIME)) {
            EG_mifs_tWorkInfo.PwrAdTime = value;
            s_DelayMs(20);
            ret = RFID_SUCCESS;
        }else
            ret = -RFID_ERRPARAM;
	}
	else if ( para == RFID_PARA_PWR) {
        if ( (value >= RFID_PWR_LEVEL0) && (value <= RFID_MAX_PWR)) {
            s_rfid_getPara(EG_mifs_tWorkInfo.RFIDModule, RFID_PARA_PWR, value, &rfdata);
            gtRfidDebugInfo.CW_A = (uchar)rfdata.pwrfield;
            gtRfidDebugInfo.CW_B = gtRfidDebugInfo.CW_A;
            if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module  //AS3911
                gas3911Reg.gTypeBmodule = (uchar)rfdata.regpara;
                as3911WriteRegister(AS3911_REG_RFO_AM_OFF_LEVEL, gtRfidDebugInfo.CW_A); //����Ӳ������0x9F
                as3911WriteRegister(AS3911_REG_RFO_AM_ON_LEVEL, gas3911Reg.gTypeBmodule);
                if ( s_rfid_mainVersion(0) == 0 ) {
                    //wp70v1.00���Զ�����type b���
                    as3911ModifyRegister(AS3911_REG_AM_MOD_DEPTH_CONTROL, 0x7E, 0x1E);
                    as3911ClearInterrupts(AS3911_IRQ_MASK_DCT);
                    as3911EnableInterrupts(AS3911_IRQ_MASK_DCT);
                    as3911ExecuteCommand(AS3911_CMD_CALIBRATE_MODULATION);
                    as3911WaitForInterruptTimed(AS3911_IRQ_MASK_DCT, 200, (ulong *)&ret);
                    if ( ret != AS3911_IRQ_MASK_DCT) {
                        return -RFID_ERROR;
                    }
                }
#endif
            }
            if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512 ) {
#ifdef EM_PN512_Module  //PN512
//                gcMifReg.GsNOn = (uchar)rfdata.pwrfield;
//                gcMifReg.CWGsP = (uchar)rfidpara_FM17550_V1EX[value].pwrfield;
                gcMifReg.CWGsP = (uchar)rfdata.pwrfield;
//                EI_mifs_vWriteReg(1, RFCfgReg, &gcMifReg.RFCfg);
#endif
            }
            ret = RFID_SUCCESS;
        }else
            ret = -RFID_ERRPARAM;
    }else if ( para == RFID_PARA_TYPEBMODULE) {
#ifdef EM_AS3911_Module  //AS3911
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
            as3911WriteRegister(AS3911_REG_RFO_AM_ON_LEVEL, value);
            gas3911Reg.gTypeBmodule = value;
        }
#endif
    }else{
		ret = -RFID_ERRPARAM;
	}

	return ret;
}

int adjust_rfid_PwrFieldSetTime(int mode)
{
	int key,iRet = 0,param,tmp = 0;
	int PwrAdTimeback = 0;

    iRet = iRet;PwrAdTimeback = PwrAdTimeback;
	PwrAdTimeback = EG_mifs_tWorkInfo.PwrAdTime;
	while(1)
	{
		lcd_cls();
		lcdDispMultiLang(0,  DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY, "��ǿ����ʱ��", "PwrField Time");
		lcdDispMultiLang(0,  DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE, "��ǰʱ��:%d", "Cur Time:%d",EG_mifs_tWorkInfo.PwrAdTime);
		lcdDispMultiLang(0,  DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "1-�Զ�����",  "1-Auto Adjust");
		lcdDispMultiLang(0,  DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE, "2-�ֶ�����",  "2-Manual Adjust");
		lcdDispMultiLang(0,  DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE, "3-�ָ�Ĭ��",  "3-Default");

		key = kb_getkey(-1);
		switch(key)
		{
    		case KEY1:
//    			iRet = rfid_ParaAdjust(RFID_PARA_PWRSETTIME, 4);
    			break;
    		case KEY2:
				lcd_cls();
				param = EG_mifs_tWorkInfo.PwrAdTime;
				while(1)
				{
                    lcdDispMultiLang(0, DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY,
                                     "��ǿʱ������", "PwrField Set up");
                    lcdDispMultiLang(0,  DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE,
                                     "��ǰֵ: %d", "Cur Level: %d", param);
                    lcdDispMultiLang(0,  DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
                                     "[\x1E/\x1F]: ����", "[\x1E/\x1F]: Adjust");
                    lcdDispMultiLang(0,  DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
                                     "[ȷ��]: ����", "[Enter]: Save");
					key = kb_getkey(-1);
			        switch(key)
			        {
			        case KEY_ENTER:
						iRet = WriteRfid_Para_Set(RFID_PARA_PWRSETTIME, param);
                        iRet = rfid_ParaAdjust(RFID_PARA_PWRSETTIME, param);
						tmp = 1;
						break;

			        case KEY_SYMBOL:
			        case KEY_UP:
						if(param<RFID_MAX_PWRSETTIME)
							param++;
			            break;

			        case KEY_ALPHA:
			        case KEY_DOWN:
						if(param>RFID_DEFAULT_PWRSETTIME)
							param--;
			            break;

					case KEY_CANCEL:
						tmp = 1;
						iRet = 1;
						break;
			        default:
			            break;
			        }

					if(tmp)
					{
						tmp = 0;
						break;
					}
				}
        		break;
            case  KEY3:
                iRet = rfid_ParaAdjust(RFID_PARA_PWRSETTIME, RFID_DEFAULT_PWRSETTIME);
    			iRet = WriteRfid_Para_Set(RFID_PARA_PWRSETTIME, EG_mifs_tWorkInfo.PwrAdTime);
    			break;
			case KEY_CANCEL:
				return 0;
    		default:
    			break;
		}
    }
//    return 0;
}
//������Ƶ����ǿ
int adjust_rfid_PwrField(int mode)
{
	int key,iRet = 0,param,tmp = 0;
	int PwrAdback = 0;
    s_rfidpara_info rfdata;

    iRet = iRet;
	iRet = ReadRfid_Para_Get(RFID_PARA_PWR, &param);
	PwrAdback = param;
	while(1)
	{
		lcd_cls();
		lcdDispMultiLang(0, DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY, "��ǿ����", "Field Level Set");
		lcdDispMultiLang(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE, "��ǰ��ǿ�ȼ�:%d(0x%02X)", "Field Level:%d(0x%02X)",PwrAdback,gtRfidDebugInfo.CW_A);
		lcdDispMultiLang(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "1-�ֶ�����",  "1-Adjust");
		lcdDispMultiLang(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE, "2-�ָ�Ĭ��",  "2-Default");
		key = kb_getkey(-1);
		switch(key)
		{
    		case KEY1:
				lcd_cls();
				while(1)
				{
                    s_rfid_getPara(EG_mifs_tWorkInfo.RFIDModule, RFID_PARA_PWR, param, &rfdata);
                    lcdDispMultiLang(0, DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY,
                                     "��ǿ�ȼ�����", "Field Level Set");
                    lcdDispMultiLang(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE,
                                     "��ǰ��ǿ�ȼ�:%d(0x%02X)", "Field Level:%d(0x%02X)", param,rfdata.pwrfield);
                    lcdDispMultiLang(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
                                     "[\x1E/\x1F]: ����", "[\x1E/\x1F]: Adjust");
                    lcdDispMultiLang(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
                                     "[ȷ��]: ����", "[Enter]: Save");
					key = kb_getkey(-1);
			        switch(key)
			        {
			        case KEY_ENTER:
                        rfid_ParaAdjust(RFID_PARA_PWR, param);
						iRet = WriteRfid_Para_Set(RFID_PARA_PWR, param);
						tmp = 1;
                        PwrAdback = param;
						break;

			        case KEY_SYMBOL:
			        case KEY_UP:
						if(param<RFID_MAX_PWR)
							param++;
			            break;

			        case KEY_ALPHA:
			        case KEY_DOWN:
						if(param>RFID_PWR_LEVEL0)
							param--;
			            break;

					case KEY_CANCEL:
						tmp = 1;
						iRet = 1;
						break;
			        default:
			            break;
			        }
					if(tmp)
					{
						tmp = 0;
						break;
					}
				}
        		break;
            case  KEY2:
                param = RFID_DEFAULT_PWR;
                PwrAdback = param;
                iRet = rfid_ParaAdjust(RFID_PARA_PWR, param);
    			iRet = WriteRfid_Para_Set(RFID_PARA_PWR, param);
    			break;
			case KEY_CANCEL:
				return 0;
    		default:
    			break;
		}
    }
//    return 0;
}

//�����������
int adjust_rfid_TypeBModule(int mode)
{
#ifdef EM_AS3911_Module  //AS3911
	int key,param;
    char databuf[16];

    if ( EG_mifs_tWorkInfo.RFIDModule != RFID_Module_AS3911 ) {
        //����оƬ��ʱ��֧������
        return 0;
    }
	ReadRfid_Para_Get(RFID_PARA_TYPEBMODULE, &param);
	while(1)
	{
		lcd_cls();
		lcdDispMultiLang(0, DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY, "��������", "Module Level Set");
		lcdDispMultiLang(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE, "��ǰ����ֵ:0x%02X %02X", "Module Level:0x%02X %02X",gas3911Reg.gTypeBmodule,param);
		lcdDispMultiLang(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "1-�ֶ�����",  "1-Adjust");
		lcdDispMultiLang(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE, "2-�ָ�Ĭ��",  "2-Default");
		key = kb_getkey(-1);
		switch(key)
		{
    		case KEY1:
				lcd_cls();
                lcdDispMultiLang(0, DISP_FONT_LINE0, DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY,
                                 "��������", "Module Level Set");
                lcdDispMultiLang(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE,
                                 "��ǰ����ֵ:0x%02X", "Module Level:0x%02X", param);
                //                    lcdDispMultiLang(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
                //                                     "[\x1E/\x1F]: ����", "[\x1E/\x1F]: Adjust");
                lcdDispMultiLang(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
                                 "����ֵȷ�ϱ���", "Set and Save");
                //					key = kb_getkey(-1);
                memset(databuf, 0, sizeof(databuf));
                lcd_goto(0,DISP_FONT_LINE5);
                key = kb_getstr(KB_SMALL_NUM, 1, 3, -1, (char *)databuf);
                TRACE("\r\n---key:%x %d",key,key);
                if ( key == -KB_CANCEL ) {
                    break;
                }
                param = (int)atoi((char const *)databuf);
                rfid_ParaAdjust(RFID_PARA_TYPEBMODULE, param);
                WriteRfid_Para_Set(RFID_PARA_TYPEBMODULE, param);
        		break;
            case  KEY2:
                param = AS3911_TYPEB_MODULATION;
                rfid_ParaAdjust(RFID_PARA_TYPEBMODULE, param);
    			WriteRfid_Para_Set(RFID_PARA_TYPEBMODULE, param);
    			break;
			case KEY_CANCEL:
				return 0;
    		default:
    			break;
		}
    }
#else
    return 0;
#endif
}
//int rfid_type_menu(int mode)
//{
//    const char *name[] = {"RC531","FM17550","RC663","AS3911","δ֪"};
//    uint i;
//    i = EG_mifs_tWorkInfo.RFIDModule;
//    if ( i == 0 || i >= DIM(name) ) {
//        i = DIM(name)-1;
//    } else {
//        --i;
//    }
//    lcd_cls();
//    lcdDispMultiLang(0, DISP_FONT_LINE0,DISP_FONT|DISP_CLRLINE|DISP_INVLINE|DISP_MEDIACY, "оƬ����", "CHIP",name[i]);
//    lcd_display(0, DISP_FONT_LINE1,DISP_FONT|DISP_CLRLINE, "%s", name[i]);
//    kb_getkey(-1);
//    return 0;
//}
int rfid_para2offset(int type, int *offset, int *len)
{
    int i;
    switch ( type )
    {
    case RFID_PARA_PWRSETTIME :
        i = FPOS(MODULE_RFID_INFO,rfid_powerfield_time);
        *len = FSIZE(MODULE_RFID_INFO,rfid_powerfield_time);
        break;
    case RFID_PARA_PWR :
        i = FPOS(MODULE_RFID_INFO,field_strength);
        *len = FSIZE(MODULE_RFID_INFO,field_strength);
        break;
    case RFID_PARA_CHIP :
        i = FPOS(MODULE_RFID_INFO,type);
        *len = FSIZE(MODULE_RFID_INFO,type);
        break;
    case RFID_PARA_TYPEBMODULE :
        i = FPOS(MODULE_RFID_INFO,gTypeBmodule);
        *len = FSIZE(MODULE_RFID_INFO,gTypeBmodule);
        break;
    default :
        return -1;
        break;
    }
    *offset = i+FPOS(SYSZONE_DEF,trfidinfo);
    return 0;
}
int WriteRfid_Para_Set(int paramIndex, int param)
{
    int ret;
    int offset,len;
    ret = rfid_para2offset(paramIndex,&offset,&len);
    if ( ret ) {
        return ret;
    }
    return s_sysconfig_write(offset,len,&param);
}

int ReadRfid_Para_Get(int paramIndex, int *param)
{
    int ret;
    int offset,len;
    ret = rfid_para2offset(paramIndex,&offset,&len);
    if ( ret ) {
        return ret;
    }
    ret = s_sysconfig_read(offset,len,param);
//    if ( !ret ) {
//        if ( *param > RFID_PWR_LEVEL3) {
//            *param = RFID_DEFAULT_PWR;
//        }
//    }
    return ret;
}

#endif


