#include "./bitmap.h"
#include "./bmp.h"

#include <vector>

Bitmap::Bitmap() {
	init();
}

Bitmap::Bitmap(DWORD height, DWORD width) {
	init();
	bmp_ih.biWidth = width;
	bmp_ih.biHeight = height;
	bmp_fh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
	                bmp_ih.biBitCount * bmp_ih.biWidth * bmp_ih.biHeight;

	matrix.resize(bmp_ih.biHeight, std::vector<RGBTRIPLE>(bmp_ih.biWidth));
}

void Bitmap::init() {
	bmp_fh.bfType = 0x4d42;
	bmp_fh.bfReserved1 = bmp_fh.bfReserved2 = 0;
	bmp_fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmp_ih.biSize = sizeof(BITMAPINFOHEADER);
	bmp_ih.biWidth = 500; /* default dimensions. */
	bmp_ih.biHeight = 500; /* ^^ */
	bmp_ih.biPlanes = 1;
	bmp_ih.biBitCount = 24;
	bmp_ih.biCompression = 0;
	bmp_ih.biSizeImage = 0;
	bmp_ih.biXPelsPerMeter = 10; /* idk */
	bmp_ih.biYPelsPerMeter = 10;
	bmp_ih.biClrUsed = 0;
	bmp_ih.biClrImportant = 0;

	bmp_fh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
	                bmp_ih.biBitCount * bmp_ih.biWidth * bmp_ih.biHeight;

	matrix.resize(bmp_ih.biHeight, std::vector<RGBTRIPLE>(bmp_ih.biWidth));
}

std::fstream& Bitmap::operator<<(std::fstream& outfile, const Bitmap& bmp) {
	outfile.write( (char*)&bmp_fh , sizeof(BITMAPFILEHEADER) );
	outfile.write( (char*)&bmp_ih , sizeof(BITMAPINFOHEADER) );

	for (const auto& row : matrix)
		for (const auto& e : row)
			outfile.write( (char*)&e , bmp_ih.biBitCount );
}
