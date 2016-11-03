#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <time.h>

#include "./bmp.h"

#define WIDTH 500
#define HEIGHT WIDTH

int main(int argc , char **argv) {
	srand(time(NULL));

	if (argc != 2) {
		std::cout << "No outfile specified!\n";
		std::exit(1);
	}

	std::fstream outfile(argv[1],
	                     std::fstream::out
	                     | std::fstream::trunc
	                     | std::fstream::binary);

	if (!outfile.is_open()) {
		std::cout << "Could not open " << argv[1] << "!\n";
		std::exit(2);
	}

	BITMAPFILEHEADER bmp_fh;
	bmp_fh.bfType = 0x4d42;
	bmp_fh.bfOffBits = 54;
	bmp_fh.bfReserved1 = bmp_fh.bfReserved2 = 0;
	bmp_fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER bmp_ih;
	bmp_ih.biSize = 40;
	bmp_ih.biWidth = WIDTH;
	bmp_ih.biHeight = HEIGHT;
	bmp_ih.biPlanes = 1;
	bmp_ih.biBitCount = 24;
	bmp_ih.biCompression = 0;
	bmp_ih.biSizeImage = 0;
	bmp_ih.biXPelsPerMeter = 10; /* idk */
	bmp_ih.biYPelsPerMeter = 10;
	bmp_ih.biClrUsed = 0;
	bmp_ih.biClrImportant = 0;

	outfile.write((char*)&bmp_fh , sizeof(BITMAPFILEHEADER));
	outfile.write((char*)&bmp_ih , sizeof(BITMAPINFOHEADER));

	std::vector<std::vector<RGBTRIPLE>>
	matrix(HEIGHT, std::vector<RGBTRIPLE>(WIDTH));

	for (size_t i = 0, j, r ; i < matrix.size() ; i++) {
		for ( j = 0 ; j < matrix[i].size() ; j++) {
			r = rand() % 64 + 1;

			if (r < 64) {
				if (i > 0 && j > 0) {
					matrix[i][j] = matrix[i-1][j-1];
				}
			} else if (r < 30) {
				if (i > 0) {
					matrix[i][j] = matrix[i][j-1];
				}
			} else if (r < 57) {
				if (j > 0) {
					matrix[i][j] = matrix[i][j-1];
				}
			} else {
				matrix[i][j] = {
					(BYTE)(rand() % 256),
					(BYTE)(rand() % 256),
					(BYTE)(rand() % 256)
				};
			}

			outfile.write((char*)&matrix[i][j], sizeof(RGBTRIPLE));
		}
	}





	outfile.close();

	return 0;
}
