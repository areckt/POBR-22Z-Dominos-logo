#pragma once

#include "IFilter.h"

namespace Filters {
	class Highpass : public IFilter {
	public:
		Highpass() {};

		void filter(cv::Mat& mat) override;
	};
}