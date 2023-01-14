#pragma once

#include <vector>
#include <map>
#include "../const.h"
#include "../Segmentators/SegmentDescriptor.h"

class TraitDetector {

public:
	void detectTraitsAndFilter(std::vector<SegmentDescriptor>& vector, std::map<Color, std::vector<SegmentDescriptor>>& map1);
	std::vector<double> getNorms(SegmentDescriptor descriptor);
	void detectTraits(std::vector<SegmentDescriptor>& segments, std::map<Color, std::vector<SegmentDescriptor>>& bins);
};