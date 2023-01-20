#include "TraitDetector.h"
#include "../const.h"
#include <cmath>
#include <windows.h>
#include <iostream>

void TraitDetector::detectTraitsAndFilter(std::vector<SegmentDescriptor>& segments,
	std::map<Color, std::vector<SegmentDescriptor>>& bins) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::vector<double> norms = {};
	for (const auto& segment : segments) {
		norms = getNorms(segment);
		switch (segment.getColor()) {
		case Color::RED:
			if (std::fabs(norms[0] - RED_NORM1_MODEL) > RED_NORM1_THRESHOLD ||
				std::fabs(norms[1] - RED_NORM2_MODEL) > RED_NORM2_THRESHOLD ||
				std::fabs(norms[2] - RED_NORM3_MODEL) > RED_NORM3_THRESHOLD ||
				std::fabs(norms[3] - RED_NORM4_MODEL) > RED_NORM4_THRESHOLD ||
				std::fabs(norms[4] - RED_NORM5_MODEL) > RED_NORM5_THRESHOLD ||
				std::fabs(norms[5] - RED_NORM6_MODEL) > RED_NORM6_THRESHOLD ||
				std::fabs(norms[6] - RED_NORM7_MODEL) > RED_NORM7_THRESHOLD ||
				std::fabs(segment.getWidthHeightRatio() - RED_WH_MODEL) > RED_WH_THRESHOLD) {
				continue;
			}

			bins[Color::RED].push_back(segment);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			std::cout << "RED   M1: " << norms[0] << '\t' <<
				"M2: " << norms[1] << '\t' <<
				"M3: " << norms[2] << '\t' <<
				"M4: " << norms[3] << '\t' <<
				"M5: " << norms[4] << '\t' <<
				"M6: " << norms[5] << '\t' <<
				"M7: " << norms[6] << "\t" <<
				"WH: " << segment.getWidthHeightRatio() << '\n';
			break;

		case Color::BLUE:
			if (std::fabs(norms[0] - BLUE_NORM1_MODEL) > BLUE_NORM1_THRESHOLD ||
				std::fabs(norms[1] - BLUE_NORM2_MODEL) > BLUE_NORM2_THRESHOLD ||
				std::fabs(norms[2] - BLUE_NORM3_MODEL) > BLUE_NORM3_THRESHOLD ||
				std::fabs(norms[3] - BLUE_NORM4_MODEL) > BLUE_NORM4_THRESHOLD ||
				std::fabs(norms[4] - BLUE_NORM5_MODEL) > BLUE_NORM5_THRESHOLD ||
				std::fabs(norms[5] - BLUE_NORM6_MODEL) > BLUE_NORM6_THRESHOLD ||
				std::fabs(norms[6] - BLUE_NORM7_MODEL) > BLUE_NORM7_THRESHOLD ||
				std::fabs(segment.getWidthHeightRatio() - BLUE_WH_MODEL) > BLUE_WH_THRESHOLD) {
				continue;
			}
			bins[Color::BLUE].push_back(segment);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			std::cout << "BLUE  M1: " << norms[0] << '\t' <<
				"M2: " << norms[1] << '\t' <<
				"M3: " << norms[2] << '\t' <<
				"M4: " << norms[3] << '\t' <<
				"M5: " << norms[4] << '\t' <<
				"M6: " << norms[5] << '\t' <<
				"M7: " << norms[6] << "\t" <<
				"WH: " << segment.getWidthHeightRatio() << '\n';
			break;

		case Color::WHITE:
			if (std::fabs(norms[0] - WHITE_NORM1_MODEL) > WHITE_NORM1_THRESHOLD ||
				std::fabs(norms[1] - WHITE_NORM2_MODEL) > WHITE_NORM2_THRESHOLD ||
				std::fabs(norms[2] - WHITE_NORM3_MODEL) > WHITE_NORM3_THRESHOLD ||
				std::fabs(norms[3] - WHITE_NORM4_MODEL) > WHITE_NORM4_THRESHOLD ||
				std::fabs(norms[4] - WHITE_NORM5_MODEL) > WHITE_NORM5_THRESHOLD ||
				std::fabs(norms[5] - WHITE_NORM6_MODEL) > WHITE_NORM6_THRESHOLD ||
				std::fabs(norms[6] - WHITE_NORM7_MODEL) > WHITE_NORM7_THRESHOLD ||
				std::fabs(segment.getWidthHeightRatio() - WHITE_WH_MODEL) > WHITE_WH_THRESHOLD) {
				continue;
			}
			bins[Color::WHITE].push_back(segment);
			SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			std::cout << "WHITE M1: " << norms[0] << '\t' <<
				"M2: " << norms[1] << '\t' <<
				"M3: " << norms[2] << '\t' <<
				"M4: " << norms[3] << '\t' <<
				"M5: " << norms[4] << '\t' <<
				"M6: " << norms[5] << '\t' <<
				"M7: " << norms[6] << "\t" <<
				"WH: " << segment.getWidthHeightRatio() << '\n';
			break;
		}
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
}

std::vector<double> TraitDetector::getNorms(SegmentDescriptor descriptor) {
	std::vector<double> norms = {};

	double m00 = 0;
	double m01 = 0;
	double m10 = 0;
	double m11 = 0;
	double m20 = 0;
	double m02 = 0;
	double m21 = 0;
	double m12 = 0;
	double m30 = 0;
	double m03 = 0;

	for (auto point : descriptor.getPoints()) {
		m00++;
		m01 += point.first;
		m10 += point.second;
		m11 += point.second * point.first;
		m20 += std::pow(point.second, 2);
		m02 += std::pow(point.first, 2);
		m21 += std::pow(point.second, 2) * point.first;
		m12 += std::pow(point.first, 2) * point.second;
		m30 += std::pow(point.second, 3);
		m03 += std::pow(point.first, 3);
	}
	double i = m10 / m00;
	double j = m01 / m00;

	double M20 = m20 - (double)std::pow(m10, 2) / m00;
	double M02 = m02 - (double)std::pow(m01, 2) / m00;
	double M11 = m11 - (double)m10 * m01 / m00;
	double M30 = m03 - 3 * m02 * j + 2 * m01 * std::pow(j, 2);
	double M12 = m12 - 2 * m11 * j - m02 * i + 2 * m10 * std::pow(j, 2);
	double M21 = m21 - 2 * m11 * i - m20 * j + 2 * m01 * std::pow(i, 2);
	double M03 = m30 - 3 * m20 * i + 2 * m10 * std::pow(i, 2);

	double M1 = (double)((M20 + M02) / std::pow(m00, 2));
	double M2 = (double)(std::pow(M20 - M02, 2) + 4 * std::pow(M11, 2)) / std::pow(m00, 4);
	double M3 = (double)(std::pow(M30 - 3 * M12, 2) + std::pow(3 * M21 - M03, 2)) / std::pow(m00, 5);
	double M4 = (double)(std::pow(M30 + M12, 2) + std::pow(M21 + M03, 2)) / std::pow(m00, 5);
	double M5 = (double)((M30 - 3 * M12) * (M30 + M12) * ((std::pow(M30 + M12, 2) - 3 * std::pow(M21 + M03, 2)))
		+ (3 * M21 - M03) * (M03 + M21) * (3 * std::pow(M30 + M12, 2) - std::pow(M21 + M03, 2))) / std::pow(m00, 10);
	double M6 = (double)((M20 - M02) * (std::pow(M30 + M12, 2) - std::pow(M21 + M03, 2))
		+ 4 * M11 * (M30 + M12) * (M21 + M03)) / std::pow(m00, 7);
	double M7 = (double)((M20 * M02 - std::pow(M11, 2)) / std::pow(m00, 4));

	norms.push_back(M1);
	norms.push_back(M2);
	norms.push_back(M3);
	norms.push_back(M4);
	norms.push_back(M5);
	norms.push_back(M6);
	norms.push_back(M7);

	return norms;
}