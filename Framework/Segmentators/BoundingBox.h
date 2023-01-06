#pragma once

#include <utility>

class BoundingBox {
	int x1;
	int y1;
	int x2;
	int y2;
	int height;
	int width;
public:
	int getHeight() const;

	int getWidth() const;

public:
	BoundingBox() :x1(0), x2(0), y1(0), y2(0), height(0), width(0) {};
	BoundingBox(int x1, int y1, int x2, int y2);

	int getX1() const;

	void setX1(int x1);

	int getY1() const;

	void setY1(int y1);

	int getX2() const;

	void setX2(int x2);

	int getY2() const;

	void setY2(int y2);

	bool isInside(std::pair<int, int> pair1);
};