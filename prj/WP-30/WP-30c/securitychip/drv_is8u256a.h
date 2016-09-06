#ifndef __DRV_IS8U256A_H__
#define __DRV_IS8U256A_H__

/**
 * \block:      Base defines
 * @{ */
/// ���ݶ���
//#define SC_HW_IS8U256A_SPI_ID   SPI_ID_2    //!< 
//#define SC_HW_IS8U256A_RST  PIOA_PIN_(12)   //!< ��λ����
#define SC_IS8U_GET_STATUS  0xE5        //!< ��ѯоƬ״̬
#define SC_IS8U_STATUS_BUSY 0x00        //!< busy
#define SC_IS8U_STATUS_IDLE 0x01        //!< idle
#define SC_IS8U_WRITE       0xC1        //!< ����
#define SC_IS8U_READ        0xA1        //!< ��ȡ
#define SC_IS8U_DUMMY       0xFF        //!< ���ֽڣ����ڶ�ȡ
#define SC_IS8U_APDU_CLA    0xBF        //!< SPI�ӿ�APDUָ� - ͷ�ַ�
/// ����
#define SC_IS8U_BUFFER_SIZE (256 + 16)  //!< ÿ���շ����ݵ���ʱ�����С
#define SC_SPI_BYTE_INTERVAL_US 120     //!< �ֽڼ���(us)
                                        //!<    ! ��ʱ��׼ȷ����Ҫ��Ϊ100����
                                        //!<    ! ����ͨ��
#define SC_IS8U_STATUS_WAIT_TIME 64     //!< ״̬�ȴ�������ÿ�εȴ�100us

typedef enum {                          //!< ����ֵ
    SC_RET_SUCCESS          = 0,        //!< �ɹ�
    SC_RET_PARAM_ERR        = -6901,    //!< ��������
    SC_IS8U_RET_BUSY        = -6902,    //!< оƬæ
    SC_IS8U_RET_UNKNOWN_STATUS = -6903, //!< оƬ״̬����
    SC_IS8U_RET_SPI_ERR     = -6904,    //!< SPI�շ�����
    SC_IS8U_RET_CLA_ERR     = -6905,    //!< CLA����
    SC_IS8U_RET_LRC_ERR     = -6906,    //!< ���ֵУ�����
    SC_IS8U_RET_INS_ERR     = -6907,    //!< INS����
    SC_IS8U_RET_P1_2_ERR    = -6908,    //!< P1��P2����
    SC_IS8U_RET_P3_ERR      = -6909,    //!< P3����
    SC_IS8U_RET_RUN_ERR     = -6910,    //!< �������ܴ���ִ��ʧ��
    SC_IS8U_RET_OUT_OFS     = -6911,    //!< ������ַԽ��
    SC_IS8U_RET_PROCESS_ERR = -6912,    //!< ���̴���
    SC_IS8U_RET_BAD_LRC     = -6913,    //!< �������ݵ����ֵ����ȷ
    SC_IS8U_RET_BAD_RESULT  = -6914,    //!< δ��ȡ����ȷ���
    SC_IS8U_RET_UNKNOWN     = -6915,    //!< δ֪����
}SC_IS8U_RET_E;
typedef struct {                        //!< SPI��������
    int spi_id;                         //!< SPI��� 0/1
    int spi_npcs;                       //!< NPCS
}SC_SPI_PARAMS_T;
typedef enum {                          //!< SM�ԳƼӽ���ģʽ
    SM_ECB_MODE,                        //!< ECB mode
    SM_CBC_MODE,                        //!< CBC mode
}SM_SYM_MODES_t;

/** @} */

//#if (defined(CFG_SECURITY_CHIP) && defined(CFG_SC_IS8U256A_SPI))
#if defined(CFG_SECURITY_CHIP)

/**
 * \block:      Base functions
 * @{ */
/**
 * \brief       оƬ��ʼ��
 * \return      0: Success  <0: Error
 */
int sc_spi_init(void);
/**
 * \brief       ����һ���ֽڣ�������һ���ֽ�
 * \param       byte        �����ֽ�����
 * \return      �����ֽ�����
 */
unsigned char sc_spi_rw_byte(unsigned char byte);
/**
 * \brief       �����ַ��������ط��͹����ж������ַ���
 * \param       data_in     ��������
 * \param       size        �������ݳ���
 * \param       data_out    ��������
 * \return      >=0: ���ݳ���  <0: Error
 */
int sc_spi_rw(unsigned char *buf_in, int size, unsigned char *buf_out);

/**
 * \brief       ��ȡоƬ״̬
 * \param       wait    0: ���ȴ�����  ~: �ȴ�����
 * \return      0: Idle     ~: Busy or Error
 */
int sc_is8u_spi_status(int wait);
/**
 * \brief       ��ȡ������
 * \param       data        �������
 * \return      >=0: Success    <0: Error
 */
int sc_is8u_get_fac_code(unsigned char * data);
/** @} */
/**
 * \block:      IS8U256A���⴦����
 * @{ */
/**
 * \brief       IS8U256A��ʼ������
 */
int sc_is8u256a_init(void);
/** @} */
/**
 * \block:      Crypto functions
 * @{ */
/**
 * \brief       ��ȡ�����
 * \param       len         ���������
 * \param       data        ��������
 * \return      >=0: Length     <0: Error
 */
int sc_is8u_get_random_data(int len, unsigned char * data);
/**
 * \brief       ��ȡ�Գ��㷨��HASH�㷨������
 * \param       len         ��ȡ����
 * \param       data        ��ȡ���ݻ���
 * \return      >=0: Length     <0: Error
 */
int sc_is8u_get_result_data(int len, unsigned char * data);
/**
 * \brief       SM1����
 * \param       mode        ����ģʽ���ο�SM_SYM_MODES_t
 * \param       dat_in      ��������
 * \param       len         ���ݳ��� (len=16*n; n>0)
 * \param       key         ��Կ, 16 byte
 * \param       iv          ��ʼ��������CBCģʽ��Ҫ, 16 byte
 * \param       dat_out     �������
 * \return      0:Success   <0:Error
 */
int sc_is8u_sm1_enc(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out);
/**
 * \brief       SM1����
 * \param       mode        ����ģʽ���ο�SM_SYM_MODES_t
 * \param       dat_in      ��������
 * \param       len         ���ݳ��� (len=16*n; n>0)
 * \param       key         ��Կ, 16 byte
 * \param       iv          ��ʼ��������CBCģʽ��Ҫ, 16 byte
 * \param       dat_out     �������
 * \return      0:Success   <0:Error
 */
int sc_is8u_sm1_dec(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out);


/**
 * \brief       ������Կ��
 * \return      0: Success  <0: Failed
 */
int sc_is8u_sm2_gen_keypair(void);
/**
 * \brief       ������Կ��
 * \param       pri_key     ˽Կ 32 byte
 * \param       pub_key     ��Կ 64 byte
 * \return      0: Success  <0: Failed
 */
int sc_is8u_sm2_import_keypair(unsigned char * pri_key, unsigned char * pub_key);
/**
 * \brief       ������Կ��
 * \param       pri_key     ˽Կ 32 byte
 * \param       pub_key     ��Կ 64 byte
 * \return      0: Success  <0: Failed
 */
int sc_is8u_sm2_export_keypair(unsigned char * pri_key, unsigned char * pub_key);
/**
 * \brief       SM2����
 * \param       pt_len      ���ĳ���
 * \param       pt          ��������
 * \param       ct          ����������ݻ���
 * \return      >0: ���ĳ���    <0: Failed
 */
int sc_is8u_sm2_enc(int pt_len, unsigned char * pt, unsigned char * ct);
/**
 * \brief       SM2����
 * \param       ct_len      �������ݳ���
 * \param       ct          ��������
 * \param       pt          ����������ݻ���
 * \return      >0: ���ĳ���    <0: Failed
 */
int sc_is8u_sm2_dec(int ct_len, unsigned char * ct, unsigned char * pt);
/**
 * \brief       SM2ǩ��
 * \param       id_len      ID����
 * \param       id          ID����
 * \param       pt_len      �������ݳ���
 * \param       pt          ��������
 * \param       digest      ժҪ�������
 * \param       sign        ǩ���������
 * \return      >0: ǩ�����ݳ���  <0: Failed
 */
int sc_is8u_sm2_sign(int id_len, unsigned char * id,
        int pt_len, unsigned char * pt, unsigned char * sign);
/**
 * \brief       SM2��ǩ - ��������ժҪֵ����ǩ
 * \param       id_len      id����
 * \param       id          id����
 * \param       pt_len      �������ݳ���
 * \param       pt          ��������
 * \param       sign        ����У���ǩ������
 * \return      0: Success  <0: Failed
 */
int sc_is8u_sm2_verify(int id_len, unsigned char * id,
        int pt_len, unsigned char * pt, unsigned char * sign);

/**
 * \brief       SHA1��ʼ��
 * \return      0: Success  -1: Failed
 */
inline int sc_is8u_sha1_init(void);
/**
 * \brief       SHA1���ݿ�����
 * \param       len         �������ݳ��ȣ��м���С�������0x40��ĩ��С��0x40
 * \param       data        ����
 * \return      >=0: ���ݳ���   <0: Error
 */
inline int sc_is8u_sha1_update(int len, unsigned char * data);
/**
 * \brief       SHA1�������
 * \param       digest      ժҪ����������
 * \return      >=0: ժҪ����   <0: Error
 */
inline int sc_is8u_sha1_finish(unsigned char * digest);
/**
 * \brief       SHA1
 * \param       len         �������ݳ���
 * \param       data        ����
 * \param       digest      ժҪ����������
 * \return      >=0: ժҪ����   <0: Error
 */
inline int sc_is8u_sha1(int len, unsigned char * data, unsigned char * digest);
/**
 * \brief       SHA256��ʼ��
 * \return      0: Success  -1: Failed
 */
inline int sc_is8u_sha256_init(void);
/**
 * \brief       SHA256���ݿ�����
 * \param       len         �������ݳ��ȣ��м���С�������0x40��ĩ��С��0x40
 * \param       data        ����
 * \return      >=0: ���ݳ���   <0: Error
 */
inline int sc_is8u_sha256_update(int len, unsigned char * data);
/**
 * \brief       SHA256�������
 * \param       digest      ժҪ����������
 * \return      >=0: ժҪ����   <0: Error
 */
inline int sc_is8u_sha256_finish(unsigned char * digest);
/**
 * \brief       SHA256
 * \param       len         �������ݳ���
 * \param       data        ����
 * \param       digest      ժҪ����������
 * \return      >=0: ժҪ����   <0: Error
 */
inline int sc_is8u_sha256(int len, unsigned char * data, unsigned char * digest);
/**
 * \brief       SM3��ʼ��
 * \return      0: Success  -1: Failed
 */
inline int sc_is8u_sm3_init(void);
/**
 * \brief       SM3���ݿ�����
 * \param       len         �������ݳ��ȣ��м���С�������0x40��ĩ��С��0x40
 * \param       data        ����
 * \return      >=0: ���ݳ���   <0: Error
 */
inline int sc_is8u_sm3_update(int len, unsigned char * data);
/**
 * \brief       SM3�������
 * \param       digest      ժҪ����������
 * \return      >=0: ժҪ����   <0: Error
 */
inline int sc_is8u_sm3_finish(unsigned char * digest);
/**
 * \brief       SM3
 * \param       len         �������ݳ���
 * \param       data        ����
 * \param       digest      ժҪ����������
 * \return      >=0: ժҪ����   <0: Error
 */
int sc_is8u_sm3(int len, unsigned char * data, unsigned char * digest);

/**
 * \brief       SM4 ����
 * \param       mode        ����ģʽ, SM_SYM_MODES_t
 * \param       dat_in      ����
 * \param       len         ���ݳ��� (len=16*n; n>0)
 * \param       key         ��Կ, 16 byte
 * \param       iv          ��ʼ��������CBCģʽ��Ҫ, 16 byte
 * \param       dat_out     ����
 * \return      0:Success   <0:Error
 */
int sc_is8u_sm4_enc(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out);
/**
 * \brief       SM4 ����
 * \param       mode        ����ģʽ, SM_SYM_MODES_t
 * \param       dat_in      ����
 * \param       len         ���ݳ��� (len=16*n; n>0)
 * \param       key         ��Կ, 16 byte
 * \param       iv          ��ʼ��������CBCģʽ��Ҫ, 16 byte
 * \param       dat_out     ����
 * \return      0:Success   <0:Error
 */
int sc_is8u_sm4_dec(int mode, unsigned char * dat_in, int len,
        unsigned char * key, unsigned char * iv, unsigned char * dat_out);
/** @} */

#if defined(DEBUG_Dx)
int sc_is8u_test(void);
#endif /* defined(DEBUG_Dx) */
#endif /* (defined(CFG_SECURITY_CHIP) && defined(CFG_SC_IS8U256A_SPI)) */
#endif /* __DRV_IS8U256A_H__ */

