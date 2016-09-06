/***************** (C) COPYRIGHT 2014 START Computer equipment *****************
 * File Name          : libdll.h
 * bfief              : 
 * Author             : luocs() 
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 11/14/2014 10:26:53 AM
 * Description        : 
 *******************************************************************************/

#ifndef __LIBDLL_H__
#define __LIBDLL_H__
#include "wp30_ctrl.h"
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



//*********************************************************************
//                         ϵͳģ�鶨��
//*********************************************************************
#define     SUPPLY_UN_AFFIRM_STATE         0x0
#define     SUPPLY_VIN_BYOFF_STATE         0x1  //������������
#define     SUPPLY_VIN_BYON_STATE          0x2  //����������+���
#define     SUPPLY_BATTERY_NOR_STATE       0x3  //���������ѹ����
#define     SUPPLY_BATTERY_LOW_STATE       0x4  //��ص͵�ѹ����
#define     SUPPLY_USB_STATE               0x5  //USB����
#define     SUPPLY_VIN_BYFULL_STATE        0x6  //����������+��س���

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
int sys_GetModuleType(int module);
int sys_get_module_type(int module);
int sys_GetSysinfo(uint type, uint *outlen, void *outbuf);
int sys_SetSysinfo(uint type, uint inlen, void *inbuf);

//*********************************************************************
//                         ��Դģ�鶨��
//*********************************************************************
#define POWEROFF_OPEN          0
#define POWEROFF_CLOSE         1
int sys_auto_poweroff(int mode,uint32_t timeout_ms);


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
//  ����ģʽ
#define     LIGHT_OFF_TEMP      0       //  ������ʱ�ر�(���ڶ�ʱģʽ��)
#define     LIGHT_TIMER_MODE    1       //  ���ⶨʱģʽ
#define     LIGHT_ON_MODE       2       //  ���ⳣ��ģʽ
#define     LIGHT_OFF_MODE      3       //  ���ⳣ��ģʽ

int lcd_SetLight(int light_mode);
int lcd_SetLighttime(int duration_ms);
void lcd_Cls(void);
void lcd_ClrLine(int begline, int endline);
void lcd_Goto(int x, int y);
int lcd_SetInv(int inv_mode);
void lcd_Plot(int x, int y, int color);
void lcd_GetPos(int *x, int *y);
void lcd_Display(int x, int y, int disp_mode, const char *format, ...);
int lcd_SetIcon(int icon_no, int icon_mode);
void lcd_BmpDisp(int x, int y, const void *bmpdata);
void lcd_BmpmapDisp(int x, int y, int width, int hight, void *scrbmp);


#define     KB_CANCEL               3501     //  �����뱻�û���ȡ�����˳�
#define     KB_TIMEOUT              3502     //  �����볬ʱ�˳�
#define     KB_ERROR                3503      //  ����������Ƿ�
#define     KB_OVERBUF              3504      //  ������������
#define     KB_CHANGE               3505      //  ���л����˳�
void kb_Sound(int freq, int duration_ms);
int kb_GetKey(int timeout_ms);
int kb_Hit(void);
void kb_Flush(void);
int kb_GetStr(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf);
int kb_GetHz(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf);
int kb_InKey(int key);
int kb_ReadKey(uchar *keybuf);


//*********************************************************************
//                         ��ӡģ�鶨��
//*********************************************************************
#if (defined CFG_TPRINTER)
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
int prn_GetAttribute(int attri, int *attrivalue);
int prn_SetAttribute(int attri, int attrivalue);
int prn_Open(void);
int prn_Close(void);
#endif

//*********************************************************************
//                         IC��ģ�鶨��
//*********************************************************************
#if (defined CFG_ICCARD)
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
#endif

//*********************************************************************
//                         �ſ�ģ�鶨��
//*********************************************************************
#if (defined CFG_MAGCARD)
void mag_Open(void);
void mag_Close(void);
void mag_Clear(void);
int mag_Check(void);
int mag_Read(void *track1, void *track2, void *track3);
#endif

//*********************************************************************
//                         ��Ƶģ�鶨��
//*********************************************************************

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
#if defined(CFG_BLUETOOTH)    
int bt_Open(void);
int bt_Close(void);
int bt_Write(uchar *pucBuff, uint uiLen);
int bt_Printf(const char *format, ...);
int bt_read(void *outbuf, int bytelen, int timeOut_ms);
int bt_Clear(void);
int bt_CheckWriteBuf(void);
int bt_CheckReadBuf(void);
int bt_SetName(uchar *pucBuff, uchar ucLen);
int bt_SetPin(uchar *pucBuff, uchar ucLen);
int bt_IoCtl(int cmd, union bt_ioctl_arg *arg);
#endif

#ifdef __cplusplus
}
#endif

#endif


