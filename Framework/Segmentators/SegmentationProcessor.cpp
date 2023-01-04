#include <stack>
#include <opencv2/opencv.hpp>
#include "SegmentationProcessor.h"
#include "../const.h"

typedef struct {
	uchar r;	// a fraction between 0 and 1
	uchar g;	// a fraction between 0 and 1
	uchar b;	// a fraction between 0 and 1

} rgb;

typedef struct {
	uchar h;	// angle in degrees
	uchar s;	// a fraction between 0 and 1
	uchar v;	// a fraction between 0 and 1
} hsv;

static hsv rgb2hsv(rgb in);

hsv rgb2hsv(rgb rgb)
{
	hsv hsv;
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return hsv;
	}

	if (rgbMax == rgb.r)
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return hsv;
}

typedef struct {
	int h;		// angle in degrees
	float s;	// a fraction between 0 and 1
	float l;	// a fraction between 0 and 1
} hsl;

static hsl RGBToHSL(rgb rgb) {
	hsl hsl;
	hsl.h = 0;
	hsl.s = 0;
	hsl.l = 0;


	float r = (rgb.r / 255.0f);
	float g = (rgb.g / 255.0f);
	float b = (rgb.b / 255.0f);

	float min = std::min(std::min(r, g), b);
	float max = std::max(std::max(r, g), b);
	float delta = max - min;

	hsl.l = (max + min) / 2;

	if (delta == 0)
	{
		hsl.h = 0;
		hsl.s = 0.0f;
	}
	else
	{
		hsl.s = (hsl.l <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

		float hue;

		if (r == max)
		{
			hue = ((g - b) / 6) / delta;
		}
		else if (g == max)
		{
			hue = (1.0f / 3) + ((b - r) / 6) / delta;
		}
		else
		{
			hue = (2.0f / 3) + ((r - g) / 6) / delta;
		}

		if (hue < 0)
			hue += 1;
		if (hue > 1)
			hue -= 1;

		hsl.h = (int)(hue * 360);
	}

	return hsl;
}

bool SegmentationProcessor::checkWhite(cv::Vec3b& vec) {

	rgb rgb1;
	rgb1.r = vec[2];
	rgb1.g = vec[1];
	rgb1.b = vec[0];
	hsl hsl = RGBToHSL(rgb1);

	if (hsl.l >= 0.67 && hsl.s <= 0.65) {
		return true;
	}
	return false;
}

bool SegmentationProcessor::checkBlue(cv::Vec3b& vec) {
	rgb rgb1;
	rgb1.r = vec[2];
	rgb1.g = vec[1];
	rgb1.b = vec[0];
	hsl hsl = RGBToHSL(rgb1);

	if (hsl.h < 213 && hsl.h > 190 && hsl.s > 0.35 && hsl.l < 0.79 && hsl.l > 0.22) {
		return true;
	}
	return false;
}

bool SegmentationProcessor::checkRed(cv::Vec3b& vec) {

	rgb rgb1;
	rgb1.r = vec[2];
	rgb1.g = vec[1];
	rgb1.b = vec[0];
	hsl hsl = RGBToHSL(rgb1);
	if ((hsl.h < 15 || hsl.h > 325) && hsl.s > 0.43 && hsl.s < 0.82 && hsl.l > 0.33 && hsl.l < 0.7) {
		return true;
	}
	return false;
}
