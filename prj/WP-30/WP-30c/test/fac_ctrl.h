#ifndef __FAC_CTRL_H__
#define __FAC_CTRL_H__


#define FAC_CTRL_NOTTEST			0   // δ����
#define FAC_CTRL_SUCCESS			1   // ���Գɹ� 
#define FAC_CTRL_FAIL			  	2   // ����ʧ��
#define FAC_CTRL_FORCE_SUC      	3   // ǿ�Ƴɹ�


#define FAC_CTRL_NOINFO			  	-1000
#define FAC_CTRL_ERRORSIZE			-1001
#define FAC_CTRL_INVALPARA			-1002
#define FAC_CTRL_ERRORDATA			-1003
#define FAC_CTRL_CANCEL			  	-1004
// ���Խ׶�
#define FAC_PHASE_DEBUG				0	//�忨���Խ׶�
#define FAC_PHASE_ASSEMBLE			1	//װ���
#define FAC_PHASE_AGING				2	//�ϻ�
#define FAC_PHASE_PRODUCT			3	//��Ʒ��
#define FAC_PHASE_NUM			  	4	//���������ܽ׶���

#define AUTO_TEST_SHIFT_SRAM        0
#define AUTO_TEST_SHIFT_KEY         1
#define AUTO_TEST_SHIFT_LCD         2
#define AUTO_TEST_SHIFT_PRINT       3
#define AUTO_TEST_SHIFT_MAG         4
#define AUTO_TEST_SHIFT_IC          5
#define AUTO_TEST_SHIFT_TF          6
#define AUTO_TEST_SHIFT_SERIAL      7
#define AUTO_TEST_SHIFT_RTC         8
#define AUTO_TEST_SHIFT_TEMP        9
#define AUTO_TEST_SHIFT_NAND        10
#define AUTO_TEST_SHIFT_RFID        11
#define AUTO_TEST_SHIFT_MODEM       12
#define AUTO_TEST_SHIFT_LAN         13
#define AUTO_TEST_SHIFT_WIRELESS    14
#define AUTO_TEST_SHIFT_SIM         15
#define AUTO_TEST_SHIFT_INFR        16
#define AUTO_TEST_SHIFT_USBH        17
#define AUTO_TEST_SHIFT_USB         18
#define AUTO_TEST_SHIFT_BATTERY     19
#define AUTO_TEST_SHIFT_SCANNER     20
#define AUTO_TEST_SHIFT_LED			21
#define AUTO_TEST_SHIFT_BT          22
#define AUTO_TEST_SHIFT_MAX         AUTO_TEST_SHIFT_BT

// ��Ҫ�������Ե�ģ�� ���8��
#define	FAC_CTRL_MODEM			0
#define	FAC_CTRL_WIRELESS		1
#define	FAC_CTRL_BLUETOOTH		2
#define FAC_CTRL_ETHERNET		3
#define FAC_CTRL_SCANNER		4	//ɨ��ͷ
#define	FAC_CTRL_INFR			5

#define DEFAULT_ASSEMBLE_CNF   ((1<<AUTO_TEST_SHIFT_KEY|(1<<AUTO_TEST_SHIFT_LCD) \
                               |(1<<AUTO_TEST_SHIFT_MAG)|(1<<AUTO_TEST_SHIFT_IC) \
                               )
#define DEFAULT_PRODUCT_CNF   ((1<<AUTO_TEST_SHIFT_KEY|(1<<AUTO_TEST_SHIFT_LCD) \
                               |(1<<AUTO_TEST_SHIFT_MAG)|(1<<AUTO_TEST_SHIFT_IC) \
                               )

//�汾��Ӧ�ĺ���Ϣ
#define   MANA_VER_MAC          0    //������
#define   MANA_VER_MAIN         1    //����汾
#define   MANA_VER_FLOOR        2	 //�װ�汾
#define   MANA_VER_BOOT         3	 //BOOT�汾
#define   MANA_VER_CTRL         4    //CTRL�汾
#define   MANA_VER_FONT         5    //FONT�汾
#define   MANA_VER_MAPP         6    //MAOO�汾
#define   MANA_VER_FAC          7    //�������Եİ汾
#define   MANA_VER_SAPP1        8    //��Ӧ��1�İ汾
#define   MANA_VER_SAPP2        9    //��Ӧ��2�İ汾
#define   MANA_VER_SAPP3        10   //��Ӧ��3�İ汾
#define   MANA_VER_SAPP4        11   //��Ӧ��4�İ汾


//Э�����(������)
#define SUB_CMD_DEL_FAC			  	0x02		//ɾ����������
#define SUB_CMD_GET_TESTRES			0x03		//��ȡ�������Խ��
#define SUB_CMD_GET_ATTACK			0x04		//��ȡ�������
#define SUB_CMD_ALARM			    0x05		//����
#define SUB_CMD_WRITE_WOINFO		0x06		//д������Ϣ���ļ�ϵͳ
#define	SUB_CMD_SET_ATTACK			0x07		//���÷������
#define SUB_CMD_MODULE_EXIST		0x08		//��ѯ���ģ���Ƿ����
#define SUB_CMD_DEL_ALLSAPP			0x09		//ɾ��������Ӧ��(������������)
#define SUB_CMD_GET_TIME			0x0a		//��ȡPOSʱ��
#define SUB_CMD_GET_APP_DESCRPT		0x0b		//��ȡ����Ӧ�õ�DESCRIPTION
#define SUB_CMD_SET_MAINBOARDID     0x0C        //��������ID
#define SUB_CMD_SENDINFOR           0x0D        //���͹ػ����������ܿؼ�ɹ���Ϣ��ʧ����Ϣ���������سɹ���ʧ����Ϣ


//�Զ����Խ���ĺ�
#define AUTO_DEBUG_RES      		0   //�忨�Զ����Խ��
#define AUTO_ASSEMBLE_RES   		1   //װ����Զ����Խ��
#define AUTO_AGING_RES      		2   //�ϻ����Խ��
#define AUTO_PRODUCT_RES    		3   //�ܼ��Զ����Խ��
#define ALL_CHECK_OK    			4   //�ܽ��
#define MANA_CHECK_DATE     		5   //�˶�����
#define MANA_CHECK_TAMPER   		6   //�˶Է���
#define MANA_CHECK_DELFAC   		7   //�˶�ɾ����������

/*-----------------------------------------------------------------------------}
 *  new_phase:FAC_PHASE_DEBUG    ������Խ�� D0:1-�ɹ� 0-ʧ�� 
 *                               ������Խ�� D1:1-�ɹ� 0-ʧ��
 *                               D2:1-����ǿ��  D3:1-����ǿ��
 *                               D8:�������    D9:�������
 *
 *            FAC_PHASE_ASSEMBLE 0-δ���� 1-�ɹ�  2-ʧ�� 3-ǿ�Ƴɹ�
 *            FAC_PHASE_AGING    0-δ���� 1-�ɹ�  2-ʧ�� 3-ǿ�Ƴɹ�
 *            FAC_PHASE_PRODUCT  0-δ���� 1-�ɹ�  2-ʧ��
 *-----------------------------------------------------------------------------{*/
int fac_ctrl_get_test_res(int phase, int id,int *result);
int fac_ctrl_set_test_res(int phase, int id,int result);
uchar fac_ctrl_get_cur_phase(void);
int fac_ctrl_set_cur_phase(int phase);
int fac_ctrl_get_autotest_res(int phase);
int fac_ctrl_set_autotest_res(int phase, int res);
int fac_ctrl_get_MBID(uchar *id);
int fac_ctrl_set_MBID(uint8_t mode,uchar *id);
int fac_ctrl_get_test_detail(int phase, int *value);
int fac_ctrl_set_test_detail(int phase, int value);
int fac_write_aginglog (s_aging_log *log);
int fac_read_aginglog (int *id,s_aging_log *log);
int fac_ctrl_get_fac_testinfo(FAC_TESTINFO *fac_tinfo);
int fac_ctrl_set_fac_testinfo(FAC_TESTINFO *fac_tinfo);
int fac_ctrl_set_fac_ctrl_flag(uchar fac_ctrl_flag);
uchar fac_ctrl_get_fac_ctrl_flag(void);
int  fac_ctrl_set_Voucherno(uchar *id);
int fac_ctrl_get_Voucherno(uchar *id);
int fac_save_log(int mode);
#endif


