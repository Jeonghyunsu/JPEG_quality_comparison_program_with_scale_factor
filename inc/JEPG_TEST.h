#pragma once

double GetPSNR(char filename[140], char comparefilename[140]);
BYTE* GetY(char filename[140], int *YSize);

int JPEG_TEST(char* oriFilePath, int scale_factor);