#ifndef _FILEDEF_H_
#define _FILEDEF_H_

//#define FILE_TOTOAL_SIZE    (1024*1024)                         // �ļ�ϵͳռ�ÿռ�1M
#define FILE_TOTOAL_SIZE    (512*1024)      
#define FS_SECTOR_SIZE   0x1000                            // 1��������С
#define MAX_FILES           2                               // ����ļ���
#define FS_UNIT_LEN         0x1000                            // 1�������洢��Ԫ��4096
#define SECTOR_BLOCK        (FS_SECTOR_SIZE / FS_UNIT_LEN) 		// ÿ�������������������ݿ��� 
#define LOG_SIZE            (FS_SECTOR_SIZE - 40)					            // ��־��С
#define DATA_SECTORS_MAX    ((FILE_TOTOAL_SIZE / FS_SECTOR_SIZE) - 3)
#define ALL_BLOCKS          (DATA_SECTORS_MAX * SECTOR_BLOCK)
#define FAT_SIZE            (MAX_FILES * sizeof(FILE_ITEM) + ALL_BLOCKS*2 + 32)

#define FAT_SECTOR_SIZE     (FS_SECTOR_SIZE*1)// FAT�����������Ĵ�С
#define LOG_SECTOR_SIZE     (FS_SECTOR_SIZE*1)// ��־�����������Ĵ�С
#define	DATA_SECTOR_SIZE    (FS_SECTOR_SIZE*1)// ���������Ĵ�С
#define FAT1_SECTORS        ((FAT_SIZE / FAT_SECTOR_SIZE) + !!(FAT_SIZE % FAT_SECTOR_SIZE))
#define FAT2_SECTORS        FAT1_SECTORS
#define LOG_SECTORS         1

#define FILE_BASE_ADDR      0//                                          // FLASH�ڴ����ַ
#define	FAT1_ADDR	        (FILE_BASE_ADDR + 0x0000)	                    // FAT1��ĵ�ַ
#define	FAT2_ADDR	        (FAT1_ADDR + FAT_SECTOR_SIZE * FAT1_SECTORS)	// FAT2��ĵ�ַ
#define	LOG_ADDR	        (FAT2_ADDR + FAT_SECTOR_SIZE * FAT2_SECTORS)   // ��־��ĵ�ַ
#define	DATA_ADDR	        (LOG_ADDR + LOG_SECTOR_SIZE * LOG_SECTORS)     // ������������ʼ��ַ

#define DATA_SECTORS        ((FILE_TOTOAL_SIZE / FS_SECTOR_SIZE) - (FAT1_SECTORS + FAT2_SECTORS + LOG_SECTORS))


//ע�ͣ�
//���FAT����̫С,�ռ䲻���������,MAX_FILES���ܻᱻϵͳ�ض���,
//����ʵ�ʵ�����ļ�������С����Ķ���ֵ

// ��������
void s_fsprompt_begformat(void);
void s_fsprompt_erase(uint32_t Addr);
void s_fsprompt_percent(uint16_t per);

#endif

