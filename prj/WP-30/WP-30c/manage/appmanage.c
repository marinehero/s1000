/*
 * =====================================================================================
 *
 *       Filename:  authmanage.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  6/20/2016 6:07:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yehf (), yehf@itep.com.cn
 *        Company:  START
 *
 * =====================================================================================
 */


#include "wp30_ctrl.h"

#ifdef CFG_SE_MANAGE
/*-----------------------------------------------------------------------------}
 *  ����˵��
 *-----------------------------------------------------------------------------{*/

/* 
 * s_read_pubk - [GENERIC] ����Կ
 *   type: 0-CA��Կ 1-�����̹̼���Կ 2-����������Կ 3-��ʵ��Ӧ�ù�Կ 4-������APK��Կ
 *
 * @ 
 */
#define SAPK_PUBKEY_ADD SA_KEYZONE+((FLASH_SECTOR_SIZE/FLASH_SECTOR_NbrOf1024)*200)
uint s_save_sys_sapk(uchar *buf)
{
	if(eraseFlashPage(SAPK_PUBKEY_ADD/2))
	{
		return ERROR;
	}
	if(writeFlashPage(SAPK_PUBKEY_ADD/2,buf,sizeof(R_RSA_PUBLIC_KEY)))
	{
		return ERROR;
	}
	return OK;
}

uint s_get_sys_sapk(uchar *buf)
{
    readFlashPage(SAPK_PUBKEY_ADD/2, buf, sizeof(R_RSA_PUBLIC_KEY));
	return OK;
}

/* 
 * s_pubk_decrypt - [GENERIC] ʹ�ù�Կ����
 *     type: 0-CA��Կ 1-�����̹̼���Կ 2-����������Կ 3-��ʵ��Ӧ�ù�Կ 4-������APK��Կ
 *     uilen:�������ݳ��ȣ�����С�ڹ�Կ����
 * @ 
 */
int s_pubk_decrypt (unsigned int type, unsigned int uilen, unsigned char *in, unsigned char *out, unsigned int* outlen)
{
    int ret,i;
    R_RSA_PUBLIC_KEY publickey;
    if ( uilen > 256 ) {
        return -1;
    }
    if ( type == 4 ) {
        s_get_sys_sapk((uchar *)(&publickey));
        TRACE_BUF("��Կ",(uint8_t *)&publickey,sizeof(R_RSA_PUBLIC_KEY));
        //����ǩ����
        TRACE_BUF("decrtpt data",in,uilen);
        ret = Rsa_calt_pub(uilen,in,&publickey,&i,out);
        TRACE_BUF("origin data",out,i);
        *outlen = i;
        memset(&publickey,0,sizeof(publickey));
        if ( ret ) {
            return -3;
        }
    } else {
        ret = s_startpubk_decrypt(type,uilen,in,out,outlen);
    }
    return ret;
}		/* -----  end of function s_pubk_decrypt  ----- */
// shaУ��ֵ�ж�
// return 0 �ɹ�
int s_Check_Sha(uint len, ST_newtaginfo *stag)
{
    uint8_t data0[512];
    uint16_t pagenum=0;
    SHA256Context sha;
    int i;
    int datalen=0;

// ����sha ����304(32+256+16)�ֽڲ�����shaУ��
    pagenum = (len-304+EXFLASH_PAGE_SIZE-1)/EXFLASH_PAGE_SIZE;
    TRACE("sha pagenum:%d\r\n", pagenum);
    sha256_init(&sha);
    for ( i=0 ; i<pagenum ; i++ ) {
        if ( i==(pagenum-1) ) {
            datalen = len-304-((pagenum-1)*EXFLASH_PAGE_SIZE); 
        }else{
            datalen = EXFLASH_PAGE_SIZE;     
        }
        exflash_read(EXFLASH_TMP_BUF_ADDR+EXFLASH_PAGE_SIZE*i, datalen, (uchar *)(&data0[0]));
        sha256_update(&sha, (uchar *)(&data0[0]), datalen);
    }
    sha256_final(&sha, data0);
    TRACE_BUF("sha", data0, 32);
    return s_check_firmware (0, data0 , (uchar *)stag);
}


/* 
 * Descript:��дgpkg sha������flash��ַ��ƫ������
 * 
 *  : 
 * */
extern uint16_t sys_manage_exflash_write(uint8_t mode,uint16_t number,uint16_t data_len,uint8_t* data);
uint16_t s_gapk_flash_write(uint16_t number, uint32_t data_len,uint8_t* data)
{
    return sys_manage_exflash_write(1, number+EXFLASH_GSHA_ADDR/EXFLASH_PAGE_SIZE, (uint16_t)data_len, data);
}
extern uint16_t sys_manage_exflash_read(uint8_t mode,uint16_t number,uint16_t data_len,uint8_t* data);
uint16_t s_gapk_flash_read(uint16_t number,uint32_t data_len,uint8_t* data)
{
    return sys_manage_exflash_read(1, number+EXFLASH_GSHA_ADDR/EXFLASH_PAGE_SIZE, (uint16_t)data_len, data);
}
/*
 * Descript: replace ���ǰ���������
 * 
 * */
uint32_t s_gapk_replace_sha(uint32_t len)
{
    uint16_t pagenum=0;
    int i;
    int shalen=0, datalen=0;
    uint32_t savenum=0;
    uint8_t data0[512];
    uint8_t data1[512];
    int ret;
    shalen = len-sizeof(ST_newtaginfo)-16;
    pagenum = (shalen + EXFLASH_PAGE_SIZE - 1)/EXFLASH_PAGE_SIZE;
    // ׼��д�����,��512Ϊ��λ�����һ�������Ч����
    for ( i=0 ; i<pagenum ; i++ ) {
        if ( i==(pagenum-1) ) {
            datalen = shalen-((pagenum-1)*EXFLASH_PAGE_SIZE); 
        }else{
            datalen = EXFLASH_PAGE_SIZE;
        }
        exflash_read(EXFLASH_TMP_BUF_ADDR+16+EXFLASH_PAGE_SIZE*i, datalen, (uchar *)(&data1[0]));
        s_DesRSAPubk(1, datalen, &data1[0], &data0[0]);
        TRACE_BUF("data0", data0, datalen);
        // ������exflash_write�������
        //            ret = sys_manage_exflash_write(1, i+EXFLASH_GSHA_ADDR/EXFLASH_PAGE_SIZE, (uint16)datalen, data0);
        ret = s_gapk_flash_write(i, datalen, data0);
        if ( ret != 0 ) {
            TRACE("write exflash err\r\n");
            return RET_WRITE_DEVICE; 
        }
        //            sys_manage_exflash_read(1,pagenum-1+EXFLASH_GSHA_ADDR/EXFLASH_PAGE_SIZE, datalen, data1);
        //            TRACE_BUF("data1", data1, datalen);
    }
    // �ɹ���д���־
    //        savenum += optnum;
    savenum = shalen/32;
    TRACE("\ngpkg savenum:%d\n", savenum);
    return s_sysinfo_gpakshainfo(1, &savenum);
}
/*Descript:ͨ����ȡflashҳ,�Ƿ���Ҫ�����ɾ����sha�ڶ�ȡ��flashҳ��
 *return  : 0 �����У���0 ��ʾ����shaֵ����λ��
 * */
int s_page_hit_sha(uint8_t *shavalue, uint8_t *comparebuf, int len)
{
    int i; 
    int shanum;
    if ( len%32 != 0 ) {
        //  
        return -1;
    }
    shanum = len/32;
    for ( i=1 ; i<=shanum ; i++ ) {
        if ( memcmp(shavalue, &comparebuf[32*(i-1)], 32) ) {
            continue;
        }
        return i;
    }
    return 0;
}
/* Descript:
 * ��ĩβƫ�ƴ�����sha���ݣ�
 * ������ӵ�shaδ����һҳ���ݲ�д��flash���棬���Ƿ��ش���������ݴ���������
 * ������ӵ�sha����һҳ��д��flash����
 *
 * */
int s_gapk_sha_addtail(uint32_t exist_lastpage, uint32_t tail_off_num, uint8_t *shadata, uint8_t *savedata, uint32_t *savelen)
{
    uint16_t savepage;
    uint32_t exist_shanum=0;    // �Ѿ�������ϵͳ���shaֵ��Ŀ
    int exist_datalen=0;
    savepage = (tail_off_num+15) / 16; 
    if ( savepage > 16 ) {
       TRACE("\r\nexceed the max store\r\n");
       return -1;
    }
    // ˵����ǰ�����Ѿ�����
    if ( savepage != exist_lastpage ) {
       TRACE("exist_lastpage is not same as savepage\r\n");
       // ����һ��sha
       memcpy(&savedata[*savelen], shadata, 32); 
       *savelen += 32;
    }else {
    // ��ǰ���������λ��Ϊͬһҳ
       if ( 0 == *savelen ) {
           // ��ȡ���ҳ�Ѿ����������
           s_sysinfo_gpakshainfo(0, &exist_shanum);
           TRACE("\r\n&&&&&&:%d\r\n", exist_shanum);
           exist_datalen = (exist_shanum*32)%EXFLASH_PAGE_SIZE;
           TRACE("&&&&&&exist_datalen:%d\r\n", exist_datalen);
           s_gapk_flash_read(exist_lastpage-1, exist_datalen, savedata);
           memcpy(&savedata[exist_datalen], shadata, 32); 
           *savelen = exist_datalen+32;
       }else{
           memcpy(&savedata[*savelen], shadata, 32); 
           *savelen += 32;
       }
    }
    // �������������ù���һҳ��д��ҳ��������
    if ( *savelen == EXFLASH_PAGE_SIZE ) {
        s_gapk_flash_write(savepage-1, EXFLASH_PAGE_SIZE, savedata);
        memset(savedata, 0, 512);
        *savelen = 0;
    }
    TRACE("save len:%d\r\n", *savelen);
    return 0;
}
/* Descript:
 * ����δд��flash�����ݣ�
 * ������ӵ�shaδ����һҳ���ݲ�д��flash���棬���Ƿ��ش���������ݴ���������
 * ������ӵ�sha����һҳ��д��flash����
 *
 * */
int s_gapk_sha_dealtail( uint32_t tail_off_num, uint8_t *savedata, uint32_t *savelen)
{
    uint16_t savepage;
    savepage = (tail_off_num+15) / 16;
    if ( *savelen != 0 ) {
        s_gapk_flash_write(savepage-1, *savelen, savedata);
    }
    return 0;
}
uint32_t s_gapk_add_sha(uint32_t len)
{
    int ret;
    int i,j,z;
    uint8_t data0[512];
    uint8_t data1[512];
    uint8_t exist_data[512];
    uint8_t save_data[512];
    uint16_t pagenum=0, exist_pagenum;
    uint32_t exist_shanum=0;    // �Ѿ�������ϵͳ���shaֵ
    uint32_t add_shanum=0;      // ���ӵ�shaֵ
    uint32_t savelen=0;
    int shalen=0, datalen=0, exist_datalen=0;
    shalen = len-sizeof(ST_newtaginfo)-16;
    pagenum = (shalen + EXFLASH_PAGE_SIZE - 1)/EXFLASH_PAGE_SIZE;
    s_sysinfo_gpakshainfo(0, &exist_shanum);
    // ���û�м�⵽�����ع�gpkg��������Ҫ�Ƚ�ֱ��д��
    if ( (exist_shanum == 0) || (exist_shanum == 0xffffffff) ) {
       return s_gapk_replace_sha(len); 
    }
    exist_pagenum = (exist_shanum + 15)/16;  
    TRACE("\r\n@get the pagenum:%d\r\n", pagenum);
    for ( i=0 ; i<pagenum ; i++ ) {
        if ( i==(pagenum-1) ) {
            datalen = shalen-((pagenum-1)*EXFLASH_PAGE_SIZE); 
        }else{
            datalen = EXFLASH_PAGE_SIZE;
        }
        TRACE("@@@@@@@@@get the datalen:%d\r\n", datalen);
        exflash_read(EXFLASH_TMP_BUF_ADDR+16+EXFLASH_PAGE_SIZE*i, datalen, (uchar *)(&data1[0]));
        s_DesRSAPubk(1, datalen, &data1[0], &data0[0]);
//        TRACE_BUF("data0", data0, datalen);
        // �Ƚ��ж��Ƿ����sha
        for ( j=0 ; j<datalen/32 ; j++ ) {
            for ( z=0 ; z<exist_pagenum; z++ ) {
                if ( z==(exist_pagenum-1) ) {
                    exist_datalen = (exist_shanum*32)%EXFLASH_PAGE_SIZE ; 
                    if ( 0 == exist_datalen ) {
                        exist_datalen = EXFLASH_PAGE_SIZE;
                    }
                }else{
                    exist_datalen = EXFLASH_PAGE_SIZE;
                }
                memset( exist_data, 0, sizeof(exist_data) );
                s_gapk_flash_read(z, exist_datalen, exist_data);
                ret = s_page_hit_sha(&data0[j*32], exist_data, exist_datalen);  
                // ret >0 ˵�����У�����
                if ( ret > 0 ) {
                    TRACE("*****hit the data:%d\r\n", i*16+j+1);
                    break;     
                }
            }
            // û��������Ҫ��ԭ��ŵ�β�����Ӽ�¼
            if ( z == exist_pagenum ) {
                add_shanum ++;
//                TRACE("*****add_shanum:%d\r\n", add_shanum);
                ret = s_gapk_sha_addtail(exist_pagenum, add_shanum + exist_shanum, &data0[j*32], save_data, &savelen);
                if ( ret != 0 ) {
                    return -1;
                }
                TRACE_BUF("savedata", save_data, savelen);
            }
        }
    }
    // ����δд��flash���б��������
    s_gapk_sha_dealtail(add_shanum + exist_shanum, save_data, &savelen);
    // �������д�뵱ǰ�������Чsha�б�ֵ
    exist_shanum = exist_shanum + add_shanum;
    TRACE("add_shanum:%d, exist_shanum:%d\r\n", add_shanum, exist_shanum);
    s_sysinfo_gpakshainfo(1, &exist_shanum);
    return 0;
}
/* Descript:
 * ɾ��ָ��λ�����ݣ����Ѵ�����ĩβ��shaֵ����ɾ��λ�ý�����䣻
 * parrm: delete_shanum(1~256)    existlast_shanum(1~256)
 * */
int s_gapk_sha_deletail(uint32_t delete_shanum, uint32_t existlast_shanum)
{
    uint32_t deletepos_page;
    uint32_t existpos_page;
    uint8_t data_delete[512];
    uint8_t data_existlast[512];
    if ( existlast_shanum <= delete_shanum ) {
        // ���һ��sha��Ϊ��Ҫɾ����sha
        TRACE("param err or del sha is the last\r\n"); 
        return -1;
    }
    deletepos_page = (delete_shanum+15) / 16;   
    existpos_page = (existlast_shanum+15) / 16;   
    memset(data_delete, 0, sizeof(data_delete));
    memset(data_existlast, 0, sizeof(data_existlast));
    //��ȡɾ��ҳ����
    s_gapk_flash_read(deletepos_page-1, EXFLASH_PAGE_SIZE, data_delete);
    // ��ͬһҳֻҪ��ȡҳ�̶�ͨ�������滻�ķ�ʽ��������
    TRACE("delete_shanum:%d, existlast_shanum:%d\r\n", delete_shanum, existlast_shanum);
    if ( deletepos_page == existpos_page ) {
        memcpy(&data_delete[((delete_shanum-1)%16)*32],&data_delete[((existlast_shanum-1)%16)*32], 32);
    }else{
        // ����ͬһҳ����ȡβҳ���̶�ͨ���������һ�������滻��Ҫɾ����sha��������
        s_gapk_flash_read(existpos_page-1, 512, data_existlast);
        memcpy(&data_delete[((delete_shanum-1)%16)*32],&data_existlast[((existlast_shanum-1)%16)*32], 32);
//        memcpy(&data_delete[(delete_shanum%32-1)*32],&data_existlast[(existlast_shanum-1)*32], 32);
    }
    return s_gapk_flash_write(deletepos_page-1, 512, data_delete);
}
uint32_t s_gapk_delete_sha(uint32_t len)
{
    int ret;
    int i,j,z;
    uint8_t data0[512];
    uint8_t data1[512];
    uint8_t exist_data[512];
    uint16_t pagenum=0, exist_pagenum;
    uint32_t exist_shanum=0;    // �Ѿ�������ϵͳ���shaֵ
    int shalen=0, datalen=0, exist_datalen=0;
    shalen = len-sizeof(ST_newtaginfo)-16;
    pagenum = (shalen + EXFLASH_PAGE_SIZE - 1)/EXFLASH_PAGE_SIZE;
    s_sysinfo_gpakshainfo(0, &exist_shanum);
    // ���û�м�⵽�����ع�gpkg��,ֱ�ӷ���
    TRACE("get the pagenum:%d\r\n", pagenum);
    if ( (exist_shanum == 0) || (exist_shanum == 0xffffffff) ) {
       return 0; 
    }
    for ( i=0 ; i<pagenum ; i++ ) {
        if ( i==(pagenum-1) ) {
            datalen = shalen-((pagenum-1)*EXFLASH_PAGE_SIZE); 
        }else{
            datalen = EXFLASH_PAGE_SIZE;
        }
        exflash_read(EXFLASH_TMP_BUF_ADDR+16+EXFLASH_PAGE_SIZE*i, datalen, (uchar *)(&data1[0]));
        s_DesRSAPubk(1, datalen, &data1[0], &data0[0]);
        TRACE_BUF("data0", data0, datalen);
        for ( j=0 ; j<datalen/32 ; j++ ) {
            exist_pagenum = (exist_shanum +15)/16;  
            for ( z=0 ; z<exist_pagenum; z++ ) {
                if ( z==(exist_pagenum-1) ) {
                    exist_datalen = (exist_shanum*32)%EXFLASH_PAGE_SIZE ; 
                    if ( 0 == exist_datalen ) {
                        exist_datalen = EXFLASH_PAGE_SIZE;
                    }
//                    TRACE("\r\nreadlen:%d", exist_datalen);
                }else{
                    exist_datalen = EXFLASH_PAGE_SIZE;
                }
                memset( exist_data, 0, sizeof(exist_data) );
                s_gapk_flash_read(z, exist_datalen, exist_data);
//                TRACE("z:%d, exist_datalen:%d\r\n", z,exist_datalen);
//                TRACE_BUF("compare origin data", &data0[j*32], 32);
                ret = s_page_hit_sha(&data0[j*32], exist_data, exist_datalen);  
                if ( ret > 0 ) {
                    // ˵�����У�׼��ɾ��������ͨ�����һ����Ч���ݸ���
                    TRACE("\r\n*****hit the data:%d\r\n", z*16+ret);
                    s_gapk_sha_deletail(z*16+ret, exist_shanum);
                    exist_shanum --;
                    break;     
                }
            }
        }
        // д�뵱ǰʣ���sha����
        s_sysinfo_gpakshainfo(1, &exist_shanum);
    }
    return 0;
}
/* 
 * app_gapk_authenticate - 
 *  Descript: GAPK authenticate and save the legal data in the spiflash
 *  param�� ����ĳ��ȣ�Ϊexflash�������ĳ���ֵ�����ݶ�ȡ��ʼ��ַͳһΪexflash��ַ
 *  sha:SHAֵ
 *  tag:ST_newtaginfo�ṹ�� 
 * @ 
��	   ����	            ˵��
VER     4       Ĭ��Ϊ0
TYPE	4	    SHA���ͣ�Ĭ��0-SHA256
NUM	    4	    ������Կ֤��SHAֵ������֧�ֶ�GAPK��Կ֤��
SHA1    32	    ����SHA����ȷ�����ȣ�һ��Ϊ32	GAPK1��Կ֤��SHAֵ
SHA2	32      ����SHA����ȷ�����ȣ�һ��Ϊ32	GAPK2��Կ֤��SHAֵ
��.		��..
SHAn	32      ����SHA����ȷ�����ȣ�һ��Ϊ32	GAPKn��Կ֤��SHAֵ
TAG	    512	    ǩ����Ϣ�����ؼ��֡� GAPK_PKC0��
ע�⣺issuseindex������
1. issuseindex=��0��:��ʾ�滻���й�Կ֤��
2. issuseindex=��1��:��ʾ���ӹ�Կ֤��
3. issuseindex=��2��:��ʾɾ��ָ����Կ֤��
4. issuseindex=��3����ʾ������й�Կ֤��
*/
int app_gapk_authenticate(int len)
{
    int ret; 
    ST_newtaginfo stag;
//    SHA256Context sha;
    uint8_t data0[512];
    uint8_t data1[512];
    uint8_t project_id[32];
    uint32_t savenum=0; 
//    uint32_t optnum=0, savenum=0; 
//    uint16_t pagenum=0;
//    int shalen=0, datalen=0;
    int shalen=0;
    int i;
    
    // ����Ҫ����ver+type+num+rfu+tag
    if ( len < sizeof(ST_newtaginfo)+16 ) {
        TRACE("len err\r\n");
        return RET_PARAM;
    }
    // ����Ҫ����sha�����ı��� 
    shalen = len-sizeof(ST_newtaginfo)-16;
    if ( (shalen%32) != 0 ) {
        TRACE("len err sha not 32 mod %d\r\n", len);
        return RET_PARAM;
    }
    // ȡ��TAG���� get the tag
    memset(data0, 0, sizeof(data0));
    memset(data1, 0, sizeof(data1));
    exflash_read(EXFLASH_TMP_BUF_ADDR+len-sizeof(ST_newtaginfo), sizeof(ST_newtaginfo), (uint8_t *)(&stag.version[0]));
//    ptag = (ST_newtaginfo *)(&data[0]);

//    if ( memcmp(s_ptag->version, "GAPK_PKC", strlen("GAPK_PKC")) ) {
    if ( memcmp(&stag.version[0], TAG_FIRMWARE_GAPK_PKC, strlen(TAG_FIRMWARE_GAPK_PKC)) ) {
        TRACE("version err :%s\r\n", &stag.version[0]);
        return RET_PARAM;
    }
    
//  �Ƚ���ĿIDΪ�Ϸ�ID ��START_ITEP_SMART_POS_0	ͨ����ĿID 
//                       START_ITEP_SMART_POS_SAPK0	ʵ����ĿID    
//                       ����	������ĿID
    if ( memcmp(&stag.issue[0], TAG_PROJECTID_COMM, strlen(TAG_PROJECTID_COMM)) ) {
        memset(project_id, 0, sizeof(project_id));
        ret = (uint8_t)s_read_syszone(SYS_PROJIECT_ID_ADDR, SYS_PROJIECT_ID_SIZE, project_id);
        for ( i=0 ; i<32 ; i++ ) {
            if ( project_id[i] == 0xff  ) {
                continue; 
            }  
            break;
        }
        // ��δд�����ĿID
        if ( i==32 ) {
            TRACE("not be writern project id\r\n");
            return RET_ILLEAGLE_SERVICE;
        }
        if ( memcmp(&stag.issue[0], (char *)project_id, strlen((char *)project_id))) {
            TRACE("not correct project id\r\n");
            return RET_ILLEAGLE_SERVICE;
        }
        TRACE("get the project id:%s\r\n", project_id);
    }
    TRACE("download the project id:%s\r\n", &stag.issue[0]);
    
    // ����sha ����304(32+256+16)�ֽڲ�����shaУ��
    ret = s_Check_Sha(len, &stag);
    if ( ret != 0 ) {
        TRACE("check tag err\r\n");
        return RET_UNPACK_LEN;
    }

    // ������������
    exflash_read(EXFLASH_TMP_BUF_ADDR, 16, (uchar *)(&data1[0]));

    s_DesRSAPubk(1, 16, &data1[0], &data0[0]);
//    optnum = data0[8]|(data0[9]<<8)|(data0[10]<<16)|(data0[11]<<24); 
    TRACE("get the operate num:%d\r\n", data0[8]|(data0[9]<<8)|(data0[10]<<16)|(data0[11]<<24));
    // �������ݿ�ʼ, ���ݲ�ͬ�����������ݱ���
    switch ( stag.issuseindex )
    {
    case  TAG_GAPK_REPLACE:
        TRACE("\r\n********TAG_GAPK_REPLACE**********\r\n");
        ret = s_gapk_replace_sha(len);
        break;
    case  TAG_GAPK_ADD:
        TRACE("\r\n********TAG_GAPK_ADD**********\r\n");
        ret = s_gapk_add_sha(len);
        break;
    case  TAG_GAPK_DEL_SAME:
        TRACE("\r\n********TAG_GAPK_DEL_SAME**********\r\n");
        ret = s_gapk_delete_sha(len);
        break;
    case  TAG_GAPK_DEL_ALL:
        TRACE("\r\n********TAG_GAPK_DEL_ALL**********\r\n");
        savenum = 0;
        s_sysinfo_gpakshainfo(1, &savenum);
        break;
    default :
        return RET_PARAM;
    }
    return ret; 
}

/* 
 * app_gapk_authenticate - 
 *  Descript: GAPK authenticate and save the legal data in the spiflash
 *  sha:SHAֵ
 *  tag:ST_newtaginfo�ṹ�� 
 * @ 
��	   ����	            ˵��
VER	    4	            �汾��Ĭ��0
TYPE	4	            Ԥ����Ĭ��0
NUM	    4	            ������Կ֤��������Ŀǰֻ֧��1
RFU	    4	            Ԥ��
��Ŀ1��Կ	            ��Կ1�ṹ��	����1��Կ
��Ŀ1 ID	32B	        ��ĿID
��Ŀn��Կ	            ��Կn�ṹ��	����n��Կ
��Ŀn ID	32B	        ��ĿID
TAG	512	ǩ����Ϣ�����ؼ���" PKC0��
*/
//typedef struct
//{
//    uint32_t modlen;         // ��Կģ����
//    uint8_t mod[256];        // ��Կģ
//    uint8_t pubindex;        // ��Կָ��
//}s_publickey;

typedef struct
{
    R_RSA_PUBLIC_KEY pubkey;             // ��Կ
    uint8_t projectid[32];          // ��ĿID
}s_pubinfo;
//typedef struct
//{
//    uint32_t version;       // �汾��Ĭ��0
//    uint32_t type;        //Ԥ����Ĭ��0
//    uint32_t num;            //������Կ֤��������Ŀǰֻ֧��1
////    s_pubinfo* ppubinfo;      //���̹�Կ
//}s_sapk_head;

extern uint sys_write_project_id(uchar *id, int len);
int app_sapk_authenticate(int len)
{
    int ret; 
//    s_sapk_head *pspk_head;
//    s_pubinfo *ppubinfo;
//    uint8_t project_id[32];
    ST_newtaginfo stag;
//    SHA256Context sha;
    uint8_t data0[512];
    uint8_t data1[512];
    uint32_t optnum=0 ; 
//    uint16_t pagenum=0;
//    int pubkeylen, datalen=0;;
    int pubkeylen;
//    int i;

    if ( len < sizeof(ST_newtaginfo)+312 ) {
        TRACE("len err\r\n");
        return RET_PARAM;
    }
    // ����Ҫ����pubkey�����ı��� 
    pubkeylen = len-sizeof(ST_newtaginfo)-16;
    if ( (pubkeylen%sizeof(s_pubinfo)) != 0 ) {
        TRACE("len err pubinfo not correct mod\r\n");
        return RET_PARAM;
    }
    // ȡ��TAG���� get the tag
    memset(data0, 0, sizeof(data0));
    memset(data1, 0, sizeof(data1));
    exflash_read(EXFLASH_TMP_BUF_ADDR+len-sizeof(ST_newtaginfo), sizeof(ST_newtaginfo), (uint8_t *)(&stag.version[0]));
//    ptag = (ST_newtaginfo *)(&data[0]);
//    if ( memcmp(s_ptag->version, "GAPK_PKC", strlen("GAPK_PKC")) ) {
    if ( memcmp(&stag.version[0], TAG_FIRMWARE_SPKC0, strlen(TAG_FIRMWARE_SPKC0)) ) {
        TRACE("version err :%s\r\n", &stag.version[0]);
        return RET_PARAM;
    }

    ret = s_check_tag (0,&stag , stag.sha);
    if ( ret != 0 ) {
        TRACE("check tag err\r\n");
        return RET_UNPACK_LEN;
    }
// ����sha ����304(32+256+16)�ֽڲ�����shaУ��
    ret = s_Check_Sha(len, &stag);
    if ( ret != 0 ) {
        TRACE("check tag err\r\n");
        return RET_UNPACK_LEN;
    }
//    pagenum = (len-304+EXFLASH_PAGE_SIZE-1)/EXFLASH_PAGE_SIZE;
//    TRACE("sha pagenum:%d\r\n", pagenum);
//    sha256_init(&sha);
//    for ( i=0 ; i<pagenum ; i++ ) {
//        if ( i==(pagenum-1) ) {
//            datalen = len-304-((pagenum-1)*EXFLASH_PAGE_SIZE); 
//        }else{
//            datalen = EXFLASH_PAGE_SIZE;     
//        }
//        exflash_read(EXFLASH_TMP_BUF_ADDR+EXFLASH_PAGE_SIZE*i, datalen, (uchar *)(&data0[0]));
//        sha256_update(&sha, (uchar *)(&data0[0]), datalen);
//    }
//    sha256_final(&sha, data0);
//    TRACE_BUF("sha", data0, 32);
//    ret = s_check_firmware (0xff, data0 , (uchar *)&stag);
//    if ( ret != 0 ) {
//        TRACE("check tag err\r\n");
//        return RET_UNPACK_LEN;
//    }

    // ������������
    exflash_read(EXFLASH_TMP_BUF_ADDR, 16, (uchar *)(&data1[0]));

    s_DesRSAPubk(1, 16, &data1[0], &data0[0]);
//    TRACE_BUF("head en", data1, 16);
//    TRACE_BUF("head de", data0, 16);
    optnum = data0[8]|(data0[9]<<8)|(data0[10]<<16)|(data0[11]<<24); 
    TRACE("get the operate num:%d\r\n", optnum);
    // not allow bypass the 32 
    if ( optnum > 32 ) {
        return RET_PARAM; 
    }
/////////////////////////////////Ŀǰ������д��ĿID/////////////////////////////
    // ˵����δд����ĿID, ȡ�õ�һ��Ϊ��ĿID,�����汾����ĿID 
    exflash_read(EXFLASH_TMP_BUF_ADDR+16, sizeof(s_pubinfo), (uchar *)(&data1[0]));
    //        TRACE_BUF("data1", data1, 296);
    s_DesRSAPubk(1, sizeof(s_pubinfo), &data1[0], &data0[0]);
    //        TRACE_BUF("data0", data0, 296);
    ret = s_save_sys_sapk(data0);
    if ( ret ) {
        TRACE("save sapk err\r\n");
    }
    //        s_get_sys_sapk(data1);
    //        TRACE_BUF("data1", data1, 296);
    // ������ĿID
    ret = sys_write_project_id(&data0[FPOS(s_pubinfo, projectid)], 32);
    if ( ret ) {
        TRACE("sys_write_project_id err\r\n");
    }
///////////////////////////////////////////////////////////////////////////////
    /*
    // judge project ID  
    ret = (uint8_t)s_read_syszone(SYS_PROJIECT_ID_ADDR, SYS_PROJIECT_ID_SIZE, project_id);
    TRACE_BUF("ID", project_id, 32);
    for ( i=0 ; i<32 ; i++ ) {
       if ( project_id[i] == 0xff  ) {
           continue; 
       }  
       break;
    }
    TRACE("i:%d\r\n", i);
    if ( i==32 ) {
        // ˵����δд����ĿID, ȡ�õ�һ��Ϊ��ĿID,�����汾����ĿID 
        exflash_read(EXFLASH_TMP_BUF_ADDR+16, sizeof(s_pubinfo), (uchar *)(&data1[0]));
//        TRACE_BUF("data1", data1, 296);
        s_DesRSAPubk(1, sizeof(s_pubinfo), &data1[0], &data0[0]);
//        TRACE_BUF("data0", data0, 296);
        ret = s_save_sys_sapk(data0);
        if ( ret ) {
            TRACE("save sapk err\r\n");
        }
//        s_get_sys_sapk(data1);
//        TRACE_BUF("data1", data1, 296);
        // ������ĿID
        ret = sys_write_project_id(&data0[FPOS(s_pubinfo, projectid)], 32);
        if ( ret ) {
            TRACE("sys_write_project_id err\r\n");
        }
    }else{
        for ( i=0; i<optnum ; i++ ) {
            exflash_read(EXFLASH_TMP_BUF_ADDR+16+sizeof(s_pubinfo)*i, sizeof(s_pubinfo), (uchar *)(&data1[0]));
            s_DesRSAPubk(1, sizeof(s_pubinfo), &data1[0], &data0[0]);
            if ( memcmp(&data0[FPOS(s_pubinfo,projectid)], project_id, 32) ) {
                continue;
            }
            // ƥ����ĿID, ��������  
            ret = s_save_sys_sapk(data0);
            if ( ret ) {
                TRACE("save sapk err\r\n");
            }
            break;
        }
        // �ж��Ƿ�ƥ�� ����δ�ҵ���Ӧ����
        if ( i==optnum ) {
            TRACE("can not find correct project id\r\n");
            return RET_NOT_FOUND;
        }
    }
*/    
    return ret;
}

/* 
 * app_gapk_authenticate -
 *  Descript: ��Կ���ܣ����ݹ�Կ���� 
 *  sha:SHAֵ
 *  tag:ST_newtaginfo�ṹ�� 
 * @ 
��	   ����	            ˵��
VER	    4	            �汾��Ĭ��0
TYPE	4	            Ԥ����Ĭ��0
NUM	    4	            ������Կ֤��������Ŀǰֻ֧��1
��Ŀ1��Կ	            ��Կ1�ṹ��	����1��Կ
��Ŀ1 ID	32B	        ��ĿID
��Ŀn��Կ	            ��Կn�ṹ��	����n��Կ
��Ŀn ID	32B	        ��ĿID
TAG	512	ǩ����Ϣ�����ؼ���" PKC0��
*/

#endif


