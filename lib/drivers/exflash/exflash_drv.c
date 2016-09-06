/*
 * =====================================================================================
 *
 *       Filename:  exflash_drv.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  5/17/2016 10:29:08 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  none (), none@itep.com.cn
 *        Company:  start
 *
 * =====================================================================================
 */
#include "w25x_hw.h"
/*
 *���ܣ�д�ⲿFLASH
 *
 *������
 *
 *����ֵ��
 *
 *ע����ȡд���ַ���ڵ�����������д�����ݳ����жϵ�һ��������Ҫд��������������ݳ��ȣ���������ƫ�ƺ�ʣ����ܳ��ȣ�
 *
 */
unsigned int exflash_write(unsigned int addr,unsigned int data_len,unsigned char* data)
{
    unsigned char sectors_buf[4*1024];
    unsigned char *data_offset;
    unsigned char  i = 0;
    unsigned char  sectors_start_no = 0;       //��ַ����������
    unsigned char  sectors_end_no = 0;       //��ַ����������
    unsigned char  sectors_count = 0;       //������ռ����������
    unsigned int  sectors_start_offset = 0;   //�����ڵ�ַƫ��  
    unsigned int sectors_data_len = 0;   //Ӧ�����һ��������д��������� 
    unsigned int sectors_end_offset = 0;   //�����ڵ�ַƫ��  
    unsigned int sectors_size = 4*1024;
     int temp;
    data_offset = data;
    //���㿪ʼ������
    sectors_start_no = addr / sectors_size;
    //���㿪ʼ��ַƫ��
    sectors_start_offset = addr % sectors_size;
    temp = data_len - sectors_size + sectors_start_offset;  
    TRACE("\r\ntemp:%d",temp); 
    if(temp > 0)//һ�������治��
    {
        TRACE("\r\n111");
        sectors_data_len =  sectors_size-sectors_start_offset;
        sectors_count = (data_len - sectors_data_len)/ sectors_size ;
        sectors_end_offset = (data_len - sectors_data_len)% sectors_size;
        //�������������
        sectors_end_no = sectors_count + sectors_start_no;
        if(sectors_end_offset != 0)
        {
           sectors_end_no += 1;

        }
    }
    else
    {
        TRACE("\r\n222");
        sectors_data_len =  data_len;
        sectors_end_no = sectors_start_no;
        sectors_count = 0;
        sectors_end_offset = 0;
    }
    
    TRACE("\r\nsectors_data_len:%d",sectors_data_len); 
    TRACE("\r\nstart no:%d  end_no:%d",sectors_start_no,sectors_end_no);
    TRACE("\r\nstart offset:%d  end offset:%d",sectors_start_offset,sectors_end_offset);
    TRACE("\r\nsectors count:%d",sectors_count);

    //д��ʼ����  
   // if(sectors_start_offset != 0)
    {
        hw_w25x_flash_read(sectors_buf,sectors_start_no*sectors_size,sectors_size);//����һ������������ 
        memcpy(sectors_buf+sectors_start_offset,data_offset,sectors_data_len);
        data_offset = data_offset+sectors_data_len;
        hw_w25_flash_erase_sector(sectors_start_no*sectors_size); 
        hw_w25x_flash_write(sectors_buf,sectors_start_no*sectors_size,sectors_size);
        sectors_start_no ++;
        TRACE("\r\nbuf offset%d",data_offset-data);
    }
    //�м���������
    for(i = 0; i < sectors_count; i++)
    {
        hw_w25_flash_erase_sector(sectors_start_no*sectors_size); 
        hw_w25x_flash_write(data_offset,sectors_start_no*sectors_size,sectors_size);
        data_offset += sectors_size;
        sectors_start_no ++;
    }
    //��������
    if(sectors_end_offset != 0)
    {
        if(sectors_start_no != sectors_end_no)
        {
            TRACE("\r\nexflash:�������");
        }
        TRACE("\r\nbuf offset%d",data_offset-data);
        hw_w25x_flash_read(sectors_buf,sectors_start_no*sectors_size,sectors_size);//����һ������������ 
        memcpy(sectors_buf,data_offset,sectors_end_offset);
        hw_w25_flash_erase_sector(sectors_start_no*sectors_size); 
        hw_w25x_flash_write(sectors_buf,sectors_start_no*sectors_size,sectors_size); 
    }   
       
    return 0;
}
/*
 *���ܣ����ⲿFLASH
 *
 *������
 *
 *����ֵ��
 *
 *ע��
 *
 */
unsigned int exflash_read(unsigned int addr,unsigned int data_len,unsigned char* data)
{
    hw_w25x_flash_read(data,addr,data_len); 

    return 0;
}







