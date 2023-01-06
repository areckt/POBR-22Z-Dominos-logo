#include <stack>
#include <opencv2/opencv.hpp>
#include "SegmentationProcessor.h"
#include "SegmentDescriptor.h"
#include "../const.h"


std::vector<SegmentDescriptor> SegmentationProcessor::segmentImage(cv::Mat& mat) {
	StateMat states = initializeStateMat(mat);
	ColorMat matOfColors = toColors(mat);
	SegmentVector listOfSegments = segmentation(matOfColors, states);

	listOfSegments = filterSegments(listOfSegments);
	showSegments(listOfSegments, mat, matOfColors);
	std::vector<SegmentDescriptor> descriptors = toDescriptors(listOfSegments, matOfColors);
	return descriptors;
}

StateMat SegmentationProcessor::initializeStateMat(cv::Mat& mat) {
	StateMat states;

	for (int i = 0; i < mat.cols; i++) {
		std::vector<State> s;
		for (int j = 0; j < mat.rows; j++) {
			s.push_back(UNVISITED);
		}
		states.push_back(s);
	}

	return states;
}

SegmentVector
SegmentationProcessor::segmentation(ColorMat& mat,
	StateMat& stateMat) {
	SegmentVector segments;
	int matHeight = mat[0].size();
	for (int i = 1; i < matHeight - 1; i++) {
		for (int j = 1; j < mat.size() - 1; j++) {
			Color color = mat[j][i];
			State state = stateMat[j][i];

			if (color == OTHER) {
				stateMat[j][i] = MISSED;
				continue;
			}

			if (state != UNVISITED) {
				continue;
			}

			Color seed = color;
			std::vector<std::pair<int, int>> segment;
			std::stack<std::pair<int, int>> pointsStack;
			pointsStack.push(std::make_pair(j, i));
			while (!pointsStack.empty()) {
				std::pair<int, int> point = pointsStack.top();
				State pointState = stateMat[point.first][point.second];
				Color pointColor = mat[point.first][point.second];

				if (pointColor != seed || pointState == ADDED) {
					pointsStack.pop();
					continue;
				}

				stateMat[point.first][point.second] = ADDED;
				segment.push_back(point);
				for (int x = 0; x < 3; x++) {
					for (int y = 0; y < 3; y++) {
						if (x == 1 && y == 1) {
							continue;
						}

						int neighbourX = point.second + x - 1;
						int neighbourY = point.first + y - 1;

						if (neighbourX < 0 || neighbourY < 0 || neighbourX >= matHeight || neighbourY >= mat.size()) {
							continue;
						}

						pointsStack.push(std::make_pair(neighbourY, neighbourX));
					}
				}

				pointsStack.pop();
			}
			segments.push_back(segment);
		}
	}

	return segments;
}

SegmentVector
SegmentationProcessor::filterSegments(SegmentVector segmentVector) {
	SegmentVector result;
	for (auto segment : segmentVector) {
		if (segment.size() > SEGMENT_SIZE_THRESHOLD) {
			result.push_back(segment);
		}
	}

	return result;
}

std::vector<SegmentDescriptor>
SegmentationProcessor::toDescriptors(SegmentVector segmentVector, ColorMat colorMat) {
	std::vector<SegmentDescriptor> descriptors;
	for (auto segment : segmentVector) {
		int exampleY = segment.front().first;
		int exampleX = segment.front().second;

		SegmentDescriptor segmentDescriptor = SegmentDescriptor(segment, colorMat[exampleY][exampleX]);
		if (segmentDescriptor.getBoundingBox().getWidth() < 10 || segmentDescriptor.getBoundingBox().getHeight() < 10) {
			continue;
		}

		descriptors.push_back(segmentDescriptor);
	}

	return descriptors;
}

typedef struct {
	uchar r;	// a fraction between 0 and 1
	uchar g;	// a fraction between 0 and 1
	uchar b;	// a fraction between 0 and 1

} rgb;

typedef struct {
	uchar h;	// angle in degrees
	uchar s;	// a fraction between 0 and 1
	uchar v;	// a fraction between 0 and 1
} hsv;

static hsv rgb2hsv(rgb in);

hsv rgb2hsv(rgb rgb)
{
	hsv hsv;
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return hsv;
	}

	if (rgbMax == rgb.r)
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return hsv;
}

typedef struct {
	int h;		// angle in degrees
	float s;	// a fraction between 0 and 1
	float l;	// a fraction between 0 and 1
} hsl;

static hsl RGBToHSL(rgb rgb) {
	hsl hsl;
	hsl.h = 0;
	hsl.s = 0;
	hsl.l = 0;


	float r = (rgb.r / 255.0f);
	float g = (rgb.g / 255.0f);
	float b = (rgb.b / 255.0f);

	float min = std::min(std::min(r, g), b);
	float max = std::max(std::max(r, g), b);
	float delta = max - min;

	hsl.l = (max + min) / 2;

	if (delta == 0)
	{
		hsl.h = 0;
		hsl.s = 0.0f;
	}
	else
	{
		hsl.s = (hsl.l <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

		float hue;

		if (r == max)
		{
			hue = ((g - b) / 6) / delta;
		}
		else if (g == max)
		{
			hue = (1.0f / 3) + ((b - r) / 6) / delta;
		}
		else
		{
			hue = (2.0f / 3) + ((r - g) / 6) / delta;
		}

		if (hue < 0)
			hue += 1;
		if (hue > 1)
			hue -= 1;

		hsl.h = (int)(hue * 360);
	}

	return hsl;
}

ColorMat SegmentationProcessor::toColors(cv::Mat& mat) {
	ColorMat colors;
	for (int i = 0; i < mat.cols; i++) {
		std::vector<Color> column;
		for (int j = 0; j < mat.rows; j++) {
			if (checkRed(mat.at<cv::Vec3b>(j, i))) {
				column.push_back(RED);
			}
			else if (checkBlue(mat.at<cv::Vec3b>(j, i))) {
				column.push_back(BLUE);
			}
			else if (checkWhite(mat.at<cv::Vec3b>(j, i))) {
				column.push_back(WHITE);
			}
			else {
				column.push_back(OTHER);
			}
		}
		colors.push_back(column);
	}

	return colors;
}

bool SegmentationProcessor::checkWhite(cv::Vec3b& vec) {

	rgb rgb1;
	rgb1.r = vec[2];
	rgb1.g = vec[1];
	rgb1.b = vec[0];
	hsl hsl = RGBToHSL(rgb1);

	if (hsl.l >= 0.79 || (hsl.l > 0.72 && hsl.s < 0.35)) {
		return true;
	}
	return false;
}

bool SegmentationProcessor::checkBlue(cv::Vec3b& vec) {
	rgb rgb1;
	rgb1.r = vec[2];
	rgb1.g = vec[1];
	rgb1.b = vec[0];
	hsl hsl = RGBToHSL(rgb1);

	if (hsl.h < 245 && hsl.h > 180 && hsl.s > 0.35 && hsl.l < 0.79 && hsl.l > 0.22) {
		return true;
	}
	return false;
}

bool SegmentationProcessor::checkRed(cv::Vec3b& vec) {

	rgb rgb1;
	rgb1.r = vec[2];
	rgb1.g = vec[1];
	rgb1.b = vec[0];
	hsl hsl = RGBToHSL(rgb1);
	if ((hsl.h < 15 || hsl.h > 315) && hsl.s > 0.43 && hsl.l > 0.32 && hsl.l < 0.79) {
		return true;
	}
	return false;
}

void SegmentationProcessor::showSegments(SegmentVector vector1, cv::Mat& mat, ColorMat& colorMat) {
	cv::Mat_<cv::Vec3b> filtered = mat.clone();
	for (int i = 0; i < RESIZED_WIDTH; i++) {
		for (int j = 0; j < RESIZED_HEIGHT; j++) {
			filtered[j][i] = cv::Vec3b((uchar)0, (uchar)0, (uchar)0);
		}
	}
	std::for_each(vector1.begin(), vector1.end(), [&](std::vector<std::pair<int, int>> segment) {

		std::for_each(segment.begin(), segment.end(), [&](std::pair<int, int> point) {
			int x = point.first;
			int y = point.second;

			switch (colorMat[x][y]) {
			case RED:
				filtered[y][x] = cv::Vec3b((uchar)0, (uchar)0, (uchar)255);
				break;
			case BLUE:
				filtered[y][x] = cv::Vec3b((uchar)255, (uchar)0, (uchar)0);
				break;
			case WHITE:
				filtered[y][x] = cv::Vec3b((uchar)255, (uchar)255, (uchar)255);
				break;
			default:
				break;
			}
		});
	});

	cv::imshow("segmentation result", filtered);
}