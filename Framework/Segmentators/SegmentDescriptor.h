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

public:
	SegmentDescriptor(std::vector<std::pair<int, int>> segment, Color color);

	const BoundingBox& getBoundingBox() const;

	void setBoundingBox(const BoundingBox& boundingBox);

	int getArea() const;

	void setArea(int area);

	Color getColor() const;

	void setColor(Color color);

	double getWidthHeightRatio() const;

	void setWidthHeightRatio(double widthHeightRatio);

	const std::vector<std::pair<int, int>>& getPoints() const;

	SegmentDescriptor addPoints(SegmentDescriptor& b);
};