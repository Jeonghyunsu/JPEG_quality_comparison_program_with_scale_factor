#include "cdjpeg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

// LogLaw N, q, s range Clip
#define NMIN 0
#define NMAX 1
#define QMIN 2
#define QMAX 3
#define SMIN 4
#define SMAX 5

#define FILEWRITE 10
#define FILEWRITE_AVERAGE 11

#define BYTE    unsigned char

#define FILENUM 5

/***** (1) AC 추출 *****/
// jcdctmgr.c Y first digits Counting
extern int default_scale_factor;
extern int tamperal_scale_factor;

extern int totalBlock;
extern int HeightNum;
extern int print;
extern int YUV_divisor;
extern int Y[11];
extern int YSum;

// jcparam.c Tamper Flag
extern int flag;

void InitLoop();
void InitFileName(char copyoriFileName[140], char strFirstFileName[140], char strFirstDecFileName[140], char strSecondFileName[140], char strSecondDecFileName[140], int j);



/***** (2) Fitting *****/
double myPicProbablity[9];
double LogArray[9];
double resultLogArray[9];

double SSE = 100000;
double tempSSE = 0;
double resultN, resultq, results;

void LogLaw(double N, double q, double s);
double clip(int type, double value, double plusminus);
double FittingMethod();


/***** (3) Chi Square *****/
int fitCntArr[1];

// Average first digits Counting
double AverageArr[9] = { 0, };
int AverageSum = 0;
int AverageCnt = 0;


/***** (4) PSNR *****/
double PSNR;

double GetPSNR(char filename[140], char comparefilename[140]);
BYTE* GetY(char filename[140], int *YSize);


// File Write 
void FileWrite(FILE* fi, FILE* fi2, char orifilename[140], char comparefilename[140], int write_type);
int etcCnt = 0;


int main(int argc, char* argv[])
{
	FILE *fi;
	FILE *fi2;
	char* oriFileameArr[FILENUM] = { "hd_test1.bmp","hd_test2.bmp", "hd_test3.bmp", "hd_test4.bmp", "hd_test5.bmp" };
		
		/*{ "akiyo_cif.bmp" , "bus_cif.bmp", "coastguard_cif.bmp", "flower_cif.bmp", "foreman_cif.bmp", "hall_cif.bmp",
		"stefan_cif.bmp", "tempete_cif.bmp", "waterfall_cif.bmp", "bridge_close_cif.bmp" , "bridge_far_cif.bmp", "container_cif.bmp", "highway_cif.bmp", "mobile_cif.bmp", "mother_daughter_cif.bmp",
		"news_cif.bmp", "paris_cif.bmp", "silent_cif.bmp" };*/

	/*{ "hd_test_simple1.bmp", "hd_test_simple2.bmp", "hd_test_simple3.bmp", "hd_test_simple4.bmp", "hd_test_simple5.bmp"};*/


	char* defaultPath = "C:\\Users\\hsay0\\Documents\\Visual Studio 2017\\Projects\\JPEG_1\\JPEG__Test\\";

	fi = fopen("C:\\Users\\hsay0\\Documents\\Visual Studio 2017\\Projects\\JPEG_1\\JPEG__Test\\test.txt", "w");
	if (fi == NULL) {
		return;
	}

for (default_scale_factor = 50; default_scale_factor <= 100 ;)
{
	for (int index = 0; index < FILENUM; index++)
	{

	/***** Tamperal Q *****/
	// (Input)
	
	tamperal_scale_factor = 1;

	char filepath[200] = "";
	char DirName[80] = "";
	memcpy(filepath, defaultPath, strlen(defaultPath));
	memcpy(DirName, oriFileameArr[index], strlen(oriFileameArr[index]) - 4);
	strncat(filepath, DirName, strlen(DirName));
	strcat(filepath, "\\result.txt");
	/*fi = fopen(filepath, "w");
	if (fi == NULL) {
		printf("Null Error : %s\n", filepath);
		return;
	}*/

	strset(filepath, 0, 200);
	memcpy(filepath, defaultPath, strlen(defaultPath));
	memcpy(DirName, oriFileameArr[index], strlen(oriFileameArr[index]) - 4);
	strncat(filepath, DirName, strlen(DirName));
	strcat(filepath, "\\resultMatlab.txt");
	fi2 = fopen(filepath, "w");
	if (fi2 == NULL) {
		printf("Null Error : %s\n", filepath);
		return;
	}
	//fprintf(fi, "\n------------------------------------------\n");
	//fprintf(fi, "\nFile Name : %s\n", oriFileameArr[index]);
	//fprintf(fi, "\n------------------------------------------\n");
	fprintf(fi2, "\n------------------------------------------\n");
	fprintf(fi2, "\nFile Name : %s\n", oriFileameArr[index]);
	fprintf(fi2, "\n------------------------------------------\n");
	printf("\n------------------------------------------\n");
	printf("\nFile Name : %s\n", oriFileameArr[index]);
	printf("\n------------------------------------------\n");

		for (int j = tamperal_scale_factor; j <= 100; )// (Input) Max Size
		{
			etcCnt = j;

			InitLoop();
			char oriFileName[140] = "";
			char tempDir[20] = "";
			memcpy(oriFileName, defaultPath, strlen(defaultPath));

			strncat(oriFileName, DirName, strlen(DirName));
			strcat(oriFileName, "\\");
			strncat(oriFileName, oriFileameArr[index], strlen(oriFileameArr[index]));

			int nResult = 0;
			char strFirstFileName[140] = "";
			char strFirstDecFileName[140] = "";
			char strSecondFileName[140] = "";
			char strSecondDecFileName[140] = "";
			
			InitFileName(oriFileName, strFirstFileName, strFirstDecFileName, strSecondFileName, strSecondDecFileName, j);
			
			argc = 3;

			/***** First Encoding *****/
			flag = 0;

			argv[1] = oriFileName;
			argv[2] = strFirstFileName;

			nResult = main_Enc(argc, argv);
			if (nResult != 0)
			{
				printf("First Enc Error");
				return -1;
			}
			
			
			/***** First Decoding *****/
			argv[1] = strFirstFileName;
			argv[2] = strFirstDecFileName;

			nResult = main_Dec(argc, argv);

			if (nResult != 0)
			{
				printf("first dec error");
				return -1;
			}
			
			/***** Second Encoding *****/
			InitLoop();
			flag = 1;

			argv[1] = strFirstDecFileName;
			argv[2] = strSecondFileName;

			nResult = main_Enc(argc, argv);

			if (nResult != 0)
			{
				printf("Second Enc Error");
				return -1;
			}
			

			/***** Second Decoding *****/
			argv[1] = strSecondFileName;
			argv[2] = strSecondDecFileName;

			nResult = main_Dec(argc, argv);

			if (nResult != 0)
			{
				printf("Second Enc Error");
				return -1;
			}

			FileWrite(fi, fi2, oriFileName, strSecondDecFileName, FILEWRITE);

			if (j == 1) {
				tamperal_scale_factor += 9;
				j += 9;
			}
			else {
				tamperal_scale_factor += 10;
				j += 10;
			}
		}
		etcCnt = 0;
		FileWrite(fi, fi2, "", "", FILEWRITE_AVERAGE);
		//fclose(fi); 
		fclose(fi2);
	}

	default_scale_factor += 10;
}

	fclose(fi);
	return 0;
}

void FileWrite(FILE* fi, FILE* fi2, char orifilename[140], char comparefilename[140], int write_type)
{
	if (write_type == FILEWRITE)
	{
		/***** Fitting *****/
		for (int k = 1; k <= 9; k++)
		{
			myPicProbablity[k - 1] = (double)Y[k] / YSum;
			AverageArr[k - 1] += myPicProbablity[k - 1];
		}
		AverageSum += YSum;
		AverageCnt++;

		double mySSE = FittingMethod();
		SSE = 100000;
		tempSSE = 0;

		/***** Chi Square *****/
		double chi = 0;
		for (int i = 0; i <= 8; i++)
		{
			if (resultLogArray[i] != 0)chi += (double)((myPicProbablity[i] - resultLogArray[i]) * (myPicProbablity[i] - resultLogArray[i])) / resultLogArray[i];
		}
		chi *= YSum;

		/***** PSNR *****/
		PSNR = GetPSNR(orifilename, comparefilename);

		/***** 파일 입출력 *****/
		printf("\n\n");
		//fprintf(fi, "\n%d\n", tamperal_scale_factor);
		//if (etcCnt != default_scale_factor)fprintf(fi, "%lf\n", mySSE);
		if (etcCnt != default_scale_factor)fprintf(fi, "%lf\n", chi);
		//fprintf(fi, "%lf\n", PSNR);
		//fprintf(fi, "%d\n", YSum);

		for (int i = 0; i <= 8; i++) {
			//fprintf(fi, "%lf\n", myPicProbablity[i]);
		}

		fprintf(fi2, "\nTamperal :   %d", tamperal_scale_factor);
		fprintf(fi2, "\nSSE :        %lf\n", mySSE);
		for (int i = 0; i <= 8; i++) {
			fprintf(fi2, "%lf, ", myPicProbablity[i]);
		}fprintf(fi2, "\n\n");
	}
	else if (write_type == FILEWRITE_AVERAGE)
	{
		/***** Average Fitting *****/
		for (int k = 1; k <= 9; k++)
		{
			myPicProbablity[k - 1] = (double)AverageArr[k - 1] / AverageCnt;
		}
		for (int k = 1; k <= 9; k++) AverageArr[k - 1] = 0;

		double mySSE = FittingMethod();
		SSE = 100000;
		tempSSE = 0;

		/***** Chi Square *****/
		double chi = 0;
		for (int i = 0; i <= 8; i++)
		{
			if (resultLogArray[i] != 0)chi += (double)((myPicProbablity[i] - resultLogArray[i]) * (myPicProbablity[i] - resultLogArray[i])) / resultLogArray[i];
		}
		chi *= YSum;

		/***** 파일 입출력 *****/
		//fprintf(fi, "\nAverage\n");
		//fprintf(fi, "%lf\n", mySSE);
		//fprintf(fi, "%lf\n", chi);
		//fprintf(fi, "%d\n", AverageSum);

		for (int i = 0; i <= 8; i++) {
			//fprintf(fi, "%lf\n", myPicProbablity[i]);
		}
		AverageSum = 0;
		AverageCnt = 0;
	}
}

void InitLoop()
{
	totalBlock = 0;
	HeightNum = 0;
	print = 0;
	YUV_divisor = 0;

	for (int i = 0; i < 11; i++) Y[i] = 0;
	YSum = 0;
}

void InitFileName(char oriFileName[140], char strFirstFileName[140], char strFirstDecFileName[140], char strSecondFileName[140], char strSecondDecFileName[140], int j)
{
	memcpy(strFirstFileName, oriFileName, strlen(oriFileName) - 3);
	strncat(strFirstFileName, "jpg", 3);

	memcpy(strFirstDecFileName, oriFileName, strlen(oriFileName) - 4);
	strncat(strFirstDecFileName, "_Dec.bmp", 8);

	char strj[4];
	itoa(j, strj, 10);
	memcpy(strSecondFileName, oriFileName, strlen(oriFileName) - 4);
	strncat(strSecondFileName, strj, strlen(strj));
	strncat(strSecondFileName, ".jpg", 4);

	memcpy(strSecondDecFileName, oriFileName, strlen(oriFileName) - 4);
	strncat(strSecondDecFileName, "_Dec2.bmp", 9);

}

void LogLaw(double N, double q, double s)
{
	tempSSE = 0;

	for (int x = 1; x <= 9; x++)
	{
		LogArray[x - 1] = N * log10(1 + ((double)1 / (s + pow((double)x, q))));
		tempSSE += pow((myPicProbablity[x - 1] - LogArray[x - 1]), 2);
	}

	if (tempSSE < SSE)
	{
		SSE = tempSSE;
		for (int i = 0; i <= 8; i++) resultLogArray[i] = LogArray[i];
		resultN = N; resultq = q; results = s;
	}
}

double clip(int type, double value, double plusminus)
{
	switch (type)
	{
	case 0:
		if (value - plusminus <= 0.1)return 0.1;
		else return value - plusminus;
	case 1:
		if (value + plusminus >= 3)return 3;
		else return value + plusminus;
	case 2:
		if (value - plusminus <= 0.1)return 0.1;
		else return value - plusminus;
	case 3:
		if (value + plusminus >= 3)return 3;
		else return value + plusminus;
	case 4:
		if (value - plusminus <= -1)return -1;
		else return value - plusminus;
	case 5:
		if (value + plusminus >= 1)return 1;
		else return value + plusminus;
	}
}

double FittingMethod()
{
	double firstPM = 0.3;
	double secondPM = 0.03;
	double thirdPM = 0.003;

	double Nm, Np;
	double qm, qp;
	double sm, sp;

	for (double i = 0.1; i <= 3; i += 0.1)
		for (double j = 0.1; j <= 3; j += 0.1)
			for (double k = -1; k <= 1; k += 0.1) LogLaw(i, j, k);

	Nm = clip(NMIN, resultN, firstPM);
	Np = clip(NMAX, resultN, firstPM);
	qm = clip(QMIN, resultq, firstPM);
	qp = clip(QMAX, resultq, firstPM);
	sm = clip(SMIN, results, firstPM);
	sp = clip(SMAX, results, firstPM);

	for (double i = Nm; i <= Np; i += 0.01)
		for (double j = qm; j <= qp; j += 0.01)
			for (double k = sm; k <= sp; k += 0.01) LogLaw(i, j, k);

	Nm = clip(NMIN, resultN, secondPM);
	Np = clip(NMAX, resultN, secondPM);
	qm = clip(QMIN, resultq, secondPM);
	qp = clip(QMAX, resultq, secondPM);
	sm = clip(SMIN, results, secondPM);
	sp = clip(SMAX, results, secondPM);

	for (double i = Nm; i <= Np; i += 0.001)
		for (double j = qm; j <= qp; j += 0.001)
			for (double k = sm; k <= sp; k += 0.001) LogLaw(i, j, k);

	Nm = clip(NMIN, resultN, thirdPM);
	Np = clip(NMAX, resultN, thirdPM);
	qm = clip(QMIN, resultq, thirdPM);
	qp = clip(QMAX, resultq, thirdPM);
	sm = clip(SMIN, results, thirdPM);
	sp = clip(SMAX, results, thirdPM);

	for (double i = Nm; i <= Np; i += 0.0001)
		for (double j = qm; j <= qp; j += 0.0001)
			for (double k = sm; k <= sp; k += 0.0001) LogLaw(i, j, k);

	return SSE;
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