#pragma once

#include "IFilter.h"

namespace Filters {
	class Lowpass : public IFilter {

	public:
		Lowpass() {};

		void filter(cv::Mat& mat) override;
	};
}