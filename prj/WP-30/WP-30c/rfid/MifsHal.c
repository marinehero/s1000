/*********************************************************************
* ��Ȩ���� (C)2003, ����ʵ������豸���޹�˾��
* �ļ����ƣ� 
*   MifsHAL.c
* ��ǰ�汾�� 
*   01-01-01
* ����ժҪ�� 
*   ���ļ�ʵ�ַǽӴ���ģ��Ӳ����ز������
* ��ʷ��¼��
*   �޸���          ����                �汾��      �޸ļ�¼
******************************************************************/
#include "wp30_ctrl.h"

#ifdef CFG_RFID
/**********************************************************************

                          ȫ�ֺ�������
                          
***********************************************************************/
/**********************************************************************

                          ����ȫ�ֱ�������
                          
***********************************************************************/

/**********************************************************************

                          �ֲ���������
                          
***********************************************************************/
void EI_mifs_vOpenCS(void);
void EI_mifs_vCloseCS(void);

/**********************************************************************
* �������ƣ� 
*   s_rfid_ReadReg
* ���������� 
*   �Ӷ���Ĵ��������ݣ�SPIģʽ��
* ��������� 
*   ucRegAddr���Ĵ�����ַ�б�
*   pucData����Ŷ���������
*   ulLen:   ����/��ַ����
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
*             06-26-2003          01-01-01
***********************************************************************/
uchar spi_exchange_block(uchar data, uchar cs)
{
//	uchar tmp;
//	SPI_SendCommand(RFID_SPIn,SPIBUS0_RFID,&data,1,&tmp);
//    hw_spi_master_write(LCD_SPIn,LCD_PCS_PIN,data,length); 
//    hw_spi_master_WriteRead(ExFLASH_SPIn,byte,ExFLASH_CSPIN,SPI_PCS_ASSERTED);
    if ( cs == 0 )
        return hw_spi_master_WriteRead(RFID_SPIn,data,RFID_PCS_PIN,SPI_PCS_ASSERTED);
    else
        return hw_spi_master_WriteRead(RFID_SPIn,data,RFID_PCS_PIN,SPI_PCS_INACTIVE);
//	return hw_spi_master_WriteRead(RFID_SPIn,data,RFID_PCS_PIN,SPI_PCS_INACTIVE);
}

void s_rfid_ReadReg(uint len, uchar *ucRegAddr, uchar *pucData)
{
#if defined(EM_RC531_Module) || defined(EM_PN512_Module)  //RC531
    uint i;
    uchar data;
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531 ||
         EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512) {
        EI_mifs_vOpenCS();
        data = (((*ucRegAddr++) << 1) & (uchar) 0x7E);
        pucData[0] = spi_exchange_block(data|0x80, 0);
        for(i=0;i<(len-1);i++)
        {
            pucData[i] =  spi_exchange_block((*ucRegAddr++)<<1, 0);
        }
        pucData[i] = spi_exchange_block(0, 1);
        EI_mifs_vCloseCS();
    }
#endif
}
/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vReadRegSPI (ulong ulLen, uchar ucRegAddr, uchar* pucData);
* ���������� 
*   �ӼĴ��������ݣ�SPIģʽ��
* ��������� 
*   ucRegAddr���Ĵ�����ַ
*   pucData����Ŷ���������
*   ulLen:   ����/��ַ����
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
*             06-26-2003          01-01-01
***********************************************************************/
void EI_mifs_vReadRegSPI(ulong ulLen, uchar ucRegAddr, uchar *pucData)
{
	uint i;
	vuchar tmp;
	if (ulLen == 0)
		return;
	s_CloseIsr();
	EI_mifs_vOpenCS();
#if defined(EM_AS3911_Module) //as3911
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
        i = 0;
        tmp = 0;
        i = i;
        tmp = tmp;
    }
#endif
#if defined(EM_RC531_Module) || defined(EM_PN512_Module)  //RC531
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531 ||
         EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512) {
        ucRegAddr = ((ucRegAddr << 1) & (uchar) 0x7E);	
        pucData[0] = spi_exchange_block(ucRegAddr|0x80, 0);
        for(i=0;i<(ulLen-1);i++)
        {
            //���ͺ�FIFOͬ���ĵ�ַ��0xFF 12022015 chenf
            tmp = spi_exchange_block(ucRegAddr|0x80, 0);
            pucData[i] = tmp;
        }
        tmp = spi_exchange_block(0, 1);
        pucData[i] = tmp;
    }
#endif
#if defined(EM_RC663_Module)
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {
        ucRegAddr = ((ucRegAddr << 1) | (uchar) 0x01);
        pucData[0] = spi_exchange_block(ucRegAddr, 0);	
        for(i=0;i<(ulLen-1);i++)
        {
            tmp = spi_exchange_block(ucRegAddr, 0);
            pucData[i] = tmp; 	
        }
        tmp = spi_exchange_block(0, 1);	
        pucData[i] = tmp;
    }
#endif
	EI_mifs_vCloseCS();
	s_OpenIsr();
}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vWriteRegSPI(ulong ulLen, uchar ucRegAddr, uchar* pucData);
* ���������� 
*   �ӼĴ��������ݣ�SPIģʽ��
* ��������� 
*   ucRegAddr���Ĵ�����ַ
*   pucData��Ҫд������
*   ulLen:   ���ݳ���
* ��������� 
*   ��
* ����ֵ�� 
*    ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
*             06-26-2003          01-01-01
***********************************************************************/
uchar EI_mifs_vWriteRegSPI(ulong ulLen, uchar ucRegAddr, uchar * pucData)
{
	uint i;
	vuchar data;
	if (ulLen == 0)
		return 0;
	s_CloseIsr();
	EI_mifs_vOpenCS();
#if defined(EM_AS3911_Module) //as3911
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
        i = 0;
        data = 0;
        i = i;
        data = data;
    }
#endif
#if defined(EM_RC531_Module) || defined(EM_PN512_Module)  //RC531
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531 ||
         EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512) {
        ucRegAddr = ((ucRegAddr<<1) & 0x7f);
        data = spi_exchange_block(ucRegAddr, 0);
        for(i=0;i<(ulLen-1);i++)
        {
            data = spi_exchange_block(pucData[i], 0);
        }
        data = spi_exchange_block(pucData[i], 1);
    }
#endif
#if defined(EM_RC663_Module)
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663 ) {
        ucRegAddr = ucRegAddr<<1;
        data = spi_exchange_block(ucRegAddr, 0);
        for(i=0;i<(ulLen-1);i++)
        {
            data = spi_exchange_block(pucData[i], 0);
        }
        data = spi_exchange_block(pucData[i], 1);
    }
#endif
	EI_mifs_vCloseCS();
	s_OpenIsr(); 
    return data;
}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vSetBitMaskSPI(uchar ucRegAddr, uchar ucBitMask)
* ���������� 
*   ��ĳ���Ĵ����ض�λ��1��SPIģʽ��
* ��������� 
*   ucRegAddr���Ĵ�����ַ
*   ucBitMask: Ҫ��1λ������
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
*             06-26-2003          01-01-01
***********************************************************************/
void EI_mifs_vSetBitMaskSPI(uchar ucRegAddr, uchar ucBitMask)
{
	uchar ucTemp;
	EI_mifs_vReadRegSPI(1, ucRegAddr, &ucTemp);
	ucTemp |= ucBitMask;
	EI_mifs_vWriteRegSPI(1, ucRegAddr, &ucTemp);
}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vClrBitMaskSPI(uchar ucRegAddr, uchar ucBitMask)
* ���������� 
*   ��ĳ���Ĵ����ض�λ��0��SPIģʽ��
* ��������� 
*   ucRegAddr���Ĵ�����ַ
*   ucBitMask: Ҫ��0λ������
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
*             06-26-2003          01-01-01
***********************************************************************/
void EI_mifs_vClrBitMaskSPI(uchar ucRegAddr, uchar ucBitMask)
{
	uchar ucTemp;
	EI_mifs_vReadRegSPI(1, ucRegAddr, &ucTemp);
	ucTemp &= ~ucBitMask;
	EI_mifs_vWriteRegSPI(1, ucRegAddr, &ucTemp);

}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vFlushFIFOSPI(void)
* ���������� 
*   ���FIFO������(����ģʽ)
* ��������� 
*   ��
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
*             06-26-2003          01-01-01
***********************************************************************/
void EI_mifs_vFlushFIFOSPI(void)
{
	uchar ucTemp;
	uchar ucRegAddr;
		
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module
        ucTemp = 0;
        ucRegAddr = 0;
        ucTemp = ucTemp;
        ucRegAddr = ucRegAddr;
#endif
    }
    else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
    {

#ifdef EM_RC663_Module
        EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_FIFOCONTROL, 0x10);
#endif

    }
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
	
#ifdef EM_PN512_Module
		ucRegAddr = FIFOLevelReg;
		EI_mifs_vReadRegSPI(1, ucRegAddr, &ucTemp);
		ucTemp |= 0x80;
		EI_mifs_vWriteRegSPI(1, ucRegAddr, &ucTemp);
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module
		ucRegAddr = EM_mifs_REG_CONTROL;
		EI_mifs_vReadRegSPI(1, ucRegAddr, &ucTemp);
		ucTemp |= 0x01;
		EI_mifs_vWriteRegSPI(1, ucRegAddr, &ucTemp);
#endif

	}

	
}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vOpenCS(void)
* ���������� 
*    ��ʼʹ��SPIģʽ
* ��������� 
*    ��
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��          �޸ı�ע
*             06-26-2003          01-01-01        ����
***********************************************************************/
void EI_mifs_vOpenCS(void)
{
//	SPI_SetCS(RFID_SPIn,SPIBUS0_RFID,ON);
//    hw_gpio_reset_bits(RFID_RST_GPIO,1<<PORT_Pin_5);
}

/**********************************************************************
* �������ƣ� 
*   void EI_mifs_vCloseCS(void)
* ���������� 
*   ֹͣʹ��SPIģʽ
* ��������� 
*   ��
* ��������� 
*   ��
* ����ֵ�� 
*   ��
* ����˵���� 
*   ��
* ��ʷ��¼��
*   �޸���      ����                �汾��          �޸ı�ע
*             06-26-2003          01-01-01        ����
***********************************************************************/
void EI_mifs_vCloseCS(void)
{
//	SPI_SetCS(RFID_SPIn,SPIBUS0_RFID,OFF);
//    hw_gpio_set_bits(RFID_RST_GPIO,1<<PORT_Pin_5);
}

/**********************************************************************
* �������ƣ� 
*     void s_Rfid_SetInt(uint mode)
* ���������� 
*     ����Rfid�ⲿ�ж�
* ��������� 
*     ��
* ��������� 
*     ��
* ����ֵ�� 
*     ��
* ����˵���� 
*     ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
***********************************************************************/
void s_Rfid_SetInt(uint mode)
{
	if(mode == ON)
	{
        if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
            //3911�޸�Ϊ�ߵ�ƽ�ж�(�������жϿ����п��ܲ��ɿ�)
//            requst_pio_interrupt(RFID_INTERRUPT, INT_HIGH, 3, EI_vMifsHisr);
//            set_irq_priority (RFID_IO_IRQn, INT_PRI_MF);
            enable_irq(RFID_IO_IRQn);
        }else{
//            requst_pio_interrupt(RFID_INTERRUPT, INT_LOW, 3, EI_vMifsHisr);
            enable_irq(RFID_IO_IRQn);
        }
//		enable_pio_interrupt(RFID_INTERRUPT);
	}
	else
	{
//		disable_pio_interrupt(RFID_INTERRUPT);
        disable_irq(RFID_IO_IRQn);
	}
}
void RFID_IO_IRQHandler(void)
{
    if (PORTx_IRQPinx(RFID_IO_PORT,RFID_IO_PINx)) {
        PORTx_IRQPinx_Clear(RFID_IO_PORT,RFID_IO_PINx);
        EI_vMifsHisr();
    }
}
/**********************************************************************
* �������ƣ� 
*     void  s_Rfid_vHalInit(void)
* ���������� 
*     ��ʼ����ƵӲ������
* ��������� 
*     ��
* ��������� 
*     ��
* ����ֵ�� 
*     ��
* ����˵���� 
*     ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
***********************************************************************/
int hw_rfid_spi_init(void)
{
    SPI_InitDef spi_init;

    memset(&spi_init,0,sizeof(SPI_InitDef));
    spi_init.spix = RFID_SPIn;
    memset(spi_init.pcs,PTxInvid,SPI_PCS_MAX);
    spi_init.pcs[RFID_SPI_PCSn] = RFID_PTxy_PCSn;
    spi_init.sck  = RFID_PTxy_SCLK;
    spi_init.mosi = RFID_PTxy_MOSI;
    spi_init.miso = RFID_PTxy_MISO;
    spi_init.mode = SPI_MODE_MASTER;
    spi_init.TxFIFO_switch = FALSE;
    spi_init.RxFIFO_switch = FALSE;
    spi_init.TxCompleteIrq = FALSE;
    spi_init.TxQueueEndIrq = FALSE;
    spi_init.TxFIFO_UnderflowIrq = FALSE;
    spi_init.RxFIFO_OverflowIrq  = FALSE;
    spi_init.TxFIFO_FillIrq  = FALSE;
    spi_init.RxFIFO_DrainIrq = FALSE;
    spi_init.TxFIFO_IrqMode  = FALSE;
    spi_init.RxFIFO_IrqMode  = FALSE;
    spi_init.p_asc  = 1; //����sck��cs���ӳ�ʱ�� ����Ҫ����3911spi�����쳣
    spi_init.t_asc  = 1; //����sck��cs���ӳ�ʱ��
    spi_init.p_csc  = 1; //����sckǰcs�������͵�ʱ��
    spi_init.t_csc  = 0;
    spi_init.t_dt  = 0;
    spi_init.p_dt  = 0;
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
        //spi clk == 8M
        spi_init.attr = SCK_BR_DIV_2|SCK_PBR_DIV_3|MODE_MSBit
            |CPOL_CLK_LOW | CPHA_DATA_CAPTURED_FollowingEdge 
            |FRAME_SIZE_8;
    }else{
        //60M����Ƶ�� 4��Ƶ 2��Ƶ ��8��Ƶ7.5M
        spi_init.attr = SCK_BR_DIV_4|SCK_PBR_DIV_2|MODE_MSBit
            |CPOL_CLK_LOW | CPHA_DATA_CAPTURED_LeadingEdge 
            |FRAME_SIZE_8;
    }

    return hw_spi_init(&spi_init);
}

void s_Rfid_vHalInit(void)
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.GPIOx = RFID_IO_GPIO;//gcKbOpt[index].gpiox;
//    gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_IN_PU 
//        |PORT_Mode_IN_PFE|PORT_Mode_IRQ_EXTI_FEdge;
    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
        gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_IN_PD
            |PORT_Mode_IN_PFE|PORT_Mode_IRQ_EXTI_REdge;
    }else{
        gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_IN_PD
            |PORT_Mode_IN_PFE|PORT_Mode_IRQ_EXTI_FEdge;
    }
    gpio_init.PORT_Pin = RFID_IO_PINx;
    gpio_init.GPIO_Pinx_Mode = GPIO_INPUT(gpio_init.PORT_Pin);
    hw_gpio_init(&gpio_init);

    //17550��λ
    gpio_init.GPIOx = RFID_RST_GPIO;
    gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_Out_DSH;
    gpio_init.PORT_Pin = RFID_RST_PINx;
    gpio_init.GPIO_Pinx_Mode = GPIO_OUTPUT(RFID_RST_PINx);
    hw_gpio_init(&gpio_init);

//    gpio_init.GPIOx = GPIOE;
//    gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_Out_DSH;
//    gpio_init.PORT_Pin = PORT_Pin_6;
//    gpio_init.GPIO_Pinx_Mode = GPIO_OUTPUT(RFID_RST_PINx);
//    hw_gpio_init(&gpio_init);

    hw_rfid_spi_init();

    //����spi��cs��Ϊioʹ�� ��ӦҪ�޸�spi_exchange_block_EX��AS3911_SEN_ON()
    /*
    gpio_init.GPIOx = GPIOE;
    gpio_init.PORT_Mode = PORT_Mode_MUX_gpio|PORT_Mode_Out_SRF|PORT_Mode_Out_DSL;
    gpio_init.PORT_Pin = PORT_Pin_4;
    gpio_init.GPIO_Pinx_Mode = GPIO_OUTPUT(gpio_init.PORT_Pin);
    hw_gpio_init(&gpio_init);
    hw_gpio_set_bits(GPIOE,GPIO_Pin_4);
    */
//	set_pio_output(RFID_Reset,1);
//	enable_pio(RFID_INTERRUPT);   // USC_INT
//	set_pio_input(RFID_INTERRUPT,1);
}
/**********************************************************************
* �������ƣ� 
*    void s_Rfid_SetRST(uint mode)
* ���������� 
*     ����RC531��λ����
* ��������� 
*     ��
* ��������� 
*     ��
* ����ֵ�� 
*     ��
* ����˵���� 
*     ��
* ��ʷ��¼��
*   �޸���      ����                �汾��
***********************************************************************/
void s_Rfid_SetRST(uint mode)
{
//	set_pio_output(RFID_Reset, (int)mode);
    if ( mode ) {
        hw_gpio_set_bits(RFID_RST_GPIO,1<<RFID_RST_PINx);
    }else
        hw_gpio_reset_bits(RFID_RST_GPIO,1<<RFID_RST_PINx);
}

/*****************************************************************
* �������ƣ�
*        
* ����������
*        �����ж�
* �����º������ã�
*        mode=1 ���ж�  =0�ر��ж�
*        irq:�ж�λ
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
****************************************************************/
void s_rfid_setRC531Int(uint mode, uchar irq)
{
	uchar tmp;
	s_CloseIsr();

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module
        tmp = 0;
        tmp = tmp;
#endif
    }
	if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
	
#ifdef EM_PN512_Module

		tmp = 0;
		if(mode == 1)
		{
			gtRfidProInfo.IntEnable |= irq;
			tmp = irq & 0x7F;
			EI_mifs_vSetBitMask(CommIEnReg,tmp);
		}
		else
		{
			gtRfidProInfo.IntEnable &= (~irq);
			tmp = irq & 0x7F;
			EI_mifs_vClrBitMask(CommIEnReg,tmp);
		}	
		gtRfidProInfo.IntEnable &= 0x7F;
		
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		if(mode == 1)
		{
			gtRfidProInfo.IntEnable |= irq;
			tmp = irq | 0x80;
		}
		else
		{
			gtRfidProInfo.IntEnable &= (~irq);
			tmp = irq & 0x3F;
		}	
		gtRfidProInfo.IntEnable &= 0x3F;	
		EI_mifs_vWriteReg(1, EM_mifs_REG_INTERRUPTEN, &tmp);

#endif

	}

	s_OpenIsr();
}

/*****************************************************************
* �������ƣ�
*        
* ����������
*        �����жϱ�־
* �����º������ã�
*        mode=1 ����жϱ�־  =0���ñ�־�ж�
*        irq:�ж�λ
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
****************************************************************/
void s_rfid_setRC531IntFlg(uint mode, uchar irq)
{
	uchar tmp;

    if ( EG_mifs_tWorkInfo.RFIDModule == RFID_Module_AS3911 ) {
#ifdef EM_AS3911_Module
        tmp = 0;
        tmp = tmp;
#endif
    }
	if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_PN512)
	{
	
#ifdef EM_PN512_Module
 
		tmp = irq&0x7F;
		if(mode == 1)
		{
			tmp = irq | 0x80;
		}
		EI_mifs_vWriteReg(1, ComIrqReg, &tmp);
	
#endif

	}
	else if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC531)
	{

#ifdef EM_RC531_Module

		tmp = irq;
		if(mode == 1)
		{
			tmp = irq | 0x80;		
		}
		EI_mifs_vWriteReg(1, EM_mifs_REG_INTERRUPTRQ, &tmp);

#endif

	}
}

/*****************************************************************
* �������ƣ�
*        
* ����������
*        �����ж�
* �����º������ã�
*        mode=1 ���ж�  =0�ر��ж�
*        irq:�ж�λ
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
****************************************************************/
void s_rfid_setRC663Int(uint mode, uint irq)
{
#ifdef EM_RC663_Module
	uchar tmp;
	s_CloseIsr();

	if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{
	
#ifdef EM_RC663_Module

		tmp = 0;
		if(mode == 1)
		{
            //�����ö�ʱ�� irq0���þͲ����ж�
			tmp = (uchar)((irq>>8) & 0x7F);
			gtRfidProInfo.TimerIntEnable |= tmp;
			EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_IRQ1EN,tmp);

			tmp = (uchar)(irq & 0x7F);
			gtRfidProInfo.IntEnable |= tmp;
			EI_mifs_vSetBitMask(PHHAL_HW_RC663_REG_IRQ0EN,tmp);
		}
		else
		{
			tmp = (uchar)(irq & 0x7F);
			gtRfidProInfo.IntEnable &= (~tmp);
			EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_IRQ0EN,tmp);

			tmp = (uchar)((irq>>8) & 0x7F);
			gtRfidProInfo.TimerIntEnable &= (~tmp);
			EI_mifs_vClrBitMask(PHHAL_HW_RC663_REG_IRQ1EN,tmp);
		}	
		gtRfidProInfo.IntEnable &= 0x7F;
		gtRfidProInfo.TimerIntEnable &= 0x7F;
		
#endif

	}
	
	s_OpenIsr();
#endif
}

/*****************************************************************
* �������ƣ�
*        
* ����������
*        �����жϱ�־
* �����º������ã�
*        mode=1 ����жϱ�־  =0���ñ�־�ж�
*        irq:�ж�λ
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
****************************************************************/
void s_rfid_setRC663IntFlg(uint mode, uint irq)
{
#ifdef EM_RC663_Module
	uchar tmp;
    uchar time;

	if(EG_mifs_tWorkInfo.RFIDModule == RFID_Module_RC663)
	{
	
#ifdef EM_RC663_Module
 
		tmp = (uchar)(irq&0x7F);
		time = (uchar)((irq>>8)&0x7F);
		if(mode == 1)
		{
			tmp |= 0x80;
			time |= 0x80;
		}
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_IRQ0, &tmp);
		EI_mifs_vWriteReg(1, PHHAL_HW_RC663_REG_IRQ1, &time);
	
#endif

	}
#endif
}

/*****************************************************************
* �������ƣ�
*        
* ����������
*        ���ö�ʱ��
* �����º������ã�
*        
* �������º�����
*        ��
* ���������
*        ��
* ���������
*        ��
* �� �� ֵ��
*        ��
* ��ʷ��¼��
*        �޸���           �޸�����          �޸�����
****************************************************************/
void s_rfid_setRC531Timercontrol(uchar control)
{
#ifdef EM_RC531_Module

	EI_mifs_vWriteReg(1, EM_mifs_REG_TIMERCONTROL, &control);

#else

	D1(TRACE("\r\n RFID Moudle Wrong!!!"););
	
#endif
}

int s_rfid_mainVersion(int mode)
{
//#if  defined(PRODUCT_F16_2)
//    uchar buf[8];
//    memset(buf, 0, sizeof(buf));
//    sys_read_ver(READ_MAINB_VER,buf);
////    TRACE("\r\nrfid main verion:%x,%x,%x,%x\r\n",buf[0],buf[1],buf[2],buf[3]);
//    if ( buf[3] >= '1' ) {
//        return 1;
//    }else
//        return 0;
//#else
//    return 1;
//#endif
    return 1;
}
#endif
