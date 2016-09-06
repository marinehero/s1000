/*
 * =====================================================================================
 *
 *       Filename:  factory.c
 *
 *    Description:  �����������
 *
 *        Version:  1.0
 *        Created:  1/14/2015 2:17:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yehf (), yehf@itep.com.cn
 *        Company:  START
 *
 * =====================================================================================
 */
#include "wp30_ctrl.h"
#include "../pinpad/pcikeystore.h"
#ifdef CFG_FACTORY 

/*-----------------------------------------------------------------------------}
 *   ��������
 *----------------------------------------------------------------------------{*/
extern int local_menu_operate(int mode);
extern int iccard_powerup(int cardslot, int *plen, uchar *pData);
extern int fac_mb_main(int mode);
/*-----------------------------------------------------------------------------}
 *  �궨��ͽṹ�嶨�� 
 *----------------------------------------------------------------------------{*/
#define     PRODUCT_VERSION             "V1.0"
#define     FAC_MENU_ETITLE_TOP             "FACTORY"PRODUCT_VERSION
#define     FAC_MENU_CTITLE_TOP             "��������"PRODUCT_VERSION

#define     FAC_AUTO_POWEROFF_TIME           30*1000*60  //�������Խ����Զ��ػ���ʱʱ��  10 minutes

#define     FAC_CTRL_ASSEMBLE_CHECK  

// �Զ����԰���
#define     KEY_TEST_ARRAY_DEBUG      0x3E22  //1��5��9��ȷ�ϡ������ȡ��������
#define     KEY_TEST_ARRAY_ASSEMBLE   0x3FFF  //ȫ��
#define     KEY_TEST_ARRAY_FINAL      0x3E22  //1��5��9��ȷ�ϡ������ȡ��������

#define DEFAULT_MODULE_CFG			((1 << AUTO_TEST_SHIFT_SRAM) |\
									 (1 << AUTO_TEST_SHIFT_KEY)	 |\
									 (1 << AUTO_TEST_SHIFT_LCD)  |\
									 (1 << AUTO_TEST_SHIFT_MAG)  |\
									 (1 << AUTO_TEST_SHIFT_RFID)  |\
									 (1 << AUTO_TEST_SHIFT_IC)  |\
									 (1 << AUTO_TEST_SHIFT_RTC)  |\
									 (1 << AUTO_TEST_SHIFT_LED)  |\
									 (1 << AUTO_TEST_SHIFT_BATTERY)\
									)
//                                   װ��ǰ��������ID��ʱ������
//									 (1 << AUTO_TEST_SHIFT_USB) \

#define DEFAULT_MAG_SUCCESS_NUM		4		//Ĭ��4������ˢ���ɹ�����ͨ��
#define DEFAULT_SAM_NUM				1       //Ĭ�ϲ�������SAM��
#define DEFAULT_AGING_CHARGE_MV	    3800    //�ϻ�Ĭ�ϳ���ѹ(���ڴ�ֵ�������쳣) 
#define DEFAULT_AGING_DISCHARGE_MV  3500    //�ϻ�Ĭ�Ϸŵ��ѹ(���ڴ�ֵ�ŵ�����쳣)
#define DEFAULT_AGING_CHARGE_TIME    90     //��һ�γ��
#define DEFAULT_AGING_DISCHARGE_TIME 30     //��һ�ηŵ�
#define DEFAULT_AGING_RECCHARGE_TIME 120    //�ڶ��γ��
#define DEFAULT_SIGNS_VALUE   20                //Ĭ���ź�ֵ
#define DEFAULT_SPEICAL_NO    0                 //Ĭ�ϵ��ϻ�����Ϊ��������
/*-----------------------------------------------------------------------------}
 *  ȫ�ֱ���
 *-----------------------------------------------------------------------------{*/
const char * const gFacPhaseName[FAC_PHASE_NUM] =
{
    "�忨��",
    "װ���",
    "�ϻ���",
    "�ܼ�"
};

// ����ȫ�ֻ�����
FAC_STAT_t  *gFacTest=NULL;

// װ����Բ˵�
const FAC_TEST_OPT gMenuAssembleTest[] = {
    {AUTO_TEST_SHIFT_LCD,       1,   test_display},
#ifdef CFG_LED
    {AUTO_TEST_SHIFT_LED,       0,   test_led},
#endif
    {AUTO_TEST_SHIFT_SERIAL,    0,   test_serial_self},
#ifdef CFG_BATTERY
    {AUTO_TEST_SHIFT_BATTERY,   1,   test_battery},
#endif
    {AUTO_TEST_SHIFT_RTC,       0,   test_rtc_autocheck},
#ifdef CFG_RFID
    {AUTO_TEST_SHIFT_RFID,      0,   test_rfid_auto},
#endif
#ifdef CFG_MAGCARD
    {AUTO_TEST_SHIFT_MAG,       (1<<17)|(1<<18)|(1<<8),   test_magcard_auto},
#endif
#if PRODUCT_NAME == PRODUCT_MPOS
#ifdef CFG_ICCARD
    {AUTO_TEST_SHIFT_IC,        (1<<1)|(1<<8),   test_iccard_auto_new},
#endif
#endif
    {AUTO_TEST_SHIFT_KEY,       KEY_TEST_ARRAY_ASSEMBLE,   test_keyboard},
#ifdef CFG_USBD_CDC
    {AUTO_TEST_SHIFT_USB,       0,   test_usb_manual},
#endif
    {AUTO_TEST_SHIFT_SRAM,     1,   test_sram_auto},
};
// �ܼ���Բ˵�
const FAC_TEST_OPT gMenuFinalTest[] = {
    {AUTO_TEST_SHIFT_SRAM,      0,   test_sram_auto},
    {AUTO_TEST_SHIFT_LCD,       1,   test_display},
#ifdef CFG_LED
    {AUTO_TEST_SHIFT_LED,       0,   test_led},
#endif
    {AUTO_TEST_SHIFT_SERIAL,    0,   test_serial_self},
#ifdef CFG_BATTERY
    {AUTO_TEST_SHIFT_BATTERY,   2,   test_battery},
#endif
#ifdef CFG_RFID
    {AUTO_TEST_SHIFT_RFID,      0,   test_rfid_auto},
#endif
#ifdef CFG_MAGCARD
    {AUTO_TEST_SHIFT_MAG,       (1<<17)|(1<<18)|(1<<8),   test_magcard_auto},
#endif
#if PRODUCT_NAME == PRODUCT_MPOS
#ifdef CFG_ICCARD
    {AUTO_TEST_SHIFT_IC,        (1<<1)|(1<<8),   test_iccard_auto_new},
#endif
#endif
    {AUTO_TEST_SHIFT_KEY,       KEY_TEST_ARRAY_FINAL,   test_keyboard},
#ifdef CFG_USBD_CDC
    {AUTO_TEST_SHIFT_USB,       0,   test_usb_manual},
#endif
    {AUTO_TEST_SHIFT_RTC,       0,   test_rtc_autocheck},
};

/*-----------------------------------------------------------------------------}
 *  ��������
 *-----------------------------------------------------------------------------{*/
int test_rtc_autostart(uint mode, uchar *time)
{
	sys_GetTime(time);
    if ( time[0] < 0x15 || time[1] < 0x01) {
        lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"ʱ�����");
        kb_GetKey(-1);
        return 1;
    }
	return 0;
}


int fac_ctrl_tip_backdoor(int new_phase)
{
    int ret;
    ret = backdoor_password_verify("456789");
    if ( ret ) {
        return 1;
    }
    TRACE_M("\r\n **********ǿ��ִ��:%s �ɹ�**********",gFacPhaseName[new_phase-1]);
    ret = fac_ctrl_set_autotest_res(new_phase-1,FAC_CTRL_FORCE_SUC);
    return ret;
}

/* 
 * fac_ctrl_show_tip - [GENERIC] װ��� 
 * @ 
 */
int fac_ctrl_show_tip(int new_phase)
{
	uint cur_phase;
    int i;
    int ret;
	cur_phase = fac_ctrl_get_cur_phase();
    TRACE_M("\r\nfac phase:%d\r\n\r\n",cur_phase);
    if ( cur_phase > FAC_PHASE_NUM ) {
        cur_phase = FAC_PHASE_DEBUG;			
    }
#if defined (DEBUG_FAC)
    TRACE_M("\r\n��ǰ:%s׼��:%s",gFacPhaseName[cur_phase],gFacPhaseName[new_phase]);
    for ( i=0 ; i<DIM(gFacPhaseName) ; i++ ) {
        ret = fac_ctrl_get_autotest_res(i);
        TRACE("\r\n�׶�:%s:%d",gFacPhaseName[i],ret);
    }
#endif
#ifdef FAC_CTRL_ASSEMBLE_CHECK
	if (new_phase == cur_phase || new_phase == FAC_PHASE_DEBUG)
#else
	if (new_phase == cur_phase || new_phase == FAC_PHASE_DEBUG || new_phase == FAC_PHASE_ASSEMBLE)
#endif
	{
		return YES;
	}
	kb_Flush();
#ifdef FAC_CTRL_ASSEMBLE_CHECK
	if (new_phase >= FAC_PHASE_ASSEMBLE && new_phase <= FAC_PHASE_PRODUCT)
#else
	if (new_phase >= FAC_PHASE_AGING && new_phase <= FAC_PHASE_PRODUCT)
#endif
	{
        ret = fac_ctrl_get_autotest_res(new_phase-1);
        // δ���Ի����ʧ��
		if ((ret!=FAC_CTRL_SUCCESS)
            && (ret!=FAC_CTRL_FORCE_SUC) )
		{
            lcd_Cls();
			lcd_Display(0, 0, DISP_FONT, "����:%sδͨ��", gFacPhaseName[new_phase-1]);
			lcd_Display(0, DISP_FONT_LINE1, DISP_FONT, "���������%s", gFacPhaseName[new_phase]);
			lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_MEDIACY, "[1]-ǿ�ƽ���");
			lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_MEDIACY, "[ȡ��]-�˳�");
			while (1)
			{
                i = kb_GetKey(-1);
                if ( i == KEY_CANCEL ) {
                    return NO;
                } else if(i == KEY1){
                    if (!fac_ctrl_tip_backdoor(new_phase))
                    {
                        break;
                    }
                }
			}
		}
	}
    lcd_cls();
	lcd_Display(0,DISP_FONT_LINE0, DISP_FONT|DISP_MEDIACY, "��ǰΪ%s", gFacPhaseName[cur_phase]);
	lcd_Display(0,DISP_FONT_LINE1, DISP_FONT, "[ȷ��]-����%s", gFacPhaseName[new_phase]);
	lcd_Display(0,DISP_FONT_LINE2, DISP_FONT, "[ȡ��]-�˳�");

	while (1)
	{
		switch (kb_GetKey(-1))
		{
			case KEY_CANCEL:
				return NO;
			case KEY_ENTER:
				fac_ctrl_set_cur_phase(new_phase);
				return YES;
			default:
				continue;
		}
	}
}
/* 
 * test_conf_require - [GENERIC] �ж��Ƿ���Ҫ���� 
 * //��ģ���Ƿ���Ҫ�Զ�����()
//����1:��Ҫ, ����0:����Ҫ
 * @ 
 */
int test_conf_require(int module)
{
    if ( IFBIT(gFacTest->tConfig.module_config,module) ) {
        return 1;
    } else {
		return 0;
	}
}

// 0-����Ҫ���²��� ��0-��Ҫ���²���
uint if_test_again(uint flag, uint config)
{
    return (flag&(1<<config));
}
int test_write_result(int ret, int config, uint *result)
{
    int data = *result;
    if ( config >= DEFAULT_MODULE_CFG) {
        return 1;
    }
    if ( ret ) {
        SETBIT(data,config);
    } else {
        CLRBIT(data,config);
    }
    *result = data;
    return 0;
}


//�ſ�ˢ������ɹ�
int test_conf_mag_success(void)
{
	return gFacTest->tConfig.mag_success_num;
}

//�⼸��SAM��
int test_conf_sam_num(void)
{
	return gFacTest->tConfig.sam_test_num;
}

int test_conf_aging_charge_mv(void)
{
	return gFacTest->tConfig.aging_charge_mv;
}

int test_conf_aging_discharge_mv(void)
{
	return gFacTest->tConfig.aging_discharge_mv;
}

int test_conf_aging_discharge_time(void)
{
	return gFacTest->tConfig.aging_discharge_time;
}

int Set_conf_aging_dicharge_time(int time)
{
	gFacTest->tConfig.aging_discharge_time = (ushort)time;
	return 0;
}

int test_conf_icsq(void)
{
	return gFacTest->tConfig.icsq;
}

int test_conf_aging_special(void)
{
	return gFacTest->tConfig.ispecial;
}
// �װ���ԣ����Լ죬װ��죬�ܼ���Խ�������
// 0-�˳� 1-���²���
int test_fail_view(unsigned int testResult)
{
    int i,key;
    char message[128] = "����:";
    for (i = 0; i <= AUTO_TEST_SHIFT_MAX; i++)
    {
        if ((1 << i) & testResult)
        {
            switch (i)
            {
            case AUTO_TEST_SHIFT_SRAM:
                strcat(message, "����,");
                break;
            case AUTO_TEST_SHIFT_KEY:
                strcat(message, "����,");
                break;
            case AUTO_TEST_SHIFT_LCD:
                strcat(message, "Һ��,");
                break;
            case AUTO_TEST_SHIFT_PRINT:
                strcat(message, "��ӡ,");
                break;
            case AUTO_TEST_SHIFT_MAG:
                strcat(message, "�ſ�,");
                break;
            case AUTO_TEST_SHIFT_IC:
                if (ic_not_in_slot)
                {
                    strcat(message, "IC����λ,");
                }
                if (gFacTest->iccardret[0])
                {
#if PRODUCT_NAME == PRODUCT_MPOS
                    strcat(message, "IC��,");
#else
                    strcat(message, "SAM��0,");
#endif
                }

                if (gFacTest->iccardret[SAM1SLOT])
                {
                    strcat(message, "SAM��1,");
                }
                break;
            case AUTO_TEST_SHIFT_SERIAL:
                strcat(message, "����,");
                break;
            case AUTO_TEST_SHIFT_RTC:
                strcat(message, "RTC,");
                break;
            case AUTO_TEST_SHIFT_TEMP:
                strcat(message, "�¶�,");
                break;
            case AUTO_TEST_SHIFT_RFID:
                strcat(message, "��Ƶ��,");
                break;
            case AUTO_TEST_SHIFT_USB:
                strcat(message, "USB,");
                break;
            case AUTO_TEST_SHIFT_BATTERY:
                if (battery_charge_err)
                    strcat(message, "����·����");
                else
                    strcat(message, "���,");
                break;
            case AUTO_TEST_SHIFT_LED:
                strcat(message, "LED��,");
                break;
            default:
                break;
            }
        }
    }
//    TRACE_M("\r\n����δͨ�� result=%x, ic_error_array=%x", testResult, ic_error_array);
    strcat(message, "δͨ��");
    lcd_Display(0, 0, DISP_FONT|DISP_CLRLINE, message);
    kb_Flush();
    if ( gTestStage == STAGE_Assemble || gTestStage == STAGE_Product) {
        sys_BeepPro(BEEP_PWM_HZ, 1000, NO);
        lcd_Display(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE, "[ȷ��]-�ز�δͨ����");
        key = kb_GetKey(-1);
        if ( key == KEY_ENTER ) {
            return 1;
        }
    } else {
        kb_GetKey(-1);
    }
    return 0;
}
int show_aging_log_new(int para)
{
	s_aging_log log;

	int page = 0;
	int iRet = 0;
    int i = 0;
	const uchar *step_name[]=
	{
		"��",
		"����",
		"Һ��",
		"����",
		"�ſ�",
		"IC��",
		"��ӡ",
	};

	int page_need = 4;//�ܹ���Ҫ����ҳ
	memset(&log, 0x00, sizeof(s_aging_log));
	iRet = fac_read_aginglog(&i,&log);
    if ( iRet == 0 ) 
    {
		lcd_Cls();
		lcd_Display(0,0, DISP_FONT | DISP_CLRLINE | DISP_MEDIACY | DISP_INVLINE, "�ϻ����");
		lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "δ����");
		kb_GetKey(-1);
		return -1;
    }

	kb_Flush();
	while (1)
	{
		lcd_Cls();
		lcd_Display(0,0, DISP_FONT | DISP_CLRLINE | DISP_MEDIACY | DISP_INVLINE, "�ϻ����");
		switch (page)
		{
			case 0:
				if (log.aging_allok== 1)
				{
					lcd_Display(0,DISP_FONT_LINE1, DISP_FONT | DISP_CLRLINE, "�ɹ�");
				}
				else
				{
					if (log.fail_step > 0)
					{
						lcd_Display(0,DISP_FONT_LINE1, DISP_FONT | DISP_CLRLINE, "ʧ�ܻ���:%s",
									step_name[log.fail_step]);
					}
					if (log.battery_error_info & (1<<2))
					{
						lcd_Display(0,DISP_FONT_LINE2, DISP_FONT | DISP_CLRLINE,
									"����������");
					}
					else if (log.battery_error_info & (1<<0))
					{
						lcd_Display(0,DISP_FONT_LINE2, DISP_FONT | DISP_CLRLINE,
									"����·����");
					}
					else if (log.battery_error_info & (1<<1))
					{
						lcd_Display(0,DISP_FONT_LINE3, DISP_FONT | DISP_CLRLINE,
									"��ز���");
					}
					else if (log.battery_error_info & (1<<4))
					{
						lcd_Display(0,DISP_FONT_LINE3, DISP_FONT | DISP_CLRLINE,
									"�����δ���");
					}
					else
					{
						lcd_Display(0,DISP_FONT_LINE3, DISP_FONT | DISP_CLRLINE,
									"�ϻ�����δ���");
					}

				}
				lcd_Display(0,DISP_FONT_LINE4, DISP_FONT | DISP_CLRLINE, "��ҳ�鿴��ϸ��Ϣ:%d",log.battery_error_info);
				break;
			case 1:
				lcd_Display(0,DISP_FONT_LINE1, DISP_FONT | DISP_CLRLINE, "�ܲ�������:%d",log.totaltimes);
				lcd_Display(0,DISP_FONT_LINE2, DISP_FONT | DISP_CLRLINE, "�Ƿ���ʧ����ֹ:%s", (log.fail_step == 0) ? "��":"��");
				break;
//			case 2:
//				break;
            case 2:
				lcd_Display(0,DISP_FONT_LINE1, DISP_FONT | DISP_CLRLINE, "�ܳ��ʱ��%dmin", log.charge_time);
				lcd_Display(0,DISP_FONT_LINE2, DISP_FONT | DISP_CLRLINE, "����ϻ�ʱ��%dmin",log.discharge_time);
				if (log.baterry_mv1 > 0)
				{
					lcd_Display(0,DISP_FONT_LINE3, DISP_FONT | DISP_CLRLINE, "�ε�Դʱ%dmv", log.baterry_mv1);
				}
				if (log.baterry_mv2 > 0)
				{
					lcd_Display(0,DISP_FONT_LINE4, DISP_FONT | DISP_CLRLINE, "�ŵ����%dmv", log.baterry_mv2);
				}
				else
				{
				}
				break;
			case 3:
				lcd_Display(0,DISP_FONT_LINE2, DISP_FONT | DISP_CLRLINE, "���ʱ��:%dmin",
						log.recharge_time);
				lcd_Display(0,DISP_FONT_LINE3, DISP_FONT | DISP_CLRLINE, "����¼��ѹ:%dmv",
						log.baterry_mv_actual);
				break;
			default:
				break;
		}

		switch (kb_GetKey(-1))
		{
			case KEY_CANCEL:
				return -2;
			case KEY_ENTER:
				return 0;
			case KEY2:
				if (page <= 0)
				{
					page = page_need - 1;
				}
				else
				{
					page--;
				}
				break;
			case KEY8:
				if (page >= (page_need -1))
				{
					page = 0;
				}
				else
				{
					page++;
				}
				break;
			default:
				break;
		}
	}
}
/* 
 * test_auto_final - [GENERIC] װ��� 
 * @ 
 */
int test_auto_final(int mode)
{
    int result;
//	int icsq = 0;
    uint i;
    uint first=1,again_flag = UINT_MAX;
    uint testResult = 0;
    FAC_TEST_OPT *p;
    uint phase;
	//��ʾ�ϴ��ϻ����
    s_aging_log log;
	memset(&log, 0x00, sizeof(s_aging_log));
	show_aging_log_new(0);

    phase = FAC_PHASE_PRODUCT;
	if (fac_ctrl_show_tip(phase) != YES)
	{
		return OK;
	}
    TRACE_M("\r\n %s��ʼ",gFacPhaseName[phase]);
    memset(gFacTest->time,0,sizeof(gFacTest->time));
	if(test_rtc_autostart(0, gFacTest->time)) {
        return ERROR;
    }
	fac_ctrl_set_cur_phase(phase);
    gTestStage = phase;
test_auto_final_again:
    p = (FAC_TEST_OPT *)&gMenuFinalTest[0];
    for ( i=0 ; i<DIM(gMenuFinalTest) ; i++,p++  ) {
        if (test_conf_require(p->id)
            && if_test_again(again_flag,p->id))
        {
            result = p->test_func(p->para);
            test_write_result(result,p->id,&testResult);
        }
    }
//    init_sram(0);
    lcd_Cls();
	kb_Flush();
    if (!testResult) {
        fac_ctrl_set_test_detail(phase,testResult);
        fac_ctrl_set_autotest_res(phase, FAC_CTRL_SUCCESS);
        lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "%sͨ��",gFacPhaseName[phase]);
        kb_GetKey(2000);
        return OK;
    } else {
        fac_ctrl_set_test_detail(phase,testResult);
        fac_ctrl_set_autotest_res(phase, FAC_CTRL_FAIL);
        if(test_fail_view(testResult)) {
            TRACE_M("\r\n ��ǰresult:%x flag:%x",testResult,again_flag);
            if ( first == 1 ) {
                first = 0;
                again_flag = (~again_flag) | testResult;
            } else {
                again_flag = testResult;
            }
            TRACE_M("\r\n ����result:%x flag:%x",testResult,again_flag);
            goto test_auto_final_again;
        }
        return ERROR;
    }
}

/* 
 * test_auto_assemble - [GENERIC] װ��� 
 * @ 
 */
int test_auto_assemble(int mode)
{
    int result;
    uint i;
    uint first=1,again_flag = UINT_MAX;
    uint testResult = 0;
    FAC_TEST_OPT *p;
    uint phase;

#if PRODUCT_NAME == PRODUCT_EPP 
        s_aging_log log;
        memset(&log, 0, sizeof(s_aging_log));	//���log����
#endif

    //���Լ�ǿ�Ƴɹ�
//    fac_ctrl_set_autotest_res(FAC_PHASE_DEBUG,FAC_CTRL_SUCCESS);

    phase = FAC_PHASE_ASSEMBLE;
	if (fac_ctrl_show_tip(phase) != YES)
	{
		return OK;
	} 
    TRACE_M("\r\n %s��ʼ",gFacPhaseName[phase]);
    memset(gFacTest->time,0,sizeof(gFacTest->time));
	if(test_rtc_autostart(0, gFacTest->time)) {
        return ERROR;
    }
	fac_ctrl_set_cur_phase(phase);
    gTestStage = phase;
test_auto_assemble_again:
    p = (FAC_TEST_OPT *)&gMenuAssembleTest[0];
    for ( i=0 ; i<DIM(gMenuAssembleTest) ; i++,p++  ) {
        if (test_conf_require(p->id)
            && if_test_again(again_flag,p->id))
        {
            result = p->test_func(p->para);
            test_write_result(result,p->id,&testResult);
        }
    }
//    init_sram(0);
    lcd_Cls();
	kb_Flush();
    if (!testResult) {
        fac_ctrl_set_test_detail(phase,testResult);
		fac_ctrl_set_autotest_res(phase, FAC_CTRL_SUCCESS);
        lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "%sͨ��",gFacPhaseName[phase]);
        kb_GetKey(2000);

#if PRODUCT_NAME == PRODUCT_EPP 
        //mk210��ͨ��װ���ʱ �ϻ��ܼ�ֱ�ӳɹ�
        log.aging_allok = 1;
        fac_write_aginglog(&log);
        fac_ctrl_set_autotest_res(FAC_PHASE_AGING, FAC_CTRL_SUCCESS);
        fac_ctrl_set_autotest_res(FAC_PHASE_PRODUCT, FAC_CTRL_SUCCESS);
#endif

        return OK;
    } else {
        fac_ctrl_set_test_detail(phase,testResult);
        fac_ctrl_set_autotest_res(phase, FAC_CTRL_FAIL);
        if(test_fail_view(testResult)) {
            TRACE("\r\n ��ǰresult:%x flag:%x",testResult,again_flag);
            if ( first == 1 ) {
                first = 0;
                again_flag = (~again_flag) | testResult;
            } else {
                again_flag = testResult;
            }
            TRACE_M("\r\n ����result:%x flag:%x",testResult,again_flag);
            goto test_auto_assemble_again;
        }

#if PRODUCT_NAME == PRODUCT_EPP 
        //mk210��ͨ��װ���ʱ �ϻ��ܼ�ֱ�ӳɹ�����ʧ��
        log.aging_allok = 0;
        fac_write_aginglog(&log);
        fac_ctrl_set_autotest_res(FAC_PHASE_AGING, FAC_CTRL_FAIL);
        fac_ctrl_set_autotest_res(FAC_PHASE_PRODUCT, FAC_CTRL_FAIL);
#endif

        return ERROR;
    }
}


int aging_charge_step(s_aging_log *log,char step)	//�ϻ���һ�γ��׶�
{
    int iRet = 0;
    int time = DEFAULT_AGING_CHARGE_TIME ;//���ʱ��
    static uint charge_start_counter = 0;
    static int charge_time_tmp = 0;
    uint tmp_counter2 = 0;
    int mv = 0;
    int j = 0;
    int endflag = 0;
    lcd_Cls();
    //����״̬
    iRet = sys_GetSupplyState();
    if (iRet == SUPPLY_VIN_BYON_STATE
        || iRet == SUPPLY_VIN_BYFULL_STATE)//����������,�е��
    {
        if(!step)//��¼��ʼ���ʱ��ʱ���
        {
            charge_time_tmp = 0;
            charge_start_counter = sys_GetCounter();
        }
        log->battery_error_info  |= (1<<2);
        log->charge_time = (sys_GetCounter() - charge_start_counter)/1000/60 + charge_time_tmp;
        if(log->charge_time >= time)//���ʱ�䳬��3.5Сʱ
        {
            lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "�ѳ����%d����",time);
        }
        else
        {
            lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "�ѳ��%d����",
                        log->charge_time);
        }
        sys_GetBattery(&mv);
        log->baterry_mv_actual = mv;
        if (iRet == SUPPLY_VIN_BYFULL_STATE)
        {
            lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv(��)", mv);
        }
        else
        {
            lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv", mv);
        }
        sys_DelayMs(2000);

    }
    else if (iRet == SUPPLY_BATTERY_NOR_STATE)//��ع���
    {
        sys_GetBattery(&mv);

        log->baterry_mv_actual = mv;
        if(log->charge_time >= time)//���ʱ�䳬��3.5Сʱ
        {
            log->baterry_mv1 = mv;
            if (mv < test_conf_aging_charge_mv())
            {
                log->battery_error_info |=  (1<<0);
            }
            log->battery_error_info &= (~(1<<2));
            endflag = 1;	//��һ�׶γ�����
        }
        else
        {
            lcd_Display(0,  0, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "���ʱ�䲻��!");
            lcd_Display(0,DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��������!");
            lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv", mv);
            tmp_counter2 = sys_GetCounter();
            charge_time_tmp = log->charge_time;
            log->battery_error_info |=  (1<<2);
            while (1)
            {
                if (sys_GetCounter() - tmp_counter2 >= (60*1000*30))	//��ʾ30����
                {
                    log->battery_error_info |=  (1<<0);
                    log->battery_error_info |=  (1<<2);
                    endflag = 1;
                    break;
                }

                j = sys_GetSupplyState();

                if (j == SUPPLY_VIN_BYON_STATE
                    || j == SUPPLY_VIN_BYFULL_STATE)
                {
                    //������������
                    tmp_counter2 = 0;
                    charge_start_counter = sys_GetCounter();
                    break;
                }
                lcd_SetLight(LIGHT_ON_MODE);
                sys_DelayMs(1000);
                sys_Beep();
                lcd_SetLight(LIGHT_OFF_MODE);
                sys_DelayMs(1000);
                sys_Beep();
            }
            lcd_SetLight(LIGHT_ON_MODE);
            //				goto BATTERY_OUT;
        }

    }
    if (endflag == 0)
    {
        return  -1;
    }
    else
    {
        return OK;
    }
}
int aging_discharge_step(s_aging_log *log,int step)//�ϻ���طŵ�׶�
{
	int endflag = 0;
	int mv = 0;
	static int tmp_counter1 = 0;
	int iRet = 0;
	int j = 0;
	static uint discharge_start_counter = 0;

		iRet = sys_GetSupplyState();
		if (!step)
		{
			tmp_counter1 = 0;
			discharge_start_counter = sys_GetCounter();//��¼��ʼ�ŵ�ʱ��ʱ���
		}
		lcd_Cls();
		if (iRet == SUPPLY_BATTERY_NOR_STATE)//��ع���
		{
			sys_GetBattery(&mv);
			log->baterry_mv_actual = mv;
			if (log->battery_error_info & (1<<0))
			{
				sys_BeepPro(BEEP_PWM_HZ, 2000, 0);
				lcd_Display(0,  0, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "����·����!");
				lcd_Display(0,  DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "battery_error_info:%d\r\n",log->battery_error_info);
				sys_DelayMs(2000);
			}

			log->discharge_time = (sys_GetCounter()-discharge_start_counter)/1000/60;

			lcd_Display(0,DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "����ϻ�%d����",
					   				log->discharge_time);
			lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv", mv);
			sys_DelayMs(2000);
		}
		else if (iRet == SUPPLY_VIN_BYON_STATE
				|| iRet == SUPPLY_VIN_BYFULL_STATE)//����������,�е��)
		{
			sys_GetBattery(&mv);
			log->baterry_mv_actual = mv;
			log->discharge_time = (sys_GetCounter()-discharge_start_counter)/1000/60;

			lcd_Display(0,  0, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "���ڽ��е���ϻ�");
			lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��γ�������!");
			lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,
								"���ϻ�%d����", log->discharge_time);
			lcd_Display(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv", mv);

			if (tmp_counter1 == 0)
			{
				tmp_counter1 = sys_GetCounter();
				while (1)
				{
					if (sys_GetCounter() - tmp_counter1 >= (60*1000*10))//����10����
					{
						//����10����,���ٱ���,�Լ����ϻ�,
						break;
					}
					j = sys_GetSupplyState();
					if (j == SUPPLY_BATTERY_NOR_STATE)	//�������γ�
					{
						tmp_counter1 = 0;
						break;
					}
					lcd_SetLight(LIGHT_ON_MODE);
				    sys_DelayMs(1000);
				    sys_Beep();
				    lcd_SetLight(LIGHT_OFF_MODE);
				    sys_DelayMs(1000);
				    sys_Beep();
				}
	//						tmp_counter = 0;

				lcd_SetLight(LIGHT_ON_MODE);
			//	goto BATTERY_OUT;
			}
			else
			{
				sys_BeepPro(BEEP_PWM_HZ, 2000, 0);
				sys_DelayMs(1000);
			}
		}

		if (log->discharge_time > test_conf_aging_discharge_time())		//Ĭ�Ϸŵ�3.5��Сʱ,�ɸ��������ļ��޸�
		{
			log->baterry_mv2 = mv;
			endflag = 1;
			if(mv < test_conf_aging_discharge_mv())//��ѹ����ֵ
			{
				log->battery_error_info |= (1<<1);
			}
			else
			{

			}

		}

		if (endflag == 0)
		{
			return -1;
		}
		else
		{
			return OK;
		}
}

int aging_recharge_step(s_aging_log *log)//�ϻ��������ٳ��׶�
{
//	int iRet = 0;
	int supply_state = 0;
	int mv = 0;
	int count = 0;
//	int charge_time = 0;
	int start_recharge_counter = 0;
	int full = 0;	//��⵽��س���
	uint full_counter = 0;//����ʱ�ļ���ֵ
	int charge_time_tmp = 0;
	int pause_flag = 0;	//����ڼ����������γ�,�øñ���1
	int iflag = 0;
	int icount = 0;
    char step = 0;
	kb_Flush();
		while (1)
		{
			lcd_Cls();

			supply_state = sys_GetSupplyState();
			if (supply_state == SUPPLY_BATTERY_NOR_STATE)//��ع���
			{
				sys_GetBattery(&mv);
				log->baterry_mv_actual = mv;
				if (pause_flag == 0)
				{
					full = 0;
					pause_flag = 1;
					charge_time_tmp = log->recharge_time;
				}
//				lcd_Display(0, (LCD_HEIGHT / 5) * 0, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "����ϻ��ѽ���");
				lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��������������");
				lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ǰ��ص�ѹ:%dmv", mv);
				if (log->recharge_time < DEFAULT_AGING_RECCHARGE_TIME && mv < DEFAULT_AGING_CHARGE_MV)
				{
					lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "���δ����!");
				}

			}
			else if (supply_state == SUPPLY_VIN_BYON_STATE
					|| supply_state == SUPPLY_VIN_BYFULL_STATE)//����������,�е��)
			{
				sys_GetBattery(&mv);
				log->baterry_mv_actual = mv;
				if (!step)	//��һ�ν��������
				{
//					sys_GetTime(log->recharge_start_rtc);
					start_recharge_counter = sys_GetCounter();
                    fac_write_aginglog(log);
                    step = 1;
				}
				if (pause_flag == 1)
				{
					pause_flag = 0;
					start_recharge_counter = sys_GetCounter();
				}
				log->recharge_time = (sys_GetCounter() - start_recharge_counter)/1000/60 + charge_time_tmp;
				lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "�ѳ��%d����",
					   			log->recharge_time);
				if (supply_state == SUPPLY_VIN_BYFULL_STATE)
				{
					if (full == 0)
					{
						lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "����");
						full_counter = sys_GetCounter();
						full = 1;
					}
					lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv(��)", mv);
				}
				else
				{
					if (full == 1)
					{
						lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "û����");
						full = 0;
						if (sys_GetCounter() - full_counter < (10*60*1000))//������10����֮��,����Ƿ����¿�ʼ���
						{
							log->battery_error_info |= (1<<4);
							log->aging_allok = 0;
							fac_write_aginglog(log);
							fac_ctrl_set_test_detail(FAC_PHASE_AGING, 1<<1);//��¼��ز���
							fac_ctrl_set_autotest_res(FAC_PHASE_AGING, FAC_CTRL_FAIL);//�ϻ�ʧ��(�ܼ�ʱ����ʾ�������)
						}
					}
					lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv", mv);
				}
			}
			lcd_Display(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��[ȷ��]�鿴�ϻ����");

//			sys_GetTime(log->actual_rtc);
			if (count % 100 == 0)
			{
				fac_write_aginglog(log);
			}
			count++;
			if (kb_Hit())
			{
				if (kb_GetKey(100) == KEY_ENTER)
				{
					show_aging_log_new(0);
				}
			}
#if 0
			if ( (log->recharge_time >= DEFAULT_AGING_RECCHARGE_TIME) && (iflag == 0))
			{
				iflag = 1;
				itime = log->recharge_time;
			}
#endif
            if ( log->recharge_time >= DEFAULT_AGING_RECCHARGE_TIME ) 
            {
                iflag = 1;
            }

//			if(iflag == 1 && (log->recharge_time > itime + 10))
            if ( iflag == 1) 
			{
				if(mv >= test_conf_aging_charge_mv())
				{
					log->aging_allok = 1;
                    fac_write_aginglog(log);
				}
				else
				{
					lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "ʧ��");
					kb_GetKey(-1);
					//ʱ�䵽��û����
					log->battery_error_info |= (1<<4);
					log->aging_allok = 0;
                    fac_write_aginglog(log);
					goto FAIL;
				}
				icount++;
				if((icount > 20) || (full == 1))//20�κ󣬻�ɹ����˳�
				{
//					sys_DelayMs(500);
                    lcd_Cls();
					lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY|DISP_INVLINE, "�ϻ��ɹ�");
					lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE|DISP_MEDIACY, "��ص�ѹ:%dmv",mv);
                    sys_DelayMs(500);
                    return 0;
				}
			}
			sys_DelayMs(2000);
		}

FAIL:
	return ERROR;
}
/* 
 * test_aging_new - [GENERIC]  �ϻ�����
 * @ 
 */
#if PRODUCT_NAME == PRODUCT_MPOS
int test_aging_new(int mode)
{
	int result = 0;
	int iRet = 0;
	uint begcounter = 0;
	int testTimer = 0;
	int lcdCnt = 0;
    int keyCnt = 0;
	int icCnt = 0;
	int magCnt = 0;
	int keyBegCounter = 0;
	int step = 0;
	int battery_step = 0; //�ϻ�ʱ��صĽ׶�,0-��ʼʱ���׶�,1-�ŵ�׶�,2-�ڶ��γ��׶�
	int ic_len = 0;
	uchar ic_buff[256];
    int id = 0;
    char chargestep = 0;
    char dischargestep = 0;

	s_aging_log log;
	int log_size = sizeof(s_aging_log);
	memset(&log, 0, log_size);	//���log����

    fac_read_aginglog(&id,&log);
	if (log.aging_allok == 1)
	{
        lcd_Cls();
		lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE,
                "�ϻ������Ѿ��ɹ�!");
        lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE,
                "[1]�ز� [2]�˳�");
        while (1)
        {
            switch (kb_GetKey(-1))
            {
                case KEY1:
					iRet = backdoor_password_verify("456789");
					if(iRet == OK)
					{
						break;
					}
					else
					{
						lcd_cls();
						lcd_display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE,
                					"�ϻ������Ѿ��ɹ�!");
        				lcd_display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE,
                					"[1]�ز� [2]�˳�");
						continue;
					}
                    break;
                case KEY2:
                    return 0;
                case KEY_CANCEL:
                    return 0;
                default:
                    continue;
            }
            break;
        }
	}
	result = fac_ctrl_show_tip(FAC_PHASE_AGING);
	if (result != YES)
		return -1;

    iRet = sys_GetSupplyState();
	memset(&log, 0, log_size);	//���log����
    while (iRet != SUPPLY_VIN_BYON_STATE
           && iRet != SUPPLY_VIN_BYFULL_STATE)//û��������
    {
        iRet = sys_GetSupplyState();
        lcd_Display(0,DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE,
                    "�����������!");
        sys_BeepPro(BEEP_PWM_HZ, 500, 0);
        sys_DelayMs(1000);
    }
	begcounter = sys_GetCounter();
    while(1)
    {
        testTimer++;
        if(battery_step == 1)
		{
			lcd_Cls();
            hw_led_on(LED_ALL);
            lcd_SetLight(LIGHT_ON_MODE);
            lcd_display(0,DISP_FONT_LINE2,DISP_FONT|DISP_CLRLINE,"���ڴ�����");
#ifdef CFG_BLUETOOTH
            bt_open();
#endif
            if (sys_GetModuleType(MODULE_RF_TYPE) != MODULE_NOTEXIST)
            {
#ifdef CFG_RFID
//                rfid_open(0); //�ϻ�������Ƶ
#endif
            }
		}
        lcdCnt++;

		lcd_Cls();
#ifdef  CFG_ICCARD
		lcdDispMultiLang(0, 0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY, "IC��,�ſ��ϵ�", "IC,mag Powerup");
		lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_MEDIACY, "IC�������ϵ�");
		iRet = iccard_powerup(USERCARD, &ic_len, ic_buff);
#endif
#ifdef CFG_MAGCARD
		lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_MEDIACY, "�ſ������ϵ�");
		mag_Open();
		lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_MEDIACY, "�ſ��ϵ�ɹ�");
#endif
		kb_GetKey(1000);
		icCnt++;
		magCnt++;

        keyCnt++;
		lcd_Cls();
        kb_Flush();

        keyBegCounter = sys_GetCounter();
        lcdDispMultiLang(0, 0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY, "��������", "TEST SERIAL");
        while (1)
        {
            if (kb_Hit())
            {
                kb_GetKey(20);
                goto FAIL;
            }
            if ((sys_GetCounter() - keyBegCounter) > 1000)
            {
                break;
            }
        }
        switch (battery_step)
        {
        case 0:
            iRet = aging_charge_step(&log,chargestep);
            if (iRet == OK)
            {
                battery_step = 1;
            }
            chargestep = 1;
            break;
        case 1:
            iRet = aging_discharge_step(&log,dischargestep);
            if (iRet == OK)
            {
                battery_step = 2;
            }
            dischargestep = 1;
            break;
        default:
            break;
        }
		lcd_Cls();
		log.totaltimes = testTimer;
		log.total_time = (sys_GetCounter() - begcounter) / 1000/ 60;
		fac_write_aginglog (&log);
		if ((log.battery_error_info & (1<<0)))
		{
			//����·����,ֱ�ӽ����ϻ�
			goto FAIL;
		}
		if ((log.battery_error_info & (1<<1)))
		{
			//��طŵ����ʧ��
			fac_ctrl_set_test_detail(FAC_PHASE_AGING, 1);//����й��Ͼͼ�¼����
			goto FAIL;
		}
		if (battery_step == 2)
		{
			//�ŵ�ʱ�����,
			goto SUCCESS;
		}
    }
SUCCESS:
	fac_ctrl_set_autotest_res(FAC_PHASE_AGING, FAC_CTRL_SUCCESS);
    lcd_SetLight(LIGHT_TIMER_MODE);
#ifdef CFG_BLUETOOTH
    bt_close();
#endif
	if (sys_GetModuleType(MODULE_RF_TYPE) != MODULE_NOTEXIST)
	{
#ifdef CFG_RFID
//	   rfid_close(); //�ϻ�������Ƶ
#endif
	}
    hw_led_off(LED_ALL);
    iRet = aging_recharge_step(&log);
    if (iRet == ERROR)
    {
        goto FAIL;
    }
    while(1)
    {
        if ( sys_GetSupplyState() ==  SUPPLY_BATTERY_NOR_STATE ) {
            sys_Shutdown();
        }
        s_DelayMs(500);
    }
//    return 0;
FAIL:
	lcd_SetLight(LIGHT_TIMER_MODE);
    hw_led_off(LED_ALL);
#ifdef CFG_BLUETOOTH
    bt_close();
#endif
	if (sys_GetModuleType(MODULE_RF_TYPE) != MODULE_NOTEXIST)
	{
#ifdef CFG_RFID
//	   rfid_close(); //�ϻ�������Ƶ
#endif
	}
	lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_CLRLINE, "��%4d�ֲ���ʧ��", testTimer);
	lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "ʧ��ԭ��:%d",log.battery_error_info);
    lcd_Display(0, DISP_FONT_LINE3, DISP_FONT|DISP_CLRLINE, "ʵʱ��ѹ:%d",log.baterry_mv_actual);
    lcd_Display(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE, "������ֹ!");

	log.error_timer = testTimer;
	log.fail_step = step;
    fac_write_aginglog(&log);

	while (1)
	{
		lcd_SetLight(LIGHT_ON_MODE);
	    sys_DelayMs(1000);
	    sys_Beep();
	    lcd_SetLight(LIGHT_OFF_MODE);
	    sys_DelayMs(1000);
	    sys_Beep();
		if (kb_Hit())
		{
			kb_GetKey(50);
			break;
		}
	}
	lcd_SetLight(LIGHT_TIMER_MODE);

	while (1)
	{
		show_aging_log_new(0);
	}
}
#else
//MK210�ϻ�
int test_aging_new(int mode)
{
	uint begcounter = 0;
	uint curcounter = 0;
	int testTimer = 0;
	int lcdCnt = 0;
    int keyCnt = 0;
	int icCnt = 0;
	int magCnt = 0;
	int keyBegCounter = 0;
	int ic_len = 0;
	uchar ic_buff[256];

	s_aging_log log;
	memset(&log, 0, sizeof(s_aging_log));	//���log����
    log.aging_allok = 1;
    fac_write_aginglog(&log);
	fac_ctrl_set_autotest_res(FAC_PHASE_AGING, FAC_CTRL_SUCCESS);

	begcounter = sys_GetCounter();
    lcd_Cls();
    while(1)
    {
        testTimer++;
//        hw_led_on(LED_ALL);
        lcd_SetLight(LIGHT_ON_MODE);
        lcdCnt++;


#ifdef  CFG_ICCARD
		lcdDispMultiLang(0, 0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY|DISP_CLRLINE, "IC���ϵ�", "IC,mag Powerup");
		lcd_Display(0, DISP_FONT_LINE1, DISP_FONT|DISP_MEDIACY|DISP_CLRLINE, "SAM0�������ϵ�");
		iccard_powerup(USERCARD, &ic_len, ic_buff);
		lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_MEDIACY|DISP_CLRLINE, "SAM1�������ϵ�");
		iccard_powerup(SAM1SLOT, &ic_len, ic_buff);
#endif
		s_DelayMs(1000);
		icCnt++;
		magCnt++;

        keyCnt++;
		lcd_Cls();
        kb_Flush();

        keyBegCounter = sys_GetCounter();
        lcdDispMultiLang(0, 0, DISP_FONT|DISP_INVLINE|DISP_MEDIACY, "��������", "TEST SERIAL");
        curcounter = sys_GetCounter();
        lcdDispMultiLang(0, DISP_FONT_LINE4, DISP_FONT|DISP_CLRLINE, 
                         "ʱ��:%d����", "Time:%dmins",(curcounter-begcounter)/1000/60);
        while (1)
        {
            if (kb_Hit())
            {
                kb_GetKey(20);
                goto SUCCESS;
            }
            if ((sys_GetCounter() - keyBegCounter) > 1000)
            {
                break;
            }
        }

    }
SUCCESS:
	lcd_SetLight(LIGHT_TIMER_MODE);
    hw_led_off(LED_ALL);
    return OK;
}

#endif
#if 0
uint Fac_SRAM(uint mode)
{
#if  defined(CFG_TAMPER)
    ST_MMAP_KEY mmk;
    CLRBUF(mmk.facchk);
    hw_dryice_read_key((FPOS(ST_MMAP_KEY,facchk)>>2),1,(uint32_t *)&mmk.facchk);
    TRACE_BUF("fack",mmk.facchk,4);
    if(mode == 0)
    {
        if(memcmp(mmk.facchk,STR_SRAM,FUN_LEN) == 0)
        {
            //����
            return 0;
        }
        else
        {
            //�����쳣
            return 1;
        }
    }
    else
    {
        if(memcmp(mmk.facchk,STR_SRAM,FUN_LEN) != 0)
        {
            sys_tamper_open(mode);
            RNG_FillRandom((uint8_t *)&mmk, MMAP_KEY_LEN);
            memcpy(mmk.facchk,STR_SRAM,FUN_LEN);
            hw_dryice_write_key((FPOS(ST_MMAP_KEY,aucMMK)>>2),
                                ((MK_UNIT_LEN+ELRCK_LEN+CHECK_LEN+FUN_LEN)>>2),
                                (uint32_t *)&mmk);
            hw_dryice_read_key((FPOS(ST_MMAP_KEY,facchk)>>2),1,(uint32_t *)&mmk.facchk);
            return (memcmp(mmk.facchk,STR_SRAM,FUN_LEN) != 0);
        }
        return 0;
    }
#else
    return 0;
#endif
}
#endif

int test_conf_init(void)
{
	//Ĭ��ֵ
	gFacTest->tConfig.module_config = DEFAULT_MODULE_CFG;
	gFacTest->tConfig.mag_success_num = DEFAULT_MAG_SUCCESS_NUM;
	gFacTest->tConfig.sam_test_num = DEFAULT_SAM_NUM;
	gFacTest->tConfig.aging_charge_mv = DEFAULT_AGING_CHARGE_MV;
	gFacTest->tConfig.aging_discharge_mv = DEFAULT_AGING_DISCHARGE_MV;
	gFacTest->tConfig.aging_discharge_time = DEFAULT_AGING_DISCHARGE_TIME;
	gFacTest->tConfig.icsq = DEFAULT_SIGNS_VALUE;
	gFacTest->tConfig.ispecial = DEFAULT_SPEICAL_NO;
	TRACE_M("\r\nĬ������Ϊ0x%x", gFacTest->tConfig.module_config);
    return 0;
}
/* 
 * test_oqc - OQC���
 * @ 
 */
int test_oqc(int mode)
{
    int result;
//	int icsq = 0;
    uint i;
    uint first=1,again_flag = UINT_MAX;
    uint testResult = 0;
    FAC_TEST_OPT *p;
//    uint phase;
	//��ʾ�ϴ��ϻ����
    s_aging_log log;
	memset(&log, 0x00, sizeof(s_aging_log));
    uchar time[8];
//	show_aging_log_new(0);

//    phase = FAC_PHASE_PRODUCT;
//	if (fac_ctrl_show_tip(phase) != YES)
//	{
//		return OK;
//	}
    lcd_cls();
    lcd_Display(0,DISP_FONT_LINE0,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"OQC��(���ܼ�����һ��)");
    lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"�����������");
    kb_GetKey(-1);

//    lcd_cls();
//    lcd_Display(0,DISP_FONT_LINE0,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"1-����͹���");
//    if ( kb_GetKey(-1) == KEY1 ) {
//        gSystem.lpwr.bm.enable = 0;
//        sys_SleepWaitForEvent(EVENT_KEYPRESS,1,0);
//        gSystem.lpwr.bm.enable = 1;
//    }

    lcd_cls();
    gFacTest = malloc(sizeof(FAC_STAT_t));
    if ( gFacTest == NULL ) {
        return 1;
    }
    memset((uchar *)gFacTest,0,sizeof(FAC_STAT_t));
    test_conf_init();
    gTestStage = STAGE_OQC;

//    TRACE_M("\r\n %s��ʼ",gFacPhaseName[phase]);
    memset(time,0,sizeof(time));
	if(test_rtc_autostart(0, time)) {
        free(gFacTest);
        return ERROR;
    }
//	fac_ctrl_set_cur_phase(phase);
//    gTestStage = phase;
test_auto_final_again:
    p = (FAC_TEST_OPT *)&gMenuFinalTest[0];
    for ( i=0 ; i<DIM(gMenuFinalTest) ; i++,p++  ) {
        if (test_conf_require(p->id)
            && if_test_again(again_flag,p->id))
        {
            result = p->test_func(p->para);
            test_write_result(result,p->id,&testResult);
        }
    }
//    init_sram(0);
    lcd_Cls();
	kb_Flush();
    if (!testResult) {
//        fac_ctrl_set_test_detail(phase,testResult);
//        fac_ctrl_set_autotest_res(phase, FAC_CTRL_SUCCESS);
        lcd_Display(0, DISP_FONT_LINE2, DISP_FONT|DISP_CLRLINE, "%sͨ��","OQC");
        kb_GetKey(-1);
        free(gFacTest);
        return OK;
    } else {
//        fac_ctrl_set_test_detail(phase,testResult);
//        fac_ctrl_set_autotest_res(phase, FAC_CTRL_FAIL);
        if(test_fail_view(testResult)) {
            TRACE_M("\r\n ��ǰresult:%x flag:%x",testResult,again_flag);
            if ( first == 1 ) {
                first = 0;
                again_flag = (~again_flag) | testResult;
            } else {
                again_flag = testResult;
            }
            TRACE_M("\r\n ����result:%x flag:%x",testResult,again_flag);
            goto test_auto_final_again;
        }
        free(gFacTest);
        return ERROR;
    }
}

#if PRODUCT_NAME == PRODUCT_EPP 
int fac_openApp(void)
{
    int ret;

    lcd_Cls();
    ret = fac_ctrl_get_autotest_res(FAC_PHASE_ASSEMBLE);
    if ((ret!=FAC_CTRL_SUCCESS)
        && (ret!=FAC_CTRL_FORCE_SUC) )
    {
        lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"װ���δͨ��");
        kb_getkey(-1);
    }else{
        if ( test_sram_auto(0) == OK ) {
            lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"1-����Ӧ��");
            if ( kb_getkey(-1) == KEY1 ) {
                if ( set_appenable(1) == 0 ) {
                    fac_ctrl_set_fac_ctrl_flag(1);
                    sys_Reset();
                    while(1);
                }else
                    lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"����ʧ��");
                kb_getkey(-1);
            }
        }else{
            lcd_Display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY|DISP_CLRLINE,"�����쳣");
            kb_getkey(-1);
        }
    }
    return 1;
}
#endif
/* 
 * fac_menu_top - [GENERIC] ��1���˵� 
 *    mode: 0-�������Բ˵�  1-������Բ˵�
 * @ 
 */
int fac_menu_top(int mode)
{
    extern const VERSION_INFO_NEW gCtrlVerInfo;
    gFacTest = malloc(sizeof(FAC_STAT_t));
    if ( gFacTest == NULL ) {
        return 1;
    }
    memset((uchar *)gFacTest,0,sizeof(FAC_STAT_t));
    test_conf_init();
    if ( mode ) {
        local_test_menu(0);
    } else {
        while ( 1 ) {
            kb_flush();
            lcd_Cls();
            lcd_Display(0,DISP_FONT_LINE0,DISP_FONT|DISP_MEDIACY|DISP_INVLINE,"%s",FAC_MENU_CTITLE_TOP);
            lcd_Display(0,DISP_FONT_LINE1,DISP_FONT,"1-���� 2-װ�� 3-�ܼ�");
            lcd_Display(0,DISP_FONT_LINE2,DISP_FONT,"4-�ϻ� 5-���� 6-����");
#if PRODUCT_NAME == PRODUCT_MPOS
            lcd_Display(0,DISP_FONT_LINE3,DISP_FONT,"7-�ܿ� ");
#else

            lcd_Display(0,DISP_FONT_LINE3,DISP_FONT,"7-�ܿ� 8-����Ӧ��");
#endif
            sys_auto_poweroff(POWEROFF_OPEN,FAC_AUTO_POWEROFF_TIME);
            switch ( kb_GetKey(-1) )
            {
            case KEY1 :
//                debugging(0);
                break;
            case KEY2 :
                test_auto_assemble(0);
                break;
            case KEY3 :
                test_auto_final(0);
                break;
            case KEY4 :
                sys_auto_poweroff(POWEROFF_CLOSE,FAC_AUTO_POWEROFF_TIME);
                test_aging_new(0);
                sys_auto_poweroff(POWEROFF_OPEN,FAC_AUTO_POWEROFF_TIME);
                break;
            case KEY5 :
                local_test_menu(0);
                break;
            case KEY6 :
#ifdef CFG_BLUETOOTH
                test_bt(0);
#endif
                break;
            case KEY7:
                dl_process(0);
                break;
            case KEY8:
#if PRODUCT_NAME == PRODUCT_MPOS
                show_aging_log_new(0);
#else
                fac_openApp();
#endif
                break;
            case KEY_CANCEL:
                break;
            default :
                break;
            }
        }
    }
    free(gFacTest);
    gFacTest = NULL;
    sys_auto_poweroff(POWEROFF_CLOSE,FAC_AUTO_POWEROFF_TIME);
    return 0;
}
//int show_test_auto_res(char *title, int phase,FAC_TESTINFO facinfo)
//{
//    const char *prompt[] = {"δ����","�ɹ�","ʧ��","ǿ�Ƴɹ�"};
//    int j,k,flg;
//    lcd_cls();
//    lcd_display(0, DISP_FONT_LINE0, DISP_FONT | DISP_CLRLINE | DISP_MEDIACY | DISP_INVLINE, "%s",title);
//    if (phase == 0)
//    {
//        lcd_display(0,DISP_FONT_LINE1,DISP_FONT|DISP_CLRLINE,"�����:%s",facinfo.mbno);
//        lcd_display(0,DISP_FONT_LINE2,DISP_FONT|DISP_CLRLINE,"��ˮ��:%s",facinfo.voucherno);
//        lcd_display(0,DISP_FONT_LINE3,DISP_FONT|DISP_CLRLINE,"ģ��:%x:%x:%x:%x",facinfo.module_res[0],facinfo.module_res[1],facinfo.module_res[2],facinfo.module_res[3]);
//        lcd_display(0,DISP_FONT_LINE4,DISP_FONT|DISP_CLRLINE,"�ܿ���Ϣ:%d",facinfo.fac_ctrl_flag);
//    }
//    else
//    {
//        phase -= 1;
//        j = facinfo.auto_res[phase];
//        k = (j>>30)&0x03;
//        lcd_display(0,DISP_FONT_LINE1,DISP_FONT|DISP_MEDIACY,"%s-%04XH",prompt[k],j);
//        if ( phase > FAC_PHASE_ASSEMBLE 
//             && (j == FAC_CTRL_FAIL)) {
//            flg = 0;
//            lcd_Goto(0,DISP_FONT_LINE2);
//            if ( !IFBIT(k,AUTO_TEST_SHIFT_SRAM) ) {
//                if ( phase == FAC_PHASE_PRODUCT) {
//                    lcd_Printf("���� ");
//                    flg = 1;
//                }
//            }
//            if ( !IFBIT(k,AUTO_TEST_SHIFT_KEY) ) {
//                lcd_Printf("���� ");
//                flg = 1;
//            }
//            if ( !IFBIT(k,AUTO_TEST_SHIFT_LCD) ) {
//                lcd_Printf("���� ");
//                flg = 1;
//            }
//            if ( !IFBIT(k,AUTO_TEST_SHIFT_MAG) ) {
//                lcd_Printf("�ſ� ");
//                flg = 1;
//            }
//            if ( !IFBIT(k,AUTO_TEST_SHIFT_IC) ) {
//                lcd_Printf("IC�� ");
//                flg = 1;
//            }
//            if ( flg ) {
//                lcd_Printf("���� ");
//            } else {
//                lcd_Printf("�ɹ� ");
//            }
//        }
//    }
//    kb_getkey(-1);
//    return 0;
//}

//��̨���������ܿؼ�ɹ���ʶ 0-ʧ�� 1-�ɹ�
//int fac_set_facctrl_mode(char mode)
//{
//    int ret;
//    const char *prompt[]={"�ɹ�","ʧ��"};
//    ret = backdoor_password_verify("456789");
//    if ( ret ) {
//        return ERROR;
//    }
//    while(1)
//    {
//        lcd_cls();
//        lcd_display(0,DISP_FONT_LINE0,DISP_FONT|DISP_CLRLINE|DISP_MEDIACY|DISP_INVLINE,"�ܿؼ�����");
//        lcd_display(0,DISP_FONT_LINE1,DISP_FONT|DISP_CLRLINE|DISP_MEDIACY,"״̬:%s",fac_ctrl_get_fac_ctrl_flag()?prompt[0]:prompt[1]);
//        lcd_display(0,DISP_FONT_LINE2,DISP_FONT|DISP_CLRLINE,"1-ǿ�Ƴɹ�");
//        lcd_display(0,DISP_FONT_LINE3,DISP_FONT|DISP_CLRLINE,"2-ǿ��ʧ��");
//        switch(kb_getkey(-1))
//        {
//        case KEY1:
//            fac_ctrl_set_fac_ctrl_flag(1);
//            return OK;
//        case KEY2:
//            fac_ctrl_set_fac_ctrl_flag(0);
//            return OK;
//        default:
//            break;
//        }
//    }
//}
/* 
 * fac_main - [GENERIC] �ܿ���Ϣ��ѯ 
 * @ 
 */
//int fac_disp_log(int mode)
//{
//	FAC_TESTINFO facinfo;
//    const char *prompt[]={" ","δ"};
//    const char *title[]={"������Ϣ","����","װ��","�ܼ�","�ϻ�"};
//    int i,key;
//	while (1)
//	{
//        memset(&facinfo, 0, sizeof(FAC_TESTINFO));
//
//        fac_ctrl_get_fac_testinfo(&facinfo);
//		lcd_cls();
//		lcd_Display(0, DISP_FONT_LINE0, DISP_FONT | DISP_CLRLINE | DISP_MEDIACY | DISP_INVLINE, "�����ܿ���Ϣ");
//        lcd_Display(0, DISP_FONT_LINE1, DISP_FONT | DISP_CLRLINE, "1-%s-%sͨ��",title[0],
//                    (facinfo. fac_ctrl_flag == 1)?prompt[0]:prompt[1]);
//        for ( i=1 ; i<DIM(title) ; i++ ) {
//            lcd_Display(LCD_LENGTH/2*((i-1)%2), DISP_FONT_LINE2+DISP_FONT*((i-1)/2), DISP_FONT, "%d-%s",i+1,title[i]);
//        }
//        key = kb_getkey(-1);
//        if ( key >= KEY1 && key <= KEY4 ) {
//            key -= KEY1;
//            show_test_auto_res((char *)title[key],key,facinfo);
//        } else if(key == KEY5){
//            show_aging_log_new(0);
//        } else if(key == KEY0){
////            fac_ctrl_set_cur_phase(FAC_PHASE_DEBUG);
////            fac_ctrl_set_autotest_res(FAC_PHASE_DEBUG,0);
//        } else if(key == KEY_F1){
//            if ( read_user_flag() == USER_SUPER) {
//                fac_set_facctrl_mode(0);
//            }
//        } else if(key == KEY_CANCEL){
//            return -1;
//        }
//	}
//}
/* 
 * fac_main - [GENERIC] ��������������
 * @ 
 */
int fac_main (int mode)
{
    // �жϹܿؼ��Ƿ�ɹ�,������������
    uint32_t lwpr_back;
    
    
    lwpr_back = gSystem.lpwr.periph;
        // �жϹܿؼ��Ƿ�ɹ�,������������
    if(gwp30SysMemory.SysCtrl.bit.factoryisok == 1)
    {
        return 0;
    }
    else
    {
        gSystem.lpwr.bm.enable = 1;
    }
    
    
    hw_led_on(LED_ALL);
    power_down_switch_set(OFF);  //�رչػ���Դ���
    fac_mb_main(0);
 //   fac_menu_top(0);
    gSystem.lpwr.periph = lwpr_back;
    power_down_switch_set(ON);  //�����ػ���Դ���
    hw_led_off(LED_ALL);
    
    return 0;
}		/* -----  end of function fac_main  ----- */


#endif



