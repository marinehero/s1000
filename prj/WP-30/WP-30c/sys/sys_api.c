/***************** (C) COPYRIGHT 2014 START Computer equipment *****************
 * File Name          : sys_api.c
 * bfief              : 
 * Author             : luocs()  
 * Email              : luocs@itep.com.cn
 * Version            : V0.00
 * Date               : 12/4/2014 11:47:04 AM
 * Description        : 
 *******************************************************************************/
#include "wp30_ctrl.h"

/*************************************
 *  ����ģ��
 *************************************/
int kb_hit(void)
{
    return drv_kb_hit();
}

int kb_getkey(int timeout_ms)
{
//    if (timeout_ms == 0) {
//        return KEY_TIMEOUT;
//    }
    return drv_kb_getkey(timeout_ms);
}

void kb_flush(void)
{
    drv_kb_clear();
}

void kb_errkey_alarm(void)
{
    sys_beep();
}
/**********************************************************************
* �������ƣ� 
*     
* ���������� 
*     ת��ΪС���㷽ʽ
* ��������� 
*     ��
* ���������
*     ��
* ����ֵ�� 
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void s_Kb_DataStrConvAmt(uchar * disp_buf, uchar * amt_buf, uchar len)
{
	uchar i = 0;
	uchar new_len = 0, bCount = 0;
	uchar *ptmp;
	ptmp = amt_buf;

	for (i = 0; i < len; i++)
	{
		if (ptmp[i] != '0')
			break;
	}

	ptmp += i;
	new_len = len - (uchar) (ptmp - amt_buf);
	amt_buf = ptmp;
	disp_buf[2] = '.';
	disp_buf[5] = 0x0;
	switch (new_len)
	{
	case 0:
		disp_buf[1] = '0';
		disp_buf[3] = '0';
		disp_buf[4] = '0';
		break;
	case 1:
		disp_buf[1] = '0';
		disp_buf[3] = '0';
		disp_buf[4] = *amt_buf;
		break;
	case 2:
		disp_buf[1] = '0';
		disp_buf[3] = *amt_buf;
		disp_buf[4] = *(amt_buf + 1);
		break;
	default:
		bCount = 0;
		for (i = 0; i < (new_len - 2); i++)
		{
			disp_buf[i + 1] = *(amt_buf + i);
		}
		disp_buf[new_len - 1] = '.';
		disp_buf[new_len] = *(amt_buf + new_len - 2);
		disp_buf[new_len + 1] = *(amt_buf + new_len - 1);
		disp_buf[new_len + 1 + 1] = '\0';
		break;
	}
	disp_buf[0] = (new_len < 3) ? (4) : (new_len + 1 + bCount);
	disp_buf[disp_buf[0] + 1] = 0x0;
}

/**********************************************************************
* �������ƣ� 
*     
* ���������� 
*     
* ��������� 
*     ��
* ���������
*     ��
* ����ֵ�� 
*     ��
* ��ʷ��¼��
*     �޸���		����		    	�汾��      �޸ļ�¼
***********************************************************************/
void s_Kb_DispString(uchar flag, uint Mode, uchar * inp_buf, uchar len, uchar fontsize,
	uchar fFlushLeft)
{
	uchar x = 0, y = 0;
	uchar max_len = 0, bOffset = 0;
    //uchar k_CurFontAttr = 0;
	uchar disp_buf[256];

	x = gLcdSys.glcd_x;
	y = gLcdSys.glcd_y;

	memset(disp_buf, 0x00, sizeof(disp_buf));
	if ((Mode & 0x20) && (Mode & 0x02))	// ���븡����(��С����)      //  (new_mode == 3)
	{
		s_Kb_DataStrConvAmt(disp_buf, inp_buf, len);
	}
	else
	{
		disp_buf[0] = len;
		if (Mode & 0x08)				//  (new_mode == 2)
		{
			memset(&disp_buf[1], '*', len);
		}
		else
		{
			memcpy(&disp_buf[1], inp_buf, len);
		}
		disp_buf[1 + len] = 0;
	}

	bOffset = 0;
	max_len = (128 - x) / fontsize;
	/*     if(max_len == 0) */
	/*         return;      */

//	k_CurFontAttr = k_FontAttr;
	if (fFlushLeft)
	{
		if (Mode & 0x0100)
		{
			lcd_printf("%s", &disp_buf[1]);
//			k_FontAttr = 0;
			if (flag)
			{
				lcd_printf("_ ");
			}
			else
			{
				lcd_printf("_");
			}
//			k_FontAttr = k_CurFontAttr;
		}
		else if (max_len != 1)
		{
			if (disp_buf[0] >= max_len)
			{
				bOffset = disp_buf[0] - max_len + 1;
				lcd_printf((char *)&disp_buf[bOffset + 1]);
//				k_FontAttr = 0;
				lcd_printf("_");
//				k_FontAttr = k_CurFontAttr;
			}
			else
			{
				lcd_printf("%s", &disp_buf[bOffset + 1]);
//				k_FontAttr = 0;
				lcd_printf("_");
				if (flag && (disp_buf[0] != max_len - 1))
					lcd_printf(" ");
//				k_FontAttr = k_CurFontAttr;
			}
		}
	}
	else
	{
		if (disp_buf[0] >= max_len)
			bOffset = max_len;
		else
		{
			bOffset = disp_buf[0];
			if (flag)
			{
				lcd_goto(128 - (bOffset + 1) * fontsize, y);
//				k_FontAttr = 0;
				lcd_printf(" ");
//				k_FontAttr = k_CurFontAttr;
			}
		}
		lcd_goto(128 - bOffset * fontsize, y);
		lcd_printf("%s", &disp_buf[disp_buf[0] - bOffset + 1]);
	}
}
/*****************************************************************************************
* ��������:
*              int kb_getstr(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf)
* ��������:
*              �����������ִ���
*              ÿ����������������Ļ����ķ�ʽ��ʱ��������ʾ����Ļ�ϡ�
* �������:
*              input_mode, ������ʾ����ģʽ,����ȡֵ����:
*                   D8 1��0�� �ǣ��������ʱ�ɻ�����ʾ��������
*                   D7	1��0�� �ǣ���StrBufԤ��������Ч
*                   D6	1��0�� ��С������
*                   D5	1��0�� �ܣ���������
*                   D4	1��0�� �ܣ������ַ�
*                   D3	1��0�� �ǣ������뷽ʽ
*                   D2	1��0�� ���ң���������
*                   D1	1��0�� �У���С����
*                   D0	1��0�� ����������ʾ
*             minlen	 0<KB_MAXLEN	��Ҫ���봮����С����
*             maxlen	[minlen, KB_MAXLEN]	��Ҫ���봮����󳤶�
*             timeout_ms     �ȴ�������ʱ����
*             str_buf    ��Ϊ����ʱ,Ӧ���Ȱ�Ԥ����ַ���ͨ��str_buf���͸�API����0x00��β
* �������:
*             str_buf    ��Ϊ���ʱ,str_buf���������ִ�����0x00��β��
* ����ֵ:
*             >=0                    �û�ʵ��������ַ�����
*             KB_CANCEL          �û�ȡ��������CANCEL�����£�
*             KB_TIMEOUT        �û���ʱδ����
*             KB_ERROR           ����ֵ�Ƿ�(����input_modeֵ�Ƿ���minlen>maxlen��maxlen=0����ʼ���ִ����зǷ��ַ�)
***************************************************************************************/
#if 0
const uchar k_KeyTable[MAX_KEYTABLE][10]={"0","1","2AaBbCc","3DdEeFf","4GgHhIi","5JjKkLl","6MmNmOo","7PpQqRrSs","8TtUuVv","9WwXxYyZz"}; 
const uchar k_KeySymbol[40]=".,*# \\:;+-=?$&%!~@^()|/_[]{}<>`\'\"";
int kb_getstr(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf)
{
	uchar x, y, i, j, prekey;
    int keystatu = 0;
	int len;
	uchar new_mode;
    //uchar old_fontattr;
	uchar fFlushLeft, fReverseEcho, fStrInput;
	uchar sBuf[260], temp[2];
	//int FontType;
    int FontWidth;
	int key;

	if ((minlen > maxlen) || (maxlen > KB_MAXLEN) || (minlen < 0) || (maxlen <= 0))
	{
		return (-KB_ERROR);
	}

	if (maxlen == 0)
	{
		return (-KB_ERROR);
	}

	if (str_buf == NULL)
	{
		return (-KB_ERROR);
	}

	memset(sBuf, 0, sizeof(sBuf));
	temp[1] = 0;

	x = gLcdSys.glcd_x;
	y = gLcdSys.glcd_y;
	keystatu = 0;
    keystatu = keystatu;
	i = 0;
	// �ж��Ƿ���������
	if (((input_mode & 0x38) == 0) || ((input_mode & 0x18) == 0x18) ||
		((input_mode & 0xa) == 0xa) || ((input_mode & 0x88) == 0x88) ||
		(!(((input_mode & 0x38) == 0x10) || ((input_mode & 0x38) == 0x20)
				|| ((input_mode & 0x38) == 0x8))) || ((input_mode & 0xa2) == 0xa2)
		|| ((input_mode & 0x18) && (input_mode & 0x2)))
	{
		return (-KB_ERROR);
	}
	// �ж���ʾ������
//	if (input_mode & 0x40)
//	{
//		FontType = lcd_set_font(FONT_SIZE16);
//		FontWidth = s_lcd_word_width();
//	}
//	else
//	{
//		// �ϲ���Ҫ��12������
//		/*         FontType = lcd_set_font(FONT_SIZE8); */
//		FontType = lcd_set_font(FONT_SIZE12);
//		FontWidth = s_lcd_word_width();
//	}

    FontWidth = 6;//12����������6

	if (input_mode & 0x20)				// ������������
	{
		new_mode = 0;
	}

	if (input_mode & 0x10)				// ���������ַ�
	{
		new_mode = 1;
	}

	if (input_mode & 0x08)				// ���ܷ�ʽ����
	{
		new_mode = 2;
	}

	if ((new_mode == 0) && (input_mode & 0x02))	// ���븡����(��С����)
	{
		new_mode = 3;
	}

//	fStrInput = 0;						// Ĭ����ʾ��䲻����Ӧ�ó���ֱ���趨
//
//	if (input_mode & 0x80)
//	{
//		fStrInput = 1;					// Ӧ�ó���Ԥ���趨��ʾ����
//	}
	fStrInput = 0;						// F24Ĭ�ϲ���ʾ

	fFlushLeft = 0;						// Ĭ���Ҷ���

	if (input_mode & 0x04)
	{
		fFlushLeft = 1;					// �����
	}

//	fReverseEcho = 1;					// Ĭ�Ϸ�����ʾ
//
//	if (input_mode & 0x01)
//	{
//		fReverseEcho = 0;				// ������ʾ
//	}
	fReverseEcho = 0;					// wp30Ĭ��������ʾ
    fReverseEcho = fReverseEcho;

	prekey = 0;
	len = 0;
	sBuf[0] = 0;
	sBuf[1] = KEY_INVALID;
	if (((new_mode == 0) || (new_mode == 1)) && fStrInput)	// ����, �ַ�
	{
		len = (strlen((char *)str_buf) > maxlen) ? maxlen : strlen((char *)str_buf);
		if (len && fStrInput)
		{
			sBuf[0] = len;
			memcpy(&sBuf[1], str_buf, len);
			sBuf[1 + len] = 0;

			keystatu = 0;
			prekey = 0;
			if ((sBuf[len] >= '0') && (sBuf[len] <= '9'))
			{
				prekey = sBuf[len];
			}
			else if (((sBuf[len] >= 'A') && (sBuf[len] <= 'Z'))
				|| ((sBuf[len] >= 'a' && sBuf[len] <= 'z')))
			{
				for (i = 2; i < MAX_KEYTABLE; i++)
				{
					for (j = 0; j < strlen((char *)k_KeyTable[i]); j++)
					{
						if (sBuf[len] == k_KeyTable[i][j])
						{
							prekey = k_KeyTable[i][0];
							keystatu = j;
							break;
						}
					}
				}
			}
			else
			{
				for (j = 0; j < strlen((char *)k_KeySymbol); j++)
				{
					if (sBuf[len] == k_KeySymbol[j])
					{
						prekey = k_KeySymbol[0];
						keystatu = j;
						break;
					}
				}
			}

			if (prekey == 0)
			{
				len = 0;
				sBuf[0] = 0;
				sBuf[1] = KEY_INVALID;
			}
		}
	}
//	old_fontattr = k_FontAttr;

//	k_FontAttr = fReverseEcho;

	if (new_mode == 0x03)
	{
		s_Kb_DispString(0, input_mode, &sBuf[1], 0, FontWidth, fFlushLeft);
	}
	else
	{
		if (len)
		{
			s_Kb_DispString(0, input_mode, &sBuf[1], len, FontWidth, fFlushLeft);
		}
		else
		{
			if ((x + FontWidth) <= 256 && fFlushLeft)
			{
				lcd_printf("_");
			}
		}
	}

	while (1)
	{
		key = kb_getkey(timeout_ms);
		if (key == (KEY_TIMEOUT))
		{
//			lcd_set_font(FontType);
//			k_FontAttr = old_fontattr;
			return (-KB_TIMEOUT);		// ��ʱ�ް���
		}

		switch (key)
		{
		case KEY_CANCEL:
			sBuf[0] = 0;
			sBuf[1] = KEY_CANCEL;
			sBuf[2] = 0x0;
//			k_FontAttr = old_fontattr;
//			lcd_set_font(FontType);
			return (-KB_CANCEL);
            
		case KEY_ENTER:
			if ((new_mode == 0x03) && (len == 0) && (prekey == KEY0))
			{
				str_buf[0] = '0';
				str_buf[1] = 0;
//				k_FontAttr = old_fontattr;
//				lcd_set_font(FontType);
				return 0;
			}

			if (len >= minlen)
			{
				memcpy(str_buf, sBuf + 1, len);
				str_buf[len] = 0;
//				k_FontAttr = old_fontattr;
//				lcd_set_font(FontType);
				return (len);
			}
			kb_errkey_alarm();
			continue;
            
#if (0)
		case KEY_CLEAR:
			if (len == 0)
			{
				kb_errkey_alarm();
				continue;
			}
			while (len)
			{
				len--;
                if (input_mode & 0x200)
                {         
                    lcd_display(128-6*3,0,FONT_SIZE12,"%3d",len);
                } 
				sBuf[1 + len] = 0;
				lcd_goto(x, y);
				s_Kb_DispString(1, input_mode, &sBuf[1], len, FontWidth, fFlushLeft);
			}
			break;
#endif            
            
//		case KEY_BACKSPACE:
		case KEY_CLEAR:
			if (len == 0)
			{
				kb_errkey_alarm();
				continue;
			}

			len--;
            if (input_mode & 0x200)
            {         
                lcd_display(128-6*3,0,FONT_SIZE12,"%3d",len);
            } 
			sBuf[1 + len] = 0;
			lcd_goto(x, y);
			s_Kb_DispString(1, input_mode, &sBuf[1], len, FontWidth, fFlushLeft);

			keystatu = 0;
			if ((sBuf[len] >= '0') && (sBuf[len] <= '9'))
			{
				prekey = sBuf[len];
			}
			else
			{
				if ((sBuf[len] >= 'A' && sBuf[len] <= 'Z') || (sBuf[len] >= 'a' && sBuf[len] < 'z'))
				{
					for (i = 1; i < MAX_KEYTABLE; i++)
					{
						for (j = 0; j < strlen((char *)k_KeyTable[i]); j++)
						{
							if (sBuf[len] == k_KeyTable[i][j])
							{
								prekey = k_KeyTable[i][0];
								keystatu = j;
								break;
							}
						}
					}
				}
				else
				{
					for (j = 0; j < strlen((char *)k_KeySymbol); j++)
					{
						if (sBuf[len] == k_KeySymbol[j])
						{
//							prekey = KEY_STAR;
							prekey = KEY_F1;
							keystatu = j;
							break;
						}
					}
				}
			}
			break;

         /*****************************
          *
          * wp30 ��ʱ��֧����Щ����
          *
          *****************************/
//		case KEY_UPWARD:
//		case KEY_DOWNWARD:
//		case KEY_JING:
//			if (new_mode != 0x01)		// �����������ַ�
//			{
//				kb_errkey_alarm();
//				continue;
//			}
//
//            if(key == KEY_UPWARD) {
//                keystatu = keystatu - 1;
//            }
//            else if(key == KEY_DOWNWARD || key == KEY_JING) {
//                keystatu = keystatu + 1;
//            }
//			if (prekey == KEY_STAR) {
//                if(keystatu < 0) {
//                    keystatu = strlen((char *)k_KeySymbol) - 1;
//                }
//				else if (keystatu >= strlen((char *)k_KeySymbol))
//				{
//					keystatu = 0;
//				}
//				temp[0] = k_KeySymbol[keystatu];
//			}
//			else
//			{
//                if(keystatu < 0) {
//                    keystatu = strlen((char *)k_KeyTable[prekey - 0x30]) - 1;
//                }
//				else if (keystatu >= strlen((char *)k_KeyTable[prekey - 0x30]))
//				{
//					/*                     if (input_mode & 0x20) // ������������ */
//					/*                     {                                      */
//					keystatu = 0;
//					/*                     } */
//					/*                     else              */
//					/*                     {                 */
//					/*                         keystatu = 1; */
//					/*                     }                 */
//				}
//				temp[0] = k_KeyTable[prekey - 0x30][keystatu];
//			}
//
//			sBuf[len] = temp[0];
//			sBuf[len + 1] = 0;
//			lcd_goto(x, y);
//			s_Kb_DispString(0, input_mode, &sBuf[1], len, FontWidth, fFlushLeft);
//			break;
//		case KEY_STAR:
//			if (new_mode != 0x01)		// �����������ַ�
//			{
//				kb_errkey_alarm();
//				continue;
//			}
//
//			if (len >= 256 || len >= maxlen)
//			{
//				kb_errkey_alarm();
//				continue;
//			}
//
//			len++;
//			keystatu = 0x00;
//
//			temp[0] = k_KeySymbol[0];
//			sBuf[len] = temp[0];
//			prekey = key;
//
//            if (input_mode & 0x200)
//            {         
//                lcd_display(128-6*3,0,FONT_SIZE12,"%3d",len);
//            } 
//			sBuf[len + 1] = 0;
//			lcd_goto(x, y);
//			s_Kb_DispString(0, input_mode, &sBuf[1], len, FontWidth, fFlushLeft);
//			break;

		default:
			if (key < KEY0 || key > KEY9 || len >= 256)
			{
				kb_errkey_alarm();
				continue;
			}
			/*             if ((input_mode & 0x20) || (input_mode & 0x8)) // ������������ */
			/*             {                                                              */
			/*             } else                                                         */
			/*             {                                                              */
			/*                 if(key < KEY0 || key > KEY9 || len >= 256)                 */
			/*                 {                                                          */
			/*                     sys_beep();                                            */
			/*                     sys_beep();                                            */
			/*                     continue;                                              */
			/*                 }                                                          */
			/*             }                                                              */
			if (len >= maxlen)
			{
				kb_errkey_alarm();
				continue;
			}

			len++;
			if ((input_mode & 0x20) || (input_mode & 0x8))	// ������������
			{
				keystatu = 0x00;
			}
			else
			{
				if (key == KEY0 || key == KEY1)
				{
					keystatu = 0x00;
				}
				else
				{
					keystatu = 0x1;
				}
			}

			if ((input_mode & 0x20) || (input_mode & 0x8))	// ������������
			{
				temp[0] = k_KeyTable[key - 0x30][0];
			}
			else
			{
				if (key == KEY0 || key == KEY1)
				{
					temp[0] = k_KeyTable[key - 0x30][0];
				}
				else
				{
					temp[0] = k_KeyTable[key - 0x30][1];
				}
			}
			sBuf[len] = temp[0];
			prekey = key;
			sBuf[len + 1] = 0;			
            if (input_mode & 0x200)
            {         
                lcd_display(128-6*3,0,FONT_SIZE12,"%3d",len);
            } 
            lcd_goto(x, y);
            s_Kb_DispString(0, input_mode, &sBuf[1], len, FontWidth, fFlushLeft);
			break;
		}								//switch(key)
	}									//while
}
#endif

extern uint8_t KB_GetPINStr(uint8_t mode, uint8_t ucMinLen, uint8_t ucMaxLen, uint32_t uiTimeOut, uint32_t *puiRetLen, uint8_t *pucGetBuf);
int kb_getstr(uint input_mode, int minlen, int maxlen, int timeout_ms, char *str_buf)
{
    int ret;
    uint32_t outlen; 
    ret = KB_GetPINStr(0 , minlen, maxlen, timeout_ms, &outlen, (uchar *)str_buf);
    if ( ret != 0 ) {
        return -KB_CANCEL;
    }
//    if(ret == 1)  //ȡ��
//    {
//        return -KB_CANCEL;
//    }
//    else if(ret == 2) //��ʱ
//    {
//        return -KB_CANCEL;
//    }
    return outlen;
}
/*************************************
 *  Һ��ģ��
 *************************************/
void lcd_cls(void)
{
#ifdef CFG_LCD
    drv_lcd_cls();
#endif
}

void lcd_goto(int x, int y)
{
#ifdef CFG_LCD
	if((x < 0)
        || (x >= LCD_LENGTH)
        || (y < 0)
        || (y >= LCD_WIDE))
    {
        return;
    }
    gLcdSys.glcd_x = x;
    gLcdSys.glcd_y = y;
#endif
}
/*************************************
 *  ����ģ��
 *************************************/
void sys_beep(void)
{
    beep_run();
    gwp30SysMemory.BeepTimerCnt10ms = 10;
    gwp30SysMemory.SysTickDeamon.bit.beep = 1;
}

void sys_beep_pro(uint freq, uint duration_ms, int choke)
{
	if(!duration_ms)
	{
		return;
	}

	if(freq < 50 || freq > 20000)
	{
		return;
	}

    drv_beep_open(freq);
    drv_beep_start();
    gwp30SysMemory.BeepTimerCnt10ms = (duration_ms+9)/10;
    gwp30SysMemory.SysTickDeamon.bit.beep = 1;
    if ( choke ) {
        while ( gwp30SysMemory.SysTickDeamon.bit.beep );
    }
}

int calc_tdea(const void *src, void *dst, const uchar *deskey, uint keylen, uint mode)
{
    uint8_t key[16];
	if(src == NULL || dst == NULL || deskey == NULL)
	{
		return(ERROR);
	}
    if (mode != TDEA_ENCRYPT && mode != TDEA_DECRYPT)
    {
        return (ERROR);
    }
	switch(keylen)
	{
	case 8:
        memcpy(key,deskey,8);
        memcpy(key+8,deskey,8);
        if (mode == TDEA_ENCRYPT)
            cau_des2_encrypt(src,key,dst);
        else
            cau_des2_decrypt(src,key,dst);
		break;
	case 16:
        if (mode == TDEA_ENCRYPT)
            cau_des2_encrypt(src,deskey,dst);
        else
            cau_des2_decrypt(src,deskey,dst);
		break;
	case 24:
        if (mode == TDEA_ENCRYPT)
            cau_des3_encrypt(src,deskey,dst);
        else
            cau_des3_decrypt(src,deskey,dst);
		break;
	default:
		return ERROR;
	}
	return OK;
}


void Hash(uchar * DataIn, uint DataInLen, uchar * DataOut)
{
    uint8_t tmp[20];
    uint8_t i;

    sha1(DataIn,DataInLen,(uint8_t*)tmp);
    for (i=0; i<5; i++)
    {
        msb_uint32_to_byte4(*((uint32_t*)tmp+i),(uint8_t*)((uint32_t*)DataOut+i));
    }
}



//void sha_init(SHA_INFO * sha_info)
//{
//
//}
//void sha_update(SHA_INFO * sha_info, const void *src, int count)
//{
//
//}
//void sha_final(void *digest, SHA_INFO * sha_info)
//{
//
//}

//in: plaint_text  len = keyinfo->bits/8
//keyinfo->modulus:  pubkey+00
//out:cipher_text  len = keyinfo->bits/8
//uint Rsa_calt_pub(int inlen, const uchar *in, R_RSA_PUBLIC_KEY *keyinfo,int *outlen, uchar *out)
int rsa_pub_dec(void *outbuf, uint * outlen, const void *inbuf, uint inlen,
                const R_RSA_PUBLIC_KEY * pubkey)
{
    rsa_context rsa;
    uint32_t ret=0;
    uint32_t i;
    uint8_t *p=NULL;
    uint8_t *in = (uint8_t *)inbuf;
    uint8_t *out = (uint8_t *)outbuf;

    rsa_init( &rsa, RSA_PKCS_V15, 0 );
    ret = mpi_read_big_endian(&rsa.N,pubkey->modulus,pubkey->bits>>3);
    ret += mpi_read_big_endian(&rsa.E,pubkey->exponent,sizeof(pubkey->exponent));
    if (ret) {
        TRACE("\nRead rsa error");
        goto PUB_CLEAN;
    }
    rsa.len = ( mpi_msb( &rsa.N ) + 7 ) >> 3;
//    TRACE_BUF("N",(uint8_t *)rsa.N.p,rsa.N.n<<2);
//    TRACE_BUF("E",(uint8_t *)rsa.E.p,rsa.E.n<<2);
//    TRACE("\n-|rsa len:%d",rsa.len);
    if( rsa_check_pubkey(&rsa) != 0)
    {
        TRACE("\nrsa key error");
        ret = 1;
        goto PUB_CLEAN;
    }
    *outlen = 0;
    if (inlen >= rsa.len)
    {
        for (i=0; i<inlen; i += rsa.len)
        {
            if (rsa_public(&rsa,(in+i),out+i)) {
                TRACE("\n-|Erro:rsa pub error");
                ret = -1;
                goto PUB_CLEAN;
            }
            *outlen += rsa.len;
        }
    }
    if (*outlen < inlen) 
    {
        if( ( p = (uint8_t *) malloc(rsa.len+4) ) == NULL ) {
            ret = -1;
            goto PUB_CLEAN;
        }
        memset(p,0,rsa.len);
        i = inlen-*outlen;
        memcpy(p+rsa.len-i,in+*outlen,i);
        if (rsa_public(&rsa,p,out+*outlen)) {
            TRACE("\n-|Erro:rsa pub error");
            free(p);
            p = NULL;
            ret = -1;
            goto PUB_CLEAN;
        }
        *outlen += rsa.len;
        free(p);
        p = NULL;
    }
//    TRACE_BUF("Chiper cau",out,rsa.len);
PUB_CLEAN:
    rsa_free( &rsa );
    return ret;
}
//in: cipher_text  len = keyinfo->bits/8
//keyinfo->modulus:  pubkey+00
//out:cipher_text  len = keyinfo->bits/8
//uint Rsa_calt_pri(int inlen, uchar *in, R_RSA_PRIVATE_KEY *keyinfo,int *outlen, uchar *out)
//int rsa_pri_enc(void *outbuf, uint * outlen, const void *inbuf, uint inlen,
//	const R_RSA_PRIVATE_KEY * prikey)
//{
//    rsa_context rsa;
//    uint32_t ret=0;
//    uint32_t i;
//    uint8_t *p=NULL;
//
//    rsa_init( &rsa, RSA_PKCS_V15, 0 );
//    ret = mpi_read_little_endian(&rsa.N,keyinfo->tPubinfo.modulus,keyinfo->tPubinfo.bits>>3);
//    ret = mpi_read_little_endian(&rsa.D,keyinfo->prikey,keyinfo->tPubinfo.bits>>3);
//    ret += mpi_read_little_endian(&rsa.E,keyinfo->tPubinfo.exponent,sizeof(keyinfo->tPubinfo.exponent));
//    if (ret) {
//        TRACE("\nRead rsa error");
//        goto PUB_CLEAN;
//    }
//    rsa.len = ( mpi_msb( &rsa.N ) + 7 ) >> 3;
////    TRACE_BUF("N",(uint8_t *)rsa.N.p,rsa.N.n<<2);
////    TRACE_BUF("E",(uint8_t *)rsa.E.p,rsa.E.n<<2);
////    TRACE("\n-|rsa len:%d",rsa.len);
//    if( rsa_check_pubkey(&rsa) != 0)
//    {
//        TRACE("\nrsa key error");
//        ret = 1;
//        goto PUB_CLEAN;
//    }
//    *outlen = 0;
//    if (inlen >= rsa.len)
//    {
//        for (i=0; i<inlen; i += rsa.len)
//        {
//            if (rsa_private(&rsa,in+i,out+i)) {
//                TRACE("\n-|Erro:rsa pub error");
//                ret = -1;
//                goto PUB_CLEAN;
//            }
//            *outlen += rsa.len;
//        }
//    }
//    if (*outlen < inlen) 
//    {
//        if( ( p = (uint8_t *) malloc(rsa.len+4) ) == NULL ) {
//            ret = -1;
//            goto PUB_CLEAN;
//        }
//        memset(p,0,rsa.len);
//        i = inlen-*outlen;
//        memcpy(p+rsa.len-i,in+*outlen,i);
//        if (rsa_private(&rsa,p,out+*outlen)) {
//            TRACE("\n-|Erro:rsa pub error");
//            free(p);
//            p = NULL;
//            ret = -1;
//            goto PUB_CLEAN;
//        }
//        *outlen += rsa.len;
//        free(p);
//        p = NULL;
//    }
////    TRACE_BUF("Chiper cau",out,rsa.len);
//PUB_CLEAN:
//    rsa_free( &rsa );
//    return ret;
//}

/*************************************
 *  flash��дģ��
 *************************************/
int flash_app_erasedata(uint addr)
{
    uchar ret = 0;
    if ( addr > (FILESYSTEM_LEN/FLASH_SECTOR_SIZE-1)) {
        return -3901;
    }
    ret = drv_flash_SectorErase(FILESYSTEM_STARTADDRESS+addr*FLASH_SECTOR_SIZE);
    return ret;
}
int flash_app_writedata(uint FlashStartAddress, uint NumberOfBytes,uchar *data)
{
    uchar ret;

    if ( data == NULL) {
        return -3901;
    }
    if ((FILESYSTEM_STARTADDRESS+FlashStartAddress+NumberOfBytes) > FILESYSTEM_ENDADDRESS) {
        return -3901;
    }
    ret = flash_write(FILESYSTEM_STARTADDRESS+FlashStartAddress, NumberOfBytes, data);
    return ret;
}
int flash_app_readdata(uint FlashStartAddress, uint NumberOfBytes,uchar *data)
{
    if ( data == NULL) {
        return -3901;
    }
    if ((FILESYSTEM_STARTADDRESS+FlashStartAddress+NumberOfBytes) > FILESYSTEM_ENDADDRESS) {
        return -3901;
    }
    memcpy(data, (void *)(FILESYSTEM_STARTADDRESS+FlashStartAddress), NumberOfBytes);
    return 0;
}


