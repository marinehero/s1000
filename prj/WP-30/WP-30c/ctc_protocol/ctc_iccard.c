/*
 * =====================================================================================
 *
 *       Filename:  ctc_iccard.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  4/7/2016 3:34:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */

#include "ctc_iccard.h"

#include "wp30_ctrl.h"

typedef struct _iccard_type{
    int32_t slot;
    uint32_t mode;
    uint32_t vol;
    uint32_t open_slot;  //�ÿ����Ƿ��
}iccard_type;
  
typedef struct _iccard_number{

    iccard_type type[10];
    uint32_t number;
}ic_type_nu;

ic_type_nu gIccard;

uint8_t CardSlot;
uint8_t CardVol = VCC_5;
uint8_t CardMode = ISOMODE;

/*
 *���ܣ���IC����
 *
 *������data[0]:�������� data[1]:������
 *
 *����ֵ��
 *
 *ע:
 *
 */
uint16_t iccard_slot_open(uint8_t slot)
{
    uint8_t i=0;
    uint8_t mode,vol;
//    int ret = 0;
TRACE("\r\nIccard_slot_open");    
    for(i = 0; i < gIccard.number; i++)
    {
        if(gIccard.type[i].slot == slot)  //�ÿ����Ѿ�����
        {
            mode = gIccard.type[i].mode;  //��ȡ������Ϣ
            vol = gIccard.type[i].vol;
            break;
        }
    }
    if(i == gIccard.number) //�ÿ��������� ʹ��Ĭ��ֵ
    {
            CardSlot = slot;
            vol = CardVol ; //���¿�����Ϣ
            mode = CardMode ;
            if(iccard_type_select(slot,vol,mode) != CMDST_OK)
            {
               return CMDST_PARAM_ER;
            }
    }
    if(gIccard.type[i].open_slot == OFF )
    {
//        if(ret != ICC_SUCCESS)
//        {
//            return CMDST_PARAM_ER; 
//        }
//        else
//        {
            gIccard.type[i].open_slot = ON; //�ÿ�����
//        }
    }
//    else
//    {
//        return CMDST_OPEN_DEV_ER ;
//    }
    return CMDST_OK;
}
/*
 *���ܣ�����IC������
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 // ������
#define     USERCARD         0x00        // ������
#define     SAM1SLOT         0x01        // SAM����1
#define     SAM2SLOT         0x02        // SAM����2
#define     SAM3SLOT		 0x03		 // SAM����3
#define     SAM4SLOT		 0x04		 // SAM����4
#define     SAM5SLOT		 0x05		 // SAM����5   ��ӦS-970���ö�����SAM1
#define     SAM6SLOT		 0x06		 // SAM����6   ��ӦS-970���ö�����SAM2
#define     SAM7SLOT		 0x07		 // SAM����7   ��ӦS-970���ö�����SAM3

// ICC��ͨ�Ų���
#define     VCC_5            0x05    	//  5V��
#define     VCC_3            0x03    	//  3V��
#define     VCC_18           0x01    	//  1.8V��

#define     ISOMODE          0x00    	// ����ISO7816�淶
#define     EMVMODE          0x01    	// ����EMV v4.1�淶
#define     SHBMODE          0x02    	// �籣�淶
#define     CITYUNIONMODE    0x03

Э���е���ֵ���壺
��ѹѡ��	1	
0x05��5.0V
0x03��3.3V
0x01��1.8V
���淶Э��	1	
0x00��ISO7816�淶
0x01��EMV v4.1�淶
0x02���籣�淶

������
0x00	������
0x01~0x04	SAM1~SAM4��
0x10	SLExx42��
0x11	SLExx28��
0x12	AT24Cxx��
 
 *
 */
extern int icc_if_cardslot(int *CardSlot);

uint16_t iccard_type_select(uint8_t slot,uint8_t vol,uint8_t mode)
{
   int8_t i=0;
   int type;
   
   type = slot;
   
   if(icc_if_cardslot(&type))
	{
		return CMDST_PARAM_ER;
	}
	if(slot == USERCARD)
	{
        if((vol != VCC_5) && (vol != VCC_3) && (vol != VCC_18))
        {
            return CMDST_PARAM_ER;
        }
	}
	else
	{
		if(vol != VCC_5)
		{
			return CMDST_PARAM_ER;
		}
	}
	
    if (mode  > CITYUNIONMODE )
    {
		return CMDST_PARAM_ER;
    }
   
   for(i= 0; i < gIccard.number; i++)
   {
      if(gIccard.type[i].slot == slot) //�Ѵ��ڸÿ���
      {
          gIccard.type[i].mode = mode; //���¿�����Ϣ
          gIccard.type[i].vol = vol;
          break;
      }
   }
   if(i == gIccard.number)  //�����ڸÿ���
   {
       gIccard.type[i].slot = slot; 
       gIccard.type[i].mode = mode; //���¿�����Ϣ
       gIccard.type[i].vol = vol;
       gIccard.number ++;
       if(gIccard.number > 9)
       {
           gIccard.number = 9;
       }
        
   }

   return CMDST_OK;
}
/*
 *���ܣ��ر�IC����
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t iccard_slot_close(uint8_t slot)
{
    uint32_t i;
   
    for(i = 0; i < gIccard.number; i++)
    {
        if(gIccard.type[i].slot == slot)  //�ÿ����Ѿ�����
        {
            break;
        }
    }
    if(i == gIccard.number) //�ÿ���������
    {
        return CMDST_PARAM_ER; 
    }
    if(gIccard.type[i].open_slot == ON)
    {
        gIccard.type[i].open_slot = OFF;
    }
    else
    {
       return CMDST_CLOSE_DEV_ER;
    }
    return CMDST_OK;
}
/*
 *���ܣ���ָ����IC���ϵ�
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t iccard_card_open(uint8_t slot,uint32_t* len,uint8_t *buffer)
{
    int ret;
    uint32_t i;
    uint32_t vol,mode;  
    for(i = 0; i < gIccard.number; i++)
    {
        TRACE("\r\nIccard_open");    
        if(gIccard.type[i].slot == slot)  //�ÿ����Ѿ�����
        {
            vol = gIccard.type[i].vol;
            mode = gIccard.type[i].mode;
            break;
        }
    }
    if(i == gIccard.number) //�ÿ���������
    {
        return CMDST_PARAM_ER; 
    }
    if( gIccard.type[i].open_slot == ON )
    {
        ret = icc_InitModule(slot,vol,mode);
        if(ret != ICC_SUCCESS)
        {
            if(ret == -ICC_ERRPARA)
            {
                return CMDST_PARAM_ER; 
            }
            if(ret == -ICC_TIMEOUT)
            {
                return CMDST_TIMEOUT;
            }
            return CMDST_IC_RESET_ER;
        }

        ret = icc_Reset((int)slot,len,buffer);
        if(ret != ICC_SUCCESS)
        {
            if(ret == -ICC_ERRPARA)
            {
                return CMDST_PARAM_ER; 
            }
            if(ret == -ICC_TIMEOUT)
            {
                return CMDST_TIMEOUT;
            }
            return CMDST_IC_RESET_ER;
        }
        else
        {
            return CMDST_OK;
        }
    }
    return CMDST_OPEN_DEV_ER;

}
/*
 *���ܣ��첽������
 *
 *������
 *
 *����ֵ��
 *
 *ע��icc_ExchangeData() �жԿ��������ϵ���
 *
 */
uint16_t iccard_exchange_apdu(uint8_t slot,uint32_t sed_len,uint8_t* data,uint32_t *rev_len, uint8_t* rev_buf)
{
    int ret;
    
    TRACE("\r\nIccard_apdu");    
    ret = icc_ExchangeData((int)slot,(int)sed_len,data,(int *)rev_len,rev_buf);
    if(ret != ICC_SUCCESS)
    {
        if(ret == -ICC_ERRPARA)
        {
            return CMDST_PARAM_ER; 
        }
        if(ret == -ICC_TIMEOUT)
        {
            return CMDST_TIMEOUT;
        }
        return CMDST_IC_APDU_ER;  
    }
    return CMDST_OK;
}
/*
 *���ܣ��ر�IC��
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t iccard_card_close(uint8_t slot)
{
    int ret;
     uint32_t i;
   
    for(i = 0; i < gIccard.number; i++)
    {
        if(gIccard.type[i].slot == slot)  //�ÿ����Ѿ�����
        {
            break;
        }
    }
    if(i == gIccard.number) //�ÿ���������
    {
        return CMDST_PARAM_ER; 
    }
    if( gIccard.type[i].open_slot == ON)
    {
        ret = icc_Close(slot);
        if(ret != ICC_SUCCESS)
        {
            if(ret == -ICC_ERRPARA)
            {
                return CMDST_PARAM_ER; 
            }
            if(ret == -ICC_TIMEOUT)
            {
                return CMDST_TIMEOUT;
            }
            return CMDST_CLOSE_DEV_ER;  
        }
        return CMDST_OK;
    }
//    return CMDST_CLOSE_DEV_ER;  
    return CMDST_OK;
}

/*
 *���ܣ�IC����λ���
 *
 *������������  ��λ��Ϣ���� ��λ��Ϣ
 *
 *����ֵ��
 *
 *ע��
 *
 */
uint16_t iccard_card_check(uint8_t slot,uint32_t *len,uint8_t *buffer)
{
    int32_t ret;

    TRACE("\r\nIccard_check");    
    ret = icc_CheckInSlot(slot);
    if(ret == ICC_SUCCESS)
    {
        *len = 1;
        *buffer = 1;
        return CMDST_OK;
    }
    else
    {
        *len = 1;
        *buffer = 0;
        if(ret == -ICC_CARDOUT)
        {
            return CMDST_IC_EXIST_ER;
        }
    }
    return CMDST_PARAM_ER;
}
