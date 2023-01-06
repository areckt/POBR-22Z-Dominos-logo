#include <algorithm>
#include <iostream>
#include "BoundingBox.h"

int BoundingBox::getX1() const {
	return x1;
}

void BoundingBox::setX1(int x1) {
	BoundingBox::x1 = x1;
}

int BoundingBox::getY1() const {
	return y1;
}

void BoundingBox::setY1(int y1) {
	BoundingBox::y1 = y1;
}

int BoundingBox::getX2() const {
	return x2;
}

void BoundingBox::setX2(int x2) {
	BoundingBox::x2 = x2;
}

int BoundingBox::getY2() const {
	return y2;
}

void BoundingBox::setY2(int y2) {
	BoundingBox::y2 = y2;
}

BoundingBox::BoundingBox(int x1, int y1, int x2, int y2) {
	int minX = std::min(x1, x2);
	int minY = std::min(y1, y2);
	int maxX = std::max(x1, x2);
	int maxY = std::max(y1, y2);

	height = maxY - minY;
	width = maxX - minX;

	this->x1 = minX;
	this->x2 = maxX;
	this->y1 = minY;
	this->y2 = maxY;
}

bool BoundingBox::isInside(std::pair<int, int> point) {

	return point.first > x1 && point.first < x2&& point.second > y1 && point.second < y2;
}

int BoundingBox::getHeight() const {
	return height;
}

int BoundingBox::getWidth() const {
	return width;
}