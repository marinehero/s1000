/*
 * =====================================================================================
 *
 *       Filename:  ctc_emv.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  4/11/2016 10:06:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */

#include "ctc_emv.h"
#include "../emv/emv.h"

#include "wp30_ctrl.h"

uint8_t gemv_open_flag;

/*
 *���ܣ�����ֵת��
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t ctc_emv_ret(uint8_t res)
{
    uint16_t ret = 0;
    
    ret = CMDST_OK;
//    switch(res)
//    {
//    case EMV_RET_OK:
//        ret = CMDST_OK;
//        break;
//    case EMV_RET_PARA_ERROR:
//        ret = CMDST_PARAM_ER;
//        break;
//    default:
//        ret = CMDST_OTHER_ER;
//        break;
//    }
    return ret;
}

/*
 *���ܣ���EMV
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
extern unsigned char ST_EMV_OpenEMVKernel(void);
uint16_t emv_open(uint32_t mode)
{
   TRACE("\r\nemv_open");
    uint8_t ret;
    if(gemv_open_flag == OFF)
    {
        ret = ST_EMV_OpenEMVKernel();
        if(ret != EMV_RET_OK)
        {
            if(ret == EMV_RET_PARA_ERROR)
            {
                return CMDST_PARAM_ER;
            }
            else if(ret == EMV_RET_OTHER_ERROR)
            {
                return CMDST_OTHER_ER;
            }
        }
        else
        {
            gemv_open_flag = ON;
            return CMDST_OK; 
        }
    }
//    return CMDST_OPEN_DEV_ER;
    return CMDST_OK;
}    

/*
 *���ܣ��ر�EMV
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
extern unsigned char ST_EMV_CloseEMVKernel(void);
uint16_t emv_close(void)
{
   TRACE("\r\nemv_close");
    uint8_t ret;
    if(gemv_open_flag == ON)
    {
        ret = ST_EMV_CloseEMVKernel();
        if(ret != EMV_RET_OK)
        {
            if(ret == EMV_RET_PARA_ERROR)
            {
                return CMDST_PARAM_ER;
            }
           else if(ret == EMV_RET_OTHER_ERROR)
            {
                return CMDST_OTHER_ER;
            }
        }
        else
        {
            gemv_open_flag = OFF;
            return CMDST_OK; 
        }
    }
    //return CMDST_CLOSE_DEV_ER;
    return CMDST_OK; 
}    

/*
 *���ܣ����ù̶�����Ԫ
 *
 *����������Ԫ����   ����Ԫ�ṹ   ����ֵ
 *
 *����ֵ��
 *
 *ע������EMV�ں�ģ���е�����Ԫ
 *
 */
uint16_t emv_set_data(uint32_t len,uint8_t *data,uint8_t* ret)
{
   TRACE("\r\nemv_set_data ���ù̶�����Դ");

   if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_InitTmFixData((TmFixData *)data);
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ����ù̶�Ӧ���������Ԫ
 *
 *������
 *
 *����ֵ��
 *
 *ע����ʼ��EMV L2�ں�ģ������Ӧ����ص�����Ԫ���Ա�����ʹ�á�
 *
 */
uint16_t emv_app_set_data(uint8_t *data,uint8_t* ret)
{
    TRACE("\r\nemv_app_set_data ���ù̶�Ӧ���������Ԫ");
   
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_InitiateEMV((AppInitData *)data);
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ���ʼ����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_begin_transaction(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_begin_transaction");
  ctc_emv_bt *bt;
  if(data_len != sizeof(ctc_emv_bt))
  {
      return CMDST_PARAM_ER;
  }
  bt = (ctc_emv_bt*)data;
  TRACE_BUF("begin data:",data,sizeof(ctc_emv_bt));
  *ret = ST_EMV_BeginTransaction(&(bt->TransInfo),&(bt->Dt));
  *rev_len = sizeof(TransProDt); 
  
  memcpy(buffer,(char*)&(bt->TransInfo),*rev_len);
  return ctc_emv_ret(*ret);
}
/*
 *���ܣ�Ӧ��ѡ��
 *
 *������
 *
 *����ֵ��
 *
 *ע��Ӧ��ѡ��
 *
 */
uint16_t emv_app_select(uint32 data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_app_select");
    ctc_emv_asd *app_s;

    if(data_len != sizeof(ctc_emv_asd))
    {
        return CMDST_PARAM_ER;
    }

    app_s = (ctc_emv_asd*)data;

    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_AppSelection(&(app_s->CardSt),app_s->SelType,buffer,(unsigned short* )rev_len);
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;


}
/*
 *���ܣ�ѡ�����ȷ�ϵ�Ӧ��
 *
 *������
 *
 *����ֵ��
 *
 *ע�����ݷ��� ���ݳ���1+��������1+���ݳ���2+��������2
 *
 */
uint16_t emv_app_fianl_select(uint32 data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_app_fianl_select");
    ctc_emv_afd *app_f;
    uint16_t len1,len2;
    uint8_t data1[256],data2[256];

    if(data_len != sizeof(ctc_emv_afd))
    {
        return CMDST_PARAM_ER;
    }

    app_f =(ctc_emv_afd *)data;
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_FinalSelect(app_f->AidNo,data1,&len1,data2,&len2);
        data[0] = (uint8_t)(len1 & 0xff);
        data[1] = (uint8_t)((len1>>8) & 0xff);
        memcpy(data+2,data1,len1);
        data[len1] = (uint8_t)(len2 & 0xff);
        data[len1 + 1] = (uint8_t)((len2>>8) & 0xff);
        memcpy(data+2+len1+2,data2,len2);
        *rev_len = 2 + len1 + 2 + len2;

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ���ʼ��Ӧ��
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_app_init(uint32 data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_app_init ��ʼ��Ӧ��");
    ctc_emv_aid *app_i;

    if(data_len != sizeof(ctc_emv_aid))
    {
        return CMDST_PARAM_ER;
    }

    app_i = (ctc_emv_aid *)data;

    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_GetProcOption(app_i->AidNo,&(app_i->AppData),buffer,(unsigned short*)rev_len);
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ���ȡӦ������
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_app_read(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_app_read ��ȡӦ������");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_ReadAppData(buffer,(unsigned short*)rev_len);
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ��ϻ�������֤
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_app_offline_data_auth(uint32_t mode,uint8_t* ret)
{
   TRACE("\r\nemv_app_offline_data_auth �ѻ�������֤");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_OfflineDataAuth();
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ��ն˷��չ���
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_trem_ris_manage(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_trem_ris_mange �ն˷��չ���");
    ctc_emv_trm *trm;
    if(data_len != sizeof(ctc_emv_trm))
    {
        return CMDST_PARAM_ER;
    }

    trm = (ctc_emv_trm*)data;

    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_TermRiskManage(trm->CardType,trm->PanAmnt,buffer);
        *rev_len = 1;
        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ��������ƹ���
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_process_restrict(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_process_restrict �������ƹ���");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_ProcessRestrict(buffer);
        *rev_len = 1;

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ��ֿ�����֤
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_card_holder_validate(uint32_t data_len,uint8_t *data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_card_holder_validate �ֿ�����֤");
    ctc_emv_chv *chv;
    chv = (ctc_emv_chv *)data;
//    if(data_len != sizeof(ctc_emv_chv))
//    {
//        return CMDST_PARAM_ER; 
//    }
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_NewCardHolderValidate(chv->Flg,chv->Cmd,buffer,(unsigned short *)rev_len);

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ��ն���Ϊ�����Ϳ�Ƭ��Ϊ����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_action_analysis(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_action_analysis �ն���Ϊ����");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_ActionAnalysis((uint8_t) mode,(TransDt *)buffer);
        *rev_len = sizeof(TransDt);
        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���������
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_online_data_process(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_online_data_process ��������");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_OnlineDataProcess(data,(unsigned short)data_len,(TransDt *)buffer);
        *rev_len = sizeof(TransDt);

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ������ں�����Ԫ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_update_data(uint32_t data_len,uint8_t* data,uint8_t* ret)
{
   TRACE("\r\nemv_update_data �����ں�����Ԫ");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_SetData(data,(unsigned short)data_len);

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ȡģ���ں�����
 *
 *������
 *
 *����ֵ��
 *
 *ע����ȡEMV�ں˵�����Ԫ�б�
 *
 */
uint16_t emv_get_kernel_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_get_kernel_data");
    if(gemv_open_flag == ON)
    {
      *ret = ST_EMV_GetTLVList(data,(unsigned short)data_len,buffer,(int *)rev_len);

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ���ȡ�ں�ģ���е�Ӧ������
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_get_app_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_get_app_data");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_GetAppData(data,(unsigned char)data_len,buffer,(unsigned short *)rev_len);

        return ctc_emv_ret(*ret);

    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ������ں�����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_set_kernel_opt(uint32_t item,uint32_t flg,uint8_t *ret)
{
   TRACE("\r\nemv_set_kernel_opt");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_SetKernOpt((unsigned char)item,(unsigned char)flg);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ʼ��AID�б�
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
extern void ST_EMV_DelAllICPara(void);
uint16_t emv_aid_init_list(uint8_t *ret)
{
   TRACE("\r\nemv_aid_inti_list ��ʼ��AID�б�");
    if(gemv_open_flag == ON)
    {
        *ret = 0; 
//        ST_EMV_DelallTermAIDList();
        ST_EMV_DelAllICPara();
        return CMDST_OK;
    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ�ɾ��ָ��AID����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_aid_delete_list_aid(uint32_t data_len,uint8_t* data,uint8_t* ret)
{
   TRACE("\r\nemv_aid_delete_list_aid ɾ��ָ��AID����");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_DelAidListAid(data,(unsigned char)data_len);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ�����AID�б�����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_aid_set_list_aid(uint32_t data_len,uint8_t* data,uint8_t mode,uint8_t* ret)
{
   TRACE("\r\nemv_aid_set_list_aid ����AID�б�����");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_SetAidListItem(data,(unsigned char)data_len,mode);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}


/*
 *���ܣ��������Ĺ�Կ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_capkey_set(uint32_t data_len,uint8_t* data,uint8_t* ret)
{
    TRACE("\r\nemv_capkey_set");
//   if(data_len != sizeof(PKFILESTRU))
//   {
//       TRACE("\r\nPKFILESTRU:%d",sizeof(PKFILESTRU));
//       TRACE("\r\ncapkey len:%d",data_len);
//       return CMDST_PARAM_ER;
//   }
    TRACE_BUF("\r\ncap_key:",data,data_len);

    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_SetCAPKey((PKFILESTRU *)data);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ�ɾ����֤���Ĺ�Կ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_capkey_delete(uint32_t index,uint32_t data_len,uint8_t* data,uint8_t* ret)
{
   TRACE("\r\nemv_capkey_delete");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_DelCAPKeyByRidIndex(data,(unsigned char)index);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ȡ��Կ��Ϣ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_capkey_get_info(uint8_t index,uint8_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t *ret)
{
   TRACE("\r\nemv_capkey_get_info");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_GetCAPKeyByRidIndex(data,index,(PKFILESTRU *)buffer);
        *rev_len = sizeof(PKFILESTRU); 

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}
/*
 *���ܣ���ȡIC��������־
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_iccard_get_log(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_iccard_get_log");
    // uint8_t num;
    if(gemv_open_flag == ON)
    {
        // *ret = (*,buffer);
        *rev_len = sizeof(PKFILESTRU); 

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ȡIC��Ȧ����־
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_iccard_get_load_log(uint32_t mode,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_iccard_get_load_log");
    // uint8_t num;
    if(gemv_open_flag == ON)
    {
        // *ret = (*data,buffer);
        *rev_len = sizeof(PKFILESTRU); 

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ȡ�ں˰汾��Ϣ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_get_kernel_version(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{

   TRACE("\r\nemv_get_kernel_version");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_GetVersionInfo((char *)buffer,(int *)rev_len);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ȡ����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_get_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_get_data ��ȡ����");
    ctc_emv_gd *gd;
    
//    if(data_len != sizeof(ctc_emv_bt))
//    {
//        return CMDST_PARAM_ER;
//    }
    gd = (ctc_emv_gd*)data;

    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_ICCommand_GetData(gd->P1,gd->P2,buffer,(int*)rev_len);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;

}

/*
 *���ܣ������һ����¼
 *
 *������
 *
 *����ֵ��
 *
 *ע����ȡ���һ����¼��������������
 *
 */
uint16_t emv_read_record_last(uint32_t *rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_read_record_last");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_ReadLastRecord();
        *rev_len = 0;
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���ȡ���Ż򿨺����ڼ�¼�ļ����ͼ�¼��
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_get_card_no_sfi(uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_get_card_no_sfi ��ȡ���Ż��߼�¼��");
    ctc_emv_cos cos;

    if(gemv_open_flag == ON)
    {
       *ret = ST_EMV_GetCardnoSfi(&cos.SFI,&cos.Record);
        memcpy(buffer,(uint8_t*)&cos,sizeof(ctc_emv_cos));
        *rev_len = sizeof(ctc_emv_cos);
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;

}

/*
 *���ܣ���ȡ���Ż���ŵ��������ڵļ�¼����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_read_card_no_data(uint32_t data_len,uint8_t* data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_read_card_no_data");
    ctc_emv_cos *cos;

    cos = (ctc_emv_cos*)data;

    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_ReadCardnoData(cos->SFI, cos->Record,(char *)buffer,(int*)rev_len);

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;

}
/*
 *���ܣ�EMV������չָ��
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_expend(uint32_t data_len,uint8_t *data,uint32_t* rev_len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_expend");
    
   if(gemv_open_flag == ON)
    {
        *ret = 0;

        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ�����EMV��IC������
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
extern unsigned char ST_EMV_SetICPara(EMVPARA *pParaStru);
uint16_t emv_set_ic_parameter(uint32_t data_len,uint8_t* data,uint8_t* ret)
{
   TRACE("\r\nemv_set_ic_parameter");
   
   if(gemv_open_flag == ON)
    {
        if(data_len != sizeof(EMVPARA))
        {
            return CMDST_PARAM_ER;
        }

        *ret = ST_EMV_SetICPara((EMVPARA*)data);
        
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ�ɾ��EMV�ں������еĹ�Կ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_delete_all_capkey(uint8_t* ret)
{
   TRACE("\r\nemv_delete_all_capkey");
    if(gemv_open_flag == ON)
    {
        *ret = ST_EMV_DelAllCAPKey();
        
        return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ���������
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_continue_transaction(uint32_t data_len,uint8_t* data,uint32_t* len,uint8_t* buffer,uint8_t* ret)
{
   TRACE("\r\nemv_continue_transaction");
    if(gemv_open_flag == ON)
    {
        if(data_len != sizeof(PINSTATUS))
        {
            return CMDST_PARAM_ER;
        }

        
        *ret = ST_EMV_ContinueTransaction((PINSTATUS*) data);
      
      *len = sizeof(PINSTATUS);
      memcpy(buffer,data,sizeof(PINSTATUS));
      
      return ctc_emv_ret(*ret);
    }
    return CMDST_OPEN_DEV_ER;
}

/*
 *���ܣ�����emv ��չ���ԣ�������emv���뵯��ʱ�Ƿ���Ҫ�����������Ƿ���Ҫ��ʾ���봰��
 *:
 *������
 *
 *����ֵ��
 *
 *ע��Ŀǰֻ֧��type=0�����봰����������
 *
 */
static char s_emv_pinpad_attribute=0;
uint16_t emv_extend_attribute_set(uint32_t data_len,uint8_t* data)
{
    char type;
    type = data[0];
    // ��ʾΪ���뵯������
    switch ( type )
    {
    case 0 :
        if ( data_len < 2 ) {
            return CMDST_PARAM_ER;
        }
        // ����λ��Ч
        s_emv_pinpad_attribute = data[1] & 0x03;
        break;
    default :
        return CMDST_PARAM_ER;
    }
    return CMDST_OK;
} 

/*
 *���ܣ���emv_extend_attribute_set��Ӧ
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t emv_extend_attribute_get(uint32_t data_len,uint8_t* data, uint32_t* len,uint8_t* buffer)
{
    char type;
    type = data[0];
    // ��ʾΪ���뵯������
    switch ( type )
    {
    case 0 :
        // ����λ��Ч
        buffer[0] = s_emv_pinpad_attribute & 0x03;
        *len = 1;
        break;
    default :
        return CMDST_PARAM_ER;
    }
    return CMDST_OK;
}

// �ṩ��emv�ں˵��ú���
// Bit0�������������ʾ��  0-����ʾ 1-��ʾ 
// Bit1��������  0-�ް��� 1-��������
char emv_extend_pinpadwindow_get(void)
{
    return s_emv_pinpad_attribute & 0x03; 
}
/*
 *���ܣ�
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
/*
 *���ܣ�
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */

