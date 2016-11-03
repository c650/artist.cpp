#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <random>
#include <chrono>

#include <time.h>

#include "./bmp.h"

#define WIDTH 1000
#define HEIGHT WIDTH

#define TLW 192//36 /* weights for copying color from a different position. */
#define TW 192//43
#define LW 130//65
#define TRW 68
#define RW 75
#define BLW 192//86
#define BRW 128
#define BW 188
#define WEIGHT_RANGE 192

int main(int argc , char **argv) {

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

	Bitmap bmp(HEIGHT, WIDTH);

	outfile.write((char*)&bmp_fh , sizeof(BITMAPFILEHEADER));
	outfile.write((char*)&bmp_ih , sizeof(BITMAPINFOHEADER));

	std::vector<std::vector<RGBTRIPLE>>
	matrix(HEIGHT, std::vector<RGBTRIPLE>(WIDTH));

	std::minstd_rand0 our_random(std::chrono::system_clock::now().time_since_epoch().count());

	std::vector<RGBTRIPLE> color_seed(our_random() % 45 + 5);
	for (auto& e : color_seed) {
		e = {
			(BYTE)(our_random() % 256),
			(BYTE)(our_random() % 256),
			(BYTE)(our_random() % 256)
		};
	}

	for (auto& row : matrix) {
		for (auto& e : row) {
			e = color_seed[our_random() & color_seed.size()];
		}
	}

	for (size_t i = 0, j, r ; i < matrix.size() ; i++) {
		for ( j = 0 ; j < matrix[i].size() ; j++) {
			r = our_random() % WEIGHT_RANGE;

			if (r < TLW) {
				if (i > 0 && j > 0) {
					matrix[i][j] = matrix[i-1][j-1];
				}
			} else if (r < TW) {
				if (i > 0) {
					matrix[i][j] = matrix[i-1][j];
				}
			} else if (r < LW) {
				if (j > 0) {
					matrix[i][j] = matrix[i][j-1];
				}
			} else if (r < TRW) {
				if (i > 0 && j < matrix[i].size() - 1) {
					matrix[i][j] = matrix[i-1][j+1];
				}
			} else if (r < RW) {
				if (j < matrix[i].size() - 1) {
					matrix[i][j] = matrix[i][j+1];
				}
			} else if (r < BLW) {
				if (i < matrix.size() - 1 && j > 0) {
					matrix[i][j] = matrix[i+1][j-1];
				}
			} else if (r < BRW) {
				if (i < matrix.size() - 1 && j < matrix[i].size() - 1) {
					matrix[i][j] = matrix[i+1][j+1];
				}
			} else if (r < BW) {
				if (i < matrix.size() - 1) {
					matrix[i][j] = matrix[i+1][j];
				}
			}

			outfile.write((char*)&matrix[i][j], sizeof(RGBTRIPLE));
		}
	}
	outfile.close();

	return 0;
}
