/*
 * =====================================================================================
 *
 *       Filename:  fac_ctrl.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/25/2014 10:25:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yehf (), yehf@itep.com.cn
 *        Company:  START
 *
 * =====================================================================================
 */
#include "wp30_ctrl.h"
/*-----------------------------------------------------------------------------}
 *  ȫ�ֶ���
 *-----------------------------------------------------------------------------{*/
/* 
 * fac_writeinfo - [GENERIC] д�������������Ϣ��
 * @ 
 */
int fac_writeinfo (uint offset, uint len, void *inbuf)
{
    int ret;
    uchar *backup;
    if ( offset > sizeof(FAC_TESTINFO) ) {
        return -1;
    }
    backup = malloc(FLASH_SECTOR_SIZE);
    if ( backup == NULL ) {
        return -1;
    }
    ret = s_write_syszone(backup,offset+FPOS(SYSZONE_DEF,tTestInfo),len,(uchar *)inbuf);
    free(backup);
    return ret;
}		/* -----  end of function fac_writeinfo  ----- */

/* 
 * fac_readinfo - [GENERIC] ���������������Ϣ��
 * @ 
 */
int fac_readinfo (uint offset, uint len, void *outbuf)
{
    int ret;
    uchar buf[sizeof(FAC_TESTINFO)];
    uint readlen,addr,i;

    if ( offset > sizeof(FAC_TESTINFO) || len > sizeof(FAC_TESTINFO)) {
        return -1;
    }

    if ( !outbuf ) {
        return -1;
    }
    if ( len == 0 ) {
        return -1;
    }
    if (offset%2) 
    {
        addr = offset-1;
        readlen = len + 1;
        i = 1;
    }
    else
    {
        addr = offset;
        readlen = len;
        i = 0;
    }
    if ( readlen%2 ) {
        readlen++;
    }
    ret = s_read_syszone(addr+FPOS(SYSZONE_DEF,tTestInfo),readlen,(uchar *)buf);
    memcpy((uchar *)outbuf,&buf[i],len);

    return ret;
}		/* -----  end of function fac_readinfo  ----- */

//����ID,��Ч�ֽ�18:��Ʒ��(��Ʒƽ̨+��Ʒ��,2B)+00+������(6B)+ʱ��(6B)+Ԥ��(3B)+CRC16(2B)
int fac_ctrl_get_MBID(uchar *id)
{
    uint offset,size;
    ushort crc16;
    offset = FPOS(FAC_TESTINFO,mbno);
    size = FSIZE(FAC_TESTINFO,mbno);
    fac_readinfo(offset,size,id);
    crc16 = calc_crc16(id,(size-2),0);
    if ( !memcmp((uchar *)&crc16,&id[(size-2)],2)) {
        return 0;
    } else {
        return 1;
    }
}

//mode:0-�����������ID,����д��    1-������û������ID,��ǿ��д��
int fac_ctrl_set_MBID(uint8_t mode ,uchar *id)
{
    uint offset,size;
    uchar buf[32];
    offset = FPOS(FAC_TESTINFO,mbno);
    size = FSIZE(FAC_TESTINFO,mbno);
    if(mode == 0)
    {
        if ( !fac_ctrl_get_MBID(buf) ) 
        {
            return 0;
        }
    }
    return fac_writeinfo(offset,size,id);
}

int fac_ctrl_get_Voucherno(uchar *id)
{
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,voucherno);
    size = FSIZE(FAC_TESTINFO,voucherno);
    fac_readinfo(offset,size,id);
    return 0;
}
int  fac_ctrl_set_Voucherno(uchar *id)
{
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,voucherno);
    size = FSIZE(FAC_TESTINFO,voucherno);
    return fac_writeinfo(offset,size,id);
}

uchar fac_ctrl_get_cur_phase(void)
{
    FAC_TESTINFO fac_tinfo;
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,mbno);
    size = sizeof(FAC_TESTINFO);
    fac_readinfo(offset,size,&fac_tinfo);
    return fac_tinfo.phase;
}
int fac_ctrl_set_cur_phase(int iphase)
{
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,phase);
    size = FSIZE(FAC_TESTINFO,phase);
    return fac_writeinfo(offset,size,&iphase);
}
/*-----------------------------------------------------------------------------}
 * ��ȡģ����Խ�� 
 *   id:ģ��ID  0~7 
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_get_test_res(int phase, int id,int *result)
{
    int ret;
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,module_res[phase]);
    size = FSIZE(FAC_TESTINFO,module_res[0]);
    fac_readinfo(offset,size,&ret);
    *result = GETBIT(ret,id);
    return 0;
}

/*-----------------------------------------------------------------------------}
 * ����ģ����Խ��
 *   id:ģ��ID  0~7 
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_set_test_res(int phase, int id,int result)
{
    uint offset,size;
    uint ret;
    offset = FPOS(FAC_TESTINFO,module_res[phase]);
    size = FSIZE(FAC_TESTINFO,module_res[0]);
    fac_readinfo(offset,size,&ret);
    if ( GETBIT(ret,id) != result) {
        // ��ֵͬ,ȡ��
        ret ^= (1<<id);
        return fac_writeinfo(offset,size,&ret);
    }
    return 0;
}
/*-----------------------------------------------------------------------------}
 * ��ȡ�Զ����Խ�� auto_res[0] ��D30~D31
 *  result:0-δ���� 1-�ɹ� 2-ʧ�� 3-ǿ�Ƴɹ�
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_get_autotest_res(int phase)
{
    uint offset,size;
    uint ret;
    offset = FPOS(FAC_TESTINFO,auto_res[phase]);
    size = FSIZE(FAC_TESTINFO,auto_res[0]);
    fac_readinfo(offset,size,&ret);
    return ret>>30;
}
/*-----------------------------------------------------------------------------}
 * �����Զ����Խ�� auto_res[0] ��D30~D31
 *  itemNo:ģ��ID  result:���Խ��
 *  result:0-δ���� 1-�ɹ� 2-ʧ�� 3-ǿ�Ƴɹ�
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_set_autotest_res(int phase, int res)
{
    uint offset,size;
    uint ret;
    offset = FPOS(FAC_TESTINFO,auto_res[phase]);
    size = FSIZE(FAC_TESTINFO,auto_res[0]);
    fac_readinfo(offset,size,&ret);
    if ( (ret>>30) != res ) {
        // ��ֵͬ,ȡ��
        ret &= (~(0x03<<30)); 
        ret |= (res<<30);
        return fac_writeinfo(offset,size,&ret);
    } else {
        return 0;
    }
}

/*-----------------------------------------------------------------------------}
 * �����Զ����Խ�� auto_res[0] ��D0~D29
 *                  result:���Խ��
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_set_test_detail(int phase, int value)
{
    uint offset,size;
    uint ret;
    offset = FPOS(FAC_TESTINFO,auto_res[phase]);
    size = FSIZE(FAC_TESTINFO,auto_res[0]);
    fac_readinfo(offset,size,&ret);
    ret &= (uint)((0x03<<30)); 
    ret |= (value);
    return fac_writeinfo(offset,size,&ret);
}
/*-----------------------------------------------------------------------------}
 * ��ȡ�Զ����Խ�� auto_res[0] ��D0~D29
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_get_test_detail(int phase, int *value)
{
    uint offset,size;
    uint ret;
    offset = FPOS(FAC_TESTINFO,auto_res[phase]);
    size = FSIZE(FAC_TESTINFO,auto_res[0]);
    fac_readinfo(offset,size,&ret);
    ret &= (~(0x03<<30)); 
    *value = ret;
    return 0;
}

/*-----------------------------------------------------------------------------}
 * ��ȡ�ܿ��Ƿ�ɹ���ʶ
 *-----------------------------------------------------------------------------{*/
uchar fac_ctrl_get_fac_ctrl_flag(void)
{
    FAC_TESTINFO fac_tinfo;
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,mbno);
    size = sizeof(FAC_TESTINFO);
    fac_readinfo(offset,size,&fac_tinfo);
    return fac_tinfo.fac_ctrl_flag;
}
/*-----------------------------------------------------------------------------}
 * ���ùܿ��Ƿ�ɹ���ʶ
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_set_fac_ctrl_flag(uchar fac_ctrl_flag)
{
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,fac_ctrl_flag);
    size = FSIZE(FAC_TESTINFO,fac_ctrl_flag);
    return fac_writeinfo(offset,size,&fac_ctrl_flag);
}
/*-----------------------------------------------------------------------------}
 * ��ȡ�ܿ���Ϣ
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_get_fac_testinfo(FAC_TESTINFO *fac_tinfo)
{
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,mbno);
    size = sizeof(FAC_TESTINFO);
    fac_readinfo(offset,size,fac_tinfo);
    return 0;
}
/*-----------------------------------------------------------------------------}
 * ���ùܿ���Ϣ
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_set_fac_testinfo(FAC_TESTINFO *fac_tinfo)
{
    uint offset,size;
    offset = FPOS(FAC_TESTINFO,mbno);
    size = sizeof(FAC_TESTINFO);
    return fac_writeinfo(offset,size,fac_tinfo);
}
/*-----------------------------------------------------------------------------}
 * ��ȡ�ϻ����Խ�� s_aging_log���ϻ����Խ�������ļ�ϵͳ��
 * �ļ�ϵͳ��32K,ʹ�����2������,д��¼ʱ����ʹ��:��¼��ʽ:0/1+log+"OK"
 *-----------------------------------------------------------------------------{*/

typedef struct __S_AGING_LOG_SAVE {
    int id;
    s_aging_log log;
    char flag[4];
}S_AGING_LOG_SAVE;				/* ----------  end of struct _S_AGING_LOG_SAVE  ---------- */

#define AGINGLOG_BLOCK_NUM  2
#define AGINGLOG_BLOCK_SIZE FLASH_SECTOR_SIZE
#define SA_AGINGLOG   (SA_RFU+LEN_RFU-AGINGLOG_BLOCK_SIZE*AGINGLOG_BLOCK_NUM)
#define SA_AGINGLOG_BLCOK1 SA_AGINGLOG
#define SA_AGINGLOG_BLCOK2 (SA_AGINGLOG+AGINGLOG_BLOCK_SIZE*1)
/* 
 * fac_read_aginglog - [GENERIC] �������ϻ����Խ��
 *    ����-��Ч��¼��� 0-�޼�¼
 * @ 
 */
int fac_read_aginglog (int *id,s_aging_log *log)
{
    int i,flag=0; 
    S_AGING_LOG_SAVE *p;
    p = malloc(AGINGLOG_BLOCK_NUM*AGINGLOG_BLOCK_SIZE);
    for ( i=0 ; i<AGINGLOG_BLOCK_NUM ; i++ ) {
      readFlashPage((SA_AGINGLOG+AGINGLOG_BLOCK_SIZE*i)/2,(uchar *)&p[i].id, (ushort)AGINGLOG_BLOCK_SIZE);
    }
//    TRACE_BUF("log",(char *)p,AGINGLOG_BLOCK_NUM*AGINGLOG_BLOCK_SIZE);
    // �ж��Ŀ�����
    for ( i=0; i<AGINGLOG_BLOCK_NUM ; i++ ) {
        if ( !memcmp(p[i].flag,ISOK,4) ) {
            flag |= (1<<i);
        }
    }
    // 0-Ϊ�� 1-��1������ 2-��2������ 3-����ID���ж�����
    if ( !flag ) {
        free(p);
        *id = 0;
        return 0;
    } else {
        if ( flag == 0x03 ) {
            if ( p[0].id > p[1].id ) {
                flag = 1;
            } else {
                flag = 2;
            }
        }
    }
    memcpy((uchar *)log,(uchar *)&p[flag-1].log,sizeof(s_aging_log));
    *id = p[flag-1].id;
    free(p);
    return flag;
}		/* -----  end of function fac_read_aginglog  ----- */
/* 
 * fac_write_aginglog - [GENERIC] д�ϻ����Խ��
 *    0-�ɹ� 1-ʧ��
 * @ 
 */
int fac_write_aginglog (s_aging_log *log)
{
    int ret,j,addr,i;
    S_AGING_LOG_SAVE t;
    uchar *p;
    j = fac_read_aginglog(&i,&t.log);
    if ( j==0 ) {
        // д��1��
        addr = SA_AGINGLOG_BLCOK1;
    } else if(j == 1){
        // д��2��
        addr = SA_AGINGLOG_BLCOK2;
    } else {
        // д��1��
        addr = SA_AGINGLOG_BLCOK1;
    }
    ret = eraseFlashPage(addr/2);
	if(ret)
	{
		return ERROR;
	}
    t.id = ++i; 
    memcpy((uchar *)&t.log,(uchar *)log,sizeof(s_aging_log));
    memcpy((uchar *)t.flag,(uchar *)ISOK,4);
    p = (uchar *)malloc(FLASH_SECTOR_SIZE);
    memset(p,0xFF,FLASH_SECTOR_SIZE);
    // д�¼�¼
    memcpy(p,&t,sizeof(t));
//    TRACE_BUF("write log",p,sizeof(t));
    ret = writeFlashPage(addr/2,p,sizeof(t));
    free(p);
    if ( addr == SA_AGINGLOG_BLCOK1 ) 
    {
        addr = SA_AGINGLOG_BLCOK2;
    }
    else
    {
        addr = SA_AGINGLOG_BLCOK1;
    }
    // ɾ���ɼ�¼
    ret = eraseFlashPage(addr/2); 
    return ret;
}		/* -----  end of function fac_write_aginglog  ----- */

/*-----------------------------------------------------------------------------}
 * ��ȡ�������Թ��̼�¼���,��������ʱ��,���
 * * �ļ�ϵͳ��32K,ʹ���������3,4����,д��¼ʱ����ʹ��:��¼��ʽ:0/1+log+"OK"
 *-----------------------------------------------------------------------------{*/
typedef struct FAC_TEST_LOG{
    int s_mv;//����ʱ��ѹ
    int e_mv;//�ػ�ʱ��ѹ
    char s_time[8];//����ʱ��
    char e_time[8];//�ػ�ʱ��
    char flag[4];
}t_fac_test_log;				

#define FACLOG_BLOCK_NUM  2
#define FACLOG_BLOCK_SIZE FLASH_SECTOR_SIZE
#define SA_FACLOG   (SA_RFU+LEN_RFU-FACLOG_BLOCK_SIZE*FACLOG_BLOCK_NUM*2)
#define SA_FACLOG_BLCOK1 SA_FACLOG
#define SA_FACLOG_BLCOK2 (SA_FACLOG+FACLOG_BLOCK_SIZE*1)

#define LOG_MAX         10 

int fac_read_runinfo_log(int *id,int *no,uchar *log)
{
    int len,offset;
    int i,j;
    uchar buf[FACLOG_BLOCK_SIZE];
    len = sizeof(t_fac_test_log);
    offset = FPOS(t_fac_test_log,flag);
    for(i=0;i<FACLOG_BLOCK_NUM;i++)
    {
      readFlashPage((SA_FACLOG+FACLOG_BLOCK_SIZE*i)/2,(uchar *)buf, (ushort)FACLOG_BLOCK_SIZE);
      if ( !memcmp(&buf[offset],ISOK,4) ) {
          break;
      }
    }
    if ( i == FACLOG_BLOCK_NUM ) {
        *no = 0;
        *id = 0;
        memcpy((uchar *)log,(uchar *)buf,FLASH_SECTOR_SIZE);
        return -1;
    }
//    TRACE_BUF("READ BUF",&buf[0], FLASH_SECTOR_SIZE);
    for(j=0;j<(FACLOG_BLOCK_SIZE/len);j++)
    {
      if ( !memcmp(&buf[offset+len*j],ISOK,4) ) {
          continue;
      }
      else
      {
          break;
      }
    }
    *id  = i;
    *no = j;
    memcpy((uchar *)log,(uchar *)buf,FLASH_SECTOR_SIZE);
    return 0;
}		/* -----  end of function   fac_read_runinfo_log  ----- */
int fac_write_runinfo_log(t_fac_test_log log)
{
    uchar buf[FACLOG_BLOCK_SIZE];
    uchar backbuf[FACLOG_BLOCK_SIZE/2];
    int no,id;
    int ret;
    int addr0,addr1;
    int len;
    len = sizeof(t_fac_test_log);
    ret = fac_read_runinfo_log(&id,&no,buf);
    if ( ret < 0 || id == 0) 
    {
        addr0 = SA_FACLOG_BLCOK1;
        addr1 = SA_FACLOG_BLCOK2;
    }
    else
    {
        addr0 = SA_FACLOG_BLCOK2;
        addr1 = SA_FACLOG_BLCOK1;
    }
    if ( no > (LOG_MAX-1) ) {
        no = LOG_MAX-1;
        memcpy((uchar *)backbuf,(uchar *)&buf[len],no*len);
        memcpy((uchar *)&backbuf[no*len],&log,len);
        memcpy((uchar *)buf,(uchar *)backbuf,FACLOG_BLOCK_SIZE/2);
    }
    else
    {
        memcpy((uchar *)&buf[no*len],&log,len);
    }

    ret = eraseFlashPage(addr0/2);
    if ( ret ) {
        return ERROR;
    }
//    TRACE_BUF("Write BUF",&buf[0], FLASH_SECTOR_SIZE);
    ret = writeFlashPage(addr0/2,buf,FACLOG_BLOCK_SIZE);
    ret = eraseFlashPage(addr1/2);
    return ret;
}		/* -----  end of function sys_write_fac_log   ----- */


t_fac_test_log g_faclog;

// 0-����д�� 1-�ػ�д��
int fac_save_log(int mode)
{
    return 0;
//#if PRODUCT_NAME == PRODUCT_MPOS
//    volatile static uint s_time = 0;
//    int mv;
//    char time[8];
//    int ret;
//    // ����ܿسɹ�,��д��
//    if(gwp30SysMemory.SysCtrl.bit.factoryisok == 1)
//    {
//        return 0;
//    }
//    if ( mode == 0 ) 
//    {
//        s_time = sys_get_counter(); 
//        CLRBUF(&g_faclog);
//        sys_GetBattery(&mv);
//        g_faclog.s_mv = mv;
//        sys_GetTime(time);
//        memcpy((uchar *)g_faclog.s_time,(uchar *)time,8);
//        memcpy((uchar *)g_faclog.flag,(uchar *)ISOK,4);
////        TRACE_BUF("gFaclog0",(uchar *)&g_faclog,sizeof(t_fac_test_log));
//        return 0;
//    }
//    if ( sys_get_counter() - s_time <= 8*60*1000 ) {
//        // С��8�����򲻼�¼
//        return OK;
//    }
//    sys_GetBattery(&mv);
//    g_faclog.e_mv = mv;
//    sys_GetTime(time);
//    memcpy((uchar *)g_faclog.e_time,(uchar *)time,8);
//    memcpy((uchar *)g_faclog.flag,(uchar *)ISOK,4);
////    TRACE_BUF("gFaclog1",(uchar *)&g_faclog,sizeof(t_fac_test_log));
//    ret = fac_write_runinfo_log(g_faclog);
//    return ret;
//#else
//    return 0;
//#endif
}

int show_run_info(int mode)
{
#if PRODUCT_NAME == PRODUCT_MPOS
    int i,no;
    int ret;
//    int key;
    t_fac_test_log *t;
    t = malloc(FACLOG_BLOCK_SIZE);
    lcd_cls();
    ret = fac_read_runinfo_log(&i,&no,(uchar *)t);
    lcd_display(0,DISP_FONT_LINE0,DISP_FONT|DISP_CLRLINE|DISP_MEDIACY|DISP_INVLINE,"������Ϣ:%d",no);
    if ( ret == -1 ) {
        lcd_display(0,DISP_FONT_LINE2,DISP_FONT|DISP_CLRLINE,"�޼�¼");
        KB_GetKey(2000);
        free(t);
        return 0;
    }
    while(no--)
    {
        lcd_display(0,DISP_FONT_LINE0,DISP_FONT|DISP_CLRLINE|DISP_MEDIACY|DISP_INVLINE,"������Ϣ:%d",no);
        lcd_display(0,DISP_FONT_LINE1,DISP_FONT|DISP_CLRLINE,"20%02X%02X%02X:%02X%02X%02X",t[no].s_time[0],t[no].s_time[1],t[no].s_time[2],t[no].s_time[3],t[no].s_time[4],t[no].s_time[5],t[no].s_time[6]);
        lcd_display(0,DISP_FONT_LINE2,DISP_FONT|DISP_CLRLINE,"���:%d",t[no].s_mv);
        lcd_display(0,DISP_FONT_LINE3,DISP_FONT|DISP_CLRLINE,"20%02X%02X%02X:%02X%02X%02X",t[no].e_time[0],t[no].e_time[1],t[no].e_time[2],t[no].e_time[3],t[no].e_time[4],t[no].e_time[5],t[no].e_time[6]);
        lcd_display(0,DISP_FONT_LINE4,DISP_FONT|DISP_CLRLINE,"���:%d",t[no].e_mv);
        if ( KB_GetKey(-1) == KEY_CANCEL ) 
        {
            free(t);
            return 0;
        }
    }
    free(t);
#endif
    return 0;
}

#ifdef DEBUG_Dx
void dbg_fac_log(void)
{
    int key;
//    t_fac_test_log t;
    int i,no;
//    int j= 0;
    t_fac_test_log *buf;
//    int mv;
//    int ret;
//    int len,offset;
//    len = sizeof(t_fac_test_log);
    buf = malloc(FLASH_SECTOR_SIZE);
    while (1)
    {
        TRACE("\r\n-|*****************gpio  debug***************|-");
        TRACE("\r\n-|1-read 2-write0 3-write1 4-erase  |-");
        TRACE("\r\n-|*******************************************|-\t");
        key = InkeyCount(0);
        CLRBUF(buf);
        switch (key)
        {
        case 1:
            fac_read_runinfo_log(&i,&no,(uchar *)buf);
            TRACE_BUF("READ BUF",(uchar *)&buf[0], FLASH_SECTOR_SIZE);
        case 2:
            CLRBUF(&g_faclog);
            fac_save_log(0);
            break;
        case 3:
            fac_save_log(1);
            break;
        case 4:
            eraseFlashPage( SA_FACLOG_BLCOK1/2);
            eraseFlashPage( SA_FACLOG_BLCOK2/2);
            break;
        case 99:
            free(buf);
            return;
        default:
            break;
        }
    }
}
#endif
