/*
* =====================================================================================
*
*       Filename:  hcm4003.c
*
*    Description:  ���������ƽ
*
*        Version:  1.0
*        Created:  5/9/2015 
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  ty
*        Company:  START
*
* =====================================================================================
*/
#include "wp30_ctrl.h"
#if (defined CFG_MAGCARD)

#if 11//MAG_PRODUCT == EM_HCM4003 
#include "bitbuf.h"
/*-----------------------------------------------------------------------------}
*  ȫ�ֱ���
*-----------------------------------------------------------------------------{*/


extern uint gMagTeckTimeout;

Mag_TimeInfo MagtimeInfo;  

volatile int          MAG_Swipe_Flag;  //idtechˢ�����ݻ�ȡ�ɹ�

Mag_DecodeInfo        *Decode_buf;


/*****************************�������****************************************/

/**********************************************************************
* �������ƣ� 
*     
* ���������� �������������������
*     
* ��������� ���� 
*     
* ���������
*     
* ����ֵ�� ������ݳ��Ȳ��㷵�� 0
*     
* ע:
*
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
*    
***********************************************************************/
//��ȡ����
ushort get_width(Time_width *width,uchar loca)
{
  if(loca <= width->len)
    return width->data[(width->present + loca)%TIME_DATA_MAX];
  else
    return 0;
}
//����ȡ���� ָ���ƶ�
void present_move(Time_width *width,uchar count)
{
  if(count <= width->len)
  {
    width->present = (width->present + count)%TIME_DATA_MAX;
    width->len -= count;
  }
  else
  {
    //width->present = (width->present + width->data_len)%TIME_DATA_MAX;
    width->len =0;		
  }
}
//��������
uchar insert_width(Time_width *width,uint value)
{
  if(width->len < TIME_DATA_MAX)
  {
    width->data[(width->present + width->len)%TIME_DATA_MAX] = value;
    width->len++;
    return OK;
  }
  else
  {
    return ERROR;
  }
}



/**********************************************************************
* �������ƣ� find_precede
*     
* ���������� Ѱ��ǰ�� 0
*     
* ��������� ���� 
*     
* ���������
*     
* ����ֵ�� 1-�ɹ�  0-ʧ��
*     
* ע:��������Ҫ��4������
*
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
*    
***********************************************************************/
uchar find_precede(Time_width* data)
{
  uchar i, found=0;
  
  for(i=0; i<3; i++) 
  {
    if( (get_width(data,i+1) > ((get_width(data,i)*3)/4)) && (get_width(data,i+1) < ((get_width(data,i)*5)/4)) ) 
    {
      found++;
      if(found >= 3)
        return 1;
    } 
  }
  return 0;
}
/**********************************************************************
* �������ƣ� decode_getwidth
*     
* ��������������ʱ�����Ϊ01,����byte�洢
*     
* ��������� �������� ����
*     
* ���������
*     
* ע: ������������5�����ܽ���
*
* ����ֵ�� ��󳤶�
*     
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
*    
***********************************************************************/
ushort decode_getwidth(Mag_DecodeInfo *mag_data)
{
  uint i,  k;
  uint cur_width, cur_width2;
  
  i = 0;
  //�˵�ë��
  
  if(get_width(&(mag_data->timer_data),i) <(mag_data->sample_width/5)) 
  { 
    cur_width = get_width(&(mag_data->timer_data),i)+ get_width(&(mag_data->timer_data),i+1) + get_width(&(mag_data->timer_data),i+2);
    i+=2;
  } 
  else 
  {
    cur_width = get_width(&mag_data->timer_data,i);
  }
  //�˵�ë��
  if(get_width(&mag_data->timer_data,i+2) <(mag_data->sample_width/5)) 
  { 
    cur_width2 = get_width(&(mag_data->timer_data),i+1) + get_width(&(mag_data->timer_data),i+2) + get_width(&(mag_data->timer_data),i+3);
    k = 3;
  } 
  else 
  {
    cur_width2 = get_width(&(mag_data->timer_data),i+1);
    k = 1;
  }
  
  if(abs(cur_width + cur_width2 - (mag_data->sample_width)) < abs(cur_width - (mag_data->sample_width)))
  {
     if(mag_data->BitOneNum >= MAG_UINT_LEN)
     {
    	mag_data->BitOneNum = 0;
     }
    (mag_data->buf[(mag_data->BitOneNum)]) |= 1<<(mag_data->byte_bit);
    i+=k;
    (mag_data->sample_width) = ((mag_data->sample_width) + cur_width + cur_width2)/2;
    
  } 
  else 
  {
    if(mag_data->BitOneNum >= MAG_UINT_LEN)
    {
        mag_data->BitOneNum = 0;
    }
    (mag_data->buf[(mag_data->BitOneNum)]) |= 0<<(mag_data->byte_bit);
    (mag_data->sample_width) = ((mag_data->sample_width) + cur_width)/2;
  }
  
  (mag_data->byte_bit)++;
  if((mag_data->byte_bit) == 8)
  {
    (mag_data->byte_bit) =0;
    (mag_data->BitOneNum) ++;
  }
  i++;
  present_move(&(mag_data->timer_data), i);		
  return i;
}

/**********************************************************************
* �������ƣ� s_mag_analog2digital
*     
* ���������� ģ���źŽ�Ϊ�����ź�
*     
* ��������� 
*     ��
* ���������
*     ��
* ����ֵ�� 
*   
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
*   
***********************************************************************/
void  decode_getwidth_end(void)
{
  
  /*   while(MagicInfo.timer_data1.data_len)
  {
  decode_getwidth(&(MagicInfo.timer_data1),&(MagicInfo.byte_bit1),&MagicInfo.sample_width1,&EG_MagOper.k_MSR_buf[0]);
}
  */
  while( Decode_buf[1].timer_data.len)
  {
    decode_getwidth(&Decode_buf[1]);
  }
  while( Decode_buf[2].timer_data.len)
  {
    decode_getwidth(&Decode_buf[2]);
  }

  if(Decode_buf[1].BitOneNum > 2 || Decode_buf[2].BitOneNum > 2)
	{
		MAG_Swipe_Flag = TRUE;
	}
  
  MagtimeInfo.swipe_flag = FALSE;
  
}

//�ŵ�����
static void mag_decode(Mag_DecodeInfo *mag_data,int now)
{
 if(mag_data->timer_data.len < TIME_DATA_MAX)
    {
      insert_width(&(mag_data->timer_data),now - mag_data->count_last);
	  
      if(mag_data->timer_data.len >= 7)
      {
        if(mag_data->sample_width != 0)
        {
          decode_getwidth(mag_data);
        }
        else
        {
          if(find_precede(&(mag_data->timer_data)))//Ѱ��ǰ��0 ��ʼ�ο�����ֵ
          {
            mag_data->sample_width = get_width(&(mag_data->timer_data),2);
			present_move(&(mag_data->timer_data), 1);
          }
          else
          {
            present_move(&(mag_data->timer_data), 1);	
          }
        }
      }			 
      mag_data->count_last = now;
      MagtimeInfo.irq_last = MagtimeInfo.t_irq;
    }
    else
    {
      MagtimeInfo.err |= 0x02; 
    }
}

/******************************������**********************************************************/

/*-----------------------------------------------------------------------------}
*  ��������
*-----------------------------------------------------------------------------{*/

//���ν�������
void hcm4003_reset(void)
{
  int i;
  
  MagtimeInfo.err = 0x00;
  MagtimeInfo.fisrtrun = TRUE;
  MAG_Swipe_Flag = FALSE;
  
  for(i=0;i<3;i++)
  {
  		  Decode_buf[i].BitOneNum = 0;
		  Decode_buf[i].timer_data.len = 0;
		  Decode_buf[i].byte_bit = 0;
		  Decode_buf[i].count_last = 0;
		  Decode_buf[i].sample_width = 0;
    Decode_buf[i].BitOneNum = 0;
    memset(Decode_buf[i].buf, 0, sizeof(Decode_buf[i].buf));
  }
  
}

//IO �����е��� 
int hcm4003_check(void)
{
  if(MAG_Swipe_Flag == TRUE)
  {
    return 0;  //0 ΪOK 
  }
  else
  {
    return 1;
  }	
}



//��ʱ���жϴ���
void drv_mag_timer_irq(void)
{ 
  
  uint now;    //��ǰʱ��
  //char i;
  uchar iolever;
  
  //��һ���ж� ������ʱ�� 
  if(MagtimeInfo.fisrtrun == TRUE)
  {   
    MagtimeInfo.fisrtrun = FALSE;
/*
	for(i = 0; i< 3; i++)
		{
		  Decode_buf[i].BitOneNum = 0;
		  Decode_buf[i].timer_data.len = 0;
		  Decode_buf[i].byte_bit = 0;
		  Decode_buf[i].count_last = 0;
		  Decode_buf[i].sample_width = 0;
		}
	MagtimeInfo.irq_last = 0;
	MagtimeInfo.t_irq = 0;
    MagtimeInfo.swipe_flag = FALSE;
    MagtimeInfo.err = 0x00;
   */ 
    LPLD_FTM_Enable_FTM(HCM_TIM); //��������ж�    
  }
  //��ʱ������ж�
  if(LPLD_FTM_IsTOF(HCM_TIM))
  {
    LPLD_FTM_ClearTOF(HCM_TIM);   //����ж�
    if(MagtimeInfo.t_irq < (MagtimeInfo.irq_last + 20))//һ��ˢ�����ʱ
    {	
      MagtimeInfo.t_irq++;
    }
    else
    { 
      //ˢ������
      LPLD_FTM_Disable_FTM(HCM_TIM);       //�ر�����ж�
      MagtimeInfo.t_irq = 0;
 
      decode_getwidth_end();  //�������λ����
  
      MagtimeInfo.fisrtrun = TRUE;
    }
  }
  
  now = MagtimeInfo.t_irq * MAGMOD + LPLD_FTM_GetCounter(HCM_TIM);//��ǰ��ʱ��������ʱ����
  
  //Ch1 �����ж� �ŵ�2
  if(LPLD_FTM_IsCHnF(HCM_TIM,FTM_Ch1))
  {
    LPLD_FTM_ClearCHnF(HCM_TIM,FTM_Ch1);


	iolever = hw_mag_iolevel_ch2read();
    if ( iolever != Decode_buf[1].io_lever ) 
		{
              mag_decode(&(Decode_buf[1]),now);        //�ŵ�����		
        }
    	Decode_buf[1].io_lever = iolever;	   
  }
  //Ch2 �����ж� �ŵ�3
  if(LPLD_FTM_IsCHnF(HCM_TIM,FTM_Ch2))
  {
    LPLD_FTM_ClearCHnF(HCM_TIM,FTM_Ch2);
	iolever = hw_mag_iolevel_ch3read();
    if ( iolever != Decode_buf[2].io_lever ) 
		{
            mag_decode(&(Decode_buf[2]),now);
        }
		Decode_buf[2].io_lever = iolever;
  }
}

void hcm4003_Init(void)
{
    hw_mag_power_init(HCM_CS_GPIO, HCM_POWER_PINx);
}

void hcm4003_enable_VCC(void)
{
    hw_mag_power_open();
}

void hcm4003_disable_VCC(void)
{	
    hw_mag_power_close();
}

int mag_hcm4003_init(int mode)
{
  //IO ��ʱ�� ��ʼ��
  FTM_InitTypeDef ftm_init_struct;

//  hw_set_gpio_output(HCM_CS_GPIO,HCM_CS_PIN);
//  hw_gpio_set_bits(HCM_CS_GPIO,HCM_CS_PIN);
  hcm4003_Init();
  hcm4003_disable_VCC();

  memset((char *)&ftm_init_struct,0,sizeof(ftm_init_struct));
  ftm_init_struct.FTM_Ftmx = HCM_TIM;             //��ʱ��0
  ftm_init_struct.FTM_Mode = FTM_MODE_IC;     //���벶��ģʽ
  ftm_init_struct.FTM_ClkDiv = FTM_CLK_DIV2;   //��Ƶ
  ftm_init_struct.FTM_ToiEnable = TRUE;        //ʹ�ܼ�������ж�
  ftm_init_struct.FTM_Isr = drv_mag_timer_irq; //�жϴ�����
  
  LPLD_FTM_Init(ftm_init_struct);  //��ʼ��
  
  //LPLD_FTM_IC_Enable(HCM_TIM,FTM_Ch1,PTC2,CAPTURE_RIFA); //����PTC2��3�����½��ز���
  //LPLD_FTM_IC_Enable(HCM_TIM,FTM_Ch2,PTC3,CAPTURE_RIFA);
  
  LPLD_FTM_Disable_FTM(HCM_TIM);
  
  set_irq_priority (FTM0_IRQn, INT_PRT_FTM_0);  //�����ж����ȼ�
  
  enable_irq(FTM0_IRQn);


  Decode_buf = (Mag_DecodeInfo *)pMagBufTmp0; //����ſ�ʹ�õĿռ�

  return 0;
}


//�ر�ģ�� ���� PD6����
int mag_hcm4003_close(int mode)
{
  int i;
  
//  hw_gpio_set_bits(HCM_CS_GPIO,HCM_CS_PIN);
  hcm4003_disable_VCC();
  LPLD_FTM_DisableChn(HCM_TIM,FTM_Ch1);
  LPLD_FTM_DisableChn(HCM_TIM,FTM_Ch2);    

  MagtimeInfo.err = 0x00;
  MagtimeInfo.fisrtrun = TRUE;
  MAG_Swipe_Flag = FALSE;
  
  for(i=0;i<3;i++)
  {
  		  Decode_buf[i].BitOneNum = 0;
		  Decode_buf[i].timer_data.len = 0;
		  Decode_buf[i].byte_bit = 0;
		  Decode_buf[i].count_last = 0;
		  Decode_buf[i].sample_width = 0;
    Decode_buf[i].BitOneNum = 0;
    memset(Decode_buf[i].buf, 0, sizeof(Decode_buf[i].buf));
  }
  
  return 0;
}
//����ģ��ʱ ���� PD6����
int mag_hcm4003_open(int mode)
{ 
//  hw_gpio_reset_bits(HCM_CS_GPIO,HCM_CS_PIN);
    hcm4003_enable_VCC();
 TRACE("\r\nHCM4003 Open");
  MagtimeInfo.fisrtrun = TRUE;
  
  LPLD_FTM_IC_Enable(HCM_TIM,FTM_Ch1,PTC2,CAPTURE_RIFA); //����PTC2��3�����½��ز���
  LPLD_FTM_IC_Enable(HCM_TIM,FTM_Ch2,PTC3,CAPTURE_RIFA);     

  hcm4003_reset();

  
  return 0;
}
int mag_hcm4003_ioctl(int mode,int value)
{
  if ( mode == 0 ) {
    //check
    return (int)hcm4003_check();
  }else if ( mode == 1 ) {
    //clear
    hcm4003_reset();
  }
  return 0;
}

//����ֵ  magret[32:0]        [6:4]-3���ŵ�ʧ�ܱ�־ [2:0]-3���ŵ��ɹ���־
int mag_hcm4003_read(void *Track1, void *Track2, void *Track3)
{
  BitBuf SrcBitBuf;
  uchar ret;
  uchar Track1Len ,Track2Len ,Track3Len ;
  int magret = 0;
  uchar  *ptrk;
  
  Track1Len = Track2Len = Track3Len =0;
  Track1Len = Track1Len;
  
  if(MagtimeInfo.err)
  {
    hcm4003_reset();
    return magret;
  }
  
  if (MAG_Swipe_Flag == FALSE)
  {
    // δˢ��
    magret  = ((1<<6) | (1<<5) ) | (1<<4);
    // DISPPOS(magret);
    return magret;
  }
/*
   int i;
TRACE("\r\nTrk2:%d\r\n",Decode_buf[1].BitOneNum);
for(i = 0;i <Decode_buf[1].BitOneNum; i++)
{
  TRACE("%02X,",Decode_buf[1].buf[i]);
}
TRACE("\r\nTrk3:%d\r\n",Decode_buf[2].BitOneNum);
for(i = 0;i <Decode_buf[2].BitOneNum; i++)
{
  TRACE("%02X,",Decode_buf[2].buf[i]);
}
*/
  // Trk2 decode
  ret = EM_mag_NULL;
  ptrk = Track2;
  if(NULL != ptrk) {
    if(Decode_buf[1].BitOneNum)
    {
      bitbufInit(&SrcBitBuf, Decode_buf[1].buf, MAG_UINT_LEN * 8);
      ret = EI_mag_vProcTrk23(&SrcBitBuf, (Decode_buf[1].BitOneNum+1)*8, &Track2Len, ptrk, 1);	// forward
      TRACE("\r\n Trk2F:%X", ret);
      if (ret != EM_mag_SUCCESS)
      {
        ret = EI_mag_vProcTrk23(&SrcBitBuf, (Decode_buf[1].BitOneNum+1)*8, &Track2Len, ptrk, 0);	// backward
        TRACE("\r\n Trk2B:%X", ret);
      }
      if (ret == EM_mag_SUCCESS)
      {
        // DISPBUFTITLE("trk2",Track2Len,0,ptrk);TRACE("\r\n trk2:%s",ptrk);
        magret  |= (1<<1);
      }
    }
    if(ret)
    {
      magret  |= (1<<5);  //�ŵ�2����
      Track2Len = 0;
    }
  }else{
      magret  |= (1<<5);  //�ŵ�2����
      Track2Len = 0;
  }
  //Trk3 decode
  ret = EM_mag_NULL;
  ptrk = Track3;
  if(NULL != ptrk) {
    if(Decode_buf[2].BitOneNum)
    {
      bitbufInit(&SrcBitBuf, Decode_buf[2].buf, MAG_UINT_LEN * 8);
      ret = EI_mag_vProcTrk23(&SrcBitBuf, (Decode_buf[2].BitOneNum+1)*8, &Track3Len, ptrk, 1);	// forward
      TRACE("\r\n Trk3F:%X", ret);
      if (ret != EM_mag_SUCCESS)
      {
        ret = EI_mag_vProcTrk23(&SrcBitBuf, (Decode_buf[2].BitOneNum+1)*8, &Track3Len, ptrk, 0);	// backward
         TRACE("\r\n Trk3B:%X", ret);
      }
      if (ret == EM_mag_SUCCESS)
      {
        //            DISPBUFTITLE("trk3",Track3Len,0,ptrk);TRACE("\r\n trk3:%s",ptrk);
        magret  |= (1<<2);
      }
    }
    if(ret)
    {
      magret  |= (1<<6);
      Track3Len = 0;
    }
  }else{
      magret  |= (1<<6);
      Track3Len = 0;
  }
  
  hcm4003_reset();
  magret |= ((Track3Len<<24) | (Track2Len<<16) ); //(Track1Len<<8)
  return magret;
}
/* 
* magteck_main - [GENERIC] 
* @ 
*/
int mag_hcm4003_main (int mode)
{

  return 0;
}
#endif
#endif
