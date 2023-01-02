#include "NearestNeighbour.h"
#include "../const.h"
#include <algorithm>

void Scalers::NearestNeighbour::scale(cv::Mat& mat) {
	double widthRatio = mat.cols / (double)RESIZED_WIDTH;
	double heightRatio = mat.rows / (double)RESIZED_HEIGHT;

	cv::Mat resized(RESIZED_HEIGHT, RESIZED_WIDTH, mat.type());
	cv::Mat_<cv::Vec3b> resized_(resized);

	for (int i = 0; i < RESIZED_WIDTH; i++) {
		for (int j = 0; j < RESIZED_HEIGHT; j++) {

			int x = std::min((int)round(i * widthRatio), mat.cols - 1);
			int y = std::min((int)round(j * heightRatio), mat.rows - 1);

			resized_[j][i] = mat.at<cv::Vec3b>(y, x);
		}
	}
	mat = resized;
}