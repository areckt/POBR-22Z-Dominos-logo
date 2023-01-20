#pragma once

#include <vector>
#include "../const.h"
#include "BoundingBox.h"

class SegmentDescriptor {
private:
	BoundingBox boundingBox;
	int area;
	Color color;
	double widthHeightRatio;
	std::vector<std::pair<int, int>> points;
	std::pair<int, int> cOG;

public:
	const std::pair<int, int>& getCOG() const;
	SegmentDescriptor(std::vector<std::pair<int, int>> segment, Color color);
	const BoundingBox& getBoundingBox() const;
	int getArea() const;
	Color getColor() const;
	double getWidthHeightRatio() const;
	const std::vector<std::pair<int, int>>& getPoints() const;
	SegmentDescriptor addPoints(SegmentDescriptor& b);
};