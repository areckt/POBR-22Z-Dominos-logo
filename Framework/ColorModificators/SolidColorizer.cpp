#include "SolidColorizer.h"
#include "../Segmentators/SegmentationProcessor.h"

void SolidColorizer::solidColorize(cv::Mat& image, colors color) {
	int imageWidth = image.cols;
	int imageHeight = image.rows;

	cv::Mat_<cv::Vec3b> filtered = image.clone();

	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {
			int pointx = i;
			int pointy = j;

			cv::Vec3b vec = image.at<cv::Vec3b>(pointy, pointx);

			switch (color)
			{
			case White:
				if (SegmentationProcessor::checkWhite(vec)) {
					filtered[pointy][pointx] = cv::Vec3b((uchar)255, (uchar)255, (uchar)255);
				}
				break;
			case Blue:
				if (SegmentationProcessor::checkBlue(vec)) {
					filtered[pointy][pointx] = cv::Vec3b((uchar)255, (uchar)0, (uchar)0);
				}
				break;
			case Red:
				if (SegmentationProcessor::checkRed(vec)) {
					filtered[pointy][pointx] = cv::Vec3b((uchar)0, (uchar)0, (uchar)255);
				}
				break;
			default:
				break;
			}
		}
	}
	image = filtered;
}