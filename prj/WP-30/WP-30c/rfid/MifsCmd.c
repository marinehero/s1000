/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ� 
*   MifsCmd.c
* ��ǰ�汾�� 
*   01-01-01
* ����ժҪ�� 
*   ���ļ�����ʵ�ַǽӴ��������ĳ���
* ��ʷ��¼��
*   �޸���      ����                �汾��      �޸ļ�¼
******************************************************************/
#include "wp30_ctrl.h"

#ifdef CFG_RFID
/**********************************************************************

                          ���س�������
                          
***********************************************************************/
#define EM_mifs_DEFAULT_WATER_LEVEL     (uchar)0x20	// ȱʡ��֪ͨ��ֵ

/**********************************************************************

                          ȫ�ֺ�������
                          
***********************************************************************/
ET_mifs_WORKSTRUCT EG_mifs_tWorkInfo UPAREA;	// ���������ṹ

//_t_RfidPn512_Info_ gtPN512Info INIRAM;

_t_RfidPro_Info_ gtRfidProInfo UPAREA;

MIFARECARD gMifareData UPAREA;
/**********************************************************************
* �������ƣ� 
*   uchar EI_mifs_ucHandleCmd(uchar cmd)
* ���������� 
*   ������������Ӧ����
* ��������� 
*   cmd: �������  
* ���������
*   ��
* ����ֵ�� 
*   ����ִ�еĽ��
*   EM_SUCCESS                  �ɹ�
*   EM_mifs_UNKNOWN_COMMAND     δ֪����
*   EM_mifs_COLLERR             ��ͻ
*   EM_mifs_PARITYERR           ��żУ���
*   EM_mifs_FRAMINGERR          ֡����
*   EM_mifs_OVFLERR             FIFO���
*   EM_mifs_CRCERR              CRCУ���
*   EM_mifs_NOTAGERR            ������Χ���޿�
*   EM_ERROR                    �����ִ�н��
*   EM_TIMEOUT                  ����ʱ
*   EM_mifs_RECBUF_OVERFLOW     ���ջ��������
* ��ʷ��¼��
*   �޸���      ����                �汾��      �޸ļ�¼
***********************************************************************/
uint if_timerout(uint timer0,uint timerout)
{
	uint timer1=sys_get_counter();
	if(timer1 >= timer0)
	{
		if(timer1 - timer0 >= timerout)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		timer0 = UINT_MAX - timer0;
		if(timer0+timer1 >= timerout)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}
#define  OptRFID_TIMEOUT  6000
uint get_timeout(ulong FWT)
{
	// FWT*128/13.56 us = FWT*128/(13.56*10^3) ms = FWT*128/13560 = FWT*16/1695 ms
	uint timeout = (uint)FWT*16/1695;
	D2(TRACE("\r\n FWT:%d-%d timeout:%d",EG_mifs_tWorkInfo.ucFWI,EG_mifs_tWorkInfo.ulFWT,timeout););
	if(timeout < OptRFID_TIMEOUT)
	{
		timeout = OptRFID_TIMEOUT;
	}	
	return (timeout+1);
}
/**********************************************************************
* �������ƣ�
*   uchar EI_mifs_ucHandleCmd(uchar cmd)
* ����������
*   ������������Ӧ����
* ���������
*   cmd: �������
* ���������
*   ��
* ����ֵ��
*   ����ִ�еĽ��
*   EM_SUCCESS                  �ɹ�
*   EM_mifs_UNKNOWN_COMMAND     δ֪����
*   EM_mifs_COLLERR             ��ͻ
*   EM_mifs_PARITYERR           ��żУ���
*   EM_mifs_FRAMINGERR          ֡����
*   EM_mifs_OVFLERR             FIFO���
*   EM_mifs_CRCERR              CRCУ���
*   EM_mifs_NOTAGERR            ������Χ���޿�
*   EM_ERROR                    �����ִ�н��
*   EM_TIMEOUT                  ����ʱ
*   EM_mifs_RECBUF_OVERFLOW     ���ջ��������
* ��ʷ��¼��
*   �޸���      ����                �汾��      �޸ļ�¼
***********************************************************************/

uchar EI_mifs_ucHandleCmd(uchar cmd)
{
	uchar ucTempData;					// �ݴ�Ҫ�����ļĴ�������
	uchar ucWaterLevelBak;				// ����FIFO��WaterLevelֵ
	uchar ucTimerCtl;					// �������õ��Ķ�ʱ�����Ʒ�ʽ
	uchar ucIntWait;					// �������ʱ�ڴ��������ж�
    uchar ucIntWait1 = 0;               // CLRC663��ʱ����ز���
	uchar ucIntEnable;					// ���������������ж�
	uchar ucrxMultiple;					// �Ƿ�Ҫ���ն��֡
	uint timer0,timeout;
	D1( LABLE(0xCC);
		DATAIN(cmd);
		DATAIN(LBYTE(EG_mifs_tWorkInfo.ulSendBytes));
		for (guiDebugj = 0; guiDebugj < EG_mifs_tWorkInfo.ulSendBytes; guiDebugj++)
		{
			DATAIN(EG_mifs_tWorkInfo.aucBuffer[guiDebugj]);
		}
		//DISPBUF(EG_mifs_tWorkInfo.aucBuffer,EG_mifs_tWorkInfo.ulSendBytes,0);
	);
		
	//timeout=get_timeout(EG_mifs_tWorkInfo.ulFWT);	

	timeout=OptRFID_TIMEOUT;
	
	// ��������������ʼֵ
	EG_mifs_tWorkInfo.ucCurResult = EM_SUCCESS;
	EG_mifs_tWorkInfo.ucINTSource = 0;
	EG_mifs_tWorkInfo.TimeSource = 0;  //CLRC663 timer flag
	EG_mifs_tWorkInfo.ulBytesSent = 0;	// how many bytes already sent
	EG_mifs_tWorkInfo.ulBytesReceived = 0;	// how many bytes received
	EG_mifs_tWorkInfo.lBitsReceived = 0;	// how many bits received
	EG_mifs_tWorkInfo.ucErrFlags = 0;
	EG_mifs_tWorkInfo.ucSaveErrState = 0x00;
	EG_mifs_tWorkInfo.ucCollPos = 0x00;
	ucrxMultiple = 0x00;
	ucIntEnable = 0x00;
	ucIntWait = 0x00;
	ucTimerCtl = 0x02;
    ucTempData = 0;
    ucWaterLevelBak = 0;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {

#ifdef EM_AS3911_Module
        ucTempData = ucTempData;
        ucWaterLevelBak = ucWaterLevelBak;
        ucTimerCtl = ucTimerCtl;
        ucIntWait = ucIntWait;
        ucIntEnable = ucIntEnable;
        ucrxMultiple = ucrxMultiple;
        timeout = timeout;

        int error;
        timer0 = 0;
        error = emvHalTransceive((u8 *)EG_mifs_tWorkInfo.aucBuffer, EG_mifs_tWorkInfo.ulSendBytes, 
                                        EG_mifs_tWorkInfo.aucBuffer, EG_mifs_tWorkInfo.expectMaxRec, (size_t *)&timer0, 
                                        EG_mifs_tWorkInfo.ulTimeout, (EmvHalTransceiveMode_t)cmd);
        EG_mifs_tWorkInfo.ucCurResult = s_as3911errorCode(error);

#endif

    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
    {
        ucIntWait1 = ucIntWait1;
#ifdef EM_RC663_Module
        // ��ֹ����������ж�Դ
        s_rfid_setRC663Int(0,RC663_IRQ_ALL);
        s_rfid_setRC663IntFlg(0,RC663_IRQ_ALL);
//        s_rfid_setRC663Int(1,RC663_IRQ_GLOBAL); //ȫ���ж�ʹ��

        // ���FIFO������
        EI_mifs_vFlushFIFO();

        // ������ǰ�������е�����
        ucTempData = PHHAL_HW_RC663_CMD_IDLE;
        EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F);
//        EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, ucTempData);

        // ����FIFO����ֵ
        EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_WATERLEVEL, &ucWaterLevelBak);
        ucTempData = RFID_DEFAULT_WATER_LEVEL;
        EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_WATERLEVEL, &ucTempData);
        // ���ݲ�ͬ������д���
        switch (cmd)
        {
        case PHHAL_HW_RC663_CMD_IDLE:
            {
                // ����
                ucTimerCtl = 0x00;			//��ʹ�ö�ʱ��
                ucIntEnable = RC663_IRQ_NONE;			//�������ж�
                ucIntWait = RC663_IRQ_NONE;			//���ȴ��ж�
                break;

            }
        case PHHAL_HW_RC663_CMD_TRANSCEIVE:
            {
                // ��������
                // ���ݽ�����ʼλ��λ��������ʵ���յ�λ��
                EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_RXBITCTRL, &ucTempData);
                ucTempData &= 0x7F;
                EG_mifs_tWorkInfo.lBitsReceived = -(long)(ucTempData >> 4);
                //TRACE("\r\nEG_mifs_tWorkInfo.lBitsReceived:%x       ucTempData:%x",EG_mifs_tWorkInfo.lBitsReceived,ucTempData);
                ucTimerCtl = PHHAL_HW_RC663_BIT_TSTOP_RX | PHHAL_HW_RC663_BIT_TSTART_TX | PHHAL_HW_RC663_VALUE_TCLK_T0;	// ���ݷ������ʱ��ʱ���Զ���ʼ
                ucIntEnable = (uchar)RC663_IRQ_ALL;			//attention CLRC663 timer
                ucIntWait1 = (uchar)(RC663_IRQ_TIMER1>>8);
                ucIntWait = RC663_IRQ_IDLE | RC663_IRQ_RX;			// IdleIRq
                break;
            }

        default:
            {
                //�Ƿ�����
                EG_mifs_tWorkInfo.ucCurResult = EM_mifs_UNKNOWN_COMMAND;
                break;
            }
        }									// switch(cmd)


        if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)
        {
            // ����Ϊ��֡����

            EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_RXCTRL, &ucTempData);  //
            ucrxMultiple = ucTempData & 0x40;
            if (ucrxMultiple == 0)
            {
                //EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_RXCTRL, 0x40);  //������֡���ջ� ����һ��ErrorReg�Ĵ�����ֵ
            }

            //��ʹ�ö�ʱ��
            if ( ucTimerCtl ) {
                // ���ö�ʱ�����Ʒ�ʽ
                ucTempData = PHHAL_HW_RC663_BIT_TAUTORESTARTED | PHHAL_HW_RC663_BIT_TSTART_TX | PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ;
                EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_T0CONTROL, &ucTempData);//�������� �Զ�����
                EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_T1CONTROL, &ucTimerCtl);
                // �����ʱ��Ҫ�˹���������������
                if ((ucTimerCtl & 0x30) == 0)
                {
                    //POINT(ucTimerCtl,99);
                    ucTempData = PHHAL_HW_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1STARTSTOPNOW | PHHAL_HW_RC663_BIT_T1RUNNING;
                    EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TCONTROL, &ucTempData);
                }
            }
            //D2(TRACE("\r\n IntWait:%2X IntEnable:%2X",ucIntWait,ucIntEnable););
            // ����������
//            TRACE("\r\n%x--data:%x",gtRfidProInfo.Cmd,EG_mifs_tWorkInfo.aucBuffer[0]);
            if ( EG_mifs_tWorkInfo.FIFOSIZE == 512 ) {
                EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOCONTROL, &ucTimerCtl);
                ucTimerCtl &= 0x03;
                EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOLENGTH, &ucTempData);
                timer0 = EG_mifs_tWorkInfo.FIFOSIZE - RFID_DEFAULT_WATER_LEVEL - (ushort)((ucTimerCtl<<8)|ucTempData) - 1;
            }else{
                //255
                EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOLENGTH, &ucTempData);
                timer0 = EG_mifs_tWorkInfo.FIFOSIZE - RFID_DEFAULT_WATER_LEVEL - ucTempData - 1;
            }
//            D2(TRACE("\r\n buf len:%d",timer0););
            if ((EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent) <= timer0)
            {
                timer0 = EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent;
            }
//            D2(TRACE("\r\n fifo size:%d--data len:%d",EG_mifs_tWorkInfo.FIFOSIZE,timer0););
//            D2(LABLE(0xCC);DATAIN(LBYTE(timer0)););	
            if(timer0)
            {
                //��һ֡������ǰ̨���� 
                //���ڵ�һ֡�����������ٻ����Ϸ������lo��־���ٴβ���!
                //��������ʱδ��ʵ�� ע���ʱ��̨���Ƿ���ٷ��ͣ���
                EI_mifs_vWriteReg(timer0, PHHAL_HW_RC663_REG_FIFODATA,
                                  EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesSent);
                EG_mifs_tWorkInfo.ulBytesSent += timer0;
//                EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F); //����1:��lo�жϺ�fifo������ݺ�����cmdҲ����
//                EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, cmd);
            }			
//            EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOLENGTH, &ucTempData);
//            D2(TRACE("\r\n len:%d",ucTempData););
            cmd &= 0x1F;
            EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F);
            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, cmd);
            // ���������εĴ���ת�Ƶ��������ɺ󣬱�����Ϊ�����жϻ������ȼ��ߵ�����������Ͳ���û��ִ�ж�ʹ����ʧ��
            // ��������ʱ����ʱ�ж�,���ҵȴ������ж�

//            s_rfid_setRC663IntFlg(0,RC663_IRQ_ALL);
//			EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_IRQ0, &ucTempData);		
//            D2(TRACE("\r\n irq flag:%x",ucTempData););
            s_rfid_setRC663Int(1, (uint)(ucIntEnable|RC663_IRQ_TIMER1|RC663_IRQ_GLOBAL));
            // �ȴ��������ж��������������Ϳ��Խ�������
            // �������6��û�д����򷵻ش���
            timer0 = sys_get_counter();
            while (1)
            {
                if(((EG_mifs_tWorkInfo.ucINTSource & ucIntWait) != 0)
                   ||(EG_mifs_tWorkInfo.TimeSource & ucIntWait1) != 0) 
                {
                    break;
                }
                if(if_timerout(timer0,timeout))
                {
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_NOTAGERR;
                    D2(LABLE(0x22););
                    break;
                }

            }

            // �����дEEPROM����, ����ȴ�����
            /*
               if (cmd == EM_mifs_PCD_WRITEE2)
               {
               while (1)
               {
               EI_mifs_vReadReg(1, EM_mifs_REG_SECONDARYSTATUS, &ucTempData);
               if ((ucTempData & 0x40) == 0x40)
               {
               break;
               }
               if(if_timerout(timer0,OptRFID_TIMEOUT))
               {
            // ��ʱ
            EG_mifs_tWorkInfo.ucCurResult = EM_TIMEOUT;
            EG_mifs_tWorkInfo.ucCollPos = 0;
            break;
            }
            }
            }
            */
            //�����������
            s_rfid_setRC663Int(0, RC663_IRQ_ALL); // ��ֹ�����ж�
            s_rfid_setRC663IntFlg(0, RC663_IRQ_ALL); // ��������ж�����λ

            ucTempData = PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING;
            EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_TCONTROL, &ucTempData);  //stop T0/T1

            // ���ص�IDLEģʽ
            ucTempData = PHHAL_HW_RC663_CMD_IDLE;
            EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F);
//            EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, ucTempData);

            // ��������ȥ��һЩ���ô���Ĵ���
//            if ((cmd == EM_mifs_PCD_WRITEE2) || (cmd == EM_mifs_PCD_READE2)
//                || (cmd == EM_mifs_PCD_LOADKEYE2) || (cmd == EM_mifs_PCD_LOADKEY))
//            {
//                // ֻ�����������
//                // ��Ҫ����Ϊ��4������ֻ�漰��CPU��RC531֮���ͨ��
//                EG_mifs_tWorkInfo.ucSaveErrState &= PN512_OVFLERR;
//            }
//            else
//            {
//                // ֻ���������������ͻ������ż����֡����CRC����
//                EG_mifs_tWorkInfo.ucSaveErrState &= PN512_ALLERR;
//            }
            // ���ش���
            if (EG_mifs_tWorkInfo.ucSaveErrState != 0)
            {
                if ((EG_mifs_tWorkInfo.ucSaveErrState & RC663_COLLERR) != 0)
                {
                    // ��ͻ
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_COLLERR;
                }
//                else if ((EG_mifs_tWorkInfo.ucSaveErrState & PN512_PARITYERR) != 0)
//                {
//                    // ��żУ���
//                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_PARITYERR;
//                }
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC663_FRAMINGERR) != 0)
                {
                    // ֡����
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_FRAMINGERR;
                }
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC663_CRCERR) != 0)
                {
                    // CRC����
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_CRCERR;
                }
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC663_OVFLERR) != 0)
                {
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_OVFLERR;
                }
//                else if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)
                else
                {
                    EG_mifs_tWorkInfo.ucCurResult = EM_ERROR;
                }
            }
            // �ָ���֡��������

            if (ucrxMultiple == 0)
            {
                //EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCTRL, 0x40);
            }

        }									// if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)

        // �ָ���ǰWaterLevel����
        EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_WATERLEVEL, &ucWaterLevelBak);

#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
	
#ifdef EM_PN512_Module

		// ��ֹ����������ж�Դ
		s_rfid_setRC531Int(0,PN512_IRQ_ALL);
		s_rfid_setRC531IntFlg(0,PN512_IRQ_ALL);

		// ���FIFO������
		EI_mifs_vFlushFIFO();
			
		// ������ǰ�������е�����
		ucTempData = PN512CMD_IDLE;
		ucTempData &= 0x0F;
		EI_mifs_vClrBitMask(CommandReg, 0x0F);
		EI_mifs_vSetBitMask(CommandReg, ucTempData);

		// ����FIFO����ֵ
		EI_mifs_vReadReg(1, WaterLevelReg, &ucWaterLevelBak);
		ucTempData = RFID_DEFAULT_WATER_LEVEL;
		EI_mifs_vWriteReg(1, WaterLevelReg, &ucTempData);
		// ���ݲ�ͬ������д���
		switch (cmd)
		{
		case PN512CMD_IDLE:
			{
				// ����
				ucTimerCtl = 0x00;			//��ʹ�ö�ʱ��
				ucIntEnable = PN512_IRQ_NONE;			//�������ж�
				ucIntWait = PN512_IRQ_NONE;			//���ȴ��ж�
				break;
			
			}
		case PN512CMD_TRANSCEIVE:
			{
				// ��������
				// ���ݽ�����ʼλ��λ��������ʵ���յ�λ��
				EI_mifs_vReadReg(1, BitFramingReg, &ucTempData);
				ucTempData &= 0x7F;
				EG_mifs_tWorkInfo.lBitsReceived = -(long)(ucTempData >> 4);
				//TRACE("\r\nEG_mifs_tWorkInfo.lBitsReceived:%x       ucTempData:%x",EG_mifs_tWorkInfo.lBitsReceived,ucTempData);
				ucTimerCtl = 0x80;			// ���ݷ������ʱ��ʱ���Զ���ʼ
				ucIntEnable = PN512_IRQ_ALL;			// TxIrq, RxIrq, IdleIRq and LoAlert
				ucIntWait = PN512_IRQ_IDLE | PN512_IRQ_RX;			// IdleIRq
				break;
			}
			
		default:
			{
				//�Ƿ�����
				EG_mifs_tWorkInfo.ucCurResult = EM_mifs_UNKNOWN_COMMAND;
				break;
			}
		}									// switch(cmd)


		if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)
		{
			// ����Ϊ��֡����
			
			EI_mifs_vReadReg(1, RxModeReg, &ucTempData);  //��֡������106k/s ʱ��Ч
			ucrxMultiple = ucTempData & 0x04;
			if (ucrxMultiple == 0)
			{
				//EI_mifs_vSetBitMask(RxModeReg, 0x04);  //������֡���ջ� ����һ��ErrorReg�Ĵ�����ֵ
			}
			
			// ���ö�ʱ�����Ʒ�ʽ
			EI_mifs_vSetBitMask(TModeReg, ucTimerCtl);
			// �����ʱ��Ҫ�˹���������������
			if ((ucTimerCtl & 0x80) == 0)
			{
				//POINT(ucTimerCtl,99);
				EI_mifs_vSetBitMask(ControlReg, 0x40); 
			}
			//D2(TRACE("\r\n IntWait:%2X IntEnable:%2X",ucIntWait,ucIntEnable););
			// ����������
			cmd &= 0x0F;
			EI_mifs_vReadReg(1, CommandReg, &ucTempData);
			ucTempData &= 0xF0;
			ucTempData |= cmd;
			EI_mifs_vWriteReg(1, CommandReg, &ucTempData);
			
			timer0 = sys_get_counter();
			// ���������εĴ���ת�Ƶ��������ɺ󣬱�����Ϊ�����жϻ������ȼ��ߵ�����������Ͳ���û��ִ�ж�ʹ����ʧ��
			// ��������ʱ����ʱ�ж�,���ҵȴ������ж�
			ucIntEnable |= PN512_IRQ_TIMER;
			ucIntWait |= PN512_IRQ_TIMER;

	//TI_ShowPN512Reg();
		
			s_rfid_setRC531Int(1, ucIntEnable);
			// �ȴ��������ж��������������Ϳ��Խ�������
			// �������10��û�д����򷵻ش���
			while (1)
			{
				if ((EG_mifs_tWorkInfo.ucINTSource & ucIntWait) != 0)
				{
					break;
				}
				if(if_timerout(timer0,timeout))
				{
					EG_mifs_tWorkInfo.ucCurResult = EM_mifs_NOTAGERR;
					D2(LABLE(0x22););
					break;
				}
			
			}
			
			// �����дEEPROM����, ����ȴ�����
			/*
			if (cmd == EM_mifs_PCD_WRITEE2)
			{
				while (1)
				{
					EI_mifs_vReadReg(1, EM_mifs_REG_SECONDARYSTATUS, &ucTempData);
					if ((ucTempData & 0x40) == 0x40)
					{
						break;
					}
					if(if_timerout(timer0,OptRFID_TIMEOUT))
					{
						// ��ʱ
						EG_mifs_tWorkInfo.ucCurResult = EM_TIMEOUT;
						EG_mifs_tWorkInfo.ucCollPos = 0;
						break;
					}
				}
			}
			*/
			//�����������
			s_rfid_setRC531Int(0, PN512_IRQ_ALL); // ��ֹ�����ж�
			s_rfid_setRC531IntFlg(0, PN512_IRQ_ALL); // ��������ж�����λ
			
			EI_mifs_vSetBitMask(ControlReg, 0x80);	// ֹͣ��ʱ��
			EI_mifs_vClrBitMask(BitFramingReg, 0x80);  //ֹͣ����
		
			// ���ص�IDLEģʽ
			ucTempData = PN512CMD_IDLE;
			ucTempData &= 0x0F;
			EI_mifs_vClrBitMask(CommandReg, 0x0F);
			EI_mifs_vSetBitMask(CommandReg, ucTempData);

			// ��������ȥ��һЩ���ô���Ĵ���
			if ((cmd == EM_mifs_PCD_WRITEE2) || (cmd == EM_mifs_PCD_READE2)
				|| (cmd == EM_mifs_PCD_LOADKEYE2) || (cmd == EM_mifs_PCD_LOADKEY))
			{
				// ֻ�����������
				// ��Ҫ����Ϊ��4������ֻ�漰��CPU��RC531֮���ͨ��
				EG_mifs_tWorkInfo.ucSaveErrState &= PN512_OVFLERR;
			}
			else
			{
				// ֻ���������������ͻ������ż����֡����CRC����
				EG_mifs_tWorkInfo.ucSaveErrState &= PN512_ALLERR;
			}
			// ���ش���
			if (EG_mifs_tWorkInfo.ucSaveErrState != 0)
			{
				if ((EG_mifs_tWorkInfo.ucSaveErrState & PN512_COLLERR) != 0)
				{
					// ��ͻ
					EG_mifs_tWorkInfo.ucCurResult = EM_mifs_COLLERR;
				}
				else if ((EG_mifs_tWorkInfo.ucSaveErrState & PN512_PARITYERR) != 0)
				{
					// ��żУ���
					EG_mifs_tWorkInfo.ucCurResult = EM_mifs_PARITYERR;
				}
				else if ((EG_mifs_tWorkInfo.ucSaveErrState & PN512_FRAMINGERR) != 0)
				{
					// ֡����
					EG_mifs_tWorkInfo.ucCurResult = EM_mifs_FRAMINGERR;
				}
				else if ((EG_mifs_tWorkInfo.ucSaveErrState & PN512_CRCERR) != 0)
				{
					// CRC����
					EG_mifs_tWorkInfo.ucCurResult = EM_mifs_CRCERR;
				}
				else if ((EG_mifs_tWorkInfo.ucSaveErrState & PN512_OVFLERR) != 0)
				{
					EG_mifs_tWorkInfo.ucCurResult = EM_mifs_OVFLERR;
				}
				else if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)
				{
					EG_mifs_tWorkInfo.ucCurResult = EM_ERROR;
				}
			}
			// �ָ���֡��������
			
			if (ucrxMultiple == 0)
			{
				//EI_mifs_vClrBitMask(RxModeReg, 0x04);
			}
			
		}									// if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)

		// �ָ���ǰWaterLevel����
		EI_mifs_vWriteReg(1, WaterLevelReg, &ucWaterLevelBak);
	
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module  //RC531

        // ��ֹ����������ж�Դ 
        s_rfid_setRC531Int(0,RC531_IRQ_ALL);
        s_rfid_setRC531IntFlg(0,RC531_IRQ_ALL);

        // ���FIFO������
        EI_mifs_vFlushFIFO();

        // ������ǰ�������е�����
        ucTempData = EM_mifs_PCD_IDLE;
        EI_mifs_vWriteReg(1, EM_mifs_REG_COMMAND, &ucTempData);

        // ����FIFO����ֵ
        EI_mifs_vReadReg(1, EM_mifs_REG_FIFOLEVEL, &ucWaterLevelBak);
        ucTempData = RFID_DEFAULT_WATER_LEVEL;
        EI_mifs_vWriteReg(1, EM_mifs_REG_FIFOLEVEL, &ucTempData);
        // ���ݲ�ͬ������д���
        switch (cmd)
        {
        case EM_mifs_PCD_IDLE:
            {
                // ����
                ucTimerCtl = 0x02;			//��ʹ�ö�ʱ��
                ucIntEnable = RC531_IRQ_NONE;			//�������ж�
                ucIntWait = RC531_IRQ_NONE;			//���ȴ��ж�
                break;
            }
        case EM_mifs_PCD_WRITEE2:
            {
                // дEEPROM
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ��
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_TX;			// LoAlert and TxIRq
                ucIntWait = RC531_IRQ_TX;			// TxIRq
                break;
            }
        case EM_mifs_PCD_READE2:
            {
                // ��EEPROM
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ��
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_HI|RC531_IRQ_IDLE; // HiAlert, LoAlert and IdleIRq
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_LOADKEYE2:
            {
                //��EEPROM������Կ
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ��
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_IDLE;			// IdleIRq and LoAlert
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_LOADCONFIG:
            {
                // ��������
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ�� 
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_IDLE;			// IdleIRq and LoAlert
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_AUTHENT1:
            {
                //��֤����1
                ucTimerCtl = 0x02;			// ���ݷ������ʱ��ʱ���Զ���ʼ
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_IDLE;			// IdleIRq and LoAlert
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_CALCCRC:
            {
                //CRCУ��
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ��             
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_TX;			// LoAlert and TxIRq
                ucIntWait = RC531_IRQ_TX;			// TxIRq
                break;
            }
        case EM_mifs_PCD_AUTHENT2:
            {
                //��֤����2
                ucTimerCtl = 0x02;			// ���ݷ������ʱ��ʱ���Զ���ʼ
                ucIntEnable = RC531_IRQ_IDLE;			// IdleIRq
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_RECEIVE:
            {
                //��������
                //���ݽ�����ʼλ��λ��������ʵ���յ�λ��
                EI_mifs_vReadReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);
                EG_mifs_tWorkInfo.lBitsReceived = -(long)(ucTempData >> 4);

                ucTimerCtl = 0x04;			//�ֶ���ʼ, ���ݽ������ʱ��ʱ���Զ�ֹͣ
                ucIntEnable = RC531_IRQ_HI|RC531_IRQ_IDLE|RC531_IRQ_RX;			// HiAlert and IdleIRq
                ucIntWait = RC531_IRQ_IDLE|RC531_IRQ_RX;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_LOADKEY:
            {
                //��FIFO������������Կ
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ��             
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_IDLE;			// LoAlert and IdleIRq
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_TRANSMIT:
            {
                //��������
                ucTimerCtl = 0x00;			// �ֶ�������ֹͣ��ʱ��          
                ucIntEnable = RC531_IRQ_LO|RC531_IRQ_IDLE;			// LoAlert and IdleIRq
                ucIntWait = RC531_IRQ_IDLE;			// IdleIRq
                break;
            }
        case EM_mifs_PCD_TRANSCEIVE:
            {
                // ��������
                // ���ݽ�����ʼλ��λ��������ʵ���յ�λ��
                EI_mifs_vReadReg(1, EM_mifs_REG_BITFRAMING, &ucTempData);
                EG_mifs_tWorkInfo.lBitsReceived = -(long)(ucTempData >> 4);
                ucTimerCtl = 0x06;			// ���ݷ������ʱ��ʱ���Զ���ʼ
                ucIntEnable = RC531_IRQ_ALL;			// TxIrq, RxIrq, IdleIRq and LoAlert
                ucIntWait = RC531_IRQ_IDLE | RC531_IRQ_RX;			// IdleIRq
                break;
            }
        default:
            {
                //�Ƿ�����
                EG_mifs_tWorkInfo.ucCurResult = EM_mifs_UNKNOWN_COMMAND;
                break;
            }
        }									// switch(cmd)

        // �ղ�û�д���
        if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)
        {
            // ����Ϊ��֡����
            EI_mifs_vReadReg(1, EM_mifs_REG_DECODERCONTROL, &ucTempData);
            ucrxMultiple = ucTempData & 0x40;
            if (ucrxMultiple == 0)
            {
                EI_mifs_vSetBitMask(EM_mifs_REG_DECODERCONTROL, 0x40);
            }
            // ���ö�ʱ�����Ʒ�ʽ
            EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCONTROL, &ucTimerCtl);
            // �����ʱ��Ҫ�˹���������������
            if ((~ucTimerCtl & 0x02) != 0)
            {
                EI_mifs_vSetBitMask(EM_mifs_REG_CONTROL, 0x02);
            }
            //D2(TRACE("\r\n IntWait:%2X IntEnable:%2X",ucIntWait,ucIntEnable););
            // ����������
            EI_mifs_vWriteReg(1, EM_mifs_REG_COMMAND, &cmd);

            timer0 = sys_get_counter();
            // ���������εĴ���ת�Ƶ��������ɺ󣬱�����Ϊ�����жϻ������ȼ��ߵ�����������Ͳ���û��ִ�ж�ʹ����ʧ��
            // ��������ʱ����ʱ�ж�,���ҵȴ������ж�
            ucIntEnable |= RC531_IRQ_TIMER;
            ucIntWait |= RC531_IRQ_TIMER;
            s_rfid_setRC531Int(1, ucIntEnable);
            // �ȴ��������ж��������������Ϳ��Խ�������
            // �������10��û�д����򷵻ش���
            while (1)
            {
                if ((EG_mifs_tWorkInfo.ucINTSource & ucIntWait) != 0)
                {
                    break;
                }
                if(if_timerout(timer0,timeout))
                {
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_NOTAGERR;
                    D2(LABLE(0x22););			
                    break;
                }
            }
            // �����дEEPROM����, ����ȴ�����
            if (cmd == EM_mifs_PCD_WRITEE2)
            {
                while (1)
                {
                    EI_mifs_vReadReg(1, EM_mifs_REG_SECONDARYSTATUS, &ucTempData);
                    if ((ucTempData & 0x40) == 0x40)
                    {
                        break;
                    }
                    if(if_timerout(timer0,timeout))
                    {
                        // ��ʱ
                        EG_mifs_tWorkInfo.ucCurResult = EM_TIMEOUT;
                        EG_mifs_tWorkInfo.ucCollPos = 0;
                        break;
                    }
                }
            }
            //�����������      
            EI_mifs_vSetBitMask(EM_mifs_REG_CONTROL, 0x04);	// ֹͣ��ʱ��        
            s_rfid_setRC531Int(0, RC531_IRQ_ALL); // ��ֹ�����ж�
            s_rfid_setRC531IntFlg(0, RC531_IRQ_ALL); // ��������ж�����λ
            // ���ص�IDLEģʽ
            ucTempData = EM_mifs_PCD_IDLE;
            EI_mifs_vWriteReg(1, EM_mifs_REG_COMMAND, &ucTempData);

            // ��������ȥ��һЩ���ô���Ĵ���        
            if ((cmd == EM_mifs_PCD_WRITEE2) || (cmd == EM_mifs_PCD_READE2)
                || (cmd == EM_mifs_PCD_LOADKEYE2) || (cmd == EM_mifs_PCD_LOADKEY))
            {
                // ֻ�����������
                // ��Ҫ����Ϊ��4������ֻ�漰��CPU��RC531֮���ͨ��
                EG_mifs_tWorkInfo.ucSaveErrState &= RC531_OVFLERR;
            }
            else
            {
                // ֻ���������������ͻ������ż����֡����CRC����
                EG_mifs_tWorkInfo.ucSaveErrState &= RC531_ALLERR;
            }
            // ���ش���
            if (EG_mifs_tWorkInfo.ucSaveErrState != 0)
            {
                if ((EG_mifs_tWorkInfo.ucSaveErrState & RC531_COLLERR) != 0)
                {
                    // ��ͻ
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_COLLERR;
                }
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC531_PARITYERR) != 0)
                {
                    // ��żУ���
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_PARITYERR;
                }
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC531_FRAMINGERR) != 0)
                {
                    // ֡����
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_FRAMINGERR;
                }		
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC531_CRCERR) != 0)
                {
                    // CRC����
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_CRCERR;
                }
                else if ((EG_mifs_tWorkInfo.ucSaveErrState & RC531_OVFLERR) != 0)
                {
                    EG_mifs_tWorkInfo.ucCurResult = EM_mifs_OVFLERR;
                }
                else if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)	
                {
                    EG_mifs_tWorkInfo.ucCurResult = EM_ERROR;
                }
            }								
            // �ָ���֡��������
            if (ucrxMultiple == 0)
            {
                EI_mifs_vClrBitMask(EM_mifs_REG_DECODERCONTROL, 0x40);
            }
        }									// if (EG_mifs_tWorkInfo.ucCurResult == EM_SUCCESS)

        // �ָ���ǰWaterLevel����
        EI_mifs_vWriteReg(1, EM_mifs_REG_FIFOLEVEL, &ucWaterLevelBak);
		
#endif

	}
	

	
    D1( LABLE(0xDD);
        DATAIN(EG_mifs_tWorkInfo.ucCurResult);
        //DATAIN(EG_mifs_tWorkInfo.ucINTSource);
        DATAIN(EG_mifs_tWorkInfo.ucSaveErrState);
        DATAIN(LBYTE(EG_mifs_tWorkInfo.ulBytesReceived));
        DATAIN(LBYTE(EG_mifs_tWorkInfo.lBitsReceived));
        //DATAIN(EG_mifs_tWorkInfo.ucnBits);
        //DATAIN(EG_mifs_tWorkInfo.ucCollPos);		
        for (guiDebugj = 0; guiDebugj < EG_mifs_tWorkInfo.ulBytesReceived; guiDebugj++)
        {
        DATAIN(EG_mifs_tWorkInfo.aucBuffer[guiDebugj]);
        }
      );
    return EG_mifs_tWorkInfo.ucCurResult;
}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vSetTimer(uchar time)
* ���������� 
*   ��ʱ������
* ��������� 
*   time: ��ʱʱ��
*   0~14: 0.000302*2^time     
*   ����: 128*(TEMP)/ 13560000��     
* ���������
*   ��
* ����ֵ�� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
***********************************************************************/
void EI_mifs_vSetTimer(ulong time)
{
	uchar ucTempData = 0;

    if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911)
    {
        ucTempData = ucTempData;
#ifdef EM_AS3911_Module
        EI_paypass_vSetTimer(time);
#endif

    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
    {
        ucTempData = ucTempData;
#ifdef EM_RC663_Module
        EI_paypass_vSetTimer(time);
#endif

    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
	
#ifdef EM_PN512_Module

		ucTempData = 0x40;      //64/6.78 = 9.43us
		EI_mifs_vWriteReg(1, TPrescalerReg, &ucTempData);
		ucTempData = 0x0F;
		EI_mifs_vClrBitMask(TModeReg, ucTempData);

		ucTempData = (uchar)(time&0xFF); 
		EI_mifs_vWriteReg(1, TReloadVal_Lo, &ucTempData);
		ucTempData = (uchar)((time>>8)&0xFF); ; 
		EI_mifs_vWriteReg(1, TReloadVal_Hi, &ucTempData);

#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module  //RC531

        switch (time)
        {
        case 0:							// (0.302 ms) FWI = 0
            {
                // TAutoRestart = 0, TPrescale = 128
                ucTempData = 0x07;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 33
                ucTempData = 0x21;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 1:							// (0.604 ms) FWI=1
            {
                // TAutoRestart=0,TPrescale=128
                ucTempData = 0x07;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 65
                ucTempData = 0x41;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 2:							// (1.208 ms) FWI=2
            {
                // TAutoRestart=0,TPrescale=128
                ucTempData = 0x07;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 129
                ucTempData = 0x81;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 3:							// (2.416 ms) FWI=3
            {
                // TAutoRestart=0,TPrescale=512
                ucTempData = 0x09;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 65
                ucTempData = 0x41;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 4:							// (4.833 ms) FWI=4
            {
                // TAutoRestart=0,TPrescale=512
                ucTempData = 0x09;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 129
                ucTempData = 0x81;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 5:							// (9.666 ms) FWI=5
            {
                // TAutoRestart=0,TPrescale=2048
                ucTempData = 0x0B;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 65
                ucTempData = 0x41;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 6:							// (19.33 ms) FWI=6  13560
            {
                // TAutoRestart=0,TPrescale=2048
                ucTempData = 0x0B;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 129
                ucTempData = 0x81;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 7:							// (38.66 ms) FWI=7
            {
                // TAutoRestart=0,TPrescale=8192
                ucTempData = 0x0D;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 65
                ucTempData = 0x41;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 8:							// (77.32 ms) FWI=8
            {
                // TAutoRestart=0,TPrescale=8192
                ucTempData = 0x0D;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 129
                ucTempData = 0x81;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 9:							// (154.6 ms) FWI=9
            {
                // TAutoRestart=0,TPrescale=32768
                ucTempData = 0x0F;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 65
                ucTempData = 0x41;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 10:							// (309.3 ms) FWI=10
            {
                // TAutoRestart=0,TPrescale=32768
                ucTempData = 0x0F;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 129
                ucTempData = 0x81;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 11:							// (618.6 ms) FWI=11
            {
                // TAutoRestart=0,TPrescale=524288
                ucTempData = 0x13;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 17
                ucTempData = 0x11;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 12:							// (1.2371 s) FWI=12
            {
                // TAutoRestart=0,TPrescale=524288
                ucTempData = 0x13;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 33
                ucTempData = 0x21;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 13:							// (2.4742 s) FWI=13
            {
                // TAutoRestart=0,TPrescale=524288
                ucTempData = 0x13;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 65
                ucTempData = 0x41;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        case 14:							// (4.9485 s) FWI=14
            {
                // TAutoRestart=0,TPrescale=524288
                ucTempData = 0x13;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);

                // TReloadVal = 129
                ucTempData = 0x81;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        default:
            {
                //ȱʡ�����ֱ������
                // TAutoRestart=0,TPrescale=128
                ucTempData = 0x07;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCLOCK, &ucTempData);
                ucTempData = (uchar)time;
                EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERRELOAD, &ucTempData);
                break;
            }
        }
		
#endif

	}
	
}

/**********************************************************************
* �������ƣ� 
*   void EI_vMifsHisr (void) 
* ���������� 
*   RC531оƬ���жϴ������
* ��������� 
*   ��
* ���������
*   ��
* ����ֵ�� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
***********************************************************************/
void EI_vMifsHisr(void)
{
	uchar ucTempData = 0,ucTempData1 = 0;
	uchar ucIrqPending = 0;
	ulong ulTemp = 0;
	vuchar Data = 0;

    if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911){

        ucTempData = ucTempData;
        ucTempData1 = ucTempData1;
        ucIrqPending = ucIrqPending;
        ulTemp = ulTemp;
        Data = Data;
#ifdef EM_AS3911_Module
        #if AS3911_IRQMODE
        as3911Isr();
        #endif
#endif

    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
    {
        ucTempData1 = ucTempData1;
#ifdef EM_RC663_Module

#if 0
        GPIO_SetBits(Port_LedGreen,Pin_LedGreen);	
        EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_IRQ0, &ucTempData);
        EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_IRQ1, &ucIrqPending);
        s_rfid_setRC663IntFlg(0,0x1F7F);
        GPIO_ResetBits(Port_LedGreen,Pin_LedGreen);	
#endif
		while(1)
		{	
			if((gtRfidProInfo.IntEnable == 0)||((gtRfidProInfo.TimerIntEnable&(RC663_IRQ_TIMER1>>8)) == 0))
			{
				break;
			}
			EI_mifs_vReadReg(1,PHHAL_HW_RC663_REG_IRQ1,(uchar *)&Data);	
//rfid_isr_start:	
			EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_IRQ0, &ucIrqPending);		
            EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_ERROR, (uchar *) &EG_mifs_tWorkInfo.ucErrFlags);
//			EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_IRQ0EN, &ucTempData1);		
//            D2(LABLE(0xBB);DATAIN(ucIrqPending);DATAIN(Data);DATAIN(ucTempData1);DATAIN(gtRfidProInfo.IntEnable););	
//            if ( ++i > 100000 ) {
//                TRACE("\r\n%x-%x-%x-%x", ucIrqPending, Data,gtRfidProInfo.IntEnable,gtRfidProInfo.TimerIntEnable);
//                break;
//            }
			ucIrqPending &= gtRfidProInfo.IntEnable;
//            Data &= gtRfidProInfo.TimerIntEnable;
			EG_mifs_tWorkInfo.ucINTSource = EG_mifs_tWorkInfo.ucINTSource | ucIrqPending;
			EG_mifs_tWorkInfo.TimeSource = EG_mifs_tWorkInfo.TimeSource | Data;
			D2(LABLE(0xAA);DATAIN(ucIrqPending);DATAIN(Data);DATAIN(EG_mifs_tWorkInfo.ucErrFlags););	
            if ((Data&(RC663_IRQ_TIMER1>>8)) == 0) {
                //time1 �ж�δ����ʱ
                if ( ucIrqPending == 0 ) {
                    D2(LABLE(0xBC);DATAIN(ucIrqPending);DATAIN(Data););	
                    break;//û����Ҫ���ж� �˳�
                }
            }
			// ����LoAlert  ��һ֡������ǰ̨����
			if (ucIrqPending & RC663_IRQ_LO)
			{		
				s_rfid_setRC663IntFlg(0, RC663_IRQ_LO);
                if ( EG_mifs_tWorkInfo.FIFOSIZE == 512 ) {
                    EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOCONTROL, &ucTempData1);
                    ucTempData1 &= 0x03;
                    EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOLENGTH, &ucTempData);
                    ulTemp = EG_mifs_tWorkInfo.FIFOSIZE - RFID_DEFAULT_WATER_LEVEL - (ushort)((ucTempData1<<8)|ucTempData) - 1;
                }else{
                    //255
                    EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOLENGTH, &ucTempData);
                    ulTemp = EG_mifs_tWorkInfo.FIFOSIZE - RFID_DEFAULT_WATER_LEVEL - ucTempData - 1;
                }

				if ((EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent) <= ulTemp)
				{
					ulTemp = EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent;
					s_rfid_setRC663Int(0, RC663_IRQ_LO);
				}
				D2(LABLE(0xF2);DATAIN(LBYTE(ulTemp)););	
				if(ulTemp)
				{
					EI_mifs_vWriteReg(ulTemp, PHHAL_HW_RC663_REG_FIFODATA,
						EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesSent);
					EG_mifs_tWorkInfo.ulBytesSent += ulTemp;

//                    EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_COMMAND, 0x1F); //����1:��lo�жϺ�fifo������ݺ�����cmdҲ����
//                    EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_COMMAND, cmd);
				}			
			}
			// TxIRQ �������׼������ 
			if (ucIrqPending & RC663_IRQ_TX)
			{			
				D2(LABLE(0xF3););
				s_rfid_setRC663IntFlg(0, RC663_IRQ_TX);
				if(gtRfidProInfo.Cmd != EM_mifs_PICC_HALT)
				{		
					// �����������������У��
					if (((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL1)
							&& (EG_mifs_tWorkInfo.lBitsReceived == 7))
						|| ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL2)
							&& (EG_mifs_tWorkInfo.lBitsReceived == 7))
						|| ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL3)
							&& (EG_mifs_tWorkInfo.lBitsReceived == 7)))
					{
						// RxCRC and TxCRC disable, parity disable               
						ucTempData = 0xC0;
						EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_FRAMECON, ucTempData); // Parity disable
						ucTempData = 0x01;
						EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_TXCRCCON, ucTempData); // CRC disable
						EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_RXCRCCON, ucTempData); // CRC disable
					}
				}
				else
				{
					s_rfid_setRC663Int(0, RC663_IRQ_ALL);
					s_rfid_setRC663IntFlg(0, RC663_IRQ_ALL);
					EG_mifs_tWorkInfo.ucINTSource |= RC663_IRQ_RX;
					return;
				}
			}
			
			if (ucIrqPending & (RC663_IRQ_RX | RC663_IRQ_HI))
			{	
				
				if(ucIrqPending & RC663_IRQ_RX)
				{
					//RX
					s_rfid_setRC663IntFlg(0, RC663_IRQ_RX);
					D2(LABLE(0xF4););				
				}
				if(ucIrqPending & RC663_IRQ_HI)
				{
					s_rfid_setRC663IntFlg(0, RC663_IRQ_HI);
					D2(LABLE(0xF5););				
				}
				// ������ʱ�Ż��ȡ����Ĵ���
				EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_ERROR, (uchar *) &EG_mifs_tWorkInfo.ucErrFlags);
				EG_mifs_tWorkInfo.ucErrFlags &= RC663_ALLERR;			
				D2(DATAIN(EG_mifs_tWorkInfo.ucErrFlags););				
				if(EG_mifs_tWorkInfo.ucErrFlags & RC663_COLLERR)
				{
					//col���� ֻ�з���ײ����,�������ͻ ��ȡ��ͻλ��
					if ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL1)			
						|| (EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL2)			
						|| (EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL3))
					{
						EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_RXCOLL, (uchar *) & EG_mifs_tWorkInfo.ucCollPos);
						EG_mifs_tWorkInfo.ucCollPos &= 0x7F; //bit0 - bit6 Ϊ��ͻλ��
                        EG_mifs_tWorkInfo.ucErrFlags &= ~RC663_CRCERR;   //��ͻ��������ż��CRCУ��
					}
					if(EG_mifs_tWorkInfo.ucnBits == 7)
					{
						//��ͻ��⣬��λ��Ϊ7, �������żУ���
//						EG_mifs_tWorkInfo.ucErrFlags &= (~RC531_PARITYERR);
					}					
				}
				EG_mifs_tWorkInfo.ucSaveErrState |= EG_mifs_tWorkInfo.ucErrFlags;
				EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_FIFOLENGTH, &ucTempData);
				if(ucTempData != 0)
				{
					D2(DATAIN(ucTempData););
					ucTempData = (uchar)MIN((ulong)ucTempData,(EM_mifs_BUFFER_LEN - EG_mifs_tWorkInfo.ulBytesReceived));
					EI_mifs_vReadReg(ucTempData,PHHAL_HW_RC663_REG_FIFODATA,EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesReceived);
					EG_mifs_tWorkInfo.ulBytesReceived += ucTempData;
//					D2(DATAIN(EG_mifs_tWorkInfo.aucBuffer[0]);
//                       DATAIN(EG_mifs_tWorkInfo.aucBuffer[1]);
//                       DATAIN(EG_mifs_tWorkInfo.aucBuffer[2]);
//                       );
				}
				if(ucIrqPending & RC663_IRQ_RX)
				{
					// ���յ����һ���ֽ�λ��
					EI_mifs_vReadReg(1, PHHAL_HW_RC663_REG_RXBITCTRL, &ucTempData);
					ucTempData &= 0x07;
					if (ucTempData != 0)
					{
						// ���һ���ֽ�û�н�����
						EG_mifs_tWorkInfo.lBitsReceived +=
							(EG_mifs_tWorkInfo.ulBytesReceived - 1) * 8 + ucTempData;
						D2(LABLE(0xF7);DATAIN(ucTempData););
					}
					else
					{
						// ���һ���ֽڽ�����
						EG_mifs_tWorkInfo.lBitsReceived += EG_mifs_tWorkInfo.ulBytesReceived * 8;
					}
				}		
			}
			if (ucIrqPending & RC663_IRQ_IDLE)
			{
				D2(LABLE(0xF8););
				s_rfid_setRC663IntFlg(0, RC663_IRQ_TIMER1 | RC663_IRQ_IDLE);
				s_rfid_setRC663Int(0, RC663_IRQ_TIMER1);
				EG_mifs_tWorkInfo.TimeSource &= ~(RC663_IRQ_TIMER1>>8);
//				Data &= ~(RC663_IRQ_TIMER1>>8);
			}
			if (Data & (RC663_IRQ_TIMER1>>8))
			{
				//��ʱ���ж�
				D1(LABLE(0xF0););
				EG_mifs_tWorkInfo.ucCurResult = EM_mifs_NOTAGERR;
				s_rfid_setRC663IntFlg(0, RC663_IRQ_TIMER1);
				s_rfid_setRC663Int(0, RC663_IRQ_TIMER1);
			}		
		}
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
	
#ifdef EM_PN512_Module
		while(1)
		{
			//SETSIGNAL1_H();
			EI_mifs_vReadReg(1, ComIrqReg, &ucIrqPending);
			EI_mifs_vReadReg(1, CommIEnReg, &ucTempData);
	
			D2(LABLE(0xAA);DATAIN(ucIrqPending);DATAIN(ucTempData);DATAIN(gtRfidProInfo.IntEnable););
			if (gtRfidProInfo.IntEnable == 0)
			{
				break;
			}
			EI_mifs_vReadReg(1,Status1Reg,(uchar *)&Data);
			//SETSIGNAL1_L();
			D2(LABLE(0xBB);DATAIN(Data););
			if ((Data & 0x10) == 0)
			{
				//D2(LABLE(0xFF););
				break;
			}

		//rfid_isr_start:
			//EI_mifs_vReadReg(1, ComIrqReg, &ucIrqPending);
			//EI_mifs_vReadReg(1, CommIEnReg, &ucTempData);
		      //ucIrqPending &= 0x7F;
			//D2(LABLE(0xAA);DATAIN(ucIrqPending);DATAIN(ucTempData);DATAIN(gtRfidProInfo.IntEnable););
			ucIrqPending &= gtRfidProInfo.IntEnable;
			EG_mifs_tWorkInfo.ucINTSource = EG_mifs_tWorkInfo.ucINTSource | ucIrqPending;
			//D2(LABLE(0xAC);DATAIN(EG_mifs_tWorkInfo.ucINTSource););
			//D2(LABLE(0xAD);DATAIN(ucIrqPending););
			// ����LoAlert
			if (ucIrqPending & PN512_IRQ_LO)
			{
				s_rfid_setRC531IntFlg(0, PN512_IRQ_LO);
				EI_mifs_vReadReg(1, FIFOLevelReg, &ucTempData);
				ucTempData &= 0x7F; //PN512 bit7 �����
				ulTemp = 64 - RFID_DEFAULT_WATER_LEVEL - ucTempData - 1;
				if ((EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent) <= ulTemp)
				{
					ulTemp = EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent;
					s_rfid_setRC531Int(0, PN512_IRQ_LO);
				}
				D2(LABLE(0xF2);DATAIN(LBYTE(ulTemp)););
				if(ulTemp)
				{
					EI_mifs_vWriteReg(ulTemp, FIFODataReg,
						EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesSent);
					EG_mifs_tWorkInfo.ulBytesSent += ulTemp;
					
					EI_mifs_vSetBitMask(BitFramingReg, 0x80);  //��������
				}
			}
			// TxIRQ �������׼������
			if (ucIrqPending & PN512_IRQ_TX)
			{
				D2(LABLE(0xF3););
				s_rfid_setRC531IntFlg(0, PN512_IRQ_TX);
				if(gtRfidProInfo.Cmd != EM_mifs_PICC_HALT)
				{
					// �����������������У��
					if (((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL1)
							&& (EG_mifs_tWorkInfo.lBitsReceived == 7))
						|| ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL2)
							&& (EG_mifs_tWorkInfo.lBitsReceived == 7))
						|| ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL3)
							&& (EG_mifs_tWorkInfo.lBitsReceived == 7)))
					{
						// RxCRC and TxCRC disable, parity disable
						ucTempData = 0x10;
						EI_mifs_vSetBitMask(ManualRCVReg, ucTempData);  // disable ��żУ��
					}
				}
				else
				{
					s_rfid_setRC531Int(0, PN512_IRQ_ALL);
					s_rfid_setRC531IntFlg(0, PN512_IRQ_ALL);
					EG_mifs_tWorkInfo.ucINTSource |= PN512_IRQ_RX;
					return;
				}
			}

			if (ucIrqPending & (PN512_IRQ_RX | PN512_IRQ_HI))
			{
				if(ucIrqPending & PN512_IRQ_RX)
				{
					//RX
					s_rfid_setRC531IntFlg(0, PN512_IRQ_RX);
					D2(LABLE(0xF4););
				}
				if(ucIrqPending & PN512_IRQ_HI)
				{
					s_rfid_setRC531IntFlg(0, PN512_IRQ_HI);
					D2(LABLE(0xF5););
				}
				// ������ʱ�Ż��ȡ����Ĵ���
				EI_mifs_vReadReg(1, ErrorReg, (uchar *) &EG_mifs_tWorkInfo.ucErrFlags);
				EG_mifs_tWorkInfo.ucErrFlags &= PN512_ALLERR;
				D2(DATAIN(EG_mifs_tWorkInfo.ucErrFlags););
				if(EG_mifs_tWorkInfo.ucErrFlags & PN512_COLLERR)
				{
					//col���� ֻ�з���ײ����,�������ͻ ��ȡ��ͻλ��
					if ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL1)
						|| (EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL2)
						|| (EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL3))
					{
						EI_mifs_vReadReg(1, CollReg, (uchar *) & EG_mifs_tWorkInfo.ucCollPos);
						EG_mifs_tWorkInfo.ucCollPos &= 0x1F; //bit0 - bit4 Ϊ��ͻλ��
					}
					if(EG_mifs_tWorkInfo.ucnBits == 7)
					{
						//��ͻ��⣬��λ��Ϊ7, �������żУ���
						EG_mifs_tWorkInfo.ucErrFlags &= (~PN512_PARITYERR);
					}
				}
				EG_mifs_tWorkInfo.ucSaveErrState |= EG_mifs_tWorkInfo.ucErrFlags;
				EI_mifs_vReadReg(1, FIFOLevelReg, &ucTempData);
				ucTempData &= 0x7F;
				D2(DATAIN(ucTempData););
				if(ucTempData != 0)
				{
					//D2(DATAIN(ucTempData););
					ucTempData = (uchar)MIN((ulong)ucTempData,(EM_mifs_BUFFER_LEN - EG_mifs_tWorkInfo.ulBytesReceived));
					
					EI_mifs_vReadReg(ucTempData,FIFODataReg,EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesReceived);
					EG_mifs_tWorkInfo.ulBytesReceived += ucTempData;
				}
				if(ucIrqPending & PN512_IRQ_RX)
				{
					// ���յ����һ���ֽ�λ��
					EI_mifs_vReadReg(1, ControlReg, &ucTempData);
					ucTempData &= 0x07;
					if (ucTempData != 0)
					{
						// ���һ���ֽ�û�н�����
						EG_mifs_tWorkInfo.lBitsReceived +=
							(EG_mifs_tWorkInfo.ulBytesReceived - 1) * 8 + ucTempData;
						D2(LABLE(0xF7);DATAIN(ucTempData););
					}
					else
					{
						// ���һ���ֽڽ�����
						EG_mifs_tWorkInfo.lBitsReceived += EG_mifs_tWorkInfo.ulBytesReceived * 8;
					}
				}
			}
			if (ucIrqPending & PN512_IRQ_IDLE)
			{
				D2(LABLE(0xF8););
				s_rfid_setRC531IntFlg(0, PN512_IRQ_TIMER | PN512_IRQ_IDLE);
				s_rfid_setRC531Int(0, PN512_IRQ_TIMER);
				EG_mifs_tWorkInfo.ucINTSource &= ~(PN512_IRQ_TIMER);
				ucIrqPending &= ~(PN512_IRQ_TIMER);
			}
			if (ucIrqPending & PN512_IRQ_TIMER)
			{
				//��ʱ���ж�
				D1(LABLE(0xF0););
				EG_mifs_tWorkInfo.ucCurResult = EM_mifs_NOTAGERR;
				s_rfid_setRC531IntFlg(0, PN512_IRQ_TIMER);
			}

		}
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

        while(1)
        {	
            //2011-02-25 yehf����
            if(gtRfidProInfo.IntEnable == 0)
            {
                break;
            }
            EI_mifs_vReadReg(1,EM_mifs_REG_PRIMARYSTATUS,(uchar *)&Data);	
            //D2(LABLE(0xAA);DATAIN(Data););	
            if ((Data & 0x08) == 0)
            {
                break;
            }

            //rfid_isr_start:
            EI_mifs_vReadReg(1, EM_mifs_REG_INTERRUPTRQ, &ucIrqPending);
            ucIrqPending &= gtRfidProInfo.IntEnable;
            EG_mifs_tWorkInfo.ucINTSource = EG_mifs_tWorkInfo.ucINTSource | ucIrqPending;
            D2(LABLE(0xAA);DATAIN(ucIrqPending););
            // ����LoAlert
            if (ucIrqPending & RC531_IRQ_LO)
            {
                s_rfid_setRC531IntFlg(0, RC531_IRQ_LO);
                EI_mifs_vReadReg(1, EM_mifs_REG_FIFOLENGTH, &ucTempData);
                ulTemp = 64 - RFID_DEFAULT_WATER_LEVEL - ucTempData - 1;
                if ((EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent) <= ulTemp)
                {
                    ulTemp = EG_mifs_tWorkInfo.ulSendBytes - EG_mifs_tWorkInfo.ulBytesSent;
                    s_rfid_setRC531Int(0, RC531_IRQ_LO);
                }
                D2(LABLE(0xF2);DATAIN(LBYTE(ulTemp)););
                if(ulTemp)
                {
                    EI_mifs_vWriteReg(ulTemp, EM_mifs_REG_FIFODATA,
                                      EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesSent);
                    EG_mifs_tWorkInfo.ulBytesSent += ulTemp;
                }
            }
            // TxIRQ �������׼������
            if (ucIrqPending & RC531_IRQ_TX)
            {
                D2(LABLE(0xF3););
                s_rfid_setRC531IntFlg(0, RC531_IRQ_TX);
                if(gtRfidProInfo.Cmd != EM_mifs_PICC_HALT)
                {
                    // �����������������У��
                    if (((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL1)
                         && (EG_mifs_tWorkInfo.lBitsReceived == 7))
                        || ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL2)
                            && (EG_mifs_tWorkInfo.lBitsReceived == 7))
                        || ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL3)
                            && (EG_mifs_tWorkInfo.lBitsReceived == 7)))
                    {
                        // RxCRC and TxCRC disable, parity disable
                        ucTempData = 0x02;
                        EI_mifs_vWriteReg(1, EM_mifs_REG_CHANNELREDUNDANCY, &ucTempData);
                    }
                }
                else
                {
                    s_rfid_setRC531Int(0, RC531_IRQ_ALL);
                    s_rfid_setRC531IntFlg(0, RC531_IRQ_ALL);
                    EG_mifs_tWorkInfo.ucINTSource |= RC531_IRQ_RX;
                    return;
                }
            }

            if (ucIrqPending & (RC531_IRQ_RX | RC531_IRQ_HI))
            {
                if(ucIrqPending & RC531_IRQ_RX)
                {
                    //RX
                    s_rfid_setRC531IntFlg(0, RC531_IRQ_RX);
                    D2(LABLE(0xF4););
                }
                if(ucIrqPending & RC531_IRQ_HI)
                {
                    s_rfid_setRC531IntFlg(0, RC531_IRQ_HI);
                    D2(LABLE(0xF5););
                }
                // ������ʱ�Ż��ȡ����Ĵ���
                EI_mifs_vReadReg(1, EM_mifs_REG_ERRORFLAG, (uchar *) &EG_mifs_tWorkInfo.ucErrFlags);
                EG_mifs_tWorkInfo.ucErrFlags &= RC531_ALLERR;
                D2(DATAIN(EG_mifs_tWorkInfo.ucErrFlags););
                if(EG_mifs_tWorkInfo.ucErrFlags & RC531_COLLERR)
                {
                    //col���� ֻ�з���ײ����,�������ͻ ��ȡ��ͻλ��
                    if ((EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL1)
                        || (EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL2)
                        || (EG_mifs_tWorkInfo.ucAnticol == EM_mifs_PICC_ANTICOLL3))
                    {
                        EI_mifs_vReadReg(1, EM_mifs_REG_COLLPOS, (uchar *) & EG_mifs_tWorkInfo.ucCollPos);
                    }
                    if(EG_mifs_tWorkInfo.ucnBits == 7)
                    {
                        //��ͻ��⣬��λ��Ϊ7, �������żУ���
                        EG_mifs_tWorkInfo.ucErrFlags &= (~RC531_PARITYERR);
                    }
                }
                EG_mifs_tWorkInfo.ucSaveErrState |= EG_mifs_tWorkInfo.ucErrFlags;
                EI_mifs_vReadReg(1, EM_mifs_REG_FIFOLENGTH, &ucTempData);
                if(ucTempData != 0)
                {
                    D2(DATAIN(ucTempData););
                    ucTempData = (uchar)MIN((ulong)ucTempData,(EM_mifs_BUFFER_LEN - EG_mifs_tWorkInfo.ulBytesReceived));
                    EI_mifs_vReadReg(ucTempData,EM_mifs_REG_FIFODATA,EG_mifs_tWorkInfo.aucBuffer + EG_mifs_tWorkInfo.ulBytesReceived);
                    EG_mifs_tWorkInfo.ulBytesReceived += ucTempData;
                }
                if(ucIrqPending & RC531_IRQ_RX)
                {
                    // ���յ����һ���ֽ�λ��
                    EI_mifs_vReadReg(1, EM_mifs_REG_SECONDARYSTATUS, &ucTempData);
                    ucTempData &= 0x07;
                    if (ucTempData != 0)
                    {
                        // ���һ���ֽ�û�н�����
                        EG_mifs_tWorkInfo.lBitsReceived +=
                            (EG_mifs_tWorkInfo.ulBytesReceived - 1) * 8 + ucTempData;
                        D2(LABLE(0xF7);DATAIN(ucTempData););
                    }
                    else
                    {
                        // ���һ���ֽڽ�����
                        EG_mifs_tWorkInfo.lBitsReceived += EG_mifs_tWorkInfo.ulBytesReceived * 8;
                    }
                }
            }
            if (ucIrqPending & RC531_IRQ_IDLE)
            {
                D2(LABLE(0xF8););
                s_rfid_setRC531IntFlg(0, RC531_IRQ_TIMER | RC531_IRQ_IDLE);
                s_rfid_setRC531Int(0, RC531_IRQ_TIMER);
                EG_mifs_tWorkInfo.ucINTSource &= ~(RC531_IRQ_TIMER);
                ucIrqPending &= ~(RC531_IRQ_TIMER);
            }
            if (ucIrqPending & RC531_IRQ_TIMER)
            {
                //��ʱ���ж�
                D1(LABLE(0xF0););
                EG_mifs_tWorkInfo.ucCurResult = EM_mifs_NOTAGERR;
                s_rfid_setRC531IntFlg(0, RC531_IRQ_TIMER);
            }
        }

#endif

    }
}
#endif
