#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "const.h"
#include "Scalers/NearestNeighbour.h"
#include "Scalers/Bilinear.h"
#include "Scalers/Scaler.h"
#include "ColorModificators/ColorReducer.h"
#include "ColorModificators/SolidColorizer.h"
#include "ColorModificators/HistogramEqualizer.h"
#include "Filters/Lowpass.h"
#include "Filters/Highpass.h"
#include "Segmentators/SegmentationProcessor.h"
#include "Segmentators/SegmentDescriptor.h"
#include "ObjectDetection/TraitDetector.h"
#include "ObjectDetection/ObjectRecognizer.h"


void resize(cv::Mat& img, int algorithm);
void reduceColor(cv::Mat& mat);
void solidColorize(cv::Mat& mat, Color color);
void equalizeHistogram(cv::Mat& mat);
void filterLow(cv::Mat& mat);
void filterHigh(cv::Mat& mat);
std::vector<SegmentDescriptor> segmentImage(cv::Mat& image);
std::map<Color, std::vector<SegmentDescriptor>> detectTraits(std::vector<SegmentDescriptor>& image);
std::vector<SegmentDescriptor> recognizeObjects(cv::Mat& ogImage, std::map<Color, std::vector<SegmentDescriptor>> bins);
void drawBoundingBox(cv::Mat& image, std::vector<SegmentDescriptor> objects);
void drawSegmentBoundary(cv::Mat& image, BoundingBox bb);


int main(int argc, char* argv[]) {

	cv::Mat img = cv::imread("Images/dominos1.png", cv::IMREAD_COLOR);

	if (!img.data) {
		std::cout << "Error with image file! \n";
		return -1;
	}
	cv::Mat imgB = img.clone();
	cv::imshow("Original image", img);

	int operationNumber = 1;

	resize(imgB, ScalingAlgorithmType::Bilinear);
	cv::imshow(std::to_string((operationNumber++)) + ". resized", imgB);
	cv::Mat resized = imgB.clone();

	//equalizeHistogram(imgB);
	//cv::imshow(std::to_string((operationNumber++)) + ". equalized histogram", imgB);

	//reduceColor(imgB);
	//cv::imshow(std::to_string((operationNumber++)) + ". reduced color", imgB);

	filterLow(imgB);
	filterLow(imgB);
	cv::imshow(std::to_string((operationNumber++)) + ". lowpass", imgB);

	filterHigh(imgB);
	cv::imshow(std::to_string((operationNumber++)) + ". highpass", imgB);

	/*solidColorize(imgB, Color::BLUE);
	solidColorize(imgB, Color::RED);
	solidColorize(imgB, Color::WHITE);
	cv::imshow(std::to_string((operationNumber++)) + ". solidColorized", imgB);*/

	std::vector<SegmentDescriptor> segments = segmentImage(imgB);
	std::map<Color, std::vector<SegmentDescriptor>> bins = detectTraits(segments);

	std::cout << '\n';
	std::cout << '\n';
	for (auto it = bins.cbegin(); it != bins.cend(); ++it)
	{
		for (auto jt = it->second.cbegin(); jt != it->second.cend(); ++jt) {
			std::cout << jt->getColor() << " " <<
				"X1: " << jt->getBoundingBox().getX1() << " " <<
				"Y1: " << jt->getBoundingBox().getY1() << " " <<
				"X2: " << jt->getBoundingBox().getX2() << " " <<
				"Y2: " << jt->getBoundingBox().getY2() << '\n';
		}
	}

	std::cout << '\n';
	std::cout << '\n';
	std::vector<SegmentDescriptor> objects = recognizeObjects(img, bins);
	for (auto logo : objects) {
		std::cout << "X1: " << logo.getBoundingBox().getX1() << '\t' <<
			"Y1: " << logo.getBoundingBox().getY1() << '\t' <<
			"X2: " << logo.getBoundingBox().getX2() << '\t' <<
			"Y2: " << logo.getBoundingBox().getY2() << '\n';
	}

	drawBoundingBox(resized, objects);
	cv::imshow("result", resized);

	cv::waitKey(-1);
	return 0;
}

void resize(cv::Mat& img, int algorithm) {

	Scaler* scaler;
	switch (algorithm) {
	case ScalingAlgorithmType::NearestNeighbour:
		scaler = new Scalers::NearestNeighbour();
		break;
	case ScalingAlgorithmType::Bilinear:
		scaler = new Scalers::Bilinear();
		break;
	default:
		throw std::runtime_error("Wrong input!");
	}

	scaler->scale(img);
}

void reduceColor(cv::Mat& mat) {
	ColorReducer colorReducer = ColorReducer();
	colorReducer.reduce(mat);
}

void solidColorize(cv::Mat& mat, Color color) {
	SolidColorizer solidColorizer = SolidColorizer();
	solidColorizer.solidColorize(mat, color);
}

void equalizeHistogram(cv::Mat& mat) {
	HistogramEqualizer histogramEqualizer = HistogramEqualizer();
	histogramEqualizer.equalize(mat);
}

void filterLow(cv::Mat& image) {
	IFilter* filter;
	filter = new Filters::Lowpass();
	filter->filter(image);
}

void filterHigh(cv::Mat& image) {
	IFilter* filter;
	filter = new Filters::Highpass();
	filter->filter(image);
}

std::vector<SegmentDescriptor> segmentImage(cv::Mat& image) {
	SegmentationProcessor processor;
	std::vector<SegmentDescriptor> descriptors = processor.segmentImage(image);
	return descriptors;
}

std::map<Color, std::vector<SegmentDescriptor>> detectTraits(std::vector<SegmentDescriptor>& segments) {
	TraitDetector detector;
	std::map<Color, std::vector<SegmentDescriptor>> bins = {
			{Color::RED, {} },
			{Color::BLUE, {} },
			{Color::WHITE, {} },
	};
	detector.detectTraitsAndFilter(segments, bins);
	return bins;
}

std::vector<SegmentDescriptor> recognizeObjects(cv::Mat& ogImage, std::map<Color, std::vector<SegmentDescriptor>> bins) {
	ObjectRecognizer recognizer;
	std::vector<SegmentDescriptor> objects = recognizer.recognizeObject(ogImage, bins);
	return objects;
}

void drawBoundingBox(cv::Mat& image, std::vector<SegmentDescriptor> objects) {
	for (auto object : objects) {
		BoundingBox bb = object.getBoundingBox();
		drawSegmentBoundary(image, bb);
	};
}

void drawSegmentBoundary(cv::Mat& image, BoundingBox bb) {
	const cv::Vec3b line = { 0, 255, 0 };

	// draw vertical
	for (auto i = bb.getX1(); i < bb.getX1() + bb.getWidth(); ++i) {
		if (i - 1 < 0 || i + 1 > image.rows || bb.getY1() - 1 < 0 || bb.getY1() + 1 > image.cols) {
			continue;
		}
		image.at<cv::Vec3b>(i, bb.getY1()) = line;
		image.at<cv::Vec3b>(i, bb.getY1() + bb.getHeight()) = line;
	}

	// draw horizontal
	for (auto i = bb.getY1(); i < bb.getY1() + bb.getHeight(); ++i) {
		if (i - 1 < 0 || i + 1 > image.cols || bb.getX1() - 1 < 0 || bb.getX1() + 1 > image.rows) {
			continue;
		}
		image.at<cv::Vec3b>(bb.getX1() + 1, i) = line;
		image.at<cv::Vec3b>(bb.getX1() + bb.getWidth(), i) = line;
	}

	return;
}