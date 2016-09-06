/*********************************************************************
* ��Ȩ���� (C)2009,
* �ļ����ƣ�
*     ��Ƶ����������
* ��ǰ�汾��
*     1.0
* ����ժҪ��
*
* ��ʷ��¼��
*     �޸���		 ����		    	 �汾��       �޸ļ�¼
*******************************************************************************/
#include "wp30_ctrl.h"

#ifdef CFG_RFID
/**********************************************************************
*
*
*  �ⲿ�������ⲿ��������
*
*
***********************************************************************/
extern void DispData(uchar *title, uint32_t len, uchar *pucData);
extern int if_rfid_module(void);
/**********************************************************************
*
*
*   ���Ժ궨��
*
*
***********************************************************************/


/**********************************************************************
*
*
*   �궨��
*
*
***********************************************************************/

/**********************************************************************
*
*
*ȫ�ֱ�������:
*
*
***********************************************************************/
/**********************************************************************
*    ��������:
*
*
***********************************************************************/



#define LCD_TYPE_LINE4  1
#define LCD_TYPE_LINE2  0
int lcd_get_type(void)
{
    return LCD_TYPE_LINE4;
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void fac_show_ret(int ret)
{
	switch(ret)
	{
		case -RFID_ERROR:
			TRACE("ʧ��:[��ģ��ʧ��]");
			break;
		case -RFID_ERRPARAM:
			TRACE("ʧ��:[��������]");
			break;
		case -RFID_NOCARD:
			TRACE("ʧ��:[�޿�]");
			break;
		case -RFID_TRANSERR:
			TRACE("ʧ��:[ͨѶ����]");
			break;
		case -RFID_PROTERR:
			TRACE("ʧ��:[��Ч��]");
			break;
		case -RFID_CARDEXIST:
			TRACE("ʧ��:[��δ�뿪]");
			break;
		case -RFID_TIMEOUT:
			TRACE("ʧ��:[��ʱ]");
			break;
		case -RFID_MULTIERR:
			TRACE("ʧ��:[���ſ�]");
			break;
		case -RFID_NOACT:
			TRACE("ʧ��:[��δ�ϵ�]");
			break;
		default:
			TRACE("ʧ��:[δ֪����0x%2X]",ret);
			break;
	}
}



/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
const char *rfid_card_string [] = {
	"S50��",
	"S70��",
	"Pro��",
	"ProS50��",
	"ProS70��",
	"TypeB��",
	"δ֪��"
};
void fac_show_cardtype(uint line, uchar type)
{
#ifdef DEBUG_Dx
	char *card_type [] = {
	"TYPE A",
	" ",
	" ",
	" ",
	" ",
	"TYPE B",
	};

	if(type <= EM_mifs_TYPEBCARD)
		TRACE("�ɹ�:[%s]",card_type[type]);
	else
		TRACE("ʧ��:[%s]",card_type[type]);
#endif
}

void fac_MIFtest(void)
{
	int ret;
	uchar buf[16];
	ret = rfid_MIFAuth(3, 0,(uchar *)"\xFF\xFF\xFF\xFF\xFF\xFF");
	if(ret)
	{
		TRACE("��֤:ʧ��[%d]",ret);
		return;
	}
	ret = rfid_MIFRead(3,buf);
    if(ret)
    {

        TRACE("����:ʧ��[%d]",ret);
        return;
    }
    TRACE("����:�ɹ� %02X%02X%02X%02X",buf[0],buf[1],buf[2],buf[3]);
    TRACE("%02X%02X%02X%02X%02X%02X%02X%02X",buf[4],buf[5],buf[6]
          ,buf[7],buf[8],buf[9],buf[10],buf[11]);
    TRACE("����:�ɹ� %02X%02X%02X%02X",ret,buf[0],buf[1],buf[2]
          ,buf[3]);
    sys_beep_pro(BEEP_PWM_HZ,30,YES);
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void fac_circle_rfid_test(void)
{
	uchar buf[64];
	int ret;
	uint j,type;
#ifdef DEBUG_Dx
    uint i=0;
	char *card_type [] = {
	"TYPE A",
	" ",
	" ",
	" ",
	" ",
	"TYPE B",
	};
#endif
    TRACE("ȡ��-�˳�");
    while(1)
    {
        TRACE("ѭ������%d",++i);
        TRACE("Ѱ��:");
        ret = rfid_poll(RFID_MODE_ISO,&type);
        if(!ret)
        {
            TRACE("Ѱ��:[%s]",card_type[type]);
            TRACE("�ϵ�:");
            ret = rfid_powerup(type,&j,buf);
            if(!ret)
            {
                TRACE("�ϵ�:�ɹ� %s",rfid_card_string[buf[j-1]]);
                TRACE("����...");
                if(buf[j-1] >= EM_mifs_PROCARD)
                {
                    ret = rfid_exchangedata(sizeof(gApduGet4rand), (uchar *)gApduGet4rand,&j,buf);
                    if(ret == 0 || ret == EM_mifs_SWDIFF)
                    {
                        TRACE("����:�ɹ�[%02X-%02X]",buf[j-2],buf[j-1]);
                        sys_beep_pro(BEEP_PWM_HZ,30,YES);
                    }
                    else
                    {
                        TRACE("����:ʧ��[%d]",ret);
                    }
                    rfid_powerdown();
                }
                else
                {
                    fac_MIFtest();
                }
            }
            else
            {
                TRACE("�ϵ�:ʧ��[%d]",ret);
            }
        }
        else
        {
            TRACE("Ѱ��:ʧ��[%d]",ret);
        }

        if(kb_getkey(500) == KEY_CANCEL)
        {
            return;
        }
    }
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void fac_exchange_rfid_test(uint *type)
{
	uchar ret;
	uchar buf[36],str[16];
	uint i=0;

	TRACE("����");

	if(*type > EM_mifs_TYPEBCARD)
	{
		TRACE("ʧ��:[��δ�ϵ�]");
		kb_getkey(-1);
	}
	else if(*type <= EM_mifs_S70)
	{
		TRACE("�ɹ�");
		kb_getkey(-1);
	}
	else
	{
		ret = rfid_exchangedata(sizeof(gApduGet4rand), (uchar *)gApduGet4rand,&i,buf);
		if(ret != 0 && ret != EM_mifs_SWDIFF)
		{
			*type = 0xFF;
			fac_show_ret(ret);
			kb_getkey(-1);
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf((char *)str,"��������:%d",i);
			DispData(str,i, buf);
		}
	}

}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void fac_powerup_rfid_test(uint *type)
{
	uchar buf[36],str[16];
	int ret;
	uint i;
	lcd_cls();
	lcd_display(0, 0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY, "���ϵ�");
	ret = rfid_poll(RFID_MODE_ISO,type);
	if(ret)
	{
		fac_show_ret(ret);
		kb_getkey(-1);
	}
	else
	{
		i = 0;
		ret = rfid_powerup(*type,&i,buf);
		if(ret)
		{
			*type = 0xFF;
			fac_show_ret(ret);
			kb_getkey(-1);
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf((char *)str,"�ϵ�����:%d",i);
			DispData(str,i, buf);
		}
	}
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ��������� mode: 0-������� 1-��������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/

int fac_poll_rfid_test(int mode ,int total,int sus)
{
	uint type;
	int i=1,ret,sustimes=0,errtimes=0;
	int first_sus = 0;//�Ƿ��⵽һ�γɹ�
    lcd_Cls();
    lcd_Display(0, 0, DISP_FONT|DISP_INVLINE|DISP_CLRLINE|DISP_MEDIACY, "Ѱ��");
	lcd_Display(0,DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,"ȡ��-�˳�");
    kb_Flush();
	while(1)
	{
		lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "����:%d",i);
        if ( kb_Hit() ) {
            if(kb_GetKey(10) == KEY_CANCEL)
            {
                goto FAIL;
            }
        }
		ret = rfid_poll(RFID_MODE_ISO,&type);
		if(!ret)
		{
			first_sus = 1;
			sustimes++;
			if(type == 0)
				lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,"��:[TYPE A]");
			else
				lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,"��:[TYPE B]");
            sys_BeepPro(BEEP_PWM_HZ,300,YES);
		}
		else
		{
			if (first_sus != 0)
			{
				errtimes++;
				sustimes = 0;
				lcd_Display(0,DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��:ʧ��");
			}
		}
		lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_MEDIACY|DISP_CLRLINE, "�ɹ�:%-2d ʧ��:%-2d",sustimes,errtimes);
        rfid_powerdown();
        if ( !mode ) 
        {
            if(kb_getkey(500) == KEY_CANCEL)
            {
                return 0;
            }
        }
        else
        {
            if(sustimes >= sus)
            {
                //			kb_GetKey(2000);
                goto SUCCESS;
            }

            if(i >= total)
            {
                goto FAIL;
            }
            if (first_sus != 0)
            {
                i++;
            }
        }
	}
SUCCESS:
    return OK;
FAIL:
    return ERROR;
}
/**********************************************************************
* �������ƣ�
*
* ����������
*
* ���������
*     ��
* ���������
*     ��
* ����ֵ��
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
int Test_RFID_Fac(int mode)
{
	uint type = 0;
	int key = 0;
	int ret;
	ret = if_rfid_module();
	if(ret)
	{
        lcd_cls();
        lcd_display(0,DISP_FONT_LINE2,DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,"û�и�ģ��");
        kb_getkey(3000);
		return 0;
	}
    type = type;
	ret = rfid_open(0);
	if(ret)
	{
		rfid_close();
		lcd_cls();
		lcd_display(0,DISP_FONT, DISP_FONT|DISP_MEDIACY, "��Ƶģ���ʧ��");
		kb_getkey(5000);
		return 0;
	}
	while(1)
	{
        if (lcd_get_type() == LCD_TYPE_LINE4)
        {
            lcd_cls();
            lcd_display(0,DISP_FONT_LINE0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY, "��Ƶ������");
            lcd_display(0,DISP_FONT_LINE2,  DISP_FONT|DISP_CLRLINE,            "1-Ѱ��   ");
            lcd_display(0,DISP_FONT_LINE4,  DISP_FONT|DISP_CLRLINE,"2-ѭ������ ");
        }
        else
        {
            lcd_cls();
            lcd_display(0,DISP_FONT_LINE2,  DISP_FONT|DISP_CLRLINE, "1-Ѱ��   ");
            lcd_display(0,DISP_FONT_LINE4,  DISP_FONT|DISP_CLRLINE,"2-ѭ������ ");
        }

//        sys_SleepWaitForEvent(EVENT_KEYPRESS,0,0);
		key = kb_getkey(-1);
		switch(key)
		{
			case KEY1:
				fac_poll_rfid_test(0,0,0);
				break;
			/*
			case KEY2:
				type = 0xFF;
				fac_powerup_rfid_test(&type);
				break;
			case KEY3:
				fac_exchange_rfid_test(&type);
				break;
			case KEY4:
				type = 0xFF;
				lcd_cls();
				lcd_display(0, 0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY, "���µ�");
				lcd_display(0, 2*DISP_FONT, DISP_FONT|DISP_CLRLINE , "�ɹ�");
				rfid_powerdown();
				kb_getkey(-1);
				break;
			case KEY5:
			*/
			case KEY2:
				type = 0xFF;
				fac_circle_rfid_test();
				break;
			case KEY_CANCEL:
				rfid_close();
				return 0;
			default:
				break;
		}
	}
}
/*
 * test_rfid_para - [GENERIC] �ⶨ��Ƶ������
 * @
 */
int test_rfid_para (uint mode)
{
    int ret;
    uint type,i;
    uint recvlen;
    uchar recv[128];
    ret = rfid_poll(mode,&type);
//    TRACE("\r\n poll ret:%d",ret);
    if ( !ret ) {
        ret = rfid_powerup(type,&recvlen,recv);
//        TRACE("\r\n powerup ret:%d",ret);
        if ( !ret ) {
            type = recv[recvlen-1];
            if (type  < RFID_CARD_PRO ) {
                return 1;
            }else{
                lcd_display(0, DISP_FONT, DISP_FONT|DISP_CLRLINE,"�ɹ�:%s",rfid_card_string[type]);
            }
            for ( i=0 ; i<3 ; i++ ) {
                ret = rfid_exchangedata(sizeof(gApduGet4rand), (uchar *)gApduGet4rand,&recvlen,recv);
//                TRACE("\r\n exchange ret:%d",ret);
                if ( ret ) {
                    break;
                }
//                vDispBufTitle("rand",recvlen,0,recv);
            }
            rfid_powerdown();
        }
    }
    return ret;
}		/* -----  end of function test_rfid_para  ----- */

int test_rfid_para_menu(int mode)
{
    int pollmode[]={
        0,
        RFID_MODE_CID|RFID_MODE_NAD,
        RFID_MODE_NOFRAME,
    };
    int i,ret;
    kb_flush();
    lcd_cls();
    lcd_display(0,0,DISP_FONT|DISP_MEDIACY|DISP_INVLINE,"��Ƶ�������ⶨ");
    lcd_display(0,DISP_FONT,DISP_FONT,"��źÿ�,�������ʼ");
    kb_getkey(-1);
    while(1)
    {
        lcd_cls();
        lcd_display(0,0,DISP_FONT|DISP_MEDIACY|DISP_INVLINE,"��Ƶ�������ⶨ");
        for(i=0;i<DIM(pollmode);i++)
        {
            lcd_display(0,DISP_FONT,DISP_FONT|DISP_CLRLINE,"����%d��...",i+1);
            ret = test_rfid_para(pollmode[i]);
            if(!ret){
                break;
            }
        }
        if ( !ret ) {
//            TRACE("\r\n %d-%x",i,pollmode[i]);
            lcd_display(0,DISP_FONT*2,DISP_FONT,"֡��:%s",
                        (pollmode[i]&RFID_MODE_NOFRAME) ? "��֧��" : "֧��");
            lcd_display(0,DISP_FONT*3,DISP_FONT,"CID:%s",
                        (pollmode[i]&RFID_MODE_CID) ? "֧��" : "��֧��");
            lcd_display(0,DISP_FONT*4,DISP_FONT,"NAD:%s",
                        (pollmode[i]&RFID_MODE_CID) ? "֧��" : "��֧��");
        } else {
            fac_show_ret(ret);
        }
        sys_beep();
        if ( kb_getkey(3000) == KEY_CANCEL) {
            break;
        }
    }
    return ret;
}
#endif



