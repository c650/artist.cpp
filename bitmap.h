#ifndef BITMAP_H
#define BITMAP_H

#include "./bmp.h"

#include <vector>
#include <functional>
#include <fstream>
#include <random>


class Bitmap {

	BITMAPFILEHEADER bmp_fh;
	BITMAPINFOHEADER bmp_ih;
	std::vector<std::vector<RGBTRIPLE>> matrix;

public:
	Bitmap();

	Bitmap(DWORD height, DWORD width);

	void write_to(std::fstream& outfile);

	template <class RandomGenerator>
	void seed(const std::vector<RGBTRIPLE>& colors, RandomGenerator& r) {
		for (auto& row : matrix)
			for (auto& e : row)
				e = colors[ r() % colors.size() ];
	}

	template <typename Func, typename RandomGenerator>
	void for_each_pixel(Func func, RandomGenerator& random) {
		for (size_t i = 0, j; i < matrix.size(); i++)
			for (j = 0; j < matrix[i].size(); j++)
				func(matrix , i , j, random);
	}

private:
	void init();


};

#endif
