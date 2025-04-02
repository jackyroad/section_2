/**
  ******************************************************************************
  * @file    bmp_encode.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   LTDC-��Ļ��ͼBMP��ʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include <string.h>

#include "./ff.h"
#include "./Bsp/lcd/bsp_lcd.h"
#include "./Bsp/bmp/bmp_ScreenShot.h"

/* ����bmp��������С���պ�һ֡ */
#define BMP_BUF_SIZE		(800 * 480 * 2)

/* ����bmp��������ָ������SDRAM�У�����ʵ����Ŀ�����޸� */
static uint8_t bmpbuf[BMP_BUF_SIZE] __attribute__ ((at(0xD0400000))) = {0};

/* bmpͼƬ·�� */
static uint8_t BMP_PATH[] = "0:/ScreenShot";

static FIL f_bfile;					/* file objects */
static FRESULT res; 
static UINT br;            	/* File R/W count */


/**
  * @brief ���㺯��
  * @param layer��LTDCͼ��
  * @param x������ĺ�����
  * @param y�������������
  * @retval ��ɫֵ
  */
static uint32_t LTDC_Read_Point(uint8_t layer, uint16_t x, uint16_t y)
{
  if(layer == LCD_FOREGROUND_LAYER)
    return *(uint16_t*)((LCD_FRAME_BUFFER + BUFFER_OFFSET) + 2 * (LCD_PIXEL_WIDTH * y + x));
  else
    return *(uint16_t*)(LCD_FRAME_BUFFER + 2 * (LCD_PIXEL_WIDTH * y + x));
}

/**
  * @brief ͨ��ָ��·�����õ�������·�����ļ������������������
  * @param path��bmp���·��
  * @param pname��������·�����ļ�������"0:/ScreenShot/ScreenShot12345.bmp"
  * @retval none
  */
static void new_pathname(uint8_t *path, uint8_t *pname)
{  
	uint16_t index=0;
	while(index < 0xFFFF)
	{
		sprintf((char*)pname, "%s/ScreenShot%05d.bmp", path, index);
		res = f_open(&f_bfile, (const TCHAR*)pname, FA_READ);/* ���Դ��ļ� */
    if(res == FR_OK)
      f_close(&f_bfile);
    if(res == FR_NO_FILE)
      break;/* �ļ��������ڣ������½��ļ� */
		index++;
	}
}

/**
  * @brief bmp���뺯��
  * @note  ����������ڽ�LTDC�Դ����ݱ���Ϊbmpͼ��
  * @param filename������·��
  * @param layer��LTDCͼ��
  * @param x0��������ʼ����
  * @param y0��������ʼ����
  * @param width��������
  * @param height������߶�
  * @retval  0���ɹ�����0��ʧ�� 
  */
static int8_t bmp_encode(uint8_t *filename, uint8_t layer, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{				
	FIL* f_bmp;
  
  BITMAPFULLDATA hbmp = {0};    /* bmpͷ */
	uint16_t bmpheadsize = 0;     /* bmpͷ���� */
	uint16_t tx, ty = 0;          /* ͼ��ߴ� */
	uint32_t pixcnt = 0;          /* ���ؼ����� */
  
  uint16_t *pbmpbuf = (uint16_t*)bmpbuf;
  
	if(width==0 || height==0)
    return -1;	/* ������� */
	if((x0 + width - 1) > LCD_PIXEL_WIDTH)
    return -1;	/* ������� */
	if((y0 + height - 1) > LCD_PIXEL_HEIGHT)
    return -1;	/* ������� */
  
	f_bmp = &f_bfile;
  
	bmpheadsize = sizeof(hbmp);/* �õ�bmpͷ�Ĵ�С */
	hbmp.bmp_InfoHeader.biSize = sizeof(BITMAPINFOHEADER);/* ��Ϣͷ���� */
	hbmp.bmp_InfoHeader.biWidth = width;	 	/* bmp�Ŀ�� */
	hbmp.bmp_InfoHeader.biHeight = height; 	/* bmp�ĸ߶� */
	hbmp.bmp_InfoHeader.biPlanes = 1;	 		  /* ��Ϊ1 */
	hbmp.bmp_InfoHeader.biBitCount = 16;	 	/* 16bitɫ�� */
	hbmp.bmp_InfoHeader.biCompression = 0;  /* ��ѹ�� */
 	hbmp.bmp_InfoHeader.biSizeImage = hbmp.bmp_InfoHeader.biHeight * hbmp.bmp_InfoHeader.biWidth
                                    * hbmp.bmp_InfoHeader.biBitCount / 8;/* bmp���������� */
 				   
	hbmp.bmp_FileHeader.bfType = ((uint16_t)'M' << 8) + 'B';/* BMP��ʽ��־ */
	hbmp.bmp_FileHeader.bfSize = bmpheadsize + hbmp.bmp_InfoHeader.biSizeImage;/* ����bmp�ĳ��� */
  hbmp.bmp_FileHeader.bfOffBits = bmpheadsize;/* ����������ƫ�� */

  /* �½��ļ� */
 	res = f_open(f_bmp, (const TCHAR*)filename, FA_WRITE|FA_CREATE_NEW);
 
 	if(res == FR_OK)//�����ɹ�
	{
    /* д��bmpͷ */
		res = f_write(f_bmp, (uint8_t*)&hbmp, bmpheadsize, &br);
    /* ����bmpˢ��˳������ͼ�����ݣ������ң����µ��ϣ�
       �����������ǰ��LTDC�Դ治�����κ��޸� */
		for(ty = y0 + height - 1; hbmp.bmp_InfoHeader.biHeight; ty--)
		{
 			for(tx = x0; tx < hbmp.bmp_InfoHeader.biWidth; tx++)
			{
        pbmpbuf[pixcnt] = LTDC_Read_Point(layer, tx, ty);/* ��ȡ���������� */
				pixcnt++;
			}
			hbmp.bmp_InfoHeader.biHeight--;
		}
    res = f_write(f_bmp, (uint8_t*)pbmpbuf, hbmp.bmp_InfoHeader.biSizeImage, &br);//д������
		f_close(f_bmp);
	}

	return res;
}

/**
  * @brief ��ͼ����
  * @param layer����Ҫ��ȡ��LTDCͼ��
  * @retval 0����ͼ�ɹ�����0����ͼʧ��
  * @note �˺���ֻ֧��RGB565��ѹ��BMP��ʽ��ͼ
  */
uint8_t screen_shot(uint8_t layer)
{
  uint8_t pname[40] = {0};
  uint8_t res = 0;
  
  /* ����������·�����ļ��� */
  new_pathname(BMP_PATH, pname);
  
  /* ��ȡ�Դ����ݲ�����Ϊbmp��ʽ */
  res = bmp_encode(pname, layer, 0, 0, 800, 480);

  return res;
}
