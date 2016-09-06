/***************** (C) COPYRIGHT 2014 START Computer equipment *****************
 * File Name          : wp30_ctrl_cfg.h
 * bfief              : 
 * Author             : luocs() 
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 8/11/2014 3:10:23 PM
 * Description        : 
 *******************************************************************************/
#ifndef __WP30_CTRL_CFG_H__
#define __WP30_CTRL_CFG_H__ 


//****************************************************************************
//     ���жϵȼ����� 0~15
//
//****************************************************************************
#define  INT_PRI_MASK           8   //�����жϵȼ�
#define  INT_PRI_PSV            15  //���ж�
#define  INT_PRI_TICK           14  //Tick�ж�
#define  INT_PRI_MF             5   //��Ƶ�ⲿ�ж�
#define  INT_PRI_ICCARD_IO      5   //iccard IO�ж�
#define  INT_PRI_LOWPOWER_WAKEUP 5   //�͹��Ļ��� �ж�
#define  INT_PRI_ICCARD_INSERT  6   //iccard������ж�
#define  INT_PRI_ICCARD_TIMEOUT 4   //iccard����ʱ�ж�
#define  INT_PRI_TPRINTER       9   //������ʱ��
#define  INT_PRI_USART0         6   //����0�����ж�
#define  INT_PRI_USART1         6   //����1�����ж�
#define  INT_PRI_USB_HIGH       7
#define  INT_PRI_USB_LOW        7
#define  INT_PRT_FTM_0			9
#define  INT_PRI_FIQ            1		//fiq�ⲿ�ж����ȼ�

/*******************************************************************************
 @---------------------------------��Ʒ���ú�-----------------------------------
 @******************************************************************************/
#define PRODUCT_MPOS         0   //WP-30 
#define PRODUCT_EPP          1   //MK-210V3 

#define PRODUCT_NAME         PRODUCT_MPOS//PRODUCT_EPP 
   
// ��Ʒ���� 
enum{
    STAR_WP30V100 = 0x000,
    STAR_WP30V101 = 0x001,
    STAR_MK210V300 = 0x010,
    STAR_MK210V301 = 0x011,
    STAR_S1000V100 = 0x020
}; 
/*******************************************************************************
 @---------------------------------ģ�����ú�-----------------------------------
 @******************************************************************************/
#if PRODUCT_NAME == PRODUCT_MPOS
#define CFG_LED         1
//#define CFG_LCD         1
#define CFG_ICCARD      1
#define CFG_MAGCARD     1
#define CFG_ICCARD_USERCARD  1
//#define CFG_BLUETOOTH   1
#define CFG_RFID        1
#define CFG_TPRINTER     1
//#define CFG_BATTERY     1
//#define CFG_USBD_CDC    1
#define CFG_TAMPER      1
//#define CFG_EXTERN_PINPAD      1 // �����������
//#define CFG_INSIDE_PINPAD      1 // �����������
#define CFG_LOWPWR      1        // �͹���
//#define CFG_SOFTPWRON   1        
#define CFG_FACTORY     1
#define CFG_MB_TESTER   1   //�忨����
//#define CFG_FS          1   //�ļ�ϵͳ
#define CFG_W25XFLASH   1   //����spiflash
#define CFG_SECURITY_CHIP        //IS8U256A��ȫоƬ
#define CFG_OPENSSL
#define CFG_SCANER      1       // �Ƿ���ɨ��ͷ
/**
 * \brief:      SM2�㷨������ͬʱ����SM3������������Ч
 * \Requires:   CFG_OPENSSL,CFG_SM3,PRODUCT_F16_2
 */
#define CFG_SM2
/**
 * \brief:      SM3�㷨
 * \Requires:   CFG_OPENSSL,PRODUCT_F16_2
 */
#define CFG_SM3
/**
 * \brief:      SM4�㷨
 * \Requires:   CFG_OPENSSL,PRODUCT_F16_2
 */
#define CFG_SM4
#define CFG_SE_MANAGE   //Ȩ�޹���֧��IC��Ȩ�ް�
#endif

/*******************************************************************************
 * ������
 @******************************************************************************/
#define   DISP_FONT  						FONT_SIZE12
#define   DISP_FONT_LINE0     				DISP_FONT*0
#define   DISP_FONT_LINE1     				DISP_FONT*1
#define   DISP_FONT_LINE2     				DISP_FONT*2
#define   DISP_FONT_LINE3     				DISP_FONT*3
#define   DISP_FONT_LINE4     				DISP_FONT*4
#define   DISP_FONT_LINE5     				DISP_FONT*5
#define   DISP_FONT_LINE6     				DISP_FONT*6
#define   DISP_FONT_LINE7     				DISP_FONT*7
#define   DISP_FONT_LINE8     				DISP_FONT*8

#endif





