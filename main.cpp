#include "bmp.h"
#include "bmp.cpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include "operations.cpp"

using namespace std;

// 灰度限制在0-255
int clamp(int value, int lowerBound, int upperBound)
{
    return std::max(lowerBound, std::min(value, upperBound));
}

int main(int argc, char *argv[])
{

    char *filename1 = (char *)"forest1.bmp";
    char *filename2 = (char *)"forest2.bmp";
    /*add you code here*/


    //1差分
    BMP_Image *f1 = BMP_LoadImage(filename1);
    BMP_Image *f2 = BMP_LoadImage(filename2);

    char *tmpfile = (char *)"tmpfile.bmp";
    BMP_SaveImage(tmpfile, f1);

    BMP_Image *f1_gray = RGB2Gray(f1);
    BMP_Image *f2_gray = RGB2Gray(f2);

    BMP_Image *diffImage;
    diffImage = (BMP_Image *)malloc(sizeof(BMP_Image));
    diffImage->biBitCount = 8;
    diffImage->width = f1_gray->width;
    diffImage->height = f1_gray->height;
    diffImage->imageData = new unsigned char[f1_gray->width * f1_gray->height];
    diffImage->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
    int i;
    for(i=0;i<256;i++)
    {
        diffImage->imageRgbQuad[i].rgbBlue = i;
        diffImage->imageRgbQuad[i].rgbGreen = i;
        diffImage->imageRgbQuad[i].rgbRed = i;
        diffImage->imageRgbQuad[i].rgbReserved = 0;
    }

    for (int i = 0; i < f1_gray->height * f1_gray->width; ++i)
    {
        int diff = abs(f1_gray->imageData[i] - f2_gray->imageData[i]); // 
        diff = clamp(diff, 0, 255);                                    // 
        //
        diffImage->imageData[i] = static_cast<unsigned char>(diff);

    }
    BMP_SaveImage("g1.bmp", f1_gray);
    BMP_SaveImage("g2.bmp", f2_gray);
    BMP_SaveImage("diff1.bmp", diffImage);

    BMP_Image* Sub_Gray=Sub(f1_gray, f2_gray);
    BMP_SaveImage("diff1_2.bmp", Sub_Gray);

    //2 对图像差进行灰度增强，然后利用Otsu阈值分割算法；
    BMP_Image* enhancedDiff = LinearTrans(Sub_Gray);
    BMP_SaveImage("2_1_enhancedDiff.bmp", enhancedDiff);
    BMP_Image* esub_enhancedDiff=eSub(enhancedDiff);
    BMP_SaveImage("2_2_esub_enhancedDiff.bmp",esub_enhancedDiff);
    BMP_Image* esub_enhancedDiff2=eSub(esub_enhancedDiff);
    BMP_Image* esub_enhancedDiff3=eSub(esub_enhancedDiff2);
    BMP_SaveImage("2_3_esub3_enhancedDiff.bmp",esub_enhancedDiff3);

    //unsigned int thred=OTSU(enhancedDiff);
    BMP_Image* OTSU_esub3=LocalSegmentation( diffImage);
    BMP_SaveImage("2_4_ostu_esub3_enhancedDiff.bmp",OTSU_esub3);



    /*

    */

    //    double S = compare2img( *f1_gray, *f2_gray);
    //    std::cout << S <<endl;
    /*if (S == 1)
        cout<<"True!";
    else
        cout<<"False!";*/

    return 0;
}
