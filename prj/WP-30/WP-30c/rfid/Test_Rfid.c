/*********************************************************************
* ��Ȩ���� (C)2009,
* �ļ����ƣ�
*
* ��ǰ�汾��
*     1.0
* ����ժҪ��
*
* ��ʷ��¼��
*     �޸���		 ����		    	 �汾��       �޸ļ�¼
*******************************************************************************/

#include "wp30_ctrl.h"

#if 0//#ifdef DEBUG_Dx

#ifdef CFG_RFID

/**********************************************************************
*
*
*  �ⲿ�������ⲿ��������
*
*
***********************************************************************/
extern void test_pboc_single(void);
extern int test_rfid_para_menu(int mode);
/**********************************************************************
*
*
*   ���Ժ궨��
*
*
***********************************************************************/

/**********************************************************************
*
*
*   �궨��
*
*
***********************************************************************/



/**********************************************************************
*
*
*ȫ�ֱ�������:
*
*
***********************************************************************/
/**********************************************************************
*    ��������:
*
*
***********************************************************************/
// ��־ F0~FE
#if 0
const uchar gKeybuf[] = {
	0x11, 0x22, 0x33, 0x55,
	0x66, 0x77, 0x88, 0x99, 0xAA,
    0xBB, 0xCC, 0xDD, 0xEE
};
void vDispBufKey(uint uiLen, uchar *ucBuf)
{
	uint i,j,k;
	TRACE("\r\n--------%d---------\r\n",uiLen);
	for(i=0;i<uiLen;i++)
	{
		k = 0;
		for(j=0;j<sizeof(gKeybuf);j++)
		{
			if(gKeybuf[j] == ucBuf[i] && gKeybuf[j] == ucBuf[i+1])
			{
				k = 1;
				if(gKeybuf[j] == 0xCC)
				{
					TRACE("\r\n------------");
				}
			}
		}
		if(k)
		{
			TRACE("\r\n%02X: ",ucBuf[i]);
			++i;
		}
		else
		{
			TRACE("%02X ",ucBuf[i]);
		}
	}
	TRACE("\r\n");
}

void vDispBuf(uint uiLen, uint Mode, uchar *ucBuf)
{
	uint i;
	TRACE("\r\n--------%d---------",uiLen);
	for(i=0;i<uiLen;i++)
	{
		if(i%32==0)
		{
			TRACE("\r\n%04X:",i);
		}
		else if(i%8==0)
		{
			TRACE(" ");
		}
		if(Mode == 1)
		{
			TRACE(" %02d",ucBuf[i]);
		}
		else if(Mode == 2)
		{
			TRACE(" %c",ucBuf[i]);
		}
		else
		{
			TRACE(" %02X",ucBuf[i]);
		}
	}
	TRACE("\r\n");
}
#endif
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void show_Rfid_WorkInfo(void)
{
	ET_mifs_WORKSTRUCT *p;
	p = &EG_mifs_tWorkInfo;
	TRACE("\r\n ------EG_mifs_tWorkInfo:%2X-%2X-------", p->ucMifCardType, p->ucCurType);
	TRACE("\r\n send:%ld-%ld recv:%ld-%ld  CardID:%ld FrameMaxSize:%ld", p->ulSendBytes,
		p->ulBytesSent, p->ulBytesReceived, p->lBitsReceived, p->ulCardID, p->ulMaxLen);
	TRACE("\r\n Pos:%02X Anti:%02X bits:%02X DF:%02X", p->ucCollPos, p->ucAnticol, p->ucnBits,
		p->ucDisableDF);
	TRACE("\r\n Stat:%02X INT:%02X Err:%02X SaveErr:%02X", p->ucCurResult, p->ucINTSource,
		p->ucErrFlags, p->ucSaveErrState);
	TRACE("\r\n EnableTransfer:%02X FirstOpenFlag:%02X CurType:%02X CurPCB:%02X",
		p->ucEnableTransfer, p->ucFirstOpenFlag, p->ucCurType, p->ucCurPCB);
	TRACE("\r\n Pwr:%02X CID:%02X FWI:%02X", p->ucPwrLevel, p->ucCIDSupport, p->ucFWI);
	TRACE("\r\n SAK: %02X %02X %02X", p->ucSAK1, p->ucSAK2, p->ucSAK3);
	TRACE("\r\n ActivateFlag:%2X ATQA:%2X-%2X UIDLen:%d", p->ucMifActivateFlag, p->ucATQA[0],
		p->ucATQA[1], p->ucUIDLen);
	TRACE("\r\n ucUIDCL1:");
	DISPBUF(p->ucUIDCL1, sizeof(p->ucUIDCL1), 0);
	TRACE("\r\n ucUIDCL2:");
	DISPBUF(p->ucUIDCL2, sizeof(p->ucUIDCL2), 0);
	TRACE("\r\n ucUIDCL3:");
	DISPBUF(p->ucUIDCL3, sizeof(p->ucUIDCL3), 0);
	TRACE("\r\n UIDB:%02X %02X %02X %02X ATQB: ", p->ucUIDB[0], p->ucUIDB[1], p->ucUIDB[2],
		p->ucUIDB[3]);
	DISPBUF(p->ucATQB, sizeof(p->ucATQB), 0);
	TRACE("\r\n SFGI:%2X SFGT:%2X FWT:%2X FWTTmp:%2X ", p->ucSFGI, p->ulSFGT, p->ulFWT,
		p->ulFWTTemp);
	TRACE("\r\n CIDFlg:%02X NADFlag:%02X WTX:%02X AnticollFlag:%02X FSC:%02X FSD:%02X",
		p->ucCIDFlag, p->ucNADFlag, p->ucWTX, p->ucAnticollFlag, p->FSC, p->FSD);
	TRACE("\r\n ------------------------------------------------");
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void Test_RC531(void)
{
	uint i, j, k;
	uchar ucBuf[64],addr[64];
	guiDebugj = 20;


	while (1)
	{
		TRACE("\r\n-------------RC531 Test------------------");
		TRACE("\r\n 1-HalInit  2-SPI_SendCommand  3-close  4-ReadReg 5-WriteReg 6-SetRegBit 7-ClrRegBit   ");
		TRACE("\r\n 8-DispReg  9-RC531CS 10-�л�������   12-s_rfid_ReadReg");
		TRACE("\r\n IN:");
		switch (InkeyCount(0))
		{
		case 1:
			s_Rfid_vHalInit();
			break;
		case 2:
			ucBuf[0] = 0x55;
			SPI_SendCommand(RFID_SPIn,SPIBUS0_RFID,&ucBuf[0],1,&ucBuf[1]);
			break;
		case 3:
			TRACE("\r\n guiDebugk:%X",guiDebugk);
			guiDebugk = InkeyHex(0);
			break;
		case 4:
			TRACE("\r\n addr:");
			j = InkeyHex(0);
			SETSIGNAL1_H();
			EI_mifs_vReadRegSPI(1, j, &ucBuf[0]);
			SETSIGNAL1_L();
			TRACE("\r\n  read:0x%02X", ucBuf[0]);
			DISP_DEBUGBUF();
			break;
		case 5:
			TRACE("\r\n addr:");
			j = InkeyHex(0);
			TRACE("\r\n Data:");
			k = InkeyHex(0);
			SETSIGNAL1_H();
			EI_mifs_vWriteRegSPI(1, j, (uchar *) & k);
			SETSIGNAL1_L();
			EI_mifs_vReadRegSPI(1, j, ucBuf);
			TRACE("\r\n0x%02X-0x%02X", k, ucBuf[0]);
			break;
		case 6:
			TRACE("\r\n addr:");
			j = InkeyHex(0);
			TRACE("\r\n bit(0~7):");
			k = InkeyCount(0);
			EI_mifs_vReadRegSPI(1, j, &ucBuf[0]);
			EI_mifs_vSetBitMaskSPI(j, k);
			EI_mifs_vReadRegSPI(1, j, &ucBuf[1]);
			TRACE("\r\n0x%02X-0x%02X", ucBuf[0], ucBuf[1]);
			break;
		case 7:
			TRACE("\r\n addr:");
			j = InkeyHex(0);
			TRACE("\r\n bit(0~7):");
			k = InkeyCount(0);
			EI_mifs_vReadRegSPI(1, j, &ucBuf[0]);
			EI_mifs_vClrBitMaskSPI(j, k);
			EI_mifs_vReadRegSPI(1, j, &ucBuf[1]);
			TRACE("\r\n0x%02X-0x%02X", ucBuf[0], ucBuf[1]);
			break;
		case 8:
			TRACE("\r\n all reg:");
			CLRBUF(ucBuf);
			for (i = 0; i < 0x40; i++)
			{
				EI_mifs_vReadRegSPI(1, i, &ucBuf[i]);
			}
			DISPBUF(ucBuf, i, 0);
			break;
		case 9:
			TRACE("\r\n 0-�ر�CS 1-����CS ");
			SPI_SetCS(RFID_SPIn,SPIBUS0_RFID,InkeyCount(0));
			break;
		case 10:

			break;
		case 11:
			s_Rfid_vHalInit();
			break;
		case 12:
			TRACE("\r\n ����:");
			i = InkeyCount(0);
			for(j=0;j<i;j++)
			{
				TRACE("\r\n ��ַ:");
				addr[j] = InkeyHex(0);
			}
			CLRBUF(ucBuf);
			s_rfid_ReadReg(i,addr,ucBuf);
			DISPBUF(addr,i,0);
			DISPBUF(ucBuf,i,0);
			break;
		case 99:

			return;
		}
	}
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void TypeACircleTest(void)
{
	uint ret, j, i = 0,time=500;
	TRACE("\r\n ���õ���Ѱ��ʱ��(ms):");
	j = InkeyCount(0);
	if(j == 0)
		time = 500;
	TRACE("\r\nTypeAѭ����������...");
	EA_ucMifsClose();
	ret = EA_ucMifsOpen(EM_mifs_TYPEA);
	if (!ret)
	{
		TRACE("\r\n�ɹ�!�������ʼѰ��....");
		InkeyCount(0);
		i = 0;
		while (1)
		{
			j = 0;
			if (IfInkey(0))
			{
				TRACE("\r\n 99-�˳� ");
				if (InkeyCount(0) == 99)
				{
					break;
				}
			}
			TRACE("\r\n----ѰA��%d...", ++i);
			ret = EA_ucMIFRequest(0, EM_mifs_HALT, (ushort *) & j);
			if (!ret)
			{
				TRACE("�ɹ�:TagType%d", j);
				TRACE("\r\n��ȡ����..." );
				ret = EA_ucMIFAntiColl(0,(ulong *)&j);
				if (!ret)
				{
					TRACE("����:%08X", j);
					sys_beep();
				}
				else
				{
					TRACE("ʧ��%d",ret);
				}
				EA_ucMIFHalt(0);
			}
			else
			{
				TRACE("ʧ��:%d", ret);
			}
			s_DelayMs(time);
		}
	}
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void TypeBCircleTest(void)
{
	uint ret, j, i = 0,time=500;
	uchar ucBuf[64];
	TRACE("\r\n ���õ���Ѱ��ʱ��(ms):");
	j = InkeyCount(0);
	if(j == 0)
		time = 500;
	TRACE("\r\nTypeBѭ����������...");
	EA_ucMifsClose();
	ret = EA_ucMifsOpen(EM_mifs_TYPEB);
	if (!ret)
	{
		TRACE("\r\n�ɹ�!�������ʼѰ��....");
		InkeyCount(0);
		i = 0;
		while (1)
		{
			if (IfInkey(0))
			{
				TRACE("\r\n 99-�˳� ");
				if (InkeyCount(0) == 99)
				{
					break;
				}
			}
			j = 0;
			TRACE("\r\n--ѰB��:%d--", ++i);
			ret = EA_ucMIFRequestB(0, EM_mifs_HALT, EM_mifs_TYPEB_REQALL, 3, &j, ucBuf);
			if (!ret)
			{
				TRACE("�ɹ�");
				DISPBUF(ucBuf, j, 0);
				sys_beep();
				EA_ucMIFHaltB(0,(ulong)ucBuf);
			}
			else
			{
				TRACE("ʧ��:%d", ret);
			}

			s_DelayMs(time);
		}
	}
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/

void Test_MFResetCircle(void)
{
	uint ret, j, i = 0;
	uchar ucBuf[128];
MFResetCircle1:
	TRACE("\r\n��Ƶ������...");
	ret = EA_ucMifsOpen(EM_mifs_TYPEA);
	if (!ret)
	{
		TRACE("\r\n�ɹ�!");
	}
	else
	{
		TRACE("\r\n��Ƶģ������ʧ��%d!", ret);
		return;
	}

	if (!ret)
	{
		j = 0;
		while (1)
		{
			TRACE("\r\n--Reset:%d--", ++j);
			ret = EA_ucMIFResetPro(0, EM_mifs_HALT, &i, ucBuf);
			if (!ret)
			{
				TRACE("�ɹ�:%d", i);
				memcpy(ucBuf, "\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x10", 15);
				ret = EA_ucMIFExchangePro(0, 15, ucBuf, &i, &ucBuf[32]);
				//ret = EA_ucMIFExchangePro(0, 5 , "\x00\x84\x00\x00\x04", &i, &ucBuf[0]);
				if (!ret)
				{
					sys_beep();
				}
				else
				{
					j = 0;
					TRACE("\r\n ----Exchangeʧ��%d----", ret);
				}
				EA_ucMIFDeselectPro(0);
			}
			else
			{
				TRACE("ʧ��:%d", ret);
				j = 0;
			}
			if (IfInkey(0))
			{
				TRACE("\r\n 1-�˳� ");
				if (InkeyCount(0) == 1)
				{
					break;
				}
				goto MFResetCircle1;
			}
			s_DelayMs(500);
		}
	}
	else
	{
		TRACE("Reset idleʧ��:%d", ret);
	}
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/

void TypeABCircleTest(void)
{
	uchar ret, type, buf[32];
	uint i=0, j = 0,time=500,flag,k,m=0;
	TRACE("\r\n ���õ���Ѱ��ʱ��(ms):");
	j = InkeyCount(0);
	if(j == 0)
		time = 500;
	TRACE("\r\n �Ƿ�Ҫȡ�����:0-��ȡ����� 1-ȡ�����  ");
	flag = InkeyCount(0);
	TRACE("\r\n ���Դ���: ");
	k = InkeyCount(0);
	if(k==0)
	{
		k = UINT_MAX;
	}
	while (k--)
	{
		if (IfInkey(0))
		{
			TRACE("\r\n 99-�˳� ");
			if (InkeyCount(0) == 99)
			{
				return;
			}
		}
		TRACE("\r\n---ѰAB��%d...",++m);
		ret = EA_ucMIFPollMifCard(0, 1, &type);
		if (!ret)
		{
			if(type == EM_mifs_TYPEBCARD)
				TRACE("�ҵ���TypeB ");
			else
				TRACE("�ҵ���TypeA:%X ", type);
			if (type == EM_mifs_PRO_S50 || type == EM_mifs_PRO_S70)
			{
				type = EM_mifs_PROCARD;
			}
			TRACE("\r\n���...");
			ret = EA_ucMIFActivate(0, type, &i, buf);
			if (!ret)
			{
				TRACE("�ɹ�!");
				DISPBUF(buf, i, 0);
				if(flag)
				{
					TRACE("\r\n��ȡ�����...");
					ret = EA_ucMIFAPDU(0, sizeof(gApduGet4rand), gApduGet4rand, &i, &buf);
					if (!ret)
					{
						TRACE("�ɹ�!");
						DISPBUF(buf, i, 0);
						sys_beep();
					}
				}
				else
				{
					sys_beep();
				}
				EA_ucMIFDeactivate(0);
			}
			else
			{
				TRACE("ʧ��%2X", ret);
			}
		}
		s_DelayMs(time);
	}
}


void Test_NewAPITypeABCircle(void)
{
	uchar ret, buf[32];
	uint type;
	uint i=0, j = 0,time=500,flag,k,m=0,mode;
	TRACE("\r\n Ѱ��ģʽ:0-ISO 1-EMV ");
	mode = InkeyCount(0);
	TRACE("\r\n ���õ���Ѱ��ʱ��(ms):");
	j = InkeyCount(0);
	if(j == 0)
		time = 500;
	TRACE("\r\n �Ƿ�Ҫȡ�����:0-��ȡ����� 1-ȡ�����  ");
	flag = InkeyCount(0);
	TRACE("\r\n ���Դ���: ");
	k = InkeyCount(0);
	if(k==0)
	{
		k = UINT_MAX;
	}
	while (k--)
	{
		if (IfInkey(0))
		{
			TRACE("\r\n 99-�˳� ");
			if (InkeyCount(0) == 99)
			{
				return;
			}
		}
		TRACE("\r\n---ѰAB��%d...",++m);
		ret = rfid_poll(mode, &type);
		if (!ret)
		{
			if(type == EM_mifs_TYPEBCARD)
				TRACE("�ҵ���TypeB ");
			else
				TRACE("�ҵ���TypeA:%X ", type);
			if (type == EM_mifs_PRO_S50 || type == EM_mifs_PRO_S70)
			{
				type = EM_mifs_PROCARD;
			}
			TRACE("\r\n���...");
			ret = rfid_powerup(type, &i, buf);
			if (!ret)
			{
				TRACE("�ɹ�!");
				DISPBUF(buf, i, 0);
				if(flag)
				{
					TRACE("\r\n��ȡ�����...");
					ret = rfid_exchangedata(sizeof(gApduGet4rand), gApduGet4rand, &i, buf);
					if (!ret)
					{
						TRACE("�ɹ�!");
						DISPBUF(buf, i, 0);
						sys_beep();
					}
				}
				else
				{
					sys_beep();
				}
				EA_ucMIFDeactivate(0);
			}
			else
			{
				TRACE("ʧ��%2X", ret);
			}
		}
		s_DelayMs(time);
	}
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
typedef struct
{
	int cmdlen;
	char *cmd;
} ET_IC_CMD;

//��ѯ�������
const ET_IC_CMD gRfidYueCmd[] = {
	{20,"\x00\xa4\x04\x00\x0e\x31\x50\x41\x59\x2e\x53\x59\x53\x2e\x44\x44\x46\x30\x31\x00"},
	{5, "\x00\xb2\x01\x0c\x00"},
	{5, "\x00\xb2\x02\x0c\x00"},
	{14,"\x00\xa4\x04\x00\x08\xa0\x00\x00\x03\x33\x01\x01\x06\x00"},
	{17,"\x80\xa8\x00\x00\x0b\x83\x09\x00\x00\x00\x00\x00\x00\x00\x01\x56\x00"},
	{5, "\x00\xb2\x01\x0c\x00"},
	{5, "\x00\xb2\x02\x0c\x00"},
	{5, "\x00\xb2\x01\x14\x00"},
	{5, "\x00\xb2\x02\x14\x00"},
	{5, "\x00\xb2\x03\x14\x00"},
	{5, "\x00\xb2\x04\x14\x00"},
    {5, "\x00\xb2\x01\x1c\x00"},
	{5, "\x00\xb2\x02\x1c\x00"},
	{5, "\x00\xb2\x03\x1c\x00"},
	{5, "\x00\xb2\x01\x24\x00"},
	//{0, ""},
};

int Test_Rfid_checkyue(uint times,uint mode)
{
	uint i,j,type;
	uchar buf[256];
	int ret;
	TRACE("\r\n ------��ѯ������%d------",times);
	ret = rfid_poll(0,&type);
	if(ret)
	{
		TRACE("\r\n Ѱ��ʧ��%d",ret);
		return ret;
	}
	TRACE("\r\n ������:%d",type);
	ret = rfid_powerup(type,&i,buf);
	if(ret)
	{
		TRACE("\r\n ���ϵ�ʧ��%d",ret);
		return ret;
	}
	vDispBufTitle("���ϵ�����", i, 0, buf);
	for(i=0;i<DIM(gRfidYueCmd);i++)
	{
		if(mode)
			TRACE("\r\n %d",i);
		else
			TRACE(" %d",i);
		if(mode)
			vDispBuf(gRfidYueCmd[i].cmdlen, 0, (uchar *)gRfidYueCmd[i].cmd);
		guiDebugi = 0;
		ret = rfid_exchangedata(gRfidYueCmd[i].cmdlen,(uchar *)gRfidYueCmd[i].cmd,(uint *)&j,buf);
		TRACE("\r\n ��������:");
		vDispBufKey(guiDebugi, (uchar *)gcDebugBuf);
		guiDebugi = 0;
		if(ret)
		{
			TRACE("\r\n ����ʧ��:%d",ret);
			break;
		}
		else
		{
			if(mode)
				vDispBuf(j, 0, buf);
		}
	}
	rfid_powerdown();
	return ret;
}

// ����16�ֽ�
void mifare_analyse(uint sector)
{
	uchar blockflag[4];
	uchar buf[64];
	int ret;
	char *flagstr[8]={
		"��(AB) д(AB) ��(AB) ��(AB) ����",
		"��(AB) д(N)  ��(N)  ��(N)  ���ݶ�",
		"��(AB) д(B)  ��(N)  ��(N)  ���ݶ�",
		"��(AB) д(B)  ��(B)  ��(AB) ��ֵ��",
		"��(AB) д(N)  ��(N)  ��(AB) ��ֵ��",
		"��(B)  д(B)  ��(N)  ��(N)  ���ݶ�",
		"��(B)  д(N)  ��(N)  ��(N)  ���ݶ�",
		"��(N)  д(N)  ��(N)  ��(N)  ���ݶ�",
		};
	uint i;
	TRACE("\r\n ==����:%d==",sector);
	ret = rfid_MIFAuth(sector,0,(uchar *)"\xFF\xFF\xFF\xFF\xFF\xFF");
	if(ret)
	{
		TRACE("\r\nAuth ret:%d",ret);
		return;
	}

	ret = rfid_MIFRead(sector, buf);
	if(ret)
	{
		TRACE("\r\nRead ret:%d",ret);
		return;
	}
	TRACE("\r\nȨ���ֽ�:%02X %02X %02X %02X",buf[6],buf[7],buf[8],buf[9]);
	TRACE("\r\n�����ֽ�:%02X %02X %02X %02X %02X %02X",buf[10],buf[11],buf[12],buf[13],buf[14],buf[15]);
	CLRBUF(blockflag);
	for(i=0;i<4;i++)
	{
		blockflag[i] = (GETBIT(HHALFB(buf[7]),i)<<2)
			|(GETBIT(LHALFB(buf[8]),i)<<1)
			|(GETBIT(HHALFB(buf[8]),i)<<0);
		TRACE("\r\nblock:%d Ȩ��%02X:%s",i,blockflag[i],flagstr[blockflag[i]]);
	}
}



/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void Test_NewAPI(void)
{
	uint i, j, k, m;
	int ret;
    long value;
	uchar ucBuf[512];
	while (1)
	{
		TRACE("\r\n-------------NewAPI Test------------------");
		TRACE("\r\n 1-open  2-close  3-poll   4-powerup 5-powerdown 6-exchange  7-��ѯ��� 8-�ۺϲ��� ");
		TRACE("\r\n 9-auth  10-read  11-write 12-Inc  13-Dec 14-Restore 15-Transfer 16-������Ȩ�� ");
		TRACE("\r\n 17-�������ֿ� IN:");
		switch (InkeyCount(0))
		{
		case 1:
			TRACE("\r\n mode:");
			ret = rfid_open(InkeyCount(0));
			TRACE("\r\n ret:%d",ret);
			break;
		case 2:
			ret = rfid_close();
			TRACE("\r\n ret:%d",ret);
			break;
		case 3:
			TRACE("\r\n mode:");
			j = InkeyCount(0);
			while(1)
			{
				ret = rfid_poll(j,&i);
				TRACE("\r\n ret:%d",ret);
				if(!ret)
				{
					TRACE(" type:%d",i);
					sys_beep();
				}
				if(IfInkey(0))
				{
					TRACE("\r\n 99-�˳�");
					if(InkeyCount(0) == 99)
						break;
				}
				s_DelayMs(500);
			}

			break;
		case 4:
			TRACE("\r\n type:");
			ret = rfid_powerup(InkeyCount(0),&i,ucBuf);
			TRACE("\r\n ret:%d",ret);
			if(!ret)
			{
				DISPBUF(ucBuf,i,0);
			}
			break;
		case 5:
			ret = rfid_powerdown();
			TRACE("\r\n ret:%d",ret);
			break;
		case 6:
			TRACE("\r\n 1-ѡ����Ŀ¼ 0-ȡ����� ");
			if(!InkeyCount(0))
			{

				ret = rfid_exchangedata(sizeof(gApduGet4rand), gApduGet4rand, &i, &ucBuf[0]);
			}
			else
			{
				//ret = rfid_exchangedata(8,"\x00\xA4\x00\x00\x02\x3F\x00\x00", &i, &ucBuf[0]);
				ret = rfid_exchangedata(sizeof(gApduSelMaindir), gApduSelMaindir, &i, &ucBuf[0]);
			}

			TRACE("\r\n ret:%d",ret);
			if(!ret)
			{
				DISPBUF(ucBuf,i,0);
			}
			break;
		case 7:
			i = 0;
			TRACE("\r\n �Ƿ���ʾ����:");
			j = InkeyCount(0);
			TRACE("\r\n 1-ʧ���˳�:");
			k = InkeyCount(0);
			while(1)
			{
				if(IfInkey(0))
				{
					if(InkeyCount(0) == 99)
						break;
				}
				if(Test_Rfid_checkyue(++i,j))
				{
					if(k == 1)
						break;
				}
			}
			break;
		case 8:
			Test_NewAPITypeABCircle();
			break;
		case 9:
			TRACE("\r\n ������:");
			i = InkeyCount(0);
			TRACE("\r\n ��Կ����:");
			j = InkeyCount(0);
			memset(ucBuf,0xFF,sizeof(ucBuf));
			ret = rfid_MIFAuth(i, j, ucBuf);
			TRACE("\r\n ret:%d",ret);
			break;
		case 10:
			TRACE("\r\n ���:");
			j = InkeyCount(0);
			CLRBUF(ucBuf);
			for(j=0;j<4;j++)
			{
				ret = rfid_MIFRead(j, &ucBuf[16*j]);
			}
			TRACE("\r\n ret:%d",ret);
			vDispBuf(64, 0, ucBuf);
			break;
		case 11:
			TRACE("\r\n ���:");
			j = InkeyCount(0);
			TRACE("\r\n 1-д0  2-дFF 3-д������ 4-дָ���� 5-д����ֵ�� ");
			m = InkeyCount(0);
			if (m == 1)
			{
				memset(ucBuf,0,sizeof(ucBuf));
			}
			else if (m == 2)
			{
				memset(ucBuf,0xFF,sizeof(ucBuf));
			}
			else if(m == 3)
			{
				for (k = 0; k < 64; k++)
				{
					ucBuf[k] = (uchar)(k+1)&0xFF;
				}
			}
			else if(m == 4)
			{
				for (k = 0; k < 16; k++)
				{
					ucBuf[k] = InkeyHex(0);
				}
			}
			else if(m == 5)
			{
				TRACE("\r\n ��ֵ:");
				k = InkeyCount(0);
				TRACE("\r\n ��ַ:");
				m = InkeyCount(0);
				rfid_MIFMakevalue((long)k,(uchar)m,ucBuf);
			}
			vDispBuf(16, 0, ucBuf);
			ret = rfid_MIFWrite(j, ucBuf);
			TRACE("\r\n ret:%d",ret);
			break;
		case 12:
			TRACE("\r\n ���:");
			j = InkeyCount(0);
			TRACE("\r\n ��ֵ:");
			k = InkeyCount(0);
			ret = rfid_MIFIncrement(j,k);
			break;
		case 13:
			TRACE("\r\n ���:");
			j = InkeyCount(0);
			TRACE("\r\n ��ֵ:");
			k = InkeyCount(0);
			ret = rfid_MIFDecrement(j,k);
			break;
		case 14:
			TRACE("\r\n ���:");
			j = InkeyCount(0);
			TRACE("\r\n ��ֵ:");
			ret = rfid_MIFRestore(j);
			break;
		case 15:
			TRACE("\r\n ���:");
			j = InkeyCount(0);
			TRACE("\r\n ��ֵ:");
			ret = rfid_MIFTransfer(j);
			break;
		case 16:
			TRACE("\r\n ��ʼ������:");
			i = InkeyCount(0);
			TRACE("\r\n ������:");
			j = InkeyCount(0);
			for(k=0;k<j;k++)
			{
				mifare_analyse(i+k);
			}
			break;
        case 17:
            value = 0x7FFFFFFF;
            rfid_MIFMakevalue(value-1,1,ucBuf);
            TRACE("\r\n %d-%x",value-1,value-1);
            vDispBuf(16,0,ucBuf);
            rfid_MIFMakevalue(value,1,ucBuf);
            TRACE("\r\n %d-%x",value,value);
            vDispBuf(16,0,ucBuf);
            rfid_MIFMakevalue(value+1,1,ucBuf);
            TRACE("\r\n %d-%x",value+1,value+1);
            vDispBuf(16,0,ucBuf);
            rfid_MIFMakevalue(value+2,1,ucBuf);
            TRACE("\r\n %d-%x",value+2,value+2);
            vDispBuf(16,0,ucBuf);
            rfid_MIFMakevalue(value+3,1,ucBuf);
            TRACE("\r\n %d-%x",value+3,value+3);
            vDispBuf(16,0,ucBuf);
            break;
		case 99:

			return;
		}
		TRACE("\r\n ��������:");
		vDispBufKey(guiDebugi, (uchar *)gcDebugBuf);
		guiDebugi = 0;
	}
}

/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void Test_CardCircle(void)
{
}


void rfid_show_ret(uchar ret)
{
	switch(ret)
	{
		case EM_mifs_SUCCESS:
			TRACE("\r\n [�ɹ�]");
			break;
		case EM_mifs_NOCARD:
			TRACE("\r\n [�޿�]");
			break;
		case EM_mifs_TRANSERR:
			TRACE("\r\n [ͨѶ����]");
			break;
		case EM_mifs_PROTERR:
			TRACE("\r\n [��Ч��]");
			break;
		case EM_mifs_CARDEXIST:
			TRACE("\r\n [��δ�뿪]");
			break;
		case EM_mifs_TIMEOUT:
			TRACE("\r\n [��ʱ]");
			break;
		case EM_mifs_MULTIERR:
			TRACE("\r\n [���ſ�]");
			break;
		case EM_mifs_NOACT:
			TRACE("\r\n [��δ����]");
			break;
		case EM_mifs_SWDIFF:
			TRACE("\r\n [��״̬����]");
			break;
		default:
			TRACE("\r\n ret:0x%2X",ret);
			break;
	}
}




/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
unsigned char mag_block;
uchar keya[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uchar keyb[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uchar keyc[4] = {0xff, 0x07, 0x80, 0x69};
uchar keydata[16];

void Test_Dianxin(void)
{
	uchar ret, buf[32];
	uint i,type;
	ret = rfid_poll(RFID_MODE_ISO, &type);
	if (!ret)
	{
		TRACE("\r\n�ҵ���%d",type);
		TRACE("\r\n���...");
		ret = rfid_powerup(type, &i, buf);
		if (!ret)
		{
			TRACE("�ϵ�ɹ�!");
			vDispBufTitle("�ϵ�����", i, 0,buf);
			TRACE("\r\n ��������:");
			vDispBufKey(guiDebugi, (uchar *)gcDebugBuf);
			while(1)
			{
				TRACE("\r\n 1-����� 2-��Ŀ¼ 3-��IMSI 99-�˳� ");
				i = InkeyCount(0);
				if(i == 1)
				{
					ret = rfid_exchangedata(sizeof(gApduGet4rand), gApduGet4rand, &i, buf);
				}
				else if(i == 2)
				{
					ret = rfid_exchangedata(sizeof(gApduSelMaindir), gApduSelMaindir, &i, buf);
				}
				else if(i == 3)
				{
					ret = rfid_exchangedata(sizeof(gApduSelMaindir), gApduSelMaindir, &i, buf);
				}
				else if(i == 99)
				{
					break;
				}
				TRACE("\r\n ��������:");
				vDispBufKey(guiDebugi, (uchar *)gcDebugBuf);
				if (!ret)
				{
					TRACE("�ɹ�!");
					DISPBUF(buf, i, 0);
					sys_beep();
				}
				else
				{
					TRACE("ʧ��%d", ret);
					rfid_powerdown();
					break;
				}
			}
		}
		else
		{
			TRACE("ʧ��%d", ret);
		}
	}
}
//0020000003123456
int Test_CheckICPsw(void)
{
	int ret;
	uint type;
	uchar buf[128];
	uint i;
	ret = rfid_poll(0,&type);
	if(ret)
	{
		TRACE("\r\n Ѱ��ʧ��%d",ret);
		return ret;
	}
	TRACE("\r\n ������:%d",type);
	ret = rfid_powerup(type,&i,buf);
	if(ret)
	{
		TRACE("\r\n ���ϵ�ʧ��%d",ret);
		return ret;
	}
	vDispBufTitle("���ϵ�����", i, 0, buf);
	TRACE("\r\nУ������0020000003123456...");
	ret = rfid_exchangedata(8, (uchar *)"\x00\x20\x00\x00\x03\x12\x34\x56", &i, buf);
	if(ret)
	{
		TRACE("\r\n ����ʧ��:%d",ret);
	}
	else
	{
		vDispBuf(i, 0, buf);
	}
	rfid_powerdown();
	return ret;
}

void Test_rfid_lowpower(void)
{
	int ret;
	uint type;
	TRACE("\r\n ��Ƶ�͹��Ĳ���");
	TRACE("\r\n ��ģ��");
	ret = rfid_open(2);
	while(1)
	{
		ret = rfid_poll(RFID_MODE_ISO,&type);
	}
}
void Test_Rfid(void)
{
	//char *cuid;
	uint i, j, k, n, m,ret;
	uchar ucBuf[128];
	DevHandle Handle=0;
	uchar type;
	int data;
	uchar addr;
	uchar rbuf[16];
	uchar sbuf[16];
	guiDebugm = 0;
	guiDebugn = 1000;
	guiDebugFlg = 1;
	EA_ucMifsOpen(0);
	while (1)
	{
		TRACE("\r\n ----------------------�ǽӴ�������----------------------------------");
		TRACE("\r\n guiDebugFlg:%d guiDebugm:%d guiDebugn:%d", guiDebugFlg,guiDebugm, guiDebugn);
		TRACE("\r\n 1-open  2-close  3-RC531  4-RequestA  5-Set Register  6-LoadKey ");
		TRACE("\r\n 7-AntiColl 8-Select 9-HaltA 10-Debugmn 11-ResetPro 12-ExchangePro0 ");
		TRACE("\r\n 13-DeselectPro 14-TypeA��ѭ������ 15-PollMifCard 16-Activate 17-MIFAPDU  ");
		TRACE("\r\n 18-Deactivate 19-ѡ����Ŀ¼  20-ExchangePro1 21-������λ 23-TypeB��ѭ������   ");
		TRACE("\r\n 24-��ӡ  25-��Ƶ���ֿ⸴�ϲ���  26-��ѯ��27-���ѡ�28-��ȡȫ���Ĵ��� ");
		TRACE("\r\n 29-RequestB 30-TypeAB��ѭ������   31-WorkInfo 32-WUPB 33-SlotMarkerB 34-Test_NewAPI ");
		TRACE("\r\n 35-�߼����ܿ�Ѱ�� 36- ���߼����ܿ� 37-д�߼����ܿ� 38-��ֵ 39-��ֵ 40-Transfer 41-Restore");
		TRACE("\r\n 42-����Ƭ 43-���ſ����� 44-�͹��Ĳ��� 45-У��������� ");
		ret = 0;
		i = InkeyCount(0);
		switch (i)
		{
		case 1:
			TRACE("\r\n 8-typeB ");
			if (InkeyCount(0) == 8)
				j = EM_mifs_TYPEB;
			else
				j = EM_mifs_TYPEA;
			i = sys_get_counter();
			ret = EA_ucMifsOpen(j);
			k = sys_get_counter();
			TRACE("\r\n ����ʱ��:%d ���:%d", k-i, ret);
			break;
		case 2:
			ret = EA_ucMifsClose();
			break;
		case 3:
			Test_RC531();
			break;
		case 4:
			TRACE("\r\n 0-EM_mifs_IDLE  1-EM_mifs_HALT   ");
			i = InkeyCount(0);
			j = 0;
			ret = EA_ucMIFRequest(Handle, i, (ushort *) & j);
			if (!ret)
				TRACE("\r\nTagType:0x%04X", j);

			break;
		case 5:
			TRACE("\r\n �Ĵ�����ַ:");
			j = InkeyHex(0);
			TRACE("\r\n ����:");
			k = InkeyHex(0);
			ret = EA_ucMIFSetRegVal(i, (uchar) j, (uchar) k);
			TRACE("\r\n ret:%d");
			break;
		case 6:
			TRACE("\r\n��Կ��(0~31):");
			j = InkeyCount(0);
			TRACE("\r\n��Կֵ:");
			i = InkeyHex(0);
			memset(ucBuf, i, sizeof(ucBuf));
			//ret = EA_ucMIFLoadKey(0, j, ucBuf);
			TRACE("\r\n ret:%d  ��Կ����:", ret);
			DISPBUF(ucBuf, sizeof(ucBuf), 0);
			break;
		case 7:
			i = 0;
			ret = EA_ucMIFAntiColl(0, (ulong *) & i);
			TRACE("\r\n i:0x%X", i);
			break;
		case 8:
			TRACE("\r\n card no:");
			i = InkeyHex(0);
			ret = EA_ucMIFSelect(0, i, &j);
			TRACE("\r\nCardSize:0x%04X", j);
			break;
		case 9:
			ret = EA_ucMIFHalt(0);
			break;
		case 10:
			TRACE("\r\n guiDebugm:%d guiDebugn:%d guiDebugFlg:%d", guiDebugm, guiDebugn,
				guiDebugFlg);
			TRACE("\r\nguiDebugm:");
			guiDebugm = InkeyCount(0);
			TRACE("\r\nguiDebugn:");
			guiDebugn = InkeyCount(0);
			TRACE("\r\nguiDebugFlg:");
			guiDebugFlg = InkeyCount(0);
			break;
		case 11:
			TRACE("\r\n ��λ��ʽ:1-Idle  2-Halt  ");
			j = InkeyCount(0) - 1;
			ret = EA_ucMIFResetPro(0, j, &i, ucBuf);
			DISPBUF(ucBuf, i, 0);
			break;
		case 12:
			memcpy(ucBuf, "\x00\x84\x00\x00\x04", 5);
			ret = EA_ucMIFExchangePro(0, 5, ucBuf, &i, &ucBuf[64]);
			//TRACE("\r\n ����:");
			//DISPBUF(ucBuf,5,0);
			TRACE("\r\n ����:");
			DISPBUF(&ucBuf[64], i, 0);
			break;
		case 13:
			ret = EA_ucMIFDeselectPro(0);
			break;
		case 14:
			TypeACircleTest();
			break;
		case 15:
			i = 0;
			TRACE("\r\n 1-�����Ӧ����TypeA�� 0-�������Ӧ����TypeA�� ");
			j = InkeyCount(0);
			TRACE("\r\n 1-ѭ������ 0-���β��� ");
			k = InkeyCount(0);
			while (1)
			{
				TRACE("\r\n %d��ѯ��...", ++i);
				ret = EA_ucMIFPollMifCard((DevHandle) ucBuf, j, ucBuf);
				if (!ret)
				{
					TRACE("�ҵ���Type%X", ucBuf[0]);
					sys_beep();
				}
				else
				{
					TRACE("δ�ҵ���:0x%02X", ret);
				}
				if (IfInkey(0))
				{
					TRACE("\r\n 1-�˳�  ");
					if (InkeyCount(0) == 1)
					{
						break;
					}
				}
				if (!k)
				{
					break;
				}
				s_DelayMs(1000);
			}
			break;
		case 16:
			TRACE("\r\n CardType(0~5):");
			i = InkeyHex(0);
			CLRBUF(ucBuf);
			ret = EA_ucMIFActivate(0, (uchar) i, &j, ucBuf);
			DISPBUF(ucBuf, j, 0);
			break;
		case 17:
			ret = EA_ucMIFAPDU(0, sizeof(gApduGet4rand), gApduGet4rand, &i, &ucBuf[64]);
			DISPBUF(&ucBuf[64], i, 0);
			break;
		case 18:
			ret = EA_ucMIFDeactivate(0);
			break;
		case 19:
			ret = EA_ucMIFAPDU(0, sizeof(gApduSelMaindir), gApduSelMaindir, &i, &ucBuf[64]);
			TRACE("\r\n ����:");
			DISPBUF(ucBuf, 8, 0);
			TRACE("\r\n ����:");
			DISPBUF(&ucBuf[64], i, 0);
			memcpy(ucBuf, "\x00\xA4\x00\x00\x02\x3F\x00\x00", 8);
			ret = EA_ucMIFExchangePro(0, 8, ucBuf, &i, &ucBuf[64]);
			TRACE("\r\n ����:");
			DISPBUF(ucBuf, 8, 0);
			TRACE("\r\n ����:");
			DISPBUF(&ucBuf[64], i, 0);
			break;
		case 20:
			CLRBUF(ucBuf);
			ucBuf[0] = 0x00;
			ucBuf[1] = 0xA4;
			ucBuf[2] = 0x04;
			ucBuf[3] = 0x00;
			ucBuf[4] = 0x0E;
			memcpy(ucBuf + 5, "1PAY.SYS.DDF01", 14);
			ucBuf[19] = 0x00;
			ret = EA_ucMIFExchangePro(0, 20, ucBuf, &i, &ucBuf[64]);
			TRACE("\r\n ����:");
			DISPBUF(ucBuf, 20, 0);
			TRACE("\r\n ����:");
			DISPBUF(&ucBuf[64], i, 0);
			break;
		case 21:
			Test_MFResetCircle();
			break;
		case 22:
			memcpy(ucBuf, "\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01\x10", 15);
			ret = EA_ucMIFExchangePro(0, 15, ucBuf, &i, &ucBuf[64]);
			TRACE("\r\n ����:");
			DISPBUF(ucBuf, 15, 0);
			TRACE("\r\n ����:");
			DISPBUF(&ucBuf[64], i, 0);
			break;

		case 23:
			TypeBCircleTest();
			break;
		case 24:
			TRACE("\r\n -------A������------");
			TRACE("\r\n CWCONDUCTANCE(0x12):0x%2X ",gtRfidDebugInfo.CW_A);
			TRACE("\r\n MODWIDTH(0x15):0x%2X",gtRfidDebugInfo.ModWidth_A);
			TRACE("\r\n CWCONDUCTANCE:");
			gtRfidDebugInfo.CW_A = InkeyHex(0);
			TRACE("\r\n MODWIDTH:");
			gtRfidDebugInfo.ModWidth_A = InkeyHex(0);
			rfid_open(0);
			break;
		case 25:
			TRACE("\r\n -------B������------");
			TRACE("\r\n CWCONDUCTANCE(0x12):0x%2X ",gtRfidDebugInfo.CW_B);
			TRACE("\r\n MODCONDUCTANCE(0x13):0x%2X",gtRfidDebugInfo.ModConduct_B);
			TRACE("\r\n CWCONDUCTANCE:");
			gtRfidDebugInfo.CW_B = InkeyHex(0);
			TRACE("\r\n MODCONDUCTANCE:");
			gtRfidDebugInfo.ModConduct_B = InkeyHex(0);
			break;
		case 26:
			TRACE("\r\n 1-���������Ϣ");
			ret = Test_Rfid_checkyue(1,InkeyCount(0));
			break;
		case 27:
			Test_NewAPITypeABCircle();
		case 29:
			TRACE("\r\n 1-����  ");
			if (InkeyCount(0) == 1)
			{
				k = 1;
			}
			else
			{
				k = 0;
			}
			TRACE("\r\n 0-EM_mifs_IDLE  1-EM_mifs_HALT   ");
			i = InkeyCount(0);
			j = 0;
			while (1)
			{
				j = 0;
				TRACE("\r\n--Request--");
				CLRBUF(ucBuf);
				ret = EA_ucMIFRequestB(0, i, EM_mifs_TYPEB_REQALL, 3, &j, ucBuf);
				if (!ret)
				{
					TRACE("�ɹ�");
					DISPBUF(ucBuf, j, 0);
					sys_beep();
					//break;
				}
				else
				{
					TRACE("ʧ��:%d", ret);
				}
				if (k)
					break;
				if (IfInkey(0))
				{
					TRACE("\r\n 1-�˳� ");
					if (InkeyCount(0) == 1)
					{
						break;
					}
					goto EA_ucMIFRequestBOver;
				}
				s_DelayMs(1000);
			}
		  EA_ucMIFRequestBOver:
			break;
		case 30:
			TypeABCircleTest();
			break;
		case 31:
			show_Rfid_WorkInfo();
			break;
		case 32:
			i = 0;
			TRACE("\r\n 1-ѭ������ 0-���β��� ");
			k = InkeyCount(0);
			while (1)
			{
				TRACE("\r\n %d��ѯB��...", ++i);
				EG_mifs_tWorkInfo.ucAnticollFlag = 1;
				ret = EI_paypass_ucWUPB(&ucBuf[0], &ucBuf[1]);
				if (!ret)
				{
					TRACE("�ҵ��� len:%d",ucBuf[0]);
					DISPBUF(&ucBuf[1],ucBuf[0],0);
					//sys_beep();

				}
				else
				{
					TRACE("δ�ҵ���:0x%02X", ret);
					//TRACE("\r\n 1-�˳�  ");

				}
				if (IfInkey(0))
				{
					TRACE("\r\n 1-�˳�  ");
					if (InkeyCount(0) == 1)
					{
						break;
					}
				}
				if (!k)
				{
					break;
				}
				//s_DelayMs(1000);
			}
			break;
		case 33:
			TRACE("\r\n ����Slot:0-1 1-2 2-4 3-8 4-16:");
			i = InkeyCount(0);
			k = (uint) 0x01 << i;
			TRACE("\r\n slot:%d",k);
			ret = EA_ucMIFRequestB(0,EM_mifs_HALT,EM_mifs_TYPEB_REQALL,i,&j,ucBuf);
			TRACE("\r\n ret:%2X",ret);
			if(!ret)
			{
				DISPBUF(ucBuf,j,0);
			}
			for(i=1;i<=k-1;i++)
			{
				ret = EA_ucMIFSlotMarkerB(0,(uchar)i,&j,ucBuf);
				TRACE("\r\n [%d-%2X]",i,ret);
				if(!ret)
				{
					DISPBUF(ucBuf,j,0);
				}
			}
			break;
		case 34:
			Test_NewAPI();
			break;
		case 35:
			TRACE("\r\n TypeA�߼����ܿ�Ѱ��...");
			EA_ucMifsClose();
			ret = EA_ucMifsOpen(EM_mifs_TYPEA);
			if (!ret)
			{
				TRACE("\r\n��������....");
				mag_block = InkeyCount(0);
				if(mag_block > 16)
				{
					TRACE("\r\n������̫��ӦС��16....");
					break;
				}
				TRACE("\r\nѡ��У���ܳ�:0-KEYA 1-KEYB 2-�޸��ܳ�A 3-�޸��ܳ�B");
				m = InkeyCount(0);
				switch(m)
				{
					case 0:
						memcpy(keya, "\xff\xff\xff\xff\xff\xff", 6);
						break;
					case 1:
						memcpy(keyb, "\xff\xff\xff\xff\xff\xff", 6);
						break;
					case 2:
						TRACE("����KEYA:");
						for(i=0; i<6; i++)
						{
							keya[i] = InkeyHex(0);
						}
						vDispBufKey(6, keya);
						break;
					case 3:
						TRACE("����KEYB:");
						for(i=0; i<6; i++)
						{
							keyb[i] = InkeyHex(0);
						}
						vDispBufKey(6, keyb);
						break;
					default:
						break;
				}
				TRACE("\r\n�ɹ�!�������ʼѰ��....");
				InkeyCount(0);
				i = 0;
				TRACE("\r\n----ѰA��%d...", ++i);
				type = EM_mifs_S50;
#if 0
				ret = EA_ucMIFPollMifCard(0, 0, type);
				if (!ret)
				{
1
//��дUID
//	memcpy((uchar *)&EG_mifs_tWorkInfo.ulCardID, pucUID, 4);
//   ������ATRѡ��
2
//������ATR����������Mifare��ûatr���أ��ֽ�����ѰB��ָ�����������ʧ�ܡ�
#endif
					//Ѱ��
					#if 1
					ret = EA_ucMIFRequest(0, EM_mifs_HALT, (ushort *) & j);
					if (!ret)
					{
						TRACE("�ɹ�:TagType%d", j);
						TRACE("\r\n��ȡ����..." );
						//��ͻ
						ret = EA_ucMIFAntiColl(0,(ulong *)&j);
						if (!ret)
						{
							TRACE("����:%08X", j);
							sys_beep();
						}
						else
						{
							TRACE("ʧ��%d",ret);
						}
						//EA_ucMIFHalt(0);
						//ѡ��
						//cuid = (char *)&j;
						//TRACE("\r\nj=%d [%x][%x][%x][%x]",j,cuid[0],cuid[1],cuid[2],cuid[3]);
						ret = EA_ucMIFSelect(0, j, &i);
						TRACE("\r\n��ȡ������..." );
						if(!ret)
						{
							TRACE("����:%d", i);
						}
						else
						{
							TRACE("ʧ��%d",ret);
						}
						#else
						#endif
						#if 0
						//LoadKEY //2�з�ʽ��
						if(m%2 == 0)
						{
							//keya ��֤:
							ret = EA_ucMIFLoadKey(0, 0, keya);
						}
						else
						{
							//keyb ��֤:
							ret = EA_ucMIFLoadKey(0, 20, keyb);
						}
						if(!ret)
						{
							TRACE("\r\n�����ܳ�");
						}
						else
						{
							TRACE("\r\nʧ��%d",ret);
						}
						//auth �Ե�0����������֤
						if(m%2 == 0)
						{
							//keya ��֤:
							ret = EA_ucMIFAuthentication(0, 0, mag_block);
						}
						else
						{
							//keyb ��֤:
							ret = EA_ucMIFAuthentication(0, 20, mag_block);
						}
						#else
						TRACE("����:%08X", EG_mifs_tWorkInfo.ulCardID);
						TRACE("\r\n�ɹ�!�������ʼУ���ܳ�...");
						InkeyCount(0);
						if(m%2 == 0)
						{
							//keya ��֤:
							ret = EA_ucMIFAuthToRam(0, mag_block, EM_mifs_KEYA, keya);
						}
						else
						{
							//keyb ��֤:
							ret = EA_ucMIFAuthToRam(0, mag_block, EM_mifs_KEYA, keyb);
						}
						#endif
						if(!ret)
						{
							TRACE("\r\n��֤����");
						}
						else
						{
							TRACE("\r\nʧ��%d",ret);
							//break;
						}

					}
					else
					{
						TRACE("ʧ��:%d", ret);
					}
			}
			break;
		case 36:
			TRACE("\r\n 0-���� 1-������....");
			m = InkeyCount(0);
			if(m == 0)
			{
				TRACE("\r\n��������:0-3....");
				m = InkeyCount(0);
				ret = EA_ucMIFRead(0, mag_block*4 + m, rbuf);
				if(!ret)
				{
					TRACE("\r\n read buf :");
					for(i=0; i<16; i++)
					TRACE("[%02x]", rbuf[i]);
				}
				else
				{
					TRACE("\r\nʧ��%d",ret);
					break;
				}
			}
			else
			{
				for(m=(mag_block*4+0); m<(mag_block*4+4); m++)
				{
					memset(rbuf, 0x00, 16);
					//��ȡ��0�����ĵ�3���ȡ
					ret = EA_ucMIFRead(0, m, rbuf);
					if(!ret)
					{
						TRACE("\r\n read buf :");
						for(i=0; i<16; i++)
						TRACE("[%02x]", rbuf[i]);
					}
					else
					{
						TRACE("\r\nʧ��%d",ret);
						break;
					}
				}
			}
			break;
		case 37:
			TRACE("\r\n����д���:0-3....");
			m = InkeyCount(0);
			TRACE("\r\n 0-ȫ0 1-ȫFF 2-��ֵ 3-��Ȩģʽ 4-���ܳ� 5-д���ݿ� 99-�˳� ����-����");
			k = InkeyCount(0);
			memset(sbuf, 0x00, 16);
			if (k == 99)
			{
				break;
			}
			else if (k == 0)
			{
				memset(sbuf, 0x00, 16);
			}
			else if (k == 1)
			{
				for(i=0; i<16; i++)
				{
					sbuf[i] = 0xff;
				}
			}
			else if (k == 2)
			{
				for(i=0; i<16; i++)
				{
					sbuf[i] = i;
				}
			}
			else if (k == 3)
			{
				TRACE("\r\n�����ܳ�:0-0~2���Ȩ�� 1-3��Ȩ��");
				k = InkeyCount(0);
				if(k == 0)
				{
					TRACE("����0~2��Ȩ�޿���:");
				}
				else
				{
					TRACE("����3��Ȩ�޿���:");
				}
				for(i=0; i<3; i++)
				{
					keyc[i] = InkeyHex(0);
				}
				keyc[3] = 0x69;
				memcpy(keya, "\xff\xff\xff\xff\xff\xff", 6);
				memcpy(keyb, "\xff\xff\xff\xff\xff\xff", 6);
				memcpy(sbuf, keya, 6);
				memcpy(sbuf+6, keyc, 4);
				memcpy(sbuf+10, keyb, 6);
			}
			else if (k == 4)
			{
				TRACE("\r\n�����ܳ�:0-KEYA 1-KEYB 2-KEYAB");
				k = InkeyCount(0);
				if(k == 0)
				{
					TRACE("����KEYA:");
					for(i=0; i<6; i++)
					{
						keya[i] = InkeyHex(0);
					}
				}
				else if(k == 1)
				{
					TRACE("����KEYB:");
					for(i=0; i<6; i++)
					{
						keyb[i] = InkeyHex(0);
					}
				}
				else
				{
					TRACE("����KEYA:");
					for(i=0; i<6; i++)
					{
						keya[i] = InkeyHex(0);
					}

					TRACE("����KEYB:");
					for(i=0; i<6; i++)
					{
						keyb[i] = InkeyHex(0);
					}
				}
				memcpy(sbuf, keya, 6);
				memcpy(sbuf+6, keyc, 4);
				memcpy(sbuf+10, keyb, 6);
			}
			else if (k == 5)
			{
				TRACE("\r\n0-��ֵ 1-��ֵ");
				k = InkeyCount(0);
				if(k == 0)
				{
					TRACE("\r\n������ֵ���ʼ��ֵ:");
					data = InkeyCount(0);
				}
				else
				{
					TRACE("\r\n������ֵ���ʼ��ֵ:(-)");
					data =  InkeyCount(0);
					data = 0 - data;
					TRACE("\r\n data = %d", data);
				}
				addr = mag_block*4 + m;
				memcpy(keydata, (uchar *)&data, 4);
				for(i=0; i<4; i++)
				{
					keydata[i+4] = ~((uchar *)&data)[i];
				}
				memcpy(keydata + 8, (uchar *)&data, 4);
				keydata[12] = addr;
				keydata[13] = ~addr;
				keydata[14] = addr;
				keydata[15] = ~addr;

				memcpy(sbuf, keydata, 16);
			}
			ret = EA_ucMIFWrite(0, mag_block*4 + m, sbuf);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			break;
		case 38:
			TRACE("\r\n����д���:0-3....");
			m = InkeyCount(0);
			TRACE("\r\n����ֵ:");
			n = InkeyCount(0);
			ret = EA_ucMIFIncrement(0, mag_block*4 + m, n);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			ret = EA_ucMIFTransfer(0, mag_block*4 + m);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			break;
		case 39:
			TRACE("\r\n����д���:0-3....");
			m = InkeyCount(0);
			TRACE("\r\n����ֵ:");
			n = InkeyCount(0);
			ret = EA_ucMIFDecrement(0, mag_block*4 + m, n);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			ret = EA_ucMIFTransfer(0, mag_block*4 + m);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			break;
		case 40:
			TRACE("\r\n���봫���:0-3....");
			m = InkeyCount(0);
			ret = EA_ucMIFTransfer(0, mag_block*4 + m);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			break;
		case 41:
			TRACE("\r\n��ȡĳ��ֵ:0-3....");
			m = InkeyCount(0);
			ret = EA_ucMIFRestore(0, mag_block*4 + m);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			TRACE("\r\n���뱣���:0-3....");
			m = InkeyCount(0);
			ret = EA_ucMIFTransfer(0, mag_block*4 + m);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			break;
		case 42:
			ret = EA_ucMIFHalt(0);
			if(ret)
			{
				TRACE("\r\nʧ��%d",ret);
			}
			break;
		case 43:
			Test_Dianxin();
			break;
        case 44:
            test_rfid_para_menu(0);
		case 45:
			Test_CheckICPsw();
			break;
		case 99:
			return;
		}
		//TRACE("\r\n ---ret:0x%02X---",ret);
		if (!ret)
		{
			TRACE("\r\n-----�ɹ�-----");
		}
		else
		{
			TRACE("\r\n-----ʧ��:0x%02X-----", ret);
		}
		TRACE("\r\n ��������:");
		//if(guiDebugi>=0x1000)
		//	guiDebugi = 0x1000;
		vDispBufKey(guiDebugi, (uchar *)gcDebugBuf);
		guiDebugi = 0;
		//DISP_DEBUGBUF();
	}
}



#endif


#endif


