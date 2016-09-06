#include "wp30_ctrl.h"
#include "drv_is8u256a.h"

#if defined(DEBUG_Dx)
#define DS_DEBUG_MAIN   0
#define DS_SC       (DBG_INFO /* | DBG_LABEL_FUNC | DBG_LABEL_LINE */)    //!< ���Կ���
#define DS_SC_DATA  (DBG_INFO /* | DBG_LABEL_FUNC | DBG_LABEL_LINE */)    //!< ���ݵ��Կ���
#define DS_SC_ERR   (DBG_INFO /* | DBG_LABEL_FUNC | DBG_LABEL_LINE */)    //!< �����������
#else
#define DS_DEBUG_MAIN   0
#endif /* defined(DEBUG_Dx) */
#include "libdebug.h"

//#if (defined(CFG_SECURITY_CHIP) && defined(CFG_SC_IS8U256A_SPI))
#if defined(CFG_SECURITY_CHIP)

static char * g_sc_result_list[][3] = { //!< ��������
    { "\x90\x00", "Success",        (char *)SC_RET_SUCCESS,  },
    { "\x6E\x00", "CLA Err",        (char *)SC_IS8U_RET_CLA_ERR,  },
    { "\x6D\x00", "INS Err",        (char *)SC_IS8U_RET_INS_ERR,  },
    { "\x6B\x00", "P1 or P2 Err",   (char *)SC_IS8U_RET_P1_2_ERR, },
    { "\x67\x00", "P3 Err",         (char *)SC_IS8U_RET_P3_ERR,   },
    { "\x68\x00", "Run Err",        (char *)SC_IS8U_RET_RUN_ERR,  },
    { "\x6A\x80", "Out offset",     (char *)SC_IS8U_RET_OUT_OFS,  },
    { "\xCC\xCC", "Process err",    (char *)SC_IS8U_RET_PROCESS_ERR, },
    { "\x6A\x00", "LRC Err",        (char *)SC_IS8U_RET_LRC_ERR,  },

    { "\xFF\xFF", "Unknown error",  (char *)SC_IS8U_RET_UNKNOWN,  },
};
typedef enum {                          //!< APDUָ����±�
    SC_SPI_APDU_RSA_GEN_KEY_STD,        //!< 0x10,RSA������Կ��STD-Mode,P1:Key-len
    SC_SPI_APDU_RSA_GEN_KEY_CRT,        //!< 0x10,RSA������Կ��CRT-Mode,P1:Key-len
    SC_SPI_APDU_RSA_ENC,                //!< 0x11,RSA����,P1:Key-len
    SC_SPI_APDU_RSA_DEC_STD,            //!< 0x12,RSA����STD-Mode,P1:Key-len
    SC_SPI_APDU_RSA_DEC_CRT,            //!< 0x12,RSA����CRT-Mode,P1:Key-len
    SC_SPI_APDU_RSA_SIGN_STD,           //!< 0x13,RSAǩ��STD-Mode,P1:Key-len
    SC_SPI_APDU_RSA_SIGN_CRT,           //!< 0x13,RSAǩ��CRT-Mode,P1:Key-len
    SC_SPI_APDU_RSA_VERIFY,             //!< 0x14,RSA��֤ǩ��,P1:Key-len
    SC_SPI_APDU_SM2_GEN_KEY,            //!< 0x20,SM2������Կ��
    SC_SPI_APDU_SM2_ENC_INIT,           //!< 0x22,SM2 Encrypt-��ʼ��
    SC_SPI_APDU_SM2_ENC_UPDATE,         //!< 0x22,SM2 Encrypt-�м������
    SC_SPI_APDU_SM2_ENC_END,            //!< 0x22,SM2 Encrypt-ĩ������
    SC_SPI_APDU_SM2_DEC_INIT,           //!< 0x22,SM2 Decrypt-��ʼ��
    SC_SPI_APDU_SM2_DEC_UPDATE,         //!< 0x22,SM2 Decrypt-�м������
    SC_SPI_APDU_SM2_DEC_END,            //!< 0x22,SM2 Decrypt-ĩ������
    SC_SPI_APDU_SM2_SIGN,               //!< 0x23,SM2ǩ��-��ǩ�����ݳ���������
    SC_SPI_APDU_SM2_VERIFY,             //!< 0x23,SM2��֤ǩ��-��ǩ�����ݳ���������
    SC_SPI_APDU_GET_VAL_Z,              //!< 0x24,��ȡZֵ
    SC_SPI_APDU_DES_ENC_ECB,            //!< 0x30,DES ECB Encrypt
    SC_SPI_APDU_DES_DEC_ECB,            //!< 0x30,DES ECB Decrypt
    SC_SPI_APDU_DES_ENC_CBC,            //!< 0x30,DES CBC Encrypt
    SC_SPI_APDU_DES_DEC_CBC,            //!< 0x30,DES CBC Decrypt
    SC_SPI_APDU_TDES_ENC_ECB,           //!< 0x31,TDES ECB Encrypt
    SC_SPI_APDU_TDES_DEC_ECB,           //!< 0x31,TDES ECB Decrypt
    SC_SPI_APDU_TDES_ENC_CBC,           //!< 0x31,TDES CBC Encrypt
    SC_SPI_APDU_TDES_DEC_CBC,           //!< 0x31,TDES CBC Decrypt
    SC_SPI_APDU_SM1_ENC_ECB,            //!< 0x32,SM1 ECB Encrypt
    SC_SPI_APDU_SM1_DEC_ECB,            //!< 0x32,SM1 ECB Decrypt
    SC_SPI_APDU_SM1_ENC_CBC,            //!< 0x32,SM1 CBC Encrypt
    SC_SPI_APDU_SM1_DEC_CBC,            //!< 0x32,SM1 CBC Decrypt
    SC_SPI_APDU_SM4_ENC_ECB,            //!< 0x33,SM4 ECB Encrypt
    SC_SPI_APDU_SM4_DEC_ECB,            //!< 0x33,SM4 ECB Decrypt
    SC_SPI_APDU_SM4_ENC_CBC,            //!< 0x33,SM4 CBC Encrypt
    SC_SPI_APDU_SM4_DEC_CBC,            //!< 0x33,SM4 CBC Decrypt
    SC_SPI_APDU_SHA1_INIT,              //!< 0x40,SHA1 ��ʼ��
    SC_SPI_APDU_SHA1_UPDATE,            //!< 0x40,SHA1 �м������
    SC_SPI_APDU_SHA1_END,               //!< 0x40,SHA1 ĩ������
    SC_SPI_APDU_SHA256_INIT,            //!< 0x41,SHA256 ��ʼ��
    SC_SPI_APDU_SHA256_UPDATE,          //!< 0x41,SHA256 �м������
    SC_SPI_APDU_SHA256_END,             //!< 0x41,SHA256 ĩ������
    SC_SPI_APDU_SM3_INIT,               //!< 0x42,SM3 ��ʼ��
    SC_SPI_APDU_SM3_UPDATE,             //!< 0x42,SM3 �м������
    SC_SPI_APDU_SM3_END,                //!< 0x42,SM3 ĩ������
    SC_SPI_APDU_IMPORT_DATA,            //!< 0x50,��������,P1:ofs_h,P2:ofs_l,P3:lenth
    SC_SPI_APDU_IMPORT_KEY,             //!< 0x51,������Կ,P1:ofs_h,P2:ofs_l,P3:lenth
    SC_SPI_APDU_EXPORT_DATA,            //!< 0x52,��������,P1:ofs_h,P2:ofs_l,P3:lenth
    SC_SPI_APDU_EXPORT_KEY,             //!< 0x53,������Կ,P1:ofs_h,P2:ofs_l,P3:lenth
    SC_SPI_APDU_CLEAN_CACHE,            //!< 0x54,�����ڴ滺����
    SC_SPI_APDU_GET_RANDOM_DATA,        //!< 0x55,��ȡ�����
    SC_SPI_APDU_GET_FAC_CODE,           //!< 0x56,��ȡ������,P3:0x0F(fix)
    SC_SPI_APDU_GET_SOFT_VER,           //!< 0x57,��ȡ����汾��
    SC_SPI_APDU_KEY_STORAGE,            //!< 0x5A,��Կ�洢
    SC_SPI_APDU_KEY_LOAD,               //!< 0x5B,��Կװ��
    SC_SPI_APDU_SOFT_RESET,             //!< 0x61,������λ
    SC_SPI_APDU_SOFT_LOWPOWER,          //!< 0x62,���õ͹���

    SC_SPI_APDU_TYPES_NUM,
}SC_IS8U_SPI_APDU_TYPES_t;
typedef enum {                          //!< APDUָ�����ݴ�������
    SC_APDU,                            //!< ������APDUָ��
    SC_APDU_READ,                       //!< ����ָ���ȡ����
    SC_APDU_WRITE,                      //!< ����ָ���������
}SC_IS8U_APDU_PORT_TYPES_t;
typedef struct {                        //!< ��ȫоƬIS8U��APDUָ���ṹ��
    unsigned char CLA;                  //!< CLA
    unsigned char INS;                  //!< INS
    unsigned char P1;                   //!< P1
    unsigned char P2;                   //!< P2
    unsigned char P3;                   //!< P3
    unsigned char port;                 //!< ���ݴ��䷽ʽ,SC_IS8U_APDU_PORT_TYPES_t
}SC_APDU_t;
static const SC_APDU_t g_sc_apdu_list[] = {   //!< APDUָ�����ݱ�
    { SC_IS8U_APDU_CLA, 0x10,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x10,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x11,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x12,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x12,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x13,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x13,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x14,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x20,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x22,   0,      0x00,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x22,   0,      0x01,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x22,   0,      0x02,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x22,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x22,   0,      0x81,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x22,   0,      0x82,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x23,   0x80,   0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x23,   0x80,   0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x24,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x30,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x30,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x30,   0x80,   0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x30,   0x80,   0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x31,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x31,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x31,   0x80,   0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x31,   0x80,   0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x32,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x32,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x32,   0x80,   0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x32,   0x80,   0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x33,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x33,   0,      0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x33,   0x80,   0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x33,   0x80,   0x80,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x40,   0,      0x00,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x40,   0,      0x01,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x40,   0,      0x02,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x41,   0,      0x00,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x41,   0,      0x01,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x41,   0,      0x02,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x42,   0,      0x00,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x42,   0,      0x01,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x42,   0,      0x02,   0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x50,   0,      0,      0,      SC_APDU_WRITE,  },
    { SC_IS8U_APDU_CLA, 0x51,   0,      0,      0,      SC_APDU_WRITE,  },
    { SC_IS8U_APDU_CLA, 0x52,   0,      0,      0,      SC_APDU_READ,   },
    { SC_IS8U_APDU_CLA, 0x53,   0,      0,      0,      SC_APDU_READ,   },
    { SC_IS8U_APDU_CLA, 0x54,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x55,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x56,   0,      0,      0x0F,   SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x57,   0,      0,      0,      SC_APDU_READ,   },
    { SC_IS8U_APDU_CLA, 0x5A,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x5B,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x61,   0,      0,      0,      SC_APDU,    },
    { SC_IS8U_APDU_CLA, 0x62,   0,      0,      0,      SC_APDU,    },
};

//static SC_SPI_PARAMS_T g_sc_is8u256a_params = {
//    SC_HW_IS8U256A_SPI_ID,
//    SPIBUS_IS8U256A,
//};
//
//static SC_SPI_PARAMS_T * g_sc_params = &g_sc_is8u256a_params;

/**
 * \block:      Other
 * @{ */
/// ��ȡ�������ֵ
static unsigned char sc_get_xor(unsigned char *buf, int length)
{
    int i;
    unsigned char lrc = 0;
    for(i = 0; i < length; i++) {
        lrc ^= buf[i];
    }
    return lrc;
}
/** @} */
/**
 * \block:      SPI control
 * @{ */
/// SPI��ʼ��
int sc_spi_init(void)
{
    SPI_InitDef spi_init;

    memset(&spi_init,0,sizeof(SPI_InitDef));
    spi_init.spix = IS8U256A_SPIn;
    memset(spi_init.pcs,PTxInvid,SPI_PCS_MAX);
    spi_init.pcs[IS8U256A_SPI_PCSn] = IS8U256A_PTxy_PCSn;
    spi_init.sck  = IS8U256A_PTxy_SCLK;
    spi_init.mosi = IS8U256A_PTxy_MOSI;
    spi_init.miso = IS8U256A_PTxy_MISO;
    spi_init.mode = SPI_MODE_MASTER;
    spi_init.TxFIFO_switch = FALSE;
    spi_init.RxFIFO_switch = FALSE;
    spi_init.TxCompleteIrq = FALSE;
    spi_init.TxQueueEndIrq = FALSE;
    spi_init.TxFIFO_UnderflowIrq = FALSE;
    spi_init.RxFIFO_OverflowIrq  = FALSE;
    spi_init.TxFIFO_FillIrq  = FALSE;
    spi_init.RxFIFO_DrainIrq = FALSE;
    spi_init.TxFIFO_IrqMode  = FALSE;
    spi_init.RxFIFO_IrqMode  = FALSE;
//    spi_init.p_asc  = 1; //����sck��cs���ӳ�ʱ�� ����Ҫ����3911spi�����쳣
//    spi_init.t_asc  = 1; //����sck��cs���ӳ�ʱ��
//    spi_init.p_csc  = 1; //����sckǰcs�������͵�ʱ��
//    spi_init.t_csc  = 0;
//    spi_init.t_dt  = 0;
//    spi_init.p_dt  = 0;

    //60M����Ƶ��12��Ƶ5M  IS8U256A����6M
    spi_init.attr = SCK_BR_DIV_6|SCK_PBR_DIV_2|MODE_MSBit
        |CPOL_CLK_LOW | CPHA_DATA_CAPTURED_LeadingEdge 
        |FRAME_SIZE_8;

    return hw_spi_init(&spi_init);
}
/// SPI��д�ֽ�
unsigned char sc_spi_rw_byte(unsigned char b)
{
    unsigned char r;
    r = hw_spi_master_WriteRead(IS8U256A_SPIn,b,IS8U256A_PCS_PIN,SPI_PCS_INACTIVE);
    s_DelayUs(SC_SPI_BYTE_INTERVAL_US); //!< �ֽڼ����������20us
    dbg_out_I(DS_SC_DATA, "Flag Write: %#02x; Read: %#02x", b, r);
    return r;
}
/// SPI��д����
int sc_spi_rw(unsigned char * buf_in, int size, unsigned char *buf_out)
{
    int i = 0;
    unsigned char tmp[SC_IS8U_BUFFER_SIZE];
    unsigned char * in = (buf_in == NULL ? tmp : buf_in);
    unsigned char * out = (buf_out == NULL ? tmp : buf_out);
    memset(tmp, SC_IS8U_DUMMY, sizeof(tmp));
    if((in == NULL && out == NULL) || size < 0) {
        dbg_out_E(DS_SC_ERR, "Param error! in:%#x; out:%#x; size:%#x",
                in, out, size);
        return SC_RET_PARAM_ERR;
    }
    dbg_out_I(DS_SC_DATA, "=> SPI; Write size: %d", size);
    dbg_dmp_HC(DS_SC_DATA, (char *)in, size);
    //IS8U256A spiͨѶ���Ե��ֽڿ���cs Ҳ������spiͨѶ�г���Ƭѡ��Ч
#if 0
    for(i = 0; i < (size-1); i++) {
        out[i] = hw_spi_master_WriteRead(IS8U256A_SPIn,in[i],IS8U256A_PCS_PIN,SPI_PCS_ASSERTED);
        s_DelayUs(SC_SPI_BYTE_INTERVAL_US); //!< �ֽڼ����������20us
    }
    out[i] = hw_spi_master_WriteRead(IS8U256A_SPIn,in[i],IS8U256A_PCS_PIN,SPI_PCS_INACTIVE);
    i++;
    s_DelayUs(SC_SPI_BYTE_INTERVAL_US); //!< �ֽڼ����������20us
#else
    for(i = 0; i < size; i++) {
        out[i] = hw_spi_master_WriteRead(IS8U256A_SPIn,in[i],IS8U256A_PCS_PIN,SPI_PCS_INACTIVE);
        s_DelayUs(SC_SPI_BYTE_INTERVAL_US); //!< �ֽڼ����������20us
    }
#endif
    dbg_out_I(DS_SC_DATA, "<= SPI; Read size: %d", size);
    dbg_dmp_HC(DS_SC_DATA, (char *)(buf_out == NULL ? tmp : buf_out), size);
    return i;
}
/** @} */
/**
 * \block:      SPI protocol
 * @{ */
/// ����д��־
static int sc_is8u_spi_write_flag(unsigned char len)
{
    int ret = 0;
    unsigned char c[2] = { SC_IS8U_WRITE, len, };
    if((ret = sc_spi_rw(c, 2, NULL)) != 2) {
        dbg_out_E(DS_SC_ERR, "Send write flag failed!(%d)", ret);
        return ret;
    }
    return 0;
}
/// ���Ͷ���־
static int sc_is8u_spi_read_flag(void)
{
    int ret = 0;
    unsigned char c[2] = { SC_IS8U_READ, SC_IS8U_DUMMY, };
    unsigned char r[2];
    if((ret = sc_spi_rw(c, 2, r)) != 2) {
        dbg_out_E(DS_SC_ERR, "Send read flag failed!(%d)", ret);
        return ret;
    }
    if(ret == 0xCC) {
        dbg_out_W(DS_SC_ERR, "It may be a bad lenth: %#x", ret);
    }
    return (int)r[1];
}
/// ��ȡоƬ״̬��wait == trueʱ�ȴ�״̬����
int sc_is8u_spi_status(int wait)
{
    int ret = 0;
    int i = 0;
    unsigned char c[2] = { SC_IS8U_GET_STATUS, SC_IS8U_DUMMY, };
    unsigned char r[2];
    for(i = 0; i < SC_IS8U_STATUS_WAIT_TIME; i++) {
        if((ret = sc_spi_rw(c, 2, r)) != 2) {
            dbg_out_E(DS_SC_ERR, "Send check status failed!(%d)", ret);
            return ret;
        }
        if(r[1] == SC_IS8U_STATUS_IDLE || wait == 0) {
            break;
        }
        dbg_out_I(DS_SC_DATA, "Wait status... (%d)", i);
        s_DelayMs(10);                  //!< �ȴ��������
    }
    switch(r[1]) {
        case SC_IS8U_STATUS_IDLE: return 0;
        case SC_IS8U_STATUS_BUSY: return SC_IS8U_RET_BUSY;
        default: {
            dbg_out_W(DS_SC_ERR, "Unknown status: %#x", r[1]);
            return SC_IS8U_RET_UNKNOWN_STATUS;
        }
    }
}
/// д���� - ����IS8U��SPIͨ��Э��
static int sc_is8u_spi_write(int size, unsigned char * data)
{
    int ret = 0;
    if(size < 0 || size > 0xff) {
        dbg_out_E(DS_SC_ERR, "Param error!(%#x)", size);
        return SC_RET_PARAM_ERR;
    }
    if((ret = sc_is8u_spi_status(1))) {
        dbg_out_E(DS_SC_ERR, "Wait status failed!(%d)", ret);
        return ret;
    }
    if((ret = sc_is8u_spi_write_flag((char)(size + 1))) < 0) {
        dbg_out_E(DS_SC_ERR, "Bad lenth: %d", ret);
        return ret;
    }
    if((ret = sc_spi_rw(data, size, NULL)) != size) {
        dbg_out_E(DS_SC_ERR, "Write data failed!(%d)", ret);
        return ret;
    }
    /// ����LRC
    unsigned char lrc = (sc_get_xor(data, size) ^ (char)(size + 1));
    if((ret = sc_spi_rw(&lrc, 1, NULL)) != 1) {
        dbg_out_E(DS_SC_ERR, "Write LRC failed!(%d)", ret);
        return ret;
    }
    return size;
}
/// ������ - ����IS8U��SPIͨ��Э�飬���س���
static int sc_is8u_spi_read(unsigned char *data)
{
    int ret = 0;
    int len_out = 0;
    if((ret = sc_is8u_spi_status(1))) {
        dbg_out_E(DS_SC_ERR, "Wait status failed!(%d)", ret);
        return ret;
    }
    if((ret = sc_is8u_spi_read_flag()) < 0) {
        dbg_out_E(DS_SC_ERR, "Bad lenth: %d", ret);
        return ret;
    }
    len_out = ret;
    if((ret = sc_spi_rw(NULL, len_out, data)) != len_out) {
        dbg_out_E(DS_SC_ERR, "Read data failed!(%d)", ret);
        return ret;
    }
    len_out--;                          //!< ȥ��LRC��1�ֽڳ���
    /// У��LRC
    unsigned char lrc = data[len_out];
    if(lrc != (sc_get_xor(data, len_out) ^ (char)(len_out + 1))) {
        dbg_out_E(DS_SC_ERR, "Read failed! LRC error!(%d != %d)",
                lrc, (sc_get_xor(data, len_out) ^ (char)(len_out + 1)));
        return SC_IS8U_RET_BAD_LRC;
    }
    return len_out;
}
/// ��ȡ�������
static int sc_is8u_spi_result(unsigned char * res)
{
    int i = 0;
//    int ret = 0;
    unsigned char dat[0x200];
    unsigned char * r = dat;
    if(res) {
        r = res;
    }
    else if( sc_is8u_spi_read(r) != 2) {
        dbg_out_W(DS_SC_ERR, "Read failed | Bad result!(%d)", sc_is8u_spi_read(r));
        return SC_IS8U_RET_BAD_RESULT;
    }
    for(i = 0; i < ((sizeof(g_sc_result_list) / sizeof(g_sc_result_list[0]))
                - 1); i++) {
        if(memcmp(g_sc_result_list[i][0], r, 2) == 0) {
            break;
        }
    }
    if(g_sc_result_list[i][2] != SC_RET_SUCCESS) {
        dbg_out_W(DS_SC_ERR, "Get bad result: %s(%d, %#02x%#02x)",
                g_sc_result_list[i][1], g_sc_result_list[i][2], r[0], r[1]);
    }
    return (int)(g_sc_result_list[i][2]);
}
/** @} */
/**
 * \block:      APDU protocol
 * @{ */
/// ��д���� - ����IS8U��APDUָ�����ȡʱdata�ռ���Ҫ���ڵ���256byte
static int sc_is8u_apdu_rw(SC_APDU_t * apdu, unsigned char * data)
{
    int ret = 0;
    int len = 0;
    unsigned char tmp[SC_IS8U_BUFFER_SIZE] = { 0 };
    if(apdu == NULL || (apdu->port != SC_APDU && data == NULL)) {
        dbg_out_E(DS_SC_ERR, "Param error: apdu: %#x data: %#x", apdu, data);
        return SC_RET_PARAM_ERR;
    }
    /// ���ݴ���
    switch(apdu->port) {
        case SC_APDU_WRITE: {
            memmove(tmp, apdu, 5);
            if(apdu->P3) {
                memmove(tmp + 5, data, apdu->P3);
            }
            if((len = sc_is8u_spi_write(5 + apdu->P3, tmp)) < 0) {
                dbg_out_E(DS_SC_ERR, "Write failed! (%d)", len);
                return len;
            }
            /// ��ȡ����ֵ
            if((ret = sc_is8u_spi_result(NULL)) < 0) {
                dbg_out_E(DS_SC_ERR, "Result error!(%d)", ret);
                return ret;
            }
            return len - 5;     //!< ��ȥAPDUͷ�ַ�����
        }
        case SC_APDU_READ: {
            if((ret = sc_is8u_spi_write(5, (unsigned char *)apdu)) != 5) {
                return ret;
            }
            if((len = sc_is8u_spi_read(tmp)) < 0) {
                dbg_out_E(DS_SC_ERR, "Read failed!(%d)", len);
                return len;
            }
            /// ��ȡ����ֵ
            len -= 2;           //!< ��ȥSW(����ֵ)����, 2 byte
            if((ret = sc_is8u_spi_result(tmp + len)) < 0) {
                dbg_out_E(DS_SC_ERR, "Result error!(%d)", ret);
                return ret;
            }
            memmove(data, tmp, len);
            return len;
        }
        case SC_APDU: {
            /// ����APDU
            if((ret = sc_is8u_spi_write(5, (unsigned char *)apdu)) < 0) {
                return ret;
            }
            /// ��ȡ����ֵ
            if((ret = sc_is8u_spi_result(NULL)) < 0) {
                dbg_out_E(DS_SC_ERR, "Result error!(%d)", ret);
                // return ret;      //!< ?ERR? SM2ǩ������SM3ʱ���󣬵������ȷ
            }
            return 0;
        }
        default: {
            dbg_out_E(DS_SC_ERR, "APDU port type error!");
            return -SC_RET_PARAM_ERR;
        }
    }
}
/// ��д����
static int sc_is8u_data_rw(int apdu_id, unsigned char *data)
{
    return sc_is8u_apdu_rw((SC_APDU_t *)&g_sc_apdu_list[apdu_id], data);
}
/// ��д���� �Զ������ֵ
static int sc_is8u_data_rw_p(int apdu_id, unsigned char p1, unsigned char p2,
        unsigned char p3, unsigned char *data)
{
    SC_APDU_t apdu;
    memmove(&apdu, &g_sc_apdu_list[apdu_id], sizeof(SC_APDU_t));
    if(p1) {
        apdu.P1 = p1;
    }
    if(p2) {
        apdu.P2 = p2;
    }
    if(p3) {
        apdu.P3 = p3;
    }
    return sc_is8u_apdu_rw(&apdu, data);
}
/** @} */
/**
 * \block:      IS8U256A���⴦����
 * @{ */
/// WP70V2.0����оƬ��Ӧ�ĸ�λ���߿���
static void sc_is8u256a_hw_reset(void)
{
//    int pin = SC_HW_IS8U256A_RST;
//    set_pio_perigh(pin, 1, 0); // ����Ϊ��������
//    set_pio_output(pin, 0);
//    sys_delay_ms(200);
//    set_pio_output(pin, 1);
//    sys_delay_ms(200);

    GPIO_InitTypeDef gpio_init;

    gpio_init.GPIOx = IS8U256A_GPIO_RST;
    gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_Out_DSH;
    gpio_init.PORT_Pin = IS8U256A_PINx_RST;
    gpio_init.GPIO_Pinx_Mode = GPIO_OUTPUT(IS8U256A_PINx_RST);
    hw_gpio_init(&gpio_init);
    hw_gpio_reset_bits(IS8U256A_GPIO_RST,1<<IS8U256A_PINx_RST);
    s_DelayMs(10); 
    hw_gpio_set_bits(IS8U256A_GPIO_RST,1<<IS8U256A_PINx_RST);
    s_DelayMs(50);
//    gpio_init.GPIOx = IS8U256A_GPIO_SHAKEHAND;
//    gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_Out_DSH;
//    gpio_init.PORT_Pin = IS8U256A_PINx_SHAKEHAND;
//    gpio_init.GPIO_Pinx_Mode = GPIO_OUTPUT(IS8U256A_PINx_SHAKEHAND);
//    hw_gpio_init(&gpio_init);
//    hw_gpio_reset_bits(IS8U256A_GPIO_SHAKEHAND,1<<IS8U256A_PINx_SHAKEHAND);
//    sys_delay_ms(200);
}
/// IS8U256A��ʼ������
int sc_is8u256a_init(void)
{
    int ret;
    unsigned char r[16];
    sc_spi_init();
    sc_is8u256a_hw_reset();
    /// ���оƬ״̬
    unsigned char chk_st[6] = { 0xFB, 0x33, 0x44, 0x55, 0x66, 0xBF, };
    if((ret = sc_spi_rw(chk_st, 6, NULL)) != 6) {
        dbg_out_E(DS_SC_ERR, "Send check status failed!(%d)", ret);
        return ret;
    }
    if((ret = sc_spi_rw(NULL, 4, r)) != 4) {
        dbg_out_E(DS_SC_ERR, "Read version failed!(%d)", ret);
        return ret;
    }
    if(sc_get_xor(r, 3) != r[3]) {
        dbg_out_E(DS_SC_ERR, "Check status failed! LRC error(%d != %d)",
                sc_get_xor(r, 3), r[3]);
        return SC_IS8U_RET_BAD_LRC;
    }
    dbg_out_H(DS_SC, "Get version: %d.%d.%d", r[0], r[1], r[2]);
    /// �趨ͨ��ģʽΪ״̬��ģʽ
    unsigned char chg_mode[] = { 0xFB, 0x00, 0x44, 0x00, 0x66, 0xD9, };
    if((ret = sc_spi_rw(chg_mode, 6, r)) != 6) {
        dbg_out_E(DS_SC_ERR, "Send change mode failed!(%d)", ret);
        return ret;
    }
    if((ret = sc_spi_rw(NULL, 3, r)) != 3) {
        dbg_out_E(DS_SC_ERR, "Read SW failed!(%d)", ret);
        return ret;
    }
    if(sc_get_xor(r, 2) != r[2]) {
        dbg_out_E(DS_SC_ERR, "Change mode failed! LRC error(%d != %d)",
                sc_get_xor(r, 2), r[2]);
        return SC_IS8U_RET_BAD_LRC;
    }
    if((ret = sc_is8u_spi_result(r))) {
        dbg_out_E(DS_SC_ERR, "Result error!(%d)", ret);
        return ret;
    }
    return 0;
}
/** @} */
/**
 * \block:      Functions
 * @{ */
/// ��������
static int sc_is8u_data_import(int len, int ofs, unsigned char * data)
{
    int ret = sc_is8u_data_rw_p(SC_SPI_APDU_IMPORT_DATA, (ofs >> 8) & 0xFF,
            ofs & 0xFF, (unsigned char)len, data);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "Import data failed(%d)!", ret);
    }
    return ret;
}
/// ������Կ
static int sc_is8u_key_import(int len, int ofs, unsigned char * key)
{
    int ret = sc_is8u_data_rw_p(SC_SPI_APDU_IMPORT_KEY, (ofs >> 8) & 0xFF,
            ofs & 0xFF,(unsigned char)len, key);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "Import key failed(%d)!", ret);
    }
    return ret;
}
/// �������ݲ����س���
static int sc_is8u_data_export(int len, int ofs, unsigned char * data)
{
    int ret = sc_is8u_data_rw_p(SC_SPI_APDU_EXPORT_DATA, (ofs >> 8) & 0xFF,
            ofs & 0xFF,(unsigned char)len, data);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "Export data failed(%d)!", ret);
    }
    return ret;
}
/// ������Կ�����س���
static int sc_is8u_key_export(int len, int ofs, unsigned char * key)
{
    int ret = sc_is8u_data_rw_p(SC_SPI_APDU_EXPORT_KEY, (ofs >> 8) & 0xFF,
            ofs & 0xFF,(unsigned char)len, key);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "Export key failed(%d)!", ret);
    }
    return ret;
}
/// �������
int sc_is8u_clean_cache(void)
{
    int ret = sc_is8u_data_rw(SC_SPI_APDU_CLEAN_CACHE, NULL);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "Clean cache failed(%d)!", ret);
    }
    return ret;
}

/// ��ȡ������
int sc_is8u_get_fac_code(unsigned char * data)
{
    int ret = 0;
    dbg_out_I(DS_SC, "IS8U - Get factory code.");
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_GET_FAC_CODE, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "Get factory code failed(%d)!", ret);
        return ret;
    }
    return sc_is8u_data_export(g_sc_apdu_list[SC_SPI_APDU_GET_FAC_CODE].P3,
            0, data);
}
/// ��ȡ�����
int sc_is8u_get_random_data(int len, unsigned char * data)
{
    int ret = 0;
    dbg_out_I(DS_SC, "IS8U - Get random data, len: %d", len);
    if((ret = sc_is8u_data_rw_p(SC_SPI_APDU_GET_RANDOM_DATA, 0, 0,
                    (unsigned char)len, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "Get random data failed(%d)!", ret);
        return ret;
    }
    return sc_is8u_data_export(len, 0, data);
}

typedef struct {                        //!< SM�ԳƼ��ܺ��������ṹ��
    char * algorithm;                   //!< �㷨��
    int apdu_id;                        //!< APDU���
    unsigned char * data_in;            //!< ��������
    int data_len;                       //!< ���ݳ���, ����Ϊ16*n, n=[1,15]
    int mode;                           //!< �ӽ���ģʽ��SM1N4_MODES_t
    unsigned char * key;                //!< ��Կ, 16 byte
    unsigned char * iv;                 //!< CBCģʽ��Ҫ, 16 byte
    unsigned char * data_out;           //!< �������
}SC_SM_SYM_PARAMS_t;
/// SM symmetric algorithm process
static int sc_is8u_sm_sym(SC_SM_SYM_PARAMS_t * p)
{
    int ret = 0;

    if(p->data_in == NULL || p->data_out == NULL || p->key == NULL
            || p->data_len % 16 || (p->mode == SM_CBC_MODE && p->iv == NULL)) {
        dbg_out_E(DS_SC_ERR, "Param error: dat_in: %#x; dat_out: %#x; key: %#x"
                "len: %#x(len=16*n); mode: %d; iv: %#x",
                p->data_in, p->data_out, p->key, p->data_len, p->mode, p->iv);
        return SC_RET_PARAM_ERR;
    }

    if((ret = sc_is8u_key_import(16, 0, p->key)) < 0) {
        dbg_out_E(DS_SC_ERR, "Import key failed!(%s, %d)", p->algorithm, ret);
        return ret;
    }
    if(p->mode == SM_CBC_MODE && (ret = sc_is8u_key_import(16, 16, p->iv)) < 0){
        dbg_out_E(DS_SC_ERR, "Import IV failed!(%s, %d)", p->algorithm, ret);
        return ret;
    }
    int len_t = p->data_len;
    unsigned char * data_in_t = p->data_in;
    unsigned char * data_out_t = p->data_out;
    while(len_t > 0) {
        int len = len_t < 240 ? len_t : 240;
        if((ret = sc_is8u_data_import(len, 0, data_in_t)) < 0) {
            return ret;
        }
        if((ret = sc_is8u_data_rw(p->apdu_id, NULL)) < 0) {
            dbg_out_E(DS_SC_ERR, "Calculate failed!(%s, %d)", p->algorithm, ret);
            return ret;
        }
        if((ret = sc_is8u_data_export(len, 0x0200, data_out_t)) < 0) {
            dbg_out_E(DS_SC_ERR, "Export data failed!(%s, %d)", p->algorithm, ret);
            return ret;
        }
        data_in_t += len;
        data_out_t += len;
        len_t -= len;
    }

    return 0;
}
/// SM1 Encrypt
int sc_is8u_sm1_enc(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out)
{
    SC_SM_SYM_PARAMS_t p = { "SM1", 0, dat_in, len, mode, key, iv, dat_out, };
    switch(mode) {
        case SM_ECB_MODE: {
            p.apdu_id = SC_SPI_APDU_SM1_ENC_ECB;
            break;
        }
        case SM_CBC_MODE: {
            p.apdu_id = SC_SPI_APDU_SM1_ENC_CBC;
            break;
        }
        default: {
            dbg_out_E(DS_SC_ERR, "Unknown mode: %#x", mode);
            return SC_RET_PARAM_ERR;
        }
    }
    return sc_is8u_sm_sym(&p);
}
/// SM1 Decrypt
int sc_is8u_sm1_dec(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out)
{
    SC_SM_SYM_PARAMS_t p = { "SM1", 0, dat_in, len, mode, key, iv, dat_out, };
    switch(mode) {
        case SM_ECB_MODE: {
            p.apdu_id = SC_SPI_APDU_SM1_DEC_ECB;
            break;
        }
        case SM_CBC_MODE: {
            p.apdu_id = SC_SPI_APDU_SM1_DEC_CBC;
            break;
        }
        default: {
            dbg_out_E(DS_SC_ERR, "Unknown mode: %#x", mode);
            return SC_RET_PARAM_ERR;
        }
    }
    return sc_is8u_sm_sym(&p);
}
/// SM4 Encrypt
int sc_is8u_sm4_enc(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out)
{
    SC_SM_SYM_PARAMS_t p = { "SM4", 0, dat_in, len, mode, key, iv, dat_out, };
    switch(mode) {
        case SM_ECB_MODE: {
            p.apdu_id = SC_SPI_APDU_SM4_ENC_ECB;
            break;
        }
        case SM_CBC_MODE: {
            p.apdu_id = SC_SPI_APDU_SM4_ENC_CBC;
            break;
        }
        default: {
            dbg_out_E(DS_SC_ERR, "Unknown mode: %#x", mode);
            return SC_RET_PARAM_ERR;
        }
    }
    return sc_is8u_sm_sym(&p);
}
/// SM4 Decrypt
int sc_is8u_sm4_dec(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out)
{
    SC_SM_SYM_PARAMS_t p = { "SM4", 0, dat_in, len, mode, key, iv, dat_out, };
    switch(mode) {
        case SM_ECB_MODE: {
            p.apdu_id = SC_SPI_APDU_SM4_DEC_ECB;
            break;
        }
        case SM_CBC_MODE: {
            p.apdu_id = SC_SPI_APDU_SM4_DEC_CBC;
            break;
        }
        default: {
            dbg_out_E(DS_SC_ERR, "Unknown mode: %#x", mode);
            return SC_RET_PARAM_ERR;
        }
    }
    return sc_is8u_sm_sym(&p);
}

/// Digest
typedef struct {                        //!< ժҪ���㺯�������ṹ��
    char * algorithm;                   //!< �㷨��
    int block_size;                     //!< ���ݿ��С
    int apdu_init;                      //!< apdu��� - ��ʼ��
    int apdu_update;                    //!< apdu��� - ��������
    int apdu_end;                       //!< apdu��� - ����
    int out_len;                        //!< ���ժҪ����
}SC_DIGEST_PARAMS_t;
typedef enum {                          //!< ժҪ�㷨����
    SC_DIGEST_SHA1,                     //!< SHA1
    SC_DIGEST_SHA256,                   //!< SHA256
    SC_DIGEST_SM3,                      //!< SM3
}SC_DIGEST_TYPES_E;
static const SC_DIGEST_PARAMS_t g_digest_params[] = {   //!< ��ժҪ�㷨����
    { "SHA1",   0x40,   SC_SPI_APDU_SHA1_INIT,      SC_SPI_APDU_SHA1_UPDATE,
        SC_SPI_APDU_SHA1_END,   0x14, },
    { "SHA256", 0x40,   SC_SPI_APDU_SHA256_INIT,    SC_SPI_APDU_SHA256_UPDATE,
        SC_SPI_APDU_SHA256_END, 0x20, },
    { "SM3",    0x40,   SC_SPI_APDU_SM3_INIT,       SC_SPI_APDU_SM3_UPDATE,
        SC_SPI_APDU_SM3_END,    0x20, },
};
static char g_sc_digest_update_flag = 0; //!< ����Update�������
int sc_is8u_digest_init(int digest_id)
{
    int ret = sc_is8u_data_rw(g_digest_params[digest_id].apdu_init, NULL);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "Init failed!(%s, %d)",
                g_digest_params[digest_id].algorithm, ret);
        return ret;
    }
    g_sc_digest_update_flag = 1;
    return 0;
}
int sc_is8u_digest_update(int digest_id, int len, unsigned char * data)
{
    int ret = 0;
    if(len > g_digest_params[digest_id].block_size * 3 || len < 0) {
        dbg_out_E(DS_SC_ERR, "Param error: len:%d", len);
        return SC_RET_PARAM_ERR;
    }
    if(len % g_digest_params[digest_id].block_size == 0) {
        if((ret = sc_is8u_data_import(len, 0, data)) < 0) {
            dbg_out_E(DS_SC_ERR, "Import data failed!(%s, %d)",
                    g_digest_params[digest_id].algorithm, ret);
            return ret;
        }
        if((ret = sc_is8u_data_rw(g_digest_params[digest_id].apdu_update,
                        NULL)) < 0) {
            dbg_out_E(DS_SC_ERR, "Update data failed!(%s, %d)",
                    g_digest_params[digest_id].algorithm, ret);
            return ret;
        }
    }
    else if(len < g_digest_params[digest_id].block_size) {
        g_sc_digest_update_flag = 0;
        if((ret = sc_is8u_data_import(len, 0, data)) < 0) {
            dbg_out_E(DS_SC_ERR, "Import last data failed!(%s, %d)",
                    g_digest_params[digest_id].algorithm, ret);
            return ret;
        }
    }
    else {
        dbg_out_E(DS_SC_ERR,
                "Param error: len:%d (len=64*n;n=[1,3];lastblock:len<64)", len);
        return SC_RET_PARAM_ERR;
    }
    return 0;
}
int sc_is8u_digest_finish(int digest_id, unsigned char * digest)
{
    int ret = 0;
    if(g_sc_digest_update_flag) {
        g_sc_digest_update_flag = 0;
        if((ret = sc_is8u_data_import(0, 0, (unsigned char *)&ret)) < 0) {
            dbg_out_E(DS_SC_ERR, "Import last data failed!(%s, %d)",
                    g_digest_params[digest_id].algorithm, ret);
            return ret;
        }
    }
    if((ret = sc_is8u_data_rw(g_digest_params[digest_id].apdu_end, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "Digest end failed!(%s, %d)",
                g_digest_params[digest_id].algorithm, ret);
        return ret;
    }
    return sc_is8u_data_export(g_digest_params[digest_id].out_len,
            0x0200, digest);
}
int sc_is8u_digest(int digest_id, int len, unsigned char * data,
        unsigned char * digest)
{
    int i = 0;
    int ofs = 0;
    int ret = 0;
    if((ret = sc_is8u_digest_init(digest_id)) < 0) {
        return ret;
    }
    for(i = 0; ofs < len; i++) {
        int blocksize = (len - ofs) > g_digest_params[digest_id].block_size
            ? g_digest_params[digest_id].block_size : (len - ofs);
        if((ret = sc_is8u_digest_update(digest_id, blocksize, data + ofs)) < 0){
            return ret;
        }
        ofs += blocksize;
    }
    return sc_is8u_digest_finish(digest_id, digest);
}
/// SHA1
inline int sc_is8u_sha1_init(void) {
    return sc_is8u_digest_init(SC_DIGEST_SHA1);
}
inline int sc_is8u_sha1_update(int len, unsigned char * data) {
    return sc_is8u_digest_update(SC_DIGEST_SHA1, len, data);
}
inline int sc_is8u_sha1_finish(unsigned char * digest) {
    return sc_is8u_digest_finish(SC_DIGEST_SHA1, digest);
}
inline int sc_is8u_sha1(int len, unsigned char * data, unsigned char * digest) {
    return sc_is8u_digest(SC_DIGEST_SHA1, len, data, digest);
}
/// SHA256
inline int sc_is8u_sha256_init(void) {
    return sc_is8u_digest_init(SC_DIGEST_SHA256);
}
inline int sc_is8u_sha256_update(int len, unsigned char * data) {
    return sc_is8u_digest_update(SC_DIGEST_SHA256, len, data);
}
inline int sc_is8u_sha256_finish(unsigned char * digest) {
    return sc_is8u_digest_finish(SC_DIGEST_SHA256, digest);
}
inline int sc_is8u_sha256(int len, unsigned char * data, unsigned char * digest){
    return sc_is8u_digest(SC_DIGEST_SHA256, len, data, digest);
}
/// SM3
inline int sc_is8u_sm3_init(void) {
    return sc_is8u_digest_init(SC_DIGEST_SM3);
}
inline int sc_is8u_sm3_update(int len, unsigned char * data) {
    return sc_is8u_digest_update(SC_DIGEST_SM3, len, data);
}
inline int sc_is8u_sm3_finish(unsigned char * digest) {
    return sc_is8u_digest_finish(SC_DIGEST_SM3, digest);
}
//inline ������iar��Ҫ����h�ļ��б��� ��������c�ļ����û���벻ͨ��
int sc_is8u_sm3(int len, unsigned char * data, unsigned char * digest) {
    return sc_is8u_digest(SC_DIGEST_SM3, len, data, digest);
}

/// General SM2 key pair
int sc_is8u_sm2_gen_keypair(void)
{
    int ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_GEN_KEY, NULL);
    if(ret < 0) {
        dbg_out_E(DS_SC_ERR, "General failed!(%d)", ret);
        return ret;
    }
    return 0;
}
/// Import SM2 key pair
int sc_is8u_sm2_import_keypair(unsigned char * pri_key, unsigned char * pub_key)
{
    int ret;
    if(pub_key && (ret = sc_is8u_key_import(0x40, 0, pub_key)) < 0) {
        dbg_out_E(DS_SC_ERR, "Import public key failed!(%d)", ret);
        return ret;
    }
    if(pri_key && (ret = sc_is8u_key_import(0x20, 0x40, pri_key)) < 0) {
        dbg_out_E(DS_SC_ERR, "Import private key failed!(%d)", ret);
        return ret;
    }
    return 0;
}
/// Export SM2 key pair
int sc_is8u_sm2_export_keypair(unsigned char * pri_key, unsigned char * pub_key)
{
    int ret;
    if(pub_key && (ret = sc_is8u_key_export(0x40, 0, pub_key)) < 0) {
        dbg_out_E(DS_SC_ERR, "Export public key failed!(%d)", ret);
        return ret;
    }
    if(pri_key && (ret = sc_is8u_key_export(0x20, 0x40, pri_key)) < 0) {
        dbg_out_E(DS_SC_ERR, "Export private key failed!(%d)", ret);
        return ret;
    }
    return 0;
}
/// SM2 Encrypt
int sc_is8u_sm2_enc(int pt_len, unsigned char * pt, unsigned char * ct)
{
    int i;
    int ret;
    int ct_len = 0;
    int blocks = pt_len / 0x80; //(pt_len % 0x80) ? (pt_len / 0x80) : ((pt_len / 0x80) - 1);
    int left = pt_len - blocks * 0x80;
    /// 1. Encrypt init
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_ENC_INIT, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 encrypt initial failed!(%d)", ret);
        return ret;
    }
    /// 2. Export C1 �������ܽ��C1����
    if((ret = sc_is8u_data_export(0x40, 0xA0, ct)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export C1 failed!(%d)", ret);
        return ret;
    }
    ct_len += 0x40;
    for(i = 0; i < blocks; i++) {
        /// 3. Import plain text(128 byte)
        if((ret = sc_is8u_data_import(0x80, 0, pt + 0x80 * i)) < 0) {
            dbg_out_E(DS_SC_ERR, "SM2 import plaintext failed!(%d)", ret);
            return ret;
        }
        /// 4. Encrypt update
        if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_ENC_UPDATE, NULL)) < 0) {
            dbg_out_E(DS_SC_ERR, "SM2 encrypt update failed!(%d)", ret);
            return ret;
        }
        /// 5. Export C2 �����м����ݿ�������
        if((ret = sc_is8u_data_export(0x80, 0xE0, ct + ct_len)) < 0) {
            dbg_out_E(DS_SC_ERR, "SM2 export C2 failed!(%d)", ret);
            return ret;
        }
        ct_len += 0x80;
    }
    /// 6. Import final plain text(<128 byte)
    if((ret = sc_is8u_data_import(left, 0, (pt + 0x80 * i))) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 import final plaintext failed!(%d)", ret);
        return ret;
    }
    /// 7. Encrypt final
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_ENC_END, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 final encrypt update failed!(%d)", ret);
        return ret;
    }
    /// 8. Export final cypher text
    if(left && (ret = sc_is8u_data_export(left, 0xE0, ct + ct_len)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export final C2 failed!(%d)", ret);
        return ret;
    }
    ct_len += left;
    /// 9. Export C3
    if((ret = sc_is8u_data_export(0x20, 0x0180, ct + ct_len)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export C3 failed!(%d)", ret);
        return ret;
    }
    ct_len += 0x20;

    return ct_len;
}
/// SM2 Decrypt
int sc_is8u_sm2_dec(int ct_len, unsigned char * ct, unsigned char * pt)
{
    int i;
    int ret;
    int len = ct_len - 0x60;
    int blocks = len / 0x80; //(len % 0x80) ? (len / 0x80) : ((len / 0x80) - 1);
    int left = len - blocks * 0x80;
    int pt_len = 0;
    /// 1. Import C1
    if((ret = sc_is8u_data_import(0x40, 0xA0, ct)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export C1 failed!(%d)", ret);
        return ret;
    }
    /// 2. Decrypt init
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_DEC_INIT, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 decrypt initial failed!(%d)", ret);
        return ret;
    }
    ct += 0x40;
    for(i = 0; i < blocks; i++) {
        /// 3. Import C2(128 byte)
        if((ret = sc_is8u_data_import(0x80, 0xE0, ct)) < 0) {
            dbg_out_E(DS_SC_ERR, "SM2 import C2 failed!(%d)", ret);
            return ret;
        }
        ct += 0x80;
        /// 4. Decrypt update
        if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_DEC_UPDATE, NULL)) < 0) {
            dbg_out_E(DS_SC_ERR, "SM2 decrypt update failed!(%d)", ret);
            return ret;
        }
        /// 5. Export plain text �����м����ݿ�������
        if((ret = sc_is8u_data_export(0x80, 0, pt + pt_len)) < 0) {
            dbg_out_E(DS_SC_ERR, "SM2 export plaintext failed!(%d)", ret);
            return ret;
        }
        pt_len += 0x80;
    }
    ct += left;
    /// 7. Import C3
    if((ret = sc_is8u_data_import(0x20, 0x0180, ct)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export C3 failed!(%d)", ret);
        return ret;
    }
    ct -= left;
    /// 6. Import final cypher text(<128 byte)
    if((ret = sc_is8u_data_import(left, 0xE0, ct)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 import final cyphertext failed!(%d)", ret);
        return ret;
    }
    /// 8. Decrypt final
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_DEC_END, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 final decrypt update failed!(%d)", ret);
        return ret;
    }
    /// 9. Export final plain text
    if(left && (ret = sc_is8u_data_export(left, 0, pt + pt_len)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export final plaintext failed!(%d)", ret);
        return ret;
    }
    pt_len += left;

    return pt_len;
}
/// SM2 get SM3 digest
static int sc_is8u_sm2_sm3(int id_len, unsigned char * id, int pt_len,
        unsigned char * pt)
{
    int ret;
    if(id_len > 32 || id_len < 0) {
        dbg_out_E(DS_SC_ERR, "Param error! id_len:%d, id:%#x, pt:%#x",
                id_len, id, pt);
        return SC_RET_PARAM_ERR;
    }
    /// 1. Import ID
    if((ret = sc_is8u_key_import(id_len, 0x60, id)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 import ID failed!(%d)", ret);
        return ret;
    }
    /// 2. Get Z
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_GET_VAL_Z, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 get Z failed!(%d)", ret);
        return ret;
    }
    /// 3~9. SM3 process
    int ofs = 0x20;
    int blocksize = (pt_len < ofs) ? pt_len : ofs;
    unsigned char tmp[0x40];
    /// SM3 init
    if((ret = sc_is8u_sm3_init()) < 0) {
        return ret;
    }
    /// Update first block
    if((ret = sc_is8u_data_import(blocksize, 0x20, pt)) < 0) {
        dbg_out_E(DS_SC_ERR, "Import data failed!(%s, %d)",
                g_digest_params[SC_DIGEST_SM3].algorithm, ret);
        return ret;
    }
    if(blocksize == 0x20) {
        if((ret = sc_is8u_data_rw(g_digest_params[SC_DIGEST_SM3].apdu_update,
                        NULL)) < 0) {
            dbg_out_E(DS_SC_ERR, "Update data failed!(%s, %d)",
                    g_digest_params[SC_DIGEST_SM3].algorithm, ret);
            return ret;
        }
    }
    /// Updating...
    while(ofs < pt_len) {
        blocksize = ((pt_len - ofs) > g_digest_params[SC_DIGEST_SM3].block_size)
            ? g_digest_params[SC_DIGEST_SM3].block_size : (pt_len - ofs);
        if((ret = sc_is8u_sm3_update(blocksize, pt + ofs)) < 0) {
            return ret;
        }
        ofs += blocksize;
    }
    /// Finish
    if((ret = sc_is8u_sm3_finish(tmp)) < 0) {
        return ret;
    }
    /// Import digest
    if((ret = sc_is8u_data_import(0x40, 0x0200, tmp)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 import digest failed!(%d)", ret);
        return ret;
    }
    return 0;
}
/// SM2 Sign
int sc_is8u_sm2_sign(int id_len, unsigned char * id,
        int pt_len, unsigned char * pt, unsigned char * sign)
{
    int ret = sc_is8u_sm2_sm3(id_len, id, pt_len, pt);
    if(ret < 0) {
        return ret;
    }
    /// 10. Sign
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_SIGN, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 sign failed!(%d)", ret);
        return ret;
    }
    /// 11~12. Export sign R&S
    if((ret = sc_is8u_data_export(0x40, 0x0220, sign)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 export sign value failed!(%d)", ret);
        return ret;
    }
    return 0x40;
}
/// SM2 Verify
int sc_is8u_sm2_verify(int id_len, unsigned char * id,
        int pt_len, unsigned char * pt, unsigned char * sign)
{
    int ret = sc_is8u_sm2_sm3(id_len, id, pt_len, pt);
    if(ret < 0) {
        return ret;
    }
    /// 10~11. Import sign value R&S
    if((ret = sc_is8u_data_import(0x40, 0x0220, sign)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 import sign value failed!(%d)", ret);
        return ret;
    }
    /// 12. Verify
    if((ret = sc_is8u_data_rw(SC_SPI_APDU_SM2_VERIFY, NULL)) < 0) {
        dbg_out_E(DS_SC_ERR, "SM2 verify failed!(%d)", ret);
        return ret;
    }
    return 0;
}

/** @} */

#endif /* (defined(CFG_SECURITY_CHIP) && defined(CFG_SC_IS8U256A_SPI)) */

