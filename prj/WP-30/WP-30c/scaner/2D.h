#ifndef __2D_H
#define __2D_H

#include "common.h"
#include "em3096.h"
#include "em1395.h"


#define SCANPIT PIT0
//#define SCANER_EM1395
#define SCANER_EM3096   //һά���ά��ͨ��һ�׳���
#define SCANER_TYPE_EM1395  0
#define SCANER_TYPE_EM3096  1

typedef struct _SCANER_ASY_PARAM{

    volatile int timecount;      // ���㳬ʱʱ�� 
    int scan_flag;      // ����ɨ��ģʽ����⿪ʼ
    int scan_timeout;   // ��ʱ��־

}SCANER_ASY_PARAM;

typedef struct __SCANER_FOPS_t {
    uchar chiptype;
    int (*init)(void);
    int (*open)(void);
    int (*close)(void);
//    int (*write)(int mode,int value);
    int (*scan)(int mode);
    int (*ioctl)(int mode, int para);
    int (*main)(int mode);
}SCANER_FOPS_t;


void scaner_2d_init();

uint8_t scaner_2d_open();

uint8_t scaner_2d_close ();

void scaner_2d_scan_start(uint32_t time, uint32_t mode); 

uint32_t scaner_2d_scan(void) ;

void scaner_2d_scan_cancel(void);

#endif
