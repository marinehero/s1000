/**********************************************************************
* ��Ȩ���� (C)    ����ʵ������豸���޹�˾��
* �ļ����ƣ�
* ����ժҪ��
*
* ��ʷ��¼��
*     �޸���		����	        �����޸�����
      liuxl         2010.11.2       ����
*    **********************************************************************/

/************************************************************************/
/*    #include������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ���                     */
/************************************************************************/

//#include "stdio.h"
#include "lib_emv.h"
#include "ICAdapter.h"
/************************************************************************/
/*    ��������                                                          */
/************************************************************************/

/************************************************************************/
/*        �ļ��ڲ�ʹ�õĺ�                                             */
/************************************************************************/

/************************************************************************/
/*        �ļ��ڲ�ʹ�õ���������ȫ�ֱ���                                */
/************************************************************************/

uint  guiCardType;
long  glCardType;                 //��Ƭ����
int   gnOpenRF = 0; //����Ƶģ���ʶ
extern int   gnICCardDebug;

struct ICAdapterStru
{
	long  (*glICAdapterSetPara)(long,ICAPARASTRU *);
	long  (*glICAdapterCardIn)(long, long*);
	long  (*glICAdapterPowerUp)(long , long *, void *);
	long  (*glICAdapterPowerDown)(long);
	long  (*glICAdapterExchangeAPDU)(long ,long , char *, long *, char *);
	long  lICComPortNo;
	long  lICComPortBPS;
	long  lICCardVCC;
	long  lICCardProtocol;
	long  lICCardIndex;
};


/************************************************************************/
/*      ���ر���������̬ȫ�ֱ���)                                        */
/************************************************************************/
struct ICAdapterStru m_struICAdapter = {
	NULL,
	ICCardIn,
	ICPowerUp,
	ICPowerDown,
	ICExchangeAPDU,
	UART_COM1,
	9600,
	VCC_5,
	ISOMODE,
	USERCARD
};

/************************************************************************/
/*     �ֲ�����ԭ�����ʵ��                                              */
/************************************************************************/
long ICPowerUp(long lICCardIndex,long *puiRetLen,void *pvOutBuf)
{
	int iRet;
	iRet = icc_Reset((int)lICCardIndex, (uint *)puiRetLen, (uchar *)pvOutBuf);
	if(iRet != ICC_SUCCESS)
	{
		return EM_ICA_COMMERR;
	}
	return EM_ICA_SUCC;
}

long ICPowerDown(long lICCardIndex)
{
	icc_Close((int)lICCardIndex);
	return EM_ICA_SUCC;
}

long ICCardIn(long lICCardIndex,long *plCardPowerStatus)
{
	long lRet;
	*plCardPowerStatus = EM_ICA_CARDPOWERNULL;

	lRet = icc_CheckInSlot((int)lICCardIndex);
	switch(lRet)
	{
	case -ICC_CARDOUT:
		return EM_ICA_CARDOUT;
	case -ICC_ERRORSLOT:
		return EM_ICA_ERRORSLOT;
	case -ICC_CARDNOPOWER:
		*plCardPowerStatus = EM_ICA_CARDPOWEROFF;
		return EM_ICA_CARDOUT;

	default:
	    break;
	}
	*plCardPowerStatus = EM_ICA_CARDPOWERUP;
	return  EM_ICA_SUCC;
}

long ICExchangeAPDU(long lICCardIndex,long  lSendLen, char *pcSendData, long *pulRecLen, char *pcRecData)
{
	long lRet;
	long lRecLen = 0;
	lRet = icc_ExchangeData((int)lICCardIndex, (int)lSendLen, (uchar *)pcSendData, (int *)&lRecLen, (uchar *)pcRecData);
	*pulRecLen = lRecLen;
	switch (lRet)
	{
	case ICC_SUCCESS:
		return EM_ICA_SUCC;
	case -ICC_CARDOUT:
		// ������λ
		return EM_ICA_CARDOUT;
	case -ICC_TIMEOUT:
		// ����ʱ��Ӧ
		return EM_ICA_TIMEOUT;
	case -ICC_COMMERR:
		// ��ͨ�Ŵ���(��ż���LRC��)
		return EM_ICA_RESPERR;
	case -ICC_RESPERR:
		// ��Ƭ������Ϣ����
		return EM_ICA_RESPERR;
	case -ICC_NOPOWER:
		// ��δ�ϵ�
		return EM_ICA_NOPOWER;
	case -ICC_ERRORSLOT:
		// ��������
		return EM_ICA_ERRORSLOT;
	case -ICC_ERRPARA:
		// ��������
	case -ICC_CARDNOPOWER:
		// ����δ�ϵ�
	default:
		return EM_ICA_OTHERERR;
	}
}

long RFID_Local_PowerUp(long lICCardIndex,long *puiRetLen,void *pvOutBuf)
{
#if 1
	int result;
	uint uiLen;
	uchar ucBuff[128];

	result = rfid_powerup(guiCardType, &uiLen, ucBuff);
	if (result == -RFID_ERROR)
	{
		return EM_ICA_OTHERERR;
	}
	else if (result == -RFID_MULTIERR)
	{
// 		lcd_Cls();
// 		lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE, "��ֻ����һ�ſ�");
// 		kb_GetKey(3000);
		return EM_ICA_RFCARDNUMERR;
	}
	else if (result == -RFID_TIMEOUT)
	{
		return EM_ICA_TIMEOUT;
	}
	else if (result == -RFID_NOCARD)
	{
		return EM_ICA_CARDOUT;
	}
	else if (result == RFID_SUCCESS)
	{
		return EM_ICA_SUCC;
	}
	else
	{
		return EM_ICA_OTHERERR;
	}
#else
	return 0;
#endif
}
long RFID_Local_PowerDown(long lICCardIndex)
{
#if 1
	int result;

	result = rfid_powerdown();
	if (result != RFID_SUCCESS)
	{
		return EM_ICA_OTHERERR;
	}

	result = rfid_close();
	if (result != RFID_SUCCESS)
	{
		return EM_ICA_OTHERERR;
	}
	gnOpenRF = 0; //�ѹر���Ƶģ��
	return EM_ICA_SUCC;
#else
	return 0;
#endif
}
long RFID_Local_CardIn(long lICCardIndex,long *plCardPowerStatus)
{
#if 1
//	uint nCardType,uiRetLen;
	int result; //,result1
//	nCardType = 0;
//	uiRetLen  = 0;
	uchar szBuffer[50];
	memset(szBuffer, 0x00, sizeof(szBuffer));
	if (gnOpenRF == 0)
	{
		//δ��
		result = rfid_open(2);
		if (result == -RFID_ERROR)
		{
// 			lcd_Cls();
// 			lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"RFID OPEN=%d",result);
// 			lcd_Display(0,4*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"ʧ�ܻ�֧����Ƶ");
// 			kb_GetKey(3000);
			return EM_ICA_OTHERERR;
		}
		gnOpenRF = 1; //�Ѵ���Ƶģ��
	}
	result = rfid_poll(RFID_MODE_EMV,&guiCardType);
	if (result == RFID_SUCCESS)
	{
// 		result1 = rfid_powerup(nCardType,&uiRetLen,szBuffer);
// 		if (result1 != RFID_SUCCESS)
// 		{
// 			lcd_Cls();
// 			lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"RFID PU=%d",result1);
// 			lcd_Display(0,4*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"ʧ�ܻ�֧����Ƶ");
// 			kb_GetKey(3000);
// 			return EM_ICA_OTHERERR;
// 		}
		return EM_ICA_SUCC;
	}
	else if (result == -RFID_MULTIERR)
	{
// 		lcd_Cls();
// 		lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE, "��ֻ����һ�ſ�");
// 		kb_GetKey(3000);
		return EM_ICA_RFCARDNUMERR;
	}
	else
	{
// 		lcd_Cls();
// 		lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"RFID POLL=%d",result);
// 		lcd_Display(0,4*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"ʧ�ܻ�֧����Ƶ");
// 		kb_GetKey(3000);
		return EM_ICA_OTHERERR;
	}
#else
	return 0;
#endif
}
long RFID_Local_ExchangeAPDU(long lICCardIndex,long  lSendLen, char  *pcSendData, long *pulRecLen, char *pcRecData)
{
#if 1
	int result;

	result = rfid_exchangedata(lSendLen, (uchar *)pcSendData, (uint *)pulRecLen, (uchar *)pcRecData);
	if (result != RFID_SUCCESS)
	{
// 		lcd_Cls();
// 		lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"RFID ex=%d",result);
// 		lcd_Display(0,4*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"����ָ��ʧ��");
// 		kb_GetKey(3000);
		return EM_ICA_OTHERERR;
	}
	return EM_ICA_SUCC;
#else
	return 0;
#endif
}

/************************************************************************/
/*     ȫ�ֺ���ԭ�����ʵ��                                               */
/************************************************************************/

long ICAdapterSelectCard(long lIndex)
{
	long lRet;

	//���ؿ�
	if ((lIndex & 0xf000) == 0)
	{
		if (lIndex == EM_ICA_LOCAL_RFCARD)
		{
			m_struICAdapter.glICAdapterCardIn         =   RFID_Local_CardIn;
			m_struICAdapter.glICAdapterExchangeAPDU   =   RFID_Local_ExchangeAPDU;
			m_struICAdapter.glICAdapterPowerUp        =   RFID_Local_PowerUp;
			m_struICAdapter.glICAdapterPowerDown      =   RFID_Local_PowerDown;
		}
		else
		{
			lRet = (long)icc_InitModule(lIndex,m_struICAdapter.lICCardVCC,m_struICAdapter.lICCardProtocol);
			if(lRet != ICC_SUCCESS)
			{
				return EM_ICA_COMMERR;
			}
			m_struICAdapter.glICAdapterCardIn         =   ICCardIn;
			m_struICAdapter.glICAdapterExchangeAPDU   =   ICExchangeAPDU;
			m_struICAdapter.glICAdapterPowerUp        =   ICPowerUp;
			m_struICAdapter.glICAdapterPowerDown      =   ICPowerDown;
		}
	}
	else
		return EM_ICA_INPUTERR;

	m_struICAdapter.lICCardIndex = (lIndex & 0x7fff);

	return EM_ICA_SUCC;
}

long ICAdapterSetCardPara(ICAPARASTRU strPara)
{
	memcpy((char *)&m_struICAdapter.lICComPortNo,(char *)&strPara.lICComPortNo,sizeof(ICAPARASTRU));
	return EM_ICA_SUCC;
}

long ICAdapterGetCardPara(ICAPARASTRU *pstrPara)
{
	memcpy((char *)&pstrPara->lICComPortNo,(char *)&m_struICAdapter.lICComPortNo,sizeof(ICAPARASTRU));
	return EM_ICA_SUCC;
}
long ICAdapterCardIn(void)
{
	long lCardPowerStatus;
	return m_struICAdapter.glICAdapterCardIn(m_struICAdapter.lICCardIndex,&lCardPowerStatus);
}

long ICAdapterPowerDown(void)
{
	return m_struICAdapter.glICAdapterPowerDown(m_struICAdapter.lICCardIndex);
}

long ICAdapterPowerUp(long *puiRetLen,char *pvOutBuf)
{
	return m_struICAdapter.glICAdapterPowerUp(m_struICAdapter.lICCardIndex, puiRetLen, pvOutBuf);
}

long  ICAdapterExchangeAPDU(long lSendLen, char *pvSendData, long *pulRecLen, char *pvRecData)
{

	return m_struICAdapter.glICAdapterExchangeAPDU(m_struICAdapter.lICCardIndex, lSendLen,pvSendData,pulRecLen, pvRecData);
}

long PubICAdapterSelectCard(long lIndex)
{

	return ICAdapterSelectCard(lIndex);
}
long PubICAdapterCardIn(long lIndex)
{
	ICAdapterSelectCard(lIndex);
	return ICAdapterCardIn();
}
long PubICAdapterPowerDown(long lIndex)
{
	ICAdapterSelectCard(lIndex);
	return ICAdapterPowerDown();
}
long PubICAdapterPowerUp(long lIndex,long *puiRetLen,char *pvOutBuf)
{
	char acTmpBuf[1000];
	long lRet;

	ICAdapterSelectCard(lIndex);
	lRet = ICAdapterPowerUp(puiRetLen,acTmpBuf);
	return lRet;
}

long PubICAdapterExchangeAPDU(long lIndex,long lSendLen, char *pvSendData, long* pulRecLen, char *pvRecData)
{
	long lRet;
//	long i;
//	char acTmpBuf[2048];

	ICAdapterSelectCard(lIndex);
	lRet = ICAdapterExchangeAPDU(lSendLen,pvSendData,pulRecLen,pvRecData);

	//debug
#if 1
	
    TRACE_BUF("\r\n��������:",(uchar *)pvSendData,(uint32_t)lSendLen);

    TRACE_BUF("\r\n��������:",(uchar *)pvRecData,(uint32_t)*pulRecLen);
#else
	for(i = 0;i< lSendLen;i++)
		sprintf(acTmpBuf + i * 3,"%02x ",(uchar)pvSendData[i]);

#endif
	//end

	return lRet;
}

void EMV_CardModule_init(void)
{
	EMV_ICAdapterStruDef stICAdapter;
	stICAdapter.glICAdapterCardIn       =  PubICAdapterCardIn;
	stICAdapter.glICAdapterSelectCard   =  PubICAdapterSelectCard;
	stICAdapter.glICAdapterExchangeAPDU =  PubICAdapterExchangeAPDU;
	stICAdapter.glICAdapterPowerDown    =  PubICAdapterPowerDown;
	stICAdapter.glICAdapterPowerUp      =  PubICAdapterPowerUp;
	emv_ICAdapterSet(stICAdapter);
}
