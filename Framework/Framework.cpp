#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "const.h"
#include "Scalers/NearestNeighbour.h"
#include "Scalers/Bilinear.h"
#include "Scalers/Scaler.h"


void resize(cv::Mat& img, int algorithm);


int main(int argc, char* argv[]) {

	cv::Mat img = cv::imread("Images/helloWorld.png");

	if (!img.data) {
		std::cout << "Error with image file! \n";
		return -1;
	}
	cv::Mat imgNN = img.clone();
	cv::Mat imgB = img.clone();

	resize(imgNN, ScalingAlgorithmType::NearestNeighbour);
	resize(imgB, ScalingAlgorithmType::Bilinear);
	
	cv::imshow("Original image", img);
	cv::imshow("1. resized (Nearest Neighbour)", imgNN);
	cv::imshow("1. resized (Bilinear)", imgB);
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