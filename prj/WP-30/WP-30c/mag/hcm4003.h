/*
 * =====================================================================================
 *
 *       Filename:  Magtek.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/9/2015 
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ty
 *        Company:  START
 *
 * =====================================================================================
 */
#ifndef _HCM4003_H_
#define _HCM4003_H_ 



#define HCM_TIM FTM0
#define HCM_CS_GPIO GPIOD
#define HCM_CS_PIN GPIO_Pin_6
#define HCM_POWER_PTx      PTD6
#define HCM_POWER_PINx     PORT_Pin_6 
#define HCM_CH2_GPIO GPIOC
#define HCM_CH2_PIN GPIO_Pin_2
#define HCM_CH3_GPIO GPIOC
#define HCM_CH3_PIN GPIO_Pin_3

//FTM0_IRQn  ��ʱ��0�ж�

#define MAGMOD	0x10000   //ÿ0x10000��ʱ���ж�һ��
#define TIME_DATA_MAX 10  //�洢 ����ʱ�� ���鳤��

typedef struct
{
	uchar present;                     //��ǰλ��
	uchar len;                         //���ݳ���
	uint data[TIME_DATA_MAX];  
}Time_width;

typedef struct 
{
	uchar swipe_flag;          //����ת����ɱ�־ ��������ת��Ϊ01����--��һ�ν���
	volatile uchar fisrtrun;            //��⵽ˢ����־
	uint t_irq;                //��ʱ������жϴ���
	uint irq_last;             //���һIO�ж�ʱ�Ķ�ʱ���жϴ���
	uint err;
	
}Mag_TimeInfo;               //����ʱ��ʱ����ؽṹ

typedef struct
{  
	uchar byte_bit;           //��һ�ֽ���ʱ ������־�����������һ���ֽ��еĴ��λ�� 
	uchar io_lever;            // ����io�жϣ��жϵ�ƽ����һ���жϵĵ�ƽ�Ƚϣ���ͬ��ƽ��������
    uint sample_width;         //�ŵ��ο�����

    uint count_last;		   //һ�ŵ����һ��ˢ��ʱ��

	Time_width timer_data;    //���ŵ��������ṹ
	
    uchar   buf[MAG_UINT_LEN]; //����������
    int     BitOneNum;         //��������ݳ���
}Mag_DecodeInfo;


/*-----------------------------------------------------------------------------}
 *  ����
 *-----------------------------------------------------------------------------{*/
void hcm4003_enable_VCC(void);
void hcm4003_disable_VCC(void);
void hcm4003_data(int mode);
uchar hcm4003_readdata(void);
void hcm4003_strobe(int mode);
int hcm4003_check(void);

int mag_hcm4003_init(int mode);
int mag_hcm4003_open(int mode);
int mag_hcm4003_close(int mode);
int mag_hcm4003_read(void *Track1, void *Track2, void *Track3);
int mag_hcm4003_ioctl(int mode, int para);
int mag_hcm4003_main (int mode);

#endif



