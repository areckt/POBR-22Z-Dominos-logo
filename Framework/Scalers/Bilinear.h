#pragma once

#include "Scaler.h"

namespace Scalers {
	class Bilinear : public Scaler {

	public:
		Bilinear() {};

		void scale(cv::Mat& mat) override;
	};
}