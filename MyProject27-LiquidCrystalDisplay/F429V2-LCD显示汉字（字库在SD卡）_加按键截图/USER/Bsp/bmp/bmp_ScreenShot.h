#ifndef __BMP_SCREENSHOT_H
#define __BMP_SCREENSHOT_H

#include "stm32f4xx.h"

/* bmp�ļ�ͷ */
typedef __packed struct
{
  uint16_t bfType;    //�ļ�����
  uint32_t bfSize;    //�ļ���С
  uint16_t bfReserved1;//�����֣�������
  uint16_t bfReserved2;//�����֣�ͬ��
  uint32_t bfOffBits; //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER;

/* bmp��Ϣͷ */
typedef __packed struct
{
  uint32_t biSize;       //ָ���˽ṹ��ĳ��ȣ�Ϊ 40
  long biWidth;          //λͼ��˵����ͼ�Ŀ�ȣ�������Ϊ��λ
  long biHeight;         //λͼ�ߣ�ָ����ͼ�ĸ߶ȣ�����Ϊ��λ
  uint16_t biPlanes;     //ƽ������Ϊ 1
  uint16_t biBitCount;   //������ɫλ���������� 1��2��4��8��16��24 �µĿ����� 32
  uint32_t biCompression;//ѹ����ʽ�������� 0��1��2������ 0 ��ʾ��ѹ��
  uint32_t biSizeImage;  //ʵ��λͼ����ռ�õ��ֽ���
  long biXPelsPerMeter;  //X ����ֱ��ʣ�������/��Ϊ��λ
  long biYPelsPerMeter;  //Y ����ֱ��ʣ�����/��Ϊ��λ
  uint32_t biClrUsed;    //ʹ�õ���ɫ�������Ϊ 0�����ʾĬ��ֵ(2^��ɫλ��)
  uint32_t biClrImportant; //��Ҫ��ɫ�������Ϊ 0�����ʾ������ɫ������Ҫ��
}BITMAPINFOHEADER;

/* ����bmpͷ���� */
typedef __packed struct
{
  BITMAPFILEHEADER bmp_FileHeader;
	BITMAPINFOHEADER bmp_InfoHeader;  
}BITMAPFULLDATA;


uint8_t screen_shot(uint8_t layer);

#endif/*__BMP_SCREENSHOT_H*/
