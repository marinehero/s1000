/*
 * =====================================================================================
 *
 *       Filename:  factory.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  1/14/2015 3:17:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yehf (), yehf@itep.com.cn
 *        Company:  START
 *
 * =====================================================================================
 */
#ifndef __FACTORY_H__
#define __FACTORY_H__ 
// ������


enum {
    STAGE_Debuging = FAC_PHASE_DEBUG,  //�忨��
    STAGE_Assemble = FAC_PHASE_ASSEMBLE,      //װ���
    STAGE_Aging = FAC_PHASE_AGING,         //�ϻ���
    STAGE_Product = FAC_PHASE_PRODUCT,
    STAGE_Individual,    //�����
    STAGE_Bottomboard,   //�װ����
    STAGE_OQC,           //OQC���
    STAGE_Err
};

typedef struct
{
	uint module_config;     //��Ӧģ���Ƿ����Զ������в���
	uchar mag_success_num;  //�ſ�����ˢ������ͨ��
	uchar sam_test_num;     //samӦ�⼸�ſ�
	uchar icsq;             //�����ź�ֵ
	uchar ispecial;         //�Ƿ�Ϊ�������칤�����ϻ�����
	ushort aging_charge_mv;   //�ϻ�����ʱ,���Ӧ��絽���ٵ�ѹ
	ushort aging_discharge_mv;//�ϻ�����ʱ,��طŵ����ѹӦ���ڶ���
	ushort aging_discharge_time;//�ϻ����Ե��Ӧ�ŵ���
    uchar rfu[2];
}S_TEST_CONFIG;


typedef struct __FAC_STAT_t {
    uchar stage;         // ���Խ׶�
    uchar iccardinslot;  // 1-����λ  0-������λ
    uchar charge_err;
    uchar rfu;
    uchar time[8];
    int   iccardret[5];
    S_TEST_CONFIG tConfig;
}FAC_STAT_t;				/* ----------  end of struct FAC_STAT_t  ---------- */

typedef int (*FAC_TEST_CALLBACK)(int param);
typedef struct _FAC_TEST_OPT
{
    uint id;
    uint para;
    int (*test_func)(int);   //���Է���
}FAC_TEST_OPT;


/*-----------------------------------------------------------------------------}
 *  ���Ժ�
 *-----------------------------------------------------------------------------{*/
#define DEBUG_FAC
#if defined (DEBUG_FAC)
#define TRACE_M(...)							TRACE(__VA_ARGS__)
#define DISPBUF_M(ptitle, len, mode,pBuf)		DebugDispbuf(ptitle, len, mode,pBuf)
#else
#define TRACE_M(...)							do{}while(0)
#define DISPBUF_M(ptitle, len, mode,pBuf)		do{}while(0)
#endif

/*-----------------------------------------------------------------------------}
 *  ȫ����������
 *-----------------------------------------------------------------------------{*/
extern FAC_STAT_t  *gFacTest;
#define gTestStage gFacTest->stage
#define ic_not_in_slot gFacTest->iccardinslot
#define battery_charge_err gFacTest->charge_err


/*-----------------------------------------------------------------------------}
 *  ��������
 *-----------------------------------------------------------------------------{*/
extern int fac_main (int mode);
extern int test_serial_self(int mode);
extern int test_rtc_autocheck(int mode);
extern int test_battery(int mode);
extern int test_rtc(int mode);
extern int test_usb_manual(int mode);
extern int test_keyboard(int mode);
extern int test_display(int mode);
extern int test_led(int mode);
extern int test_bt(int mode);
extern int test_magcard_auto(int mode);
extern int test_rfid_auto(int mode);
extern int test_iccard_auto_new(int mode);
extern int test_rfid(int mode);
extern int local_test_menu(int mode);
extern int fac_menu_top(int mode);
extern int init_sram(int mode);
extern int test_sram_auto(int mode);
extern void BeepErr(void);
extern void TestEndHookFail(void);
int test_conf_init(void);
int test_conf_require(int module);
int test_conf_mag_success(void);
int test_conf_sam_num(void);
int test_conf_disp(int mode);
int test_conf_aging_discharge_mv(void);
int test_conf_aging_charge_mv(void);
int test_conf_aging_discharge_time(void);
int Set_conf_aging_dicharge_time(int time);
int test_conf_ic_retest_count(void);
int test_conf_icsq(void);
int test_conf_aging_special(void);
int fac_disp_log(int mode);
int test_oqc(int mode);
#endif


