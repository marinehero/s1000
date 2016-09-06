/*
 * =====================================================================================
 *
 *       Filename:  ctc_magcard.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  4/7/2016 3:01:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */


#include "ctc_magcard.h"

#include "wp30_ctrl.h"

uint8_t gmag_open_flag;
uint8_t gtrack1_flag;
uint8_t gtrack2_flag;
uint8_t gtrack3_flag;


/*
 *���ܣ��򿪴�����ģ��
 *
 *��������
 *
 *����ֵ��ERT_OK
 *
 * ע��mag_open()������һЩ�Ĵ������ã�����ֻ���سɹ�
 */
uint16_t magcard_open(uint32_t mode)
{
    if(gmag_open_flag == OFF)
    {
        gtrack1_flag = (mode & 1<<0);
        gtrack2_flag = (mode & 1<<1);
        gtrack3_flag = (mode & 1<<2);

        gmag_open_flag = ON;
        return CMDST_OK;
    }
//    return CMDST_OPEN_DEV_ER; 
    return CMDST_OK; 
}

/*
 *���ܣ��رմſ�ģ��
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t magcard_close(void)
{
    if(gmag_open_flag == ON)
    {
        gmag_open_flag = OFF;
        gSystem.lpwr.bm.mtk = 0; //�͹�������
        return CMDST_OK;
    }
    return CMDST_CLOSE_DEV_ER;
}

/*
 *���ܣ���������
 *
 *��������
 *
 *����ֵ����
 *
 *ע��
 *
 */
uint16_t magcard_credit_card(uint16_t mode)
{
    if(gmag_open_flag == ON)
    {
        mag_open();
        gSystem.lpwr.bm.mtk = 1; //�͹�������
        return CMDST_OK;
    }
    return CMDST_OPEN_DEV_ER;

}
/*
 *���ܣ�ȡ��������
 *
 *��������
 *
 *����ֵ����
 *
 *ע��
 *
 */
uint16_t magcard_cancle_card(void)
{
     if(gmag_open_flag == ON)
    {
        mag_close();
        return CMDST_OK;
    }
    return CMDST_OPEN_DEV_ER;

}



