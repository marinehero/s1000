/*
 * =====================================================================================
 *
 *       Filename:  libapi.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/8/2014 11:46:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  
 *        Company:  start
 *
 * =====================================================================================
 */

#ifndef __LIBAPI_H__
#define __LIBAPI_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <io_macros.h>
#include <assert.h>

//#include "MacroDef.h"
//*********************************************************************
//
//                          ���Ͷ���
//
//*********************************************************************
#ifndef uchar
	#define uchar unsigned char
	#define vuchar volatile uchar
#endif
#ifndef UCHAR
	#define UCHAR unsigned char
	#define VUCHAR volatile UCHAR
#endif 
#ifndef puchar
	#define puchar unsigned char *
	#define vpuchar volatile puchar
#endif
#ifndef PUCHAR
	#define PUCHAR unsigned char *
	#define VPUCHAR volatile PUCHAR
#endif

#ifndef ushort
	#define ushort unsigned short
	#define vushort volatile ushort
#endif
#ifndef USHORT
	#define USHORT unsigned short
	#define VUSHORT volatile USHORT
#endif

#ifndef pushort
	#define pushort unsigned short *
	#define vpushort volatile pushort
#endif
#ifndef PUSHORT
	#define PUSHORT unsigned short *
	#define VPUSHORT volatile PUSHORT
#endif

#ifndef uint
	#define uint unsigned int
	#define vuint volatile uint
#endif
#ifndef UINT
	#define UINT unsigned int
	#define VUINT volatile UINT
#endif

#ifndef puint
	#define puint unsigned int *
	#define vpuint volatile puint
#endif
#ifndef PUINT
	#define PUINT unsigned int *
	#define VPUINT volatile PUINT
#endif

#ifndef ulong
	#define ulong  unsigned long
	#define vulong volatile ulong
#endif
#ifndef ULONG
	#define ULONG  unsigned long
	#define VULONG volatile ULONG
#endif

#ifndef pulong
	#define pulong  unsigned long *
	#define vpulong volatile pulong
#endif
#ifndef PULONG
	#define PULONG  unsigned long *
	#define VPULONG volatile PULONG
#endif 
#ifndef ssize_t
	#define ssize_t  int
	#define size_t   unsigned int
#endif 
typedef struct
{
	uchar AppName[32];
	uchar AppVer[16];
	uchar AppProvider[32];
	uchar BuildTime[14];
	uchar Descript[64];
	int AppNo;
} app_info_t;

#define UPAREA   @ ".uparea"    //������Ƭ��
#define DOWNAREA   @ ".downarea"    //������Ƭ��
//*********************************************************************
//
//                          ��������
//
//*********************************************************************
#define  YES                 0x01
#define  NO                  0x00

#define  OK                  0x00
#define  ERROR               0x01

#define  ON                  0x01
#define  OFF                 0x00

//*********************************************************************
//
//                          ��ģ��API����
//
//*********************************************************************


//*********************************************************************
//                          �㷨API����
//*********************************************************************
#define TDEA_ENCRYPT            0x01
#define TDEA_DECRYPT            0x00
/* RSA public and private key. */
#define MIN_RSA_MODULUS_BITS    508
#define MAX_RSA_MODULUS_BITS    2048
#define MAX_RSA_MODULUS_LEN     ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS      ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN       ((MAX_RSA_PRIME_BITS + 7) / 8)

typedef struct
{
	uint bits;							/* length in bits of modulus */
	uchar modulus[MAX_RSA_MODULUS_LEN];	/* modulus ���ģʽ*/
	uchar exponent[4];					/* public exponent 
                                           00 00 00 03(3) 
                                           00 01 00 01(65537)
                                       */
} R_RSA_PUBLIC_KEY;

typedef struct
{
	uint bits;							/* length in bits of modulus */
	uchar modulus[MAX_RSA_MODULUS_LEN];	/* modulus ���ģʽ HSB+LSB*/
	uchar publicExponent[4];			/* public exponent */
	uchar exponent[MAX_RSA_MODULUS_LEN];	/* private exponent */
	uchar prime[2][MAX_RSA_PRIME_LEN];	/* prime factors */
	uchar primeExponent[2][MAX_RSA_PRIME_LEN];	/*exponents for CRT */
	uchar coefficient[MAX_RSA_PRIME_LEN];	/* CRT coefficient */
} R_RSA_PRIVATE_KEY;

// Hash���ݽṹ���壺
typedef struct
{
	uint digest[5];						/* message digest */
	uint count_lo, count_hi;			/* 64-bit bit count */
	uchar data[64];						/* SHA data buffer */
	int local;							/* unprocessed amount in data */
} SHA_INFO;

ushort calc_crc16(const void *src_buf, uint bytelen, ushort pre_value);
int calc_tdea(const void *src, void *dst, const uchar * deskey, uint keylen, uint mode);
int rsa_pub_dec(void *outbuf, uint * outlen, const void *inbuf, uint inlen,
	const R_RSA_PUBLIC_KEY * pubkey);
int rsa_pri_enc(void *outbuf, uint * outlen, const void *inbuf, uint inlen,
	const R_RSA_PRIVATE_KEY * prikey);
void sha_init(SHA_INFO * sha_info);
void sha_update(SHA_INFO * sha_info, const void *src, int count);
void sha_final(void *digest, SHA_INFO * sha_info);
void Hash(uchar * DataIn, uint DataInLen, uchar * DataOut);

//SM�����㷨�û��ӿ�
int sm2_generate_pubk(char **sm2_param, int type, int point_bit_length, uchar* privatekey,uchar* public_x,uchar* public_y);
int sm2_encrypt(char **sm2_param, int type, int point_bit_length, uchar* message, uchar* encipher,
	uchar* public_x, uchar* public_y) ;
int sm2_decrypt(char **sm2_param, int type, int point_bit_length, uchar* privatekey, uchar* encryptmessage,
	int decrymessage_length, uchar* decrymessage) ;
int sm2_sign(uchar *message_digest, int length, uchar *id,  int idlength, uchar *privatekey, uchar *rs, uchar *publickey);
int sm2_verify(uchar *message_digest, int length, uchar *key, uchar *id, int idlength);
void sm3(uchar *input, int ilen, uchar output[32]);
int sm4_encrypt(uchar *encrymessage, int length, uchar *key, uchar *output);
int sm4_decrypt(uchar *decrymessage, int length, uchar *key, uchar *output);


//*********************************************************************
//                         ϵͳģ�鶨��
//*********************************************************************
// λͼ����
#define EM_BMP_TRANSPARENCE             0x00
#define EM_BMP_NOT_TRANSPARENCE         0x01

#define EM_BMP_ROTATE0                  0x00
#define EM_BMP_ROTATE90                 0x01
#define EM_BMP_ROTATE180                0x02
#define EM_BMP_ROTATE270                0x03

#define     SUPPLY_UN_AFFIRM_STATE         0x0
#define     SUPPLY_VIN_BYOFF_STATE         0x1  //������������
#define     SUPPLY_VIN_BYON_STATE          0x2  //����������+���
#define     SUPPLY_BATTERY_NOR_STATE       0x3  //���������ѹ����
#define     SUPPLY_BATTERY_LOW_STATE       0x4  //��ص͵�ѹ����
#define     SUPPLY_USB_STATE               0x5  //USB����
#define     SUPPLY_VIN_BYFULL_STATE        0x6  //����������+��س���

#define     SUPPLY_BATTERY_STATE           0x3  //��ع��� Ӧ����

int sys_FirstRun(void);
void sys_ReadSn(void *sn);
int sys_WriteSn(void *sn);
void sys_ReadSn_new(void *sn);
int sys_WriteSn_new(void *sn);
int sys_FirstRunExp(void);
void sys_Beep(void);
void sys_BeepPro(uint freq, uint duration_ms, int choke);
int sys_SetTime(const void  *intime);
void sys_GetTime(void *outtime);
uint sys_GetCounter(void);
void sys_DelayMs(uint duration_ms);
void sys_Reset(void);
int sys_GetBattery(int *pMV);
int sys_GetBatteryPercentum(void);
int sys_GetSupplyState(void);
void sys_Shutdown(void);
int sys_auto_poweroff(int mode,uint32_t timeout_ms);
int sys_GetModuleType(int module);


typedef struct
{
    uchar charset;        //  �ַ���
    uchar height;         //  ����߶�
    uchar width;          //  ������
    uchar rfu;
}_exfontinfo;


int sys_GetSysinfo(uint type, uint *outlen, void *outbuf);
int sys_SetSysinfo(uint type, uint inlen, void *inbuf);

//*********************************************************************
//                         LEDģ�鶨��
//*********************************************************************
#define LED_POWER           1
#define LED_INFORMATION     2
#define LED_KEY             3
#define LED_BLUE            4
#define LED_YELLOW          5 
#define LED_GREEN           6
#define LED_RED             7 
//#define LED_ORANGE          8
int sys_SetLed(int led, int value);
//*********************************************************************
//                         Һ��ģ�鶨��
//*********************************************************************
#define     LCD_WIDTH           128
#define     LCD_HEIGHT          64
//  lcd_display��ʾģʽ
#define     FONT_SIZE8          8       //  С������ʾ(6*8) �޺���
#define     FONT_SIZE12         12      //  ��������ʾ(8*12) ����(12*12)
#define     FONT_SIZE16         16      //  ��������ʾ(8*16) ����(16*16)
#define     FONT_SIZE24         24      //  ��������ʾ(12*24) ����(24*24)
#define     DISP_CLRLINE        0x100   //  �����ʾռ�õ���
#define     DISP_MEDIACY        0x200   //  ������ʾ
#define     DISP_INVCHAR        0x400   //  ������ʾ���ַ���
#define     DISP_INVLINE        0x800   //  ������ʾ�����У�

////  ͼ�����
//#define     ICON_PHONE          1		//  �绰
//#define     ICON_SIGNAL         2		//  �ź�
//#define     ICON_PRINTER        3		//  ��ӡ��
//#define     ICON_ICCARD         4		//  IC����λ�ź�
//#define     ICON_LOCK           5		//  ����
//#define     ICON_BATTERY        6		//  ���
#define     ICON_UP             7		//  ����
#define     ICON_DOWN           8		//  ����

//  ����ģʽ
#define     LIGHT_OFF_TEMP      0		//  ������ʱ�ر�(���ڶ�ʱģʽ��)
#define     LIGHT_TIMER_MODE    1		//  ���ⶨʱģʽ
#define     LIGHT_ON_MODE       2		//  ���ⳣ��ģʽ
#define     LIGHT_OFF_MODE      3		//  ���ⳣ��ģʽ

int lcd_SetLight(int light_mode);
int lcd_SetLighttime(int duration_ms);
void lcd_Cls(void);
void lcd_ClrLine(int begline, int endline);
void lcd_Goto(int x, int y);
int lcd_SetInv(int inv_mode);
void lcd_Plot(int x, int y, int color);
void lcd_GetPos(int *x, int *y);
void lcd_DrawLine(uint8_t sx,uint8_t sy,uint8_t ex,uint8_t ey,uint8_t val);
void lcd_Display(int x, int y, int disp_mode, const char *format, ...);
void lcd_Printf(const char *format, ...);
int lcd_SetIcon(int icon_no, int icon_mode);
void lcd_BmpDisp(int x, int y, const void *bmpdata);
void lcd_BmpmapDisp(int x, int y, int width, int hight, void *scrbmp);

typedef struct
{
	uchar frametype;
	uchar button;
    uchar titlealign;
    uchar contextalign;
    ushort rfu;
    ushort titlelen;
	uchar* title;
    ushort contextlen;
    uchar* context;
}s_lcd_emv;


//*********************************************************************
//                         ����ģ�鶨��
//*********************************************************************
#define     KEY0                    0x30	//  '0'
#define     KEY1                    0x31	//  '1'
#define     KEY2                    0x32	//  '2'
#define     KEY3                    0x33	//  '3'
#define     KEY4                    0x34	//  '4'
#define     KEY5                    0x35	//  '5'
#define     KEY6                    0x36	//  '6'
#define     KEY7                    0x37	//  '7'
#define     KEY8                    0x38	//  '8'
#define     KEY9                    0x39	//  '9'
#define     KEY_BACKSPACE           0x08	//  back
#define	    KEY_CLEAR				0x2E
#define     KEY_SYMBOL              0x13
#define     KEY_ALPHA               0x07    //��ĸ
#define     KEY_UP                  0x26
#define     KEY_DOWN                0x28
#define     KEY_F1                  0x15
#define     KEY_F2                	0x14
#define     KEY_F3                  0x16
#define     KEY_F4                  0x17
#define     KEY_F8                  0x18
#define     KEY_ENTER               0x0D
#define     KEY_CANCEL              0x1B
#define     KEY_FUNC                KEY_F1 
#define     KEY_STAR                KEY_SYMBOL 
#define     KEY_INVALID             0xFF
#define     KEY_TIMEOUT             0x00

//  ��������ƺͷ���ֵ
#define 	KB_MAXLEN          		256         //  �����������������ĳ���
#define 	KB_EN_REVDISP     		0x01        //  1��0�� ����������ʾ
#define 	KB_EN_FLOAT        		0x02        //  1��0�� �У���С����
#define 	KB_EN_SHIFTLEFT   		0x04        //  1��0�� ���ң���������
#define 	KB_EN_CRYPTDISP   		0x08        //  1��0�� �ǣ������뷽ʽ
#define 	KB_EN_CHAR         		0x10        //  1��0�� �ܣ������ַ�
#define 	KB_EN_NUM           	0x20        //  1��0�� �ܣ���������
#define 	KB_EN_BIGFONT      		0x40        //  1��0�� ��С������
#define 	KB_EN_PRESETSTR    		0x80    	//  1��0�� �ǣ�������Ԥ����ַ���
#define     KB_EN_NEWLINE           0x100       //  1��0�� �ǣ���������������뻻����ʾ//  Ԥ�輸�鳣�õ��������ģʽ
#define     KB_BIG_ALPHA            0x1D5       //  �������ַ�������
#define     KB_BIG_NUM              0x1E5       //  ���������ִ�����
#define     KB_BIG_PWD              0x4D        //  ��������������
#define     KB_SMALL_ALPHA          0x195       //  С�����ַ�������
#define     KB_SMALL_NUM            0x1A5       //  С�������ִ�����
#define     KB_SMALL_PWD            0xD         //  С������������

#define     KB_CANCEL               3501     //  �����뱻�û���ȡ�����˳�
#define     KB_TIMEOUT              3502     //  �����볬ʱ�˳�
#define     KB_ERROR                3503      //  ����������Ƿ�
#define     KB_OVERBUF              3504      //  ������������
#define     KB_CHANGE               3505      //  ���л����˳�

//	�������ģʽ
#define		KB_HZMODE_NORMAL		(0<<1)			//����ģʽ
#define		KB_HZMODE_EX			(1<<0)		//��չģʽ


//Ĭ�����뷨������ KB_HZMODE_NORMAL,KB_HZMODE_NORMALʹ��"��"���

#define 	KB_HZMODE_CUR_PY 			(0<<1)      //Ĭ��ƴ�����뷨
#define 	KB_HZMODE_CUR_BH 			(1<<1)		//Ĭ�ϱʻ����뷨
#define 	KB_HZMODE_CUR_CHAR 			(2<<1)		//Ĭ���ַ����뷨
#define 	KB_HZMODE_CUR_NUM 			(3<<1)		//Ĭ���������뷨

void kb_Sound(int freq, int duration_ms);
int kb_Hit(void);
int kb_GetKey(int timeout_ms);
void kb_Flush(void);
int kb_GetStr(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf);
int kb_GetHz(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf);
int kb_InKey(int key);
int kb_ReadKey(uchar *keybuf);


//*********************************************************************
//                         ��ӡģ�鶨��
//*********************************************************************
#define     PRN_WIDTH                   384
#define     PRN_DEFAULT_FONT_WIDTH      12
#define     PRN_DEFAULT_FONT_HEIGHT     24

#define     PRN_DEFAULT_TAB_WIDTH       12

//  ����ֵ
#define     EPRN_BUSY            3601
#define     EPRN_PAPEROUT        3602
#define     EPRN_ERROR           3603
#define     EPRN_OVERHEAT        3604
#define     EPRN_BUFFULL         3605

int prn_Reset(void);
int prn_SetFont(int prnfont);
void prn_SetZoom(int x_zoom, int y_zoom);
void prn_SetSpace(int text_space, int line_space);
int prn_SetTab(int Width);
int prn_SetIndent(int indent);
int prn_GetCurline(void);
void prn_Move(int line_offset);
int prn_GetTemperature(void);
int prn_Printf(const char *format, ...);
int prn_Bmp(int rotate, const void *bmpbuf);
int prn_Pic(int Width, int Height, const uchar *bmpbuf);
int prn_Start(void);
int prn_GetStatus(void);
//����
#define PRN_ATTRI_ROTATE    0 //��ӡ��תģʽ	
	#define PRN_ROTATE_0          0
	#define PRN_ROTATE_90         1

#define PRN_ATTRI_GRAY      1 //��ӡ���	
	#define PRN_GRAY_MIN          0
	#define PRN_GRAY_MAX          6
	#define PRN_GRAY_DEFAULT      PRN_GRAY_MIN

#define PRN_ATTRI_PAGESIZE  2 //ҳ��С 
	#define PRN_PAGESIZE_MIN      8
	#define PRN_PAGESIZE_MAX      1280
	#define PRN_PAGESIZE_DEFAULT  PRN_PAGESIZE_MAX

#define PRN_ATTRI_DEFAULT  3 //Ĭ������
int prn_GetAttribute(int attri, int *attrivalue);
int prn_SetAttribute(int attri, int attrivalue);
int prn_Open(void);
int prn_Close(void);



//*********************************************************************
//                         IC��ģ�鶨��
//*********************************************************************
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

int icc_InitModule(int CardSlot, uint CardVol, uint CardMode);
int icc_CheckInSlot(int CardSlot);
int icc_CheckVcc(int CardSlot);
int icc_Reset(int CardSlot, uint *pAtrLen, uchar *pAtrData);
int icc_ExchangeData(int CardSlot, int sendlen, uchar *psenddata, int *precvlen, uchar *precvdata);
int icc_Close(int CardSlot);

//*********************************************************************
//                         �ſ�ģ�鶨��
//*********************************************************************
typedef struct MAGCARD
{
	uchar ucTrkFlag;
	uchar ucTrkLength;
	uchar aucTrkBuf[108+2];
} ET_MAGCARD;

void mag_Open(void);
void mag_Close(void);
void mag_Clear(void);
int mag_Check(void);
int mag_Read(void *track1, void *track2, void *track3);

//*********************************************************************
//                         ��Ƶ��ģ�鶨��
//*********************************************************************
// ��Ƶ��
// Ѱ��ģʽ
// D0: Ѱ����ʽ 0-ISO 1-EMV   
// D1: �Ƿ��жϿ���֡�� 0-���ж� 1-�ж�
// D2: �Ƿ�֧��CID 
// D3: �Ƿ�֧��NAD 
// D8: ֧��TYPEA�� (�˹�����ʱ��֧��)
// D9: ֧��TYPEB�� (�˹�����ʱ��֧��)
#define RFID_MODE_ISO                (0<<0)   //ISO��ʽѰ�������ԴӶ��ſ���Ѱ��1�ſ�
#define RFID_MODE_EMV                (1<<0)   //EMV��ʽѰ����ֻ����1�ſ�
#define RFID_MODE_NOFRAME            (1<<1)   //���жϿ��˷���֡��
#define RFID_MODE_CID                (1<<2)   //֧��CID
#define RFID_MODE_NAD                (1<<3)   //֧��NAD
#define RFID_MODE_ALL                (0<<0)   //��ISO��ʽһ��

// ������
#define RFID_CARD_S50                0
#define RFID_CARD_S70                1
#define RFID_CARD_PRO                2
#define RFID_CARD_PROS50             3
#define RFID_CARD_PROS70             4
#define RFID_CARD_B                  5
// ����ֵ
enum RFID_RET{
	RFID_SUCCESS = 0,
	RFID_ERROR = 4601,   //��ģ��ʧ��
	RFID_ERRPARAM,	     //��������
	RFID_TIMEOUT,        //��ʱ
	RFID_NOCARD,         //�޿�
	RFID_TRANSERR,       //��Ƭ�������֮��ͨ�Ŵ���
	RFID_PROTERR,        //��Ƭ�������ݲ�����ISO14443����PAYPASS�淶
	RFID_MULTIERR,	     //��Ӧ���࿨����	
	RFID_NOACT,          //��δ�ϵ�
	RFID_CARDEXIST,      //��δ�뿪
	RFID_NOAUTH,         //δ��֤
	RFID_AUTHERR,        //��֤ʧ��
	RFID_UNCHANGE,		 //�����޸�
	RFID_KEYNOTPOWER,	 //��Կ��Ȩ��
};
typedef struct _BLKALTER
{
	uchar mode;
	uchar access;
	uchar keyA[6];
	uchar keyB[6];
	uchar rfu[2];
}BLKALTER;
int rfid_open(uint mode);
int rfid_close(void);
int rfid_poll(uint mode, uint *cardtype);
int rfid_powerup(uint cardtype, uint *puiOutLen, uchar *pvOut);
int rfid_exchangedata(uint sendlen, uchar *psenddata, uint *precvlen, uchar *precvdata);
int rfid_powerdown(void);
int rfid_MIFAuth(uint Sec, uint keytype, uchar *pData);
int rfid_MIFRead(uint block, uchar *out);
int rfid_MIFWrite(uint block, uchar *in);
int rfid_MIFIncrement(uint block, ulong value);
int rfid_MIFDecrement(uint block, ulong value);
int rfid_MIFRestore(uint block);
int rfid_MIFTransfer(uint block);
int rfid_MIFModify(uint block, const BLKALTER *alterVal);
void rfid_MIFMakevalue(long value,uchar addr, uchar *out);
int rfid_ResetField(int para);

//*********************************************************************
//                         �ļ�ϵͳģ�鶨��
//*********************************************************************
//  ����ֵ����
#define     FS_INVAL            3901
#define     FS_EXIST            3902
#define     FS_NOTEXIST         3903
#define     FS_OPENED           3904
#define     FS_NOTOPENED        3905
#define     FS_NOSPACE          3906
#define     FS_MAXFILENO        3907
//  �ļ���ģʽ�������
#ifndef O_RDWR
#define     O_RDWR              02
#endif
#ifndef O_CREAT
#define     O_CREAT             0100
#endif
//  �ļ���λ���
#ifndef SEEK_SET
#define     SEEK_SET            0
#endif
#ifndef SEEK_CUR
#define     SEEK_CUR            1
#endif
#ifndef SEEK_END
#define     SEEK_END            2
#endif

typedef struct
{
	char filename[32];
	int belong_app;
	int file_len;
} file_attr_t;

int fs_Open(const char *file_name, int open_mode);
int fs_Close(int fileno);
int fs_Seek(int fileno, int offset, int whence);
int fs_Read(int fileno, void *outbuf, int bytelen);
int fs_Write(int fileno, const void *inbuf, int bytelen);
int fs_Size(const char *file_name);
int fs_Exist(const char *file_name);
int fs_Truncate(int fileno);
int fs_Remove(const char *file_name);
int fs_Rename(const char *oldname, const char *newname);
int fs_Freespace(void);
int fs_List(int begno, file_attr_t * fileattr);
//*********************************************************************
//                         ����ģ�鶨��
//*********************************************************************
// ��������ֵ����
#define     UART_COM1				0x01        //  ���ô���1�����أ�
#define     UART_COM2				0x02        //  ���ô���2
#define     UART_COM3				0x03        //  ���ô���3
#define     UART_COM4				0x04        //  ���ô���4

//  ����ֵ
#define     EUART_VAL                4101        //  ��Ч��ͨѶ����
#define     EUART_NOTOPEN            4102        //  ����δ��
#define     EUART_BUSY               4103        //  ����æ
int uart_Open(int uart_id, const char *uart_para);
int uart_Read(int uart_id, void *outbuf, int bytelen, int timeout_ms);
int uart_Write(int uart_id, const void *inbuf, int bytelen);
int uart_Close(int uart_id);
int uart_Printf(int uart_id, const char *format, ...);
int uart_Flush(int uart_id);
//int uart_CheckWriteBuf(int uart_id);
int uart_CheckReadBuf(int uart_id);
//int uart_CheckWriteBufAvailable(int uart_id);

//*********************************************************************
//                         USB CDCģ�鶨��
//*********************************************************************
#define     USBSER_VAL                4201        //  ��Ч��ͨѶ����
#define     USBSER_NOTOPEN            4202        //  USB����δ��
#define     USBSER_BUSY               4203        //  USB����æ
#define     USBSER_NOTREADY           4204        //  USB����δ׼����
#define     USBSER_TIMEOUT            4205        //  ��ʱ
#define     USBSER_NOTACTIVE          4206        //  USB����δ����
int usbdserial_Open(void);
int usbdserial_Close(void);
int usbdserial_Read(void *data, unsigned int size, int timeout_ms);
int usbdserial_Write(void *data, unsigned int size);
int usbdserial_Printf(const char *format, ...);
int usbdserial_Clear(void);
int usbdserial_CheckReadBuf(void);
int usbdserial_Ready(void);

//*********************************************************************
//                         ����ģ�鶨��
//*********************************************************************
#define     BT_VAL                4301        //  ��Ч��ͨѶ����
#define     BT_NOTOPEN            4302        //  ��������δ��
#define     BT_BUSY               4303        //  ��������æ
#define     BT_ERROR              4304        //  ��������
#define     BT_NOTEXIST           4305        //  ����ģ�鲻����
#define     BT_NOTREADY           4306        //  δ����
#define     BT_NOTCONNECT         4307        // ����δ���������ܷ�������

#define BT_IOCTL_SET_NAME       1
#define BT_IOCTL_GET_NAME       2
#define BT_IOCTL_GET_PIN        3
#define BT_IOCTL_SET_PIN        4
#define BT_IOCTL_GET_BTADDR     5
#define BT_IOCTL_CHECK_CONNECTED 6

union bt_ioctl_arg
{
    char name[32];
    char pin[32];
    unsigned char addr[6];
};
int bt_Open(void);
int bt_Close(void);
int bt_Write(uchar *pucBuff, uint uiLen);
int bt_Printf(const char *format, ...);
int bt_Read(void *outbuf, int bytelen, int timeOut_ms);
int bt_Clear(void);
int bt_CheckWriteBuf(void);
int bt_CheckReadBuf(void);
int bt_SetName(uchar *pucBuff, uchar ucLen);
int bt_SetPin(uchar *pucBuff, uchar ucLen);
int bt_IoCtl(int cmd, union bt_ioctl_arg *arg);

//*********************************************************************
//                         flashģ�鶨��
//*********************************************************************
#define FILESYSTEM_STARTADDRESS  (uint32_t)0x0004F000
#define FILESYSTEM_ENDADDRESS    (uint32_t)0x00056FFF
#define FILESYSTEM_LEN           (uint32_t)(32*1024)
int flash_app_erasedata(uint addr);
int flash_app_writedata(uint FlashStartAddress, uint NumberOfBytes,uchar *data);
int flash_app_readdata(uint FlashStartAddress, uint NumberOfBytes,uchar *data);


/*************************************
 *  �͹���ģ��
 *************************************/
#define     EVENT_TIMEOUT           0x01
#define     EVENT_KEYPRESS          0x02
#define     EVENT_MAGSWIPED         0x04
#define     EVENT_ICCIN             0x08
#define     EVENT_UARTRECV          0x10
#define     EVENT_USBRECV           0x20
#define     EVENT_ALL  (EVENT_TIMEOUT|EVENT_KEYPRESS|EVENT_MAGSWIPED|EVENT_ICCIN|EVENT_UARTRECV|EVENT_USBRECV)
int sys_SleepWaitForEvent(uint waitevent, uint mode,uint timeout_ms);

#ifdef __cplusplus
}
#endif

#endif


