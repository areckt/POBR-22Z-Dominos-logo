#pragma once

#include <opencv2/core/mat.hpp>

class IFilter {

public:
	virtual ~IFilter() {};

	virtual void filter(cv::Mat& mat) = 0;
};
