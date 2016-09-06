/***************** (h) COPYRIGHT 2016 START Computer equipment *****************
 * File Name          : pinpad_parser.h
 * bfief              : 
 * Author             : luocs 
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 4/24/2016 1:53:38 PM
 * Description        : 
 *******************************************************************************/
#ifndef __PINPAD_PARSER_H__
#define __PINPAD_PARSER_H__ 


struct PED_SYS {
    uint8_t status;  //�Ƿ��� 0-���� 1-����,��ʾ��ͬ��ȫ�ȼ�  FF-���𴥷�����ʹ���������
    uint8_t curr_index_mk;//current MK index:0-��Ч
    uint8_t curr_index_wk;//current WK index:0-��Ч
    uint8_t curr_type_wk; //current WK type :0-��Ч
    uint8_t rand[24];     //������Կ�������
};

typedef struct __OFFLINE_KEYINFO
{
	uint8_t keylen; //��Կ����
    uint8_t indatalen;  //�������ݳ���
    uint8_t keybuf[24]; //��Կ���24
}_OFFLINE_KEYINFO;


/* 
 *    mode:0-����������Կ  1-��������Կ
 *    00(2B)+Type(��Կ����,1B) + Index(��Կ����,1B) + KEKType(������Կ����,1B)+KEKIndex (������Կ����,1B)
 *    + EncMode(����ģʽ,1B) + CheckMode(У��ģʽ,1B) +checklen(У���볤��,1B)+ KeyLen(��Կ����,1B) 
 *    + Key_E (��Կ,�䳤) + Check(У���룬�䳤) +IV(��ʼ�������䳤) 
 *    Encode����ģʽ:  WK 0-������ 1-����
 *                   ��WK 0-����   1-���� 
 *    CheckModeУ��ģʽ: 0-��У��  1-������Կ��8�ֽ�0x00��3DES���ܵó����
 *    checklenУ���볤��:  4-4�ֽ� 8-8�ֽ�
 * @ 
 */
struct PEDLoadKeyHead {
    uint16_t ver;
    uint8_t type;
    uint8_t index;
    uint8_t kektype;
    uint8_t kekindex;
    uint8_t algr;
    uint8_t encmode;
    uint8_t checkmode;
    uint8_t checklen;
    uint8_t ivlen;
    uint8_t keylen;
    uint8_t keyval[8];//minlen
};

struct PEDOnlinePinHead {
    uint8_t type;
    uint8_t index;
    uint8_t mode;
    uint8_t minlen;
    uint8_t maxlen;
    uint8_t timeout;
    uint8_t showlenflag;
    uint8_t beepflag;
    char    cardno[12];
    char* string;//minlen:�ַ���ʽ
};

struct PEDOfflinePinHead {
    uint8_t mode;
    uint8_t minlen;
    uint8_t maxlen;
    uint8_t timeout;
    uint8_t showlenflag;
    uint8_t beepflag;
    uint16_t rfu;
    uint8_t encmode;
    char    cardno[12];
    char* string;//minlen:�ַ���ʽ
};


#define PED_PIN_LEN_MIN    (uint8_t)0
#define PED_PIN_LEN_MAX    (uint8_t)14
#define PED_PIN_TOUT_MIN   (uint8_t)30
#define PED_PIN_TOUT_MAX   (uint8_t)120

struct PEDEncrDataHead {
    uint8_t type;
    uint8_t index;
    uint8_t algr;
    uint8_t scatter;
    uint8_t rfu[2];
    uint8_t len_l;
    uint8_t len_h;
    uint8_t val[8];//minlen
};

struct PEDCalcuMACHead {
    uint8_t type;
    uint8_t index;
    uint8_t algr;
    uint8_t scatter;
    uint8_t mode; //0-X99 1-ECB 2-X919 
    uint8_t flag; //1-���к����� 0-���1�飬���ڼ���MAC�ı��ĳ���1024Bʱʹ��
    uint8_t rfu[3];
    uint8_t ivlen;//��ʼ��������
    uint8_t len_l;
    uint8_t len_h;
    uint8_t val[8];//minlen
};


struct PEDSelectKeyCHead {
    uint8_t mode_l;
    uint8_t mode_h;
    uint8_t type;
    uint8_t index;
};


// key type
#define KEY_TYPE_SRK            0x00  ////Security Root Key(MMK)
#define KEY_TYPE_AUTHMK		    0x01
#define KEY_TYPE_AUTHPINK		0x02
#define KEY_TYPE_AUTHMACK		0x03
#define KEY_TYPE_AUTHDESK		0x04
#define KEY_TYPE_MK				0x05
#define KEY_TYPE_PINK			0x06
#define KEY_TYPE_MACK			0x07
#define KEY_TYPE_DESK			0x08
#define KEY_TYPE_TK             0x09   //������Կ
#define KEY_TYPE_PROCESSKEY     0x0C   //������Կ
#define KEY_TYPE_CURKEY         0x0D   //��ǰ��Կ
#define KEY_TYPE_RSAKEY         0x0E

enum PED_ALGR {
    PED_ALGR_DES_ENC = 1,
    PED_ALGR_DES_DEC,
    PED_ALGR_AES_ENC, 
    PED_ALGR_AES_DEC, 
    PED_ALGR_SM1_ENC, 
    PED_ALGR_SM1_DEC, 
    PED_ALGR_SM4_ENC, 
    PED_ALGR_SM4_DEC, 
    PED_ALGR_RSA = 0x20, 
    PED_ALGR_SM2 = 0x21, 
};



//Return Value:RTV
typedef enum
{
    RTV_PED_SUCCE = 0,
    RTV_PED_ERR,
    RTV_PED_ERR_CMD,
    RTV_PED_ERR_PARA,
    RTV_PED_ERR_MACK,
    RTV_PED_ERR_ALGR,
    RTV_PED_ERR_USAGE_FAULT,
    RTV_PED_ERR_KEYLEN,
    RTV_PED_ERR_KEYTYPE,
    RTV_PED_ERR_KEYINDEX,
    RTV_PED_ERR_KEYNULL,
    RTV_PED_ERR_KEYFLASH,
    RTV_PED_ERR_KEYCHECK,
    RTV_PED_ERR_INPUT_NONE,
    RTV_PED_ERR_INPUT_CANCEL,
    RTV_PED_ERR_INPUT_TIMEOUT,
}PED_RTV;

enum PED_CMD {
    PED_CMD_OPEN      = 1,
    PED_CMD_CLOSE     = 2, 
    PED_CMD_LOADMK    = 3, 
    PED_CMD_LOADWK    = 4, 
    PED_CMD_RANDOM    = 5, 
    PED_CMD_ONLINE    = 6, 
    PED_CMD_OFFLINE   = 7, 
    PED_CMD_ENCRDATA  = 8, 
    PED_CMD_MAC       = 9, 
    PED_CMD_SELECTKEY = 0x0A, 
    PED_CMD_AUTHKEY   = 0x0B, 
    PED_CMD_SHAKEHADN = 0x0D, 
    PED_CMD_EXTEND    = 0x12, 
};

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

int ped_init(uint32_t mode);
int ped_open(uint32_t mode);
int ped_close(uint32_t mode);
int ped_load_mk(uint32_t length,uint8_t *protocol);
int ped_load_wk(uint32_t length,uint8_t *protocol);
int ped_get_rand(uint32_t inlen,uint8_t *input,uint16_t *outlen,uint8_t *output);
int ped_get_pin_online(uint32_t inlen,uint8_t *packet,uint16_t *outlen,uint8_t *output);
int ped_get_pin_offline(uint32_t inlen,uint8_t *input,uint16_t *outlen,uint8_t *output);
int ped_get_encrpt_data(uint32_t inlen,uint8_t *packet,uint16_t *outlen,uint8_t *output);
int ped_get_mac(uint32_t inlen,uint8_t *packet,uint16_t *outlen,uint8_t *output);
int ped_select_key(uint32_t inlen, uint8_t *input);
int ped_extern_main(uint32_t inlen,uint8_t *input,uint16_t *outlen,uint8_t *output);

int pubk_get_blk_num(uint8_t index);
int ped_parse_cmd(uint32_t inlen,uint8_t *input,uint32_t *outlen,uint8_t *output);

#endif


