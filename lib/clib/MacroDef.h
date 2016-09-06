/*********************************************************************************
* ��Ȩ���� (C)2007, 
* �ļ����ƣ� 
*     MacroDef.h
* ��ǰ�汾�� 
*     01-01-01
* ����ժҪ�� 
*     ���ļ�
* �����ĺ�����
*	
* ��ʷ��¼��
*     �޸���		����			    	�汾��
*    
*********************************************************************************/

#ifndef	__MACRODEF_H__
#define	__MACRODEF_H__

#include <string.h>

/**************************************************
       ���Ͷ���
***************************************************/
#define MODE_uchar                //����uchar uint ulong
#define MODE_Int8_16_32_64        //����Int8U Int16U Int32U Int64U
#define MODE_u8_16_32_64          //����u8 u16 u32 u64


#ifdef MODE_uchar
	#ifndef uchar
		#define uchar unsigned char
		#define vuchar volatile uchar
	#endif
	#ifndef UCHAR
		#define UCHAR unsigned char
		#define VUCHAR volatile UCHAR
	#endif 
	#ifndef puchar
		#define puchar unsigned char *
		#define vpuchar volatile puchar
	#endif
	#ifndef PUCHAR
		#define PUCHAR unsigned char *
		#define VPUCHAR volatile PUCHAR
	#endif

	#ifndef ushort
		#define ushort unsigned short
		#define vushort volatile ushort
	#endif
	#ifndef USHORT
		#define USHORT unsigned short
		#define VUSHORT volatile USHORT
	#endif

	#ifndef pushort
		#define pushort unsigned short *
		#define vpushort volatile pushort
	#endif
	#ifndef PUSHORT
		#define PUSHORT unsigned short *
		#define VPUSHORT volatile PUSHORT
	#endif

	#ifndef uint
		#define uint unsigned int
		#define vuint volatile uint
	#endif
	#ifndef UINT
		#define UINT unsigned int
		#define VUINT volatile UINT
	#endif

	#ifndef puint
		#define puint unsigned int *
		#define vpuint volatile puint
	#endif
	#ifndef PUINT
		#define PUINT unsigned int *
		#define VPUINT volatile PUINT
	#endif

	#ifndef ulong
		#define ulong  unsigned long
		#define vulong volatile ulong
	#endif
	#ifndef ULONG
		#define ULONG  unsigned long
		#define VULONG volatile ULONG
	#endif

	#ifndef pulong
		#define pulong  unsigned long *
		#define vpulong volatile pulong
	#endif
	#ifndef PULONG
		#define PULONG  unsigned long *
		#define VPULONG volatile PULONG
	#endif
#endif

//---------------------------------------------------------
#ifdef MODE_Int8_16_32_64
	#ifndef Boolean
		#define Boolean  unsigned int
	#endif
	#ifndef pBoolean
		#define pBoolean unsigned int *
	#endif

	#ifndef Int8S
		#define Int8S signed char
	#endif
	#ifndef pInt8S
		#define pInt8S signed char *
	#endif
	#ifndef Int8U
		#define Int8U unsigned char
	#endif
	#ifndef pInt8U
		#define pInt8U unsigned char *
	#endif

	#ifndef Int16S
		#define Int16S signed short
	#endif
	#ifndef pInt16S
		#define pInt16S signed short *
	#endif
	#ifndef Int16U
		#define Int16U unsigned short
	#endif
	#ifndef pInt16U
		#define pInt16U unsigned short *
	#endif

	#ifndef Int32S
		#define Int32S signed int
	#endif
	#ifndef pInt32S
		#define pInt32S signed int *
	#endif
	#ifndef Int32U
		#define Int32U unsigned int
	#endif
	#ifndef pInt32U
		#define pInt32U unsigned int *
	#endif

	#ifndef Int64S
		#define Int64S signed long long
	#endif
	#ifndef pInt64S
		#define pInt64S signed long long *
	#endif
	#ifndef Int64U
		#define Int64U unsigned long long
	#endif
	#ifndef pInt64U
		#define pInt64U unsigned long long *
	#endif

	#ifndef Flo32
		#define Flo32  float
	#endif

	#ifndef pFlo32
		#define pFlo32 float *
	#endif 
 
	#ifndef Flo64
		#define Flo64  double
	#endif

	#ifndef pFlo64
		#define pFlo64 double *
	#endif
#endif
//---------------------------------------------------------
#ifdef MODE_u8_16_32_64
	#ifndef s8
		#define s8 signed char
	#endif
	#ifndef vs8
		#define vs8 volatile signed char
	#endif
	#ifndef sc8
		#define sc8 signed char  const /* Read Only */
	#endif
	#ifndef vs8
		#define vs8 volatile signed char  const /* Read Only */
	#endif
	#ifndef u8
		#define u8 unsigned char
	#endif
	#ifndef vu8
		#define vu8 volatile unsigned char
	#endif
	#ifndef uc8
		#define uc8 unsigned char  const /* Read Only */
	#endif
	#ifndef vuc8
		#define vuc8 volatile unsigned char  const  /* Read Only */
	#endif

	#ifndef s16
		#define s16 signed short
	#endif
	#ifndef vs16
		#define vs16 volatile signed short
	#endif
	#ifndef sc16
		#define sc16 signed short  const /* Read Only */
	#endif
	#ifndef vsc16
		#define vsc16 volatile signed short  const /* Read Only */
	#endif
	#ifndef u16
		#define u16 unsigned short
	#endif
	#ifndef vu16
		#define vu16 volatile unsigned short
	#endif
	#ifndef uc16
		#define uc16 unsigned short  const /* Read Only */
	#endif
	#ifndef vuc16
		#define vuc16 volatile unsigned short  const  /* Read Only */
	#endif

	#ifndef s32
		#define s32 signed long
	#endif
	#ifndef vs32
		#define vs32 volatile signed long
	#endif
	#ifndef sc32
		#define sc32 signed long  const /* Read Only */
	#endif
	#ifndef vsc32
		#define vsc32 volatile signed long  const /* Read Only */
	#endif
	#ifndef u32
		#define u32 unsigned long
	#endif
	#ifndef vu32
		#define vu32 volatile unsigned long
	#endif
	#ifndef uc32
		#define uc32 unsigned long  const /* Read Only */
	#endif
	#ifndef vuc32
		#define vuc32 volatile unsigned long  const  /* Read Only */
	#endif

	#ifndef s64
		#define s64 signed long long
	#endif
	#ifndef vs64
		#define vs64 volatile signed long long
	#endif
	#ifndef sc64
		#define sc64 signed long long  const /* Read Only */
	#endif
	#ifndef vsc64
		#define vsc64 volatile signed long long  const /* Read Only */
	#endif
	#ifndef u64
		#define u64 unsigned long long
	#endif
	#ifndef vu64
		#define vu64 volatile unsigned long long
	#endif
	#ifndef uc64
		#define uc64 unsigned long long  const /* Read Only */
	#endif
	#ifndef vuc64
		#define vuc64 volatile unsigned long long  const  /* Read Only */
	#endif
#endif
/**************************************************
       �ؼ��ֺ궨��
***************************************************/ 
#ifdef _INLINE_
	#define _INLINE_ inline
#else
	#define _INLINE_ 
#endif

/**************************************************
       �����궨��
***************************************************/ 
#ifndef INT_MAX
	#define INT_MAX		((int)(~0U>>1))
#endif
#ifndef INT_MIN
	#define INT_MIN		(-INT_MAX - 1)
#endif 
#ifndef UINT_MAX
	#define UINT_MAX	(~0U)
#endif 
#ifndef LONG_MAX
	#define LONG_MAX	((long)(~0UL>>1))
#endif 
#ifndef LONG_MIN
	#define LONG_MIN	(-LONG_MAX - 1)
#endif 
#ifndef ULONG_MAX
	#define ULONG_MAX	(~0UL)
#endif 
#ifndef LLONG_MAX
	#define LLONG_MAX	((long long)(~0ULL>>1))
#endif 
#ifndef LLONG_MIN
	#define LLONG_MIN	(-LLONG_MAX - 1)
#endif 
#ifndef ULLONG_MAX	
	#define ULLONG_MAX	    (~0ULL)
#endif 

#ifndef LOW 
    #define   LOW              0
#endif
#ifndef HIGH 
    #define   HIGH             1
#endif
#ifndef YES
	#define    YES    0x01
#endif
#ifndef NO
	#define     NO    0x00
#endif

#ifndef OK
	#define     OK     0x00
#endif
#ifndef ERROR
	#define     ERROR  0x01
#endif

#ifndef ON
	#define     ON     0x01
#endif
#ifndef OFF
	#define     OFF    0x00
#endif


#ifndef FALSE
	#define FALSE (1 == 0)
#endif

#ifndef TRUE
	#define TRUE  (1 == 1)
#endif

#ifndef NULL
	#define NULL ((void*)0)
#endif

/**************************************************
       �����궨��
***************************************************/
//��ȡ��ԱMEMBER�ڽṹ��TYPE�е�ƫ��
#ifndef FPOS
	#define FPOS(TYPE, MEMBER) ((uint)&((TYPE *)0)->MEMBER)
#endif

//��ȡTYPE�ṹ���MEMBER��Ա�Ĵ�С
#ifndef FSIZE
	#define FSIZE(TYPE, MEMBER) (sizeof(((TYPE*)0)->MEMBER))
#endif

#ifndef LEN
	#define LEN(x,Type)   (sizeof(x)/sizeof(Type))  
#endif

#ifndef ARRAY_SIZE
	#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef DIM
	#define DIM(x)    (sizeof(x)/sizeof(x[0]))   //�������
#endif

#ifndef CLRBUF
	#define CLRBUF(Buf)     memset(Buf, 0, sizeof(Buf))  //��buf
#endif

#ifndef FILLBUF
	#define FILLBUF(Buf,x)  memset(Buf, x, sizeof(Buf))  //���buf  
#endif

#ifndef MAX
    #define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
	#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

//�Ͻ����,UPALIGN(3888,4096)=4096,UPALIGN(4096,4096)=4096,UPALIGN(4222,4096)=8192
//�½����,DWNALIGN(3888,4096)=0,DWNALIGN(4096,4096)=4096,DWNALIGN(4222,4096)=4096
#ifndef UPALIGN
	#define  UPALIGN(x,a) (((x)+(a)-1)&~(a-1)) 
#endif
#ifndef DWNALIGN
	#define  DWNALIGN(x,a) ((x)&~(a-1))
#endif

//�õ�ָ����ַaddr��type���ͱ���
#ifndef ADDR2VAR
	#define  ADDR2VAR(addr,type) (*((type *)(addr)))
#endif
//����ַaddrתΪtype����ָ��
#ifndef ADDR2PTR
	#define  ADDR2PTR(addr,type)  ((type *)addr)
#endif
/**************************************************
       gcc���ź궨��
***************************************************/

//����type��Աmember��ָ��ptr����ȡtype��ָ��
#ifndef container_of
#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({			\
                                         const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
                                         (type *)( (char *)__mptr - offsetof(type,member) );})
#endif



/**************************************************
       ת���궨��
***************************************************/
#ifndef LHALFB 
	#define LHALFB(x)     (unsigned char)((x)&0x0F)  //��ȡ�Ͱ��ֽ�
#endif
#ifndef HHALFB 
	#define HHALFB(x)     (unsigned char)(((x)>>4)&0x0F)    //��ȡ�߰��ֽ�
#endif
#ifndef LBYTE 
	#define LBYTE(x)      (unsigned char)((x)&0xFF)       //��ȡ0~7λ
#endif
#ifndef HBYTE 
	#define HBYTE(x)      (unsigned char)(((x)>>8)&0xFF)    //��ȡ8~15λ
#endif
#ifndef H2BYTE 
	#define H2BYTE(x)     (unsigned char)(((x)>>16)&0xFF)   //��ȡ16~23λ
#endif
#ifndef H3BYTE 
	#define H3BYTE(x)     (unsigned char)(((x)>>24)&0xFF)   //��ȡ24~31λ
#endif

#ifndef LHALFW
	#define LHALFW(x)     (unsigned short)(x)      //��ȡ�Ͱ���
#endif
#ifndef HHALFW
	#define HHALFW(x)     (unsigned short)((x)>>16)  //��ȡ�߰���
#endif
//----------------λ�������------------------
#ifndef IFBIT 
	#define IFBIT(Data,bit)      (((uint)Data)&((uint)(1<<(bit))))  //
#endif

#ifndef GETBIT 
	#define GETBIT(Data,bit)     ((((uint)Data)&((uint)(1<<(bit))))>>(bit))  //
#endif

// ��ָ��λold�ƶ��µ�λnewbit ��:0010,0000 MOVEBIT(0x20,5,3)=0000,1000
#ifndef MOVEBIT 
	#define MOVEBIT(Data,oldbit,newbit)     (((((uint)Data)&(uint)(1<<(oldbit)))>>(oldbit))<<(newbit)) 
#endif
// ��ָ��λold���ƶ�Offsetλ ��:0010,0000 MOVERBIT(0x20,5,2)=0000,1000
#ifndef MOVERBIT 
	#define MOVERBIT(Data,oldbit,OffsetBit)    ((((uint)Data)&(uint)(1<<(oldbit)))>>(OffsetBit)) 
#endif
// ��ָ��λold���ƶ�Offsetλ ��:0010,0000 MOVELBIT(0x20,5,2)=1000,0000
#ifndef MOVELBIT 
	#define MOVELBIT(Data,oldbit,OffsetBit)    ((((uint)Data)&(uint)(1<<(oldbit)))<<(OffsetBit)) 
#endif

#ifndef SETBIT 
	#define SETBIT(Data,bit)     (Data = (((uint)Data)|(uint)(1<<(bit)))) 
#endif

#ifndef CLRBIT 
	#define CLRBIT(Data,bit)     (Data = (((uint)Data)&(uint)(~(1<<(bit))))) 
#endif
//----------------�ֽ�ת���������----------------
//���ֽ�ת�� b3<->b0   b2<->b1
#ifndef CONVERTHBYTE  
	#define CONVERTHBYTE(x)  (MOVERBIT(x,3,(3-0))|MOVERBIT(x,2,(2-1))|MOVELBIT(x,0,(3-0))|MOVELBIT(x,1,(2-1)))
#endif
//�ֽ�ת�� b7<->b0   b6<->b1  b5<->b2  b4<->b3
#ifndef CONVERTBYTE
	#define CONVERTBYTE(x)  ((x>>7)|MOVERBIT(x,6,(6-1))|MOVERBIT(x,5,(5-2))|MOVERBIT(x,4,(4-3))|MOVELBIT(x,1,(6-1))|MOVELBIT(x,2,(5-2))|MOVELBIT(x,3,(4-3))|(x<<7))
#endif
//----------------�ֽںϲ��������----------------
//���ֽںϲ���8λ
#ifndef COMBINE8  
	#define COMBINE8(High,Low)         ((unsigned char)((High)<<4) | (unsigned char)((Low)&0x0F))
#endif
//2�ֽںϲ���16λ
#ifndef COMBINE16
	#define COMBINE16(High,Low)        ((unsigned short)((High)<<8) | (unsigned short)((Low)&0xFF))
#endif
//4�ֽںϲ���32λ	
#ifndef COMBINE32
	#define COMBINE32(Hhigh,Hlow,Lhigh,Llow)  ( (unsigned long)((Hhigh)<<24) | (unsigned long)(((unsigned char)(Hlow))<<16) | (unsigned long)(((unsigned char)(Lhigh))<<8) | (unsigned long)((unsigned char)(Llow)))
#endif	

//----------------2����תΪ16����----------------
#define LongToBin(n) (((n >> 21) & 0x80) | \
                      ((n >> 18) & 0x40) | \
                      ((n >> 15) & 0x20) | \
                      ((n >> 12) & 0x10) | \
                      ((n >>  9) & 0x08) | \
                      ((n >>  6) & 0x04) | \
                      ((n >>  3) & 0x02) | \
                      ((n      ) & 0x01))

#define __BIN(n) LongToBin(0x##n##l) 
//2���Ʊ�Ϊ16����1�ֽ�,��:BIN8(10001001)ת��Ϊ0x89
#ifndef BIN8 
	#define BIN8(n)             __BIN(n)
#endif
//2���Ʊ�Ϊ16����2�ֽ�,��:BIN16(10001001,11111001)ת��Ϊ0x89f9
#ifndef BIN16  
	#define BIN16(High,Low)     ((__BIN(High)  <<  8UL) +  __BIN(Low))
#endif                                      
//2���Ʊ�Ϊ16����4�ֽ�
#ifndef BIN32                                     
	#define BIN32(HHigh,Hlow,LHigh,Llow) ((((unsigned long)__BIN(HHigh)) << 24UL) + \
                            	           (((unsigned long)__BIN(Hlow)) << 16UL) + \
                            	           (((unsigned long)__BIN(LHigh)) << 8UL) + \
                              	           (unsigned long)__BIN(Llow))
#endif                                

//----------------����----------------                              
//16BitתΪ2��8Bit,С��ģʽ(���ֽ���ǰ,���ֽ��ں�),һ���������鶨����
#ifndef _2BL
	#define _2BL(a)   (unsigned char)(a),(unsigned char)((a)>>8)
#endif
//16BitתΪ2��8Bit,���ģʽ(���ֽ��ں�,���ֽ���ǰ),һ���������鶨����
#ifndef _2BB
	#define _2BB(a)   (unsigned char)((a)>>8),(unsigned char)(a)
#endif
#ifndef _3BL
	#define _3BL(a)   (unsigned char)(a),(unsigned char)((a)>>8),(unsigned char)((a)>>16)
#endif
#ifndef _3BB
	#define _3BB(a)   (unsigned char)((a)>>16),(unsigned char)((a)>>8),(unsigned char)(a)
#endif
#ifndef _4BL
	#define _4BL(a)   (unsigned char)(a),(unsigned char)((a)>>8),(unsigned char)((a)>>16),(unsigned char)((a)>>24)
#endif
#ifndef _4BB
	#define _4BB(a)   (unsigned char)((a)>>24),(unsigned char)((a)>>16),(unsigned char)((a)>>8),(unsigned char)(a)
#endif

/**************************************************
       �жϷ���ֵ�궨��
***************************************************/
#define  Assert_OK(x)              {if(x)  return x;} //��0����
#define  Assert_OKy(x,y)           {if(x)  return y;}
#define  Assert_Same(x,y,z)  	   {if((x) == (y))  return z;}  //
#define  Assert_Null(x)            {if(!(x))  return x;}  //�жϿ�ָ��


/**************************************************
       ��ģ�鹲�����
***************************************************/
#define     EM_SUCCESS                  (unsigned char)0x00
#define     EM_ERROR                    (unsigned char)0x01
#define     EM_ABOLISH                  (unsigned char)0x1B
#define     EM_DEVICE_BUSY              (unsigned char)0x88  //�豸æ
#define     EM_DEVICE_USED              (unsigned char)0x89  //�豸��ռ��
#define     EM_TIMEOUT                  (unsigned char)0x8A
#define     EM_ERRPARAM                 (unsigned char)0x8B
#define     EM_ERRHANDLE                (unsigned char)0x8C
#define     EM_DEVICE_DISABLE           (unsigned char)0x8D  //�豸δ����
	#define     EM_DEVICE_NOUSED        	EM_DEVICE_DISABLE
#define     EM_ALLOCERR                 (unsigned char)0x8E
#define     EM_FAILED                   (unsigned char)0x8F
#define     EM_NULL                     (void *)0x00

/**************************************************
       ISOModem��WirelessModule�������
***************************************************/
#define	EM_tcpip_SUCCESS	EM_SUCCESS
#define	EM_tcpip_FAIL		EM_ERROR
#define EM_ppp_SUCCESS		EM_SUCCESS
#define EM_ppp_FAIL			EM_ERROR
#define NU_SUCCESS    		EM_SUCCESS
#define NU_NULL	      		EM_NULL

#define  ISOK   "ISOK"
#define  STR_NULL "NULL"

#ifndef DevHandle
typedef unsigned long DevHandle;
#endif

#endif
