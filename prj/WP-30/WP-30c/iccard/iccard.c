#include "wp30_ctrl.h"

#if (defined CFG_ICCARD)

/*-----------------------------------------------------------------------------}
 * �ⲿ�����ͱ���
 *-----------------------------------------------------------------------------{*/
extern volatile UINT64 gl_char_wait_etu;
extern volatile UINT64 gl_total_atr_etu;  //ATR���ճ�ʱʱ��
extern void icc_DisableIOInt(void);
/*-----------------------------------------------------------------------------}
 * ȫ�ֱ���
 *-----------------------------------------------------------------------------{*/
unsigned char gl_PpsMode;

/*-----------------------------------------------------------------------------}
 * ��������
 *-----------------------------------------------------------------------------{*/

/*-----------------------------------------------------------------------------}
 * ��������
 *-----------------------------------------------------------------------------{*/
void icc_InitIccModule(void)
{
	int i = 0;
	for(i = 0; i < CARD_SLOTNUM; i++)
	{
		memset((uchar *)&gl_CardSlotInfo[i], 0x00, sizeof(ICCARD_WORK_STRUCT));
	}

	memset((uchar *)&gl_CardInfo, 0x00, sizeof(ICCWorkInfo));
    memset(&gIccHalInfo,0,sizeof(gIccHalInfo));
	get_iccard_Ver();  //��ȡIC���汾
    TRACE("\r\n---Ver:%d chip:%d samslot:%d-%d---",gIccHalInfo.version,gIccHalInfo.chiptype,
          gIccHalInfo.samslots_inside,gIccHalInfo.samslots_outside);
	icc_SamCardInit();
	icc_CpuCardInit();
//	icc_init_etu_timer();
//    icc_close_etu_timer();
//	icc_init_inter_timer();
//    icc_disable_inter_timer();
	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;
	icc_CheckInSlot(USERCARD);
}

int icc_if_cardslot(int *CardSlot)
{
//    if ( *CardSlot == USERCARD ) {
//        // ��֧�ִ�
////		return (-ICC_ERRORSLOT);
//    } else if(*CardSlot > (gIccHalInfo.samslots_inside+gIccHalInfo.samslots_outside)){
//		return (-ICC_ERRORSLOT);
//    }
	if(*CardSlot < USERCARD
       || *CardSlot > (gIccHalInfo.samslots_inside+gIccHalInfo.samslots_outside))
	{
		return (-ICC_ERRORSLOT);
	}
	return 0;
}

int icc_InitModule(int CardSlot, uint CardVol, uint CardMode)
{
    uint tmp;
	if(icc_if_cardslot(&CardSlot))
	{
		return (-ICC_ERRORSLOT);
	}
	if(CardSlot == USERCARD)
	{
        if((CardVol != VCC_5) && (CardVol != VCC_3) && (CardVol != VCC_18))
        {
            return (-ICC_ERRPARA);
        }
	}
	else
	{
		if(CardVol != VCC_5)
		{
			return (-ICC_ERRPARA);
		}
	}
	tmp = LBYTE(CardMode);
    if ( tmp > CITYUNIONMODE ) {
		return (-ICC_ERRPARA);
    }
	if(tmp != ISOMODE && tmp != CITYUNIONMODE)
	{
		if(HBYTE(CardMode) > 0)
		{
			return (-ICC_ERRPARA);
		}
	}
	gl_CardSlotInfo[CardSlot].VolMode = LBYTE(CardVol);
	gl_CardSlotInfo[CardSlot].PortMode = tmp;
    gl_CardSlotInfo[CardSlot].BaudRate = HBYTE(CardMode);
	gl_CardSlotInfo[CardSlot].ExChangeBaudRate = H2BYTE(CardMode);
    if ( IFBIT(CardMode,31) ) {
		gl_CardSlotInfo[CardSlot].BaudRateClassFlag = 1;
	}
	else
	{
		gl_CardSlotInfo[CardSlot].BaudRateClassFlag = 0;
	}
    if ( IFBIT(CardMode,30) ) {
		gl_CardSlotInfo[CardSlot].OnlyAtrFlag = 1;
	}
	else
	{
		gl_CardSlotInfo[CardSlot].OnlyAtrFlag = 0;
	}
	return ICC_SUCCESS;
}

int icc_CheckInSlot(int CardSlot)
{
	if(icc_if_cardslot(&CardSlot))
	{
		return (-ICC_ERRORSLOT);
	}
    return icc_checkCardInsert(CardSlot);
}

int icc_CheckVcc(int CardSlot)
{
	if(icc_if_cardslot(&CardSlot))
	{
		return (-ICC_ERRORSLOT);
	}
#ifdef __SHB_TEST_
	if(gl_CardSlotInfo[CardSlot].PortMode == SHBMODE)
	{
		if(CardSlot != USERCARD)
		{
			if(!gl_CardSlotInfo[CardSlot].CardPowerOn)
			{
				return ICC_SUCCESS;
			}
		}
	}
#endif
	if(!gl_CardSlotInfo[CardSlot].CardPowerOn)
	{
		return (-ICC_CARDNOPOWER);
	}
	return ICC_SUCCESS;
}

//���ÿ��ߵĿ�����1��������λʱ�򣬽����������µ�
void icc_PowerdownOtherSlot(int CardSlot)
{
    int samslotmax = (SAM1SLOT+gIccHalInfo.samslots_inside);
    if((CardSlot > USERCARD) && (CardSlot <= samslotmax))
    {
        for(CardSlot=SAM1SLOT;CardSlot<samslotmax;CardSlot++)
        {
            //D1(TRACE("\r\nOtherSlot:%d",CardSlot););
            gl_CardSlotInfo[CardSlot].CardPowerOn = 0;
        }
    }
}

int icc_lpwr_switch(int cardslot,int val)
{
    val = (val?1:0);
    switch (cardslot)
    {
    case USERCARD:
        gSystem.lpwr.bm.iccard = val;
        break;
    case SAM1SLOT:
        gSystem.lpwr.bm.sam1 = val;
        break;
    case SAM2SLOT:
        gSystem.lpwr.bm.sam2 = val;
        break;
    default:
        break;
    }
    return 0;
}

int icc_Reset(int CardSlot,uint *pAtrLen,uchar *pAtrData)
{
	int Ret;
	int Cold_Ret=0;
    uchar rstbuf[256];
    uint i,j;
	if((pAtrLen == NULL) || (pAtrData == NULL))
	{
		return (-ICC_ERRPARA);
	}
	if(icc_if_cardslot(&CardSlot))
	{
		return (-ICC_ERRORSLOT);
	}
	if(!gl_CardSlotInfo[CardSlot].VolMode)
	{
		return (-ICC_ERRPARA);
	}
	Ret = icc_checkCardInsert(CardSlot);
    if(Ret)
    {
		return (-ICC_CARDOUT);
    }
    if ( CardSlot == USERCARD) {
        //ͬ������ʹ��
        if (icc_get_usercard_workmode() == 1) {
            return (-ICC_ERRMODE);
        }
    }
	icc_SelectSlot(CardSlot);
	s_InitCardInfo();
	icc_InitICCTiming();
	icc_init_etu_timer();
	icc_init_inter_timer();
    memset(rstbuf, 0x00, sizeof(rstbuf));
	icc_PowerdownOtherSlot(CardSlot);
LABLE(D_COLD);
    /*�临λһ��	*/
    Ret = icc_ColdReset(rstbuf);
    gl_CardSlotInfo[gl_CardInfo.current_slot].ResetStatus = 0x01;
#ifdef CFG_DBG_ICCARD 
DATAIN(LBYTE(Ret));
if ( gucDebugm == 0x55 ) {
    Ret = ATR_TA1ERR;
    InkeyCount(0);
}
#endif
    /* ������ɹ����ȸ�λһ��  */
    if(Ret)
    {
    	 //�籣���ȸ�λû��ʱ���ر�־
    	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
    	{
    		Cold_Ret = Ret;
    	}
   		/* ����TS���󣬳�ʱ��żУ�顢TCK�������������д����ͷ����� */
		if((Ret != ATR_TSERR)
			&&(Ret != ATR_TIMEOUT)
	        &&(Ret != ATR_PARERR)
	        &&(Ret != ATR_TCKERR))
		{
LABLE(D_WARM);
			sys_delay_ms(100);
			Ret = icc_WarmReset(rstbuf);
#ifdef DEBUG_Dx
DATAIN(Ret);
#endif
			gl_CardSlotInfo[gl_CardInfo.current_slot].ResetStatus = 0x02;
		}
    }
    if(Ret)
    {
    	//�ͷ�
		/*  ����Բ��ɹ���������˳� */
		if(Ret == ATR_TSERR)
		{
			s_DelayUs(120);
		}
       	icc_Close(CardSlot);
		if((Ret == ATR_PARERR) || (Ret == ATR_TCKERR))
        {
            return (-ICC_COMMERR);
        }
        else if(Ret == ATR_TIMEOUT)
        {
        	//�籣���ȸ�λû��ʱ���ر�־
        	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
    		{
    			if((Cold_Ret == ATR_PARERR) || (Cold_Ret == ATR_TCKERR))
    			{
					return (-ICC_COMMERR);
				}
				else if(Cold_Ret == ATR_TIMEOUT)
				{
					return (-ICC_TIMEOUT);
				}
				else
				{
					return (-ICC_RESPERR);			// ��������
				}
        	}
			else
			{
            	return (-ICC_TIMEOUT);
			}
        }

        return (-ICC_RESPERR);			// ��������
    }

    memcpy(pAtrData, rstbuf+1, rstbuf[0]);  /* RSTBUF��ʽ������+TS+TO+TA......  */
    *pAtrLen = rstbuf[0];

    gl_CardSlotInfo[gl_CardInfo.current_slot].T = 0;

    gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = 0x00;

    if((gl_CardSlotInfo[gl_CardInfo.current_slot].TD1&0x0F) == 0x01)
    {
        gl_CardSlotInfo[gl_CardInfo.current_slot].T = 1;
    }
    else if((gl_CardSlotInfo[gl_CardInfo.current_slot].TD1&0x0F) == 0x00)
    {
        gl_CardSlotInfo[gl_CardInfo.current_slot].T = 0;
    }
    else
    {
    	//�ͷ�
        icc_Close(gl_CardInfo.current_slot);
		return (-ICC_RESPERR);                    /* ����Э���趨����  */
    }
    gl_CardSlotInfo[gl_CardInfo.current_slot].CardPowerOn = 0x01; /* ����ʼ���ɹ���IC���ϵ��ʶ */
	//2010-01-16 yehf���� ATR�󱣻�ʱ�� 2.56ms
	s_DelayMs(1);
	icc_adjustTiming();
	// ����֧��PPSЭ��Ŀ�Ƭ�����ȷ���PPSЭ�����������
//D2(TRACE("\r\nPPSENABLE = %d", gl_CardSlotInfo[gl_CardInfo.current_slot].PPSENABLE););
	if(gl_CardSlotInfo[CardSlot].OnlyAtrFlag == 1)
	{
		goto atrover;
	}
    if(gl_CardSlotInfo[gl_CardInfo.current_slot].PPSENABLE)
    {
		icc_DisableOtherInt();
		if(gl_CardSlotInfo[gl_CardInfo.current_slot].BaudRateClassFlag == 1)
		{
		    Ret = icc_PPSSelect(gl_PpsMode);
		}
		else
		{
        	Ret = icc_PPSRequest();
		}
		icc_EnableOtherInt();
		if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
		{
			if(Ret == -ICC_TIMEOUT)
			{
				Ret = icc_ColdReset(rstbuf);
				if(Ret == 0)
				{
                    i = (uint)gl_CardSlotInfo[gl_CardInfo.current_slot].TA1;
					gl_CardSlotInfo[gl_CardInfo.current_slot].D = (uchar)D_Table[(uint)LHALFB(i)];
                    j = gl_CardSlotInfo[gl_CardInfo.current_slot].D;
					gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = (uchar)F_Table[(uint)HHALFB(i)] / j;
					if(i == 0x14)
					{
						gl_CardSlotInfo[gl_CardInfo.current_slot].ETU -=1;
					}
                    i = (uint)gl_CardSlotInfo[gl_CardInfo.current_slot].TC2;
					gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = ((960 * i * i) + 1);
					icc_adjustTiming();
				}
			}

		}
        if(Ret)
        {
        	//�ͷ�
			icc_Close(CardSlot);
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
			{
				if((Ret == ATR_PARERR) || (Ret == ATR_TCKERR))
		        {
		            return (-ICC_COMMERR);
		        }
		        else if(Ret == ATR_TIMEOUT)
		        {
		           return (-ICC_TIMEOUT);
		        }

				return (-ICC_RESPERR);			// ��������
			}
			else
			{
				return Ret;
			}
        }
    }

    // T=1Э�鿨Ƭ���ȷ���IFSD����
    if(gl_CardSlotInfo[gl_CardInfo.current_slot].T)
    {
		/*Ԥ�ô�IC�����յ�I����ȷ��PCB�����֣�˳�����1*/
        gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB = 0x40;
        /* ����IFSD���� */
        Ret = icc_ifsdCmd();
        if(Ret)
        {
        	//�ͷ�
            icc_Close(gl_CardInfo.current_slot);
        }

		if((Ret == T1_CWTERR) || (Ret == T1_BWTERR))
        {
			return (-ICC_TIMEOUT);
        }
        else if(Ret != ICC_SUCCESS)
        {
			return (-ICC_RESPERR);
        }
    }
atrover:
	//�ͷ�
	s_DelayMs(1);
	// 2011-07-14 ��λ��Ϣ����1B����T=0/1��Ϣ
	pAtrData[*pAtrLen] = gl_CardSlotInfo[gl_CardInfo.current_slot].T;
	*pAtrLen += 1;
    icc_lpwr_switch(CardSlot,1);
    return ICC_SUCCESS;
}

int icc_ExchangeData(int CardSlot, int sendlen, uchar *psenddata, int *precvlen, uchar *precvdata)
{
	int Ret = 0;
	unsigned char BaudRate = 0;
	if((psenddata == NULL) || (precvlen == NULL) || (precvdata == NULL))
	{
		return (-ICC_ERRPARA);
	}
	if(icc_if_cardslot(&CardSlot))
	{
		return (-ICC_ERRORSLOT);
	}
	if(!gl_CardSlotInfo[CardSlot].CardPowerOn)
	{
		return (-ICC_CARDNOPOWER);
	}
	Ret = icc_checkCardInsert(CardSlot);
    if(Ret) {
		return (-ICC_CARDOUT);
    }
	icc_SelectSlot(CardSlot);
	//D1(TRACE("\r\ExChangeBaudRate:%x",gl_CardSlotInfo[gl_CardInfo.current_slot].ExChangeBaudRate););
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].ExChangeBaudRate > 0)
	{
		BaudRate = gl_CardSlotInfo[gl_CardInfo.current_slot].ExChangeBaudRate - 1;
		switch(BaudRate)
		{
			case B9600:
				gl_CardSlotInfo[gl_CardInfo.current_slot].D = 1;
				gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 372;
				break;
			case B19200:
				gl_CardSlotInfo[gl_CardInfo.current_slot].D = 2;
				gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 186;
				break;
			case B38400:
				gl_CardSlotInfo[gl_CardInfo.current_slot].D = 4;
				gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 93;
				break;
			case B55800:
				gl_CardSlotInfo[gl_CardInfo.current_slot].D = 8;
				gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 64;
				break;
			case B115200:
				gl_CardSlotInfo[gl_CardInfo.current_slot].D = 8;
				gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 46;
				break;
			default:
				gl_CardSlotInfo[gl_CardInfo.current_slot].D = 1;
				gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 372;
				break;
		}
	}
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode != SHBMODE)
	{
		if(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU < 32)
		{
			return (-ICC_ERRPARA);
		}
	}
	icc_adjustTiming();
	icc_init_etu_timer();
	icc_init_inter_timer();
	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0;

	*precvlen = 0;
//    TRACE_BUF("send",psenddata,sendlen);
	Ret = inter_icc_exchange(sendlen, psenddata, (uint*)precvlen, precvdata);
//    if ( !Ret ) {
//        TRACE_BUF("recv",precvdata,*precvlen);
//    }
	if(Ret)
	{
		//�ͷ�
		icc_Close(gl_CardInfo.current_slot);
		if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
		{
			if(Ret == T0_MORESENDERR)
			{
				return (-ICC_RESPERR);
			}
			else if(Ret == T0_PARERR)
			{
				return (-ICC_RESPERR);
			}
		}
		if((Ret == T0_TIMEOUT)||(Ret == T1_BWTERR)||(Ret == T1_CWTERR))
		{
			return (-ICC_TIMEOUT);
		}
		else if((Ret == T0_MORESENDERR) || (Ret == T0_PARERR) || (Ret == T1_PARITYERR) || (Ret == T1_EDCERR))
		{
			return (-ICC_COMMERR);
		}
		else
		{
			return (-ICC_RESPERR);
		}
	}
	return Ret;
}

int icc_Close(int CardSlot)
{
	int Ret=0;
	if(icc_if_cardslot(&CardSlot))
	{
		return (-ICC_ERRORSLOT);
	}
	icc_SelectSlot(CardSlot);
	gl_CardSlotInfo[CardSlot].CardPowerOn = 0;
	if(gl_CardSlotInfo[CardSlot].PortMode == SHBMODE)
	{
		//�籣���жϿ��Ƿ���λ��
		Ret = icc_checkCardInsert(CardSlot);
        if(Ret)
        {
            Ret = (-ICC_CARDOUT);
        }
	}
    s_DelayMs(5);//�ӳ�5ms ��ֹ̫���µ� 10202014 chenf  1719xy==31 10092015chenf
    // yehf �õͳ���IO�ж�
    // �޸�Ϊ������1���ֽں�ر��ж�
//    icc_DisableIOInt();
    icc_ClrVCC();
    icc_ClrRST();
    icc_close_clk();
    icc_SetIOOutput();
    icc_ClrIO();
    icc_ControlLowpower(ON);
    icc_lpwr_switch(CardSlot,0);
    return Ret;
}

//���ICģ����Ϣ
int icc_GetInfo(int *ver,int *maxslot)
{
	*ver = gIccHalInfo.version;
	*maxslot = gIccHalInfo.samslots_inside+gIccHalInfo.samslots_outside;
	return 0;
}


#ifdef DEBUG_Dx
void ShowCardSlotInfo(uint ucSlotNo)
{
#if 0
	//int i;
	TRACE("\n----gl_CardSlotInfo[%d]------",ucSlotNo);
	TRACE("\n Vcc:%d Mode:%d PowerOn:%d Reset:%d CardIn:%d T:%d D:%d Etu:%d",
		gl_CardSlotInfo[ucSlotNo].VolMode,gl_CardSlotInfo[ucSlotNo].PortMode,gl_CardSlotInfo[ucSlotNo].CardPowerOn,
		gl_CardSlotInfo[ucSlotNo].ResetStatus,gl_CardSlotInfo[ucSlotNo].CardInsert,gl_CardSlotInfo[ucSlotNo].T,
		gl_CardSlotInfo[ucSlotNo].D,gl_CardSlotInfo[ucSlotNo].ETU);

	TRACE("\n TS:%02X T0:%02X",gl_CardSlotInfo[ucSlotNo].TS,gl_CardSlotInfo[ucSlotNo].T0);
	if(gl_CardSlotInfo[ucSlotNo].TAFlag)
	{
		TRACE("\n");
		if(gl_CardSlotInfo[ucSlotNo].TAFlag & 0x01)
		{
			TRACE("TA1:%02X ",gl_CardSlotInfo[ucSlotNo].TA1);
		}
		if(gl_CardSlotInfo[ucSlotNo].TAFlag & 0x02)
		{
			TRACE("TA2:%02X ",gl_CardSlotInfo[ucSlotNo].TA2);
		}
		if(gl_CardSlotInfo[ucSlotNo].TAFlag & 0x04)
		{
			TRACE("TA3:%02X ",gl_CardSlotInfo[ucSlotNo].TA3);
		}
		if(gl_CardSlotInfo[ucSlotNo].TAFlag & 0x08)
		{
			TRACE("TA4:%02X ",gl_CardSlotInfo[ucSlotNo].TA4);
		}
	}
	if(gl_CardSlotInfo[ucSlotNo].TBFlag)
	{
		TRACE("\n");
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x01)
		{
			TRACE("TB1:%02X ",gl_CardSlotInfo[ucSlotNo].TB1);
		}
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x02)
		{
			TRACE("TB2:%02X ",gl_CardSlotInfo[ucSlotNo].TB2);
		}
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x04)
		{
			TRACE("TB3:%02X ",gl_CardSlotInfo[ucSlotNo].TB3);
		}
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x08)
		{
			TRACE("TB4:%02X ",gl_CardSlotInfo[ucSlotNo].TB4);
		}
	}
	if(gl_CardSlotInfo[ucSlotNo].TCFlag)
	{
		TRACE("\n");
		if(gl_CardSlotInfo[ucSlotNo].TCFlag & 0x01)
		{
			TRACE("TC1:%02X ",gl_CardSlotInfo[ucSlotNo].TC1);
		}
		if(gl_CardSlotInfo[ucSlotNo].TCFlag & 0x02)
		{
			TRACE("TC2:%02X ",gl_CardSlotInfo[ucSlotNo].TC2);
		}
		if(gl_CardSlotInfo[ucSlotNo].TCFlag & 0x04)
		{
			TRACE("TC3:%02X ",gl_CardSlotInfo[ucSlotNo].TC3);
		}
		if(gl_CardSlotInfo[ucSlotNo].TCFlag & 0x08)
		{
			TRACE("TC4:%02X ",gl_CardSlotInfo[ucSlotNo].TC4);
		}
	}
	if(gl_CardSlotInfo[ucSlotNo].TDFlag)
	{
		TRACE("\n");
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x01)
		{
			TRACE("TD1:%02X ",gl_CardSlotInfo[ucSlotNo].TD1);
		}
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x02)
		{
			TRACE("TD2:%02X ",gl_CardSlotInfo[ucSlotNo].TD2);
		}
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x04)
		{
			TRACE("TD3:%02X ",gl_CardSlotInfo[ucSlotNo].TD3);
		}
		if(gl_CardSlotInfo[ucSlotNo].TBFlag & 0x08)
		{
			TRACE("TD4:%02X ",gl_CardSlotInfo[ucSlotNo].TD4);
		}
	}
	/*
	if(gl_CardSlotInfo[ucSlotNo].TiFlag[0])
	{
		TRACE("\n");
		for(i=0;i<4;i++)
		{
			if(gl_CardSlotInfo[ucSlotNo].TiFlag[0] & (1<<i))
			{
				TRACE("TA%d:%02X ",i+1,gl_CardSlotInfo[ucSlotNo].TA[i]);
			}
		}
	}
	if(gl_CardSlotInfo[ucSlotNo].TiFlag[1])
	{
		TRACE("\n");
		for(i=0;i<4;i++)
		{
			if(gl_CardSlotInfo[ucSlotNo].TiFlag[1] & (1<<i))
			{
				TRACE("TB%d:%02X ",i+1,gl_CardSlotInfo[ucSlotNo].TB[i]);
			}
		}
	}
	if(gl_CardSlotInfo[ucSlotNo].TiFlag[2])
	{
		TRACE("\n");
		for(i=0;i<4;i++)
		{
			if(gl_CardSlotInfo[ucSlotNo].TiFlag[2] & (1<<i))
			{
				TRACE("TC%d:%02X ",i+1,gl_CardSlotInfo[ucSlotNo].TC[i]);
			}
		}
	}
	if(gl_CardSlotInfo[ucSlotNo].TiFlag[3])
	{
		TRACE("\n");
		for(i=0;i<4;i++)
		{
			if(gl_CardSlotInfo[ucSlotNo].TiFlag[3] & (1<<i))
			{
				TRACE("TD%d:%02X ",i+1,gl_CardSlotInfo[ucSlotNo].TD[i]);
			}
		}
	}
	TRACE("\n N:%d PPS:%d",gl_CardSlotInfo[ucSlotNo].N,gl_CardSlotInfo[ucSlotNo].PPSENABLE);
	*/
	TRACE("\n IFSC:%d IFSD:%d WWT:%d CWT:%d BWT:%d BWT_tmp:%d GBT:%d",gl_CardSlotInfo[ucSlotNo].IFSC,gl_CardSlotInfo[ucSlotNo].IFSD,gl_CardSlotInfo[ucSlotNo].WWT,
		gl_CardSlotInfo[ucSlotNo].CWT,gl_CardSlotInfo[ucSlotNo].BWT,gl_CardSlotInfo[ucSlotNo].BWT_TEMP,gl_CardSlotInfo[ucSlotNo].BGT);
	TRACE("\n SW1:0x%2X SW2:0x%2X",gl_CardSlotInfo[ucSlotNo].sw1,gl_CardSlotInfo[ucSlotNo].sw2);
	TRACE("\n TermPCB:0x%02X CardPCB:0x%02X",gl_CardSlotInfo[ucSlotNo].TERM_PCB,gl_CardSlotInfo[ucSlotNo].CARD_PCB);
#endif
}

void Show_CardInfo(void)
{
#if 0
	int i;
	uint *p;
	p = (uint *)&gl_CardInfo.Rec_P15;
	TRACE("\n--gl_CardInfo %d--",gl_CardInfo.current_slot);
	TRACE("\nRecvP:");
	TRACE("\n13:");
	for(i=0;i<13;i++)
	{
		TRACE("%4d ",i);
	}
	TRACE("\n   ");
	for(i=0;i<13;i++)
	{
		TRACE("%04d ",*(p++));
	}
	TRACE("\nSendP:");
	TRACE("\n12:");
	for(i=0;i<12;i++)
	{
		TRACE("%4d ",i);
	}
	TRACE("\n   ");
	for(i=0;i<12;i++)
	{
		TRACE("%04d ",*(p++));
	}
	TRACE("\nComErr:%X ErrPar:%X ErrTimeOut:%X Check_Enable:%X",gl_CardInfo.k_IccComErr,gl_CardInfo.k_IccErrPar,gl_CardInfo.k_IccErrTimeOut,gl_CardInfo.Parity_Check_Enable);
	TRACE("\nCharWait:%d ATR_TotalTime:%d TotalATRTime:%d",gl_CardInfo.k_Ic_CharWait_TimeCount,gl_CardInfo.k_Ic_ATR_TotalTimeCount,gl_CardInfo.k_IC_TotalATRTime);
	TRACE("\nRecvWait:%d WWT:%d BWT:%d CWT:%d",gl_CardInfo.k_RecvFirst,gl_CardSlotInfo[gl_CardInfo.current_slot].WWT
		,gl_CardSlotInfo[gl_CardInfo.current_slot].BWT,gl_CardSlotInfo[gl_CardInfo.current_slot].CWT);
	TRACE("\n---------------------------------------");
#endif
}
#endif

#endif

