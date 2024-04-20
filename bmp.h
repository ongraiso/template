#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>

//定义位图头文件结构体变量
typedef struct
{
/*
    u_int16_t bfType;
    u_int32_t bfSize;
    u_int16_t bfReserved1;
    u_int16_t bfReserved2;
    u_int32_t bfOffBits;*/
    
    unsigned short    bfType;   //位图文件的类型，必须为BM(1-2字节）
    unsigned long     bfSize;   //位图文件的大小，以字节为单位（3-6字节，低位在前）
    unsigned short    bfReserved1; //位图文件保留字，必须为0(7-8字节）
    unsigned short    bfReserved2;  //位图文件保留字，必须为0(9-10字节）
    unsigned long     bfOffBits;   //位图数据的起始位置，以相对于位图（11-14字节，低位在前）
    
} __attribute__((packed)) BMP_BitMapFileHeader;

//定义位图信息结构体变量
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
    
    unsigned long    biSize;    //本结构所占用字节数（15-18字节）
    long             biWidth;   //位图的宽度，以像素为单位（19-22字节）
    long             biHeight;  //位图的高度，以像素为单位（23-26字节）
    unsigned short   biPlanes;   //目标设备的级别，必须为1(27-28字节）
    unsigned short   biBitCount;  //每个像素所需的位数，必须是1（双色），（29-30字节）
//4(16色），8(256色）16(高彩色)或24（真彩色）之一
    unsigned long    biCompression;//位图压缩类型，必须是0（不压缩），（31-34字节）
    unsigned long    biSizeImage; //位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位（35-38字节）
    long             biXPelsPerMeter; //位图水平分辨率，每米像素数（39-42字节）
    long             biYPelsPerMeter; //位图垂直分辨率，每米像素数（43-46字节)
    unsigned long    biClrUsed;   //位图实际使用的颜色表中的颜色数（47-50字节）
    unsigned long    biClrImportant; //位图显示过程中重要的颜色数（51-54字节）
    
} __attribute__((packed)) BMP_BitMapInfoHeader;

//定义调色板结构体变量
typedef struct
{
    unsigned char rgbBlue; //该颜色的蓝色分量
    unsigned char rgbGreen; //该颜色的绿色分量
    unsigned char rgbRed; //该颜色的红色分量
    unsigned char rgbReserved; //保留值
}BMP_RgbQuad;


//定义像素值结构体变量
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

//连通区域结构体
struct RowGroups{
	int begin_pos;      //区域的y轴起始值
	long end_pos;        //区域的y轴结束值
	int row;            //区域所在的行
	int block_flag;     //区域的标记
	int change_times;   //区域标记改变的次数
	//	std::vector<int> other_flags;   //该区域所连接的还未进行融合其他区域的标记  用于连接了多个区域时
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
