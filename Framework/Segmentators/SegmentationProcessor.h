#pragma once

#include <opencv2/core/mat.hpp>
#include "../const.h"

class SegmentationProcessor {
public:
	static bool checkWhite(cv::Vec3b& vec);
	static bool checkBlue(cv::Vec3b& vec);
	static bool checkRed(cv::Vec3b& vec);
};