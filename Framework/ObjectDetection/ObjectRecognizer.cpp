#include "ObjectRecognizer.h"
#include "../const.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

std::vector<SegmentDescriptor> ObjectRecognizer::recognizeObject(cv::Mat& image, std::map<Color, std::vector<SegmentDescriptor>>& bins) {
	std::vector<SegmentDescriptor> objects;
	objects = findRedBluePart(bins);
	objects = findWhiteCircles(objects, bins);

	return objects;
}

std::vector<SegmentDescriptor> ObjectRecognizer::findRedBluePart(std::map<Color, std::vector<SegmentDescriptor>>& bins) {
	std::vector<SegmentDescriptor> redBlueObjects;

	for (auto redSegment : bins[Color::RED]) {
		for (auto blueSegment : bins[Color::BLUE]) {
			std::pair<int, int> redCog = redSegment.getCOG();
			std::pair<int, int> blueCog = blueSegment.getCOG();

			double blueWidth = blueSegment.getBoundingBox().getWidth();
			double distance = std::sqrt(std::pow(redCog.first - blueCog.first, 2) + std::pow(redCog.second - blueCog.second, 2));

			if (std::fabs(blueWidth / distance - BLUE_WIDTH_TO_BLUE_RED_DISTANCE_MODEL) < BLUE_WIDTH_TO_BLUE_RED_DISTANCE_THRESHOLD) {
				SegmentDescriptor redBlue = redSegment.addPoints(blueSegment);
				redBlueObjects.push_back(redBlue);
			}
		}
	}
	return redBlueObjects;
}

std::vector<SegmentDescriptor> ObjectRecognizer::findWhiteCircles(std::vector<SegmentDescriptor> objects, std::map<Color, std::vector<SegmentDescriptor>>& bins) {
	std::vector<SegmentDescriptor> redBlueObjectsWithThreeCircles;

	for (auto redBluePart : objects) {
		BoundingBox rbBB = redBluePart.getBoundingBox();
		int count = 0;

		for (const auto& whitePart : bins[Color::WHITE]) {
			BoundingBox whBB = whitePart.getBoundingBox();

			if (whBB.getX1() > rbBB.getX1() &&
				whBB.getX2() < rbBB.getX2() &&
				whBB.getY1() > rbBB.getY1() &&
				whBB.getY2() < rbBB.getY2()) {
				count++;
			}
		}

		if (count == 3) {
			redBlueObjectsWithThreeCircles.push_back(redBluePart);
		}
	}
	return redBlueObjectsWithThreeCircles;
}
