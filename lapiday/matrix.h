#ifndef LAPIDAY_MATRIX_H
#define LAPIDAY_MATRIX_H

#include <cstddef>
#include <stdexcept>
#include <iostream>

using std::size_t;
using std::logic_error;
using std::invalid_argument;
using std::out_of_range;
using std::ostream;

namespace lapiday {
	namespace matrix {
		/**
		* Type of elementary row operation
		*/
		enum rowop_type {
			rowop_switch,
			rowop_multiply,
			rowop_add
		};

		/**
		* Specification of a row operation
		* on a matrix
		*/
		struct rowop {
			/**
			* Type of row operation
			*/
			rowop_type type;

			/**
			* Row affected (zero-based)
			*/
			size_t row;

			/**
			* Constant of multiplication
			* (for multiply and add
			* operations)
			*/
			double multiplier;

			/**
			* Row to "borrow" from (for add
			* and switch operations)
			* (zero-based)
			*/
			size_t other_row;
		};

		/**
		* Matrix, useful in linear algebra
		*/
		class matrix {
		public:
			/**
			* Create a new zero-by-zero matrix
			*/
			matrix();

			/**
			* Create a new zero matrix with the given number
			* of rows and columns.
			* @param rows Number of rows
			* @param cols Number of columns
			*/
			matrix(size_t rows, size_t cols);

			/**
			* Create a new identity matrix with the given size
			* (rows and columns).
			* @param size Number of rows and columns
			*/
			matrix(size_t size);

			/**
			* Create a new elementary matrix of the given size
			* that represents the given row operation.
			* @param size Number of rows and columns
			* @param op Operation the matrix should represent
			* @throw invalid_argument If the row operation
			* cannot be applied to the identity matrix of
			* the given size
			*/
			matrix(size_t size, rowop op);

			/**
			* Create a new matrix equal to the given matrix.
			* @param m Original matrix
			*/
			matrix(const matrix& m);

			/**
			* Deallocate all dynamic memory associated
			* with this object.
			*/
			~matrix();

			/**
			* Get the number of rows this matrix has.
			* @return Number of rows
			*/
			size_t rows() const;

			/**
			* Get the number of columns this matrix has.
			* @return Number of columns
			*/
			size_t cols() const;

			/**
			* Check if this matrix is square.
			* @return true if this matrix is square,
			* false otherwise
			*/
			bool square() const;

			/**
			* Get the transpose of this matrix.
			* @return Transpose
			*/
			matrix transpose() const;

			/**
			* Check if this matrix is symmetric.
			* @return true if this matrix is symmetric,
			* false otherwise
			*/
			bool symmetric() const;

			/**
			* Check if this matrix has the same
			* size as the given matrix.
			* @param m Matrix to compare with
			* @return true if this matrix and
			* the given matrix have the same
			* size, false otherwise
			*/
			bool same_size(const matrix& m) const;

			/**
			* Perform a row operation on this matrix.
			* @param op Operation to apply
			* @return Matrix resulting from the operation
			* @throw invalid_argument If the row operation
			* cannot be applied to this matrix (usually
			* if this matrix is too small)
			*/
			void do_rowop(rowop op);

			/**
			* Reduce this matrix to reduced row-echelon
			* form with Gauss-Jordan elimination.
			*/
			void gauss_jordan();

			/**
			* Get the determinant of this matrix.
			* @return Determinant
			* @throw logic_error If this matrix is not square
			*/
			double determinant() const;

			/**
			* Check if this matrix is invertible
			* (determinant is not 0).
			* @return true if this matrix is invertible,
			* false otherwise
			*/
			bool invertible() const;

			/**
			* Find the inverse.
			* @return Inverse
			* @throw logic_error If this matrix is not
			* square, or if this matrix is square but
			* not invertible
			*/
			matrix inverse() const;

			/**
			* Get the adjoint (adjugate) matrix of this matrix.
			* @return Adjoint (adjugate) matrix
			* @throw logic_error If this matrix is not square
			*/
			matrix adjoint() const;

			/**
			* Get the submatrix obtained by deleting the given
			* row and column. If this matrix is square,
			* the minor is the determinant of the submatrix.
			* @param i Row to delete (zero-based)
			* @param j Column to delete (zero-based)
			* @return Submatrix
			* @throw invalid_argument If the row or column
			* is not in the range of this matrix
			*/
			matrix submatrix(size_t i, size_t j) const;

			/**@{*/
			/**
			* Get the entry at the given position.
			* @param i Row (zero-based)
			* @param j Column (zero-based)
			* @return Entry
			* @throw out_of_range If the row or column
			* is not in the range of this matrix
			*/
			double& operator()(size_t i, size_t j);
			double operator()(size_t i, size_t j) const;
			/**@}*/

			/**
			* Check if the two matrices are equal.
			* @param first First matrix
			* @param second Second matrix
			* @return true if the matrices are equal,
			* false otherwise
			*/
			friend bool operator ==(const matrix& first, const matrix& second);

			/**
			* Check if the two matrices are not equal.
			* @param first First matrix
			* @param second Second matrix
			* @return true if the matrices are not equal,
			* false otherwise
			*/
			friend bool operator !=(const matrix& first, const matrix& second);

			/**
			* Add two matrices.
			* @param first First addend
			* @param second Second addend
			* @return Sum
			* @throw invalid_argument If the matrices do not
			* have the same size
			*/
			friend matrix operator +(const matrix& first, const matrix& second);

			/**
			* Add a matrix to this matrix.
			* @param m Matrix to add
			* @return This matrix
			* @throw invalid_argument If the given matrix
			* is not the same size as this matrix
			*/
			matrix& operator +=(const matrix& m);

			/**
			* Subtract one matrix from the other
			* @param min Minuend
			* @param sub Subtrahend
			* @return Difference (min - sub)
			* @throw invalid_argument If the matrices do not
			* have the same size
			*/
			friend matrix operator -(const matrix& min, const matrix& sub);

			/**
			* Subtract a matrix from this matrix.
			* @param m Matrix to subtract
			* @return This matrix
			* @throw invalid_argument If the given matrix
			* is not the same size as this matrix
			*/
			matrix& operator -=(const matrix& m);

			/**@{*/
			/**
			* Multiply a matrix by a scalar.
			* @param scalar Scalar
			* @param m Matrix
			* @return Scalar multiple
			*/
			friend matrix operator *(double scalar, const matrix& m);
			friend matrix operator *(const matrix& m, double scalar);
			/**@}*/

			/**
			* Multiply this matrix by a scalar.
			* @param scalar Scalar
			* @return This matrix
			*/
			matrix& operator *=(double scalar);

			/**
			* Divide a matrix by a scalar (multiply the matrix
			* by the reciprocal of the scalar).
			* @param m Matrix
			* @param scalar Scalar
			* @return Scalar multiple
			* @throw invalid_argument If the scalar is zero
			*/
			friend matrix operator /(const matrix& m, double scalar);

			/**
			* Divide this matrix by a scalar (multiply it
			* by the reciprocal of the scalar).
			* @param scalar Scalar
			* @return This matrix
			* @throw invalid_argument If the scalar is zero
			*/
			matrix& operator /=(double scalar);

			/**
			* Negate a matrix (multiply it by
			* the scalar -1).
			* @return Negative
			*/
			matrix operator -() const;

			/**
			* Multiply two matrices.
			* @param first First factor
			* @param second Second factor
			* @return Product
			* @throw invalid_argument If the matrices cannot
			* be multiplied in this order
			*/
			friend matrix operator *(const matrix& first, const matrix& second);

			/**
			* Multiply this matrix by another matrix.
			* @param m Matrix to multiply by
			* @throw invalid_argument If the matrices cannot
			* be multiplied with this matrix on the left
			* @return This matrix
			*/
			matrix& operator *=(const matrix& m);

			/**
			* Exponentiate the matrix (multiply it by itself).
			* If the exponent is negative, the matrix is inverted
			* and then exponentiated. If the exponent is zero,
			* the identity matrix is returned.
			* @param m Matrix
			* @param exp Exponent
			* @return Power of the matrix
			* @throw invalid_argument If the matrix is not square,
			* or if the exponent is negative and the matrix is
			* not invertible
			*/
			friend matrix operator ^(const matrix& m, int exp);

			/**
			* Exponentiate this matrix (multiply it by itself).
			* If the exponent is negative, the matrix is inverted
			* and then exponentiated. If the exponent is zero,
			* the identity matrix is returned.
			* @param exp Exponent
			* @return This matrix
			* @throw invalid_argument If this matrix is not square,
			* or if the exponent is negative and the matrix is
			* not invertible
			*/
			matrix& operator ^=(int exp);

			/**
			* Assign another matrix to this matrix.
			* @param m Matrix to assign
			* @return A reference to this matrix
			*/
			matrix& operator =(const matrix& m);

			/**
			* Output this matrix to the stream.
			* @param out Stream to output to
			* @param m Matrix to output
			* @return The stream
			*/
			friend ostream& operator <<(ostream& out, const matrix& m);
		private:
			/**
			* Entries, as an array of pointers to rows
			*/
			double** _entries;

			/**
			* Number of rows
			*/
			size_t _rows;

			/**
			* Number of columns
			*/
			size_t _cols;

			/**
			* Allocate memory for the given number of rows
			* and columns. The members _rows and _cols are
			* set appropriately. If _entries is not NULL,
			* old data is deleted.
			* @param rows Number of rows to allocate
			* @param cols Number of columns to allocate
			*/
			void _allocate(size_t rows, size_t cols);

			/**
			* Deallocate all memory for the entries
			* and set _entries to NULL, if _entries
			* is not already NULL. The members
			* _rows and _cols are not affected.
			*/
			void _deallocate_entries();

			/**
			* Copy the data from another matrix,
			* allocating and deallocating as necessary.
			* @param m Matrix to copy from
			*/
			void _copy_data(const matrix& m);

			/**
			* Make this matrix an identity matrix,
			* if the matrix is square (_rows == _cols).
			*/
			void _make_identity();

			/**
			* Add the given matrix to this matrix.
			* @param m Matrix to add
			* @throw invalid_argument If the matrices do not
			* have the same size
			*/
			void _add(const matrix& m);

			/**
			* Multiply this matrix by a scalar.
			* @param scalar Scalar
			*/
			void _multiply(double scalar);

			/**
			* Multiply this matrix by another matrix.
			* @param m Matrix to multiply by
			* @throw invalid_argument If the matrices cannot
			* be multiplied with this matrix on the left
			*/
			void _multiply(const matrix& m);

			/**
			* Invert this matrix (set this matrix to its
			* inverse).
			* @throw logic_error If this matrix is not
			* square, or if this matrix is square but
			* not invertible
			*/
			void _invert();
		};
	}
}

#endif