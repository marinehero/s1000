/***************** (C) COPYRIGHT 2014 START Computer equipment *****************
 * File Name          : prototype.c
 * bfief              : 
 * Author             : luocs()  
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 10/10/2014 4:12:06 PM
 * Description        : 
 *******************************************************************************/
#include "wp30_ctrl.h"
#include "prototype.h"

#if defined(CFG_DBG_PROTOTYPE)
/* 
-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
*/

#include <stdarg.h>
#define EM_ASC_UNIT_LEN                 (uchar)9
#define EM_HZ_UNIT_LEN                  (uchar)18
//---------------------------------------
const uint8_t gcBmp_union[] = 
{
    //����ȡģ
    /*--  ������һ��ͼ��E:\VSS\freescale\WP-30\Docs\����.bmp  --*/
    /*--  ���x�߶�=56x29  --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=56x32  --*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xFC,0xFE,0xFE,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFE,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xFC,
    0xFF,0xF7,0xCB,0x08,0x0C,0xCD,0xCD,0xFD,0xFF,0x00,0x00,0xA4,0xA4,0xA4,0x80,0x80,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0x01,0xCD,0xCD,0x01,0x01,0xFD,0x72,0x70,0x01,0x03,
    0x01,0x70,0x73,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x03,0x00,0x00,0x00,0x00,
    0x00,0x00,0xE0,0xF8,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x80,0x80,0x9C,0xDC,0xFF,
    0xFF,0x80,0x80,0xBF,0xE1,0xC9,0x99,0x99,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xE0,0xEC,
    0xEC,0x80,0x80,0xEF,0xBE,0xC6,0xE0,0xF8,0xE0,0xC6,0x9E,0xFF,0xFF,0xFF,0x3F,0x0F,
    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x7F,0x7F,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const uint8_t gcBmp_Company[] = 
{
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
//0x08,0x08,0x89,0xCE,0x28,0x18,0x02,0x7A,0x4A,0x4A,0x4A,0x4A,0x4A,0x7A,0x02,0x00,
//0x02,0x01,0x00,0xFF,0x01,0x02,0xFF,0x49,0x49,0x49,0x7F,0x49,0x49,0x49,0xFF,0x00,
///*--  ����:  ��  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
//0x00,0x42,0x62,0x5A,0xC6,0x10,0x54,0x54,0x54,0xFF,0x54,0x54,0x7C,0x10,0x10,0x00,
//0x40,0x23,0x14,0x08,0x17,0x28,0x29,0x49,0x49,0x7F,0x49,0x49,0x49,0x49,0x40,0x00,
/*--  ����:  ʵ  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x10,0x0C,0x04,0x4C,0xB4,0x94,0x05,0xF6,0x04,0x04,0x04,0x14,0x0C,0x04,0x00,
0x00,0x82,0x82,0x42,0x42,0x23,0x12,0x0A,0x07,0x0A,0x12,0xE2,0x42,0x02,0x02,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x82,0x84,0x0C,0x10,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
0x40,0x20,0x1F,0x20,0x50,0x48,0x44,0x43,0x40,0x41,0x42,0x44,0x5C,0x48,0x20,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xF8,0x48,0x48,0x48,0x48,0xFF,0x48,0x48,0x48,0x48,0xF8,0x00,0x00,0x00,
0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x3F,0x44,0x44,0x44,0x44,0x4F,0x40,0x70,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0xFE,0x12,0x12,0xFE,0x00,0xC8,0x08,0x49,0x8A,0x8C,0x68,0x08,0xC8,0x08,0x00,
0x60,0x1F,0x21,0x41,0x3F,0x00,0x7F,0x30,0x2C,0x23,0x23,0x2C,0x20,0x7F,0x00,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x41,0xCE,0x04,0x00,0x80,0x40,0xBE,0x82,0x82,0x82,0xBE,0xC0,0x40,0x40,0x00,
0x00,0x00,0x7F,0x20,0x90,0x80,0x40,0x43,0x2C,0x10,0x10,0x2C,0x43,0xC0,0x40,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x20,0x10,0x08,0x87,0x8A,0x52,0x22,0x22,0x52,0x8E,0x82,0x00,0x00,0x00,0x00,
0x02,0x02,0x01,0xFF,0x4A,0x4A,0x4A,0x7E,0x4A,0x4A,0x4A,0xFF,0x01,0x03,0x01,0x00,
///*--  ����:  ��  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
//0x00,0x04,0x84,0x44,0xE4,0x34,0x2C,0x27,0x24,0x24,0x24,0xE4,0x04,0x04,0x04,0x00,
//0x02,0x01,0x00,0x00,0xFF,0x09,0x09,0x09,0x29,0x49,0xC9,0x7F,0x00,0x00,0x00,0x00,
///*--  ����:  ��  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
//0xFE,0x02,0x32,0x4E,0x82,0x00,0xFE,0x4A,0xCA,0x4A,0x4A,0x4A,0x7E,0x00,0x00,0x00,
//0xFF,0x00,0x02,0x04,0x03,0x00,0xFF,0x40,0x20,0x03,0x0C,0x12,0x21,0x60,0x20,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x80,0x40,0x30,0x0E,0x84,0x00,0x00,0x0E,0x10,0x60,0xC0,0x80,0x80,0x00,
0x00,0x01,0x20,0x70,0x28,0x24,0x23,0x31,0x10,0x10,0x14,0x78,0x30,0x01,0x00,0x00,
/*--  ����:  ˾  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x10,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x12,0x02,0x02,0xFE,0x00,0x00,
0x00,0x00,0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x20,0x40,0x3F,0x00,0x00,
};

const uint8_t gcCompnayEnglish[] =
{
/*--  ����:  S  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,
/*--  ����:  t  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
/*--  ����:  a  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,
/*--  ����:  r  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,
/*--  ����:  t  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
///*--  ����:  C  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,
///*--  ����:  o  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
///*--  ����:  m  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,
///*--  ����:  p  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,
///*--  ����:  u  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,
///*--  ����:  t  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
///*--  ����:  e  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
///*--  ����:  r  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,
///*--  ����:  E  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,
///*--  ����:  q  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,
///*--  ����:  u  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,
///*--  ����:  i  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
///*--  ����:  p  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,
///*--  ����:  m  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,
///*--  ����:  e  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
///*--  ����:  n  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,
///*--  ����:  t  --*/
///*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
//0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
/*--  ����:  C  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,
/*--  ����:  o  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
/*--  ����:  .  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,
/*--  ����:  ,  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,
/*--  ����:  L  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,
/*--  ����:  t  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
/*--  ����:  d  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,
};

const uint8_t gcWelcomeHZ[] =
{
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x14,0x24,0x44,0x84,0x64,0x1C,0x20,0x18,0x0F,0xE8,0x08,0x08,0x28,0x18,0x08,0x00,
0x20,0x10,0x4C,0x43,0x43,0x2C,0x20,0x10,0x0C,0x03,0x06,0x18,0x30,0x60,0x20,0x00,
/*--  ����:  ӭ  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x41,0xCE,0x04,0x00,0xFC,0x04,0x02,0x02,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x40,0x20,0x1F,0x20,0x40,0x47,0x42,0x41,0x40,0x5F,0x40,0x42,0x44,0x43,0x40,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x40,0x42,0x44,0x5C,0xC8,0x40,0x7F,0x40,0xC0,0x50,0x4E,0x44,0x60,0x40,0x00,
0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,
/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0xF8,0x00,0x00,0xFE,0x40,0x30,0x8F,0x0A,0x08,0x18,0x68,0x08,0x88,0x08,0x00,
0x00,0x1F,0x00,0x00,0x7F,0x00,0x00,0x7F,0x21,0x21,0x3F,0x21,0x21,0x7F,0x01,0x00,
};
const uint8_t gcWelcomeEng[] = 
{
/*--  ����:  W  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,
/*--  ����:  e  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
/*--  ����:  l  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  ����:  c  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,
/*--  ����:  o  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
/*--  ����:  m  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,
/*--  ����:  e  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
/*--  ����:  !  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,
};

uint8_t proto_lcd_valid(void)
{
    return gLcdSys.SysOperate.bit.enable;
}



void proto_welcome(void)
{
    uint16_t i,x,y;
    uint16_t length;

    if (proto_lcd_valid() == 0) {
        return;
    }

    drv_lcd_cls();
    x = 32;
    y = 16;
    for (i=0; i<4*32; i+=32)
    {
        drv_lcd_FillVertMatrix1(x,y,16,16,gcWelcomeHZ+i);
        x += 16;
        if (x > 128-16)
        {
            x = 0;
            y += 16;
        }
        if (y > 64-16)
        {
            y = 0;
            break;
        }
    }
    x = 32;
    y = 16+16;
    length = sizeof(gcWelcomeEng);
    for (i=0; i<length; i+=16)
    {
        drv_lcd_FillVertMatrix1(x,y,8,16,gcWelcomeEng+i);
        x += 8;
        if (x > 128-8)
        {
            x = 0;
            y += 16;
        }
        if (y > 64-16)
        {
            y = 0;
            break;
        }
    }
    if (power_ifcharging() == 1)  //charging
    {
        if (power_charge_ifFull())
        {
            lcd_display(4,48,FONT_SIZE12|DISP_MEDIACY,"BatteryStatusFull!");
        } else
        {
            lcd_display(4,48,FONT_SIZE12|DISP_MEDIACY,"BatteryCharging:%3d%%",sys_get_batter_percentum());
        }
    } else
    {
        lcd_display(4,48,FONT_SIZE12|DISP_MEDIACY,"BatteryCharging:%3d%%",sys_get_batter_percentum());
    }
        drv_lcd_update();
}


void proto_menu(void)
{
    uint16_t i,x,y;
//    uint16_t length;

    if (proto_lcd_valid() == 0) {
        return;
    }
    drv_lcd_cls();
    drv_lcd_FillVertMatrix1(4,2,56,32,gcBmp_union);
//    length = sizeof(gcBmp_Company);
    x = 64-4;
    y = 0;
    for (i=0; i<4*32; i+=32) //ʵ�����
    {
        drv_lcd_FillVertMatrix1(x,y,16,16,gcBmp_Company+i);
        x += 16;
        if (x > 128-16)
        {
            x = 0;
            y += 16;
        }
        if (y > 64-16)
        {
            y = 0;
            break;
        }
    }
    x = 64-4;
    y = 16;
    for (i=0; i<4*32; i+=32) //�豸��˾
    {
        drv_lcd_FillVertMatrix1(x,y,16,16,gcBmp_Company+4*32+i);
        x += 16;
        if (x > 128-16)
        {
            x = 0;
            y += 16;
        }
        if (y > 64-16)
        {
            y = 0;
            break;
        }
    }
    lcd_display(4,32,FONT_SIZE12|DISP_MEDIACY,"StartComputerCo.,Ltd");
}



void proto_time_set(void)
{
    unsigned long second=0;
    struct rtc_time tm;

    memset(&tm,0x00,sizeof(tm));
    tm.tm_year = kb_InCount(0,0,"year")-1900;
    TRACE("\n-|year:%d",tm.tm_year);
    tm.tm_mon = kb_InCount(0,12,"month")-1;
    TRACE("\n-|month:%d",tm.tm_mon);
    tm.tm_mday = kb_InCount(64,12,"day");
    TRACE("\n-|day:%d",tm.tm_mday);
    tm.tm_hour= kb_InCount(0,12*2,"hour");
    TRACE("\n-|hour:%d",tm.tm_hour);
    tm.tm_min= kb_InCount(64,12*2,"min");
    TRACE("\n-|minute:%d",tm.tm_min);
    tm.tm_sec= kb_InCount(0,12*3,"sencond");
    TRACE("\n-|second:%d",tm.tm_sec);
    if (rtc_valid_tm(&tm)) {
        TRACE("\nerror");
    } else {
        rtc_tm_to_time(&tm,&second);
        hw_rtc_init(second,0);
    }
}



void proto_time(void)
{
    unsigned long second=0;
    struct rtc_time tm;

    if (proto_lcd_valid() == 0) {
        return;
    }

    if (hw_rtc_read_second((uint32_t *)&second)) {
        TRACE("\nRead second reg error!");
        return;
    }
    rtc_time_to_tm(second,&tm);
    lcd_display(4,32+16,FONT_SIZE12|DISP_MEDIACY,"%04d-%02d-%02d %02d:%02d:%02d",tm.tm_year+1900,
                      tm.tm_mon+1, tm.tm_mday,tm.tm_hour,tm.tm_min, tm.tm_sec);
}

//extern uint16_t trk2_n,trk3_n,trk_n;
void proto_magcard(void)
{
#ifdef CFG_MAGCARD
    uint32_t tk1_succ=0,tk2_succ=0,tk_total=0;
    int iRet,scale=2;
    char     Track[3][128];
    drv_lcd_cls();
    mag_open();
    lcd_display(0,0,FONT_SIZE12|DISP_MEDIACY,"    Magcard Test     ");
    while (1)
    {
        if ( mag_check(0) == EM_SUCCESS) {
            memset(Track,0x00,sizeof(Track));
            TRACE("\nhave data");
            gLcdSys.LcdBackLightTimerCnt10ms = LCD_BACKLIGHT_TIMERCNT/10;
            drv_lcd_setbacklight(LCD_MODE_TIMEROUT);
            drv_lcd_cls();
            lcd_display(0,0,FONT_SIZE12|DISP_MEDIACY,"Magcard Test");
            beep_powerswitch_nonblock(BEEP_PWM_TIMERCNT_POWER/2);
            iRet = mag_magtek_read(Track[0],Track[1],Track[2]);
            tk_total++;
            if ((uint8_t)(iRet>>16) > 0)  
            {
                tk1_succ++;
            }
            if ((uint8_t)(iRet>>24) > 0)  
            {
                tk2_succ++;
            }
            TRACE_BUF("TR2",(uint8_t *)(Track[1]+0),128);
            TRACE_BUF("TR3",(uint8_t *)(Track[2]+0),128);
            TRACE("\n-|TR2:%s",Track[1]+0);
            TRACE("\n-|TR3:%s",Track[2]+0);
//            lcd_display(0,FONT_SIZE12,FONT_SIZE12|DISP_MEDIACY,"TK2:%s",&Track[1][0]);
//            if (strlen(Track[1]+0) > 21-4)
//            {
//                lcd_display(0,FONT_SIZE12*2,FONT_SIZE12|DISP_MEDIACY,"%s",Track[1]+21-4);
//            }
//            lcd_display(0,FONT_SIZE12*3,FONT_SIZE12|DISP_MEDIACY,"TK3:%s",&Track[2][0]);
//            if (strlen(Track[2]+0) > 21-4)
//            {
//                lcd_display(0,FONT_SIZE12*4,FONT_SIZE12|DISP_MEDIACY,"%s",Track[2]+21-4);
//            }
            lcd_display(0,FONT_SIZE12,FONT_SIZE12|DISP_MEDIACY,"TK2:%s",&Track[1][0]);
            lcd_display(0,FONT_SIZE12*2,FONT_SIZE12|DISP_MEDIACY,"TK3:%s",&Track[2][0]);
            lcd_display(0,FONT_SIZE12*3,FONT_SIZE12|DISP_MEDIACY,"T2|S:%02d F:%02d %.*lf%%",tk1_succ,tk_total-tk1_succ,scale,(tk1_succ*100.0)/tk_total);
            lcd_display(0,FONT_SIZE12*4,FONT_SIZE12|DISP_MEDIACY,"T3|S:%02d F:%02d %.*lf%%",tk2_succ,tk_total-tk2_succ,scale,(tk2_succ*100.0)/tk_total);
        }
        if (drv_kb_hit())
        {
            mag_close();
            break;
        }
    }
#endif
}
void proto_keyboard(void)
{

    int     iRet = 0, KeyBack = 0;
    int result_line = 24;
    int state_line = 12;

    lcd_cls();
    kb_flush();
    while(1)
    {
        lcdDispMultiLang(0, state_line, FONT_SIZE12|DISP_CLRLINE, "������[ȷ��]�˳�", "Dual [0] to Exit");
        iRet = kb_getkey(-1);
        switch(iRet)
        {
            case KEY0:
            case KEY1:
            case KEY2:
            case KEY3:
            case KEY4:
            case KEY5:
            case KEY6:
            case KEY7:
            case KEY8:
            case KEY9:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[%c]��", "KEY: %c", iRet);
                break;
            case KEY_UP:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[\x1E]��", "KEY: \x1E");
                break;
            case KEY_DOWN:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[\x1F]��", "KEY: \x1F");
                break;
            case KEY_F1:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[F1]��", "KEY: F1");
                break;
            case KEY_F2:
                break;
            case KEY_F3:
                break;
            case KEY_ALPHA:
                break;
            case KEY_ENTER:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[ȷ��]��", "KEY: ENTER");
                break;
            case KEY_CANCEL:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[ȡ��]��", "KEY: CANCEL");
                break;
            case KEY_CLEAR:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[���]��", "KEY: CLEAR");
                break;
            case KEY_BACKSPACE:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[�˸�]��", "KEY: BACKSPACE");
                break;
            case KEY_SYMBOL:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "[����]��", "KEY: SYMBOL");
                break;
            default:
                lcdDispMultiLang(0, result_line, FONT_SIZE12|DISP_CLRLINE, "��Ч����: %02X", "INVALID: %02X", iRet);
                break;
        }

        if(iRet != KEY_TIMEOUT)
        {
            if((iRet == KEY_ENTER) && (KeyBack == KEY_ENTER))
            {
                return ;
            }
            KeyBack = iRet;
        }
    }
}

void proto_bt(void)
{
#ifdef CFG_BLUETOOTH
    int result;
    unsigned char data[128];
    uint8_t name[16];
    char pin[8];
    unsigned char addr[6];

    lcd_cls();
    lcd_display(0,0,FONT_SIZE12|DISP_MEDIACY|DISP_MEDIACY,"bt test");
    bt_close();
    result = bt_open();
    if (result < 0)
    {
        goto END;
    }
    while (1)
    {
        lcd_display(0,36,FONT_SIZE12|DISP_CLRLINE,"bt status:%d",bt_status());
        if (bt_check_readbuf() > 0)
        {
            result = bt_read(data, 128, 1000);
            if (result > 0)
            {
                lcd_display(0, FONT_SIZE12, FONT_SIZE12|DISP_CLRLINE|DISP_MEDIACY, "Recv: %d", result);
                bt_write(data, result);
            }
        }
        if (drv_kb_hit())
        {
            memset(name,0,sizeof(name));
            memset(pin,0,sizeof(pin));
            memset(addr,0,sizeof(addr));
            memset(data, 0, sizeof(data));
            switch (kb_getkey(10))
            {
                case KEY_CANCEL:
                    goto END;
                case KEY_ENTER:
                    strcpy((char *)data, "BLUETOOTH TEST!\n");
                    result = bt_write(data, sizeof("BLUETOOTH TEST!\n"));
                    lcd_display(0,FONT_SIZE12*2, FONT_SIZE12|DISP_CLRLINE, "Send: %d", result);
                    break;
                case KEY1:
                    result = bt_getname((char *)name);
                    if (result < 0)
                    {
                        lcd_display(0,FONT_SIZE12,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Name Fail %d", result);
                        goto END;
                    }
                    else
                    {
                        lcd_display(0, FONT_SIZE12*1,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Name Suc");
                        lcd_display(0,FONT_SIZE12*2,  FONT_SIZE12|DISP_CLRLINE,
                                "%s", name);
                    }
                    break;
                case KEY2:
                    result = bt_getpin(pin);
                    if (result < 0)
                    {
                        lcd_display(0,   FONT_SIZE12,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Pin Fail%d", result);
                        goto END;
                    }
                    else
                    {
                        lcd_display(0,   FONT_SIZE12,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Pin Suc");
                        lcd_display(0,   FONT_SIZE12*2,  FONT_SIZE12|DISP_CLRLINE,
                                "%s", pin);
                    }
                        break;
                case KEY3:
                        result = bt_getaddr(addr);
                    if (result < 0)
                    {
                        lcd_display(0,   FONT_SIZE12,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Addr Fail %d", result);
                        goto END;
                    }
                    else
                    {
                        lcd_display(0, FONT_SIZE12*1,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Addr Suc");
                        lcd_display(0,  FONT_SIZE12*2,  FONT_SIZE12|DISP_CLRLINE,
                                "%x:%x:%x:%x:%x:%x", addr[0], addr[1], addr[2],
                                addr[3], addr[4], addr[5]);
                    }
                    break;
                case KEY4:
                    memcpy(name,"wp30_bt",7);
                    result = bt_setname(name,7);
                    if (result < 0)
                    {
                        lcd_display(0,FONT_SIZE12,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Name Fail %d", result);
                        goto END;
                    }
                    else
                    {
                        lcd_display(0, FONT_SIZE12*1,  FONT_SIZE12|DISP_CLRLINE,
                                "Get Name Fail");
                        lcd_display(0,FONT_SIZE12*2,  FONT_SIZE12|DISP_CLRLINE,
                                "%s", name);
                    }
                    break;
                default:
                    break;
            }
        }
        s_DelayMs(500);
    }

END:
    bt_close();
#endif
}
void proto_setmenu(void)
{
    int key;

    drv_lcd_cls();
    while (1)
    {
        lcd_cls();
        lcd_display(0,0,FONT_SIZE12|DISP_MEDIACY|DISP_MEDIACY,"1-time 2-magcard");
        lcd_display(0,FONT_SIZE12*1,FONT_SIZE12|DISP_MEDIACY,"3-bt  4-keyboard");
        key = drv_kb_getkey(-1);
        switch (key)
        {
        case KEY1:
            proto_time_set();
            break;
        case KEY2:
            proto_magcard();
            break;
        case KEY3:
            proto_bt();
            break;
        case KEY4:
            proto_keyboard();
            break;
        case KEY_CANCEL:
            return;
        default:
            break;
        }
    }
}





void proto_main(void)
{
    uint32_t time;
    uint8_t flag,exit_flg=0;
    int key;

#ifndef CFG_LCD
    lcd_init();
#endif
    while (1)
    {
        proto_welcome();
        time = sys_get_counter();
        while(1)
        {
            if (sys_get_counter()-time > 2500)
                break;
            if (drv_kb_hit())
            {
                if (drv_kb_getkey(100) == KEY_CANCEL){
                    exit_flg++;
                } else {
                    exit_flg = 0;
                }
            }
            if (exit_flg > 3)
            {
                NVIC_SystemReset();
                return;
            }
        }
        proto_menu();
        proto_time();
        time = sys_get_counter();
        flag = 0;
        while (1)
        {
            if (sys_get_counter()-time > 1000)
            {
                time = sys_get_counter();
                proto_menu();
                proto_time();
                flag++;
            }
            if (flag > 3)
                break;
            if (drv_kb_hit())
            {
                key = drv_kb_getkey(100);
                if (key == KEY_CANCEL){
                    exit_flg++;
                } else if (key == KEY_F1){
                    proto_setmenu();
                    exit_flg = 0;
                } else {
                    exit_flg = 0;
                }
            }
            if (exit_flg > 3)
            {
                NVIC_SystemReset();
                return;
            }
        }
    }
}

#endif
