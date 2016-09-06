#ifndef _THERMALDETECT_H
#define _THERMALDETECT_H

#include "wp30_ctrl.h"
#ifdef CFG_TPRINTER

#define TPRINT_TIMER_ID			PIT1//��ӡ������ʱ��
#define TPRINT_TIMER_IRQn		PIT1_IRQn//��ӡ������ʱ�����ȼ�����
#define TPRINT_BASE_TIME        60  //��׼ʱ��60��Ƶ ��Ƶ60MHZ ʱ��1us
#define TPRINT_ADCMAX_VALUE     0xFFFF  //16·ad������ֵ

#define TP_PAP_TEM_ADC    ADC0
#define TP_BATTERY_ADC    ADC1
#define TP_FUNC_ADC_CTRLR         ADC_CFG2_Mux_ADxxa 
#define TP_TEM_ADC_CHANEL         ADC_SC1n_ChnSelt_SEDP1_DiffDAD1 
#define TP_PAP_ADC_CHANEL         ADC_SC1n_ChnSelt_SE16 
#define TP_BAT_ADC_CHANEL         ADC_SC1n_ChnSelt_SE16 

//#define PAPER_AD_LTHRESHOLD    (0X00200UL)  /* ��ֽʱһ����С��0xA0 1.5V */
#define PAPER_AD_LTHRESHOLD      (10000)  /* ��ֽʱʵ��adԼΪ5500-6000���� */

#define DISABLE_TIMER_INTERRUPT()	do{	\
    tp_timer_close(TPRINT_TIMER_ID, UINT_MAX, tp_printer_handler); \
	}while(0)

#define LATCH_HIGH()	do{	\
		hw_gpio_set_bits(TP_LATCH_GPIO,1<<TP_LATCH_PINx);	\
	}while(0)

#define LATCH_LOW()	      do{	\
	    hw_gpio_reset_bits(TP_LATCH_GPIO,1<<TP_LATCH_PINx);	\
	}while(0)

#define MOTOR_PWR_ON()   do{	\
		hw_gpio_set_bits(TP_MOTPWR_GPIO,1<<TP_MOTPWR_PINx);	\
	}while(0)

#define MOTOR_PWR_OFF()  do{	\
		hw_gpio_reset_bits(TP_MOTPWR_GPIO,1<<TP_MOTPWR_PINx);	\
	}while(0)


#define MOTOR_PHASE_1A_HIGH()  do{	\
		hw_gpio_set_bits(TP_MnA_GPIO,1<<TP_MnA_PINx);	\
	}while(0)


#define MOTOR_PHASE_1A_LOW()   do{	\
		hw_gpio_reset_bits(TP_MnA_GPIO,1<<TP_MnA_PINx);	\
	}while(0)

#define MOTOR_PHASE_1B_HIGH()  do{	\
		hw_gpio_set_bits(TP_MA_GPIO,1<<TP_MA_PINx);	\
	}while(0)

#define MOTOR_PHASE_1B_LOW()   do{	\
		hw_gpio_reset_bits(TP_MA_GPIO,1<<TP_MA_PINx);	\
	}while(0)
	
#define MOTOR_PHASE_2A_HIGH()  do{	\
		hw_gpio_set_bits(TP_MnB_GPIO,1<<TP_MnB_PINx);	\
	}while(0)

#define MOTOR_PHASE_2A_LOW()   do{	\
		hw_gpio_reset_bits(TP_MnB_GPIO,1<<TP_MnB_PINx);	\
	}while(0)

#define MOTOR_PHASE_2B_HIGH() do{	\
		hw_gpio_set_bits(TP_MB_GPIO,1<<TP_MB_PINx);	\
	}while(0)

#define MOTOR_PHASE_2B_LOW()   do{	\
		hw_gpio_reset_bits(TP_MB_GPIO,1<<TP_MB_PINx);	\
	}while(0)

#define STROBE_1_ON()    do{	\
		hw_gpio_set_bits(TP_STB_GPIO,1<<TP_STB_PINx);	\
	}while(0)

#define STROBE_1_OFF()   do{	\
		hw_gpio_reset_bits(TP_STB_GPIO,1<<TP_STB_PINx);	\
	}while(0)

#define PRN_POWER_CHARGE()  do{	\
		hw_gpio_set_bits(TP_WORK_GPIO,1<<TP_WORK_PINx);	\
	}while(0)

#define PRN_POWER_DISCHARGE()  do{	\
		hw_gpio_reset_bits(TP_WORK_GPIO,1<<TP_WORK_PINx);	\
	}while(0)

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

uint16_t tp_temp_value(void);

void tp_gpio_init(void);

void tp_data_send(uint8_t *in_buf, uint32_t in_len);

void tp_spi_int(void);

void tp_ad_init(void);

void tp_ad_close(void);

uint8_t tp_printer_ready(void);

uint16_t  tp_power_value(void);

uint16_t tp_paper_ad_value(void);

void tp_timer_init(PITx time, uint32_t us, IRQ_CALLBACK funtion);

void tp_timer_start(PITx time, uint32_t us, IRQ_CALLBACK funtion);

void tp_timer_close(PITx time, uint32_t us, IRQ_CALLBACK funtion);

#endif
#endif

