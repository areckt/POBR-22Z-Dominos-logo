#include "SegmentDescriptor.h"
#include <cmath>
#include <algorithm>
#include <climits>
#include <iterator>

SegmentDescriptor::SegmentDescriptor(std::vector<std::pair<int, int>> segment, Color color) {
	this->points = segment;
	this->area = segment.size();
	this->color = color;

	int minX = INT_MAX;
	int minY = INT_MAX;
	int maxX = INT_MIN;
	int maxY = INT_MIN;

	for (auto point : segment) {
		int x = point.second;
		int y = point.first;

		minX = std::min(x, minX);
		minY = std::min(y, minY);
		maxX = std::max(x, maxX);
		maxY = std::max(y, maxY);
	}
	int cOGX = 0;
	int cOGY = 0;

	for (auto point : points) {
		cOGX += point.second;
		cOGY += point.first;
	}
	cOGX /= area;
	cOGY /= area;

	this->cOG = std::make_pair(cOGX, cOGY);

	this->boundingBox = BoundingBox(minX, minY, maxX, maxY);

	this->widthHeightRatio = (double)(boundingBox.getWidth()) / (boundingBox.getHeight());
}

const BoundingBox& SegmentDescriptor::getBoundingBox() const {
	return boundingBox;
}

int SegmentDescriptor::getArea() const {
	return area;
}

Color SegmentDescriptor::getColor() const {
	return color;
}

double SegmentDescriptor::getWidthHeightRatio() const {
	return widthHeightRatio;
}

const std::vector<std::pair<int, int>>& SegmentDescriptor::getPoints() const {
	return points;
}

SegmentDescriptor SegmentDescriptor::addPoints(SegmentDescriptor& b) {
	std::vector<std::pair<int, int>> accumulatedPoints;
	std::copy(this->points.begin(), this->points.end(), std::back_inserter(accumulatedPoints));
	std::copy(b.points.begin(), b.points.end(), std::back_inserter(accumulatedPoints));

	std::sort(accumulatedPoints.begin(), accumulatedPoints.end());
	accumulatedPoints.erase(std::unique(accumulatedPoints.begin(), accumulatedPoints.end()), accumulatedPoints.end());
	return SegmentDescriptor(accumulatedPoints, this->color);
}

const std::pair<int, int>& SegmentDescriptor::getCOG() const {
	return cOG;
}