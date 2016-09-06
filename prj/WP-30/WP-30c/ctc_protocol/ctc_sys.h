/*
 * =====================================================================================
 *
 *       Filename:  ctc_sys.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  4/12/2016 3:38:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */
#ifndef __CTC_SYS_H__
#define __CTC_SYS_H__ 

#include "ParseCmd.h"
#include "wp30_ctrl.h"
#include "..\..\..\..\lib\drivers\time\rtc-lib.h"

#define EXFLASH_NUMBER_MAX     128
#define EXFLASH_DATA_LEN_MAX   512

//#define MDF_MASK 0x7F01FC 
#define MDF_MASK 0x7F00FC 

// download file type
#define DOWNLOAD_TYPE_CTRL 0x00     // ���ؼ������ 
#define DOWNLOAD_TYPE_SAPK 0x01     // ���ض���APK��Կ 
#define DOWNLOAD_TYPE_GAPK 0x02     // ���ع̼�APK��Կ 

//  safemanage type
#define SAFEMANAGE_TYPE_ORIGIN      0x00       // �л����� 
#define SAFEMANAGE_TYPE_TAMPERDIS   0x01       // ���������� 
#define SAFEMANAGE_TYPE_SUPERAUTH   0x02       // ��Ȩģʽ 
//  safemanage mode 
#define SAFEMANAGE_MODE_IC          0x00       // IC��ģʽ 
#define SAFEMANAGE_MODE_AUTHEPAG    0x01       // Ȩ�ް�ģʽ 

//Э�� �͹���
#define LOWPOWER_DELAYED 2000

extern uint32 Lowpower_Timer;

typedef struct _SYSTICK_DEAMON_
{
    uint8_t flash_flag;
    uint8_t flash_led;
}sys_tick_deamon;


typedef struct _LED_STATUS
{
    uint8_t type;           //�Ƿ���Ƹõ� 1-���� 0-������
    uint8_t ctr;            //�Ƶ����� 1-�� 0-��
    //uint8_t flash; //�Ƿ���˸ 0-���� 1-��˸
    uint8_t rfu[1]; //���
}led_status;


typedef struct _LED_OPT
{
    led_status status;      //��״̬
    uint16_t flashing_times;//��˸���� 
    uint16_t flashing_cyc;  //��˸���� ��λ10ms
    uint16_t Ontime;        //����ʱ�� ��λ10ms
    uint16_t time_count;    //ʱ�����
}led_opt;

typedef struct _CTRL_SYSMEMORY_LED
{
    led_opt led;
    sys_tick_deamon sys_tick;
}MK210_sys_memory;

typedef struct _DL_FILE
{
    int type;             //�ļ�����
    uchar version[32];    //�ļ��汾
    int len;              //�ļ�����
    uchar sha[32];        //shaУ��
}dl_file;

typedef struct _DL_info
{
    dl_file file_info;      //�ļ���Ϣ
    
    int current_offset;     //��ǰ�����ƫ��
    int current_len;     //��ǰ�����ƫ��
    SHA256Context sha;

}dl_info;

typedef struct _UPDATE_INFO
{
    int data_len;  //��ش�С ��λ�ֽ�
    int flag;      //������־ 0xaa-���µ������ļ� 0x55-��������
}update_info;

extern dl_info sys_dl_info;

extern MK210_sys_memory gled_sys_memory;

//LED
uint16_t sys_manage_led(uint8_t* data);
//BEEP
uint16_t sys_manage_beep(uint16_t time,uint16_t frequency);

uint16_t terminal_info_soft(uint32_t *len,uint8_t* buffer);

uint16_t terminal_info_hard(uint32_t *len,uint8_t* buffer);
//RTC
uint16_t sys_manage_get_rtc(struct rtc_time* tm);

uint16_t sys_manage_set_rtc(struct rtc_time* tm);

//EXFLASH
uint16_t sys_manage_exflash_write(uint8_t mode,uint16_t number,uint16_t data_len,uint8_t* data);
uint16_t sys_manage_exflash_read(uint8_t mode,uint16_t number,uint16_t data_len,uint8_t* data);

//MDF
uint16_t sys_mdf_status_get(int *status, uchar *time);
uint16_t sys_mdf_unlock(void);

//download
uint16_t sys_manage_download_inform(uint8_t *data,uint8_t *buffer,uint32_t *data_len);
uint16_t sys_manage_download_data(uint8_t *data,uint8_t *buffer,uint32_t *buffer_len);
uint16_t sys_manage_download_finish(uint8_t *data);

//cpu manage
uint16_t sys_cpu_get_inform(uint8_t *data,uint8_t *output,uint32_t *data_len);
uint16_t sys_cpu_set_inform(uint8_t *input, int input_len);

//sys state
uint16_t sys_entry_lowpower(uint8_t* data);
void sys_power_state_inform(void);

// sys safe
uint16_t sys_safe_manage(uint8_t* data, int datalen, uint8_t* output, uint32_t *outlen);
#endif /*end __CTC_SYS_H__*/


