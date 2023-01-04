#pragma once

#include <opencv2/core/mat.hpp>

class HistogramEqualizer {
	std::array<int, 256> createLookuptable(std::array<int, 256> histogram, int pixelCount);

	void applyLUT(cv::Mat& image, std::array<int, 256> lut, int color);

	static std::array<int, 256> createHistogram(cv::Mat& image, int color);
public:
	void equalize(cv::Mat& mat);

};