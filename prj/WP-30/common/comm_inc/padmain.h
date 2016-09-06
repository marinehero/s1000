#ifndef __PADMAIN_H__
#define __PADMAIN_H__



//**************************
//    
// Ĭ�϶���
//
//**************************
#define DEFAULT_V1_APP     "SAPP_V1"
#if PRODUCT_TYPE == PRODUCT_TYPE_MK210
#define DEFAULT_SERIAL_BAUD      9600      //Ĭ�ϲ�����
#else
#define DEFAULT_SERIAL_BAUD      115200      //Ĭ�ϲ�����
#endif

/*-----------------------------------------------------------------------------}
 *  ��Կ����
 *-----------------------------------------------------------------------------{*/
#define MK_UNIT_LEN   (uchar)16  //MMK APPEK MagKey 
#define KEY_UNIT_LEN  (uchar)24  //authkey work mk
#define FSK_LEN       (uchar)16
#define ELRCK_LEN     (uchar)8   //elrck
#define CHECK_LEN     (uchar)4   //
#define FUN_LEN       (uchar)4   //
#define PSW_LEN       (uchar)LEN_PSW
//*******************************************************
//
//    key type
//
//*******************************************************
//#define KEY_TYPE_AUTHMK		    (uchar)0x01
//#define KEY_TYPE_AUTHPINK		(uchar)0x02
//#define KEY_TYPE_AUTHMACK		(uchar)0x03
//#define KEY_TYPE_AUTHDESK		(uchar)0x04
//#define KEY_TYPE_MK				(uchar)0x05
//#define KEY_TYPE_PINK			(uchar)0x06
//#define KEY_TYPE_MACK			(uchar)0x07
//#define KEY_TYPE_DESK			(uchar)0x08
//#define KEY_TYPE_APPAK          (uchar)0x09
//#define KEY_TYPE_APPEK          (uchar)0x0A
//#define KEY_TYPE_TRSK           (uchar)0x0B

#define KEY_TYPE_MAGK           (uchar)0x10
#define KEY_TYPE_MCK            (uchar)0x11

#define KEY_TYPE_APPFLG         (uchar)0x80
#define KEY_TYPE_APPNAME        (uchar)0x81
#define KEY_TYPE_APPPSWA        (uchar)0x82
#define KEY_TYPE_APPPSWB        (uchar)0x83
#define KEY_TYPE_APPSN          (uchar)0x84
#define KEY_TYPE_FSKDEV         (uchar)0x85   // ��ȫģ��FSK
#define KEY_TYPE_FSKHST_BOOT    (uchar)0x86   // ����BOOT FSK
#define KEY_TYPE_FSKHST_CTRL    (uchar)0x87   // ����CTRL FSK
#define KEY_TYPE_FSKHST_APP     (uchar)0x88   // ����Ӧ��FSK
#define KEY_TYPE_FSKAPPAK       (uchar)0x89   // û��
//*******************************************************
//    FSK type 
//*******************************************************
#define FSK_SMAPP   (uchar)KEY_TYPE_FSKDEV
#define FSK_CMCTRL  (uchar)KEY_TYPE_FSKHST_CTRL
#define FSK_CMAPP   (uchar)KEY_TYPE_FSKHST_APP
#define FSK_APPAK   (uchar)KEY_TYPE_APPAK 
//**************************
//    
// �ַ��������� 
//
//**************************
#define STR_ERR    "ERR"
#define STR_OK     "OK"
#define STR_Change "CHANGE"
#define STR_Input  "INPUT"
#define STR_PSW    "PSW"
#define STR_SYS    "SYS"
#define STR_USER   "USER"
#define STR_ADMINA  "ADMINA"
#define STR_ADMINB  "ADMINB"
#define STR_USERA  "USERA"
#define STR_USERB  "USERB"
#define STR_PTK    "PTK"
#define STR_APPK   "APPK"
#define STR_MMK    "MMK"

#define STR_TEST   "TEST"
#define STR_DEBUG  "DEBUG"
#define STR_ASSEMBLE "ASSEMBLE"
#define STR_PRODUCT "PRODUCT"
#define STR_AGE    "AGE"
#define STR_SINGLE "SINGLE"
#define STR_FAC    "FAC"
#define STR_VER    "VER"
#define STR_ASV    "ASV"
#define STR_BSV    "BSV"
#define STR_MAC    "MAC"
#define STR_SHA    "SHA"
#define STR_Enable "Enable"
#define STR_MENU   "MENU"
#define STR_SET    "SET"
#define STR_IS     "IS"
#define STR_BEEP   "BEEP"
#define STR_UART   "UART"
#define STR_SRAM   "SRAM"
#define STR_LCD    "LCD"
#define STR_BPS    "BPS"
#define STR_KEY    "KEY"
#define STR_DL     "DL"
#define STR_FSK    "FSK"
#define STR_DEL    "DEL"
#define STR_ENTER  "ENTER"
#define STR_CANCEL "CANCEL"
#define STR_EXIT   "EXIT"
#define STR_CREATE "CREATE"
#define STR_APP    "APP"
#define STR_DOT    "..."
#define STR_AUTHERR "AUTH ERR EXCEED 5"
#define STR_PIN_PROMPT  "����������:"
#define STR_INIT    "INIT"
#define STR_SYSRECOVER    "SYS RECOVER"



//******************************************
//   �궨��
//******************************************
#define PSW_SYS     (uchar)0
#define PSW_USER_A  (uchar)1
#define PSW_USER_B  (uchar)2

#define MIN_PIN_LEN     0      //V1 ��С���볤��
#define MIN_PCIPIN_LEN  4      //PCI ��С���볤��
#define MAX_PIN_LEN     14 

#define MAXNUM_PSW    5     // PSW Max Err Time
#define MAXNUM_AUTH   5     // Auth Max Err Time



// Э���ʽ
// STX+len(1B)+CMD(����)+SEQ(���)+Data+Stat(������״̬03/17)+CRC16(2B,LSB+MSB)+ETX
// Len=CMD+SEQ+Data+Stat+CRC16
#define ACK_OK				0x06
#define ACK_ERR				0x15

#define STX					0x02
#define ETX					0x03
#define FRAME_STAT_END		0x03
#define FRAME_STAT_NEXT	    0x17
#define FRAME_LEN_OFF		0x01
#define FRAME_CMD_OFF		0x02
#define FRAME_CMD_SEQ		0x03
#define FRAME_DATA_OFF      0x04

#define FRAME_LRC_LEN		0x01
#define FRAME_CRC_LEN		0x02

#define FRAME_FLAG_LEN		5
#define SEND_PACK_LEN	    250   //Data��󳤶�

#define NAK    0x55         //������Ӧ

// ���� cid+packno+data+03/17
//CID enum
enum PPKB_CMD
{
	CMD_KEY = 0,       //00
	CMD_REQUEST,       //01
	CMD_LCD_DISP,      //02 line+col+str
	CMD_LCD_CLS,       //03 cls
	CMD_LCD_SETINVERSE,//04 0- 1-
	CMD_BEEP,          //05
	CMD_DELAY,         //06 1ms
	CMD_FACTEST,       //07 
	CMD_SYSCOVERY,     //08 ϵͳ�ָ�
    CMD_RESET,         //09 none 
	CMD_SETTIME,       //0A
	CMD_GETTIME,       //0B
	CMD_CHECK_SYSPSW,  //0C 
	CMD_OPEN_MAGCARD,  //0D
	CMD_READ_MAGCARD,  //0E
	CMD_SYSMENU,       //0F  0-dl fsk_ctrl 1-modify syspsw 2-modify userpsw 3-dl key
	CMD_CHECK_SHA,     //10 
	CMD_DESDATA,       //11 ��ʽ type+mode+2B 00+data(8~1024)
	CMD_EXIT,          //12  
	CMD_GetSecuLevel,  //13
	CMD_SetSecuLevel,  //14
	CMD_SET_CONFIG,    //15  0-mag head type 1-
	CMD_CHECKAUTH,     //16 У����֤��Կ�Ƿ���֤�ɹ� type+index         
    CMD_TAMPER,        //17 ��ȡtamper���ŵ�ƽ
    CMD_LCD_PLUS,      //18 ��չ��ʾָ�� line(4B)+col(4B)+dispmode(4B)+str
};

/*-----------------------------------------------------------------------------}
 *  V1��չЭ������IO����:display,cls,����,����,����
 *-----------------------------------------------------------------------------{*/
#define V1_CMD_IO      0xE0

/*-----------------------------------------------------------------------------}
 *  PCIЭ�� 
 * -----------------------------------------------------------------------------{*/
#define PCI_CMD_HS 			0xB1
#define PCI_CMD_SN			0xB2
#define PCI_CMD_MK			0xB3
#define PCI_CMD_RND			0xB4
#define PCI_CMD_CK			0xB5
#define PCI_CMD_WK			0xB6
#define PCI_CMD_GET_PIN		0xB7
#define PCI_CMD_GET_MAC		0xB8
#define PCI_CMD_GET_DES		0xB9
#define PCI_CMD_GET_MAG		0xBA
#define PCI_CMD_VER			0xBB
#define PCI_CMD_LCD			0xBC
#define PCI_CMD_PIN      	0xBD   //����PIN
#define PCI_CMD_SETSN       0xBE        
#define PCI_CMD_APPMANAGE   0xBF   //��Ӧ�ù���
	#define REQ_APP_OPEN   0x01  // open app
	#define ACK_APP_OPEN   0x02
	#define REQ_APP_AUTH   0x03  // auth app
	#define ACK_APP_AUTH   0x04
	#define REQ_APP_CREAT  0x05  //
	#define ACK_APP_CREAT  0x06  //
	#define REQ_APP_DEL    0x07  //
	#define ACK_APP_DEL    0x08  //
	
#define PCI_CMD_DEBUG0      0xC0
#define PCI_CMD_EXTEND      0xC1 //��չָ�� 0-���ò�����
    #define PCI_CMD_EXTEND_SETBPS  0   //���ò�����  ����������̲�֧��
    #define PCI_CMD_EXTEND_IC      1   //IC������ 
    enum PCI_CMD_EXTEND_IC_OPT{
        PCI_CMD_EXTEND_IC_OPEN = 0,    // ��ģ��
        PCI_CMD_EXTEND_IC_CLOSE,       // �ر�ģ��
        PCI_CMD_EXTEND_IC_INIT,        // ��ģ��
        PCI_CMD_EXTEND_IC_CHECK,       // ��⿨��״̬
        PCI_CMD_EXTEND_IC_POWERUP,     // �����ϵ�
        PCI_CMD_EXTEND_IC_EXCHANGE,    // ��Ϣ����
        PCI_CMD_EXTEND_IC_POWERDOWN    // �����µ�
    };
    #define PCI_CMD_EXTEND_FUNC     2   //����ָ�� 
    enum PCI_EXTEND_FUNC_OPT {
        PCI_EXTCMD_FUNC_LED = 0
    };


// boot������
enum PPKB_REQUEST
{
	PPKB_REQUEST_ACK = 0,
	PPKB_REQUEST_DL_APP,    //Ӧ�ò㿪ʼ����
	PPKB_REQUEST_DL_APP_Start,  //����������ɿ�ʼ���أ�ʹ��͸��
	PPKB_REQUEST_DL_APP_Over,   //�������
	PPKB_REQUEST_DL_FSK,        //�޸�ϵͳ������û���������PTK��ɣ�ʹ��͸��
	PPKB_REQUEST_DL_FSK_Over,   // FSK�������
	PPKB_REQUEST_Init_Over,    
	PPKB_REQUEST_APP_RUN,       // Ӧ�ò㿪ʼ����
	PPKB_APP_RECOVERY_ERR = 0xFB, //�ָ�ʧ��
	PPKB_APP_RECOVERY_OK = 0xFC, //�ָ��ɹ�
	PPKB_REQUEST_RECOVERY_ERR = 0xFD, //�ָ�ʧ��
	PPKB_REQUEST_RECOVERY_OK = 0xFE, //�ָ��ɹ�
	PPKB_REQUEST_STOP = 0xFF   //
};



#define FLAG_HS					1
#define FLAG_AHTH				1<<1

//AppAuthFlag Ӧ����֤��־
#define STAT_APPAUTH_NOT       0
#define STAT_APPAUTH_ING       1   
#define STAT_APPAUTH_OK        2

//HsFlag
#define HS_STAT_NOT			0
#define HS_STAT_ING			1
#define HS_STAT_SUC			2

// AuthFlg
#define STAT_AUTH_NOT    0
#define STAT_AUTH_ING    1
#define STAT_AUTH_SUS    2


#define PSW_NUM_MAX			5

#if (defined EM_DEBUG) || (defined __DEBUG_AUTOINPUT__) 
	#define AUTH_DL_TIMEOUT			999999
	#define HS_DL_TIMEOUT			999999
	#define PIN_INPUT_LIMITED_TIME	1
	#define AUTH_WORK_TIMER		    999999
	#define PSW_INPUT_TIMER         999999
#else
	#define AUTH_DL_TIMEOUT			60000
	#define HS_DL_TIMEOUT			300000
	#define PIN_INPUT_LIMITED_TIME	1
	#define AUTH_WORK_TIMER		    100
	#define PSW_INPUT_TIMER         12000
#endif

//*******************************************************
//
//    gWorkMode
//  ˵��: MK-210 Fac Mode  MODE_IDLE MODE_APP
//               PCI Mode  MODE_IDLE MODE_CTRLMANAGE  MODE_APP  MODE_APPMANAGE
//        S-980  Fac Mode  all
//               PCI Mode  all
//*******************************************************
enum PINPAD_STATUS
{
	MODE_INIT = PPKB_REQUEST_APP_RUN, //ctrl not open 7
	MODE_CTRLMANAGE,                  //ctrl open     8
	MODE_IDLE,                        //app is close  9
	MODE_APP,                         //app open      10
	MODE_APPMANAGE,                   //app manage    11
	MODE_SYSRECOVER=0xFE,
	MODE_LOCKED = 0xFF
};

typedef struct
{
    uint32_t idle;
    uint32_t self_check;
    uint32_t self_check_24H;
}PINPAD_TIME_DEF;


//******************************************
//   ����
//******************************************
extern uchar gWorkMode;
extern uchar gReadyAppid;
extern uchar gAppAuthFlag;
extern uchar gAppProcolType; 
extern uchar gEnableSelfCheck;
extern uchar gInPinEntry;
//******************************************
//    function
//******************************************
//#define SET_STAT(x)   do{gtCurKey.Stat |= (uchar)(x);}while(0)            
//#define CLR_STAT(x)   do{gtCurKey.Stat &= (uchar)(~(x));}while(0)
//#define GET_STAT(x)   (gtCurKey.Stat & (x)) 


#define SA_ucKBHit      KB_Hit
#define SA_vClrKeyBuf   KB_Flush
#define SA_ucInkey(x)   KB_GetKey(x)
#define SA_vDisplay     LCD_Display
#define SA_ucSetInverse LCD_SetInverse
#define SA_vCls         LCD_ClearScreen
#define SA_vLCDLightOn  LCD_LightOn
#define SA_vLCDLightOff LCD_LightOff
#define SA_vBeepMs      BuzzerOn

void LCD_DispDlFsk(uint mode);
void LCD_ClrLine(uint line);
void PPRT_DealWith(void);
uchar PPRT_Send(uchar cmd, uchar *ucData, ushort usLen);
uchar PPRT_Receive(uchar *ucCmd, uchar *ucData, ushort * usLen);
uint Fac_SRAM(uint mode);
uint32_t Factory_IO_Test(uint8_t mode);

uchar CheckPsw(uint appno, uchar pswType);
uint ChangePsw(uint appno, uint mode);
uint InputPtk(uint appno);
uint Check_user_psw(uint appno);
void LCD_ShowPINPrompt(void);
uchar menu_sys(uchar item);
void LCD_DispPlsReboot(uint line, uint stat);
void app_inkey(uchar key);
void app_get_time(uchar *pucData, ushort *pusLen);
void app_set_time(uchar *pucData, ushort *pusLen);
void app_magcard_parse(uchar *pucData, ushort *pusLen);
void PPRT_FactoryTest(uchar *pucData, ushort *pusLen);
void PPRT_TamperPinStatus(uchar *pucData,ushort *pusLen);
void DispScreen(uint mode);
void PadMain(void);

#define DEBUG_PIN0  2
#define DEBUG_PIN1  4
#define DEBUG_PIN2  5

#define SET_IO(pin) //do{PO1|=(1<<(pin));}while(0) 
#define CLR_IO(pin) //do{PO1&=(~(1<<(pin)));}while(0)

#endif









