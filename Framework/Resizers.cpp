#include "Resizers.h"
#include "const.h"
#include <algorithm>

void resizeNearestNeighbour(cv::Mat& mat) {
	double widthRatio = mat.cols / (double)RESIZED_WIDTH;
	double heightRatio = mat.rows / (double)RESIZED_HEIGHT;

	cv::Mat resized(RESIZED_HEIGHT, RESIZED_WIDTH, mat.type());

	for (int i = 0; i < RESIZED_WIDTH; i++) {
		for (int j = 0; j < RESIZED_HEIGHT; j++) {
			int x = std::min((int)round(i * widthRatio), mat.cols - 1);
			int y = std::min((int)round(j * heightRatio), mat.rows - 1);

			resized.at<cv::Vec3b>(j, i) = mat.at<cv::Vec3b>(y, x);
		}
	}
	mat = resized;
}

void resizeBilinear(cv::Mat& mat) {
	double widthRatio = mat.cols / (double)RESIZED_WIDTH;
	double heightRatio = mat.rows / (double)RESIZED_HEIGHT;

	cv::Mat resized(RESIZED_HEIGHT, RESIZED_WIDTH, mat.type());

	for (int i = 0; i < RESIZED_WIDTH; i++) {
		for (int j = 0; j < RESIZED_HEIGHT; j++) {
			int x = i * widthRatio;
			int y = j * heightRatio;

			int x1 = std::min(x, mat.cols - 1);
			int y1 = std::min(y, mat.rows - 1);
			int x2 = std::min(x + 1, mat.cols - 1);
			int y2 = std::min(y + 1, mat.rows - 1);

			for (int k = 0; k < 3; k++) {
				uchar colorX1Y1 = mat.at<cv::Vec3b>(y1, x1)[k];
				uchar colorX1Y2 = mat.at<cv::Vec3b>(y2, x1)[k];
				uchar colorX2Y1 = mat.at<cv::Vec3b>(y1, x2)[k];
				uchar colorX2Y2 = mat.at<cv::Vec3b>(y2, x2)[k];

				double interpolation1 = (x2 - x) * colorX1Y1 + (x - x1) * colorX2Y1;
				double interpolation2 = (x2 - x) * colorX1Y2 + (x - x1) * colorX2Y2;

				if (x1 == x2) {
					interpolation1 = colorX1Y1;
					interpolation2 = colorX2Y2;
				}

				uchar finalInterpolation = std::max(std::min((int)((y2 - y) * interpolation1 + (y - y1) * interpolation2), 255), 0);

				resized.at<cv::Vec3b>(j, i)[k] = finalInterpolation;
			}
		}
	}
	mat = resized;
}