#include "snowflake.h"
#include "matrix.h"
#include <cmath>

using namespace std;

namespace lapiday {
	using matrix::matrix;

	namespace snowflake {
		line::line(const matrix& t, bool c) {
			transformation = t;
			completed = c;
		}

		matrix scale(double factor) {
			matrix temp(3);
			temp(0, 0) = factor;
			temp(1, 1) = factor;
			//(2, 2) remains 1
			return temp;
		}

		matrix translate(double x, double y) {
			matrix temp(3);
			temp(0, 2) = x;
			temp(1, 2) = y;
			return temp;
		}

		matrix rotate(double angle) {
			matrix temp(3);
			temp(0, 0) = cos(angle);
			temp(0, 1) = -sin(angle);
			temp(1, 0) = sin(angle);
			temp(1, 1) = cos(angle);
			return temp;
		}
	}
}