#include "bmp.h"


BMP_Image* BMP_LoadImage(char* path)
{

    BMP_BitMapFileHeader bmpFileHeader;
    BMP_BitMapInfoHeader bmpInfoHeader;
    BMP_RgbQuad* quad;
    BMP_Image* bmpImg;

    FILE* pFile;

    int width = 0;
    int height = 0;
	int offset;
    unsigned char pixVal;


    int i, j, k;
    bmpImg = (BMP_Image*)malloc(sizeof(BMP_Image));
    pFile = fopen(path, "rb");
    if (!pFile)
    {
        free(bmpImg);
        return NULL;
    }


    fread(&bmpFileHeader, sizeof(BMP_BitMapFileHeader), 1, pFile);
    /*
    printf("bmp文件头信息：\n");
    printf("文件大小：%d \n", bmpFileHeader.bfSize);
    printf("保留字：%d \n", bmpFileHeader.bfReserved1);
    printf("保留字：%d \n", bmpFileHeader.bfReserved2);
    printf("位图数据偏移字节数：%d \n", bmpFileHeader.bfOffBits);*/

    fread(&bmpInfoHeader, sizeof(BMP_BitMapInfoHeader), 1, pFile);
    
    printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
    printf("bmp文件信息头\n");
    printf("结构体长度：%d \n", bmpInfoHeader.biSize);
    printf("位图宽度：%d \n", bmpInfoHeader.biWidth);
    printf("位图高度：%d \n", bmpInfoHeader.biHeight);
    printf("位图平面数：%d \n", bmpInfoHeader.biPlanes);
    printf("颜色位数：%d \n", bmpInfoHeader.biBitCount);
    printf("压缩方式：%d \n", bmpInfoHeader.biCompression);
    printf("实际位图数据占用的字节数：%d \n", bmpInfoHeader.biSizeImage);
    printf("X方向分辨率：%d \n", bmpInfoHeader.biXPelsPerMeter);
    printf("Y方向分辨率：%d \n", bmpInfoHeader.biYPelsPerMeter);
    printf("使用的颜色数：%d \n", bmpInfoHeader.biClrUsed);
    printf("重要颜色数：%d \n", bmpInfoHeader.biClrImportant);
    printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");

    printf("File size is:%d\n",sizeof(BMP_BitMapFileHeader));
 	printf("Info size is:%d\n",sizeof(BMP_BitMapInfoHeader));
  	printf("RGB  size is:%d\n",sizeof(BMP_RgbQuad)); 


        if (bmpInfoHeader.biBitCount == 1)
        {
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->biBitCount = 1;

            bmpImg->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height/8);

            quad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
            fread(quad, sizeof(BMP_RgbQuad), 2, pFile);
			for(i=0;i<2;i++)
			{

 		     bmpImg->imageRgbQuad[i].rgbBlue = quad[i].rgbBlue;
			 bmpImg->imageRgbQuad[i].rgbGreen = quad[i].rgbGreen;
 		     bmpImg->imageRgbQuad[i].rgbRed = quad[i].rgbRed;
 		     bmpImg->imageRgbQuad[i].rgbReserved = quad[i].rgbReserved;

            }

            for (i=0; i<height; i++)
            {
                for (j=0; j<width/8; j++)
                {
					fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    bmpImg->imageData[(height-1-i)*width/8+j] = pixVal ;
                }

            }
        }
        else if (bmpInfoHeader.biBitCount == 8)
        {
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->biBitCount = 8;

            bmpImg->imageRgbQuad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);

            quad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
            fread(quad, sizeof(BMP_RgbQuad), 256, pFile);

			for(i=0;i<256;i++)
			{
 		     bmpImg->imageRgbQuad[i].rgbBlue = quad[i].rgbBlue;
			 bmpImg->imageRgbQuad[i].rgbGreen = quad[i].rgbGreen;
 		     bmpImg->imageRgbQuad[i].rgbRed = quad[i].rgbRed;
 		     bmpImg->imageRgbQuad[i].rgbReserved = quad[i].rgbReserved;
            }

            offset = width%4;
            if (offset != 0)
            {
                offset = 4 - offset;
            }

            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    bmpImg->imageData[(height-1-i)*width+j] = pixVal;
                }
                if (offset != 0)
                {
                    for (j=0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }
            }
        }
        else if (bmpInfoHeader.biBitCount == 24)
        {
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->biBitCount = 24;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*3*height);

            offset = (3*width)%4;
            if (offset != 0)
            {
                offset = 4 - offset;
            }
            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    for (k=0; k<3; k++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                        bmpImg->imageData[(height-1-i)*width*3+j*3+k] = pixVal;
                    }
                }
                if (offset != 0)
                {
                    for (j=0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }

			}
		}
        else if (bmpInfoHeader.biBitCount == 32)
        {
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->biBitCount = 32;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*4*height);

            offset = (4*width)%4;
            if (offset != 0)
            {
                offset = 4 - offset;
            }
            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    for (k=0; k<4; k++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                        bmpImg->imageData[(height-1-i)*width*4+j*4+k] = pixVal;
                    }
                }
                if (offset != 0)
                {
                    for (j=0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }

			}
		}

    return bmpImg;
}

bool BMP_SaveImage(char* path, BMP_Image* bmpImg)
{
    FILE *pFile;
   // unsigned short fileType;
    BMP_BitMapFileHeader bmpFileHeader;
    BMP_BitMapInfoHeader bmpInfoHeader;
    unsigned char pixVal; //= '\0';
    int i, j;
	int step,offset;
    BMP_RgbQuad* quad;

    pFile = fopen(path, "wb");
    if (!pFile)
    {
        return false;
    }

    //printf("Bitcount is:%d\n",bmpImg->biBitCount);

    if (bmpImg->biBitCount == 1)//1位，单通道，黑白图
    {
        bmpFileHeader.bfType = 0x4D42;
        bmpFileHeader.bfSize = 54 + 2*4 + bmpImg->height*bmpImg->width/8;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54 + 2*4;
        fwrite(&bmpFileHeader, sizeof(BMP_BitMapFileHeader), 1, pFile);

        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 1;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*bmpImg->width/8;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 0;
        bmpInfoHeader.biClrImportant = 0;
        fwrite(&bmpInfoHeader, sizeof(BMP_BitMapInfoHeader), 1, pFile);

        quad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*2);
        for (i=0; i<2; i++)
        {
 		      quad[i].rgbBlue = bmpImg->imageRgbQuad[i].rgbBlue;
			  quad[i].rgbGreen = bmpImg->imageRgbQuad[i].rgbGreen;
 		      quad[i].rgbRed = bmpImg->imageRgbQuad[i].rgbRed;
 		      quad[i].rgbReserved = bmpImg->imageRgbQuad[i].rgbReserved;

        }
        fwrite(quad, sizeof(BMP_RgbQuad), 2, pFile);
        free(quad);

        for (i=bmpImg->height-1; i>-1; i--)
        {
            for (j=0; j<bmpImg->width/8; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width/8+j];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
        }
    }

    else if (bmpImg->biBitCount == 8)//8位，单通道，灰度图
    {
        step = bmpImg->width;
        offset = step%4;
        if (offset != 4)
        {
            step += 4-offset;
        }

        bmpFileHeader.bfType = 0x4D42;
        bmpFileHeader.bfSize = 54 + 256*4 + bmpImg->height*step;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54 + 256*4;
        fwrite(&bmpFileHeader, sizeof(BMP_BitMapFileHeader), 1, pFile);

        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 8;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 256;
        bmpInfoHeader.biClrImportant = 256;
        fwrite(&bmpInfoHeader, sizeof(BMP_BitMapInfoHeader), 1, pFile);

        quad = (BMP_RgbQuad*)malloc(sizeof(BMP_RgbQuad)*256);
        for (i=0; i<256; i++)
        {
 		      quad[i].rgbBlue = bmpImg->imageRgbQuad[i].rgbBlue;
			  quad[i].rgbGreen = bmpImg->imageRgbQuad[i].rgbGreen;
 		      quad[i].rgbRed = bmpImg->imageRgbQuad[i].rgbRed;
 		      quad[i].rgbReserved = bmpImg->imageRgbQuad[i].rgbReserved;
        }
        fwrite(quad, sizeof(BMP_RgbQuad), 256, pFile);
        free(quad);

        for (i=bmpImg->height-1; i>-1; i--)
        {
            for (j=0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width+j];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            if (offset!=0)
            {
                for (j=0; j<4-offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }

        }
    }

   else if (bmpImg->biBitCount == 24)//24位，通道，彩图
    {
        step = 3*bmpImg->width;
        offset = step%4;
        if (offset != 4)
        {
            step += 4-offset;
        }

        bmpFileHeader.bfType = 0x4D42;
        bmpFileHeader.bfSize = bmpImg->height*step + 54;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54;
        fwrite(&bmpFileHeader, sizeof(BMP_BitMapFileHeader), 1, pFile);

        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 24;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 0;
        bmpInfoHeader.biClrImportant = 0;
        fwrite(&bmpInfoHeader, sizeof(BMP_BitMapInfoHeader), 1, pFile);



        for (i=bmpImg->height-1; i>-1; i--)
        {
            for (j=0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+1];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+2];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            if (offset!=0)
            {
                for (j=0; j<4-offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }
   else if (bmpImg->biBitCount == 32)//32位，通道，彩图
    {
        step = 4*bmpImg->width;
        offset = step%4;
        if (offset != 4)
        {
            step += 4-offset;
        }

        bmpFileHeader.bfType = 0x4D42;
        bmpFileHeader.bfSize = bmpImg->height*step + 54;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54;
        fwrite(&bmpFileHeader, sizeof(BMP_BitMapFileHeader), 1, pFile);

        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 32;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 0;
        bmpInfoHeader.biClrImportant = 0;
        fwrite(&bmpInfoHeader, sizeof(BMP_BitMapInfoHeader), 1, pFile);



        for (i=bmpImg->height-1; i>-1; i--)
        {
            for (j=0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width*4+j*4];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*4+j*4+1];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*4+j*4+2];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*4+j*4+3];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
        }
        if (offset!=0)
        {
            for (j=0; j<4-offset; j++)
            {
                pixVal = 0;
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
        }
    }

    fclose(pFile);

    return true;
}
