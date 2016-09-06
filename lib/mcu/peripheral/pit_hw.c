

#include "common.h"
#include "pit_hw.h"
//�û��Զ����жϷ���������
//static PIT_ISR_CALLBACK PIT_ISR[4];
static uchar gPitUsetimes=0;
/*
 * LPLD_PIT_Init
 * PITͨ�ó�ʼ��������ѡ��PITx�������ж����ڡ��жϺ���
 * 
 * ����:
 *    pit_init_structure--PIT��ʼ���ṹ�壬
 *                        ���嶨���PIT_InitTypeDef
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */
uint8 LPLD_PIT_Init(PIT_InitTypeDef *pit_init_structure)
{ 
    //���㶨ʱ����ֵ
    PITx pitx = pit_init_structure->PIT_Pitx;
    //�������
    ASSERT( pitx <= PIT3);        //�ж�ģ���
    if ( gPitUsetimes == 0 ) {
//        TRACE("\nPit init");
        SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
        PIT_MCR = 0;
    }
    gPitUsetimes |= (1<<pitx);
    PIT_TFLG(pitx) |= PIT_TFLG_TIF_MASK;//Clear Timer Interrupt Flag
    if(pit_init_structure->PIT_Isr != NULL){
//        PIT_ISR[pitx] = pit_init_structure->PIT_Isr;
        //ʹ���ж�
        PIT_TCTRL(pitx) = PIT_TCTRL_TIE_MASK;
        //��NVIC��ʹ��PIT�ж�
        enable_irq((IRQn_Type)(PIT0_IRQn + (IRQn_Type)pitx)); 
    }
    //period = (period_ns/bus_period_ns)-1m
    if (pit_init_structure->PIT_Period > 1) {
        PIT_LDVAL(pitx) = pit_init_structure->PIT_Period-1;
    }
    return 1;
}

/*
 * LPLD_PIT_Deinit
 * PIT����ʼ���������ر�ѡ���PITx
 * 
 * ����:
 *    pit_init_structure--PIT��ʼ���ṹ�壬
 *                        ���嶨���PIT_InitTypeDef
 *
 * ���:
 *    ��
 */
void LPLD_PIT_Deinit(PIT_InitTypeDef *pit_init_structure)
{ 
  PITx pitx = pit_init_structure->PIT_Pitx;
  //�������
  ASSERT( pitx <= PIT3);        //�ж�ģ���              
  if ((SIM_SCGC6 & SIM_SCGC6_PIT_MASK) == 0 ) {
      SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
  }
  PIT_TCTRL(pitx) &= PIT_TCTRL_TIE_MASK;//keep TIE
  if(pit_init_structure->PIT_Isr != NULL){
      PIT_TCTRL(pitx) &= (~PIT_TCTRL_TIE_MASK);//Disable TIE
      disable_irq((IRQn_Type)(PIT0_IRQn + (IRQn_Type)pitx)); 
  }
  gPitUsetimes &= (uchar)(~(1<<pitx));
  if ( !gPitUsetimes ) {
      SIM_SCGC6 &= ~SIM_SCGC6_PIT_MASK;
  }
}

/*
 * LPLD_PIT_Enable
 *   ������ʱ��
 * 
 * ����:
 *    pit_init_structure--PIT��ʼ���ṹ�壬
 *                        ���嶨���PIT_InitTypeDef
 *
 * ���:
 *    ��
 *
 */
void LPLD_PIT_Enable(PIT_InitTypeDef *pit_init_structure)
{
    PITx pitx = pit_init_structure->PIT_Pitx;
    //�������
    ASSERT( pitx <= PIT3);                //�ж�PITx
    if ((SIM_SCGC6 & SIM_SCGC6_PIT_MASK) == 0 ) {
        SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    }
    //��ʼ��ʱ
    PIT_TFLG(pitx) |= PIT_TFLG_TIF_MASK;//Clear Timer Interrupt Flag
    PIT_TCTRL(pitx) |= PIT_TCTRL_TEN_MASK;
}

/*
 * LPLD_PIT_Stop
 * ����PITx
 * 
 * ����:
 *    pit_init_structure--PIT��ʼ���ṹ�壬
 *                        ���嶨���PIT_InitTypeDef
 *
 * ���:
 *    ��
 *
 */
void LPLD_PIT_Disable(PIT_InitTypeDef *pit_init_structure)
{
    PITx pitx = pit_init_structure->PIT_Pitx;

    //�������
    ASSERT( pitx <= PIT3);                //�ж�PITx
    if ((SIM_SCGC6 & SIM_SCGC6_PIT_MASK) == 0 ) {
        SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    }
    PIT_TCTRL(pitx) &= (~PIT_TCTRL_TEN_MASK);
}

/* 
 * LPLD_PIT_Delay - [GENERIC] ��ʱʱ��������׼ʱ��50MHz
 * @ 
 */
int LPLD_PIT_Delay (uint32 pitx, uint32 cnt)
{
    //�������
    ASSERT( pitx <= PIT3);        //�ж�ģ���
    if ((SIM_SCGC6 & SIM_SCGC6_PIT_MASK) == 0 ) {
        SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    }
    // �ر�
    PIT_TCTRL(pitx) = 0;
    PIT_LDVAL(pitx) = cnt;
    // ���־
    PIT_TFLG(pitx) |= PIT_TFLG_TIF_MASK;//Clear Timer Interrupt Flag
    // ����
    PIT_TCTRL(pitx) |= PIT_TCTRL_TEN_MASK;
    while ( !(PIT_TFLG(pitx) & PIT_TFLG_TIF_MASK) ) ;
    PIT_TCTRL(pitx) = 0;
    if ( gPitUsetimes == 0 ) {
        SIM_SCGC6 &= ~SIM_SCGC6_PIT_MASK;
    }
    return 0;
}		/* -----  end of function LPLD_PIT_Delay  ----- */

uint32 LPLD_PIT_Restart(PIT_InitTypeDef *pit_init_structure)
{
    PITx pitx = pit_init_structure->PIT_Pitx;

    if ((SIM_SCGC6 & SIM_SCGC6_PIT_MASK) == 0 ) {
//        TRACE("\npit clk");
        SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    }
    gPitUsetimes |= (1<<pitx);
    PIT_TFLG(pitx) |= PIT_TFLG_TIF_MASK; //Clear Timer Interrupt Flag
    if(pit_init_structure->PIT_Isr != NULL){
        //ʹ���ж�
        PIT_TCTRL(pitx) |= PIT_TCTRL_TIE_MASK;
        //��NVIC��ʹ��PIT�ж�
        enable_irq((IRQn_Type)(PIT0_IRQn + (IRQn_Type)pitx)); 
    }
    PIT_TCTRL(pitx) &= ~PIT_TCTRL_TEN_MASK;
    PIT_LDVAL(pitx) = pit_init_structure->PIT_Period;
    PIT_TCTRL(pitx) |= PIT_TCTRL_TEN_MASK;
    return 0;
}

