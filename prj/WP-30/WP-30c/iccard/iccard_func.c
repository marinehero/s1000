#include "wp30_ctrl.h"

#ifdef CFG_ICCARD


/*-----------------------------------------------------------------------------}
 *  �ⲿ����
 *-----------------------------------------------------------------------------{*/
extern volatile UINT64 gl_char_wait_etu;
extern volatile UINT64 gl_total_atr_etu;
/*-----------------------------------------------------------------------------}
 *  ȫ�ֱ���
 *-----------------------------------------------------------------------------{*/
//----Fi/Di��-----
//   0      1      2      3      4      5      6      7      8      9      10     11     12     13     14     15
//Fi:372    372    558    744    1116   1488   1860   0      0      512    768    1024   1536   2048   0      0
//Di:0      1      2      4      8      16     32     64     12     20     0      0      0      0      0      0
ushort const 	F_Table[16] = {372,372,558,744,1116,1488,1860,0,0,512,768,1024,1536,2048,0,0};
uchar const 	D_Table[16] = {0,1,2,4,8,16,32,64,12,20,0,0,0,0,0,0};

//=====ETU��======
//   0      1      2      3      4      5      6      7      8      9      10     11     12     13     14     15
//1 :372    372    558    744    1116   1488   1860   0      0      512    768    1024   1536   2048   0      0
//2 :186    186    279    372    558    744    930    0      0      256    384    512    768    1024   0      0
//3 :93     93     139    186    279    372    465    0      0      128    192    256    384    512    0      0
//4 :46     46     69     93     139    186    232    0      0      64     96     128    192    256    0      0
//5 :23     23     34     46     69     93     116    0      0      32     48     64     96     128    0      0
//6 :11     11     17     23     34     46     58     0      0      16     24     32     48     64     0      0
//7 :5      5      8      11     17     23     29     0      0      8      12     16     24     32     0      0
//8 :31     31     46     62     93     124    155    0      0      42     64     85     128    170    0      0
//9 :18     18     27     37     55     74     93     0      0      25     38     51     76     102    0      0

//=====EPT - PPS��======
//Di:0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
//1 :93   93   93   93   93   93   93   93   93   93   93   93   93   93   372  93
//2 :372  372  372  372  372  372  372  372  372  372  372  372  372  372  372  372
//3 :186  186  279  372  372  372  372  372  372  372  372  372  372  372  372  372
//4 :93   93   372  186  279  372  372  372  372  372  372  372  372  372  372  372
//5 :372  372  372  93   372  186  372  372  372  372  372  372  372  372  372  372
//6 :372  372  372  372  372  93   372  372  93   372  372  372  372  372  372  372
//7 :372  372  372  372  372  372  372  372  372  372  372  372  372  372  372  372
//8 :372  372  372  372  372  372  372  372  372  372  372  372  372  372  372  372
//9 :93   93   372  93   93   124  155  372  372  372  372  372  372  372  372  372

/* 0��ʾ����Ĭ��9600ͨ��,1��ʾ���Ը���TA1����,2��ʾ������Э�̵�38400*/
const uchar gucPPSEnbleBps[9][16]={
  //0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,
  	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //1	�ж�ӦFֵ0-F���ж�ӦD��ֵ1-9
  	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, //2
  	{1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0}, //3
  	{0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0}, //4
  	{0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0}, //5
  	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //6
  	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //7
  	{2,2,0,2,1,1,1,0,0,0,0,0,0,0,0,0}, //8
  	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0}  //9
};

ICCARD_WORK_STRUCT gl_CardSlotInfo[CARD_SLOTNUM] UPAREA;
/*-----------------------------------------------------------------------------}
 *  ���� 
 *-----------------------------------------------------------------------------{*/
void s_InitCardInfo(void)
{
//	uchar Init_temp;
    gl_CardSlotInfo[gl_CardInfo.current_slot].CardPowerOn = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].ResetStatus = 0x00;
	gl_CardSlotInfo[gl_CardInfo.current_slot].T = 0;
	
	switch(gl_CardSlotInfo[gl_CardInfo.current_slot].BaudRate)
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
//	Init_temp = gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode;
//	gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode = Init_temp & 0xff;
	gl_CardSlotInfo[gl_CardInfo.current_slot].PPSENABLE = 0x00;
	gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC = 0x20;
	gl_CardSlotInfo[gl_CardInfo.current_slot].IFSD = 0xFE;

	gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = 10081;
	gl_CardSlotInfo[gl_CardInfo.current_slot].CWT = 48;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = 16331;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP = 16331;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BGT = 16;

	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0x90;
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0x00;

	gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = 0x00;
	gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB = 0x00;

	// ��λ����ATR�ӿ��ַ�
	memset((uchar*)&gl_CardSlotInfo[gl_CardInfo.current_slot].TS, 0x00, 23);
}

void icc_set_RecTime(void)
{
#if 2
	gl_CardInfo.Rec_P15 =  (uint)(15 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P25 =  (uint)(25 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P35 =  (uint)(35 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P45 =  (uint)(45 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P55 =  (uint)(55 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P65 =  (uint)(65 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P75 =  (uint)(75 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P85 =  (uint)(85 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P95 =  (uint)(95 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P100 = (uint)(100 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P105 = (uint)(105 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P110 = (uint)(110 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
	gl_CardInfo.Rec_P120 = (uint)(120 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU / 10);
#else
	gl_CardInfo.Rec_P15 =  (uint)(1.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P25 =  (uint)(2.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P35 =  (uint)(3.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P45 =  (uint)(4.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P55 =  (uint)(5.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P65 =  (uint)(6.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P75 =  (uint)(7.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P85 =  (uint)(8.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P95 =  (uint)(9.5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P100 = (uint)(10  * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P105 = (uint)(10.5* gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P110 = (uint)(11 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
	gl_CardInfo.Rec_P120 = (uint)(12 *  gl_CardSlotInfo[gl_CardInfo.current_slot].ETU);
#endif
}


void icc_InitICCTiming(void)
{
	//gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = 372;
	gl_CardSlotInfo[gl_CardInfo.current_slot].TS = 0xFF;
//D2(TRACE("\r\n[ETU0:%d]",gl_CardSlotInfo[gl_CardInfo.current_slot].ETU););
    icc_set_RecTime();

	gl_CardInfo.Parity_Check_Enable = 0x00;
	gl_CardInfo.k_Ic_CharWait_TimeCount = 10081;
	gl_CardInfo.k_Ic_LastSendByte = 0;
}

void icc_adjustTiming(void)
{
//D2(TRACE("\r\n[ETU1:%d]",gl_CardSlotInfo[gl_CardInfo.current_slot].ETU););
	// �����ֽڽ���ʱ���
    icc_set_RecTime();
	// �����ֽڷ���ʱ���
	gl_CardInfo.Send_P1 = 1 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P2 = 2 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P3 = 3 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P4 = 4 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P5 = 5 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P6 = 6 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P7 = 7 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P8 = 8 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P9 = 9 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P10 = 10 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P11 = 11 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
	gl_CardInfo.Send_P13 = 13 * gl_CardSlotInfo[gl_CardInfo.current_slot].ETU;
}

// ����ֵ˵��:
// 0   --- �ɹ�
// -1 --- ��ʱʧ��
// -2 --- TS��
// -3 --- ��żУ���
// -4 --- ATR���ݴ�
static int icc_GetAtr(uchar *Buff)
{
	uchar	i = 0,Flag = 0,x = 0,y,z,atr_x;
	uchar	*Lenth_Point;
	//uchar	*ptr_atr;
	uchar	T = 0,edc = 0,TCK = 0,CWI = 0,BWI = 0;
	ushort	N = 0,j = 0;
	uchar	Lenth = 2;
	uint  	Fi = 0;
	uint  	Di = 0;
	uint  	ETU = 0;
	uint k;
	edc = 0;
	T = 0;

	gl_CardInfo.k_total_timeover = 0x00;
	gl_CardInfo.Parity_Check_Enable = 0x00;

	gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag = 0;
	gl_CardSlotInfo[gl_CardInfo.current_slot].TS = 0xFF;

	Lenth_Point = Buff;

	// ��ȡTS�ֽ�
	*Buff = 0x00;
	*++Buff = icc_RecvByte();
	if(gl_CardInfo.k_IccComErr)
	{
		if(gl_CardInfo.k_IccErrTimeOut)
		{
            return ATR_TIMEOUT;
		}
		else if(gl_CardInfo.k_IccErrPar)
		{
            return ATR_PARERR;
		}
		else
		{
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode != SHBMODE)
			{
            	return ATR_TSERR;
			}
		}
	}
	gl_CardSlotInfo[gl_CardInfo.current_slot].TS = *Buff;

	//�籣�淶��֧��TS!=3F�Ŀ�
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		if(gl_CardSlotInfo[gl_CardInfo.current_slot].TS!= 0x3b)
		{
			return ATR_TSERR_SHB;
		}
	}

	edc = 0;
	//ptr_atr = Buff;

	// ��ȡT0�ֽ�
	*++Buff = icc_RecvByte();
	if(gl_CardInfo.k_IccComErr)
	{
		if(gl_CardInfo.k_IccErrTimeOut)
		{
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
			{
				return ATR_TIMEOUT_SHB;
			}
			else
			{
				return ATR_TIMEOUT;
			}
		}
		else
		{
			return ATR_PARERR;
		}
	}

	edc = edc^*Buff;
	Flag = *Buff;
	// ������ʷ�ֽڸ���
	x = (Flag & 0x0f);
	i = 1;
	for(;;)
	{
		if((Flag & 0x10) == 0x10)
		{   /* TA */
			*++Buff = icc_RecvByte();
			if(gl_CardInfo.k_IccComErr)
			{
				if(gl_CardInfo.k_IccErrTimeOut)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
					{
						return ATR_TIMEOUT_SHB;
					}
					else
					{
						return ATR_TIMEOUT;
					}
				}
				else
				{
					return ATR_PARERR;
				}
			}

			if(i == 1)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag |= 0x01;
			}
			else if(i == 2)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TA2 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag |= 0x02;
			}
			else if(i == 3)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TA3 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag |= 0x04;
			}
			else
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TA4 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag |= 0x08;
			}
			edc = edc^*Buff;
			Lenth++;
		}

		if((Flag&0x20) == 0x20)
		{	/* TB  */

			*++Buff = icc_RecvByte();
			if(gl_CardInfo.k_IccComErr)
			{
				if(gl_CardInfo.k_IccErrTimeOut)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
					{
						return ATR_TIMEOUT_SHB;
					}
					else
					{
						return ATR_TIMEOUT;
					}
				}
				else
				{
					return ATR_PARERR;
				}
			}

			if(i == 1)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TB1 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag |= 0x01;
			}
			else if(i == 2)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TB2 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag |= 0x02;
			}
			else if(i == 3)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TB3 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag |= 0x04;
			}
			else
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TB4 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag |= 0x08;
			}
			edc = edc^*Buff;
			Lenth++;
		}

		if((Flag&0x40) == 0x40)
		{   /* TC */
			*++Buff = icc_RecvByte();
			if(gl_CardInfo.k_IccComErr)
			{
				if(gl_CardInfo.k_IccErrTimeOut)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
					{
						return ATR_TIMEOUT_SHB;
					}
					else
					{
						return ATR_TIMEOUT;
					}
				}
				else
				{
					return ATR_PARERR;
				}
			}

			if(i == 1)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TC1 = *Buff;
                gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag |= 0x01;
			}
			else if(i == 2)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TC2 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag |= 0x02;
			}
			else if(i == 3)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TC3 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag |= 0x04;
			}
			else
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TC4 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag |= 0x08;
			}
			edc = edc^*Buff;
			Lenth++;
		}

		if((Flag & 0x80) == 0x00)
		{	/*	no TD  */
			for(i = 0; i < x; i++)
			{
				*++Buff = icc_RecvByte();
				if(gl_CardInfo.k_IccComErr)
				{
					if(gl_CardInfo.k_IccErrTimeOut)
					{
						if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
						{
							return ATR_TIMEOUT_SHB;
						}
						else
						{
							return ATR_TIMEOUT;
						}
					}
					else
					{
						return ATR_PARERR;
					}
				}
				edc = edc ^ (*Buff);
			}

			Lenth = Lenth + x;

			if(T)
			{  /* ���ΪT<>0Э�鿨Ƭһ����TCK  */

				*++Buff = icc_RecvByte();
				if(gl_CardInfo.k_IccComErr)
				{
					if(gl_CardInfo.k_IccErrTimeOut)
					{
						if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
						{
							return ATR_TIMEOUT_SHB;
						}
						else
						{
							return ATR_TIMEOUT;
						}
					}
					else
					{
						return ATR_PARERR;
					}
				}

				TCK = *Buff;
				if(edc != TCK)
				{
					return ATR_TCKERR;
				}
				Lenth++;
			}

			*Lenth_Point = Lenth;
			// �رն�ʱ�����
			gl_char_wait_etu = 0;
			gl_total_atr_etu = 0;
			icc_close_etu_timer();
			icc_disable_inter_timer();

			gl_CardSlotInfo[gl_CardInfo.current_slot].PPSENABLE = 0;   // Ĭ�Ͻ�ֹPPSЭ��

			/* ֧��TA1 =  '11��'12'	'13'ֵ��ATR  */
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x02)
			{
				//EMVģʽ������TA1,TA2������b5=0��TA1������0x11~0x13��Χ
				//         ����TA1,TA2������,����F=372 D=1����Э��
				//         ������TA1,����F=372 D=1
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					y = gl_CardSlotInfo[gl_CardInfo.current_slot].TA2;
					if((y & 0x10) || ((y & 0x0F) != (gl_CardSlotInfo[gl_CardInfo.current_slot].TD1 & 0x0F)))
					{
						//  TA2��Bit5��Ϊ0�����ߵ���λ��ΪTD1�ĵ���λ
						return ATR_TA2ERR;
					}
				}

				if(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x01)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
					{
						if((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 < 0x11) || (gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 > 0x13))
						{
							return ATR_TA1ERR;
						}
					}
				}
			}
			if(!(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x02))
			{
				// TA2�����ڣ�Ĭ�ϰ���TA1 = 11����
				if((gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == ISOMODE) && (gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x01))
				{
					// ISOģʽ������PPSЭ��
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].D == 1 && gl_CardSlotInfo[gl_CardInfo.current_slot].ETU == 372)
					{
						gl_CardSlotInfo[gl_CardInfo.current_slot].PPSENABLE = 1;
					}
				}
				//�籣��TA2��TA1Ӧ����Э��ģʽ����TA1����
				if((gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
					&& (gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x01))
				{
					// �籣ģʽ������PPSЭ��
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].D == 1 && gl_CardSlotInfo[gl_CardInfo.current_slot].ETU == 372)
					{
						gl_CardSlotInfo[gl_CardInfo.current_slot].PPSENABLE = 1;
					}
				}
			}
			else if(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x01)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
				{
					 //TA2����������籣Ҫ��
	                Fi = (gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) >> 4;
	                Di = gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F;
	                ETU = F_Table[Fi]/D_Table[Di];
					gl_CardSlotInfo[gl_CardInfo.current_slot].D = D_Table[Di];
					gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = ETU;
				}
				else
				{
				    //TA2���������
	                Fi = (gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) >> 4;
					if(Fi == 0x07 || Fi == 0x08 || Fi > 0x0D)
					{
						// ISO7816 F��Χ0~6 9~D
						return ATR_TA1ERR;
					}
	                Di = gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F;
					if(Di == 0x00 || Di == 0x07 || Di > 0x09)
					{
						// ISO7816 D��Χ1~6  8~9
						return ATR_TA1ERR;
					}
	                ETU = F_Table[Fi]/D_Table[Di];
					gl_CardSlotInfo[gl_CardInfo.current_slot].D = D_Table[Di];
					gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = ETU;
				}
			}

			/*	�临λ����TB1ֵ��TB1!=0ʱ������ȸ�λ	*/
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].ResetStatus == 0x00)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag & 0x01) == 0x00) || (gl_CardSlotInfo[gl_CardInfo.current_slot].TB1 != 0x00))
					{
						return ATR_TB1ERR;
					}
				}
			}


			if((gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag & 0x01) == 0x01)
			{
				//�籣������TD1��Ӧ�÷��ص�λΪ0
				if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TD1 & 0x0f) != 0x00)
					&& (gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE))
				{
					return ATR_TD1ERR;
				}
				/* TD1���� */
				if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TD1 & 0x0f) > 1) && (gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE))
				{
					return ATR_TD1ERR;
				}

				if(gl_CardSlotInfo[gl_CardInfo.current_slot].TD1 & 0x0f)
				{
					// TD1��T=1  TD2��T!=1�������
					if((gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag&0x02) && ((gl_CardSlotInfo[gl_CardInfo.current_slot].TD2&0x0f) != 0x01))
					{
						if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
						{
							return ATR_TD2ERR;
						}
					}
				}
				else
				{
					if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag & 0x02)
						&&(gl_CardSlotInfo[gl_CardInfo.current_slot].TD2 & 0x0f) != 0x0f)
						&& (gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE))
					{
						return ATR_TD2ERR;
					}
					//TD1��T=0
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag & 0x02)
					{
						// TD2�� T!=1��T!=E�����,��Ϊ��TD2�ĸ߰��ֽ�Ϊ����ֵ�ҵͰ��ֽڵ�ֵΪ��1����E��
						if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TD2 & 0x0F) != 0x01) && ((gl_CardSlotInfo[gl_CardInfo.current_slot].TD2 & 0x0F) != 0x0E))
						{
							if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
							{
								return ATR_TD2ERR;
							}
						}
						else
						{
							if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TD2 & 0x0F) == 0x0E)
								&& ((gl_CardSlotInfo[gl_CardInfo.current_slot].TD1 & 0x0F) != 0x00))
							{
								if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
								{
									return ATR_TD2ERR;
								}
							}
						}
					}
				}
			}

			/* ��Ч��TB2�ֽ� */
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag & 0x02)
				{
					return ATR_TB2ERR;
				}

				/* ��Ч��TC2�ֽ� */
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag & 0x02)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].TC2 != 0x0A)
					{
						return ATR_TC2ERR;
					}
				}
			}
			//�籣�������ȴ�ʱ��
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
			{
				if((gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag & 0x02) == 0)
				{
					gl_CardSlotInfo[gl_CardInfo.current_slot].TC2 = 0x0A;
				}
				y = gl_CardSlotInfo[gl_CardInfo.current_slot].D;
				atr_x = gl_CardSlotInfo[gl_CardInfo.current_slot].TC2;
				gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = (960 * y * atr_x);
				atr_x = gl_CardSlotInfo[gl_CardInfo.current_slot].WWT;
				gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = atr_x + 1 + 113;
			}
			else
			{
                gl_CardSlotInfo[gl_CardInfo.current_slot].TC2 = 0x0A;
                z = gl_CardSlotInfo[gl_CardInfo.current_slot].TC2;
				y = gl_CardSlotInfo[gl_CardInfo.current_slot].D;
				gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = (960 * y * z) + (480 * y);
				gl_CardSlotInfo[gl_CardInfo.current_slot].WWT += 1;
			}


			//�籣��Ӧ��TA3
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
			{
				if((gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x04) != 0x00)
				{
					if((gl_CardSlotInfo[gl_CardInfo.current_slot].TA3 & 0x3c) != 0x00)
					{
						return ATR_TA3ERR;
					}
				}
				else
				{
					if(T == 15)
					{
						gl_CardSlotInfo[gl_CardInfo.current_slot].TA3 = 0x01;
					}
				}
				atr_x = gl_CardSlotInfo[gl_CardInfo.current_slot].TA3;
				gl_CardSlotInfo[gl_CardInfo.current_slot].XI = (atr_x & 0xc0)>>6;
				gl_CardSlotInfo[gl_CardInfo.current_slot].UI = atr_x & 0x03;
			}
			else
			{
				/*	��Ч��TA3�ֽ� */
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x04)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].TA3 < 0x10)
					{
						return ATR_TA3ERR;
					}

					if(gl_CardSlotInfo[gl_CardInfo.current_slot].TA3 == 0xFF)
					{
						return ATR_TA3ERR;
					}
				}
				else
				{
					gl_CardSlotInfo[gl_CardInfo.current_slot].TA3 = 0x20;
				}
				y = gl_CardSlotInfo[gl_CardInfo.current_slot].TA3;
				gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC = y;
			}

			/*	��Ч��TB3�ֽ� */
			if(T == 1)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					if((gl_CardSlotInfo[gl_CardInfo.current_slot].TBFlag & 0x04) == 0x00)
					{
						return ATR_TB3ERR;
					}
				}

				CWI = gl_CardSlotInfo[gl_CardInfo.current_slot].TB3 & 0x0F;
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					if(CWI > 5)
					{
						return ATR_TB3ERR;
					}
				}
				// ����CWTʱ��CWT=2^CWI+11+4+1
				gl_CardSlotInfo[gl_CardInfo.current_slot].CWT = ((uint)0x01 << CWI) + 11 + 5;

				BWI = (gl_CardSlotInfo[gl_CardInfo.current_slot].TB3 >> 4)&0x0f;
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					if(BWI > 4)
					{
						return ATR_TB3ERR;
					}
				}
				// ����BWTʱ��
				// BWT=[2^BWI ��960 ��372D/F]+11+D*960      ��Fi!=372ʱ 
				// BWT=[2^BWI ��960 + 11] + D*960           ��Fi=372ʱ
				k = (((uint)1 << BWI) * 960 *gl_CardSlotInfo[gl_CardInfo.current_slot].D) + 11;
				k += (gl_CardSlotInfo[gl_CardInfo.current_slot].D * 960);
                k += 1;
				gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = k;
				gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP = k;

				// �ʵ��ӳ������ַ����
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].TC1 == 0xFF)
				{
					N = 0;
				}
				else
				{
					N = gl_CardSlotInfo[gl_CardInfo.current_slot].TC1 + 1;
				}
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					j = (ushort)0x01 << CWI;
					if(j <= N)
					{
						return ATR_TB3ERR;
					}
				}
			}

			/* ��Ч��TC3ֵ	*/
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].TCFlag & 0x04)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == EMVMODE)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].TC3)
					{
						return ATR_TC3ERR;
					}
				}
			}

			//��TS����󳤶�Ϊ32���ֽ�
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
			{
				if(TCK != 0)
				{
					if(T != 15)
					{
						return ATR_TCKERR;
					}
				}
				if((Lenth - 1) > 32)
				{
					return ATR_LENGTHERR;
				}
			}
			return ICC_SUCCESS;
		}

		if(Flag & 0x80)
		{   //  TD1  TD2
			*++Buff = icc_RecvByte();
			if(gl_CardInfo.k_IccComErr)
			{
				if(gl_CardInfo.k_IccErrTimeOut)
				{
					if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
					{
						return ATR_TIMEOUT_SHB;
					}
					else
					{
						return ATR_TIMEOUT;
					}
				}
				else
				{
					return ATR_PARERR;
				}
			}

			if(i == 1)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TD1 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag |= 0x01;
				T = *Buff&0x0f;
			}
			else if(i == 2)
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TD2 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag |= 0x02;
				//���TD2���ֽڵ���E,��ҲҪ����TCK
				T |= *Buff&0x0f;
			}
			else
			{
				gl_CardSlotInfo[gl_CardInfo.current_slot].TD3 = *Buff;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TDFlag |= 0x04;
				T |= *Buff&0x0f;
			}

			edc = edc^*Buff;
			Lenth++;
			Flag = *Buff;
		}
		i++;
	}
}

int  icc_ColdReset(uchar *ATR)
{
	int Ret = 0;

	gl_CardInfo.k_total_timeover = 0;
	gl_CardInfo.k_timeover = 0;
	gl_CardInfo.k_IccComErr = 0;
	gl_CardInfo.k_IccErrTimeOut = 0;

	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;

	icc_init_etu_timer();
	icc_init_inter_timer();
	// �����ͷ����д���
	icc_ClrVCC();
	icc_SetIOOutput();
	icc_ClrRST();
	icc_ClrIO();
	icc_close_clk();
	icc_SetVCC(gl_CardSlotInfo[gl_CardInfo.current_slot].VolMode);
	icc_SetIO();
	icc_SetIOInput();
	icc_open_clk();        // ��ʱ��
	icc_delay_clock(42500); //ʱ��=3.104M ��ʱ42500CLK*(1/3.104M)= 13.69ms

	// �ȴ�IC��42000CLK�ڻ���TS�ֽ�
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		gl_CardInfo.k_Ic_CharWait_TimeCount = 9600;
	}
	else
	{
		gl_CardInfo.k_Ic_CharWait_TimeCount = 10081;
	}
    gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;

	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE) {
		//�籣40000*0.322=12.88ms
		gl_CardInfo.k_Ic_ATR_TotalTimeCount = 40000;
    } else if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == CITYUNIONMODE) {
        //Ŀǰ��1�ſ���λ��IOӦ��ʱ����Ҫ22ms,����һ��ֻҪ5ms
		gl_CardInfo.k_Ic_ATR_TotalTimeCount = 90000;        
    } else {
		gl_CardInfo.k_Ic_ATR_TotalTimeCount = 42000 + 25;
	}
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
    	gl_CardInfo.k_IC_TotalATRTime = 19200;
	}
	else
	{
		gl_CardInfo.k_IC_TotalATRTime = 20161;
	}
	icc_SetRST();                   // ��RST�ź��ø�
    if ( gl_CardInfo.k_Ic_ATR_TotalTimeCount > 65564) {
        gl_total_atr_etu = 2;
        icc_start_etu_timer(gl_CardInfo.k_Ic_ATR_TotalTimeCount/2);      // �ȴ�TS�ֽ�ʱ��
    } else {
        gl_total_atr_etu = 1;
        icc_start_etu_timer(gl_CardInfo.k_Ic_ATR_TotalTimeCount);      // �ȴ�TS�ֽ�ʱ��
    }

	icc_DisableOtherInt();
	Ret = icc_GetAtr(ATR);
    icc_EnableOtherInt();
	if(Ret == ICC_SUCCESS)
	{
		icc_adjustTiming();
	}
	icc_close_etu_timer();
	icc_disable_inter_timer();
	gl_total_atr_etu = 0;
	gl_char_wait_etu = 0;

	return Ret;
}

int  icc_WarmReset(uchar *ATR)
{
	int Ret;
	memset((uchar*)&gl_CardSlotInfo[gl_CardInfo.current_slot].TS, 0x00, 23);
	gl_CardInfo.k_total_timeover = 0;
	gl_CardInfo.k_timeover = 0;
	gl_CardInfo.k_IccComErr = 0;
	gl_CardInfo.k_IccErrTimeOut = 0;

	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;

	icc_init_etu_timer();
	icc_init_inter_timer();
	icc_ClrRST();         // ��RST�õͣ������ȸ�λ����
	icc_SetIOInput();
	gl_CardInfo.k_Ic_ATR_TotalTimeCount = 42500;  // ��ʱ42500CLK
	icc_delay_clock(gl_CardInfo.k_Ic_ATR_TotalTimeCount);

	// �ȴ�IC��42000CLK�ڻ���TS�ֽ�
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		gl_CardInfo.k_Ic_CharWait_TimeCount = 9600;
	}
	else
	{
		gl_CardInfo.k_Ic_CharWait_TimeCount = 10081;
	}
    gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE) {
		//�籣40000*0.322=12.88ms
		gl_CardInfo.k_Ic_ATR_TotalTimeCount = 40000;
    } else if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == CITYUNIONMODE) {
        //Ŀǰ��1�ſ���λ��IOӦ��ʱ����Ҫ22ms,����һ��ֻҪ5ms
		gl_CardInfo.k_Ic_ATR_TotalTimeCount = 90000;        
    } else {
		gl_CardInfo.k_Ic_ATR_TotalTimeCount = 42000 + 25;
	}
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
    	gl_CardInfo.k_IC_TotalATRTime = 19200;
	}
	else
	{
		gl_CardInfo.k_IC_TotalATRTime = 20161;
	}
	icc_SetRST();                                          // ��RST�ź��ø�
    if ( gl_CardInfo.k_Ic_ATR_TotalTimeCount > 65564) {
        gl_total_atr_etu = 2;
        icc_start_etu_timer(gl_CardInfo.k_Ic_ATR_TotalTimeCount/2);      // �ȴ�TS�ֽ�ʱ��
    } else {
        gl_total_atr_etu = 1;
        icc_start_etu_timer(gl_CardInfo.k_Ic_ATR_TotalTimeCount);      // �ȴ�TS�ֽ�ʱ��
    }

	icc_DisableOtherInt();
	Ret = icc_GetAtr(ATR);
    icc_EnableOtherInt();
	if(Ret == ICC_SUCCESS)
	{
		icc_adjustTiming();
	}
	icc_close_etu_timer();
	icc_disable_inter_timer();
	gl_total_atr_etu = 0;
	gl_char_wait_etu = 0;
	return Ret;
}

static int icc_t0Exchange(int len, uchar *inbuff, uchar *outbuff, uchar *rslt)
{
	uint    i,len1,len2;
	uint    reclen = 0;
	int		Ret;
	uchar	tempsw1,tempsw2;
	uchar	*ptr1,*ptr2,status[4],cmdbuf[5];
	uchar	cmd_case,is_rece_send;
	uchar	FirstByte_Is_Status;
	uchar	Change_Status_Word;
	gl_CardInfo.Parity_Check_Enable = 0x01;
	// �����ַ������ʱ��
	gl_CardInfo.k_Ic_CharWait_TimeCount = gl_CardSlotInfo[gl_CardInfo.current_slot].WWT;
    gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;

	cmd_case = *rslt;  /* ����CASE�� 1��2��3��4*/

	FirstByte_Is_Status = 1; /* ��һ���ֽ�Ϊ״̬�ֽ� ��1-�ǣ�0-�� */
	Change_Status_Word = 0;  /* ���·���״̬�ֽڱ�־��1-���� */

	gl_CardInfo.k_timeover = 0x00;
	is_rece_send = 0;        /* ��ǰ�������ݰ����������״̬��0=���գ�1=���� */
	if(cmd_case == 0x01)
	{
		is_rece_send = 0;
	}
	else if(cmd_case == 0x02)
	{
		is_rece_send = 0;
	}
	else if(cmd_case == 0x03)
	{
		is_rece_send = 1;
	}
	else
	{
		is_rece_send = 1;
	}

	/* ������ͷ��  */
	cmdbuf[0] = inbuff[0];
	cmdbuf[1] = inbuff[1];
	cmdbuf[2] = inbuff[2];
	cmdbuf[3] = inbuff[3];
	cmdbuf[4] = inbuff[4];
	if(cmd_case == 0x01)
	{
		cmdbuf[4] = 0x00;
	}
	status[0] = 0xff;
	status[1] = 0xff;
	status[2] = 0xff;//����INS OR ~INS
	status[3] = 0xff;//��־INS�Ƿ���յ�2�Ρ���=0��

	reclen = 0;
	ptr1 = inbuff+5;
	ptr2 = outbuff + 2;

	/*	��IC��0ʱ���ֲ�������ʱ����  */
	tempsw1 = 0xFF;
	tempsw2 = 0xFF;


	while(1)
	{
flg1:
		icc_init_etu_timer();
		icc_init_inter_timer();
		gl_char_wait_etu = 0;
		gl_total_atr_etu = 0;

		icc_delay_etu(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU, 8);
		len1 = (uint)cmdbuf[4];  /*  �������ݳ���  */
		len2 = (uint)cmdbuf[4];  /*  �������ݳ���  */
//		if(len2 == 0)
//		{
//			//������ճ���00,��Ҫ����256 yehf 2011-10-18 modify
//			len2 = 256;
//		}
		Ret = icc_SendByte(cmdbuf[0]);   /* ����CLA  */
		if(Ret)
		{
			return T0_MORESENDERR;
		}

		Ret = icc_SendByte(cmdbuf[1]);   /* ����INS  */
		if(Ret)
		{
			return T0_MORESENDERR;
		}

		Ret = icc_SendByte(cmdbuf[2]);   /* ����P1   */
		if(Ret)
		{
			return T0_MORESENDERR;
		}

		Ret = icc_SendByte(cmdbuf[3]);   /*  ����P2  */
		if(Ret)
		{
			return T0_MORESENDERR;
		}

		gl_CardInfo.k_Ic_LastSendByte = 1;
		Ret = icc_SendByte(cmdbuf[4]);   /* ����P3   */
		gl_CardInfo.k_Ic_LastSendByte = 0;
		if(Ret)
		{
			return T0_MORESENDERR;
		}

flg2:
		status[0] = icc_RecvByte(); /* ��ȡһ�����ֽ� */
		if(gl_CardInfo.k_IccComErr)
		{
			if(gl_CardInfo.k_IccErrPar)
			{
				return T0_PARERR;
			}
			else
			{
				return T0_TIMEOUT;
			}
		}

		if(status[0] == cmdbuf[1])
		{   /*  �����ֽڵ���INS----���ͻ������������ݰ�	*/
			if(is_rece_send)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
				{
					if(status[3] != 0xff)
					{
						status[2] = status[0];
						goto flg2;
					}

				}

				/* �������ݰ�  */
				icc_delay_etu(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU, 8);
				for(i = 0; i < len1; i++)
				{
					if(i == (len1-1))
					{
						gl_CardInfo.k_Ic_LastSendByte = 1;
					}
					Ret = icc_SendByte(*ptr1);
					gl_CardInfo.k_Ic_LastSendByte = 0;
					if(Ret)
					{
						return T0_MORESENDERR;
					}
					ptr1++;
				}

				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
				{
					status[3] = 0x00;
				}

				FirstByte_Is_Status = 0;
				goto flg2;	      /* �ȴ���һ�������ֽ�  */
			}
			else
			{  /*  �������ݰ�  */
				if(len2 == 0)
				{
					//������ճ���00,��Ҫ����256 yehf 2011-10-18 modify
					len2 = 256;
				}
				reclen += len2;
				if(reclen > 256)
				{
					return (-ICC_RESPERR);
				}
				for(i = 0; i < len2; len2--)
				{
					*ptr2 = icc_RecvByte();
					if(gl_CardInfo.k_IccComErr)
					{
						if(gl_CardInfo.k_IccErrPar)
						{
							return T0_PARERR;
						}
						else
						{
							return T0_TIMEOUT;
						}
					}
					ptr2++;
				}
				FirstByte_Is_Status = 0;
				goto flg2;
			}
		}
		else if(status[0] == (uchar)(~cmdbuf[1]))
		{	/* ����INS�Ĳ���  ���ջ�����һ������  */
			if(is_rece_send)
			{
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
				{
					if(len1 == 0)
					{
						status[2] = status[0];
						goto flg2;
					}
				}
				/* ����һ�ֽ�����	*/
				icc_delay_etu(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU, 8);
				gl_CardInfo.k_Ic_LastSendByte = 1;
				Ret = icc_SendByte(*(ptr1));
				gl_CardInfo.k_Ic_LastSendByte = 0;
				if(Ret)
				{
					return T0_MORESENDERR;
				}
				ptr1++;
				len1--;
				FirstByte_Is_Status = 0;
				goto flg2;
			}
			else
			{/*  ������һ�ֽ�����  */
				*ptr2 = icc_RecvByte();
				if(gl_CardInfo.k_IccComErr)
				{
					if(gl_CardInfo.k_IccErrPar)
					{
						return T0_PARERR;
					}
					else
					{
						return T0_TIMEOUT;
					}
				}
				ptr2++;
				reclen++;
				if(len2 == 0)
				{
					//������ճ���00,��Ҫ����256 yehf 2011-10-18 modify
					len2 = 256;
				}
				len2--;
				FirstByte_Is_Status = 0;

				if(reclen > 256)
				{
					return (-ICC_RESPERR);
				}
				goto flg2;
			}
		}
		else if(status[0] == 0x60)
		{
			FirstByte_Is_Status = 0;
			goto flg2;
		}

		if(((status[0]&0xf0)!= 0x60)&&((status[0]&0xf0) != 0x90))
		{  /* �յ�����Ĺ��̻�״̬�ֽں��ͷŴ��� */
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode != SHBMODE)
			{
				return T0_INVALIDSW;
			}
		}

		status[1] = icc_RecvByte();
		if(gl_CardInfo.k_IccComErr)
		{
			if(gl_CardInfo.k_IccErrPar)
			{
				return T0_PARERR;
			}
			else
			{
				return T0_TIMEOUT;
			}
		}

		/* �Զ���Ӧ  */
		if((status[0] == 0x61) && ((cmd_case == 0x02) || (cmd_case == 0x04)))
		{
			cmdbuf[0] = 0x00;
			cmdbuf[1] = 0xc0;
			cmdbuf[2] = 0x00;
			cmdbuf[3] = 0x00;
			cmdbuf[4] = status[1];
			is_rece_send = 0;  /* ����Ĳ���ֻ�ܴ�IC���������ݰ� */
			FirstByte_Is_Status = 0;
			if(cmd_case == 0x04)
			{
				cmd_case = 0x02;
			}
			goto flg1;		 /* �����¸�����ͷ��  */
		}

		if(status[0] == 0x6C)
		{
			cmdbuf[4] = status[1];
			is_rece_send = 0;
			FirstByte_Is_Status = 0;
			if(cmd_case == 0x04)
			{
				cmd_case = 0x02;
			}
			goto flg1;
		}

		if((cmd_case == 0x04) && (FirstByte_Is_Status == 0))
		{
			if((status[0] == 0x62)
				||(status[0] == 0x63)
				||(((status[0]&0xf0) == 0x90) && ((status[0]!=0x90) || (status[1] != 0x00))))
			{
				cmdbuf[0] = 0x00;
				cmdbuf[1] = 0xc0;
				cmdbuf[2] = 0x00;
				cmdbuf[3] = 0x00;
				cmdbuf[4] = 0x00;
				is_rece_send = 0;
				Change_Status_Word = 1;

				cmd_case = 0x02; /* �޸�CASE״̬��ֹ�ط� */

				tempsw1 = status[0];
				tempsw2 = status[1];
				goto flg1;
			}
		}

		if(Change_Status_Word)
		{
			*rslt = tempsw1;
			*(rslt+1) = tempsw2;
		}
		else
		{
			if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
			{
				if(status[2] != 0xff)
				{
					*rslt = status[2];
					*(rslt + 1) = status[0];
				}
				else
				{
					*rslt = status[0];
					*(rslt + 1) = status[1];
				}
			}
			else
			{
				*rslt = status[0];
				*(rslt + 1) = status[1];
			}
		}
		*outbuff = reclen/256;
		*(outbuff + 1) = reclen%256; /* ������յ����ݳ���  */

		return ICC_SUCCESS;
	}
}

static int  icc_t1send(uchar *Inbuf)
{
	uchar	edc = 0x00;
	//int Ret = 0;
	uint	i;
        //uint len;

	//len = Inbuf[2]+3;   /*	�����ܳ�NAD+PCB+LEN+��LEN��DATA����*/
	edc = 0;			/*	��������������Ԫ��У��EDC */
LABLE(D_T1_SEND);
	gl_CardInfo.k_Ic_CharWait_TimeCount = gl_CardSlotInfo[gl_CardInfo.current_slot].BWT;
	gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;

	icc_init_etu_timer();
	icc_init_inter_timer();
	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;

	icc_delay_etu(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU, 22);
	icc_SendByte(Inbuf[0]);  // ����NAD
	edc ^= Inbuf[0];

	icc_SendByte(Inbuf[1]);  // ����PCB
	edc ^= Inbuf[1];

	icc_SendByte(Inbuf[2]);  // ����LEN
	edc ^= Inbuf[2];

	for(i = 0; i < Inbuf[2]; i++)
	{
		icc_SendByte(Inbuf[3+i]);  // ����LEN���ȵ�������
		edc ^= Inbuf[3+i];
	}

	gl_CardInfo.k_Ic_LastSendByte = 1;
	icc_SendByte(edc);   // ����EDC�ֽ�
	gl_CardInfo.k_Ic_LastSendByte = 0;
	return ICC_SUCCESS;
}

static int  icc_t1recv(uchar *Outbuf)
{
	uchar	i,ch,edc;
	uchar	Parity_Error_Flag;
	uchar	r_nad,r_pcb,r_len,r_inf;
	uchar	r_len_temp;
LABLE(D_T1_RECV);
	edc = 0;
	Parity_Error_Flag = 0;
	// 2010-01-17  yehf������Ҫgl_CardInfo.k_Ic_CharWait_TimeCount-8����
	gl_CardInfo.k_Ic_CharWait_TimeCount = gl_CardSlotInfo[gl_CardInfo.current_slot].CWT;
    gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;
	ch = icc_RecvByte();				// NAD
	if(gl_CardInfo.k_IccErrTimeOut)
		return T1_BWTERR;
	if(gl_CardInfo.k_IccErrPar)
		Parity_Error_Flag = 0x01;

	edc = edc ^ ch;
	*Outbuf++ = ch;
	r_nad = ch;

	ch = icc_RecvByte();				// PCB
	if(gl_CardInfo.k_IccErrTimeOut)
		return T1_CWTERR;
	if(gl_CardInfo.k_IccErrPar)
		Parity_Error_Flag = 0x01;

	edc = edc ^ ch;
	*Outbuf++ = ch;
	r_pcb = ch;

	ch = icc_RecvByte();				//LEN
	if(gl_CardInfo.k_IccErrTimeOut)
		return T1_CWTERR;
	if(gl_CardInfo.k_IccErrPar)
		Parity_Error_Flag = 0x01;

	edc = edc ^ ch;
	*Outbuf++ = ch;
	r_len = ch;
	r_len_temp = ch;
	//20140106  yehf�޸�
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		if((r_pcb&0xC0) == 0x80)
		{
			if(ch)
				r_len_temp = 0;
		}
		else if((r_pcb&0xC0) == 0xC0)
		{
			if(ch > 1)
				r_len_temp = 1;
		}
	}

	for(i = 0; i < r_len_temp; i++)
	{
		ch = icc_RecvByte();				// Data[n]
		if(gl_CardInfo.k_IccErrTimeOut)
			return T1_CWTERR;
		if(gl_CardInfo.k_IccErrPar)
			Parity_Error_Flag = 0x01;
		edc = edc ^ ch;
		*Outbuf++ = ch;
	}

	r_inf = ch;

	ch = icc_RecvByte();				// EDC
	if(gl_CardInfo.k_IccErrTimeOut)
		return T1_CWTERR;
	if(gl_CardInfo.k_IccErrPar)
		Parity_Error_Flag = 0x01;

	icc_close_etu_timer();
	icc_disable_inter_timer();
	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;

	*Outbuf = ch;

	if(ch != edc)
		return T1_EDCERR;
	if(Parity_Error_Flag)
		return T1_PARITYERR;

	if(r_nad)
		return T1_INVALIDBLOCK;   //nad!=0
	if((r_pcb&0x80) == 0)
	{ // I_BLOCK
		if(r_len == 0xFF)
			return T1_INVALIDBLOCK;
	}
	else if((r_pcb&0xC0) == 0x80)
	{ // R_BLOCK
		if(r_len)
			return T1_INVALIDBLOCK;

		if((r_pcb&0x20) == 0x20)
			return T1_INVALIDBLOCK; //rblock bit6=1
	}
	else
	{ // S_BLOCK
		if(r_pcb == 0xC1)
		{
			if((r_inf < 0x10) || (r_inf > 0xFE))
				return T1_INVALIDBLOCK;
		}
		else if(r_pcb == 0xC2)  // abort request
		    //����Ӧ
			return T1_ABORTERR;

		else if(r_pcb == 0xE3) return T1_INVALIDBLOCK; //S(WTX response)
		else if(r_pcb == 0xE2) return T1_INVALIDBLOCK; //S(ABORT response)
		else if(r_pcb == 0xE0) return T1_INVALIDBLOCK; //S(RESYNCH response)

		if(r_len != 1)
			return T1_INVALIDBLOCK;

		if((r_pcb&0x1F) > 4)    //10_01_12yehf ���VCC����
			return T1_INVALIDBLOCK;
	}
	return ICC_SUCCESS;
}

#if 11
//06052014 chenf T1
static int icc_t1Exchange(uchar *inbuf,uchar *outbuf)
{
	uchar	sour[300],oldsour[300],respbuf[300],oldrblock[5];
	uchar	OLDIFSC;
	uchar	PCB,tmpch = 0,tmpch1;
	uint	len,len1,i;

	uint	total_send_len,sent_len,remain_len;
	uchar	re_send_i_times,re_send_r_times;
	uint	WTX = 0;
	int		Ret = 0;
	OLDIFSC = gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC;
	gl_CardInfo.Parity_Check_Enable = 0x00;
//    uchar   rblockmax = 1;//�ط�r��������

start_t1_exchange:

	re_send_i_times = 0;
	re_send_r_times = 0;
	total_send_len = inbuf[0]*256 + inbuf[1];
	remain_len = inbuf[0]*256 + inbuf[1];
	sent_len = 0;

	outbuf[0] = 0x00;
	outbuf[1] = 0x00;
	outbuf[2] = 0x00;
	len = 0;
	len1 = 0;

	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0xFF;
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0xFF;

	memset(respbuf, 0x00, sizeof(respbuf));
	memset(sour, 0x00, sizeof(sour));
	memset(oldsour, 0x00, sizeof(oldsour));
	memset(oldrblock, 0x00, sizeof(oldrblock));

send_remain_data:
	if(remain_len == 0)
	{
		return 0xFF;
	}

	if(remain_len > gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC)
	{   /*  ������ݰ�������IFSCʱ���򽫰������зֺ��ٷ���*/
		gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB |= 0x20;   /*��I����������ӷ� */
		memcpy(&sour[3],&inbuf[2+sent_len],gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC);
		/*�趨��������I_BLOCK  */
		sour[2] = gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC;  /* �趨I_block�����ݳ���*/
	}
	else
	{   /*����ֻ��һ���Ϳ���ȫ����  */
		gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB &= 0xDF;
		memcpy(&sour[3],&inbuf[2+sent_len],remain_len);
		sour[2] = remain_len;
	}
	sent_len += sour[2]; /* �趨�ѷ������ݳ���	*/
	remain_len = total_send_len - sent_len;
	sour[1] = gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB; /* �趨���Ͱ�ͷ��PCBЭ������ֽ�*/

	sour[0] = 0x00;
	memcpy(oldsour, sour, sour[2]+3);/* ���ݷ��͵�I���� */
	gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = sour[1]^0x40;
	re_send_i_times = 0;

re_send_i_block:
//    rblockmax = 1;
	if(re_send_i_times > 2)
	{
		s_DelayUs(100);
		return (-ICC_COMMERR);
	}
	re_send_i_times++;
	re_send_r_times = 0;
	sour[0] = 0x00;

	icc_t1send(sour);

receive_one_block:
	Ret = icc_t1recv(respbuf);
	// ��BWTʱ�������ԭ����ʱ��
    i = gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = i;

	// ����һ֡���ݺ󣬹ر����ʱ����
	if((Ret == T1_CWTERR)||(Ret == T1_BWTERR)||(Ret == T1_ABORTERR) || (Ret == ERROR))
	{
		s_DelayUs(100);
		return Ret;
	}

	if((Ret == T1_PARITYERR) || (Ret == T1_EDCERR))
	{
        if ( re_send_r_times ) {
            //֮ǰ�ѷ���R ���ٽ���ʱ �ط���һ�ε�R
            sour[0] = oldrblock[0];
            sour[1] = oldrblock[1];
            sour[2] = oldrblock[2];
        }else{
            tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^0x40;
            /* ���Ϊ���ڽ��տ�Ƭ��I����˳��ţ���Ϊ�ϴν��յ��¸���ţ�*/
            sour[1] = ((tmpch >> 2) & 0x10) | 0x81; /* ����ż/EDC���� */
            sour[2] = 0;
        }
		goto re_send_r_block;
	}

	if(Ret == T1_INVALIDBLOCK){
        if ( re_send_r_times ) {
            //֮ǰ�ѷ���R ���ٽ���ʱ �ط���һ�ε�R
            sour[0] = oldrblock[0];
            sour[1] = oldrblock[1];
            sour[2] = oldrblock[2];
            goto re_send_r_block;
        }else{
            goto set_rblock_with_othererr;
        }
    }

	PCB = respbuf[1]; /* ��ȡ��Ӧ���ݰ�ͷ*/

	if((PCB & 0x80) == 0x00)
	{
		if(remain_len > 0)
		{
			//�������ӿ��յ�I��˵������
			goto set_rblock_with_othererr;
		}

		if((PCB & 0x40) != (gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB & 0x40))
		{/*  �յ���I�������кŲ�ͬ����ǰ�յ���I�������к�
		ʱ��ȷ�Ͻ�����ȷ����������յ����ݵ�OUTBUF�С�*/
			re_send_r_times = 0;

			if(respbuf[2] > 0)
			{ /*����������ݳ��Ȳ�Ϊ���򱣴��������*/
				len1 = outbuf[0] * 256 + outbuf[1];
				len=len1 + respbuf[2];
				outbuf[0]= len / 256;
				outbuf[1]= len % 256;
				memcpy(&outbuf[len1+2],&respbuf[3],respbuf[2]);
			}

			gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB = PCB;
			/*�����µ�I����˳���*/
			if((PCB&0x20) == 0x20)
			{ /* �յ�������I���� */
				tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^0x40;
				/* �¸�I_Block˳���*/
				sour[1] = ((tmpch >>2 ) & 0x10) | 0x80;
				sour[2] = 0;
				goto re_send_r_block;
			}
			/* ����ȷ���յ������ӵ�I�������˳�*/
			Ret = ICC_SUCCESS;
			goto end_t1_exchange;
		}
		else
		{
            if(re_send_r_times == 0){
                goto set_rblock_with_othererr;
            }
            else{
                //���ٴ��յ���������������ʱ(��������) Ҫ�ط���һ��R
                sour[0] = oldrblock[0];
                sour[1] = oldrblock[1];
                sour[2] = oldrblock[2];
//                rblockmax = 2;
				goto re_send_r_block;
            }
		}
	}

	else if((PCB&0XC0) == 0X80)
	{// ?r_block
		tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB;
		if((tmpch&0x40) == ((PCB&0x10) << 2))
		{
			/*	��ŵ����ն��¸�Ҫ���͵�I������� */
			if((PCB&0xef) == 0x80)
			{// r-block with error-free
				if((oldsour[1]&0x20) == 0x20) /* �ն����� */
				{
					//���ͳɹ�
					goto send_remain_data;
				}
				else
				{   // ˵���������ն�I��,�ն�û���յ�,���ط�R��Ҫ���ն�Ӧ��
                    //1770_L40-41
					tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^ 0x40;
					tmpch1 = ((tmpch >> 2 ) & 0x10) | 0x82;
					re_send_r_times = 0;
					sour[1] = tmpch1;
					sour[2] = 0;
					goto re_send_r_block;
				}
			}
			else
			{/* ���д����ʶ��R���� */
				if(re_send_r_times == 0)
				{ /* ��һ����R���� */
//					tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB ^ 0x40;
//					sour[1] = ((tmpch >>2 ) & 0x10) | 0x80;
//					sour[2] = 0;
//					goto re_send_r_block;

                    //���˷���һЩ����R ���ұ��Ϊ��һ���ն�Ҫ���͵�I
                    //(������ȷӦ�ÿ���Ҫ��I����Ӧ ���ڿ���ȴ�����˴���R)
                    goto set_rblock_with_othererr;
				}
				else
				{ /* �޸Ĵ����ʶ */
//					if((sour[1]&0x0f)!= 0x00)
//					{
//						sour[1] = sour[1]&0xf0;
//						re_send_r_times = 0;
//					}
                    //�ڶ��� ����R ��Ӧ���ط���һ�ε�R case1772
                    sour[0] = oldrblock[0];
                    sour[1] = oldrblock[1];
                    sour[2] = oldrblock[2];
//                    rblockmax = 2;
					re_send_r_times = 0; //�յ����д�����Ϣ��R ������Ч��
                    goto re_send_r_block;
				}
			}
			/* �ط�R���� */
//			goto re_send_r_block;
		}


		if((tmpch&0x40) != ((PCB&0X10) << 2))
		{
			/* �����ŵ����ն��ѷ��͵�I����������ط���I���� */
			if((gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB&0x20) == 0x20)
			{
				//�ϴν��յ����ӿ�,�ն˷��͵�R��Ӧ��,��δ�յ���Ҫ���ն��ط�R��
				goto re_send_r_block;
			}

			if(OLDIFSC != gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC)
			{
				//����I��ʱ���յ�����IFSC����,��Ӧ�����յ�R��Ҫ���ն˰����µ�IFSC��С�ط�I��
				// ��ʱ��Ϊ�ն˷���I���Ѿ�ȷ�ϣ�����ն��ط�I�飬I���Ű����µĿ�ŷ���
				OLDIFSC = gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = oldsour[1];
				goto start_t1_exchange;
			}
			memcpy(sour, oldsour, oldsour[2]+3);
			goto re_send_i_block;
		}
		/* �ط�R���� */
		goto re_send_r_block;

	}
	else if((PCB&0XC0) == 0XC0)
	{// ?s_block
		if((PCB & 0x20) == 0X00)
		{/*  ����յ�����IC�����͵������ź�����ݴ�����������
		�����޸���Ӧ���趨������S��Ӧ�����IC��  */
			sour[1] = 0x20 | respbuf[1] ;
			sour[2] = respbuf[2];
			memcpy(&sour[3], &respbuf[3], sour[2]);
			if((respbuf[1] & 0x1F) == 0x03)
			{	/* ����BWT����	BWT=BWT * respbuf[3]; */
                i = gl_CardSlotInfo[gl_CardInfo.current_slot].BWT;
				gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP = i;
				if(respbuf[3] != 0)
				{
                    WTX = (gl_CardSlotInfo[gl_CardInfo.current_slot].TB3 >> 4)&0x0f;
                    WTX = (((uint)1 << WTX) * 960 *gl_CardSlotInfo[gl_CardInfo.current_slot].D) + 11;
                    i = (WTX + 960 *gl_CardSlotInfo[gl_CardInfo.current_slot].D) * respbuf[3] + 100;
					gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = i;
				}
			}
			else if ((respbuf[1] & 0x1f) == 0x01)
			{/* ��Ϣ�ֶ���������*/
				gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC = respbuf[3];
			}

			else if((respbuf[1]&0x1f) == 0x00)
			{ /*  ͬ������	*/

			}
			sour[0] = 0x00;  /* ����S��Ӧ�����IC��*/

			icc_t1send(sour);
			re_send_r_times = 0;

			goto receive_one_block;
		}
		else
		{ /* ���յ���IC�����͵�S��Ӧ�ź�*/
			if((sour[1]&0xe0) == 0xc0)
			{/*  ����ն˷�����������Ϣʱ�������Ӧ������Ӧ�Ĳ���*/
				re_send_r_times = 0;

				if ((respbuf[1] & 0x1f) == 0x01)
				{
					gl_CardSlotInfo[gl_CardInfo.current_slot].IFSD = respbuf[3];
				}
				else if((respbuf[1] & 0x1f) == 0x00)
				{	 //  Reset or deactivation
					gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = 0x00;
					gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB = 0x40;
					goto start_t1_exchange;
				}
				goto end_t1_exchange;
			}
            if(re_send_r_times){
                //���ٴ��յ���������������ʱ(��������) Ҫ�ط���һ��R
                sour[0] = oldrblock[0];
                sour[1] = oldrblock[1];
                sour[2] = oldrblock[2];
//                rblockmax = 2;
                goto re_send_r_block;
            }
		}
	}
set_rblock_with_othererr:
	tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^0x40;
	sour[1] = ((tmpch >>2 ) & 0x10) | 0x82; /* �����������R���� */
	sour[2] = 0;

re_send_r_block:
	if(re_send_r_times != 0)
	{
		if((oldrblock[0]!= sour[0]) || (oldrblock[1] != sour[1]) || (oldrblock[2] != sour[2]))
			re_send_r_times = 0;
	}
	if(re_send_r_times == 0)
	{
		oldrblock[0]=sour[0];
		oldrblock[1]=sour[1];
		oldrblock[2]=sour[2];
	}

	if(re_send_r_times > 1)
//	if(re_send_r_times > 2)
//	if(re_send_r_times > rblockmax)
	{
		s_DelayUs(100);
		return T1_MOREERR;
	}

	icc_t1send(sour);
	re_send_r_times++;
	goto receive_one_block;

end_t1_exchange:
	if(Ret != 0)
	{/* ���ͨ���봦��ʧ�ܣ�����SW1 SW2Ϊ0xFF��*/
		gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0xFF;
		gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0xFF;
		return Ret;
	}

	len = outbuf[0]*256 + outbuf[1]+2;
	/* �����յ��������λ���ݱ��浽SW1��SW2*/
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = outbuf[len-2];
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = outbuf[len-1];
	return ICC_SUCCESS;
}

#else

static int icc_t1Exchange(uchar *inbuf,uchar *outbuf)
{
	uchar	sour[300],oldsour[300],respbuf[300],oldrblock[5];
	uchar	OLDIFSC;
	uchar	PCB,tmpch = 0,tmpch1;
	uint	len,len1,i;

	uint	total_send_len,sent_len,remain_len;
	uchar	re_send_i_times,re_send_r_times;
	uint	WTX = 0;
	int		Ret = 0;
	OLDIFSC = gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC;
	gl_CardInfo.Parity_Check_Enable = 0x00;

start_t1_exchange:

	re_send_i_times = 0;
	re_send_r_times = 0;
	total_send_len = inbuf[0]*256 + inbuf[1];
	remain_len = inbuf[0]*256 + inbuf[1];
	sent_len = 0;

	outbuf[0] = 0x00;
	outbuf[1] = 0x00;
	outbuf[2] = 0x00;
	len = 0;
	len1 = 0;

	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0xFF;
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0xFF;

	memset(respbuf, 0x00, sizeof(respbuf));
	memset(sour, 0x00, sizeof(sour));
	memset(oldsour, 0x00, sizeof(oldsour));
	memset(oldrblock, 0x00, sizeof(oldrblock));

send_remain_data:
	if(remain_len == 0)
	{
		return 0xFF;
	}

	if(remain_len > gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC)
	{   /*  ������ݰ�������IFSCʱ���򽫰������зֺ��ٷ���*/
		gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB |= 0x20;   /*��I����������ӷ� */
		memcpy(&sour[3],&inbuf[2+sent_len],gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC);
		/*�趨��������I_BLOCK  */
		sour[2] = gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC;  /* �趨I_block�����ݳ���*/
	}
	else
	{   /*����ֻ��һ���Ϳ���ȫ����  */
		gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB &= 0xDF;
		memcpy(&sour[3],&inbuf[2+sent_len],remain_len);
		sour[2] = remain_len;
	}
	sent_len += sour[2]; /* �趨�ѷ������ݳ���	*/
	remain_len = total_send_len - sent_len;
	sour[1] = gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB; /* �趨���Ͱ�ͷ��PCBЭ������ֽ�*/

	sour[0] = 0x00;
	memcpy(oldsour, sour, sour[2]+3);/* ���ݷ��͵�I���� */
	gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = sour[1]^0x40;
	re_send_i_times = 0;

re_send_i_block:
	if(re_send_i_times > 2)
	{
		s_DelayUs(100);
		return (-ICC_COMMERR);
	}
	re_send_i_times++;
	re_send_r_times = 0;
	sour[0] = 0x00;

	icc_t1send(sour);

receive_one_block:
	Ret = icc_t1recv(respbuf);
	// ��BWTʱ�������ԭ����ʱ��
    i = gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = i;

	// ����һ֡���ݺ󣬹ر����ʱ����
	if((Ret == T1_CWTERR)||(Ret == T1_BWTERR)||(Ret == T1_ABORTERR) || (Ret == ERROR))
	{
		s_DelayUs(100);
		return Ret;
	}

	if((Ret == T1_PARITYERR) || (Ret == T1_EDCERR))
	{
		tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^0x40;
		/* ���Ϊ���ڽ��տ�Ƭ��I����˳��ţ���Ϊ�ϴν��յ��¸���ţ�*/
		sour[1] = ((tmpch >> 2) & 0x10) | 0x81; /* ����ż/EDC���� */
		sour[2] = 0;
		goto re_send_r_block;
	}

	if(Ret == T1_INVALIDBLOCK)
		goto set_rblock_with_othererr;

	PCB = respbuf[1]; /* ��ȡ��Ӧ���ݰ�ͷ*/

	if((PCB & 0x80) == 0x00)
	{
		if(remain_len > 0)
		{
			//�������ӿ��յ�I��˵������
			goto set_rblock_with_othererr;
		}

		if((PCB & 0x40) != (gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB & 0x40))
		{/*  �յ���I�������кŲ�ͬ����ǰ�յ���I�������к�
		ʱ��ȷ�Ͻ�����ȷ����������յ����ݵ�OUTBUF�С�*/
			re_send_r_times = 0;

			if(respbuf[2] > 0)
			{ /*����������ݳ��Ȳ�Ϊ���򱣴��������*/
				len1 = outbuf[0] * 256 + outbuf[1];
				len=len1 + respbuf[2];
				outbuf[0]= len / 256;
				outbuf[1]= len % 256;
				memcpy(&outbuf[len1+2],&respbuf[3],respbuf[2]);
			}

			gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB = PCB;
			/*�����µ�I����˳���*/
			if((PCB&0x20) == 0x20)
			{ /* �յ�������I���� */
				tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^0x40;
				/* �¸�I_Block˳���*/
				sour[1] = ((tmpch >>2 ) & 0x10) | 0x80;
				sour[2] = 0;
				goto re_send_r_block;
			}
			/* ����ȷ���յ������ӵ�I�������˳�*/
			Ret = ICC_SUCCESS;
			goto end_t1_exchange;
		}
		else
		{
			goto set_rblock_with_othererr;
		}
	}

	else if((PCB&0XC0) == 0X80)
	{// ?r_block
		tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB;
		if((tmpch&0x40) == ((PCB&0x10) << 2))
		{
			/*	��ŵ����ն��¸�Ҫ���͵�I������� */
			if((PCB&0xef) == 0x80)
			{// r-block with error-free
				if((oldsour[1]&0x20) == 0x20) /* �ն����� */
				{
					//���ͳɹ�
					goto send_remain_data;
				}
				else
				{   // ˵���������ն�I��,�ն�û���յ�,���ط�R��Ҫ���ն�Ӧ��
					tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^ 0x40;
					tmpch1 = ((tmpch >> 2 ) & 0x10) | 0x82;
					re_send_r_times = 0;
					sour[1] = tmpch1;
					sour[2] = 0;
					goto re_send_r_block;
				}
			}
			else
			{/* ���д����ʶ��R���� */
				if(re_send_r_times == 0)
				{ /* ��һ����R���� */
					tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB ^ 0x40;
					sour[1] = ((tmpch >>2 ) & 0x10) | 0x80;
					sour[2] = 0;
					goto re_send_r_block;
				}
				else
				{ /* �޸Ĵ����ʶ */
					if((sour[1]&0x0f)!= 0x00)
					{
						sour[1] = sour[1]&0xf0;
						re_send_r_times = 0;
					}
				}
			}
			/* �ط�R���� */
			goto re_send_r_block;
		}


		if((tmpch&0x40) != ((PCB&0X10) << 2))
		{
			/* �����ŵ����ն��ѷ��͵�I����������ط���I���� */
			if((gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB&0x20) == 0x20)
			{
				//�ϴν��յ����ӿ�,�ն˷��͵�R��Ӧ��,��δ�յ���Ҫ���ն��ط�R��
				goto re_send_r_block;
			}

			if(OLDIFSC != gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC)
			{
				//����I��ʱ���յ�����IFSC����,��Ӧ�����յ�R��Ҫ���ն˰����µ�IFSC��С�ط�I��
				// ��ʱ��Ϊ�ն˷���I���Ѿ�ȷ�ϣ�����ն��ط�I�飬I���Ű����µĿ�ŷ���
				OLDIFSC = gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC;
				gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = oldsour[1];
				goto start_t1_exchange;
			}
			memcpy(sour, oldsour, oldsour[2]+3);
			goto re_send_i_block;
		}
		/* �ط�R���� */
		goto re_send_r_block;

	}
	else if((PCB&0XC0) == 0XC0)
	{// ?s_block
		if((PCB & 0x20) == 0X00)
		{/*  ����յ�����IC�����͵������ź�����ݴ�����������
		�����޸���Ӧ���趨������S��Ӧ�����IC��  */
			sour[1] = 0x20 | respbuf[1] ;
			sour[2] = respbuf[2];
			memcpy(&sour[3], &respbuf[3], sour[2]);
			if((respbuf[1] & 0x1F) == 0x03)
			{	/* ����BWT����	BWT=BWT * respbuf[3]; */
				i = gl_CardSlotInfo[gl_CardInfo.current_slot].BWT;
				gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP = i;
				if(respbuf[3] != 0)
				{
                    WTX = (gl_CardSlotInfo[gl_CardInfo.current_slot].TB3 >> 4)&0x0f;
                    WTX = (((uint)1 << WTX) * 960 *gl_CardSlotInfo[gl_CardInfo.current_slot].D) + 11;
					i = (WTX + 960 *gl_CardSlotInfo[gl_CardInfo.current_slot].D) * respbuf[3] + 100;
                    gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = i;
				}
			}
			else if ((respbuf[1] & 0x1f) == 0x01)
			{/* ��Ϣ�ֶ���������*/
				gl_CardSlotInfo[gl_CardInfo.current_slot].IFSC = respbuf[3];
			}

			else if((respbuf[1]&0x1f) == 0x00)
			{ /*  ͬ������	*/

			}
			sour[0] = 0x00;  /* ����S��Ӧ�����IC��*/

			icc_t1send(sour);
			re_send_r_times = 0;

			goto receive_one_block;
		}
		else
		{ /* ���յ���IC�����͵�S��Ӧ�ź�*/
			if((sour[1]&0xe0) == 0xc0)
			{/*  ����ն˷�����������Ϣʱ�������Ӧ������Ӧ�Ĳ���*/
				re_send_r_times = 0;

				if ((respbuf[1] & 0x1f) == 0x01)
				{
					gl_CardSlotInfo[gl_CardInfo.current_slot].IFSD = respbuf[3];
				}
				else if((respbuf[1] & 0x1f) == 0x00)
				{	 //  Reset or deactivation
					gl_CardSlotInfo[gl_CardInfo.current_slot].TERM_PCB = 0x00;
					gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB = 0x40;
					goto start_t1_exchange;
				}
				goto end_t1_exchange;
			}
		}
	}
set_rblock_with_othererr:
	tmpch = gl_CardSlotInfo[gl_CardInfo.current_slot].CARD_PCB^0x40;
	sour[1] = ((tmpch >>2 ) & 0x10) | 0x82; /* �����������R���� */
	sour[2] = 0;

re_send_r_block:
	if(re_send_r_times != 0)
	{
		if((oldrblock[0]!= sour[0]) || (oldrblock[1] != sour[1]) || (oldrblock[2] != sour[2]))
			re_send_r_times = 0;
	}
	if(re_send_r_times == 0)
	{
		oldrblock[0]=sour[0];
		oldrblock[1]=sour[1];
		oldrblock[2]=sour[2];
	}

	if(re_send_r_times > 1)
	{
		s_DelayUs(100);
		return T1_MOREERR;
	}

	icc_t1send(sour);
	re_send_r_times++;
	goto receive_one_block;

end_t1_exchange:
	if(Ret != 0)
	{/* ���ͨ���봦��ʧ�ܣ�����SW1 SW2Ϊ0xFF��*/
		gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0xFF;
		gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0xFF;
		return Ret;
	}

	len = outbuf[0]*256 + outbuf[1]+2;
	/* �����յ��������λ���ݱ��浽SW1��SW2*/
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = outbuf[len-2];
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = outbuf[len-1];
	return ICC_SUCCESS;
}
#endif

int inter_icc_exchange(int sendlen, uchar *psenddata, uint *precvlen, uchar *precvdata)
{
	uchar test[8];
	uchar cmd_case;
	uchar inbuf[512];
	uchar outbuf[512];
	uchar *ptr, len;
	ushort i;
	int Ret = 0;

	gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = 0xff;
	gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = 0xff;
	memset(outbuf, 0x00, sizeof(outbuf));

	switch(gl_CardSlotInfo[gl_CardInfo.current_slot].T)
	{/* ����T=1��T=0Э�������Ӧ�Ĵ���*/
		case 1:
			i = 4;
			inbuf[2] = psenddata[0];
			inbuf[3] = psenddata[1];
			inbuf[4] = psenddata[2];
			inbuf[5] = psenddata[3];
			ptr = &inbuf[6];

			if(sendlen == 4)
			{
				i++;
				*ptr++ = 0;
			}
			else if(sendlen == 5)
			{
				i++;
				*ptr++ = psenddata[4];
			}
			else
			{
				len = psenddata[4];
				i += (len + 1);
				*ptr++ = len;
				if(len > 0)
				{
					memcpy(ptr, psenddata+5, len);
					ptr += len;
				}

				if(sendlen == (6 + psenddata[4]))
				{
					i++;
					*ptr++ = psenddata[sendlen-1];
				}
			}

			inbuf[0] = i/256;
			inbuf[1] = i%256;
LABLE(D_T1_ST);
            icc_DisableOtherInt();
			Ret = icc_t1Exchange(inbuf, outbuf);
            icc_EnableOtherInt();
LABLE(D_T1_END);
			icc_close_etu_timer();
			icc_disable_inter_timer();
			gl_char_wait_etu = 0;
			gl_total_atr_etu = 0;

			if(Ret == ICC_SUCCESS)
			{
				memcpy(precvdata,&outbuf[2],outbuf[0]*256+outbuf[1]);
				*precvlen = outbuf[0]*256+outbuf[1];
			}
			return Ret;

		case 0:
			memcpy(inbuf, psenddata, 4);
			cmd_case = 0x01;
			if(sendlen == 4)
			{
				cmd_case = 0x01;  // case 1 command
				len = 4;
			}
			else if(sendlen == 5)
			{
				cmd_case = 0x02;  // case 2 command
				inbuf[4] = psenddata[4];
				len = 5;
			}
			else if(sendlen == (5 + psenddata[4]))
			{
				cmd_case = 0x03;  // case 3 command
			}
			else
			{
				cmd_case = 0x04;  // case 4 command
			}
            CLRBUF(test);
			test[0] = cmd_case;

			if((cmd_case == 0x03) || (cmd_case == 0x04))
			{   /* ����IC������P3���ȵ�����*/
				len = 5 + psenddata[4];
				inbuf[4] = psenddata[4];
				memcpy(inbuf+5, &psenddata[5], psenddata[4]);
				if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
				{
					if(cmd_case == 0x04)
					{
						inbuf[len]= psenddata[len];
						len++;
					}
				}
			}
LABLE(D_T0_ST);
            icc_DisableOtherInt();
			Ret = icc_t0Exchange(len,inbuf,outbuf,test);
            icc_EnableOtherInt();
LABLE(D_T0_END);
			icc_close_etu_timer();
			icc_disable_inter_timer();
			gl_char_wait_etu = 0;
			gl_total_atr_etu = 0;

			if(Ret)
			{
				return Ret;
			}
			memcpy(precvdata,outbuf+2,outbuf[0]*256 + outbuf[1]);
			*precvlen = outbuf[0]*256 + outbuf[1];

			precvdata[*precvlen] = test[0];
			precvdata[(*precvlen) + 1] = test[1];
			*precvlen += 2;
			gl_CardSlotInfo[gl_CardInfo.current_slot].sw1 = test[0];
			gl_CardSlotInfo[gl_CardInfo.current_slot].sw2 = test[1];
			return ICC_SUCCESS;
		default:
			break;
	}
	return (-ICC_RESPERR);
}

int  icc_ifsdCmd(void)
{
	int Ret = 0;
	uchar ifsdbuf[10], respbuf[300];
	uchar resendtimes = 0;

	if(gl_CardSlotInfo[gl_CardInfo.current_slot].T == 0)
		return ICC_SUCCESS;

	gl_CardInfo.Parity_Check_Enable = 0x00;

	memset(ifsdbuf, 0, sizeof(ifsdbuf));
	memset(respbuf, 0, sizeof(respbuf));

	ifsdbuf[0] = 0x00;
	ifsdbuf[1] = 0xC1;
	ifsdbuf[2] = 0x01;
	ifsdbuf[3] = 0xFE;
	ifsdbuf[4] = 0x00;

re_send_ifsd:

	if(resendtimes < 3)
	{
		Ret = icc_t1send(ifsdbuf);
	}
	else
	{
		s_DelayUs(100);
		return T1_IFSDERR;
	}

	resendtimes++;
    icc_DisableOtherInt();
    Ret = icc_t1recv(respbuf);
    icc_EnableOtherInt();

	if((Ret == T1_CWTERR)||(Ret == T1_BWTERR)||(Ret == T1_ABORTERR)||(Ret == ERROR))
	{
		s_DelayUs(100);
		return Ret;
	}
	if((Ret == T1_PARITYERR)||(Ret == T1_EDCERR)||(Ret == T1_INVALIDBLOCK))
		goto re_send_ifsd;
	if(((respbuf[1]&0xE0) != 0xE0)||((respbuf[1]&0x0F) != 0x01))
		goto re_send_ifsd;

	if((respbuf[2] != 0x01)||(respbuf[3] != 0xFE))
		goto re_send_ifsd;

	gl_CardSlotInfo[gl_CardInfo.current_slot].IFSD = 254;
	return ICC_SUCCESS;

}

// PPSЭ��
int icc_PPSRequest(void)
{
    uchar ucSendBuf[4];
    uchar ucRecvBuf[4];
    int Ret = 0;
    uint i = 0;
    uint Fi = 0;
    uint Di = 0;
    uint ETU = 0;
	uchar TA1=0x11;
    memset(ucSendBuf, 0x00, sizeof(ucSendBuf));
    memset(ucRecvBuf, 0x00, sizeof(ucRecvBuf));

    gl_CardInfo.Parity_Check_Enable = 0x01;
    gl_CardInfo.k_Ic_CharWait_TimeCount = gl_CardSlotInfo[gl_CardInfo.current_slot].WWT;
    gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;

    if(!(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x01))  // ��TA1����Э�̣��԰���D=1ͨ��
    {
        return ICC_SUCCESS;
    }

    if((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F) == 0x01) // TA1ΪD=1,��û�б�ҪЭ��
    {
        return ICC_SUCCESS;
    }

    // �ж��Ƿ�Ϊ�Ϸ���Fiֵ
    if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0x70)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0x80)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0xE0)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0xF0))
    {
        // ��֧�ֵ�Fiֵ����Э���԰�Initial ETUͨ��
        return ICC_SUCCESS;
    }
    Fi = (gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) >> 4;

    // �ж��Ƿ�Ϊ�Ϸ���Diֵ
    if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F) == 0x00)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F) > 0x09))
    {
        // ��֧�ֵ�Diֵ����Э���԰�Initial ETUͨ��
        return ICC_SUCCESS;
    }
    Di = gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F;

    // ���֧��5MHz
	ucSendBuf[0] = 0xFF;
    ucSendBuf[1] = 0x10 | gl_CardSlotInfo[gl_CardInfo.current_slot].T;
	ucSendBuf[2] = gl_CardSlotInfo[gl_CardInfo.current_slot].TA1;

    ETU = (F_Table[Fi]/D_Table[Di]);
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		 if(ETU < 46)
		 {
		 	if(F_Table[Fi] == 512 && D_Table[Di] == 16)
		 	{
				TA1 = 0x11;
			}
			else
			{
				TA1 = 0x94;
			}
		 }
		 else
		 {
			TA1 = gl_CardSlotInfo[gl_CardInfo.current_slot].TA1;
		 }
	}
	else
	{
	    if(ETU < 64)
	    {
	    	if(F_Table[Fi] == 512 && D_Table[Di] == 16)
			{
				//yehf 2012-03-08 ���⴦����Ҫ��11Э��
				TA1 = 0x11;
				// ����95������Э��
				//TA1 = 0x95;
			}
			else
			{
				//����55800������94����Э��
				TA1 = 0x94;
			}

	    }
		else
		{
			TA1 = gl_CardSlotInfo[gl_CardInfo.current_slot].TA1;
		}
	}
	ucSendBuf[2] = TA1;
	Di = (TA1 & 0x0F);
	Fi = ((TA1 & 0xF0) >> 4);
	ETU = (F_Table[Fi]/D_Table[Di]);

	ucSendBuf[3] = (ucSendBuf[0]^ucSendBuf[1]^ucSendBuf[2]);

	icc_init_inter_timer();
	icc_init_etu_timer();
	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;
	icc_delay_etu(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU,8);
LABLE(D_PPS_ST);
    // ����PPSS
    Ret = icc_SendByte(ucSendBuf[0]);
    if(Ret)
    {
        return (-ICC_COMMERR);
    }

    // ����PPS0
    Ret = icc_SendByte(ucSendBuf[1]);
    if(Ret)
    {
        return (-ICC_COMMERR);
    }

    // ����PPS1
    Ret = icc_SendByte(ucSendBuf[2]);
    if(Ret)
    {
        return (-ICC_COMMERR);
    }

    // ����PPCK
    gl_CardInfo.k_Ic_LastSendByte = 1;
    Ret = icc_SendByte(ucSendBuf[3]);
    gl_CardInfo.k_Ic_LastSendByte = 0;
    if(Ret)
    {
        return (-ICC_COMMERR);
    }
    for(i = 0; i < 4; i++)
    {
		ucRecvBuf[i] = icc_RecvByte();
        if(gl_CardInfo.k_IccComErr)
        {
            if(gl_CardInfo.k_IccErrTimeOut)
            {
            	D2(TRACE("\r\n PPS��ʱ"););
                return (-ICC_TIMEOUT);
            }
            else
            {
                return (-ICC_COMMERR);
            }
        }
    }
    // �ȽϷ��ͽ��յ��ַ��Ƿ�һ��
    if(memcmp(ucSendBuf, ucRecvBuf, sizeof(ucRecvBuf)))
    {
        return (-ICC_RESPERR);
    }

    gl_CardSlotInfo[gl_CardInfo.current_slot].D = D_Table[Di];
    gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = ETU;
//T0 ����ʱ�䡣
	Di = gl_CardSlotInfo[gl_CardInfo.current_slot].D;
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		i = ((960 * Di * gl_CardSlotInfo[gl_CardInfo.current_slot].TC2) + 1);
	}
	else
	{
		i = ((960 * Di * gl_CardSlotInfo[gl_CardInfo.current_slot].TC2) + (480 * Di) + 1);
	}
	gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = (i);
	//TRACE("\r\n Di:%d ETU:%d TC2:%02X WWT:%d",Di,ETU,gl_CardSlotInfo[gl_CardInfo.current_slot].TC2,gl_CardSlotInfo[gl_CardInfo.current_slot].WWT);
//T1 ����ʱ�䡣
	Di = gl_CardSlotInfo[gl_CardInfo.current_slot].TB3;
	i = (((uint)0x01<<((Di & 0xF0) >> 4))*960*gl_CardSlotInfo[gl_CardInfo.current_slot].D) + 11;
	i += (gl_CardSlotInfo[gl_CardInfo.current_slot].D * 960);
	i += 1;
    gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = i;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP = i;
	D2(
		TRACE("\r\n Di:%d BWT:%d-%d",Di,gl_CardSlotInfo[gl_CardInfo.current_slot].BWT,i);
	);
    icc_adjustTiming();
    return ICC_SUCCESS;
}

// PPSЭ��
int icc_PPSSelect(int mode)
{
    uchar ucSendBuf[4];
    uchar ucRecvBuf[4];
    int Ret = 0;
    uint i = 0;
    uint Fi = 0;
    uint Di = 0;
    uint ETU = 0;
	uchar TA1 = mode;
    memset(ucSendBuf, 0x00, sizeof(ucSendBuf));
    memset(ucRecvBuf, 0x00, sizeof(ucRecvBuf));
//D2(TRACE("\r\n PPS mode=%x", mode););
    gl_CardInfo.Parity_Check_Enable = 0x01;
    gl_CardInfo.k_Ic_CharWait_TimeCount = gl_CardSlotInfo[gl_CardInfo.current_slot].WWT;
    gl_CardInfo.k_RecvFirst = gl_CardInfo.k_Ic_CharWait_TimeCount - 11;

    if(!(gl_CardSlotInfo[gl_CardInfo.current_slot].TAFlag & 0x01))  // ��TA1����Э�̣��԰���D=1ͨ��
    {
        return ICC_SUCCESS;
    }

    if((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F) == 0x01) // TA1ΪD=1,��û�б�ҪЭ��
    {
        return ICC_SUCCESS;
    }

    // �ж��Ƿ�Ϊ�Ϸ���Fiֵ
    if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0x70)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0x80)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0xE0)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0xF0) == 0xF0))
    {
        // ��֧�ֵ�Fiֵ����Э���԰�Initial ETUͨ��
        return ICC_SUCCESS;
    }
    Fi = ((TA1 & 0xF0) >> 4);

    // �ж��Ƿ�Ϊ�Ϸ���Diֵ
    if(((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F) == 0x00)
        || ((gl_CardSlotInfo[gl_CardInfo.current_slot].TA1 & 0x0F) > 0x09))
    {
        // ��֧�ֵ�Diֵ����Э���԰�Initial ETUͨ��
        return ICC_SUCCESS;
    }
    Di = (TA1 & 0x0F);
	ETU = (F_Table[Fi]/D_Table[Di]);
    // ���֧��5MHz
	ucSendBuf[0] = 0xFF;
    ucSendBuf[1] = 0x10 | gl_CardSlotInfo[gl_CardInfo.current_slot].T;
	ucSendBuf[2] = TA1;
	ucSendBuf[3] = (ucSendBuf[0]^ucSendBuf[1]^ucSendBuf[2]);

	icc_init_inter_timer();
	icc_init_etu_timer();
	gl_char_wait_etu = 0;
	gl_total_atr_etu = 0;
	icc_delay_etu(gl_CardSlotInfo[gl_CardInfo.current_slot].ETU,8);
LABLE(D_PPS_ST);
    // ����PPSS
    Ret = icc_SendByte(ucSendBuf[0]);
    if(Ret)
    {
        return (-ICC_COMMERR);
    }

    // ����PPS0
    Ret = icc_SendByte(ucSendBuf[1]);
    if(Ret)
    {
        return (-ICC_COMMERR);
    }

    // ����PPS1
    Ret = icc_SendByte(ucSendBuf[2]);
    if(Ret)
    {
        return (-ICC_COMMERR);
    }

    // ����PPCK
    gl_CardInfo.k_Ic_LastSendByte = 1;
    Ret = icc_SendByte(ucSendBuf[3]);
    gl_CardInfo.k_Ic_LastSendByte = 0;
    if(Ret)
    {
        return (-ICC_COMMERR);
    }
    for(i = 0; i < 4; i++)
    {
		ucRecvBuf[i] = icc_RecvByte();
        if(gl_CardInfo.k_IccComErr)
        {
            if(gl_CardInfo.k_IccErrTimeOut)
            {
            	D2(TRACE("\r\n PPS��ʱ"););
                return (-ICC_TIMEOUT);
            }
            else
            {
                return (-ICC_COMMERR);
            }
        }
    }
    // �ȽϷ��ͽ��յ��ַ��Ƿ�һ��
    if(memcmp(ucSendBuf, ucRecvBuf, sizeof(ucRecvBuf)))
    {
        return (-ICC_RESPERR);
    }

    gl_CardSlotInfo[gl_CardInfo.current_slot].D = D_Table[Di];
    gl_CardSlotInfo[gl_CardInfo.current_slot].ETU = ETU;
//T0 ����ʱ�䡣
	Di = gl_CardSlotInfo[gl_CardInfo.current_slot].D;
	if(gl_CardSlotInfo[gl_CardInfo.current_slot].PortMode == SHBMODE)
	{
		i = ((960 * Di * gl_CardSlotInfo[gl_CardInfo.current_slot].TC2) + 1);
	}
	else
	{
		i = ((960 * Di * gl_CardSlotInfo[gl_CardInfo.current_slot].TC2) + (480 * Di) + 1);
	}
	gl_CardSlotInfo[gl_CardInfo.current_slot].WWT = (i);
	//TRACE("\r\n Di:%d ETU:%d TC2:%02X WWT:%d",Di,ETU,gl_CardSlotInfo[gl_CardInfo.current_slot].TC2,gl_CardSlotInfo[gl_CardInfo.current_slot].WWT);

//T1 ����ʱ�䡣
	Di = gl_CardSlotInfo[gl_CardInfo.current_slot].TB3;
	i = (((uint)0x01<<((Di & 0xF0) >> 4))*960*gl_CardSlotInfo[gl_CardInfo.current_slot].D) + 11;
	i += (gl_CardSlotInfo[gl_CardInfo.current_slot].D * 960);
	i += 1;
    gl_CardSlotInfo[gl_CardInfo.current_slot].BWT = i;
	gl_CardSlotInfo[gl_CardInfo.current_slot].BWT_TEMP = i;
	D2(
		TRACE("\r\n Di:%d BWT:%d-%d",Di,gl_CardSlotInfo[gl_CardInfo.current_slot].BWT,i);
	);
    icc_adjustTiming();
    return ICC_SUCCESS;
}

#endif

