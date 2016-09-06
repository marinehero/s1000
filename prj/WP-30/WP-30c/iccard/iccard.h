#ifndef ICCARD_H_
#define ICCARD_H_
//#define __SHB_TEST_

#define UINT64  unsigned long
/*-----------------------------------------------------------------------------}
 *  ���Ժ�
 *-----------------------------------------------------------------------------{*/
#ifdef CFG_DBG_ICCARD 
extern unsigned int guiDebugi,guiDebugj,guiDebugk,guiDebugFlg;
extern unsigned char gcDebugBuf[1024];
extern unsigned char  gucDebugm,gucDebugn;
#define D_ATR_ST     (uchar )0xF0
#define D_ATR_END    (uchar )0xF1
#define D_PPS_ST     (uchar )0xF2
#define D_PPS_END    (uchar )0xF3

#define D_T0_ST      (uchar )0xF4
#define D_T0_END     (uchar )0xF5

#define D_T1_ST      (uchar )0xF6
#define D_T1_END     (uchar )0xF7
#define D_T1_SEND    (uchar )0xF8
#define D_T1_RECV    (uchar )0xF9

#define D_COLD       (uchar )0xFA
#define D_WARM       (uchar )0xFB

#define D_ATR_OUT    (uchar )0xFD  //ATR��ʱ
#define D_OUT        (uchar )0xFE  //��д��ʱ

#define INIT_DEBUGBUF()   do{guiDebugi=0;}while(0)
#define DISP_DEBUGBUF()   do{vDispBufTitle("��̨����",guiDebugi,0,gcDebugBuf);guiDebugi=0;}while(0)
#define DATAIN(x)         if(guiDebugFlg) {do{if(guiDebugi < (sizeof(gcDebugBuf)-1)) gcDebugBuf[guiDebugi++] = (unsigned char)x;else guiDebugi=0;}while(0);}
#define LABLE(x)          do{DATAIN(x);DATAIN(x);}while(0)
#define D1(x)             do{x}while(0)
#define D2(x)             do{x}while(0)
#define SETSIGNAL0_H()      //hw_gpio_set_bits(LED_GREEN_GPIO,1<<LED_GREEN_PINx)
#define SETSIGNAL0_L()      //hw_gpio_reset_bits(LED_GREEN_GPIO,1<<LED_GREEN_PINx)
#define SETSIGNAL1_H()      //hw_gpio_set_bits(LED_BLUE_GPIO,1<<LED_BLUE_PINx)
#define SETSIGNAL1_L()      //hw_gpio_reset_bits(LED_BLUE_GPIO,1<<LED_BLUE_PINx)
#define SETSIGNAL2_H()      //hw_gpio_set_bits(LED_YELLOW_GPIO,1<<LED_YELLOW_PINx)
#define SETSIGNAL2_L()      //hw_gpio_reset_bits(LED_YELLOW_GPIO,1<<LED_YELLOW_PINx)
#else
#define INIT_DEBUGBUF()   
#define DISP_DEBUGBUF()  
#define DATAIN(x)       
#define LABLE(x)       
#define D1(x)  
#define D2(x) 
#define SETSIGNAL1_H()
#define SETSIGNAL1_L()
#define SETSIGNAL2_H()
#define SETSIGNAL2_L()
#endif

// ������
#define     USERCARD         0x00        // ������
#define     SAM1SLOT         0x01        // SAM����1
#define     SAM2SLOT         0x02        // SAM����2
#define     SAM3SLOT		 0x03		 // SAM����3
#define     SAM4SLOT		 0x04		 // SAM����4
#define     SAM5SLOT		 0x05		 // SAM����5   ��ӦS-970���ö�����SAM1
#define     SAM6SLOT		 0x06		 // SAM����6   ��ӦS-970���ö�����SAM2
#define     SAM7SLOT		 0x07		 // SAM����7   ��ӦS-970���ö�����SAM3

// ICC��ͨ�Ų���
#define     VCC_5            0x05    	//  5V��
#define     VCC_3            0x03    	//  3V��
#define     VCC_18           0x01    	//  1.8V��

#define     ISOMODE          0x00    	// ����ISO7816�淶
#define     EMVMODE          0x01    	// ����EMV v4.1�淶
#define     SHBMODE          0x02    	// �籣�淶
#define     CITYUNIONMODE    0x03

#define 	B9600			 0x00
#define 	B19200			 0x01
#define 	B38400			 0x02
#define 	B55800			 0x03
#define 	B115200			 0x04
//  ����ֵ����
#define     ICC_SUCCESS          0         // �������ɹ�
#define     ICC_TIMEOUT          3801      // ����ʱ��Ӧ
#define     ICC_COMMERR          3802      // ��ͨ�Ŵ���(��ż���LRC��)
#define     ICC_RESPERR          3803      // ��Ƭ������Ϣ����
#define     ICC_CARDOUT          3804      // ������λ
#define     ICC_NOPOWER          3805      // ��δ�ϵ�
#define     ICC_ERRORSLOT        3806      // ��������
#define     ICC_ERRPARA          3807      // ��������
#define     ICC_CARDNOPOWER		 3808      // ����δ�ϵ�
#define     ICC_NODEVICE		 3809      // �޿�ģ�� 
#define     ICC_NOOPEN           3810      // ģ��δ��
#define     ICC_OPENED           3811      // ģ���Ѿ���
#define     ICC_ERRMODE          3812      // ����ģʽ����ͬ������ʹ�� 


/*-----------------------------------------------------------------------------}
 *  ȫ�ֱ���
 *-----------------------------------------------------------------------------{*/
extern const uchar gApduGet4rand[5];
extern const uchar gApduSelMaindir[8];

/*-----------------------------------------------------------------------------}
 *  ���� 
 *-----------------------------------------------------------------------------{*/
void icc_InitIccModule(void);
int icc_InitModule(int CardSlot, uint CardVol, uint CardMode);
int icc_CheckInSlot(int CardSlot);
int icc_CheckVcc(int CardSlot);
int icc_Reset(int CardSlot,uint *pAtrLen,uchar *pAtrData);
int icc_ExchangeData(int CardSlot, int sendlen, uchar *psenddata, int *precvlen, uchar *precvdata);
int icc_Close(int CardSlot);
int icc_GetInfo(int *ver,int *maxslot);
int icc_if_cardslot(int *CardSlot);
void vTest_Icc(void);

#endif

