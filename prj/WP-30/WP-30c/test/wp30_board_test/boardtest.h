#ifndef _BOARDTEST_H_
#define _BOARDTEST_H_
// ģ����
#define MODULE_NAME_SHORT_CIRCUIT   "SCT" // short circuit
#define MODULE_NAME_KEY             "KEY"
#define MODULE_NAME_PRN             "PRN"
#define MODULE_NAME_SCANNER         "SCA" // scanner
#define MODULE_NAME_LCD             "LCD"
#define MODULE_NAME_ICCARD          "ICC" // ICcard
#define MODULE_NAME_SDCARD          "SDC" // SDCard
#define MODULE_NAME_SAMCARD         "SAM" // Modem
#define MODULE_NAME_MAGCARD         "MAG"
#define MODULE_NAME_GPRS            "GPR" // GPRS
#define MODULE_NAME_MODEM           "MOD" // Modem
#define MODULE_NAME_USB             "USB"
#define MODULE_NAME_SAFE            "SAF"
#define MODULE_NAME_SYS             "SYS"
#define MODULE_NAME_RTC             "RTC"
#define MODULE_NAME_PWR				"PWR"
#define MODULE_NAME_LED             "LED"
#define MODULE_NAME_BEEP            "BEP"
#define MODULE_NAME_BATTERY         "BAT"
#define MODULE_NAME_BLUETOOTH       "BLT"
#define MODULE_NAME_RFID            "RFID" //
#define MODULE_NAME_PRN_ADC         "PRN_ADC"
#define MODULE_NAME_RTC_SET         "RTC_SET"//����RTCʱ��

#define MODULE_NAME_ANDROID_UART     "AND_UART"
#define MODULE_NAME_ANDROID_GPIO     "AND_GPIO"
#define MODULE_NAME_ANDROID_CHARGING "AND_CHARGING"
#define MODULE_NAME_ANDROID_TRUMPET  "AND_TRU"

// ģ���
#define MODULE_SHORT_CIRCUIT   0
#define MODULE_KEY             1
#define MODULE_PRN             2
#define MODULE_SCANNERS        3
#define MODULE_LCD             4
#define MODULE_ICCARD          5
#define MODULE_SDCARD          6
#define MODULE_SAMCARD         7
#define MODULE_MAGCARD         8
#define MODULE_GPRS            9
#define MODULE_MODEM           10
#define MODULE_USB             11
#define MODULE_SAFE            12
#define MODULE_SYS             13
#define MODULE_RTC             14
#define MODULE_PWR			   15
#define MODULE_LED             16
#define MODULE_BEEP            17
#define MODULE_BATTERY         18
#define MODULE_BLUETOOTH       19
#define MODULE_ICCARD_PULL     20
#define MODULE_RFID            21
#define MODULE_PRN_ADC         22
#define MODULE_RTC_SET         23

#define MODULE_ANDROID_UART     24
#define MODULE_ANDROID_GPIO     25
#define MODULE_ANDROID_CHARGING 26
#define MODULE_ANDROID_TRUMPET  27




typedef enum _FAC_PIN_ID_t{
	 ID_KEY_COL0 =COMBINE16(MODULE_KEY,0),ID_KEY_COL1,ID_KEY_COL2,ID_KEY_COL3,
	              ID_KEY_ROW0,ID_KEY_ROW1,ID_KEY_ROW2,ID_KEY_ROW3,
	 ID_IC_VCC   =COMBINE16(MODULE_ICCARD,0),ID_IC_RST,  ID_IC_CLK, ID_IC_INT, ID_IC_IO, ID_IC_C4, ID_IC_C8,
	 ID_SAM_VCC  =COMBINE16(MODULE_SAMCARD,0),ID_SAM_RST, ID_SAM_CLK, ID_SAM_IO,  ID_SAM_CS1, ID_SAM_CS2,
	 ID_MAG_VCC  =COMBINE16(MODULE_MAGCARD,0),ID_MAG_RST, ID_MAG_CLK, ID_MAG_DATA,ID_MAG_HEAD_A,ID_MAG_HEAD_B,ID_MAG_HEAD_COMMON,ID_MAG_STROBE,
	 ID_LCD_CS   =COMBINE16(MODULE_LCD,0),ID_LCD_BL,  ID_LCD_XA0, ID_LCD_DATA,ID_LCD_CLK,
	 ID_PWR_SLEEP=COMBINE16(MODULE_PWR,0),ID_PWR_VCC_1616,ID_PWR_VBAT,ID_PWR_V_LIBAT,ID_PWR_VCC3_3,ID_PWR_VCC5_0,
	    	      ID_PWR_VCC5_0_1_CPU,ID_PWR_VCC5_0_2_CPU,
	 ID_SIM_RST  =COMBINE16(MODULE_GPRS,0),ID_SIM_CLK,ID_SIM_IO,
	 ID_PRN_DATA =COMBINE16(MODULE_PRN,0),ID_PRN_CLK,ID_PRN_LAT,ID_PRN_STB,ID_PRN_MTB,ID_PRN_MTBN,
	              ID_PRN_MTA,ID_PRN_MTAN,ID_PRN_PAPER,ID_PRN_TEMP,
	 ID_LED_R    =COMBINE16(MODULE_LED,0),ID_LED_Y,ID_LED_B,ID_LED_G,
	 ID_SYS_TAMPER0=COMBINE16(MODULE_SYS,0),ID_SYS_TAMPER1,ID_SYS_TAMPER2, ID_SYS_EZP_CLK,ID_SYS_EZP_CS,
	  		        ID_SYS_EZP_DI,ID_SYS_EZP_DO, ID_SYS_RESET, ID_SYS_MAINVER,ID_SYS_SPEAKER_L,ID_SYS_SPEAKER_R,
	  		        ID_SYS_JTAG_TMS, ID_SYS_JTAG_TRS,
}FAC_PIN_ID_t;


typedef struct _STRUCT_PIN_LIST_{
    char *pinname;         //������ʾPIN������
    char *TestDot;         //���Ե�,�봲�ϵĲ��Ե���,����ʱ��ӡ���Ե㣬����ά��
    FAC_PIN_ID_t  pinid;   //�������豸֮���ͨѶ��PIN IDΨһ
    uchar pin;             //�������豸�ϵ�PIN����ID
    uchar mode;    //����ģʽ:0-��ƽ��ͬ 1-��ƽ�෴ 2-����Ԥ��
    uchar rfu0;
    uchar rfu1;
}STRUCT_PIN_LIST;

typedef struct
{
    char *pModuleName;
    int Moduleid;
    int listnum;
    STRUCT_PIN_LIST *pinList;
    int (*pin_test_func)(void *module,void *param);  //�Զ�����Է���,���Ϊ��,�����������ʽ
    int (*function_test_func)(void *module);
}STRUCT_FAC_MODULE;


// ��Ƭ����
typedef struct
{
    int id;        //��Ƭ����ģʽ
    int mode;        //��Ƭ����ģʽ
    char *name;  //��Ƭ�ļ���
    // ����ΪNULLʱ����ʾ����Ҫ����
    int (*init_func)(int mode);    //��ʼ������
    int (*finish_func)(int mode);    //��������
    int (*shake_func)(int mode);   //������������ַ���
    int (*shakeboot_func)(int mode);  //����boot���ַ���
    int (*dlboot_func)(void *param);  //����boot����
    int (*shakectrl_func)(int mode);  //����ctrl���ַ���
    int (*dlctrl_func)(void *param);  //����ctrl����
    int (*shakefontlib_func)(int mode);//�����ֿ����ַ���
	int (*dlfontlib_func)(void *param); //�����ֿⷽ��
    int (*shaketest_func)(int mode); //����ǰ�����ַ���
    int (*maintest_func)(void *param); //���Է���
}STRUCT_FAC_OPT;

enum {
    FAC_STAT_INIT = 0,
    FAC_STAT_SHAKE_HOST, //�������������ֳɹ�
    FAC_STAT_SHAKE_BOOT, //����ISP���ֳɹ�,׼������BOOT
    FAC_STAT_DL_BOOT,    //��������BOOT������
    FAC_STAT_SHAKE_CTRL, //����BOOT���ֳɹ�,׼������CTRL
    FAC_STAT_DL_CTRL,    //��������CTRL������
    FAC_STAT_SHAKE_FONTLIB, //����ISP���ֳɹ�,׼�������ֿ�
    FAC_STAT_DL_FONTLIB,    //���������ֿ������
    FAC_STAT_SHAKE_INFO,//��ȡ������Ϣ����
    FAC_STAT_SHAKE_TEST,//�������������������
    FAC_STAT_TESTING,//��������Թ���
    FAC_STAT_OK,    //���Գɹ�
    FAC_STAT_ERR = 0xFF,    //����ʧ��
};

// ����״̬
typedef struct _STRUCT_WORKINFO_{
    uint mode;  //0-ȫ����������� 1-һ�����ֹ��Ͻ���
    uint stat;  //0-��ʼ״̬
    uint workcom; //�����˿�
    char* workbps; //����������
    uint dbgcom;  //���Զ˿�
    uchar* dbgbps;  //���Զ˿ڲ�����
    STRUCT_FAC_OPT *pOpt; //��Ƭ����
    uint moduleNum;
    STRUCT_FAC_MODULE *pModulelist;
}STRUCT_WORKINFO;

// ������Э��
#define  FAC_STX  0x02
#define  FAC_ETX  0x03
#define  FAC_CIDH 0xDF   //���������λ

#define  FAC_CIDL_SHAKE         0x00
#define  FAC_CIDL_TESTMODULE	0x01
#define  FAC_CIDL_TESTPIN		0x02
#define  FAC_CIDL_TESTFUNCTION	0x03
#define  FAC_CIDL_ENDTEST       0x04
#define  FAC_CIDL_ENDMODULETEST 0x05
#define  FAC_CIDL_WRITE_ID      0x06  //д����ID
#define  FAC_CIDL_HANDLE_PIN    0x07  //���������Ŀ���
#define  FAC_CIDL_UNMODLEPININIT    0x08  //�ǹ��ܲ��Ե�ģ�飬���ǵĿ��߳�ʼ������

//����ģʽ��
#define FAC_SHAKE_POWER_ON 0x00
#define FAC_SHAKE_TEST 0x01
#define FAC_SHAKE_BOOT 0x02
#define FAC_SHAKE_CTRL 0x03
#define FAC_SHAKE_FONT 0x04
#define FAC_SHAKE_INFO  0x05//��ȡ������Ϣ��������

int testcom_printf(int uart_id, const char *format, ...);
int testcom_clear(int uart_id);
int testcom_close(int uart_id);
int testcom_open(int uart_id, const char *uart_para);
int testcom_ready(int uart_id);
int testcom_write(int uart_id, const void *inbuf, int bytelen);
int testcom_read(int uart_id, void *outbuf, int bytelen, int timeout_ms);
int testcom_check_readbuf(int uart_id);

int fac_function_test_demo(void *module);

int fac_receive (uint *outlen, uchar *outbuf, uint timeout);
int fac_send(uchar *data, uint len);

void boardtest_set_pio_output(int pin, uchar val);
void boardtest_set_pio_input(int pin);

void tester_main(void);

#endif

