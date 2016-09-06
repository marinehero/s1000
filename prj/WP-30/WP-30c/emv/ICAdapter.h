#ifndef ICADAPTER_H
#define ICADAPTER_H
/************************************************************************/
/*
��������,
ȫ�ֺ�,
ȫ����������,
*/
/************************************************************************/
//#include "../inc/libapi.h"

typedef struct ICAPARAStru
{
	long  lICComPortNo;
	long  lICComPortBPS;
	long  lICCardVCC;
	long  lICCardProtocol;
	long  lICCardIndex;
}ICAPARASTRU;

#define     EM_ICA_USERCARD       (long)USERCARD         // ����������
#define     EM_ICA_SAM1SLOT       (long)SAM1SLOT         // ����SAM����1
#define     EM_ICA_SAM2SLOT       (long)SAM2SLOT         // ����SAM����2
#define     EM_ICA_SAM3SLOT		  (long)SAM3SLOT         // ����SAM����3
#define     EM_ICA_SAM4SLOT		  (long)SAM4SLOT         // ����SAM����3
#define     EM_ICA_LOCAL_RFCARD	  (long)0x00FF           // ������Ƶ
#define     EM_ICA_RSD_USERCARD	  (long)0x8000           // SD��������
#define     EM_ICA_RSD_SAM1SLOT	  (long)0x8010           // SD������SAM����
#define     EM_ICA_RSD_RFCARD	  (long)0x80FF           // SD��������Ƶ

//��Ƭ״̬
#define     EM_ICA_CARDPOWERUP      (long)0x0001  //��Ƭ�ϵ�
#define     EM_ICA_CARDPOWEROFF     (long)0x0000  //��Ƭδ�ϵ�
#define     EM_ICA_CARDPOWERNULL    (long)-1  //��Ƭδ�ϵ�
/************************************************************************/
/*  ����ֵ��                                                         */
/************************************************************************/
#define  EM_ICA_SUCC               (long)0x0000    // ������ȷִ��
#define  EM_ICA_CARDRESULTERR      (long)0x8000    // ������ȷִ��
#define  EM_ICA_INPUTERR           (long)0x8081    // ��������
#define  EM_ICA_DATAERR            (long)0x8082    // ͨѶ���ݴ���
#define  EM_ICA_COMMERR            (long)0x8083    // �豸����
#define  EM_ICA_ERRORSLOT          (long)0x8001    // ��֧�ָÿ�������
#define  EM_ICA_CARDOUT            (long)0x8002    // ��δ��λ
#define  EM_ICA_POWERUPREADY       (long)0x8003    // ��Ƭ�Ѿ��ϵ�
#define  EM_ICA_NOPOWER            (long)0x8004    // ��Ƭδ�ϵ�򼤻�
#define  EM_ICA_TIMEOUT            (long)0x8006    // ��ƬͨѶ��ʱ
#define  EM_ICA_RESPERR            (long)0x8007    // ��ƬӦ�����ݴ���
#define  EM_ICA_RFHALTERR          (long)0x8008    // ��Ƶ��ƬHALTʧ��
#define  EM_ICA_RFCARDNUMERR       (long)0x8009    // ��Ƶ��Ӧ�����ڶ��ſ�
#define  EM_ICA_OTHERERR           (long)0x80FF    // ��������
/************************************************************************/
/*  �ֲ�����ԭ��                                                         */
/************************************************************************/
long ICPowerUp(long lICCardIndex,long *puiRetLen,void *pvOutBuf);
long ICPowerDown(long lICCardIndex);
long ICCardIn(long lICCardIndex,long *plCardPowerStatus);
long ICExchangeAPDU(long lICCardIndex,long  lSendLen, char  *pcSendData, long *pulRecLen, char *pcRecData);

long RFID_Local_PowerUp(long lICCardIndex,long *puiRetLen,void *pvOutBuf);
long RFID_Local_PowerDown(long lICCardIndex);
long RFID_Local_CardIn(long lICCardIndex,long *plCardPowerStatus);
long RFID_Local_ExchangeAPDU(long lICCardIndex,long  lSendLen, char  *pcSendData, long *pulRecLen, char *pcRecData);
/************************************************************************/
/*  ȫ�ֺ���ԭ��                                                         */
/************************************************************************/
long ICAdapterSelectCard(long lIndex);
long ICAdapterSetCardPara(ICAPARASTRU strPara);
long ICAdapterGetCardPara(ICAPARASTRU *pstrPara);
long ICAdapterCardIn(void);
long ICAdapterPowerDown(void);
long ICAdapterPowerUp(long *puiRetLen,char *pvOutBuf);
long ICAdapterExchangeAPDU(long lSendLen, char *pvSendData, long* pulRecLen, char *pvRecData);
void EMV_CardModule_init(void);


/************************************************************************/
/*  ȫ�ֱ���                                                         */
/************************************************************************/
extern long  glCardType;                 //��Ƭ����


#endif
