#include "common.h"
#include "bitmap.h"

const RGB_QUAD black_white_CLT[] = {
	[0] = {0x00, 0x00, 0x00, 0x00},
	[1] = {0xFF, 0xFF, 0xFF, 0x00},
};

void encoding8to1(const void *src, uint32_t size, void *des)
{
	const uint8_t *source = (uint8_t *)src;
	uint8_t *encodebuf = (uint8_t *)des;
	uint8_t data;
	int32_t i, k, length;

	for (i = 0; i < size; i += 8) {
		length = size - i > 8 ? 8 : size - i;
		for (k = 0, data = 0; k < length; k++) {
			data |= (source[i+k] & 1) << (7 - k);
		}
		*encodebuf++ = ~data;
	}
}

void encodeing1to1(const void *src, uint32_t size, void *des)
{
	const uint8_t *source = (uint8_t *)src;
	uint8_t *encodebuf = (uint8_t *)des;
	int32_t length = (size + 3) / 4 * 4, i;

	for (i = 0; i < size; i++)
	{
		encodebuf[i] = ~source[i];
	}

	memset(&encodebuf[size], 0x00, length - size);
}

int encode2bmp(const void *image, uint32_t width, uint32_t height,
					const RGB_QUAD clt[], uint32_t cltnum,
					uint32_t bitperpixel, uint32_t bytesperline,
					void (*encode_a_line)(const void * src, uint32_t size, void * des),
					void *bmpfilebuffer)
{

	BITMAPINFO bmpinfo;
	int32_t DataSizePerLine, DataSize, k;
	uint8_t	*buffer = (uint8_t *)bmpfilebuffer, *src;

   // ����ɨ��һ����Ҫ�ֽ���
   DataSizePerLine= ((width * bitperpixel + 3 )/8 + 3)/ 4 * 4;
   DataSize = DataSizePerLine * height;

	memset(&bmpinfo, 0x00, sizeof(bmpinfo));

	/* ����λͼ��־ */
	memcpy(&bmpinfo.bmiHeader.wBmpFlag, BMP_FILE_FLAGE, sizeof(BMP_FILE_FLAGE));

	/* �����ļ���С */
	bmpinfo.bmiHeader.bfSize = 	sizeof(bmpinfo.bmiHeader) + /* �ṹͷ */
								sizeof(clt[0]) * cltnum +	/* ��ɫ�� */
								DataSize;	/* ���� */
	/* ������������ļ���ƫ�� */
	bmpinfo.bmiHeader.bfOffBits = bmpinfo.bmiHeader.bfSize - DataSize;

	/* ����λͼͷ��С */
	bmpinfo.bmiHeader.biSize = 0x28;

	/* ���û�����Ϣ */
	bmpinfo.bmiHeader.biWidth			= width;
	bmpinfo.bmiHeader.biHeight			= height;
	bmpinfo.bmiHeader.biPlanes			= 1;
	bmpinfo.bmiHeader.biBitCount		= bitperpixel;
	bmpinfo.bmiHeader.biCompression		= 0; 			/* ��ѹ�� */
	bmpinfo.bmiHeader.biSizeImage		= DataSize;
	bmpinfo.bmiHeader.biXPelsPerMeter	= 0;
	bmpinfo.bmiHeader.biYPelsPerMeter	= 0;
	bmpinfo.bmiHeader.biClrUsed			= cltnum;
	bmpinfo.bmiHeader.biClrImportant	= 0;

	memcpy(buffer, &bmpinfo, sizeof(bmpinfo));
	buffer += sizeof(bmpinfo);

	/* ����ɫ�� */
	memcpy(buffer, clt, sizeof(RGB_QUAD) * cltnum);
	buffer += sizeof(RGB_QUAD) * cltnum;
	/* ������� */
	for (k = 0; k < height; k++) {
		src = (uint8_t *)((unsigned long)image + bytesperline * (height - 1 - k));
		encode_a_line(src, width*bitperpixel/8, buffer);
		buffer += DataSizePerLine;
	}

	return (int)((unsigned long)buffer - (unsigned long)bmpfilebuffer);
}



//	 ��ȡλͼ��Ϣ
uint8_t s_GetBitmapInfo(const void *pvBitmapSrc, uint *puiWidth, uint *puiHigh, uint *puiColor)
{
    BITMAP_INFO_HEADER BitmapInfoHead;

    // ����Ƿ�Ϊbmp�ļ�
    if (memcmp(pvBitmapSrc, "BM", sizeof(uint16_t)) != 0)
    {
        return NO;
    }

    // ����λͼ�ļ�ͷ
    memcpy(&BitmapInfoHead, (void*)((unsigned long)pvBitmapSrc), sizeof(BITMAP_INFO_HEADER));
    *puiWidth = BitmapInfoHead.biWidth;
    *puiHigh  = BitmapInfoHead.biHeight;
    *puiColor = 1 << BitmapInfoHead.biBitCount;
    return YES;
}

// ��λͼ��λͼ��Դ����λͼ�ļ�����
int s_DrawBitmap(int iStartX, int iStartY, uint8_t ucType,uint8_t ucTransparent,
                                const void *pvBitmapSrc,
                                int (*Paint)(int X, int Y, int Color))
{
    BITMAP_INFO_HEADER const *pBitmapInfoHead = (BITMAP_INFO_HEADER *)pvBitmapSrc;
//    RGB_QUAD           RgbQuad[256];
    RGB_QUAD const * pRgbQuad = (RGB_QUAD *)((uint8_t *)pvBitmapSrc+sizeof(BITMAP_INFO_HEADER));
    uint8_t            *pbBitmapPointer = NULL;
    uint8_t		bMask;
//    int32_t		RGBCount;
    int32_t		DataSizePerLine;
    int32_t		iColor, iBits;
    int32_t		iLineBytes, count;
    int32_t		i, k, n;
    int32_t		x, y, Colors;
    int32_t   	FillHeight = 0;
    int32_t   	iRet;

    if (!pvBitmapSrc)
    {
        return(FillHeight);
    }
    // ����Ƿ�Ϊbmp�ļ�
    if (memcmp(pvBitmapSrc, "BM", sizeof(uint16_t)) != 0)
    {
        return(FillHeight);
    }

    // ����λͼ�ļ�ͷ
//    memcpy(&BitmapInfoHead, (void*)((unsigned long)pvBitmapSrc), sizeof(BITMAP_INFO_HEADER));
//    pBitmapInfoHead = (BITMAP_INFO_HEADER *)pvBitmapSrc;

    // ������ɫ������
//    if (pBitmapInfoHead->biBitCount <= 8)
//    {
//        RGBCount = (1<<pBitmapInfoHead->biBitCount) & 0x00FF;
//        memcpy(RgbQuad, (void*)((unsigned long)pvBitmapSrc+sizeof(BITMAP_INFO_HEADER)), RGBCount*sizeof(RGB_QUAD));
//    }

    // ����ɨ��һ����Ҫ�ֽ���
    DataSizePerLine= (pBitmapInfoHead->biWidth * pBitmapInfoHead->biBitCount+31)/8;

    // ����洢һ����Ҫ���ֽ�������4�ı���
    iLineBytes = DataSizePerLine/4*4;

    // ��֧��ѹ��λͼ
    if (pBitmapInfoHead->biCompression != 0)
    {
        return(FillHeight);
    }

    // ��λͼ
    switch(pBitmapInfoHead->biBitCount)
    {
    case 1:     // ��ɫλͼ
    case 4:     // 16ɫλͼ
    case 8:     // 256ɫλͼ
        iBits = 8 / pBitmapInfoHead->biBitCount;
        bMask = (uint8_t)(0xFF >> (8-pBitmapInfoHead->biBitCount));
        for(i=0; i < pBitmapInfoHead->biHeight; i++) // ɨ��߶�
        {
            // λͼ�����Ǵ��ļ�β��ʼɨ���
            pbBitmapPointer = (uint8_t*)((uint32_t)pvBitmapSrc + pBitmapInfoHead->bfSize - (i+1) *iLineBytes);
            for(k=0, count = 0; k < DataSizePerLine; k++) // ɨ��һ��
            {
                for(n=0; n < iBits && count < pBitmapInfoHead->biWidth; n++, count++)
                {
                    iColor = (pbBitmapPointer[k] >> (8- (n+1)*pBitmapInfoHead->biBitCount)) & bMask;
                    switch(ucType) // λͼ��ת
                    {
                    case EM_BMP_ROTATE90:
                        y =  k*iBits+n;
                        x =  pBitmapInfoHead->biHeight - i;
                        break;
                    case EM_BMP_ROTATE180:
                        x = pBitmapInfoHead->biWidth - (k*iBits+n);
                        y = pBitmapInfoHead->biHeight - i;
                        break;
                    case EM_BMP_ROTATE270:
                        y =  pBitmapInfoHead->biWidth - (k*iBits+n);
                        x =  i;
                        break;
                    default:
                        x = k*iBits+n;
                        y = i;
                        break;
                    }
                    Colors = RGB(pRgbQuad[iColor].rgbRed, pRgbQuad[iColor].rgbGreen, pRgbQuad[iColor].rgbBlue);
                    if (ucTransparent != EM_BMP_TRANSPARENCE) // ��͸������
                    {
                        iRet = Paint(iStartX+x, iStartY+y, Colors);
                        if(iRet == OK)
                        {
                            FillHeight = MAX(FillHeight, y+1);
                        }
                    }
                }
            }
        }
        break;
    case 24:    // 24λ���ɫλͼ
        for(i=0; i < pBitmapInfoHead->biHeight; i++)
        {
            for(n=0; n < pBitmapInfoHead->biWidth; n++)
            {
                switch(ucType)  // λͼ��ת
                {
                case EM_BMP_ROTATE90:
                    y =  n;
                    x =  pBitmapInfoHead->biHeight - i;
                    break;
                case EM_BMP_ROTATE180:
                    x = pBitmapInfoHead->biWidth - n;
                    y = pBitmapInfoHead->biHeight - i;
                    break;
                case EM_BMP_ROTATE270:
                    y =  pBitmapInfoHead->biWidth - n;
                    x =  i;
                    break;
                default:
                    x = n;
                    y = i;
                    break;
                }
                k = n*3;
                Colors = RGB(pbBitmapPointer[k+2], pbBitmapPointer[k+1], pbBitmapPointer[k+0]);
                if(ucTransparent != EM_BMP_TRANSPARENCE)   // ��͸������
                {
                    iRet = Paint(iStartX+x, iStartY+y, Colors);
                    if(iRet == OK)
                    {
                        FillHeight = MAX(FillHeight, y+1);
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    return(FillHeight);
}

// ����������ת����BMP��ʽ�ļ�
int sys_encode2bmp(const void *image, uint width, uint height, void *bmpbuf)
{
    if(image != NULL && bmpbuf != NULL)
    {
        return encode2bmp(image, width, height, black_white_CLT,2,1,(width+7)/8, encodeing1to1,bmpbuf);
    }
    else
    {
        return 0;
    }
}

