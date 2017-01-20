/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ� 
*   MifsHal.h
* ��ǰ�汾�� 
*   01-01-01
* ����ժҪ�� 
*   ���ļ�ʵ��MifsHal.c��ͷ�ļ�,�����ṩ�ӿڡ�
* ��ʷ��¼��
*   �޸���      ����            �汾��          �޸ļ�¼
******************************************************************/
 // ֻ����һ��
#ifndef _MIFS_HAL_H_
#define _MIFS_HAL_H_

#include "wp30_ctrl.h"

/**********************************************************************

                         ȫ�ֺ���ԭ��
                          
***********************************************************************/
#define AS3911_IRQMODE  1
//#define SPI_RFID        SPI_ID_1            //spi0����
#define SPIBUS0_RFID    0


#if defined(PRODUCT_F16_2)
#define BOARD_PIN_RF_INT   PIN_INT_RFID
#define BOARD_PIN_RF_RST   PIN_PWREN_RFID
#define RFID_INTERRUPT 	   BOARD_PIN_RF_INT    //PC5
#define RFID_Reset         BOARD_PIN_RF_RST    //PC4
#define RFID_IRQ_DEV       PIOC_ID
#endif

//#define s_CloseIsr()   disable_dev_irq(RFID_IRQ_DEV) //disable_Interrupts(IRQ_MASK)
//#define s_OpenIsr()    enable_dev_irq(RFID_IRQ_DEV)  //enable_Interrupts(IRQ_MASK)  
/**********************************************************************

                         ȫ�ֺ���ԭ��
                          
***********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

	#define  EI_mifs_vReadReg       EI_mifs_vReadRegSPI
	#define  EI_mifs_vWriteReg      EI_mifs_vWriteRegSPI
	#define  EI_mifs_vSetBitMask    EI_mifs_vSetBitMaskSPI
	#define  EI_mifs_vClrBitMask    EI_mifs_vClrBitMaskSPI
	#define  EI_mifs_vFlushFIFO     EI_mifs_vFlushFIFOSPI

	void EI_mifs_vReadRegSPI(ulong ulLen, uchar ucRegAddr, uchar * pucData);	// ���Ĵ���
	uchar EI_mifs_vWriteRegSPI(ulong ulLen, uchar ucRegAddr, uchar * pucData);	// д�Ĵ���
	void EI_mifs_vSetBitMaskSPI(uchar ucRegAddr, uchar ucBitMask);	// ���Ĵ�����Ӧλ��1
	void EI_mifs_vClrBitMaskSPI(uchar ucRegAddr, uchar ucBitMask);	// ���Ĵ�����Ӧλ��0
	void EI_mifs_vFlushFIFOSPI(void);	//���RC531��FIFO������

	void EI_mifs_vReadReg(ulong ulLen, uchar ucRegAddr, uchar * pucData);	// ���Ĵ���
	uchar EI_mifs_vWriteReg(ulong ulLen, uchar ucRegAddr, uchar * pucData);	// д�Ĵ���
	void EI_mifs_vSetBitMask(uchar ucRegAddr, uchar ucBitMask);	// ���Ĵ�����Ӧλ��1
	void EI_mifs_vClrBitMask(uchar ucRegAddr, uchar ucBitMask);	// ���Ĵ�����Ӧλ��0
	void EI_mifs_vFlushFIFO(void);		//���RC531��FIFO������
	void s_Rfid_vHalInit(void);			//��ʼ��Ӳ������
	void s_Rfid_SetInt(uint mode);
	void s_Rfid_SetRST(uint mode);
	void s_rfid_setRC531Int(uint mode, uchar irq);
	void s_rfid_setRC531IntFlg(uint mode, uchar irq);
    void s_rfid_setRC663Int(uint mode, uint irq);
    void s_rfid_setRC663IntFlg(uint mode, uint irq);
	void s_rfid_setRC531Timercontrol(uchar control);
	void s_rfid_ReadReg(uint len, uchar *ucRegAddr, uchar *pucData);
    uchar spi_exchange_block(uchar data, uchar cs);
    void RFID_IO_IRQHandler(void);
    int s_rfid_mainVersion(int mode);
    int hw_rfid_spi_init(void);
#ifdef __cplusplus
}
#endif

#endif									//_MIFS_HAL_H_
