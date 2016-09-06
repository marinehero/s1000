/*
 * =====================================================================================
 *
 *       Filename:  wp30_ctrl_menu.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  1/5/2015 3:00:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangjp (), zhangjp@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */

#ifndef __WP30_CTRL_MENU_H__
#define __WP30_CTRL_MENU_H__ 

#define MENU_OK               0
#define MENU_TIMEOUT          1001
#define MENU_ERRPARAM         1002
#define MENU_CANCEL           1003

#define     SUPPLY_UN_AFFIRM_STATE         0x0
#define     SUPPLY_VIN_BYOFF_STATE         0x1  //������������
#define     SUPPLY_VIN_BYON_STATE          0x2  //����������+���
#define     SUPPLY_BATTERY_NOR_STATE       0x3  //���������ѹ����
#define     SUPPLY_BATTERY_LOW_STATE       0x4  //��ص͵�ѹ����
#define     SUPPLY_USB_STATE               0x5  //USB����
#define     SUPPLY_VIN_BYFULL_STATE        0x6  //����������+��س���

typedef struct
{
    char       *prompt_chn;
    char       *prompt_en;
    int         selectline;
    int         para;
    int         (*menu_func)(int);
}menu_unit_t;
int local_test_menu(int mode);
int local_menu_operate(int mode);
int select_menu(int mode ,char *chntitle, char *entitle, uint iHighlight, menu_unit_t *pMenu_unit, uint menuSize, uint timeout_ms);
int dl_process(int mode);

#endif
