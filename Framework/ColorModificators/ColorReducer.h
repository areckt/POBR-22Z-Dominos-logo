#pragma once

#include <opencv2/core/mat.hpp>

class ColorReducer {
	static std::array<uchar, 256> createLookupTable();
public:
	void reduce(cv::Mat& mat);
};