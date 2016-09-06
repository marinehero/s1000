/*
 * =====================================================================================
 *
 *       Filename:  authmanage.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  6/20/2016 6:07:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yehf (), yehf@itep.com.cn
 *        Company:  START
 *
 * =====================================================================================
 */

#ifndef __SE_MANAGE__ 
#define __SE_MANAGE__

// ��Կ CA��FSK��SE��START_APK
#define  START_CA_PUBK       0
#define  START_FSK_PUBK      1
#define  START_SE_PUBK       2
#define  START_APK_PUBK      3 
#define  USER_APK_PUBK       4 


typedef struct
{
    unsigned char version[16]; //�̼���Ϣ�����磺��S-980E_CTRL��
    unsigned int pkcver;        //pkc�汾     Ĭ��0
    unsigned int ca;            //CA����Կ����  Ĭ��0
    unsigned int ca_start;      //���̸���Կ����   Ĭ��0
    unsigned int codelen;      //tag��ǰ�Ĺ̼���С
    unsigned int offset;        //�̼�ǩ����ʼƫ�ƣ�Ԥ��
    unsigned int len;           //�̼�ǩ�����ȣ�Ԥ��
    unsigned int issuseindex;       //����������ʵ��Ӧ�ù̼�ǩ����Կʱʹ�ã����ֲ�ͬ�ͻ�
    unsigned char publisher[16]; //"START IFT CA"
    unsigned char buildtime[16]; //ǩ��ʱ��
    unsigned char deadtime[16];  //ǩ������ʱ��
    unsigned char issue[64];      //�ͻ���������Կ֤��ʱʹ��
    unsigned char rfu[52];      //Ԥ��
    unsigned char sha[32];      //SHAУ��������, ���Ͻ���SHA����
    unsigned char RSA[256];     //ǩ���������ģ���˽Կ����
    unsigned char tag[16];       //"CHECK:RSA"
}ST_newtaginfo;


#define LABLE_NRSA256CHECK  "CHECK:RSAN256" //ST_newtaginfo:SHA256+RSA256


#define sys_get_time  sys_GetTime

int se_des_decrypt (unsigned int inlen, void *in, void *out, unsigned int keylen, void *key);
int se_des_encrypt (unsigned int inlen, void *in, void *out, unsigned int keylen, void *key);
int s_DesRSAPubk(uint mode, uint inlen, uchar *in, uchar *out);
/*
 * s_check_tag - [GENERIC] У��ST_newtaginfo���������ؽ��ܺ��shaֵ
 *    type: 0-����̼�  1-Ȩ�ް��̼�
 * @
 */
int s_check_tag (uint type,ST_newtaginfo *ptag, uchar *sha);
/*
 * s_startpubk_decrypt - [GENERIC] ʹ�ó��̹�Կ����
 *     type: 0-CA��Կ 1-�����̹̼���Կ 2-����������Կ 3-��ʵ��Ӧ�ù�Կ
 *     uilen:�������ݳ��ȣ�����С�ڹ�Կ����
 * @
 */
int s_startpubk_decrypt (unsigned int type, unsigned int uilen, unsigned char *in, unsigned char *out, unsigned int* outlen);
/*
 * s_check_firmware - [GENERIC] �̼�ǩ��
 *  type:�̼�����  0-����̼�  1-Ȩ�ް��̼�
 *  sha:SHAֵ
 *  tag:ST_newtaginfo�ṹ��
 *  ����ֵ: < 0: ����
 *          >=0: �̼�����
 *
 * @
 */
int s_check_firmware (unsigned int type, unsigned char *sha, unsigned char *p);
/*
 * s_scert_authenticate - [GENERIC] Ȩ�ް���֤
 * ����:   inlen:Ȩ�ް����� 
 *         in:Ȩ�ް�
 *
 * ���:   outlen:�������
 *         out:   �����Ϣ: Ȩ�ް�����(1B)+�µļ�Ȩ��(8B)
 *
 * ����ֵ: 0-�ɹ�     1-�����ȴ��� 2-tag�ؼ��ִ��� 3-ǩ��У�����   4-��ͷ���� 
 *         5-�������� 6-ID�Ƚϴ��� 7-����Ȩ�����  8-��Ȩ��Ƚϴ��� 9-д��Ȩ�����
 * @
 */
int se_cert_authenticate (uint inlen, uchar *in, uint *outlen, uchar *out);
/*
 * s_OperAuthICCard - [GENERIC] IC����֤
 * ����:   iCmdId:   ����      0-�������� 1-������� 2-������Ȩ�ָ� 255-��ȡ��汾
 *         iInLen:   ���볤�ȣ�����Ϊ6λ 
 *         pacInData:���뿨Ƭ����
 *
 * ���:   piOutLen:     �������
 *         pacOutData:   ���������ʾ��������ֵΪ0��4~12ʱ���8λ����   
 	  0-"�����ɹ�",
    1-"������λ",
    2-"�ϵ�ʧ��",
    3-"��Ч��",
    4-"��Ƭ������Ч",
    5-"��Ƭ��֤ʧ��",
    6-"�������",
    7-"�Ƿ���",
    8-"����Ч�����þ�",
    9-"����Ч��������",
    10-"��Ƭ�쳣",
    11-"���볤�ȴ���",
    12-"��Ƭ������",
 *
 * ����ֵ: ����
 * @
 */
int s_OperAuthICCard(int iCmdId,int iInLen,char *pacInData,int *piOutLen, char *pacOutData);

#endif

