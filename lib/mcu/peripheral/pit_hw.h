/**
 * @file HW_PIT.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief PIT�ײ�ģ����غ���
 *
 * ���Ľ���:�������޸�
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 *
 * @par
 * ����������������[LPLD]������ά������������ʹ���߿���Դ���롣
 * �����߿���������ʹ�û��Դ���롣�����μ�����ע��Ӧ���Ա�����
 * ���ø��Ļ�ɾ��ԭ��Ȩ���������������ο����߿��Լ�ע���ΰ�Ȩ�����ߡ�
 * ��Ӧ�����ش�Э��Ļ����ϣ�����Դ���롢���ó��۴��뱾��
 * �������²���������ʹ�ñ��������������κ��¹ʡ��������λ���ز���Ӱ�졣
 * ����������������͡�˵��������ľ���ԭ�����ܡ�ʵ�ַ�����
 * ������������[LPLD]��Ȩ�������߲��ý�������������ҵ��Ʒ��
 */
#ifndef __HW_PIT_H__
#define __HW_PIT_H__
/********************************************************************/
#include "common.h"
#include "cpu.h"
//PITģ���жϻص���������
typedef void (*PIT_ISR_CALLBACK)(void);

//PITģ��Ŷ���
typedef enum PITx
{
  PIT0=0,
  PIT1=1,
  PIT2=2,
  PIT3=3
}PITx;

//PITģ���ʼ���ṹ�壬��������PIT�������
typedef struct
{  
  /*
    ������
      ѡ��PITx
    ȡֵ��
      PIT0��PIT1��PIT2��PIT3
    ��ʼ����
      �����ʼ��
  */
  PITx PIT_Pitx;
  
  /*
    ������
      PITxģ������ڣ���λ20us
    ȡֵ��
      1~0xFFFFFFFF
    ��ʼ����
  */
  uint32 PIT_Period;
//  /*
//    ������
//      PITxģ������ڣ���λus
//    ȡֵ��
//      1~0xFFFFFFFF
//    ��ʼ����
//      �������ʼ����PIT_PeriodUs��PIT_PeriodMs��PIT_PeriodS����һ����ʼ��
//  */
//  uint32 PIT_PeriodUs;
//  
//  /*
//    ������
//      PITxģ������ڣ���λms
//    ȡֵ��
//      1~0xFFFFFFFF
//    ��ʼ����
//      �������ʼ����PIT_PeriodUs��PIT_PeriodMs��PIT_PeriodS����һ����ʼ��
//  */
//  uint32 PIT_PeriodMs;
  
//  /*
//    ������
//      PITxģ������ڣ���λs
//    ȡֵ��
//      1~0xFFFFFFFF
//    ��ʼ����
//      �������ʼ����PIT_PeriodUs��PIT_PeriodMs��PIT_PeriodS����һ����ʼ��
//  */
//  uint32 PIT_PeriodS;

  /*
    ������
      PITx�жϻص�����
    ȡֵ��
      ��������Ϊ�޷���ֵ,�޲���(eg. void isr(void);)
      ��ΪNULLʱ����ʾ����ʼ�ж�
    ��ʼ����
      �������ʼ������δ��ʼ���򲻻ᴥ���ж�
  */
  PIT_ISR_CALLBACK PIT_Isr; 
} PIT_InitTypeDef;
// ��ȡ��ǰ����ֵ
//#define LPLD_PIT_GetValue (pitx)    PIT_CVAL(pitx)
inline uint32 LPLD_PIT_GetValue (uint32 pitx) {return PIT_CVAL(pitx);}
// �ж��Ƿ�ʱ
//#define LPLD_PIT_IfTimeout(pitx)    (PIT_TFLG(pitx)&0x01)
inline uint32 LPLD_PIT_IfTimeout (uint32 pitx) {return (PIT_TFLG(pitx)&0x01);}
// ��������
//#define LPLD_PIT_Start(pitx)        do{PIT_TCTRL(pitx) |= PIT_TCTRL_TEN_MASK;}while(0)
inline void LPLD_PIT_Start (uint32 pitx) {PIT_TCTRL(pitx) |= PIT_TCTRL_TEN_MASK;}
// �رռ���
//#define LPLD_PIT_Stop(pitx)         do{PIT_TCTRL(pitx) &= PIT_TCTRL_TEN_MASK;}while(0)
inline void LPLD_PIT_Stop (uint32 pitx) {PIT_TCTRL(pitx) &= PIT_TCTRL_TEN_MASK;}
    
// ������������
uint32 LPLD_PIT_Restart(PIT_InitTypeDef *pit_init_structure);
//#define LPLD_PIT_Restart(pitx,cnt)   do{\
//    PIT_TCTRL(pitx) &= ~PIT_TCTRL_TEN_MASK;\
//    PIT_LDVAL(pitx) = cnt;\
//    PIT_TFLG(pitx) |= PIT_TFLG_TIF_MASK;\
//    PIT_TCTRL(pitx) |= PIT_TCTRL_TEN_MASK;\
//\}while(0)



//PITͨ�ó�ʼ��������ѡ��PITx�������ж����ڡ��жϺ���
uint8 LPLD_PIT_Init(PIT_InitTypeDef *pit_init_structure);
//PIT����ʼ���������ر�ѡ���PITx
void LPLD_PIT_Deinit(PIT_InitTypeDef *pit_init_structure);
//ʹ��PITx�ж�
void LPLD_PIT_Enable(PIT_InitTypeDef *pit_init_structure);
//����PITx�ж�
void LPLD_PIT_Disable(PIT_InitTypeDef *pit_init_structure);
int LPLD_PIT_Delay (uint32 pitx, uint32 cnt);
void PIT0_IRQHandler(void);

#endif /* __HW_PIT_H__ */
