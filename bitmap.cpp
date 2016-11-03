#include "./bitmap.h"
#include "./bmp.h"

#include <functional>
#include <vector>
#include <fstream>
#include <random>
#include <iostream>

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
	bmp_ih.biXPelsPerMeter = 5; /* idk */
	bmp_ih.biYPelsPerMeter = 5;
	bmp_ih.biClrUsed = 0;
	bmp_ih.biClrImportant = 0;

	bmp_fh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
	                bmp_ih.biBitCount * bmp_ih.biWidth * bmp_ih.biHeight;

	matrix.resize(bmp_ih.biHeight, std::vector<RGBTRIPLE>(bmp_ih.biWidth));
}

void Bitmap::write_to(std::fstream& outfile) const {
	outfile.write( (char*)&bmp_fh , sizeof(BITMAPFILEHEADER) );
	outfile.write( (char*)&bmp_ih , sizeof(BITMAPINFOHEADER) );

	for (const auto& row : matrix)
		for (const auto& e : row)
			outfile.write( (char*)&e , bmp_ih.biBitCount );
}

void Bitmap::process() {
	const std::vector<std::vector<int>> kernel = {
		// {0, -1, 0},
		// {-1, 5, -1},
		// {0, -1, 0}
		{1,2,1},
		{2,4,2},
		{1,2,1}
	};

	auto new_matrix = matrix;
	const size_t sharp_radius = 1;
	const size_t dia = 2*sharp_radius + 1;
	const size_t area = dia * dia;

	size_t i,j,n,m,  k,l;
	int blue, green, red;
	for (i = sharp_radius, n = matrix.size() - sharp_radius ; i < n ; i++) {
		for (j = sharp_radius, m = matrix[i].size() - sharp_radius ; j < m ; j++) {

			blue = green = red = 128;
			for (k = 0; k < dia; k++) {
				for (l = 0; l < dia; l++) {
					blue  += kernel[k][l] * matrix[i - sharp_radius + k][j - sharp_radius + l].rgbtBlue;
					green += kernel[k][l] * matrix[i - sharp_radius + k][j - sharp_radius + l].rgbtGreen;
					red   += kernel[k][l] * matrix[i - sharp_radius + k][j - sharp_radius + l].rgbtRed;
				}
			}
			new_matrix[i][j] = {
				(BYTE)(blue / area),
				(BYTE)(green / area),
				(BYTE)(red / area)
			};
		}
	}
	matrix = new_matrix;
}
