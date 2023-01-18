#include "Filters.h"
#include "../const.h"

void filterLow(cv::Mat& mat) {
	const int KERNEL_SIZE = 5;
	const int KERNEL_SIZE_SQUARED = KERNEL_SIZE * KERNEL_SIZE;
	cv::Mat filtered = mat.clone();

	for (int i = 0; i < RESIZED_WIDTH; i++) {
		for (int j = 0; j < RESIZED_HEIGHT; j++) {
			int x = i;
			int y = j;

			if (x < KERNEL_SIZE / 2 || y < KERNEL_SIZE / 2 ||
				x >= RESIZED_WIDTH - KERNEL_SIZE / 2 || y >= RESIZED_HEIGHT - KERNEL_SIZE / 2) {
				continue;
			}

			for (int k = 0; k < 3; k++) {
				double temp = 0;
				for (int l = 0; l < KERNEL_SIZE; l++) {
					for (int m = 0; m < KERNEL_SIZE; m++) {
						temp += mat.at<cv::Vec3b>(y - KERNEL_SIZE / 2 + m, x - KERNEL_SIZE / 2 + l)[k];
					}
				}
				filtered.at<cv::Vec3b>(j, i)[k] = (uchar)(temp / KERNEL_SIZE_SQUARED);
			}
		}
	}
	mat = filtered;
}

void filterHigh(cv::Mat& mat) {
	const int KERNEL_SIZE = 3;

	cv::Mat kernel = (cv::Mat_<double>(KERNEL_SIZE, KERNEL_SIZE) << -1, -1, -1,
		-1, 9, -1,
		-1, -1, -1);
	cv::Mat filtered = mat.clone();

	for (int i = 0; i < RESIZED_WIDTH; i++) {
		for (int j = 0; j < RESIZED_HEIGHT; j++) {
			int x = i;
			int y = j;

			if (x < KERNEL_SIZE / 2 || y < KERNEL_SIZE / 2 ||
				x >= RESIZED_WIDTH - KERNEL_SIZE / 2 || y >= RESIZED_HEIGHT - KERNEL_SIZE / 2) {
				continue;
			}

			for (int k = 0; k < 3; k++) {
				double temp = 0;
				for (int l = 0; l < KERNEL_SIZE; l++) {
					for (int m = 0; m < KERNEL_SIZE; m++) {
						temp += kernel.at<double>(m, l) *
							mat.at<cv::Vec3b>(y - KERNEL_SIZE / 2 + m, x - KERNEL_SIZE / 2 + l)[k];
					}
				}
				filtered.at<cv::Vec3b>(j, i)[k] = (uchar)(std::max(std::min((int)temp, 255), 0));
			}
		}
	}
	mat = filtered;
}
