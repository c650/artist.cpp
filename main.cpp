#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <random>
#include <chrono>
#include <functional>

#include <time.h>

#include "./bmp.h"
#include "./bitmap.h"

#define WIDTH 1000
#define HEIGHT WIDTH

#define TLW 36 /* weights for copying color from a different position. */
#define TW 43
#define LW 65
#define TRW 192//68
#define RW 192//75
#define BLW 192//86
#define BRW 192//128
#define BW 192//188
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

	std::minstd_rand0 our_random(std::chrono::system_clock::now().time_since_epoch().count());

	std::vector<RGBTRIPLE> color_seed(our_random() % 45 + 5);
	for (auto& e : color_seed) {
		e = {
			(BYTE)(our_random() % 256),
			(BYTE)(our_random() % 256),
			(BYTE)(our_random() % 256)
		};
	}

	bmp.seed(color_seed, our_random);

	bmp.for_each_pixel([](std::vector<std::vector<RGBTRIPLE>>& matrix, const size_t& i, const size_t& j, std::minstd_rand0& random){
		size_t r = random() % WEIGHT_RANGE;

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
	}, our_random);
	bmp.write_to(outfile);
	outfile.close();

	return 0;
}
