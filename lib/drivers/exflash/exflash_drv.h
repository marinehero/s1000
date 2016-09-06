/*
 * =====================================================================================
 *
 *       Filename:  exflash_drv.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  5/17/2016 10:29:13 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */
#ifndef __EXFLASH_DRV_H__
#define __EXFLASH_DRV_H__

/**
    ����	��ʼ��ַ	��С���ֽڣ�	˵��
        0	0x0000	    4096	        ������
        1	0x1000	    4096*2	Ӧ��ԭ��ǩ����Ȩ��Ӧ��ǩ��Hash����SHA256��ÿ��32�ֽڣ�
                                ÿҳ���Դ洢16�飬һ�����Դ洢256�飬�����֧������ǩ256��Ԥ��Ӧ��ǩ��
        2	0x3000	    4096	        ʵ��Ԥ��ǩ����Կ���ݶ�8�飬1ҳ1�顣
        3	0x4000	    4096	        ����Ӧ����Ҫʹ�õ�����������
        4	0x5000	    4096*4	        ������������SPI FLASHд����ʱ������
        5	0x9000	    4096*503	    Ԥ��
 */
#define EXFLASH_SECTOR_SIZE     4096 
#define EXFLASH_PAGE_SIZE       512

#define EX_CFG_LEN              EXFLASH_SECTOR_SIZE
#define EX_GSHA_LEN             EXFLASH_SECTOR_SIZE*2
#define EX_START_PUB_LEN        EXFLASH_SECTOR_SIZE 
#define EX_YINLIAN_LEN          EXFLASH_SECTOR_SIZE 
#define EX_TMP_BUF_LEN          EXFLASH_SECTOR_SIZE*4 

typedef struct _EXFLASH_ROM
{
    uint8_t cfg[EX_CFG_LEN];            // 4096K
    uint8_t gsha[EX_GSHA_LEN];          //  
    uint8_t start_pub[EX_START_PUB_LEN];// (MK-210��Կ��)
    uint8_t yilian[EX_YINLIAN_LEN];     // 92 (Ԥ��)
    uint8_t tmp_buf[EX_TMP_BUF_LEN];    //320K
}EXFLASH_R0M;

#define EXFLASH_CFGINFO_ADDR            (FPOS(EXFLASH_R0M,cfg))
#define EXFLASH_CFGINFO_SIZE            (FSIZE(EXFLASH_R0M,cfg))

#define EXFLASH_GSHA_ADDR               (FPOS(EXFLASH_R0M,gsha))
#define EXFLASH_GSHA_SIZE               (FSIZE(EXFLASH_R0M,gsha))

#define EXFLASH_START_PUB_ADDR          (FPOS(EXFLASH_R0M,start_pub))
#define EXFLASH_START_PUB_SIZE          (FSIZE(EXFLASH_R0M,start_pub))

#define EXFLASH_YINLIAN_ADDR            (FPOS(EXFLASH_R0M,yilian))
#define EXFLASH_YINLIAN_SIZE            (FSIZE(EXFLASH_R0M,yilian))

#define EXFLASH_TMP_BUF_ADDR            (FPOS(EXFLASH_R0M,tmp_buf))
#define EXFLASH_TMP_BUF_SIZE            (FSIZE(EXFLASH_R0M,tmp_buf))

unsigned int exflash_write(unsigned int addr,unsigned int data_len,unsigned char* data);

unsigned int exflash_read(unsigned int addr,unsigned int data_len,unsigned char* data);
#endif

