/*
 * =====================================================================================
 *
 *       Filename:  local_dl_fac.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  1/27/2015 3:54:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangjp (), zhangjp@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */
#ifndef __LOCAL_DL_FAC_H__
#define __LOCAL_DL_FAC_H__ 

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

extern const struct LocalDlCallBack gcLocaldlCallBack;

#endif
