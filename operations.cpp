#include "bmp.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
/*#define a 20
#define b 120
#define a1 77
#define b1 128*/

BMP_Image* LinearTrans(BMP_Image* Image_In)
{
    BMP_Image* Image_LinerTrans;
      Image_LinerTrans = (BMP_Image*)malloc(sizeof(BMP_Image));
    int i;
    int a=20,b=120,a1=77,b1=128;

    memcpy(Image_LinerTrans,Image_In,sizeof(BMP_Image));
    //��ѭ�����ڶ�ͼ��ÿһ���������Ա任����
    for (i=0; i<Image_LinerTrans->height*Image_LinerTrans->width; i++)
    {
        if(Image_In->imageData[i]<=a)
            Image_LinerTrans->imageData[i] = 2;
        else if(Image_In->imageData[i]>=b)
            Image_LinerTrans->imageData[i] = 255;
        else
        /********** Begin *********/
            Image_LinerTrans->imageData[i] = (int) (255*1.0/(b-a)*Image_In->imageData[i] - 255*a/(b-a));
        /********** End *********/
       //��ɻҶ�ͼ���Ա任���������д��ע��ֶκ����߽������Ĵ�����
    }
    return Image_LinerTrans;
}

BMP_Image* InvertColor(BMP_Image* Image_In)
{
    BMP_Image* Image_InvertColor;
    Image_InvertColor = (BMP_Image*)malloc(sizeof(BMP_Image));
    int i;
    memcpy(Image_InvertColor,Image_In,sizeof(BMP_Image));
//��ѭ�����ڶ�ͼ��ÿһ�����ط�ɫ����
    for (i=0; i<Image_InvertColor->height*Image_InvertColor->width; i++)
    {
        /********** Begin *********/
        Image_InvertColor->imageData[i] = 0xffff - Image_In->imageData[i];
        /********** End *********/
        //��ɻҶ�ͼ��ɫ���������д
    }
    return Image_InvertColor;
}

BMP_Image* HistNormolize(BMP_Image* Image_In)
{
     int    NumPixel [256] = {0};    //ͳ��ԭͼ����Ҷ���Ŀ����256���Ҷȼ�
     float  ProbPixel [256] = {0};   //ͳ��ԭͼ����Ҷȸ���
     float  AccuProbPixel [256] = {0}; //�ۼƻҶȸ���
     int    AccuPixel [256] = {0};    //����
     int    HistNumPixel [256] = {0};    //ͳ��ֱ��ͼ���⻯��ͼ����Ҷ���Ŀ����256���Ҷȼ�
      BMP_Image* Image_HistNormolize;
      Image_HistNormolize = (BMP_Image*)malloc(sizeof(BMP_Image));
      memcpy(Image_HistNormolize,Image_In,sizeof(BMP_Image));
     int size = Image_In->height * Image_In->width;
     int i;
    //�������ػҶ�ͳ��
     for (i=0; i<size; i++)
     {
        /********** Begin *********/
         NumPixel[Image_In->imageData[i]]++;//�������ͼ������ֵͳ�ƴ���
        /********** End *********/
        //�������ͼ������ֵͳ�ƴ���
     }
     //����Ҷȷֲ��ܶ�
     for (i=0; i<256; i++)
     {
        /********** Begin *********/
         ProbPixel[i] = (float)NumPixel[i]/(float)size;//�������ͼ������ֵ���ʴ��� ��ע��������ת���ɸ�����
        /********** End *********/
       //�������ͼ������ֵ���ʴ��� ��ע��������ת���ɸ�����
     }
     // �����ۼ�ֱ��ͼ�ֲ�
     AccuProbPixel[0] = ProbPixel[0];
     for ( i = 1; i< 256; i++)
     {
        /********** Begin *********/
         AccuProbPixel[i] = AccuProbPixel[i-1] + ProbPixel[i];//�������ͼ���ۻ�ֱ��ͼ����
        /********** End *********/
        //�������ͼ���ۻ�ֱ��ͼ����
     }
     //�ۼƷֲ�ȡ���������������ĻҶ�ӳ���ϵ
     for (i = 0; i< 256; i++)
     {
        /********** Begin *********/
         AccuPixel[i] = (int)(AccuProbPixel[i]*255);//�������ͼ���ۻ�ֱ��ͼȡ�������д��ע�⸡����ת��Ϊ������
        /********** End *********/
         //�������ͼ���ۻ�ֱ��ͼȡ�������д��ע�⸡����ת��Ϊ������
     }
    //���лҶ�ӳ����⻯
     for (i=0; i<size; i++)
     {
        /********** Begin *********/
         Image_HistNormolize->imageData[i] = AccuPixel[Image_In->imageData[i]];//ͨ�����⻯���ԭͼ������ֵ����ӳ��
        /********** End *********/
        //ͨ�����⻯���ԭͼ������ֵ����ӳ��
     }
    return Image_HistNormolize;
}

/*image sharpening*/
BMP_Image* Roberts(BMP_Image* Image_In)
{
    BMP_Image* Roberts;
      Roberts = (BMP_Image*)malloc(sizeof(BMP_Image));
    Roberts->width = Image_In->width -1;
    Roberts->height = Image_In->height -1;
    Roberts->biBitCount = 8;
    Roberts->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Roberts->imageData =     (unsigned char*)malloc((Image_In->height-1)*(Image_In->width-1));
    int i;
    for(i=0;i<256;i++)
    {
     Roberts->imageRgbQuad[i].rgbBlue =     Image_In->imageRgbQuad[i].rgbBlue;
     Roberts->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Roberts->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Roberts->imageRgbQuad[i].rgbReserved = 0;
     Roberts->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Roberts->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Roberts->imageRgbQuad[i].rgbRed =         Image_In->imageRgbQuad[i].rgbRed;
     Roberts->imageRgbQuad[i].rgbReserved = 0;
     Roberts->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Roberts->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Roberts->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Roberts->imageRgbQuad[i].rgbReserved = 0;
    }
    int j,index1,index2,index3,k=0,tmp;
    for (i=0; i<Image_In->height-1; i++)
    {
      for (j=0; j<Image_In->width-1; j++)
        {
          /********* Begin *********/
                index1=i*Image_In->width+j;
                index2=(i+1)*Image_In->width+j;
                int d1 = Image_In->imageData[index2+1];
                int d2 = Image_In->imageData[index1];
                int d3 = Image_In->imageData[index2];
                int d4 = Image_In->imageData[index1+1];
                tmp = abs(Image_In->imageData[index2+1] - Image_In->imageData[index1]) + abs(Image_In->imageData[index2] - Image_In->imageData[index1+1]);
                Roberts->imageData[k] = tmp;
                k++;
          /********* End *********/
      }
    }
    return Roberts;
}

BMP_Image* Sobel(BMP_Image* Image_In)
{
    BMP_Image* Image_Sobel;
      Image_Sobel = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_Sobel->width = Image_In->width;
    Image_Sobel->height = Image_In->height;
    Image_Sobel->biBitCount = 8;
    Image_Sobel->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_Sobel->imageData =     (unsigned char*)malloc((Image_In->height-2)*(Image_In->width-2));
    int i;
    for(i=0;i<256;i++)
    {
     Image_Sobel->imageRgbQuad[i].rgbBlue =     Image_In->imageRgbQuad[i].rgbBlue;
     Image_Sobel->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Sobel->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Image_Sobel->imageRgbQuad[i].rgbReserved = 0;
     Image_Sobel->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Sobel->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Sobel->imageRgbQuad[i].rgbRed =         Image_In->imageRgbQuad[i].rgbRed;
     Image_Sobel->imageRgbQuad[i].rgbReserved = 0;
     Image_Sobel->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Sobel->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Sobel->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Image_Sobel->imageRgbQuad[i].rgbReserved = 0;
    }
    //printf("Sobel�񻯿�ʼ\n");
    int j,index1,index2,index3,k=0;
    for (i=1; i<Image_In->height-1; i++)
    {
      for (j=1; j<Image_In->width-1; j++)
        {
          /********* Begin *********/
                index1=(i-1)*Image_In->width+j;
                index2=i*Image_In->width+j;
                index3=(i+1)*Image_In->width+j;
                int tmp1 = abs(-Image_In->imageData[index1-1] - 2*Image_In->imageData[index1] - Image_In->imageData[index1+1] +
Image_In->imageData[index3-1] +  2*Image_In->imageData[index3] + Image_In->imageData[index3+1]);
                int tmp2 = abs(-Image_In->imageData[index1-1] - 2*Image_In->imageData[index2-1] - Image_In->imageData[index3-1] +
Image_In->imageData[index1+1] + 2*Image_In->imageData[index2+1] + Image_In->imageData[index3+1]);
Image_Sobel->imageData[k] = tmp1+tmp2;
                k++;
          /********* End *********/
      }
    }
    return Image_Sobel;
}

BMP_Image* Prewitt(BMP_Image* Image_In)
{
    BMP_Image* Image_Prewitt;
      Image_Prewitt = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_Prewitt->width = Image_In->width;
    Image_Prewitt->height = Image_In->height;
    Image_Prewitt->biBitCount = 8;
    Image_Prewitt->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_Prewitt->imageData =     (unsigned char*)malloc((Image_In->height-2)*(Image_In->width-2));
    int i;
    for(i=0;i<256;i++)
    {
     Image_Prewitt->imageRgbQuad[i].rgbBlue =     Image_In->imageRgbQuad[i].rgbBlue;
     Image_Prewitt->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Prewitt->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Image_Prewitt->imageRgbQuad[i].rgbReserved = 0;
     Image_Prewitt->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Prewitt->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Prewitt->imageRgbQuad[i].rgbRed =         Image_In->imageRgbQuad[i].rgbRed;
     Image_Prewitt->imageRgbQuad[i].rgbReserved = 0;
     Image_Prewitt->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Prewitt->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Prewitt->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Image_Prewitt->imageRgbQuad[i].rgbReserved = 0;
    }
    //printf("Prewitt�񻯿�ʼ\n");
    int j,index1,index2,index3,k=0;
    for (i=1; i<Image_In->height-1; i++)
    {
      for (j=1; j<Image_In->width-1; j++)
        {
          /********* Begin *********/
                index1=(i-1)*Image_In->width+j;
                index2=i*Image_In->width+j;
                index3=(i+1)*Image_In->width+j;
                int tmp1 = abs(-Image_In->imageData[index1-1] - Image_In->imageData[index1] - Image_In->imageData[index1+1] +
Image_In->imageData[index3-1] + Image_In->imageData[index3] + Image_In->imageData[index3+1]);
                int tmp2 = abs(-Image_In->imageData[index1-1] - Image_In->imageData[index2-1] - Image_In->imageData[index3-1] +
Image_In->imageData[index1+1] + Image_In->imageData[index2+1] + Image_In->imageData[index3+1]);
                Image_Prewitt->imageData[k] = tmp1+tmp2;
                k++;
          /********* End *********/
      }
    }
    return Image_Prewitt;
}

BMP_Image* Laplacian(BMP_Image* Image_In)
{
    BMP_Image* Image_Laplacian;
      Image_Laplacian = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_Laplacian->width = Image_In->width;
    Image_Laplacian->height = Image_In->height;
    Image_Laplacian->biBitCount = 8;
    Image_Laplacian->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_Laplacian->imageData =     (unsigned char*)malloc((Image_In->height-2)*(Image_In->width-2));
    int i;
    for(i=0;i<256;i++)
    {
     Image_Laplacian->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Laplacian->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Laplacian->imageRgbQuad[i].rgbRed =          Image_In->imageRgbQuad[i].rgbRed;
     Image_Laplacian->imageRgbQuad[i].rgbReserved = 0;
     Image_Laplacian->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Laplacian->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Laplacian->imageRgbQuad[i].rgbRed =         Image_In->imageRgbQuad[i].rgbRed;
     Image_Laplacian->imageRgbQuad[i].rgbReserved = 0;
     Image_Laplacian->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_Laplacian->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_Laplacian->imageRgbQuad[i].rgbRed =          Image_In->imageRgbQuad[i].rgbRed;
     Image_Laplacian->imageRgbQuad[i].rgbReserved = 0;
    }
    //printf("Laplacian�񻯿�ʼ\n");
    int j,index1,index2,index3,k=0,tmp;
    for (i=1; i<Image_In->height-1; i++)
    {
      for (j=1; j<Image_In->width-1; j++)
        {
          /********* Begin *********/
                index1=(i-1)*Image_In->width+j;
                index2=i*Image_In->width+j;
                index3=(i+1)*Image_In->width+j;
                tmp =(-Image_In->imageData[index1] - Image_In->imageData[index2-1] + 4*Image_In->imageData[index2] - Image_In->imageData[index2+1] - Image_In->imageData[index3]);
                if(tmp<0) tmp = 0;
                else if(tmp>=255) tmp = 255;
                Image_Laplacian->imageData[k] = tmp ;
                k++;
          /********* End *********/
      }
    }
    return Image_Laplacian;
}

/*image smoothing*/
BMP_Image* MeanFilter(BMP_Image* Image_In)
{
    BMP_Image* Image_MeanFilter;
      Image_MeanFilter = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_MeanFilter->width = Image_In->width;
    Image_MeanFilter->height = Image_In->height;
    Image_MeanFilter->biBitCount = 8;
    Image_MeanFilter->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_MeanFilter->imageData =     (unsigned char*)malloc((Image_In->height-2)*(Image_In->width-2));
    int i;
    for(i=0;i<256;i++)
    {
     Image_MeanFilter->imageRgbQuad[i].rgbBlue =     Image_In->imageRgbQuad[i].rgbBlue;
     Image_MeanFilter->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_MeanFilter->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Image_MeanFilter->imageRgbQuad[i].rgbReserved = 0;
     Image_MeanFilter->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_MeanFilter->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_MeanFilter->imageRgbQuad[i].rgbRed =         Image_In->imageRgbQuad[i].rgbRed;
     Image_MeanFilter->imageRgbQuad[i].rgbReserved = 0;
     Image_MeanFilter->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_MeanFilter->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_MeanFilter->imageRgbQuad[i].rgbRed =      Image_In->imageRgbQuad[i].rgbRed;
     Image_MeanFilter->imageRgbQuad[i].rgbReserved = 0;
    }
//�˲�����ʱ�����Ǳ�Ե��3*3��ֵ�˲��Ļ���ʹͼ�����2��2��
//���˲�ʱӦ�ӵڶ��еڶ��п�ʼ�������ڶ��еڶ��н���
    int j,index1,index2,index3,k=0,tmp;
    for (i=1; i<Image_In->height-1; i++)
    {
      for (j=1; j<Image_In->width-1; j++)
        {
          /********* Begin *********/
                index1 = (i-1)*Image_In->width+j;
                index2 = i*Image_In->width+j;
                index3 = (i+1)*Image_In->width+j;
                tmp = (Image_In->imageData[index1-1] + Image_In->imageData[index1] + Image_In->imageData[index1+1] + Image_In->imageData[index2-1] + Image_In->imageData[index2]  +  Image_In->imageData[index2+1]  +
                Image_In->imageData[index3-1] + Image_In->imageData[index3] + Image_In->imageData[index3+1])/9;
                Image_MeanFilter->imageData[k] = tmp;
                k++;
          /********* End *********/
      }
    }
    return Image_MeanFilter;
}

int Mid(int a,int b,int c)
{
int t;
if(a > b) {t = a; a=b; b=t;}
if(a > c) {t = a; a=c; c=t;}
if(b > c) {t = b; b=c; c=t;}
return b;
}

BMP_Image* MidFilter(BMP_Image* Image_In)
{
    BMP_Image* Image_MedFilter;
      Image_MedFilter = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_MedFilter->width = Image_In->width;
    Image_MedFilter->height = Image_In->height;
    Image_MedFilter->biBitCount = 8;
    Image_MedFilter->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_MedFilter->imageData =     (unsigned char*)malloc((Image_In->height-2)*(Image_In->width-2));
    int i;
    for(i=0;i<256;i++)
    {
     Image_MedFilter->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_MedFilter->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_MedFilter->imageRgbQuad[i].rgbRed =          Image_In->imageRgbQuad[i].rgbRed;
     Image_MedFilter->imageRgbQuad[i].rgbReserved =  0;
     Image_MedFilter->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_MedFilter->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_MedFilter->imageRgbQuad[i].rgbRed =         Image_In->imageRgbQuad[i].rgbRed;
     Image_MedFilter->imageRgbQuad[i].rgbReserved =  0;
     Image_MedFilter->imageRgbQuad[i].rgbBlue =      Image_In->imageRgbQuad[i].rgbBlue;
     Image_MedFilter->imageRgbQuad[i].rgbGreen =      Image_In->imageRgbQuad[i].rgbGreen;
     Image_MedFilter->imageRgbQuad[i].rgbRed =          Image_In->imageRgbQuad[i].rgbRed;
     Image_MedFilter->imageRgbQuad[i].rgbReserved =  0;
    }
//�˲�����ʱ�����Ǳ�Ե��3*3��ֵ�˲��Ļ���ʹͼ�����2��2��
//���˲�ʱӦ�ӵڶ��еڶ��п�ʼ�������ڶ��еڶ��н���
    int j,index1,index2,index3,k=0;
    for (i=1; i<Image_In->height-1; i++)
    {
      for (j=1; j<Image_In->width-1; j++)
        {
          /********* Begin *********/
                index1=(i-1)*Image_In->width+j;
                index2=i*Image_In->width+j;
                index3=(i+1)*Image_In->width+j;
                int Med1 = Mid(Image_In->imageData[index1-1],Image_In->imageData[index1],Image_In->imageData[index1+1]);
                int Med2 = Mid(Image_In->imageData[index2-1],Image_In->imageData[index2],Image_In->imageData[index2+1]);
                int Med3 = Mid(Image_In->imageData[index3-1],Image_In->imageData[index3],Image_In->imageData[index3+1]);
                int Med4 = Mid(Med1,Med2,Med3);
                Image_MedFilter->imageData[k] = Med4;
                k++;
          /********* End *********/
      }
    }
    return Image_MedFilter;
}

/*color model transformation*/
BMP_Image* RGB2Gray(BMP_Image* Image_In)
{
    BMP_Image* Image_Gray;
    Image_Gray = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_Gray->width = Image_In->width ;
    Image_Gray->height = Image_In->height;
    Image_Gray->biBitCount = 8;
    Image_Gray->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_Gray->imageData =     (unsigned char*)malloc((Image_In->height)*(Image_In->width));
    int i;
    for(i=0;i<256;i++)
    {
        Image_Gray->imageRgbQuad[i].rgbBlue = i;
        Image_Gray->imageRgbQuad[i].rgbGreen = i;
        Image_Gray->imageRgbQuad[i].rgbRed = i;
        Image_Gray->imageRgbQuad[i].rgbReserved = 0;
    }
    cout<<Image_In->height<<endl;
    cout<<Image_In->width<<endl;
    for (i=0; i<Image_In->height*Image_In->width*3; i=i+3)
    {
        int img_b = Image_In->imageData[i];
        int img_g = Image_In->imageData[i+1];
        int img_r = Image_In->imageData[i+2];
        Image_Gray->imageData[i/3]= (img_r*38 + img_g*75 + img_b*15) >> 7; 
    }
    return Image_Gray;
}

HSI* RGB2HSI(BMP_Image* Image_In)
{
    HSI* BMP_HSI;
//��RGB2HSI    ��H��S��I���������ڴ沢��ֵ
      BMP_HSI = (HSI*)malloc(sizeof(HSI));
      BMP_HSI->HSI_H = (BMP_Image*)malloc(sizeof(BMP_Image));
      BMP_HSI->HSI_S = (BMP_Image*)malloc(sizeof(BMP_Image));
      BMP_HSI->HSI_I = (BMP_Image*)malloc(sizeof(BMP_Image));
    BMP_HSI->HSI_H->width = Image_In->width;
    BMP_HSI->HSI_H->height = Image_In->height;
    BMP_HSI->HSI_H->biBitCount = 8;
    BMP_HSI->HSI_H->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    BMP_HSI->HSI_H->imageData = (unsigned char*)malloc(Image_In->height*Image_In->width);
    BMP_HSI->HSI_S->width = Image_In->width;
    BMP_HSI->HSI_S->height = Image_In->height;
    BMP_HSI->HSI_S->biBitCount = 8;
    BMP_HSI->HSI_S->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    BMP_HSI->HSI_S->imageData = (unsigned char*)malloc(Image_In->height*Image_In->width);
    BMP_HSI->HSI_I->width = Image_In->width;
    BMP_HSI->HSI_I->height = Image_In->height;
    BMP_HSI->HSI_I->biBitCount = 8;
    BMP_HSI->HSI_I->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    BMP_HSI->HSI_I->imageData = (unsigned char*)malloc(Image_In->height*Image_In->width);
//��RGB2HSI    ��H��S��I������ŵ���8λ�Ҷ�ֵ����˶Ե�ɫ�帳ֵ
    int i;
    for(i=0;i<256;i++)
    {
     BMP_HSI->HSI_H->imageRgbQuad[i].rgbBlue = i;
     BMP_HSI->HSI_H->imageRgbQuad[i].rgbGreen = i;
     BMP_HSI->HSI_H->imageRgbQuad[i].rgbRed = i;
     BMP_HSI->HSI_H->imageRgbQuad[i].rgbReserved = 0;
     BMP_HSI->HSI_S->imageRgbQuad[i].rgbBlue = i;
     BMP_HSI->HSI_S->imageRgbQuad[i].rgbGreen = i;
     BMP_HSI->HSI_S->imageRgbQuad[i].rgbRed = i;
     BMP_HSI->HSI_S->imageRgbQuad[i].rgbReserved = 0;
     BMP_HSI->HSI_I->imageRgbQuad[i].rgbBlue = i;
     BMP_HSI->HSI_I->imageRgbQuad[i].rgbGreen = i;
     BMP_HSI->HSI_I->imageRgbQuad[i].rgbRed = i;
     BMP_HSI->HSI_I->imageRgbQuad[i].rgbReserved = 0;
    }
    for (i=0; i<Image_In->height*Image_In->width*3; i=i+3)
    {
    int img_b = Image_In->imageData[i];
    int img_g = Image_In->imageData[i+1];
    int img_r = Image_In->imageData[i+2];
    // ����Hue����
    float numerator = (float)(img_r - img_g + img_r - img_b ) / 2;
    double denominator = sqrt((img_r - img_g)*(img_r - img_g) + (img_r - img_b)*(img_g - img_b));
    double Hue;
      if(denominator != 0)
      {
         double theta = acos( numerator/denominator) * 180/3.1415926;         // ����theta�ǣ���һ����0~360֮��
         /********* Begin *********/
        // ����Hue����
        if(img_b <= img_g)
                Hue = theta ;
        else
            Hue = 360 - theta;
         /********* End *********/
      }
      else
               Hue = 0;
    // ����Saturation����
    int fTemp = img_r < img_g ? img_r : img_g;
    int min_rgb = fTemp < img_b ? fTemp : img_b;      // �õ�RGB�����е���Сֵ
    /********* Begin *********/
    float Saturation = 1 - (float)(3 * min_rgb)/(img_r + img_g + img_b);
    /********* End *********/
    // ����Intensity����
    /********* Begin *********/
    int Intensity = (int)((img_b + img_g + img_r)/3); 
    /********* End *********/
//��һ�����Ҷ�ͼ������ʾ
    BMP_HSI->HSI_H->imageData[i/3] = (int)(Hue*255/360);
    BMP_HSI->HSI_S->imageData[i/3] = (int)(Saturation*255);
    BMP_HSI->HSI_I->imageData[i/3] = Intensity;
    }
    return BMP_HSI;
}

BMP_Image* PseudoColor(BMP_Image* Image_In)
{
    BMP_Image* Image_PseudoColor;
    Image_PseudoColor = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_PseudoColor->width = Image_In->width;
    Image_PseudoColor->height = Image_In->height;
    Image_PseudoColor->biBitCount = 24;
    Image_PseudoColor->imageData =     (unsigned char*)malloc(Image_In->height*Image_In->width*3);
    int i;
    for (i=0; i<Image_In->height*Image_In->width; i++)
    {
        int Image_In_Pixel = Image_In->imageData[i];
        int Image_PseudoColor_b ;
        int Image_PseudoColor_g ;
        int Image_PseudoColor_r ;
        Image_PseudoColor->imageData[i*3] = Image_PseudoColor_b;
        Image_PseudoColor->imageData[i*3+1] = Image_PseudoColor_g;
        Image_PseudoColor->imageData[i*3+2] = Image_PseudoColor_r;
   //***********��bgrͨ����ֵ***********//
    //��Bͨ������
    if(Image_In_Pixel < 64)
        /********* Begin *********/
        Image_PseudoColor_b = 255;
        /********* End *********/
    else if(Image_In_Pixel< 128)
        /********* Begin *********/
        Image_PseudoColor_b = -255/64 * (Image_In_Pixel - 64) + 255 ;
        /********* End *********/
    else
        /********* Begin *********/
        Image_PseudoColor_b = 0;
        /********* End *********/
    //��Gͨ������
    if(Image_In_Pixel < 64)
        /********* Begin *********/
        Image_PseudoColor_g = 255/64 * Image_In_Pixel;
        /********* End *********/
    else if(Image_In_Pixel < 192)
        /********* Begin *********/
        Image_PseudoColor_g = 255 ; 
        /********* End *********/
    else
        /********* Begin *********/
        Image_PseudoColor_g = -255/64 * (Image_In_Pixel -192) + 255 ; 
        /********* End *********/
    //��Rͨ������
    if(Image_In_Pixel < 128)
        /********* Begin *********/
    Image_PseudoColor_r = 0;
        /********* End *********/
    else if(Image_In_Pixel < 192)
        /********* Begin *********/
    Image_PseudoColor_r = 255/64 * (Image_In_Pixel -128); 
        /********* End *********/
    else
        /********* Begin *********/
    Image_PseudoColor_r = 255 ;
        /********* End *********/
    }
    return Image_PseudoColor;
}

BMP_Image* FalseColor(BMP_Image* Image_In)
{
    BMP_Image* Image_FalseColor;
      Image_FalseColor = (BMP_Image*)malloc(sizeof(BMP_Image));
    memcpy(Image_FalseColor,Image_In,sizeof(BMP_Image));
    int i;
    for (i=0; i<Image_FalseColor->height*Image_FalseColor->width*3; i=i+3)
    {
     //ʵ�ּٲ�ɫ��ǿ
        int Image_In_b = Image_In->imageData[i];
        int Image_In_g = Image_In->imageData[i+1];
        int Image_In_r = Image_In->imageData[i+2];
        int Image_FalseColor_b ;
        int Image_FalseColor_g ;
        int Image_FalseColor_r ;
        //***********��bgrͨ����ֵ***********//
        /********* Begin *********/
            Image_FalseColor_b = Image_In_g;       //��Bͨ������
            Image_FalseColor_g = Image_In_r;       //��Gͨ������
            Image_FalseColor_r = Image_In_b;       //��Rͨ������
        /********* End *********/
        Image_FalseColor->imageData[i] = Image_FalseColor_b;
        Image_FalseColor->imageData[i+1] = Image_FalseColor_g;
        Image_FalseColor->imageData[i+2] = Image_FalseColor_r;
        
    }
    return Image_FalseColor;
}

/*image algebra*/
BMP_Image* Sub(BMP_Image* Image_In1, BMP_Image* Image_In2)
{
	if(Image_In1->width != Image_In2->width || Image_In1->height != Image_In2->height)
		exit(0);    

    BMP_Image* Image_Gray;
    Image_Gray = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_Gray->width = Image_In1->width;
    Image_Gray->height = Image_In1->height;
    Image_Gray->biBitCount = 8;
    Image_Gray->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_Gray->imageData =     (unsigned char*)malloc((Image_In1->height)*(Image_In1->width));
    int i;
    for(i=0;i<256;i++)
    {
        Image_Gray->imageRgbQuad[i].rgbBlue = i;
        Image_Gray->imageRgbQuad[i].rgbGreen = i;
        Image_Gray->imageRgbQuad[i].rgbRed = i;
        Image_Gray->imageRgbQuad[i].rgbReserved = 0;
    }
    for (i=0; i<Image_In1->height; i=i+1)
    {
        for(int j = 0; j< Image_In1->width;j++)
            Image_Gray->imageData[i*Image_In1->width+j]= abs((int)Image_In1->imageData[i*Image_In1->width+j]-(int)Image_In2->imageData[i*Image_In1->width+j]); 
    }
    return Image_Gray;    
}

BMP_Image* eSub(BMP_Image* Image_SubIn)
{
    int A=0, B=0;
    double C;
    BMP_Image* Image_Gray;
    Image_Gray = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_Gray->width = Image_SubIn->width;
    Image_Gray->height = Image_SubIn->height;
    Image_Gray->biBitCount = 8;
    Image_Gray->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    Image_Gray->imageData =     (unsigned char*)malloc((Image_SubIn->height)*(Image_SubIn->width));
    int i;
    for(i=0;i<256;i++)
    {
        Image_Gray->imageRgbQuad[i].rgbBlue = i;
        Image_Gray->imageRgbQuad[i].rgbGreen = i;
        Image_Gray->imageRgbQuad[i].rgbRed = i;
        Image_Gray->imageRgbQuad[i].rgbReserved = 0;
    }
    for (i=0; i<Image_SubIn->height*Image_SubIn->width; i=i+1)
    {
        A++;//total gray value
        if(Image_SubIn->imageData[i]<30)
            B++;
    }
    C = (double)B/A;
    if (C>0.92) 
    {
        for (i=0; i<Image_SubIn->height*Image_SubIn->width; i=i+1)
        {
            Image_Gray->imageData[i] = Image_SubIn->imageData[i]*3;
        }       
    }
    return Image_Gray;    
}

/*segmentation*/
unsigned int OTSU(BMP_Image* Image_In)
{
    unsigned long height = Image_In->height;
    unsigned long width = Image_In->width;
    
    int PixelCount[256]={0};
    float PixelPro[256]={0};
    int total_Pixel = width * height;
    float threshold = 0;
    //ͳ�ƻҶȼ���ÿ������������ͼ�еĸ���
     for (int i=0; i<total_Pixel; i++)
     {
        /********** Begin *********/
         PixelCount[Image_In->imageData[i]]++;//�������ͼ������ֵͳ�ƴ���
        /********** End *********/
        //�������ͼ������ֵͳ�ƴ���
     }
    
    //����ÿ������������ͼ�еĸ���
    for(int i = 0; i < 256; i++){
        PixelPro[i] = (float)(PixelCount[i]) / (float)(total_Pixel);
    }
    //�����Ҷȼ�[0,255]��������������ĻҶ�ֵ��Ϊ�����ֵ
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for(int i = 0; i < 256; i++){
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for(int j = 0; j < 256; j++){
            if(j <= i){//��iΪ��ֵ���࣬��һ���ܵĸ���
                w0 += PixelPro[j];
                u0tmp += j * PixelPro[j];
            }else{//ǰ������
                w1 += PixelPro[j];
                u1tmp += j * PixelPro[j];
            }
        }
        u0 = u0tmp / w0; //��һ���ƽ���Ҷ�
        u1 = u1tmp / w1; //�ڶ����ƽ���Ҷ�
        u = u0 + u1; //����ͼ���ƽ���Ҷ�
        //������䷽��
        deltaTmp = w0 * (u0 - u) * (u0 - u) + w1 * (u1 - u) * (u1 - u);
        //�ҳ������䷽���Լ���Ӧ��ֵ
        if(deltaTmp > deltaMax){
            deltaMax = deltaTmp;
            threshold = i;
        }
    }
    return (int)threshold;
}

BMP_Image* GlobalSegmentation(BMP_Image* Image_In)
{

    BMP_Image* Image_GlobalSegmentation;
  	Image_GlobalSegmentation = (BMP_Image*)malloc(sizeof(BMP_Image));


    Image_GlobalSegmentation->width = Image_In->width;
    Image_GlobalSegmentation->height = Image_In->height;
	Image_GlobalSegmentation->biBitCount = 1;
	Image_GlobalSegmentation->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
	Image_GlobalSegmentation->imageData = 	(unsigned char*)malloc(Image_In->height*Image_In->width/8);

	 Image_GlobalSegmentation->imageRgbQuad[0].rgbBlue =     0;
	 Image_GlobalSegmentation->imageRgbQuad[0].rgbGreen = 	 0;
	 Image_GlobalSegmentation->imageRgbQuad[0].rgbRed = 	 0;
	 Image_GlobalSegmentation->imageRgbQuad[0].rgbReserved = 0;

	 Image_GlobalSegmentation->imageRgbQuad[1].rgbBlue = 	 255;
	 Image_GlobalSegmentation->imageRgbQuad[1].rgbGreen = 	 255;
	 Image_GlobalSegmentation->imageRgbQuad[1].rgbRed =	     255;
	 Image_GlobalSegmentation->imageRgbQuad[1].rgbReserved = 0;

	int i,tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;

    //int Th=135;

    unsigned int Th = OTSU(Image_In);

    for (i=0; i<Image_In->height*Image_In->width; i=i+8)
    {
		/********* Begin *********/
        int k = i/8;
        Image_GlobalSegmentation->imageData[k] = 0;
        for (int j = 0; j <= 7; j++)
        {
            if (Image_In->imageData[i+j] > Th)
                Image_GlobalSegmentation->imageData[k]               
                |=(128>>j);
        }

		/********* End *********/
	}
    return Image_GlobalSegmentation;
}

BMP_Image* LocalSegmentation(BMP_Image* Image_In)
{
    BMP_Image* Image_LocalSegmentation;
      Image_LocalSegmentation = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_LocalSegmentation->width = Image_In->width;
    Image_LocalSegmentation->height = Image_In->height;
    Image_LocalSegmentation->biBitCount = 1;
    Image_LocalSegmentation->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
    Image_LocalSegmentation->imageData =     (unsigned char*)malloc(Image_In->height*Image_In->width/8);
     Image_LocalSegmentation->imageRgbQuad[0].rgbBlue =     0;
     Image_LocalSegmentation->imageRgbQuad[0].rgbGreen =      0;
     Image_LocalSegmentation->imageRgbQuad[0].rgbRed =      0;
     Image_LocalSegmentation->imageRgbQuad[0].rgbReserved = 0;
     Image_LocalSegmentation->imageRgbQuad[1].rgbBlue =      255;
     Image_LocalSegmentation->imageRgbQuad[1].rgbGreen =      255;
     Image_LocalSegmentation->imageRgbQuad[1].rgbRed =         255;
     Image_LocalSegmentation->imageRgbQuad[1].rgbReserved = 0;
    int i,j,tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;
    int Th1=135;
    int Th2=195;
    int Th3=55;
    int Th4=95;
    for (i=0; i<Image_In->height/2; i++)
        for (j=0; j<Image_In->width/2; j=j+8)
    {
            /********* Begin *********/
            //�����Ͻǵ�ͼƬ���д���
            
        if(Image_In->imageData[i*Image_In->width+j] > Th1)    tmp1 = 1; else tmp1 = 0;
        if(Image_In->imageData[i*Image_In->width+j+1] > Th1)  tmp2 = 1; else tmp2 = 0;
        if(Image_In->imageData[i*Image_In->width+j+2] > Th1)  tmp3 = 1; else tmp3 = 0;
        if(Image_In->imageData[i*Image_In->width+j+3] > Th1)  tmp4 = 1; else tmp4 = 0;
        if(Image_In->imageData[i*Image_In->width+j+4] > Th1)  tmp5 = 1; else tmp5 = 0;
        if(Image_In->imageData[i*Image_In->width+j+5] > Th1)  tmp6 = 1; else tmp6 = 0;
        if(Image_In->imageData[i*Image_In->width+j+6] > Th1)  tmp7 = 1; else tmp7 = 0;
        if(Image_In->imageData[i*Image_In->width+j+7] > Th1)  tmp8 = 1; else tmp8 = 0;
            tmp = tmp1*128 + tmp2*64 + tmp3*32 + tmp4*16 + tmp5*8 + tmp6*4  + tmp7*2  + tmp8*1;
            Image_LocalSegmentation->imageData[(i*Image_In->width+j)/8] = tmp;
            /********* End *********/
    }
    for (i=0; i<Image_In->height/2; i++)
        for (j=Image_In->width/2; j<Image_In->width; j=j+8)
    {
            /********* Begin *********/
            //�����Ͻǵ�ͼƬ���д���
        if(Image_In->imageData[i*Image_In->width+j] > Th2)    tmp1 = 1; else tmp1 = 0;
        if(Image_In->imageData[i*Image_In->width+j+1] > Th2)  tmp2 = 1; else tmp2 = 0;
        if(Image_In->imageData[i*Image_In->width+j+2] > Th2)  tmp3 = 1; else tmp3 = 0;
        if(Image_In->imageData[i*Image_In->width+j+3] > Th2)  tmp4 = 1; else tmp4 = 0;
        if(Image_In->imageData[i*Image_In->width+j+4] > Th2)  tmp5 = 1; else tmp5 = 0;
        if(Image_In->imageData[i*Image_In->width+j+5] > Th2)  tmp6 = 1; else tmp6 = 0;
        if(Image_In->imageData[i*Image_In->width+j+6] > Th2)  tmp7 = 1; else tmp7 = 0;
        if(Image_In->imageData[i*Image_In->width+j+7] > Th2)  tmp8 = 1; else tmp8 = 0;
            tmp = tmp1*128 + tmp2*64 + tmp3*32 + tmp4*16 + tmp5*8 + tmp6*4  + tmp7*2  + tmp8*1;
            Image_LocalSegmentation->imageData[(i*Image_In->width+j)/8] = tmp;
            /********* End *********/
    }
    for (i=Image_In->height/2; i<Image_In->height; i++)
        for (j=0; j<Image_In->width/2; j=j+8)
    {
            /********* Begin *********/
            //�����½ǵ�ͼƬ���д���
        if(Image_In->imageData[i*Image_In->width+j] > Th3)    tmp1 = 1; else tmp1 = 0;
        if(Image_In->imageData[i*Image_In->width+j+1] > Th3)  tmp2 = 1; else tmp2 = 0;
        if(Image_In->imageData[i*Image_In->width+j+2] > Th3)  tmp3 = 1; else tmp3 = 0;
        if(Image_In->imageData[i*Image_In->width+j+3] > Th3)  tmp4 = 1; else tmp4 = 0;
        if(Image_In->imageData[i*Image_In->width+j+4] > Th3)  tmp5 = 1; else tmp5 = 0;
        if(Image_In->imageData[i*Image_In->width+j+5] > Th3)  tmp6 = 1; else tmp6 = 0;
        if(Image_In->imageData[i*Image_In->width+j+6] > Th3)  tmp7 = 1; else tmp7 = 0;
        if(Image_In->imageData[i*Image_In->width+j+7] > Th3)  tmp8 = 1; else tmp8 = 0;
            tmp = tmp1*128 + tmp2*64 + tmp3*32 + tmp4*16 + tmp5*8 + tmp6*4  + tmp7*2  + tmp8*1;
            Image_LocalSegmentation->imageData[(i*Image_In->width+j)/8] = tmp;
            /********* End *********/
    }
    for (i=Image_In->height/2; i<Image_In->height; i++)
        for (j=Image_In->width/2; j<Image_In->width; j=j+8)
    {
            /********* Begin *********/
            //�����½ǵ�ͼƬ���д���
        if(Image_In->imageData[i*Image_In->width+j] > Th4)    tmp1 = 1; else tmp1 = 0;
        if(Image_In->imageData[i*Image_In->width+j+1] > Th4)  tmp2 = 1; else tmp2 = 0;
        if(Image_In->imageData[i*Image_In->width+j+2] > Th4)  tmp3 = 1; else tmp3 = 0;
        if(Image_In->imageData[i*Image_In->width+j+3] > Th4)  tmp4 = 1; else tmp4 = 0;
        if(Image_In->imageData[i*Image_In->width+j+4] > Th4)  tmp5 = 1; else tmp5 = 0;
        if(Image_In->imageData[i*Image_In->width+j+5] > Th4)  tmp6 = 1; else tmp6 = 0;
        if(Image_In->imageData[i*Image_In->width+j+6] > Th4)  tmp7 = 1; else tmp7 = 0;
        if(Image_In->imageData[i*Image_In->width+j+7] > Th4)  tmp8 = 1; else tmp8 = 0;
            tmp = tmp1*128 + tmp2*64 + tmp3*32 + tmp4*16 + tmp5*8 + tmp6*4  + tmp7*2  + tmp8*1;
            Image_LocalSegmentation->imageData[(i*Image_In->width+j)/8] = tmp;
            /********* End *********/
    }
    return Image_LocalSegmentation;
}

/*morphology*/
int** strelDisk() //int Radius
{
    int** circle_struct_elem=NULL;
    ///*int **diskStruct = NULL;
    int center_x = SIZE / 2;
    int center_y = SIZE / 2;
    int radius = center_x;
    circle_struct_elem = (int **)malloc(sizeof(int *) * SIZE);
    for (int i = 0; i < SIZE; i++)
    {
        circle_struct_elem[i] = (int *)malloc(sizeof(int) * SIZE);
    } 
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            circle_struct_elem[i][j] = 0;
            if(pow((i-center_x), 2) + pow((j-center_y), 2) <= pow(radius, 2)) {
                circle_struct_elem[i][j] = 1;
            }
        }
    }
 
    /*for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            printf("%d ", circle_struct_elem[i][j]);
        }
        printf("\n");
    }*/
 
    return circle_struct_elem;
}

int** strelLine()
{
    int** line_struct_elem = NULL;
   line_struct_elem = (int **)malloc(sizeof(int *) * SIZE);
    for (int i = 0; i < SIZE; i++)
    {
        line_struct_elem[i] = (int *)malloc(sizeof(int) * SIZE);
    }  
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            line_struct_elem[i][j] = 0;
            if(i==0 || i==SIZE-1 || j==0 || j==SIZE-1) {
                line_struct_elem[i][j] = 1;
            }
        }
    }
 
    /*for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            printf("%d ", line_struct_elem[i][j]);
        }
        printf("\n");
    }*/
 
    return line_struct_elem;
}

int**  strelRec()
{
    int** rec_struct_elem= NULL;
    rec_struct_elem = (int **)malloc(sizeof(int *) * SIZE);
    for (int i = 0; i < SIZE; i++)
    {
        rec_struct_elem[i] = (int *)malloc(sizeof(int) * SIZE);
    }  
    int center_x = SIZE / 2;
    int center_y = SIZE / 2;
    //int radius = center_x;

    for(int i=0; i<SIZE; i++) {
        int j;
        for(j=0; j<SIZE; j++) {
            rec_struct_elem[i][j] = 0;
            if(j==center_y) {
                rec_struct_elem[i][j] = 1;
            }
            if(i==center_x){
                rec_struct_elem[i][j]=1;
            }
        }
       
    }
 
    /*for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            printf("%d ", circle_struct_elem[i][j]);
        }
        printf("\n");
    }*/
 
    return rec_struct_elem;
}

BMP_Image MorphErosion(BMP_Image src,  int structWidth, int structHeight)
{
    if (src.width - structWidth < 0 && src.height - structHeight < 0) exit(0);
    if (structWidth != structHeight) exit(0);
    
    BMP_Image Image_LocalSegmentation;
    //Image_LocalSegmentation = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_LocalSegmentation.width = src.width;
    Image_LocalSegmentation.height = src.height;
    Image_LocalSegmentation.biBitCount = 1;
    Image_LocalSegmentation.imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
    Image_LocalSegmentation.imageData =     (unsigned char*)malloc(src.height*src.width/8);
     Image_LocalSegmentation.imageRgbQuad[0].rgbBlue =     0;
     Image_LocalSegmentation.imageRgbQuad[0].rgbGreen =      0;
     Image_LocalSegmentation.imageRgbQuad[0].rgbRed =      0;
     Image_LocalSegmentation.imageRgbQuad[0].rgbReserved = 0;
     Image_LocalSegmentation.imageRgbQuad[1].rgbBlue =      255;
     Image_LocalSegmentation.imageRgbQuad[1].rgbGreen =      255;
     Image_LocalSegmentation.imageRgbQuad[1].rgbRed =         255;
     Image_LocalSegmentation.imageRgbQuad[1].rgbReserved = 0;
    
    int mid = (structWidth + 1) / 2 - 1;
    unsigned char val = 255;
    for (int i = mid; i < src.height - mid; i++)
    {
        for (int j = mid; j < src.width - mid; j++)
        {
            for (int m = 0; m < structWidth; m++)
            {
                for (int n = 0; n < structHeight; n++)
                {
                    if (m == mid && n == mid )continue;
                        val &=  src.imageData[(i+m) * src.width + j + n];
                }
            }
 
            Image_LocalSegmentation.imageData[i * src.width + j] = val;
            val = 255;
        }
    }

    return Image_LocalSegmentation;
}

//����
BMP_Image MorphDilation(BMP_Image src,  int structWidth, int structHeight)
{
    if (src.width - structWidth < 0 && src.height - structHeight < 0) exit(0);
    if (structWidth != structHeight) exit(0);
    
    BMP_Image Image_LocalSegmentation;
    //Image_LocalSegmentation = (BMP_Image*)malloc(sizeof(BMP_Image));
    Image_LocalSegmentation.width = src.width;
    Image_LocalSegmentation.height = src.height;
    Image_LocalSegmentation.biBitCount = 1;
    Image_LocalSegmentation.imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
    Image_LocalSegmentation.imageData =     (unsigned char*)malloc(src.height*src.width/8);
     Image_LocalSegmentation.imageRgbQuad[0].rgbBlue =     0;
     Image_LocalSegmentation.imageRgbQuad[0].rgbGreen =      0;
     Image_LocalSegmentation.imageRgbQuad[0].rgbRed =      0;
     Image_LocalSegmentation.imageRgbQuad[0].rgbReserved = 0;
     Image_LocalSegmentation.imageRgbQuad[1].rgbBlue =      255;
     Image_LocalSegmentation.imageRgbQuad[1].rgbGreen =      255;
     Image_LocalSegmentation.imageRgbQuad[1].rgbRed =         255;
     Image_LocalSegmentation.imageRgbQuad[1].rgbReserved = 0;
    
    int mid = (structWidth + 1) / 2 - 1;
    unsigned char val = 0;
    for (int i = mid; i < src.height - mid; i++)
    {
        for (int j = mid; j < src.width - mid; j++)
        {
            for (int m = 0; m < structWidth; m++)
            {
                for (int n = 0; n < structHeight; n++)
                {
                    if (m == mid && n == mid )continue;
                        val |=  src.imageData[(i+m) * src.width + j + n];
                }
            }
 
            Image_LocalSegmentation.imageData[i * src.width + j] = val;
            val = 0;
        }
    }

    return Image_LocalSegmentation;
        
}

BMP_Image MorphMErosion(BMP_Image src,  int structWidth, int structHeight, int num)
{
    int i;
    BMP_Image tmp, tmp1 = src;
    //BMP_Image Image_LocalSegmentation;

    for(i=0; i<num; i++)
    {
        tmp = MorphErosion(tmp1, structWidth, structHeight);
        tmp1 = tmp;
    }

    return tmp;
}

BMP_Image MorphMDilation(BMP_Image src,  int structWidth, int structHeight, int num)
{
    int i;
    BMP_Image tmp, tmp1 = src;
    //BMP_Image Image_LocalSegmentation;

    for(i=0; i<num; i++)
    {
        tmp = MorphDilation(tmp1, structWidth, structHeight);
        tmp1 = tmp;
    }

    return tmp;
}

//�ȸ�ʴ������
BMP_Image MorphOpen(BMP_Image src,  int structWidth, int structHeight, int num)
{
    BMP_Image tmp, tmp1 = src;
    tmp = MorphMErosion(tmp1, structWidth, structHeight,num);
    tmp1 = tmp;
    tmp = MorphMDilation(tmp1, structWidth, structHeight,num);

    return tmp;
}

BMP_Image MorphClose(BMP_Image src,  int structWidth, int structHeight, int num)
{
    BMP_Image tmp, tmp1 = src;
    tmp = MorphMDilation(tmp1, structWidth, structHeight,num);
    tmp1 = tmp;
    tmp = MorphMErosion(tmp1, structWidth, structHeight,num);

    return tmp;
}


//������ͨ�ɷ����   ������һ�������ı�ǩ  �ϲ���ͨ�ɷ�ʱ��
std::vector<RowGroups> add_Groups(std::vector<RowGroups>& v1, std::vector<RowGroups>& v2)
{
	std::vector<RowGroups> r;
 
	for (int i = 0; i < v1.size(); ++i) {
		r.push_back(v1[i]);
	}
	for (int j = 0; j < v2.size(); ++j){
    //	v2[j].block_flag = v1[0].block_flag;               //������ǩ �����ڸó����������ǩʱֱ�Ӷ�ȡÿ����ͨ�ɷֵ�һ�����ص�ı�ǩ����
		r.push_back(v2[j]);
	}
	return r;
}
 
double compare2img(BMP_Image img1, BMP_Image img2) {
	
	if(img1.width != img2.width || img1.height != img2.height || img1.biBitCount != img2.biBitCount)
		return 0;

    int lwidth, bytespp=img1.biBitCount/8;
    lwidth = img1.width*img1.biBitCount/8;
    unsigned long count = img1.height*img1.width, same = 0, sum=0;
    if(img1.biBitCount>1)
    {

        for (int x = 0; x < img1.height; x++)
        {
            for (int y = 0; y < img1.width; y++)
            {
                /*BMPColor c1 = img1.get(x, y);
                BMPColor c2 = img2.get(x, y);*/

                for(int i = 0; i< bytespp; i++)
                {
                    if(img1.imageData[x*lwidth+y+i]==img2.imageData[x*lwidth+y+i])
                        sum++;
                }
                if(sum==bytespp) same++;
                //set(x, y, c2);
                //set(x, height - y - 1, c1);
            }
        }
    }
    else
    {
        for (int i=0; i<count; i=i+8)
        {
            /********* Begin *********/
            int k = i/8;
            
            for (int j = 0; j <= 7; j++)
            {
                if ((img1.imageData[k] &= (128>>j))==(img2.imageData[k] &= (128>>j)))
                    same++;
            }

            /********* End *********/
        }        
        
    }
    double S=(double)same/count;
    if(S>0.98)
        cout<<"True!";
    else
        cout<<"False!";


	return (double)same/count;
}
