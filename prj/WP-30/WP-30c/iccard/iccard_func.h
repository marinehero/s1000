#ifndef ICCARD_FUNC_H_
#define ICCARD_FUNC_H_

#define   CARD_SLOTNUM   3

typedef struct
{
	uchar   VolMode;           // ��ǰ������ѹ
	uchar   PortMode;          // ����ģʽ
	uchar   BaudRate;          // �ϵ粨����
	uchar   ExChangeBaudRate;  //���в�����
	uchar   BaudRateClassFlag; //������ͳ�ƿ���
	uchar   OnlyAtrFlag;       //ֻȡATR�Ŀ���

	uchar   CardPowerOn;    // ���Ƿ����ϵ磬 0 - δ�ϵ磬 1 - ���ϵ�
	uchar   ResetStatus;    // ��ǰ��λ�׶Σ� 0 - δ��λ�� 1 - �临λ�� 2 - �ȸ�λ
	uchar   CardInsert;		// ���Ƿ���λ
	uchar   T;              // ��ǰ��ƬЭ�飬 0 - T=0Э��     1 - T=1Э��

	uchar   D;              // Ƶ�ʷ�Ƶ����
	uint  	ETU;            // λ����ʱ��

	uchar   TS;             // ATR��ʼ�ַ�
	uchar   T0;             // T0�ַ�

	uchar   TAFlag;         // TAi�ֽڴ��ڱ�־
	uchar   TA1;
	uchar   TA2;
	uchar   TA3;
	uchar   TA4;

	uchar   TBFlag;         // TBi�ֽڴ��ڱ�־
	uchar   TB1;
	uchar   TB2;
	uchar   TB3;
	uchar   TB4;

	uchar   TCFlag;         // TCi�ֽڴ��ڱ�־
	uchar   TC1;
	uchar   TC2;
	uchar   TC3;
	uchar   TC4;

	uchar   TDFlag;         // TDi�ֽڴ��ڱ�־
	uchar   TD1;
	uchar   TD2;
	uchar   TD3;
	uchar   TD4;

	uchar   HistNum;        // ATRЯ������ʷ�ֽڸ���

	uchar   PPSENABLE;      // PPS��������,  0 - ������ 1 - ����
	uint  	IFSC;           // ICC�ܽ��ܵ����֡����
	uint  	IFSD;           // Device�ܽ��ܵ����֡����

	uint   	WWT;            // T=0Э����ַ������ʱ��
	uint   	CWT;            // T=1Э����ַ������ʱ��
	uint  	BWT;            // T=1Э��Ŀ������ʱ��
	uint  	BWT_TEMP;       // T=1Э�����ʱ�������ʱ��
	uint  	BGT;            // T=1�����С���򱣻�ʱ��

	uchar   sw1;
	uchar   sw2;

	uchar   TERM_PCB;       // T=1Э�����ն˷����¸��ֶθ�ʽ�����к�
	uchar   CARD_PCB;       // T=1Э���¿������¸��ֶθ�ʽ�����к�

	uchar   XI;				//ʱ��ָֹͣʾ��
	uchar   UI;				//���ָʾ��
}ICCARD_WORK_STRUCT;

// ATR�����д��󷵻ض���
#define   ATR_TSERR          0x11  /*  ������Լ������  */
#define   ATR_TCKERR         0x12  /*  ��λУ�����    */
#define   ATR_TIMEOUT        0x13  /*  ��λ�ȴ���ʱ    */
#define   ATR_TA1ERR         0x14  /*  TA1����         */
#define   ATR_TA2ERR         0X15  /*  TA2����         */
#define   ATR_TA3ERR         0x16  /*  TA3����         */
#define   ATR_TB1ERR         0x17  /*  TB1����         */
#define   ATR_TB2ERR         0x18  /*  TB2����         */
#define   ATR_TB3ERR         0x19  /*  TB3����         */
#define   ATR_TC1ERR         0x1A  /*  TC1����         */
#define   ATR_TC2ERR         0x1B  /*  TC2����         */
#define   ATR_TC3ERR         0x1C  /*  TC3����         */
#define   ATR_TD1ERR         0x1D  /*  TD1����         */
#define   ATR_TD2ERR         0x1E  /*  TD2����         */
#define   ATR_LENGTHERR      0x1F  /*  ATR���ݳ��ȴ���  */
#define   ATR_PARERR         0x20  /*  ATR�ַ���żУ���*/

// T=0Э��ͨ�Ź����д��󷵻ض���
#define   T0_TIMEOUT         0x21  /*  T=0�ȴ�ʱ�����  */
#define   T0_MORESENDERR     0x22  /*  T=0�ַ��ط���������*/
#define   T0_MORERECEERR     0x23  /*  T=0�����ַ����մ�������*/
#define   T0_PARERR          0x24  /*  T=0�ַ�������ż����*/
#define   T0_INVALIDSW       0x25  /*  T=0���ش���״̬��*/

// T=1Э��ͨ�Ź����д��󷵻ض���
#define   T1_BWTERR          0x31  /*   T=1����ȴ�ʱ�����  */
#define   T1_CWTERR          0x32  /*   T=1�ַ��ȴ�ʱ�����  */
#define   T1_ABORTERR        0x33  /*   ����ͨ��            */
#define   T1_EDCERR          0x34  /*   У�������          */
#define   T1_SYNCHERR        0x35  /*   ͬ������            */
#define   T1_EGTERR          0x36  /*   �ַ�����ʱ�����    */
#define   T1_BGTERR          0x37  /*   ���鱣��ʱ�����    */
#define   T1_NADERR          0x38  /*   ������NAD����       */
#define   T1_PCBERR          0x39  /*   ����PCB����         */
#define   T1_LENGTHERR       0x3A  /*   ����LEN����         */
#define   T1_IFSCERR         0x3B  /*   IFSC����            */
#define   T1_IFSDERR         0x3C  /*   IFSD����            */
#define   T1_MOREERR         0x3D  /*   ��δ��ʹ��󲢷���  */
#define   T1_PARITYERR       0x3E  /*   T=1�������ż�����ֽ� */
#define   T1_INVALIDBLOCK    0x3F  /*   Ϊ�Ƿ�T=1��*/

// �籣��ATR�����д��󷵻ض���
#define   ATR_TSERR_SHB          0x41  /*  ������Լ������  */
#define   ATR_TIMEOUT_SHB		 0x42  /*ATR �ֽڳ�ʱ*/

extern ICCARD_WORK_STRUCT gl_CardSlotInfo[CARD_SLOTNUM];
extern ushort const F_Table[16];
extern uchar const D_Table[16];

void s_InitCardInfo(void);
void icc_InitICCTiming();

void icc_adjustTiming(void);
int  icc_ColdReset(uchar *ATR);
int  icc_WarmReset(uchar *ATR);
int inter_icc_exchange(int sendlen, uchar *psenddata, uint *precvlen, uchar *precvdata);
int  icc_ifsdCmd(void);
int icc_PPSRequest(void);
int icc_PPSSelect(int mode);

#endif

