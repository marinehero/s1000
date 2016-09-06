/*
 * =====================================================================================
 *
 *       Filename:  ctc_emv.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  4/11/2016 10:06:14 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */
#ifndef __CTC_EMV_H__
#define __CTC_EMV_H__ 

#include "ParseCmd.h"
#include "../emv/emv.h"

/*Ӧ��ѡ�����ݽṹ
 *SelType:
 *[0] - �Ȱ�֧��ϵͳ����(PSE) ������ѡ�б����ʧ�ܵ�����ת��AIDѡ���ٰ���AID�б�ʽ������ѡ�б�����ʹ�ø�ֵ��������..
 *[1] - ͨ��PSE��ʽ������ѡ�б�
 *[2] - ͨ��AID�б�ʽ������ѡ�б�
 *
 *���ȣ�8
 */
typedef struct APP_SELECT_DATA_STRUCT
{
    unsigned char SelType;   //Ӧ��ѡ��ʽ
    DCardSt CardSt;          //��������
}ctc_emv_asd;

//���ȷ��Ӧ�����ݽṹ
typedef struct APP_FINAL_DATA_STRUCT
{
    unsigned char AidNo;       //����ѡ���AID���
}ctc_emv_afd;

//Ӧ�ó�ʼ�����ݽṹ
typedef struct APP_INIT_DATA_STRUCT
{
    unsigned char AidNo;       //����ѡ���AID���
    AppDt    AppData;          //���׿�ʼ����Ҫ������
}ctc_emv_aid;

//�ն˷��չ������ݽṹ
typedef struct TERM_RISK_MANAGE_STRUCT
{
    unsigned char CardType;       //�Ƿ�Ϊ�ڿ�
    unsigned short PanAmnt;          //��ǰ����ˮ��¼�����һ�ν��׽��
}ctc_emv_trm;
//�ֿ�����֤���ݽṹ
typedef struct CARD_HOLDER_VALIDATE
{
    unsigned char Flg;
    unsigned char Cmd;
}ctc_emv_chv;

//��ʼ�������ݽṹ
typedef struct BEGIN_TRANSACTION
{
    TransProDt TransInfo;
//    AppInitData InitDt;
    AppDt Dt;     
}ctc_emv_bt;

//��ȡ���ݽṹ
typedef struct GET_DTAT
{
    int P1;
    int P2;
}ctc_emv_gd;

//���������ļ��� ��¼��
typedef struct CARD_ON_SFI
{
    int SFI;    //�������ڵ��ļ���
    int Record; //��¼��
}ctc_emv_cos;


uint16_t emv_open(uint32_t mode);

uint16_t emv_close(void);

uint16_t emv_set_data(uint32_t len,uint8_t *data,uint8_t* ret);

uint16_t emv_app_set_data(uint8_t *data,uint8_t* ret);

uint16_t emv_app_select(uint32 data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_app_fianl_select(uint32 data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_app_init(uint32 data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_app_read(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_app_offline_data_auth(uint32_t mode,uint8_t* ret);

uint16_t emv_trem_ris_manage(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_process_restrict(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_card_holder_validate(uint32_t data_len,uint8_t *data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);
//uint16_t emv_card_holder_validate(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_action_analysis(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_online_data_process(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_update_data(uint32_t data_len,uint8_t* data,uint8_t* ret);

uint16_t emv_get_kernel_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_get_app_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_set_kernel_opt(uint32_t item,uint32_t flg,uint8_t *ret);

uint16_t emv_aid_init_list(uint8_t *ret);

uint16_t emv_aid_delete_list_aid(uint32_t data_len,uint8_t* data,uint8_t* ret);

uint16_t emv_aid_set_list_aid(uint32_t data_len,uint8_t* data,uint8_t mode,uint8_t* ret);

uint16_t emv_capkey_set(uint32_t data_len,uint8_t* data,uint8_t *ret);
//uint16_t emv_capkey_set(uint32_t index,uint32_t data_len,uint8_t* data,uint8_t* ret);

uint16_t emv_capkey_delete(uint32_t index,uint32_t data_len,uint8_t* data,uint8_t* ret);

uint16_t emv_capkey_get_info(uint8_t index,uint8_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t *ret);

uint16_t emv_iccard_get_log(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_iccard_get_load_log(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_get_kernel_version(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_expend(uint32_t data_len,uint8_t *data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_get_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_begin_transaction(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_read_record_last(uint32_t *rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_get_card_no_sfi(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_read_card_no_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_set_ic_parameter(uint32_t data_len,uint8_t* data,uint8_t* ret);

uint16_t emv_delete_all_capkey(uint8_t* ret);

uint16_t emv_continue_transaction(uint32_t data_len,uint8_t* data,uint32_t* len,uint8_t* buffer,uint8_t* ret);

uint16_t emv_extend_attribute_set(uint32_t data_len,uint8_t* data);

uint16_t emv_extend_attribute_get(uint32_t data_len,uint8_t* data, uint32_t* len,uint8_t* buffer);
#endif /*end __CTC_EMV_H__*/

