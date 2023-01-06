#pragma once

#include <opencv2/core/mat.hpp>
#include "../const.h"

class SolidColorizer {
public:
	void solidColorize(cv::Mat& mat, Color color);
};