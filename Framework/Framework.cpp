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


void resize(cv::Mat& img, int algorithm);
void reduceColor(cv::Mat& mat);
void solidColorize(cv::Mat& mat, Color color);
void equalizeHistogram(cv::Mat& mat);
void filterLow(cv::Mat& mat);
void filterHigh(cv::Mat& mat);
std::vector<SegmentDescriptor> segmentImage(cv::Mat& image);


int main(int argc, char* argv[]) {

	cv::Mat img = cv::imread("Images/lenna.png", cv::IMREAD_COLOR);

	if (!img.data) {
		std::cout << "Error with image file! \n";
		return -1;
	}
	cv::Mat imgNN = img.clone();
	cv::Mat imgB = img.clone();
	cv::imshow("Original image", img);

	int operationNumber = 1;

	resize(imgB, ScalingAlgorithmType::Bilinear);
	cv::imshow(std::to_string((operationNumber++))+". resized", imgB);

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