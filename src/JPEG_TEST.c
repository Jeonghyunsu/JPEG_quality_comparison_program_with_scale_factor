#include "cdjpeg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#define BYTE    unsigned char

extern int default_scale_factor;
extern int tamperal_scale_factor;

// jcparam.c Tamper Flag
extern int flag;
double PSNR;

double GetPSNR(char filename[140], char comparefilename[140]);
BYTE* GetY(char filename[140], int *YSize);

double JPEG_TEST(char* oriFilePath, char* _oriFileName, char* Ext, int scale_factor)
{
	default_scale_factor = scale_factor;

	char** argv = (char**)malloc(sizeof(char*) * 3);
	int argc = 3;
	int nResult = 0;

	if (!strcmp(Ext, "bmp")) {
		char oriFileName[256] = "";

		memcpy(oriFileName, oriFilePath, strlen(oriFilePath));

		char strFirstFileName[256] = "./";
		char strFirstDecFileName[256] = "./";

		strncat(strFirstFileName, _oriFileName, strlen(_oriFileName) - 3);
		strncat(strFirstFileName, "jpg", 3);

		strncat(strFirstDecFileName, _oriFileName, strlen(_oriFileName) - 4);
		strncat(strFirstDecFileName, "_Dec.bmp", 8);

		
		argv[0] = 0;
		argv[1] = oriFileName;
		argv[2] = strFirstFileName;

		nResult = main_Enc(argc, argv);
		if (nResult != 0)
		{
			printf("Enc Error");
			return -1;
		}

		argv[1] = strFirstFileName;
		argv[2] = strFirstDecFileName;

		nResult = main_Dec(argc, argv);

		if (nResult != 0)
		{
			printf("Dec error");
			return -1;
		}

		
		free(argv);
		return GetPSNR(oriFileName, strFirstDecFileName);
	}
	if (!strcmp(Ext, "jpg")) {
		char oriFileName[256] = "";

		memcpy(oriFileName, oriFilePath, strlen(oriFilePath));

		char strFirstDecFileName[256] = "./";
		char strFirstFileName[256] = "./";
		char strSecondDecFileName[256] = "./";

		strncat(strFirstDecFileName, _oriFileName, strlen(_oriFileName) - 4);
		strncat(strFirstDecFileName, "_Dec.bmp", 8);

		strncat(strFirstFileName, strFirstDecFileName, strlen(strFirstDecFileName) - 7);
		strncat(strFirstFileName, "Re.jpg", 6);

		strncat(strSecondDecFileName, strFirstDecFileName, strlen(strFirstDecFileName) - 4);
		strncat(strSecondDecFileName, "2.bmp", 5);

		argv[0] = 0;
		argv[1] = oriFileName;
		argv[2] = strFirstDecFileName;

		nResult = main_Dec(argc, argv);
		if (nResult != 0)
		{
			printf("Dec error");
			return -1;
		}


		argv[1] = strFirstDecFileName;
		argv[2] = strFirstFileName;

		nResult = main_Enc(argc, argv);
		if (nResult != 0)
		{
			printf("Enc Error");
			return -1;
		}


		argv[1] = strFirstFileName;
		argv[2] = strSecondDecFileName;

		nResult = main_Dec(argc, argv);
		if (nResult != 0)
		{
			printf("Dec error");
			return -1;
		}

		free(argv);
		return GetPSNR(strFirstDecFileName, strSecondDecFileName);
	}	
}

double GetPSNR(char orifilename[140], char comparefilename[140])
{
	BYTE *Y_1, *Y_2;
	int s1, s2;
	int *YSize1 = &s1, *YSize2 = &s2;

	double MSE = 0;
	double PSNR = 0;

	Y_1 = GetY(orifilename, YSize1);//
	Y_2 = GetY(comparefilename, YSize2);//

	if (*YSize1 != *YSize2) {
		printf("다른 사이즈의 bmp 파일입니다.\n %d %d", YSize1, YSize2);
		free(Y_1), free(Y_2);
		exit(1);
	}

	for (int i = 0; i < *YSize1; i++)
	{
		MSE += (Y_1[i] - Y_2[i])*(Y_1[i] - Y_2[i]);
	}
	MSE = (double)MSE / *YSize1;

	PSNR = 10 * log10(255 * 255 / MSE);

	free(Y_1), free(Y_2);
	return PSNR;
}

BYTE* GetY(char filename[140], int *YSize)
{
	FILE *infile;
	int Y_Size;

	infile = fopen(filename, "rb");

	if (infile == NULL) { printf("No Image File"); exit(1); }

	// BMP Header Information
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);

	Y_Size = hInfo.biWidth*hInfo.biHeight;
	*YSize = Y_Size;

	//Get B, G, R
	BYTE *lpImg = (BYTE*)malloc(sizeof(BYTE) * 3 * Y_Size);
	BYTE *B = (BYTE*)malloc(sizeof(BYTE)  * Y_Size);
	BYTE *G = (BYTE*)malloc(sizeof(BYTE)  * Y_Size);
	BYTE *R = (BYTE*)malloc(sizeof(BYTE)  * Y_Size);

	fread(lpImg, sizeof(char), 3 * Y_Size, infile);
	fclose(infile);

	for (int i = 0; i < 3 * Y_Size; i++) {
		if (i % 3 == 0) B[i / 3] = lpImg[i];
		else if (i % 3 == 1) G[i / 3] = lpImg[i];
		else if (i % 3 == 2) R[i / 3] = lpImg[i];
	}

	// Get Y
	BYTE* Y = (BYTE*)malloc(sizeof(BYTE)  * Y_Size);
	for (int i = 0; i < Y_Size; i++) Y[i] = 0.299*R[i] + 0.587*G[i] + 0.114*B[i];

	free(lpImg), free(B), free(G), free(R);
	return Y;
}
