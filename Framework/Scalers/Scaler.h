#pragma once

#include <opencv2/core/mat.hpp>

class Scaler {

public:
	virtual ~Scaler() {};

	virtual void scale(cv::Mat& mat) = 0;
};
