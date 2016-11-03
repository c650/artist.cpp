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

#define TLW weights[0] /* weights for copying color from a different position. */
#define TW weights[1]
#define LW weights[2]
#define TRW weights[3]
#define RW weights[4]
#define BLW weights[5]
#define BRW weights[6]
#define BW weights[7]
#define WEIGHT_RANGE 1024

static std::vector<unsigned int> weights(8,0);

static void make_weights(std::minstd_rand0& random);
static void store(const Bitmap& bmp , const char* path);
static void seed_colors(Bitmap& bmp, std::minstd_rand0& random);

int main(int argc , char **argv) {

	if (argc < 2) {
		std::cout << "No outfile specified!\n";
		std::exit(1);
	}

	std::minstd_rand0 our_random(std::chrono::system_clock::now().time_since_epoch().count());
	make_weights(our_random);

	Bitmap bmp(HEIGHT, WIDTH);
	seed_colors(bmp, our_random);
	bmp.for_each_pixel([](std::vector<std::vector<RGBTRIPLE>>& matrix, const size_t& i, const size_t& j, std::minstd_rand0& random){
		size_t r = random() % WEIGHT_RANGE;

		fkljfad(); /* must fix this logic*/
		if (r < TLW && i > 0 && j > 0) {
			matrix[i][j] = matrix[i-1][j-1];
		} else if (r < TW && i > 0) {
			matrix[i][j] = matrix[i-1][j];
		} else if (r < LW && j > 0) {
			matrix[i][j] = matrix[i][j-1];
		} else if (r < TRW && i > 0 && j < matrix[i].size() - 1) {
			matrix[i][j] = matrix[i-1][j+1];
		} else if (r < RW && j < matrix[i].size() - 1) {
			matrix[i][j] = matrix[i][j+1];
		} else if (r < BLW && i < matrix.size() - 1 && j > 0) {
			matrix[i][j] = matrix[i+1][j-1];
		} else if (r < BRW && i < matrix.size() - 1 && j < matrix[i].size() - 1) {
			matrix[i][j] = matrix[i+1][j+1];
		} else if (r < BW && i < matrix.size() - 1) {
			matrix[i][j] = matrix[i+1][j];
		}
	}, our_random);

	store(bmp, argv[1]);
	if (argc >= 3) {
		Bitmap bmp2 = bmp;
		bmp2.process();
		store(bmp2, argv[2]);
	}

	return 0;
}

static void make_weights(std::minstd_rand0& random) {
	weights[0] = random() % (WEIGHT_RANGE / 8) + 1;
	for (size_t i = 1 ; i < weights.size() ; i++)
		weights[i] = weights[i-1] + random() % (WEIGHT_RANGE) + 1;

	for (auto& w : weights)
		std::cout << w << " ";
	std::cout << std::endl;
}

static void store(const Bitmap& bmp , const char* path) {
	std::fstream outfile(path,
						 std::fstream::out
						 | std::fstream::trunc
						 | std::fstream::binary);

	if (!outfile.is_open()) {
		std::cout << "Could not open " << path << "!\n";
		std::exit(2);
	}
	bmp.write_to(outfile);
	outfile.close();
}

static void seed_colors(Bitmap& bmp, std::minstd_rand0& random) {
	std::vector<RGBTRIPLE> color_seed(random() % 45 + 5);
	for (auto& e : color_seed) {
		e = {
			(BYTE)(random() % 256),
			(BYTE)(random() % 256),
			(BYTE)(random() % 256)
		};
	}
	bmp.seed(color_seed, random);
}
