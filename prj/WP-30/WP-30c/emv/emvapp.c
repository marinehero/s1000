//#include "../inc/libapi.h"
#include "lib_emv.h"
#include "emv.h"
#include "ICAdapter.h"
#include "emvcallback.h"

#define     EMV_USERCARD       USERCARD         // ����������
#define     EMV_RFCARD	       0xFF             // ������Ƶ
#define     EMV_MAX_PARALIST_NUM   10

int printk(const char *fmt, ...);
int console_check_used(void);
#define TRACE_DEBUG(...)        do{if(console_check_used() == 0)printk(__VA_ARGS__);}while(0)

unsigned char guc_OpenEMVFlag = 0;  //EMV�ں˴򿪱�־
static EMVPARA gt_emv_paralist[EMV_MAX_PARALIST_NUM];
static volatile int gn_emv_paralist_num;

// extern int glGenAdvice;

unsigned char ST_EMV_SetICPara(EMVPARA *pParaStru);
unsigned char ST_EMV_DelAllICPara(void);
extern int  AsciiToHex(unsigned char *upcHexBuf, unsigned char *upcAscBuf, int nConvLen, unsigned char ucType);
extern int  HexToAscii(unsigned char *upcAscBuf, unsigned char *upcHexBuf, int nConvLen, unsigned char ucType);

PKFILESTRU capk_cup_t02 =
{
	{0xA0, 0x00, 0x00, 0x03, 0x33},
	0x02,
	144,
	{0xa3,0x76,0x7a,0xbd,0x1b,0x6a,0xa6,0x9d,0x7f,0x3f,0xbf,0x28,0xc0,0x92,0xde,0x9e,0xd1,
	0xe6,0x58,0xba,0x5f,0x09,0x09,0xaf,0x7a,0x1c,0xcd,0x90,0x73,0x73,0xb7,0x21,0x0f,
	0xde,0xb1,0x62,0x87,0xba,0x8e,0x78,0xe1,0x52,0x9f,0x44,0x39,0x76,0xfd,0x27,0xf9,
	0x91,0xec,0x67,0xd9,0x5e,0x5f,0x4e,0x96,0xb1,0x27,0xca,0xb2,0x39,0x6a,0x94,0xd6,
	0xe4,0x5c,0xda,0x44,0xca,0x4c,0x48,0x67,0x57,0x0d,0x6b,0x07,0x54,0x2f,0x8d,0x4b,
	0xf9,0xff,0x97,0x97,0x5d,0xb9,0x89,0x15,0x15,0xe6,0x6f,0x52,0x5d,0x2b,0x3c,0xbe,
	0xb6,0xd6,0x62,0xbf,0xb6,0xc3,0xf3,0x38,0xe9,0x3b,0x02,0x14,0x2b,0xfc,0x44,0x17,
	0x3a,0x37,0x64,0xc5,0x6a,0xad,0xd2,0x02,0x07,0x5b,0x26,0xdc,0x2f,0x9f,0x7d,0x7a,
	0xe7,0x4b,0xd7,0xd0,0x0f,0xd0,0x5e,0xe4,0x30,0x03,0x26,0x63,0xd2,0x7a,0x57},
	1,
	{0x03,0x00,0x00},
	{0x20,0x25,0x12,0x30},
	0x01,
	{0x03,0xbb,0x33,0x5a,0x85,0x49,0xa0,0x3b,0x87,0xab,0x08,0x9d,0x00,0x6f,0x60,0x85,0x2e,0x4b,0x80,0x60}
};
PKFILESTRU capk_cup_t03 =
{
	{0xA0, 0x00, 0x00, 0x03, 0x33},
	0x03,
	176,
	{0xb0,0x62,0x7d,0xee,0x87,0x86,0x4f,0x9c,0x18,0xc1,0x3b,0x9a,0x1f,0x02,0x54,0x48,0xbf,
	0x13,0xc5,0x83,0x80,0xc9,0x1f,0x4c,0xeb,0xa9,0xf9,0xbc,0xb2,0x14,0xff,0x84,0x14,
	0xe9,0xb5,0x9d,0x6a,0xba,0x10,0xf9,0x41,0xc7,0x33,0x17,0x68,0xf4,0x7b,0x21,0x27,
	0x90,0x7d,0x85,0x7f,0xa3,0x9a,0xaf,0x8c,0xe0,0x20,0x45,0xdd,0x01,0x61,0x9d,0x68,
	0x9e,0xe7,0x31,0xc5,0x51,0x15,0x9b,0xe7,0xeb,0x2d,0x51,0xa3,0x72,0xff,0x56,0xb5,
	0x56,0xe5,0xcb,0x2f,0xde,0x36,0xe2,0x30,0x73,0xa4,0x4c,0xa2,0x15,0xd6,0xc2,0x6c,
	0xa6,0x88,0x47,0xb3,0x88,0xe3,0x95,0x20,0xe0,0x02,0x6e,0x62,0x29,0x4b,0x55,0x7d,
	0x64,0x70,0x44,0x0c,0xa0,0xae,0xfc,0x94,0x38,0xc9,0x23,0xae,0xc9,0xb2,0x09,0x8d,
	0x6d,0x3a,0x1a,0xf5,0xe8,0xb1,0xde,0x36,0xf4,0xb5,0x30,0x40,0x10,0x9d,0x89,0xb7,
	0x7c,0xaf,0xaf,0x70,0xc2,0x6c,0x60,0x1a,0xbd,0xf5,0x9e,0xec,0x0f,0xdc,0x8a,0x99,
	0x08,0x91,0x40,0xcd,0x2e,0x81,0x7e,0x33,0x51,0x75,0xb0,0x3b,0x7a,0xa3,0x3d},
	1,
	{0x03,0x00,0x00},
	{0x20,0x25,0x12,0x30},
	0x01,
	{0x87,0xf0,0xcd,0x7c,0x0e,0x86,0xf3,0x8f,0x89,0xa6,0x6f,0x8c,0x47,0x07,0x1a,0x8b,0x88,0x58,0x6f,0x26}
};

PKFILESTRU capk_cup_t04 =
{
	{0xA0, 0x00, 0x00, 0x03, 0x33},
	0x04,
	248,
	{0xbc,0x85,0x3e,0x6b,0x53,0x65,0xe8,0x9e,0x7e,0xe9,0x31,0x7c,0x94,0xb0,0x2d,0x0a,0xbb,
	0x0d,0xbd,0x91,0xc0,0x5a,0x22,0x4a,0x25,0x54,0xaa,0x29,0xed,0x9f,0xcb,0x9d,0x86,
	0xeb,0x9c,0xcb,0xb3,0x22,0xa5,0x78,0x11,0xf8,0x61,0x88,0xaa,0xc7,0x35,0x1c,0x72,
	0xbd,0x9e,0xf1,0x96,0xc5,0xa0,0x1a,0xce,0xf7,0xa4,0xeb,0x0d,0x2a,0xd6,0x3d,0x9e,
	0x6a,0xc2,0xe7,0x83,0x65,0x47,0xcb,0x15,0x95,0xc6,0x8b,0xcb,0xaf,0xd0,0xf6,0x72,
	0x87,0x60,0xf3,0xa7,0xca,0x7b,0x97,0x30,0x1b,0x7e,0x02,0x20,0x18,0x4e,0xfc,0x4f,
	0x65,0x30,0x08,0xd9,0x3c,0xe0,0x98,0xc0,0xd9,0x3b,0x45,0x20,0x10,0x96,0xd1,0xad,
	0xff,0x4c,0xf1,0xf9,0xfc,0x02,0xaf,0x75,0x9d,0xa2,0x7c,0xd6,0xdf,0xd6,0xd7,0x89,
	0xb0,0x99,0xf1,0x6f,0x37,0x8b,0x61,0x00,0x33,0x4e,0x63,0xf3,0xd3,0x5f,0x32,0x51,
	0xa5,0xec,0x78,0x69,0x37,0x31,0xf5,0x23,0x35,0x19,0xcd,0xb3,0x80,0xf5,0xab,0x8c,
	0x0f,0x02,0x72,0x8e,0x91,0xd4,0x69,0xab,0xd0,0xea,0xe0,0xd9,0x3b,0x1c,0xc6,0x6c,
	0xe1,0x27,0xb2,0x9c,0x7d,0x77,0x44,0x1a,0x49,0xd0,0x9f,0xca,0x5d,0x6d,0x97,0x62,
	0xfc,0x74,0xc3,0x1b,0xb5,0x06,0xc8,0xba,0xe3,0xc7,0x9a,0xd6,0xc2,0x57,0x87,0x75,
	0xb9,0x59,0x56,0xb5,0x37,0x0d,0x1d,0x05,0x19,0xe3,0x79,0x06,0xb3,0x84,0x73,0x62,
	0x33,0x25,0x1e,0x8f,0x09,0xad,0x79,0xdf,0xbe,0x2c,0x6a,0xbf,0xad,0xac,0x8e,0x4d,
	0x86,0x24,0x31,0x8c,0x27,0xda,0xf1},
	1,
	{0x03,0x00,0x00},
	{0x20,0x25,0x12,0x30},
	0x01,
	{0xf5,0x27,0x08,0x1c,0xf3,0x71,0xdd,0x7e,0x1f,0xd4,0xfa,0x41,0x4a,0x66,0x50,0x36,0xe0,0xf5,0xe6,0xe5}
};

PKFILESTRU capk_cup_t08 =
{
	{0xA0, 0x00, 0x00, 0x03, 0x33},
	0x08,
	144,
	{0xB6,0x16,0x45,0xED,0xFD,0x54,0x98,0xFB,0x24,0x64,0x44,0x03,0x7A,0x0F,0xA1,0x8C,0x0F,0x10,
	0x1E,0xBD,0x8E,0xFA,0x54,0x57,0x3C,0xE6,0xE6,0xA7,0xFB,0xF6,0x3E,0xD2,0x1D,0x66,0x34,0x08,
	0x52,0xB0,0x21,0x1C,0xF5,0xEE,0xF6,0xA1,0xCD,0x98,0x9F,0x66,0xAF,0x21,0xA8,0xEB,0x19,0xDB,
	0xD8,0xDB,0xC3,0x70,0x6D,0x13,0x53,0x63,0xA0,0xD6,0x83,0xD0,0x46,0x30,0x4F,0x5A,0x83,0x6B,
	0xC1,0xBC,0x63,0x28,0x21,0xAF,0xE7,0xA2,0xF7,0x5D,0xA3,0xC5,0x0A,0xC7,0x4C,0x54,0x5A,0x75,
	0x45,0x62,0x20,0x41,0x37,0x16,0x96,0x63,0xCF,0xCC,0x0B,0x06,0xE6,0x7E,0x21,0x09,0xEB,0xA4,
	0x1B,0xC6,0x7F,0xF2,0x0C,0xC8,0xAC,0x80,0xD7,0xB6,0xEE,0x1A,0x95,0x46,0x5B,0x3B,0x26,0x57,
	0x53,0x3E,0xA5,0x6D,0x92,0xD5,0x39,0xE5,0x06,0x43,0x60,0xEA,0x48,0x50,0xFE,0xD2,0xD1,0xBF},
	1,
	{0x03,0x00,0x00},
	{0x20,0x25,0x12,0x30},
	0x01,
	{0xEE,0x23,0xB6,0x16,0xC9,0x5C,0x02,0x65,0x2A,0xD1,0x88,0x60,0xE4,0x87,0x87,0xC0,0x79,0xE8,0xE8,0x5A}
};

PKFILESTRU capk_cup_t09 =
{
	{0xA0, 0x00, 0x00, 0x03, 0x33},
	0x09,
	176,
	{0xeb,0x37,0x4d,0xfc,0x5a,0x96,0xb7,0x1d,0x28,0x63,0x87,0x5e,0xda,0x2e,0xaf,0xb9,0x6b,
	0x1b,0x43,0x9d,0x3e,0xce,0x0b,0x18,0x26,0xa2,0x67,0x2e,0xee,0xfa,0x79,0x90,0x28,
	0x67,0x76,0xf8,0xbd,0x98,0x9a,0x15,0x14,0x1a,0x75,0xc3,0x84,0xdf,0xc1,0x4f,0xef,
	0x92,0x43,0xaa,0xb3,0x27,0x07,0x65,0x9b,0xe9,0xe4,0x79,0x7a,0x24,0x7c,0x2f,0x0b,
	0x6d,0x99,0x37,0x2f,0x38,0x4a,0xf6,0x2f,0xe2,0x3b,0xc5,0x4b,0xcd,0xc5,0x7a,0x9a,
	0xcd,0x1d,0x55,0x85,0xc3,0x03,0xf2,0x01,0xef,0x4e,0x8b,0x80,0x6a,0xfb,0x80,0x9d,
	0xb1,0xa3,0xdb,0x1c,0xd1,0x12,0xac,0x88,0x4f,0x16,0x4a,0x67,0xb9,0x9c,0x7d,0x6e,
	0x5a,0x8a,0x6d,0xf1,0xd3,0xca,0xe6,0xd7,0xed,0x3d,0x5b,0xe7,0x25,0xb2,0xde,0x4a,
	0xde,0x23,0xfa,0x67,0x9b,0xf4,0xeb,0x15,0xa9,0x3d,0x8a,0x6e,0x29,0xc7,0xff,0xa1,
	0xa7,0x0d,0xe2,0xe5,0x4f,0x59,0x3d,0x90,0x8a,0x3b,0xf9,0xeb,0xbd,0x76,0x0b,0xbf,
	0xdc,0x8d,0xb8,0xb5,0x44,0x97,0xe6,0xc5,0xbe,0x0e,0x4a,0x4d,0xac,0x29,0xe5},
	1,
	{0x03,0x00,0x00},
	{0x20,0x25,0x12,0x30},
	0x01,
	{0xa0,0x75,0x30,0x6e,0xab,0x00,0x45,0xba,0xf7,0x2c,0xdd,0x33,0xb3,0xb6,0x78,0x77,0x9d,0xe1,0xf5,0x27}
};

PKFILESTRU capk_cup_t0B =
{
	{0xA0, 0x00, 0x00, 0x03, 0x33},
	0x0B,
	248,
	{0xcf,0x9f,0xdf,0x46,0xb3,0x56,0x37,0x8e,0x9a,0xf3,0x11,0xb0,0xf9,0x81,0xb2,0x1a,0x1f,
	0x22,0xf2,0x50,0xfb,0x11,0xf5,0x5c,0x95,0x87,0x09,0xe3,0xc7,0x24,0x19,0x18,0x29,
	0x34,0x83,0x28,0x9e,0xae,0x68,0x8a,0x09,0x4c,0x02,0xc3,0x44,0xe2,0x99,0x9f,0x31,
	0x5a,0x72,0x84,0x1f,0x48,0x9e,0x24,0xb1,0xba,0x00,0x56,0xcf,0xab,0x3b,0x47,0x9d,
	0x0e,0x82,0x64,0x52,0x37,0x5d,0xcd,0xbb,0x67,0xe9,0x7e,0xc2,0xaa,0x66,0xf4,0x60,
	0x1d,0x77,0x4f,0xea,0xef,0x77,0x5a,0xcc,0xc6,0x21,0xbf,0xeb,0x65,0xfb,0x00,0x53,
	0xfc,0x5f,0x39,0x2a,0xa5,0xe1,0xd4,0xc4,0x1a,0x4d,0xe9,0xff,0xdf,0xdf,0x13,0x27,
	0xc4,0xbb,0x87,0x4f,0x1f,0x63,0xa5,0x99,0xee,0x39,0x02,0xfe,0x95,0xe7,0x29,0xfd,
	0x78,0xd4,0x23,0x4d,0xc7,0xe6,0xcf,0x1a,0xba,0xba,0xa3,0xf6,0xdb,0x29,0xb7,0xf0,
	0x5d,0x1d,0x90,0x1d,0x2e,0x76,0xa6,0x06,0xa8,0xcb,0xff,0xff,0xec,0xbd,0x91,0x8f,
	0xa2,0xd2,0x78,0xbd,0xb4,0x3b,0x04,0x34,0xf5,0xd4,0x51,0x34,0xbe,0x1c,0x27,0x81,
	0xd1,0x57,0xd5,0x01,0xff,0x43,0xe5,0xf1,0xc4,0x70,0x96,0x7c,0xd5,0x7c,0xe5,0x3b,
	0x64,0xd8,0x29,0x74,0xc8,0x27,0x59,0x37,0xc5,0xd8,0x50,0x2a,0x12,0x52,0xa8,0xa5,
	0xd6,0x08,0x8a,0x25,0x9b,0x69,0x4f,0x98,0x64,0x8d,0x9a,0xf2,0xcb,0x0e,0xfd,0x9d,
	0x94,0x3c,0x69,0xf8,0x96,0xd4,0x9f,0xa3,0x97,0x02,0x16,0x2a,0xcb,0x5a,0xf2,0x9b,
	0x90,0xba,0xde,0x00,0x5b,0xc1,0x57},
	1,
	{0x03,0x00,0x00},
	{0x20,0x25,0x12,0x30},
	0x01,
	{0xbd,0x33,0x1f,0x99,0x96,0xa4,0x90,0xb3,0x3c,0x13,0x44,0x10,0x66,0xa0,0x9a,0xd3,0xfe,0xb5,0xf6,0x6c}
};

void Debug_ComPrintData(int nSendLen, char *pvSendData)
{
	int i;
	char acTmpBuf[1024];

	memset(acTmpBuf, 0x00, sizeof(acTmpBuf));
	for(i = 0;i< nSendLen;i++)
		sprintf(acTmpBuf + i * 3, "%02x ", pvSendData[i]);
#if 0
	usbdserial_Close();
	usbdserial_Open();
	usbdserial_Write(acTmpBuf, nSendLen*3);
	usbdserial_Write("\r\n\r\n\r\n", 6);
#else
	TRACE_DEBUG("%s\r\n\r\n\r\n", acTmpBuf);
#endif
}

void Debug_ComPrintInt(int nSendLen, char *pvTitle, int Num)
{
#if 0
	char acTmpBuf[10];

	memset(acTmpBuf, 0x00, sizeof(acTmpBuf));
	sprintf(acTmpBuf, "=%02x", Num);
	usbdserial_Close();
	usbdserial_Open();
	usbdserial_Write(pvTitle, nSendLen);
	usbdserial_Write(acTmpBuf, 3);
	usbdserial_Write("\r\n\r\n\r\n", 6);
#else
	TRACE_DEBUG("%s = %d\r\n\r\n\r\n", pvTitle, Num);
#endif
}

void Debug_ComPrintTitle(int nTitleLen, char *pvTitle, int nSendLen, char *pvSendData)
{
//	int i, nOffset;
//	char acTmpBuf[1024];
//
//	memset(acTmpBuf, 0x00, sizeof(acTmpBuf));
//	memcpy(acTmpBuf, pvTitle, nTitleLen);
//	memcpy(acTmpBuf+nTitleLen, " : ", 3);
//	nOffset = nTitleLen+3;
//	for(i = 0;i< nSendLen;i++)
//		sprintf(acTmpBuf + nOffset + i * 3, "%02x ", pvSendData[i]);
    TRACE("\r\n%s",pvTitle);
    TRACE_BUF("EMV_TEST",(uchar *)pvSendData,nSendLen);

#if 0
	usbdserial_Close();
	usbdserial_Open();

	usbdserial_Write(acTmpBuf, nOffset+nSendLen*3);
	usbdserial_Write("\r\n\r\n\r\n", 6);
#else
	//TRACE_DEBUG("%s\r\n\r\n\r\n", acTmpBuf);
#endif
}

void Debug_ComPrintStr(int nSendLen, char *pvSendData)
{
#if 0
	usbdserial_Close();
	usbdserial_Open();
	usbdserial_Write(pvSendData, nSendLen);
	usbdserial_Write("\r\n\r\n\r\n", 6);
#else
	TRACE_DEBUG("%s\r\n\r\n\r\n", pvSendData);
#endif
}

int emvDetectInput(void)
{
	uchar TimeStr[9];
	int   nKey = 0;
	long  lRet = 0;

	mag_Open();
	mag_Clear();
	while(1)
	{
		memset(TimeStr, 0x00, sizeof(TimeStr));
		sys_GetTime(TimeStr);
		lcd_Display(0,6*8,FONT_SIZE8,"20%02x-%02x-%02x   %02x:%02x:%02x",TimeStr[0],TimeStr[1],TimeStr[2],TimeStr[3],TimeStr[4],TimeStr[5]);
		ICAdapterSelectCard(EM_ICA_USERCARD);
		if (ICAdapterCardIn()==0)
		{
			glCardType = EM_ICA_USERCARD;
			lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"���Ժ�...");
			return EMV_RET_ERROR_OK;
		}
		ICAdapterSelectCard(EM_ICA_LOCAL_RFCARD);
		lRet = ICAdapterCardIn();
		if (lRet == 0)
		{
			glCardType = EM_ICA_LOCAL_RFCARD;
			lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"���Ժ�...");
			return EMV_RET_ERROR_OK;
		}
		else if (lRet == EM_ICA_RFCARDNUMERR)
		{
			//�ǽӴ�������ʧ�ܷ��ص����½��׽���
			return KEY_CANCEL;
		}

		if(kb_Hit() == YES)
		{
			nKey = kb_GetKey(-1);
// 			if( nKey == KEY_FUNC)
// 			{
// 				return KEY_FUNC;
// 			}
// 			else if (nKey == KEY_CANCEL)
// 			{
// 				return KEY_CANCEL;
// 			}
			if (nKey == KEY_CANCEL)
			{
				return KEY_CANCEL;
			}
		}
		sys_DelayMs(100);
	}
}

#if 0

/*******************************************************
* ����:
*		MCK������ʼ��
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
int emv_init_mck_config(void)
{
	TermMCK tMck;

	memset(&tMck, 0x00, sizeof(TermMCK));
	tMck.cTmType = 0x22;
	memcpy(tMck.tTmCap, "\xE0\xE1\xC8",3);
	memcpy(tMck.tTmCapAd, "\x60\x00\xF0\xF0\x01", 5);

	tMck.cSupPSESelect = 1;
	tMck.cSupAppConfirm = 1;
	tMck.cSupAppDispOrder = 1;
	tMck.cSupMulLang = 0;
	tMck.cSupIssCertCheck = 1;
	tMck.cSupDDOLFlg = 1;

	tMck.cSupBypassPIN = 1;
	tMck.cSupFlrLmtCheck = 1;
	tMck.cSupRndOnlineSlt = 1;
	tMck.cSupVeloCheck = 1;
	tMck.cSupTransLog = 1;

	tMck.cSupExcptCheck = 1;
	tMck.cTmRmCheckFlg = 0;
	tMck.cSupTmActionCode = 1;
	tMck.cSupForceOnline = 0;
	tMck.cSupForceAccept = 0;

	tMck.cSupAdvice = 1;
	tMck.cSupIssVoiceRef = 1;
	tMck.cSupCardVoiceRef = 0;
	tMck.cSupBatchCollect = 1;
	tMck.cSupOnlineCollect = 0;

	tMck.cSupTDOLFlg = 1;
	tMck.cPOSEntryMode = 0x25;

	Hash((uchar*)&tMck,sizeof(TermMCK)-20,tMck.tHashVal);

	return emv_set_mck(&tMck);
}

/*******************************************************
* ����:
*		�ն˹̶�������ʼ��
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
int emv_init_terminal_fix_data(void)
{
	EMV_TERM_FIX_DATA tFixData;

	memset(&tFixData, 0x00, sizeof(EMV_TERM_FIX_DATA));
	memcpy(tFixData.TermID, "ABCD1234", 8);
	memcpy(tFixData.MerchantID, "123456789ABCDEF", 15);
	memcpy(tFixData.MerchCatCode, "\x00\x01", 2);
	tFixData.MCHLOCLen = 5;
	memcpy(tFixData.MCHNameLoc, "China", 5);

	memcpy(tFixData.TermCntrCode, "\x01\x56", 2);
	memcpy(tFixData.TransCurCode, "\x01\x56", 2);
	memcpy(tFixData.ReferCurCode, "\x01\x56", 2);
	tFixData.TransCurExp = 0x02;
	tFixData.ReferCurExp = 0x02;
	tFixData.ReferCurrCon = 1000;

	return emv_set_terminal_data(&tFixData);
}

/*******************************************************
* ����:
*		Ӧ�ò�����ʼ��
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
int emv_init_app_data(void)
{
	int Ret;
	EMV_APP_INIT_DATA tAppInitData;

	memset(&tAppInitData, 0x00, sizeof(EMV_APP_INIT_DATA));
	tAppInitData.TargetPer    = 0;           	// Ŀ��ٷֱ���
	tAppInitData.MaxTargetPer = 0;        	// ���Ŀ��ٷֱ���
	tAppInitData.FloorLimit   = 10000;          		// ����޶�
	tAppInitData.Threshold    = 100;           	// ��ֵ
	tAppInitData.ECTransLimit = 100000;           // �ն˵����ֽ����޶�
	tAppInitData.QPBOCTransLimit = 20000;        // �ǽӴ������޶�
	tAppInitData.QPBOCFloorLimit = 15000;        // �ǽӴ��ѻ�����޶�
	tAppInitData.QPBOCCVMLimit   = 10000;          // ִ��CVM�޶�
	memset(tAppInitData.TACDenial, 0x00, 5);        	// �ն���Ϊ����(�ܾ�)
	memset(tAppInitData.TACOnline, 0x00, 5);        	// �ն���Ϊ����(����)
	memset(tAppInitData.TACDefault, 0x00, 5);       	// �ն���Ϊ����(ȱʡ)
	memcpy(tAppInitData.AcquierId, "\x01\x22\x33\x44\x55\x66", 6);        	// �յ��б�־
	tAppInitData.LenOfdDOL = 3;           	// �ն�ȱʡDDOL����
	memcpy(tAppInitData.dDOL, "\x9F\x37\x04", 3);           	// �ն�ȱʡDDOL
	tAppInitData.LenOftDOL = 15;           	// �ն�ȱʡTDOL����
	memcpy(tAppInitData.tDOL, "\x9F\x02\x06\x5F\x2A\x02\x9A\x03\x9C\x01\x95\x05\x9F\x37\x04", 15);           	// �ն�ȱʡTDOL
	memcpy(tAppInitData.Version, "\x00\x8C", 2);          		// Ӧ�ð汾
	tAppInitData.RiskManDataLen = 0;      	// ���չ������ݳ���
	memset(tAppInitData.RiskManData, 0x00, 8);      	// ���չ�������

	Ret = emv_set_app_init_data(&tAppInitData);
	return Ret;
}
#endif

//����Ĭ�ϵ�IC����
int emvSetDefaultPara(void)
{
	int    i;
	long   lTmp;
	EMVPARA ParaInfo;

	//����Ĭ��ֵ
	for (i = 0; i < 2; i++)
	{
		memset((char *)&ParaInfo, 0, sizeof(EMVPARA));
		ParaInfo.cLenAID = 7;
		if (i == 0)
		{
			memcpy(ParaInfo.AID, "\xA0\x00\x00\x03\x33\x01\x01", 7);
		}
		else
		{
			memcpy(ParaInfo.AID, "\xA0\x00\x00\x00\x03\x10\x10", 7);
		}
		ParaInfo.cFlag = 0;
		memcpy(ParaInfo.tTAC_Default, "\xD8\x60\x04\xA8\x00", 5);
		memcpy(ParaInfo.tTAC_Online, "\xD8\x68\x04\xF8\x00", 5);
		memcpy(ParaInfo.tTAC_Denial, "\x00\x10\x98\x00\x00", 5);
		lTmp = 10000;
		sprintf((char *)ParaInfo.tFloorLmt, "%010ld", lTmp);
		lTmp = 0;
		sprintf((char *)ParaInfo.tLmt, "%010ld", lTmp);
		memcpy(ParaInfo.tMaxPercent, "\x30\x30", 2);
		memcpy(ParaInfo.tPercent, "\x30\x30", 2);
		ParaInfo.tOnline = 1;
		ParaInfo.cLenDDOL = 3;
		memcpy(ParaInfo.tDDOL, "\x9F\x37\x04", ParaInfo.cLenDDOL);
		memcpy(ParaInfo.tAppVerNo, "\x00\x20", 2);
		lTmp = 100000;
		sprintf((char *)ParaInfo.tECTransLimit, "%012ld", lTmp);
		sprintf((char *)ParaInfo.tQPBOCLimit, "%012ld", lTmp);
		sprintf((char *)ParaInfo.tQPBOCFloorLimit, "%012ld", lTmp);
		sprintf((char *)ParaInfo.tCVMLimit, "%012ld", lTmp);
		ST_EMV_SetICPara(&ParaInfo);
	}

	return 0;
}

int emv_init_callback_func(void)
{
	int Ret = 0;
	EMV_CALLBACK_FUNC tFunc;

	tFunc.emv_select_candidate_application = emv_select_candidate_application;
	tFunc.emv_account_type_select = emv_account_type_select;
	tFunc.emv_get_transaction_amount = emv_get_transaction_amount;
	tFunc.emv_get_cardholder_pin = emv_get_cardholder_pin;
	tFunc.emv_show_pin_verify_result = emv_show_pin_verify_result;
	tFunc.emv_process_online_pin = emv_process_online_pin;
	tFunc.emv_process_pboc_certificate = emv_process_pboc_certificate;
	tFunc.emv_process_advice = emv_process_advice;
	tFunc.emv_process_online = emv_process_online;
	tFunc.emv_process_issuer_refer = emv_process_issuer_refer;
	tFunc.emv_set_transaction_data = emv_set_transaction_data;
	tFunc.emv_ec_account_select = emv_ec_account_select;

	Ret = emv_set_callback_function(&tFunc);
	return Ret;
}

/*******************************************************
* ����:
*		EMV������ʼ��
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*       EMV_RET_PARA_ERROR              �����������
*********************************************************/
unsigned char ST_EMV_ParaInit(void)
{
	unsigned char  ucRet;
	TmFixData tFixData;
	AppInitData tInitData;

	memset(&tFixData, 0x00, sizeof(TmFixData));
	memcpy(tFixData.tTmCntrCode, "\x01\x56", 2);
	memcpy(tFixData.tAquirerID, "\x01\x22\x33\x44\x55\x66", 6);
	memcpy(tFixData.tMerchCatCode, "\x00\x01", 2);
	memcpy(tFixData.tTmTransCur, "\x01\x56", 2);
	tFixData.cTmTransCurExp = 0x02;
	memcpy(tFixData.tTmRefCurCode, "\x01\x56", 2);
	tFixData.tTmRefCurExp = 0x02;
	memcpy(tFixData.tTmRefCurConv, "1000", 4);
	memcpy(tFixData.tTmID, "ABCD1234", 8);
	memcpy(tFixData.tMerchantID, "123456789ABCDEF", 15);
	memcpy(tFixData.tMCHNAMELOC, "China", 5);
	tFixData.MCHLOCLen = 5;
	tFixData.tTmTransCateCode = 0x22;
	ucRet = ST_EMV_InitTmFixData(&tFixData);
	if (ucRet)
	{
		return EMV_RET_PARA_ERROR;
	}

	memset(&tInitData, 0x00, sizeof(AppInitData));
	memcpy(tInitData.tAppVerNo, "\x00\x8C", 2);
	// �ն���Ϊ����
	memset(tInitData.tTAC_Denial, 0x00, 5);
	memset(tInitData.tTAC_Online, 0x00, 5);
	memset(tInitData.tTAC_Default, 0x00, 5);
	tInitData.cLenTDOL = 15;           	// �ն�ȱʡTDOL����
	memcpy(tInitData.tTDOL, "\x9F\x02\x06\x5F\x2A\x02\x9A\x03\x9C\x01\x95\x05\x9F\x37\x04", 15);           	// �ն�ȱʡTDOL
	tInitData.cLenDDOL = 3;           	// �ն�ȱʡDDOL����
	memcpy(tInitData.tDDOL, "\x9F\x37\x04", 3);           	// �ն�ȱʡDDOL
	sprintf((char *)tInitData.tFloorLmt, "%010d", 10000);
	sprintf((char *)tInitData.tLmt, "%010d", 100);
	sprintf((char *)tInitData.tPercent, "%02d", 0);
	sprintf((char *)tInitData.tMaxPercent, "%02d", 0);
	sprintf((char *)tInitData.tECTransLimit, "%012d", 100000);
	sprintf((char *)tInitData.tQPBOCTransLimit, "%012d", 20000);
	sprintf((char *)tInitData.tQPBOCFloorLimit, "%012d", 15000);
	sprintf((char *)tInitData.tQPBOCCVMLimit, "%012d", 10000);
	ucRet = ST_EMV_InitiateEMV(&tInitData);
	if (ucRet)
	{
		return EMV_RET_PARA_ERROR;
	}

	return EMV_RET_OK;
}

/*******************************************************
* ����:
*		����EMV��IC������
* ����:
*		*pParaStru   ----- IC������
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*       EMV_RET_PARA_ERROR              �����������
*       EMV_RET_OVER_NUM                ���������õ�������������10��
*********************************************************/
unsigned char ST_EMV_SetICPara(EMVPARA *pParaStru)
{
	Debug_ComPrintTitle(7, "EMVPARA", sizeof(EMVPARA), (char *)pParaStru);

	if(pParaStru == NULL)
	{
		return EMV_RET_PARA_ERROR;
	}

	if(gn_emv_paralist_num >= EMV_MAX_PARALIST_NUM)
	{
		return EMV_RET_OVER_NUM;
	}
	memcpy(&gt_emv_paralist[gn_emv_paralist_num], pParaStru, sizeof(EMVPARA));
	gn_emv_paralist_num++;

	//����AID���ں�
	if (pParaStru->cFlag == 0x01)
	{
		ST_EMV_SetAidListItem(pParaStru->AID, pParaStru->cLenAID, 0);
	}
	else
	{
		ST_EMV_SetAidListItem(pParaStru->AID, pParaStru->cLenAID, 1);
	}
	return EMV_RET_OK;
}

/*******************************************************
* ����:
*		���EMV��IC������
* ����:
*		��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
unsigned char ST_EMV_DelAllICPara(void)
{
	Debug_ComPrintData(strlen("DelAllICPara"), "DelAllICPara");

	memset(gt_emv_paralist, 0x00, sizeof(gt_emv_paralist));
	gn_emv_paralist_num = 0;

	//���AID�б�
	emv_init_aidlist();
	return EMV_RET_OK;
}

/*******************************************************
* ����:
*		��ȡƥ���IC������
* ����:
*       pAid         ----- AID
*       nAidLen      ----- AID����
* ���:
*		*pParaStru   ----- IC������
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*       EMV_RET_PARA_ERROR              �����������
*       EMV_RET_ERROR                   �Ҳ���ƥ��Ĳ���
*********************************************************/
unsigned char ST_EMV_GetICPara(unsigned char *pAid, int nAidLen, EMVPARA *pParaStru)
{
	int i;

	if(pAid == NULL || pParaStru == NULL)
	{
		return EMV_RET_PARA_ERROR;
	}

	for (i = 0; i < gn_emv_paralist_num; i++)
	{
		if (memcmp(gt_emv_paralist[i].AID, pAid, gt_emv_paralist[i].cLenAID) == 0)
		{
			if ((gt_emv_paralist[i].cFlag == 1) && (gt_emv_paralist[i].cLenAID != nAidLen))
			{
				//ȫ������ƥ��
				continue;
			}
			else
			{
				break;
			}
		}
	}
	if (i == gn_emv_paralist_num)
	{
		return EMV_RET_ERROR;
	}

	memcpy(pParaStru, &gt_emv_paralist[i], sizeof(EMVPARA));
	return EMV_RET_OK;
}

/*******************************************************
* ����:
*		��EMV�ں�
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
unsigned char ST_EMV_OpenEMVKernel(void)
{
	//��ʼ��EMV�ں˵����ݴ洢�ռ�
	ST_EMV_Init();

	memset(gt_emv_paralist, 0x00, sizeof(gt_emv_paralist));
	gn_emv_paralist_num = 0;

	//����ģ���ʼ��
	EMV_CardModule_init();

	emv_init_callback_func();

	guc_OpenEMVFlag = 1;

	return EMV_RET_OK;
}

/*******************************************************
* ����:
*		�ر�EMV�ں�
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
unsigned char ST_EMV_CloseEMVKernel(void)
{
	guc_OpenEMVFlag = 0;
	return EMV_RET_OK;
}

/*******************************************************
* ����:
*		����Ĭ�ϲ��������ڲ��ԣ�ʵ�ʲ����������˴���
* ����:
*       ��
* ���:
*       ��
* ����ֵ:
*		EMV_RET_OK				        �ɹ�
*********************************************************/
void SetTestPara(void)
{
// 	uchar ucTmp[1024];
// 	int  nLen;
// 	PKFILESTRU tPk;

	//������ʼ�����˴���ʼ��ֻ�Ǹ���������ֵ�����ڲ���
	//ʵ�ʵ�ֵ����������˴���
	ST_EMV_ParaInit();

	//���빫Կ
	ST_EMV_SetCAPKey(&capk_cup_t02);
	ST_EMV_SetCAPKey(&capk_cup_t03);
	ST_EMV_SetCAPKey(&capk_cup_t04);
	ST_EMV_SetCAPKey(&capk_cup_t08);
	ST_EMV_SetCAPKey(&capk_cup_t09);
	ST_EMV_SetCAPKey(&capk_cup_t0B);

// 	memset(ucTmp, 0x00, sizeof(ucTmp));
// 	sprintf((char *)ucTmp, "%s", "A00000033303B1B0B0627DEE87864F9C18C13B9A1F025448BF13C58380C91F4CEBA9F9BCB214FF8414E9B59D6ABA10F941C7331768F47B2127907D857FA39AAF8CE02045DD01619D689EE731C551159BE7EB2D51A372FF56B556E5CB2FDE36E23073A44CA215D6C26CA68847B388E39520E0026E62294B557D6470440CA0AEFC9438C923AEC9B2098D6D3A1AF5E8B1DE36F4B53040109D89B77CAFAF70C26C601ABDF59EEC0FDC8A99089140CD2E817E335175B03B7AA33D000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001030000323032340187F0CD7C0E86F38F89A66F8C47071A8B88586F26000000000000000000000000");
// 	nLen = strlen((char *)ucTmp);
// 	memset(&tPk, 0x00, sizeof(PKFILESTRU));
// 	AsciiToHex((uchar *)&tPk, ucTmp, nLen, 0);
// 	tPk.cHashFlg = 0;
// 	ST_EMV_SetCAPKey(&tPk);
// 
// 	memset(ucTmp, 0x00, sizeof(ucTmp));
// 	emv_get_transerr_log((char *)ucTmp, &nLen);
// 	lcd_Display(0,2*8,DISP_CLRLINE|FONT_SIZE12|DISP_MEDIACY,"Err: %s", ucTmp);
// 	kb_GetKey(-1);

	//���ó�ʼ����
	emvSetDefaultPara();
	//end
}

#if 0
//main������һ�����Ժ��������ڲ���IC���������̣������˲��������������EMV�ӿ�
int main (void)
{
	int Ret = 0, nLen;
	unsigned short nCardLen;
	uchar ucBuff[256], ucRet, ucCard[20];
	TransProDt tTransData;
	AppDt tAppData;
	TransDt  tOutData;
	PINSTATUS tPinInfo;

	sys_init();
	if (sys_FirstRunExp() == YES)
	{
		lcd_Cls();
		lcd_Display(0,0,DISP_CLRLINE|FONT_SIZE16|DISP_MEDIACY,"EMV INIT...");
		sys_WriteSn("12345678");

		sys_FirstRun();
	}

	//EMV���̴����￪ʼ
	ST_EMV_OpenEMVKernel(); //ÿ�ο����ϵ綼Ҫ����һ��open

	//����Ĭ�ϲ��������ڲ��ԣ�ʵ�ʲ����������˴���
	SetTestPara();

	while(1)
	{
		//����Ѱ��Ҫ����������ɣ�Ѱ���ɹ������EMV�ӿ�
		lcd_Cls();
		lcd_Display(0,2*8,FONT_SIZE16|DISP_MEDIACY|DISP_CLRLINE,"��忨");
		Ret = emvDetectInput();
		if(Ret == EMV_RET_ERROR_OK)
		{
			//Ѱ���ɹ�
			//�������ݸ�ֵ���ڲ��ԣ���ʵ���ݴ���ڲ�������
			memset(&tTransData, 0x00, sizeof(TransProDt));
			if (glCardType == EM_ICA_USERCARD)
			{
				tTransData.ucCardType = EMV_USERCARD;
				tTransData.ucProcessFlag = 1;
			}
			else
			{
				tTransData.ucCardType = EMV_RFCARD;
				tTransData.ucProcessFlag = 3;
			}
			tTransData.ucCmd = 0;

			memset(&tAppData, 0x00, sizeof(AppDt));
			sprintf((char *)tAppData.AmntAuth, "%012d", 1);
			tAppData.cTransType = 0x00;
			sys_GetTime(ucBuff);
			memcpy(tAppData.tDate, ucBuff, 3);
			memcpy(tAppData.tTime, ucBuff+3, 3);
			memcpy(tAppData.tTrace, "\x00\x00\x00\x01", 4);
			//�������ݸ�ֵ���

			//�����˿�ʼ����EMV�ӿڣ���ʵ��������ڲ�������
			//BeginTransaction�������Ӧ��ѡ�񣬶�IC���ϵ�Ӧ�����ݵĹ���
			ucRet = ST_EMV_BeginTransaction(&tTransData, &tAppData);
			Debug_ComPrintTitle(strlen("ST_EMV_BeginTransaction"), "ST_EMV_BeginTransaction", 1, (char *)&ucRet);
			if (ucRet)
			{
				memset(ucBuff, 0x00, sizeof(ucBuff));
				emv_get_transerr_log((char *)ucBuff, &nLen);
				lcd_Display(0,2*8,DISP_CLRLINE|FONT_SIZE12|DISP_MEDIACY,"Err: %s", ucBuff);
				kb_GetKey(-1);
				continue;
			}

#if 1
			//BeginTransaction��ɺ󣬾Ϳ��Ի�ȡ���ţ����ŵ����ݣ��ֿ��������ȵ�IC���ϵ�����
			memset(ucCard, 0x00, sizeof(ucCard));
			memset(ucBuff, 0x00, sizeof(ucBuff));
			//��ȡ���ţ�IC�����ݶ���TLV��ʽ�����ô˺�����ȡValue�����ŵ�Tag��0x5A
			ST_EMV_GetAppData((uchar *)"\x5A", 1, ucCard, &nCardLen); 
			//��ȡ�Ŀ�����BCD��ʽ����Ҫת����ASCII��ʽ����������ʾ���ӡ
			HexToAscii(ucBuff, ucCard, nCardLen*2, 0);
			lcd_Display(0, 2*8, DISP_MEDIACY|DISP_CLRLINE|FONT_SIZE12, "����");
			lcd_Display(0, 4*8, DISP_CLRLINE|FONT_SIZE12, "%s", ucBuff);
			kb_GetKey(-1);
			//��ʾ���
#endif

			//����������ContinueTransaction����������ѻ�������֤���������ƣ��ֿ�����֤���ն˷��չ����ն���Ϊ�����Ϳ�Ƭ��Ϊ����
			memset(&tPinInfo, 0x00, sizeof(PINSTATUS));
			memcpy(tPinInfo.acCardNo, "482310541906", 12);
			ucRet = ST_EMV_ContinueTransaction(&tPinInfo);
			Debug_ComPrintTitle(strlen("ST_EMV_ContinueTransaction"), "ST_EMV_ContinueTransaction", 1, (char *)&ucRet);
			if (ucRet != 0x00 && ucRet != 0x02 && ucRet != 0x03)
			{
				memset(ucBuff, 0x00, sizeof(ucBuff));
				emv_get_transerr_log((char *)ucBuff, &nLen);
				lcd_Display(0,2*8,DISP_CLRLINE|FONT_SIZE12|DISP_MEDIACY,"Err: %s", ucBuff);
				kb_GetKey(-1);
				continue;
			}
			Debug_ComPrintData(sizeof(PINSTATUS), (char *)&tPinInfo);
			//������������ף�ContinueTransaction��ɺ�������Ҫ��ContinueTransaction�ĳ��ڲ����������������ݣ�����������к�̨��

			//���к�̨����Ӧ��������˰�Ӧ������ͨ���������µ�OnlineDataProcess��������EMV�ںˣ��������ں��жϽ��׽�������ս��ͨ�����ڲ�������
			memset(ucBuff, 0x00, sizeof(ucBuff));
			memcpy(ucBuff, "\x00\x00\x8A\x02\x30\x30\x89\x06\x30\x30\x30\x31\x31\x31", 14);
			memset(&tOutData, 0x00, sizeof(TransDt));
			ucRet = ST_EMV_OnlineDataProcess(ucBuff, 14, &tOutData);
			Debug_ComPrintTitle(strlen("ST_EMV_OnlineDataProcess"), "ST_EMV_OnlineDataProcess", 1, (char *)&ucRet);
			if (ucRet)
			{
				memset(ucBuff, 0x00, sizeof(ucBuff));
				emv_get_transerr_log((char *)ucBuff, &nLen);
				lcd_Display(0,2*8,DISP_CLRLINE|FONT_SIZE12|DISP_MEDIACY,"Err: %s", ucBuff);
				kb_GetKey(-1);
				continue;
			}
			Debug_ComPrintData(sizeof(TransDt), (char *)&tOutData);
			lcd_Display(0,2*8,DISP_CLRLINE|FONT_SIZE12|DISP_MEDIACY,"���׳ɹ�");
			kb_GetKey(-1);
			//���ں��жϽ��׳ɹ�����������ͨ�����ڲ�����ȡ�������ݣ���ӡƾ֤���洢�������ݣ���ɽ��ס�
			//���ں��жϽ���ʧ�ܣ�����������ʾ������ʾ��Ϣ����ɽ���
		}

// 		else if(Ret == 2)
// 		{
// 			Ret = emvTermConfigMenu();
// 		}
//
// 		else if(Ret == KEY_FUNC)
// 		{
// 			Ret = emvTermSetMenu();
// 		}
	}
}
#endif
