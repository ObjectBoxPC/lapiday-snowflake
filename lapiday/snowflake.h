#ifndef LAPIDAY_SNOWFLAKE_H
#define LAPIDAY_SNOWFLAKE_H

#include "matrix.h"

namespace lapiday {
	/**
	 * In all cases, points are two-dimensional
	 * and represented as column vectors with
	 * an additional final component of value 1.
	 */
	namespace snowflake {
		using matrix::matrix;
		/**
		 * Line with transformation from
		 * a base, a line from (0, 0)
		 * to (0, 1).
		 */
		class line {
		public:
			/**
			 * Initialize the line with the
			 * given members.
			 * @param t Transformation matrix
			 * @param c Whether or not the
			 * transformations for this line
			 * are completed
			 */
			line(const matrix& t = matrix(3), bool c = false);

			/**
			 * Transformation matrix from
			 * the base line to this line.
			 * The endpoints of this line
			 * are determined by multiplying
			 * the transformation matrix
			 * by the endpoints of the base
			 * line with the transformation
			 * matrix on the left.
			 */
			matrix transformation;

			/**
			 * Whether or not the
			 * transformations for this line
			 * are completed
			 */
			bool completed;
		};

		/**
		 * Generate a matrix for scaling
		 * points about the origin.
		 * The transformation is applied
		 * by multiplying with this
		 * on the left.
		 * @param factor Scale factor
		 * @return Transformation
		 * matrix for scaling
		 */
		matrix scale(double factor);

		/**
		 * Generate a matrix for
		 * translating points by a
		 * certain amount. The
		 * transformation is applied
		 * by multiplying with this
		 * on the left.
		 * @param x Amount by which to
		 * translate in the x-direction
		 * @param y Amount by which to
		 * translate in the y-direction
		 * @return Transformation
		 * matrix for translation
		 */
		matrix translate(double x, double y);

		/**
		 * Generate a matrix for
		 * rotating points about the
		 * origin counterclockwise
		 * by an angle.
		 * @param angle Rotation
		 * angle in radians
		 * @return Transformation
		 * matrix for rotation
		 */
		matrix rotate(double angle);
	}
}

#endif
