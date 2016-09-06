/***************** (C) COPYRIGHT 2015 START Computer equipment *****************
 * File Name          : local_dl.h
 * bfief              : 
 * Author             : luocs() 
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 1/13/2015 9:54:45 AM
 * Description        : 
 *******************************************************************************/
#ifndef __LOCAL_DL_H__
#define __LOCAL_DL_H__ 

#define DL_HANDSHAKE_BOOT           'B'
#define DL_HANDSHAKE_CTRL           'C'
#define DL_HANDSHAKE_BOOT_REPLY     'b'
#define DL_HANDSHAKE_CTRL_REPLY     'c'

#define DLCMD_SETBUAD               0x9001      //  �������ز�����
#define DLCMD_GETTERMINFO           0x9002      //  ��ȡ�ն���Ϣ
#define DLCMD_SETTIME               0x9003      //  ����ϵͳʱ��
#define DLCMD_SETSN                 0x9004      //  �������к�
#define DLCMD_FORMATFS              0x9005      //  ��ʽ���ļ�ϵͳ
#define DLCMD_GETAPPINFO            0x9006      //  ��ȡ�ն�Ӧ����Ϣ
#define DLCMD_DELFILE               0x9007      //  ɾ���ļ�
#define DLCMD_LOADFILE              0x9008      //  ���������ļ�
#define DLCMD_TRANSDATA             0x9009      //  ��������
#define DLCMD_SAVEFILE              0x900A      //  �����ļ�
#define DLCMD_FINISH                0x900B      //  ��������
#define	DLCMD_FAC_CTRL				0x900C		//	�����ܿ�
#define DLCMD_GETBININFO            0x9050      //  ��ȡ�����ļ�����
#define DLCMD_GETBINDATA            0x9055      //  ��ȡ�����ļ�



#define DL_LOCAL_STX          (uint8_t)0x02
#define DL_LOCAL_STXLEN       (uint8_t)0x01
#define DL_LOCAL_CRCLEN       (uint8_t)0x02
#define DL_LOCAL_FIXEDLEN     (uint8_t)0x06 //cmdcode[2B]+return code[2B]+length[2B]

#define DL_LOCAL_FRAME_SIZE   (DL_CFG_PACKLEN+9)//cmd=0x9009 add 9Bytes

//typedef __packed struct _LOCAL_DL_DEF
struct LocalDlFrame
{
    uint8_t stx;
    uint8_t cmd[2];
    uint8_t recode[2]; //return-code
    uint8_t length[2];
    uint8_t bin[DL_LOCAL_FRAME_SIZE+2];
    uint16_t bin_len;
};

//  �ļ�����
#define     FILE_TYPE_BOOTLOADER    0x80    //  BOOT LOADER���
#define     FILE_TYPE_BOOT          0x81    //  BOOT���
#define     FILE_TYPE_CTRL          0x82    //  CTRL���
#define     FILE_TYPE_FONT          0xA0    //  �ֿ��ļ�

#define     FILE_TYPE_APP           0x01    //  Ӧ�ó���
#define     FILE_TYPE_DATA          0x03    //  Ӧ�ó���������ļ�
#define     FILE_PINPAD_APP         0x04    //  Ӧ�ó����Ӧ��������̳���

#define     FILE_CHECK_LEN   4 

#define     LOCALDL_FINSH_TIMEOUT   (3*1000)

struct LocalDlCallBack 
{
    int (*open_comport)(uint8_t commport,int bps,uint8_t *buffer,uint32_t buffer_size);
    int (*close_comport)(uint8_t commport);
    int (*check_buffer)(uint8_t commport);
    int (*clear_buffer)(uint8_t commport);
    int (*read_data)(uint8_t commport,uint8_t *output,uint32_t length,int t_out_ms);
    int (*write_data)(uint8_t commport,uint8_t const *input,uint32_t length);
    int (*save_data)(void *data);
    int (*save_sn)(uint8_t *sn,uint8_t length);
    int (*get_terminal_info)(struct LocalDlFrame *frame);
    int (*after_done)(void *data);
    int (*cancel)(void);
    int (*display)(int mode,void *data);
    int (*fac_ctrl)(struct LocalDlFrame *frame);
};

typedef union _LOCALDL_CONTRL
{
    uint8_t byte;
    struct {
        uint8_t chagebps:1;   //1-chage bps
        uint8_t finish:1;     //
        uint8_t finish_timer:1;    //
        uint8_t ctrl_update:1;     //
        uint8_t boot_update:1;     //
        uint8_t rfu_1:1;     //reserver for future
        uint8_t rfu_2:1;     //
        uint8_t rfu_3:1;     //
    }bit;
}LOCALDL_CONTRL;

#define lOCALDL_COMPORT_NUM  1
struct LocalDlOpt
{
    uint8_t *serialbuf[lOCALDL_COMPORT_NUM];
    uint32_t seriallen[lOCALDL_COMPORT_NUM];
    struct LocalDlFrame *frame;
    const struct LocalDlCallBack *callback; 
    uint32_t bps;
    uint32_t filelen;
    uint32_t request;//������������
    char *title;
//    uint8_t file_chk[FILE_CHECK_LEN];

    LOCALDL_CONTRL contrl;
    uint8_t dllevel; //��������ģʽ:C:ctrl B:boot
    uint8_t commindex;
    uint8_t commbuf[lOCALDL_COMPORT_NUM];
    uint8_t percent;
    uint16_t dlrecode;
};



//return-code
#define EAPP_ERROR             4001     //  ʧ��
#define EAPP_UNKNOWNCMD        4002     //  ���֧��
#define EAPP_CHECKSUM          4003     //  ���ݰ�У���
#define EAPP_FILECHECK         4004     //  �ļ�У���
#define EAPP_WRITE             4005     //  д��ʧ��
#define EAPP_NOSPACE           4006     //  �ռ䲻��
#define EAPP_MAXAPP            4007     //  Ӧ�ø����ﵽ���ֵ
#define EAPP_MAXFILE           4008     //  �ļ������ﵽ���ֵ
#define EAPP_NOAPP             4009     //  �Ҳ���Ӧ��
#define EAPP_UNKNOWNAPP        4010     //  Ӧ�ø�ʽ����ʶ��
#define EAPP_BAUDERR           4011     //  �����ʲ�֧��
#define EAPP_CLOCK             4012     //  ʱ�Ӵ���
#define EAPP_TIMEOUT           4013     //  ��ʱ
#define EAPP_FINISH            4014
#define EAPP_DOWNLOAD          4015
#define EAPP_MEMERY        	   4016		//�ڴ治��
#define EAPP_NOTRSAKEY         4017
#define EAPP_RSAKEYCHECK       4018

#define LOCALDL_ERROR          ((int)-1)
#define LOCALDL_PARA           ((int)-2)
#define LOCALDL_TIMEOUT        ((int)-3)
#define LOCALDL_CANCEL         ((int)-4)
#define LOCALDL_CHECK          ((int)-5)
#define LOCALDL_SEND           ((int)-6)
#define LOCALDL_RECV           ((int)-7)


void localdl_form_reurncode(struct LocalDlFrame *frame,uint16_t recode);
uint8_t localdl_com_open(struct LocalDlOpt *localdl);
int localdl_process(struct LocalDlOpt *localdl);
void localdl_com_close(struct LocalDlOpt *localdl);

#endif

