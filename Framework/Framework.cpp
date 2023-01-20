#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "const.h"
#include "PreProcessing/Resizers.h"
#include "PreProcessing/Filters.h"
#include "PreProcessing/ColorReducer.h"
#include "PreProcessing/SolidColorizer.h"
#include "PreProcessing/HistogramEqualizer.h"
#include "Segmentators/SegmentationProcessor.h"
#include "Segmentators/SegmentDescriptor.h"
#include "ObjectDetection/TraitDetector.h"
#include "ObjectDetection/ObjectRecognizer.h"


void resize(cv::Mat& img, int algorithm);
void filter(cv::Mat& img, int type);
void reduceColor(cv::Mat& mat);
void solidColorize(cv::Mat& mat, Color color);
void equalizeHistogram(cv::Mat& mat);
std::vector<SegmentDescriptor> segmentImage(cv::Mat& image);
std::map<Color, std::vector<SegmentDescriptor>> detectTraits(std::vector<SegmentDescriptor>& image);
std::vector<SegmentDescriptor> recognizeObjects(cv::Mat& ogImage, std::map<Color, std::vector<SegmentDescriptor>> bins);
void drawBoundingBox(cv::Mat& image, std::vector<SegmentDescriptor> logos, cv::Vec3b color);
void drawSegmentBoundary(cv::Mat& image, BoundingBox bb, cv::Vec3b color);


int main(int argc, char* argv[]) {

	const std::string FILE_NAME = "dominos1";
	cv::Mat img = cv::imread("Images/" + FILE_NAME + ".png", cv::IMREAD_COLOR);

	if (!img.data) {
		std::cout << "Error with image file! \n";
		return -1;
	}
	cv::Mat imgB = img.clone();
	// cv::imshow("Original image", img);
	int operationNumber = 0;


	/*    ___                 ___                             _
	*	 | _ \ _ _  ___  ___ | _ \ _ _  ___  __  ___  ___ ___(_) _ _   __ _
	*	 |  _/| '_|/ -_)|___||  _/| '_|/ _ \/ _|/ -_)(_-<(_-<| || ' \ / _` |
	*	 |_|  |_|  \___|     |_|  |_|  \___/\__|\___|/__//__/|_||_||_|\__, |
	*																  |___/
	*/
	resize(imgB, ScalingAlgorithmType::Bilinear);
	cv::imshow(std::to_string((++operationNumber)) + ". resizedB", imgB);
	cv::imwrite("./Results/" + FILE_NAME + std::to_string((operationNumber)) + "resized.png", imgB);

	cv::Mat resized = imgB.clone();

	//equalizeHistogram(imgB);
	//cv::imshow(std::to_string((operationNumber++)) + ". equalized histogram", imgB);

	//reduceColor(imgB);
	//cv::imshow(std::to_string((operationNumber++)) + ". reduced color", imgB);

	filter(imgB, FilteringType::LowPass);
	cv::imshow(std::to_string((operationNumber++)) + ". lowpass", imgB);
	cv::imwrite("./Results/" + FILE_NAME + std::to_string((operationNumber)) + "lowpass.png", imgB);

	filter(imgB, FilteringType::HighPass);
	cv::imshow(std::to_string((operationNumber++)) + ". highpass", imgB);
	cv::imwrite("./Results/" + FILE_NAME + std::to_string((operationNumber)) + "highpass.png", imgB);

	/*solidColorize(imgB, Color::BLUE);
	solidColorize(imgB, Color::RED);
	solidColorize(imgB, Color::WHITE);
	cv::imshow(std::to_string((operationNumber++)) + ". solidColorized", imgB);*/



	/*	  ___                         _        _   _                         _
	*	 / __| ___ __ _ _ __  ___ _ _| |_ __ _| |_(_)___ _ _    __ _ _ _  __| |
	*	 \__ \/ -_) _` | '  \/ -_) ' \  _/ _` |  _| / _ \ ' \  / _` | ' \/ _` |
	*	 |___/\___\__, |_|_|_\___|_||_\__\__,_|\__|_\___/_||_| \__,_|_||_\__,_|
	*	  _____   |___/ _ _     ___      _          _   _
	*	 |_   _| _ __ _(_) |_  |   \ ___| |_ ___ __| |_(_)___ _ _
	*	   | || '_/ _` | |  _| | |) / -_)  _/ -_) _|  _| / _ \ ' \
	*	   |_||_| \__,_|_|\__| |___/\___|\__\___\__|\__|_\___/_||_|
	*/
	std::vector<SegmentDescriptor> segments = segmentImage(imgB);
	cv::Mat allSegmentsBb = resized.clone();
	drawBoundingBox(allSegmentsBb, segments, cv::Vec3b(255, 0, 255));
	cv::imshow(std::to_string((operationNumber++)) + ". all segments", allSegmentsBb);
	cv::imwrite("./Results/" + FILE_NAME + std::to_string((operationNumber)) + "allSegments.png", allSegmentsBb);

	std::map<Color, std::vector<SegmentDescriptor>> bins = detectTraits(segments);

	std::cout << "\n\nBounding boxes of found segments:\n";
	cv::Mat segmentsBb = resized.clone();
	for (auto it = bins.cbegin(); it != bins.cend(); ++it)
	{
		drawBoundingBox(segmentsBb, it->second, cv::Vec3b(0, 0, 0));
		for (auto jt = it->second.cbegin(); jt != it->second.cend(); ++jt) {
			std::cout << jt->getColor() << " " <<
				"X1: " << jt->getBoundingBox().getX1() << " " <<
				"Y1: " << jt->getBoundingBox().getY1() << " " <<
				"X2: " << jt->getBoundingBox().getX2() << " " <<
				"Y2: " << jt->getBoundingBox().getY2() << '\n';
		}
	}
	cv::imshow(std::to_string((operationNumber++)) + ". segments that meet the criteria", segmentsBb);
	cv::imwrite("./Results/" + FILE_NAME + std::to_string((operationNumber)) + "segmentsThatMeetCriteria.png", segmentsBb);

	std::cout << "\n\nBounding boxes of found logos:\n";
	std::vector<SegmentDescriptor> logos = recognizeObjects(img, bins);
	for (auto logo : logos) {
		std::cout << "X1: " << logo.getBoundingBox().getX1() << '\t' <<
			"Y1: " << logo.getBoundingBox().getY1() << '\t' <<
			"X2: " << logo.getBoundingBox().getX2() << '\t' <<
			"Y2: " << logo.getBoundingBox().getY2() << '\n';
	}


	/*	  ___                   _ _             ___
	*	 | _ ) ___ _  _ _ _  __| (_)_ _  __ _  | _ ) _____ _____ ___
	*	 | _ \/ _ \ || | ' \/ _` | | ' \/ _` | | _ \/ _ \ \ / -_|_-<
	*	 |___/\___/\_,_|_||_\__,_|_|_||_\__, | |___/\___/_\_\___/__/
	*									|___/
	*/
	drawBoundingBox(resized, logos, cv::Vec3b(34, 255, 34));
	cv::imshow(std::to_string((operationNumber++)) + ". result", resized);
	cv::imwrite("./Results/" + FILE_NAME + std::to_string((operationNumber)) + "finalResult.png", resized);

	cv::waitKey(-1);
	return 0;
}

void resize(cv::Mat& img, int algorithm) {
	switch (algorithm) {
	case ScalingAlgorithmType::NearestNeighbour:
		resizeNearestNeighbour(img);
		break;
	case ScalingAlgorithmType::Bilinear:
		resizeBilinear(img);
		break;
	default:
		throw std::runtime_error("Wrong input!");
	}
}

void filter(cv::Mat& img, int type) {
	switch (type) {
	case FilteringType::LowPass:
		filterLow(img);
		break;
	case FilteringType::HighPass:
		filterHigh(img);
		break;
	default:
		throw std::runtime_error("Wrong input!");
	}
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

void drawBoundingBox(cv::Mat& image, std::vector<SegmentDescriptor> logos, cv::Vec3b color) {
	for (auto logo : logos) {
		BoundingBox bb = logo.getBoundingBox();
		drawSegmentBoundary(image, bb, color);
	};
}

void drawSegmentBoundary(cv::Mat& image, BoundingBox bb, cv::Vec3b color) {
	const cv::Vec3b line = color;

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