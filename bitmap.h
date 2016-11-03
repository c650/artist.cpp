#ifndef BITMAP_H
#define BITMAP_H

#include "./bmp.h"

#include <vector>

class Bitmap {

	BITMAPFILEHEADER bmp_fh;
	BITMAPINFOHEADER bmp_ih;
	std::vector<std::vector<RGBTRIPLE>> matrix;

public:
	Bitmap();

	Bitmap(DWORD height, DWORD width);

	std::fstream& operator<<(std::fstream& outfile, const Bitmap& bmp);

private:
	void init();


};

#endif
