#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>


int main(int, char* []) {
	cv::Mat helloWorld = cv::imread("helloWorld.png");
	cv::imshow("Hello World", helloWorld);
	cv::waitKey(-1);

	return 0;
}
