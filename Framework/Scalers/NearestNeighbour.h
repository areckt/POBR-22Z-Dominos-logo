#pragma once

#include "Scaler.h"

namespace Scalers {
	class NearestNeighbour : public Scaler {

	public:
		NearestNeighbour() {}

		virtual void scale(cv::Mat& mat) override;
	};
}