/***************** (C) COPYRIGHT 2014 START Computer equipment *****************
 * File Name          : sys_inf_zone.h
 * bfief              : 
 * Author             : luocs() 
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 4/3/2014 7:01:13 PM
 * Description        : 
 *******************************************************************************/

#ifndef __SYS_INF_ZONE_H__
#define __SYS_INF_ZONE_H__

#include "map.h"

//��ȫ��֤
// FSK ID
enum {
    SAFE_CTRLFSK_ID  = 0,
    SAFE_MAPPFSK_ID,
    SAFE_RFU_ID
};
// AUTHKEY ID
enum {
    SAFE_FAC_AUTHKEY_ID  = 0,
    SAFE_RFU0_AUTHKEY_ID,
    SAFE_RFU1_AUTHKEY_ID
};

//sys_set_sysinfo sys_get_sysinfo sys_get_module_type ��ȡ�汾��Ϣ�������
#define     MODULE_PRINT_TYPE         0x01
#define     MODULE_LCD_TYPE           0x02
#define     MODULE_MAGNETIC_TYPE      0x03
#define     MODULE_MODEM_TYPE         0x04
#define     MODULE_GPRS_TYPE          0x05
#define     MODULE_CDMA_TYPE          0x06
#define     MODULE_BLUETOOTH_TYPE     0x07
#define     MODULE_WIFI_TYPE          0x08
#define     MODULE_LAN_TYPE           0x09
#define     MODULE_RF_TYPE            0x0A
#define     MODULE_COM1_TYPE          0x0B
#define     MODULE_COM2_TYPE          0x0C
#define     MODULE_INFR_TYPE          0x0D
#define     MODULE_ZIGBEE_TYPE        0x0E
#define     MODULE_PN512_TYPE         0x0F
#define     MODULE_BATTERY_TYPE       0x10
#define     MODULE_USBHOST_TYPE       0x11
#define     MODULE_NAND_TYPE          0x12
#define     MODULE_SCANNER_TYPE       0x13
#define     MODULE_S980_TYPE          0x14  //k_S980_type
#define     MODULE_CARDREADER_TYPE    0x15
#define     MODULE_COM3_TYPE          0x16
#define     MODULE_COM4_TYPE          0x17
#define     MODULE_IOM_TYPE           0x18
#define     MODULE_STANDBY_BAT_TYPE   0x19  //���õ�Դ


#define     MODULE_MODE_PRODUCT       0x1000  //��Ʒ��� 4�ֽ�:��Ʒ������+������+�汾+Ԥ��
#define     MODULE_MODE_TESTMODE      0x1001  //����ģʽ 0-����ģʽ 1-EMC���� 2-��֤����
#define     MODULE_MODE_LCD           0x1002  //Һ������
#define     MODULE_MODE_SDCARD        0x1003  // SD��
#define     MODULE_MODE_COMM          0x1004  //ͨ��
#define     MODULE_MODE_SCANNER       0x1005  //ɨ��ͷ
#define     MODULE_MODE_ICCARD        0x1006  //IC��
#define     MODULE_MODE_MAGCARD       0x1007  //�ſ�
#define     MODULE_MODE_RFID          0x1008  //��Ƶ
#define     MODULE_MODE_PRINTER       0x1009  //��ӡ
#define     MODULE_MODE_FONT          0x100A  //�ֿ�����
#define     MODULE_MODE_FACWORK       0x1010  //�������Թ�����Ϣ

// ����ֻ����ص���
#define     SU_MODULE_MODE_PINPAD        0x2000
#define     SU_MODULE_MODE_SAFT          0x2001  //����״̬ 4B: ��������(0-δ���� 1-����)+����״̬(0-δ���� 1-����) + 2B(Ԥ��)
#define     SU_MODULE_MODE_APPAUTH       0x2002  //Ӧ����֤ 4B: ״̬(0-δ���� !0-����) + 3B(Ԥ��)

// sys_get_module_type����ֵ

#define     MODULE_NOTEXIST             0x000
#define     MODULE_PRINT_FUJITSU        0x010
#define     MODULE_PRINT_SEIKO          0x011
#define     MODULE_PRINT_OTHER          0x01F
#define     MODULE_LCD_TRULYSEMI        0x020
#define     MODULE_LCD_GOWORLD          0x021
#define     MODULE_LCD_ICON             0x022
#define     MODULE_LCD_NOICON           0x023
#define     MODULE_LCD_LIERDA_TFT       0x024
#define     MODULE_LCD_TOUCH_TFT        0x025
#define     MODULE_LCD_OTHER            0x02F
#define     MODULE_MAGNETIC_MAGTEK      0x030
#define     MODULE_MAGNETIC_IDTECH      0x031
#define     MODULE_MAGNETIC_MT318       0x032
#define     MODULE_MAGNETIC_HCM4003     0x033
#define     MODULE_MAGNETIC_MTK211      0x034
#define     MODULE_MAGNETIC_OHTER       0x03F
#define     MODULE_MODEM_CX_MODEM       0x040
#define     MODULE_MODEM_SI_MODEM       0x041
#define     MODULE_MODEM_OTHER          0x04F
#define     MODULE_GPRS_EM310           0x050
#define     MODULE_GPRS_GC864           0x051
#define     MODULE_GPRS_GSM0306         0x052
#define     MODULE_GPRS_MG323           0x053
#define     MODULE_CDMA_EM200           0x060
#define     MODULE_CDMA_MC323           0x061
#define     MODULE_CDMA_CC864           0x062
#define     MODULE_BLUETOOTH_AUBTM      0x070
#define     MODULE_BLUETOOTH_BLK        0x071
#define     MODULE_BLUETOOTH_BM77SPP    0x072
#define     MODULE_WIFI_OTHER           0x08F
#define     MODULE_LAN_OTHER            0x09F
#define     MODULE_RF_RC531             0x0A0
#define     MODULE_RF_RC663             0x0A1
#define     MODULE_RF_PN512_BASE        0x0A2
#define     MODULE_RF_PN512_ENLARGE     0x0A3
#define     MODULE_RF_AS3911            0x0A4
#define     MODULE_RF_AS3910            0x0A5
#define     MODULE_RF_FM17550           0x0A6
#define     MODULE_COM1                 0x0B0
#define     MODULE_COM2                 0x0C0
#define     MODULE_INFR                 0x0D0
#define     MODULE_ZIGBEE               0x0E0
#define     MODULE_PN512                0x0F0
#define     MODULE_CPUCARD_IOREV        0x0F1 //io����
#define     MODULE_CPUCARD_IO           0x0F2 //ioֱ��
#define     MODULE_CPUCARD_8035         0x0F3
#define     MODULE_CPUCARD_8113         0x0F4
#define     MODULE_CPUCARD_6001         0x0F5
#define     MODULE_BATTERY_1LI          0x100
#define     MODULE_BATTERY_2LI          0x101
#define     MODULE_USBHOST              0x110
#define     MODULE_NAND                 0x120
#define     MODULE_SCANNER              0x130
#define     MODULE_SCANNER_EM1300       MODULE_SCANNER  //һά�� EM_1300
#define     MODULE_SCANNER_5X1080       0x131  //��ά�� 5X10-80
#define     MODULE_SCANNER_EM3000	    0x132  //��ά�� EM_3000
#define     MODULE_COM3                 0x140
#define     MODULE_COM4                 0x150
#define     MODULE_IOM                  0x160
#define     MODULE_STANDBY_BATTERY      0x170  //���õ�Դ


#define     MODULE_GPRS_MASK       (1 << 9)
#define     MODULE_CDMA_MASK       (1 << 8)
#define     MODULE_BLUETOOTH_AUBTM_MASK  (1 << 7)
#define     MODULE_WIFI_OTHER_MASK       (1 << 6)

// ��Ƶ���  4*5=20B
typedef struct __MODULE_RFID_INFO {
    uchar type;                  // FF-��֧�� 0- 1-3911 2-FM17550 3-PN512
//    uchar rfu[3];
    uchar gTypeBmodule;  //type b�������
    uchar gTypeArec;     //type a����������
    uchar gTypeBrec;     //type b����������
    uint rfid_powerfield_time;   // ��ǿ������ʱʱ�� (4B)
    uint field_strength;         // ��ǿ��С
    uint  irfu[2];
}MODULE_RFID_INFO;				/* ----------  end of struct MODULE_RFID_INFO  ---------- */

//�������
typedef struct __MODULE_BLUET_INFO {
    uchar bluetooth;             // FF-��֧�� 0-��֧�� 1-BM77
    uchar rfu[3];
}MODULE_BLUET_INFO;				/* ----------  end of struct MODULE_BLUET_INFO  ---------- */

// �ſ����
typedef struct __MODULE_MAGCARD_INFO {
    uchar type;             // ��ͷ����:FF-Ĭ��3 0-��֧�� 1-IDTECH 2-MTK211 3-MAGTEK 4-HCM4003
    uchar trkflag;          // ֧�ִŵ���  0-3���ŵ� 1-2��3�ŵ�
    uchar rfu[2];
}MODULE_MAGCARD_INFO;				/* ----------  end of struct MODULE_MAGCARD_INFO  ---------- */

//cpu�����
typedef struct __MODULE_ICCARD_INFO {
    uchar type;            // IC������оƬ���� 0-��֧�� 1-���� 2-cpu���� 3-8035 4-8113 5-6001
    uchar excardreader;    // �Ƿ�֧�����ö�����
    uchar slots;            // �ܿ�����
    uchar insamslots;       // ����SAM����
    uchar exmaxslots;       // ���ô�����
    uchar exsamslots;       // ����SAM������
    uchar rfu[2];
}MODULE_ICCARD_INFO;				/* ----------  end of struct MODULE_ICCARD_INFO  ---------- */

// ϵͳ�������  32B
typedef struct __MODULE_SYS_INFO {
    uchar menupassword[16];  // �˵�����
    uchar lcdcontrast;       //Һ���Աȶ�
    uchar lcdlighttime;      //����ʱ��
    uchar keytone;           //��������
    uchar language;          //ϵͳ����  0-Ӣ�� 1-����
    uint  irfu[3];
}MODULE_SYS_INFO;				/* ----------  end of struct MODULE_SYS_INFO  ---------- */

// ���������Ϣ
// 20+12+8+4+16=60B
typedef struct _FAC_TESTINFO_{
    uchar mbno[20];   //����ID 
    uchar voucherno[12];// ��Ʊ��
    uchar phase;      //��ǰ���Խ׶�
    uchar fac_ctrl_flag; //�ܿ��Ƿ�ɹ���ʶ
    uchar rfu[6];
    // D0~D7  D0:modem D1:WIRELESS D2:BT D3:ETH D4:SCANNER D5:INFR
    uchar module_res[4]; //ģ����Խ�� 1-�ɹ� 0-ʧ��
    // D0~D29:���������������  1-�ɹ� 0-ʧ��
    // D30~D31:03-ǿ�Ƴɹ�  02-ʧ�� 01-�����ɹ� 00-δ����
    uint auto_res[4];    //�Զ����Խ��
}FAC_TESTINFO;

// 6+11*2=
typedef struct
{
	uchar  aging_allok;	//�ϻ����Խ���,(�����WP50��S980�������ĳ������δ����)
	uchar  fail_step;//����ʧ�ܵĻ���
	uchar  rfu[4];
	//battery_error_info����ϻ�������Ϣ:��bitΪ0ʱ��ʾ����
	//D0 - 1:��������ʱ��ѹδ�ﵽ�涨ֵ     charge_error  
	//D1 - 1:����ϻ�����ʱ��ѹ��Ԥ��ֵ��   discharge_error
	//D2 - 1:���ʱ������,������ȴ���γ�,��¼���������� need_charge
	//D3 - 1:�����׶�����������������δ�Σ����δ�γ�����10���������Զ�������磬�е�ز��� recharge_error
	//D4 - 1:�����δ���(���ʱ�䲻������û����) recharge_incomplete
	//D5 ~ D15:Ԥ��
	ushort battery_error_info;	
	ushort totaltimes;//һ��������x��
	ushort error_timer;//�ڵ�x�ֲ���ʧ��
	ushort prn_count;//��ӡ���Դ���
	ushort baterry_mv1;//��������ʱ��ص�ѹ(mv)
	ushort baterry_mv2;//�ŵ����ʱ��ص�ѹ(mv)	
	ushort total_time;//�ϻ�������ʱ��(����)
	ushort charge_time;//�ܳ��ʱ��
	ushort discharge_time;//�ŵ�ʱ��
	ushort baterry_mv_actual;//ʵʱ����ѹ
	ushort recharge_time;//�ڶ��γ��ʱ��
}s_aging_log;

// Ӳ���汾��Ϣ
typedef struct
{
    unsigned char flag; // FF-��Ч  00-AD����ʵ��ֵ 0x01���ֶ�����ֵ
    unsigned char main;
    unsigned char secondary;
    unsigned char revise;
} hardware_verion_t;

// ����汾��Ϣ
typedef struct
{
    unsigned char main;
    unsigned char secondary;
    unsigned char revise;
    unsigned char reserve;
} version_t;

typedef struct
{
    uchar lcd;
    uchar rfid;
    uchar printer;
    uchar iccard;
    uchar gprs;
    uchar cdma;
    uchar bluetooth;
    uchar modem;
    uchar wifi;
    uchar ethernet;
    uchar scanner;
    uchar magcard;
    uchar sd;
    uchar pinpad;
    uchar battery;
    uchar com1;
    uchar com2;
    uchar com3;
    uchar com4;
    uchar infr;
    uchar usbhost;
    uchar nandflash;
    uchar zigbee;
    uchar standbybat;//���õ��
    uchar wlm_flag;//���β���ֻ�������������жϣ������о���ģ��ʶ�� 1-�� 0xFF-��
    uchar reserve[7];
} module_type_t;

typedef struct _SYSZONE_DEF
{
    uint8_t appcheck[8];    //0  "STAR"+4 app_length(4B)
    uint8_t boot_info[16];  //8  "WP-30BOOT"
    uint8_t boot_sha2[32];  //24
    uint8_t boot_mac[4];    //56
    uint32_t first_run;     //60 /*CTRL��һ�����У����ڳ�ʼ��ϵͳ��Ϣ��*/
    uint32_t app_enable;    //64 /*CTRLʹ�ܱ�־����������������ɺ�*/
    uint32_t app_update;    //68 /*Ӧ��ÿ�θ��±�־:����Ӧ�õ�һ������*/
    uint32_t bps;           //72
    uint32_t beep;          //76
    uint32_t lcd_contract;  //80
    uint32_t attack_flag;   //84
    uint8_t superpsw[8];    //88
    uint8_t adminApsw[8];   //96
    uint8_t adminBpsw[8];   //104 
    uint8_t sn_dev[32];     //112 
    uint8_t fsk_dev[24];    //144 
    uint8_t fsk_host[24];   //168
    MODULE_RFID_INFO trfidinfo; //192  sizeof(MODULE_RFID_INFO)= 20 
	MODULE_SYS_INFO tsysinfo;   //212  sizeof(MODULE_SYS_INFO) = 32
    FAC_TESTINFO tTestInfo;     //244  sizeof(FAC_TESTINFO)    = 60
    hardware_verion_t main_version; //304  sizeof(hardware_verion_t) = 4
    MODULE_BLUET_INFO bluetoothinfo;//308 sizeof(MODULE_BLUET_INFO) = 4
    MODULE_MAGCARD_INFO tmagcardinfo;//312 sizeof(MODULE_MAGCARD_INFO) = 4
    MODULE_ICCARD_INFO ticcardinfo; //316 sizeof(MODULE_ICCARD_INFO) = 8
    uint32_t secu_leve;         //324 
    uint32_t tamper[4];         //328
    uint8_t exflash_key[8];     //344 
    uint8_t project_id[32];     //352
    uint32_t gapksha_num;       //384 ����������flash��gapk sha������ 
    uint8_t secure_random[24];  //388  ����3��8�ֽ�������� 
    uint8_t rfu1[2048+1632];    //412  | 2044-412 = 1632
    uint32_t syszone_check;     //2048+2044
}SYSZONE_DEF; /*�޸�SYSZONE_DEF�ṹ���ȷ�������ṹ��СΪ2048�ֽ�,ע���ֽڶ���*/

//*******************************************************
//  syszone ����  1024Byte
//*******************************************************
#define OFFSET_APP_CHECK         (FPOS(SYSZONE_DEF,appcheck))   //Ӧ�ó����־  8B
	#define LEN_APP_CHECK           (FSIZE(SYSZONE_DEF,appcheck))  //��STAR��+ 4�ֽ�Ӧ�ó�����볤��
#define OFFSET_BSV              (OFFSET_APP_CHECK+LEN_APP_CHECK)  //52B
	#define OFFSET_BSV_BOOTINFO  0   //boot�汾��Ϣ"WP-30BOOT"
	#define LEN_BSV_BOOTINFO        (FSIZE(SYSZONE_DEF,boot_info))
	#define OFFSET_BSV_SHA      (OFFSET_BSV_BOOTINFO+LEN_BSV_BOOTINFO) //SHA2У����
	#define LEN_BSV_SHA             (FSIZE(SYSZONE_DEF,boot_sha2))
	#define OFFSET_BSV_MAC      (OFFSET_BSV_SHA+LEN_BSV_SHA)
	#define LEN_BSV_MAC             (FSIZE(SYSZONE_DEF,boot_mac))
	#define LEN_BSV                 (LEN_BSV_BOOTINFO+LEN_BSV_SHA+LEN_BSV_MAC) //
	
#define OFFSET_FIRST_FLAG       (FPOS(SYSZONE_DEF,first_run)) //��һ�����б�־ 
	#define LEN_FIRST_FLAG           (FSIZE(SYSZONE_DEF,first_run))  //Ϊ0xFFΪ��һ�����У�����WP-30����Ϊ�ǵ�һ������
#define OFFSET_APPENABLE_FLAG   (FPOS(SYSZONE_DEF,app_enable))  //����Ӧ�ñ�־  0xFFFFFFFF  0x55AA-д������־  0-����
	#define LEN_APPENABLE_FLAG       (FSIZE(SYSZONE_DEF,app_enable))
#define OFFSET_APPUPDATE_FLAG   (FPOS(SYSZONE_DEF,app_update))  //Ӧ��ÿ�θ��±�־:����Ӧ�õ�һ������
	#define LEN_APPUPDATE_FLAG       (FSIZE(SYSZONE_DEF,app_update))
#define OFFSET_BPS              (FPOS(SYSZONE_DEF,bps))           //������ Ԥ����115200��
	#define LEN_BPS                  (FSIZE(SYSZONE_DEF,bps))
#define OFFSET_BEEP             (FPOS(SYSZONE_DEF,beep)) //���������أ�Ԥ����
	#define LEN_BEEP                 (FSIZE(SYSZONE_DEF,beep))
#define OFFSET_LCD              (FPOS(SYSZONE_DEF,lcd_contract))//�Աȶȣ�Ԥ����
	#define LEN_LCD                  (FSIZE(SYSZONE_DEF,lcd_contract))
#define OFFSET_ATTACKFLG        (FPOS(SYSZONE_DEF,attack_flag))  //
	#define LEN_ATTACKFLG            (FSIZE(SYSZONE_DEF,attack_flag))
#define OFFSET_SECURITY_LEVELS  (FPOS(SYSZONE_DEF,secu_leve))  //��ȫ�ȼ�
	#define LEN_SECURITY_LEVELS       (FSIZE(SYSZONE_DEF,secu_leve))
#define SYS_TAMPER_EVENT_ADDR    (FPOS(SYSZONE_DEF,tamper))     // tamper reg
    #define SYS_TAMPER_EVENT_SIZE    (FSIZE(SYSZONE_DEF,tamper))

#define SYS_SUPER_PSW_ADDR        (FPOS(SYSZONE_DEF,superpsw))
#define SYS_SUPER_PSW_SIZE        (FSIZE(SYSZONE_DEF,superpsw))
#define OFFSET_PSW       SYS_SUPER_PSW_ADDR
#define LEN_PSW          SYS_SUPER_PSW_SIZE

#define SYS_ADMINA_PSW_ADDR       (FPOS(SYSZONE_DEF,adminApsw))
#define SYS_ADMINA_PSW_SIZE       (FSIZE(SYSZONE_DEF,adminApsw))

#define SYS_ADMINB_PSW_ADDR       (FPOS(SYSZONE_DEF,adminBpsw))
#define SYS_ADMINB_PSW_SIZE       (FSIZE(SYSZONE_DEF,adminBpsw))

#define SYS_SN_DEV_ADDR         (FPOS(SYSZONE_DEF,sn_dev))
#define SYS_SN_DEV_SIZE         (FSIZE(SYSZONE_DEF,sn_dev))
#define OFFSET_SN               SYS_SN_DEV_ADDR 
	#define LEN_SN                   SYS_SN_DEV_SIZE

#define OFFSET_FSK_SM    SYS_SN_DEV_ADDR  

#define SYS_FSK_host_ADDR         (FPOS(SYSZONE_DEF,fsk_host))
#define SYS_FSK_host_SIZE         (FSIZE(SYSZONE_DEF,fsk_host))
#define OFFSET_FSK_CTRL  SYS_FSK_host_ADDR  
#if 0
#define OFFSET_FSK_SM           (OFFSET_PSW+SYSPSWINFO_LEN*3)   //20B      //fixpsw encrypt 
#define OFFSET_FSK_CTRL         (OFFSET_FSK_SM+FSKINFO_LEN)     //20B
#define OFFSET_APP_ZONE         (OFFSET_FSK_CTRL+FSKINFO_LEN)   //
	#define LEN_APP_ZONE            (APPINFO_LEN*(NUM_APP+1))   //48*5=240
#define OFFSET_FSK_APP_ZONE     (OFFSET_APP_ZONE+LEN_APP_ZONE)
    #define LEN_FSK_APP_ZONE        (FSKINFO_LEN*(NUM_APP+1))   //20*5=100
#define OFFSET_TRANSFERKEY_ZONE (OFFSET_FSK_APP_ZONE+LEN_FSK_APP_ZONE)	
    #define LEN_TRANSFERKEY_UNIT    (24)
	#define LEN_TRANSFERKEY         (LEN_TRANSFERKEY_UNIT*(NUM_APP+1)) //24*5=120
#endif
#define SYS_RFIDINFO_ADDR         (FPOS(SYSZONE_DEF,trfidinfo))
#define SYS_RFIDINFO_SIZE         (FSIZE(SYSZONE_DEF,trfidinfo))

#define SYS_MAINVERSION_ADDR         (FPOS(SYSZONE_DEF,main_version))
#define SYS_MAINVERSION_SIZE         (FSIZE(SYSZONE_DEF,main_version))

#define OFFSET_Reserved1          (FPOS(SYSZONE_DEF,rfu1))                              //ʣ�� 228B

#define SYS_EXFLASH_KEY_ADDR      (FPOS(SYSZONE_DEF,exflash_key))
#define SYS_EXFLASH_KEY_SIZE        (FSIZE(SYSZONE_DEF,exflash_key))

#define SYS_PROJIECT_ID_ADDR      (FPOS(SYSZONE_DEF,project_id))
#define SYS_PROJIECT_ID_SIZE        (FSIZE(SYSZONE_DEF,project_id))

#define SYS_GAPKSHA_NUM_ADDR      (FPOS(SYSZONE_DEF,gapksha_num))
#define SYS_GPAKSHA_NUM_SIZE        (FSIZE(SYSZONE_DEF,gapksha_num))

#define SYS_SECURITY_RANDOM_ADDR  (FPOS(SYSZONE_DEF,secure_random))
#define SYS_SECURITY_RANDOM_SIZE    (FSIZE(SYSZONE_DEF,secure_random))

#define OFFSET_SYSZONE_CHECK      (FPOS(SYSZONE_DEF,syszone_check))
	#define LEN_SYSZONE_CHECK        (FSIZE(SYSZONE_DEF,syszone_check))
#define OFFSET_SYSZONE_END        (OFFSET_SYSZONE_CHECK + LEN_SYSZONE_CHECK) 	//1024B

//*******************************************************
// 
//  Appsyszone ������App�������288B SHAУ�鷶Χ:Դ����+�汾��Ϣ(16B) ƫ��Ϊ��
// APP_INFO(16B) + APP_CHECK(256) + CHECK_FLAG(16B)
// APP_CHECK:APP_SHA(32B) + APP_MAC(4B) + Ԥ��(220)
// 
//*******************************************************
#if 0
#define OFFSET_APPSYS_APPINFO    288
	#define LEN_APPSYS_APPINFO     16
#define OFFSET_APPSYS_SHA        (OFFSET_APPSYS_APPINFO-LEN_APPSYS_APPINFO)
	#define LEN_APPSYS_SHA         32
#define OFFSET_APPSYS_MAC        (OFFSET_APPSYS_SHA-LEN_APPSYS_SHA)
	#define LEN_APPSYS_MAC         4
#define OFFSET_APPSYS_RESERVED   (OFFSET_APPSYS_MAC-LEN_APPSYS_MAC)
	#define LEN_APPSYS_RESERVED    220
#define OFFSET_APPSYS_SHAFLG     (OFFSET_APPSYS_SHA-LEN_APPSYS_SHA)
	#define LEN_APPSYS_SHAFLG      16
#endif

//*******************************************************
// 
//   APPINFO offset
// 
//*******************************************************
//#define SA_APPINFO(appno)      (OFFSET_APP_ZONE+(appno-1)*APPINFO_LEN)

/*-----------------------------------------------------------------------------}
 * ϵͳ��Ϣ�����ñ�־��
 *-----------------------------------------------------------------------------{*/
#define SYSZONE_APPUPDATE_FLAG     (uint32_t)0xAA55A5A5


/*-----------------------------------------------------------------------------}
 *  Ĭ������
 *-----------------------------------------------------------------------------{*/
#define DEFAULT_MENU_PASSWORD  "000000" //�˵����� 
//Ĭ��ϵͳ����
#define DEFAULT_SUPERPSW_SYS    "20112010"  
#define DEFAULT_ADMINAPSW_SYS   "00000000"  
#define DEFAULT_ADMINBPSW_SYS   "00000000"  
//Ĭ�ϰ�ȫģ��FSK
#define DEFAULT_FSK_SM     "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff"


/*-----------------------------------------------------------------------------}
 * ��Ʒ�汾 
 *-----------------------------------------------------------------------------{*/
#define     READ_PRODUCT_VER  0x00   //��Ʒ
#define     READ_MAINB_VER    0x01
#define     READ_MAINBID_VER  0x02   //����ID
#define     READ_BOOT_VER     0x03   //BOOT
#define     READ_CTRL_VER     0x04   //CTRL
#define     READ_CTRLTIME_VER 0x05   //�ֿ�
#define     READ_FONT_VER     0x06   //�ֿ�
#define     READ_SN_VER       0x07
/*-----------------------------------------------------------------------------}
 * ȫ�ֱ��� 
 *-----------------------------------------------------------------------------{*/
extern const SYSZONE_DEF * const gpSys; 
extern version_t k_MainVer;

//int sys_zone_read(uint32_t addr,uint32_t length, uint8_t *output);
//int sys_zone_write(uint32_t addr,uint32_t length,const uint8_t *input);
//int sys_zone_recover(void);
//int sys_zoneback_recover(void);
//int sys_first_run(void);

inline void readFlashPage(uint32_t pageAddr, uint8_t *data, uint16_t len)
{
    memcpy(data,(char *)(pageAddr*2),len);
}
inline void readFlash(uint32_t pageAddr, uint32_t offset, uint8_t * data, uint16_t len)
{
    memcpy(data,(uint8_t *)(pageAddr+offset),len);
}
inline int eraseFlashPage(int pageAddress)
{
    return (int)drv_flash_SectorErase(pageAddress*2);
}
inline int writeFlashPage(uint32_t pageAddr, uint8_t *data, uint16_t len)
{
    return (int)drv_flash_write(pageAddr*2,(uint32_t *)data,(uint)len);
}

extern const char * const k_TermName;

uint s_read_syszone(uint offset, uint len, uchar *ucBuf);
uint s_write_syszone(uchar *backbuf, uint uiOffset, uint uiLen, uchar *ucBuf);
uint save_syszone_back(uchar *buf);
uint sys_start(void);
uint read_syspsw(uint mode, uint *len, uchar *psw);
uint write_syspsw(uint mode, uint len, uchar *psw);
uint PCI_GetHSK(uint mode, uchar *hsk);
int s_sysinfo_rfidinfo (uint mode, void *p);
int s_sysinfo_bluetoothinfo (uint mode, void *p);
int s_sysinfo_magcardinfo (uint mode, void *p);
int s_sysinfo_iccardinfo(uint mode, void *p);
int s_sysinfo_sysinfo (uint mode, void *p);
int s_sysinfo_mbVersioninfo(uint mode, void *p);
int s_sysinfo_gpakshainfo(uint mode, uint32_t *p);
int sys_get_module_type(int module);
int s_sysinfo_security_random(uint mode, uint index, char *p);
int32_t  s_sysconfig_read(int offset,int readlen,void *vbuf);
int32_t  s_sysconfig_write(int offset,int writelen,void *vbuf);
uint check_syszone(uint type, uchar *buf);
uint resume_syszone(uchar *buf);
int sys_read_ver(int module, char *ver);
int sys_get_ctrllen (int mode);


#endif

