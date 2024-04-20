#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>

//����λͼͷ�ļ��ṹ�����
typedef struct
{
/*
    u_int16_t bfType;
    u_int32_t bfSize;
    u_int16_t bfReserved1;
    u_int16_t bfReserved2;
    u_int32_t bfOffBits;*/
    
    unsigned short    bfType;   //λͼ�ļ������ͣ�����ΪBM(1-2�ֽڣ�
    unsigned long     bfSize;   //λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ��3-6�ֽڣ���λ��ǰ��
    unsigned short    bfReserved1; //λͼ�ļ������֣�����Ϊ0(7-8�ֽڣ�
    unsigned short    bfReserved2;  //λͼ�ļ������֣�����Ϊ0(9-10�ֽڣ�
    unsigned long     bfOffBits;   //λͼ���ݵ���ʼλ�ã��������λͼ��11-14�ֽڣ���λ��ǰ��
    
} __attribute__((packed)) BMP_BitMapFileHeader;

//����λͼ��Ϣ�ṹ�����
typedef struct
{
    /*u_int32_t biSize;
    u_int32_t biWidth;
    u_int32_t biHeight;
    u_int16_t biPlanes;
    u_int16_t biBitCount;
    u_int32_t biCompression;
    u_int32_t biSizeImage;
    u_int32_t biXPelsPerMeter;
    u_int32_t biYPelsPerMeter;
    u_int32_t biClrUsed;
    u_int32_t biClrImportant;*/
    
    unsigned long    biSize;    //���ṹ��ռ���ֽ�����15-18�ֽڣ�
    long             biWidth;   //λͼ�Ŀ�ȣ�������Ϊ��λ��19-22�ֽڣ�
    long             biHeight;  //λͼ�ĸ߶ȣ�������Ϊ��λ��23-26�ֽڣ�
    unsigned short   biPlanes;   //Ŀ���豸�ļ��𣬱���Ϊ1(27-28�ֽڣ�
    unsigned short   biBitCount;  //ÿ�����������λ����������1��˫ɫ������29-30�ֽڣ�
//4(16ɫ����8(256ɫ��16(�߲�ɫ)��24�����ɫ��֮һ
    unsigned long    biCompression;//λͼѹ�����ͣ�������0����ѹ��������31-34�ֽڣ�
    unsigned long    biSizeImage; //λͼ�Ĵ�С(���а�����Ϊ�˲���������4�ı�������ӵĿ��ֽ�)�����ֽ�Ϊ��λ��35-38�ֽڣ�
    long             biXPelsPerMeter; //λͼˮƽ�ֱ��ʣ�ÿ����������39-42�ֽڣ�
    long             biYPelsPerMeter; //λͼ��ֱ�ֱ��ʣ�ÿ����������43-46�ֽ�)
    unsigned long    biClrUsed;   //λͼʵ��ʹ�õ���ɫ���е���ɫ����47-50�ֽڣ�
    unsigned long    biClrImportant; //λͼ��ʾ��������Ҫ����ɫ����51-54�ֽڣ�
    
} __attribute__((packed)) BMP_BitMapInfoHeader;

//�����ɫ��ṹ�����
typedef struct
{
    unsigned char rgbBlue; //����ɫ����ɫ����
    unsigned char rgbGreen; //����ɫ����ɫ����
    unsigned char rgbRed; //����ɫ�ĺ�ɫ����
    unsigned char rgbReserved; //����ֵ
}BMP_RgbQuad;


//��������ֵ�ṹ�����
typedef struct
{
    /*u_int32_t width;
    u_int32_t height;
    u_int16_t biBitCount;*/
    unsigned long width;
    unsigned long height;
    unsigned short biBitCount;
	BMP_RgbQuad *imageRgbQuad;
    unsigned char* imageData;
}BMP_Image;

typedef struct
{
   BMP_Image *HSI_H;
   BMP_Image *HSI_S;
   BMP_Image *HSI_I;
}HSI;

//��ͨ����ṹ��
struct RowGroups{
	int begin_pos;      //�����y����ʼֵ
	long end_pos;        //�����y�����ֵ
	int row;            //�������ڵ���
	int block_flag;     //����ı��
	int change_times;   //�����Ǹı�Ĵ���
	//	std::vector<int> other_flags;   //�����������ӵĻ�δ�����ں���������ı��  ���������˶������ʱ
};

BMP_Image* BMP_LoadImage(char* path);
bool BMP_SaveImage(char* path, BMP_Image* bmpImg);

BMP_Image* RGB2Gray(BMP_Image* Image_In);
HSI* RGB2HSI(BMP_Image* Image_In);
BMP_Image* FalseColor(BMP_Image* Image_In);
BMP_Image* PseudoColor(BMP_Image* Image_In);

int Mid(int a,int b,int c);
BMP_Image* MidFilter(BMP_Image* Image_In);
BMP_Image* MeanFilter(BMP_Image* Image_In);

BMP_Image* Roberts(BMP_Image* Image_In);
BMP_Image* Sobel(BMP_Image* Image_In);
BMP_Image* Prewitt(BMP_Image* Image_In);
BMP_Image* Laplacian(BMP_Image* Image_In);

BMP_Image* InvertColor(BMP_Image* Image_In);
BMP_Image* HistNormolize(BMP_Image* Image_In);
BMP_Image* LinearTrans(BMP_Image* Image_In);

unsigned int OTSU(BMP_Image* Image_In);
BMP_Image* LocalSegmentation(BMP_Image* Image_In);
BMP_Image* GlobalSegmentation(BMP_Image* Image_In);

BMP_Image* eSub(BMP_Image* Image_SubIn);//multiply
BMP_Image* Sub(BMP_Image* Image_In1, BMP_Image* Image_In2);

#define SIZE 5
int**  strelRec();
int**  strelLine();
int**  strelDisk();
BMP_Image MorphErosion(BMP_Image src,  int structWidth, int structHeight);
BMP_Image MorphDilation(BMP_Image src,  int structWidth, int structHeight);
BMP_Image MorphMErosion(BMP_Image src,  int structWidth, int structHeight,int num);
BMP_Image MorphMDilation(BMP_Image src,  int structWidth, int structHeight,int num);
BMP_Image MorphOpen(BMP_Image src,  int structWidth, int structHeight,int num);
BMP_Image MorphClose(BMP_Image src,  int structWidth, int structHeight,int num);
#endif
