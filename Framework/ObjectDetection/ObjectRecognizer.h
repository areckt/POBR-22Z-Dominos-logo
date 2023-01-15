#pragma once

#include <vector>
#include <map>
#include <opencv2/core/mat.hpp>
#include "../const.h"
#include "../Segmentators/SegmentDescriptor.h"

class ObjectRecognizer {
	std::vector<SegmentDescriptor> findRedBluePart(std::map<Color, std::vector<SegmentDescriptor>>& vector1);
	std::vector<SegmentDescriptor> findWhiteCircles(std::vector<SegmentDescriptor> vector1, std::map<Color, std::vector<SegmentDescriptor>>& map);

public:
	std::vector<SegmentDescriptor> recognizeObject(cv::Mat& image, std::map<Color, std::vector<SegmentDescriptor>>& map1);
};