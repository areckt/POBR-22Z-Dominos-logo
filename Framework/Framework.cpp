#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "const.h"
#include "Scalers/NearestNeighbour.h"
#include "Scalers/Bilinear.h"
#include "Scalers/Scaler.h"
#include "ColorModificators/ColorReducer.h"
#include "ColorModificators/SolidColorizer.h"


void resize(cv::Mat& img, int algorithm);
void reduceColor(cv::Mat& mat);
void solidColorize(cv::Mat& mat, colors color);


int main(int argc, char* argv[]) {

	cv::Mat img = cv::imread("Images/lenna.png", cv::IMREAD_COLOR);

	if (!img.data) {
		std::cout << "Error with image file! \n";
		return -1;
	}
	cv::Mat imgNN = img.clone();
	cv::Mat imgB = img.clone();

	//resize(imgNN, ScalingAlgorithmType::NearestNeighbour);
	resize(imgB, ScalingAlgorithmType::Bilinear);
	
	cv::imshow("Original image", img);
	//cv::imshow("1. resized (Nearest Neighbour)", imgNN);
	cv::imshow("1. resized (Bilinear)", imgB);

	//reduceColor(imgNN);
	reduceColor(imgB);

	//cv::imshow("2. reduced color (Nearest Neighbour)", imgNN);
	cv::imshow("2. reduced color (Bilinear)", imgB);

	solidColorize(imgB, colors::Blue);
	solidColorize(imgB, colors::Red);
	solidColorize(imgB, colors::White);

	//cv::imshow("3. SolidColorized (Nearest Neighbour)", imgNN);
	cv::imshow("3. SolidColorized (Bilinear)", imgB);

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

void solidColorize(cv::Mat& mat, colors color) {
	SolidColorizer solidColorizer = SolidColorizer();
	solidColorizer.solidColorize(mat, color);
}