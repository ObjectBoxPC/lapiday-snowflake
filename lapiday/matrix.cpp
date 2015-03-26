#include "matrix.h"
#include <cstddef>
#include <stdexcept>
#include <iostream>

using std::size_t;
using std::logic_error;
using std::invalid_argument;
using std::out_of_range;
using std::ostream;
using std::endl;

namespace lapiday {
	namespace matrix {
		matrix::matrix() {
			_entries = NULL;
			_allocate(0, 0);
		}

		matrix::matrix(size_t rows, size_t cols) {
			_entries = NULL;
			_allocate(rows, cols);
			if((_rows > 0) && (_cols > 0)) {
				for(size_t i = 0; i < _rows; i++) {
					for(size_t j = 0; j < _cols; j++) {
						_entries[i][j] = 0;
					}
				}
			}
		}

		matrix::matrix(size_t size) {
			_entries = NULL;
			_allocate(size, size);
			_make_identity();
		}

		matrix::matrix(size_t size, rowop op) {
			_entries = NULL;
			_allocate(size, size);
			_make_identity();
			do_rowop(op);
		}

		matrix::matrix(const matrix& m) {
			_entries = NULL;
			_copy_data(m);
		}

		matrix::~matrix() {
			_deallocate_entries();
		}

		size_t matrix::rows() const {
			return _rows;
		}

		size_t matrix::cols() const {
			return _cols;
		}

		bool matrix::square() const {
			return (_rows == _cols);
		}

		matrix matrix::transpose() const {
			matrix temp(_cols, _rows);
			for(size_t i = 0; i < _rows; i++) {
				for(size_t j = 0; j < _cols; j++) {
					temp._entries[j][i] = _entries[i][j];
				}
			}
			return temp;
		}

		bool matrix::symmetric() const {
			if(!square()) {
				return false;
			}
			return (*this == transpose());
		}

		bool matrix::same_size(const matrix& m) const {
			return ((_rows == m._rows) && (_cols == m._cols));
		}

		void matrix::do_rowop(rowop op) {
			//Check row
			if(op.row >= _rows) {
				throw invalid_argument("Row out of range");
			}
			switch(op.type) {
			case rowop_switch:
				//Check other row
				if(op.other_row >= _rows) {
					throw invalid_argument("Other row out of range");
				}
				{
					//Switch the pointers to the rows
					double* temp = _entries[op.row];
					_entries[op.row] = _entries[op.other_row];
					_entries[op.other_row] = temp;
				}
				break;
			case rowop_multiply:
				for(size_t i = 0; i < _cols; i++) {
					_entries[op.row][i] *= op.multiplier;
				}
				break;
			case rowop_add:
				//Check other row
				if(op.other_row >= _rows) {
					throw invalid_argument("Other row out of range");
				}
				for(size_t i = 0; i < _cols; i++) {
					_entries[op.row][i] += (op.multiplier * _entries[op.other_row][i]);
				}
				break;
			}
		}

		void matrix::gauss_jordan() {
			//NOT IMPLEMENTED
		}

		double matrix::determinant() const {
			if(!square()) {
				throw logic_error("Matrix is not square");
			}
			if(_rows == 0) {
				//The determinant of an empty matrix is 1
				return 1;
			}
			return 0; //NOT IMPLEMENTED
		}

		bool matrix::invertible() const {
			if(!square()) {
				return false;
			} else {
				return (determinant() != 0);
			}
		}

		matrix matrix::inverse() const {
			matrix temp = *this;
			temp._invert();
			return temp;
		}

		matrix matrix::adjoint() const {
			if(!square()) {
				throw logic_error("Matrix is not square");
			}
			matrix temp(_cols, _rows);
			for(size_t i = 0; i < _rows; i++) {
				for(size_t j = 0; j < _cols; j++) {
					temp._entries[j][i] = submatrix(i, j).determinant();
					if((i + j) % 2 == 1) {
						temp._entries[j][i] = -temp._entries[j][i];
					}
				}
			}
			return temp;
		}

		matrix matrix::submatrix(size_t i, size_t j) const {
			//"Normalize" zero dimensions to one
			//so the "dimension minus one" works
			size_t normrows = _rows ? _rows : 1;
			size_t normcols = _cols ? _cols : 1;
			matrix temp(normrows - 1, normcols - 1);
			size_t newrow = 0;
			size_t newcol;
			//Use original dimensions in loop
			for(size_t oldrow = 0; oldrow < _rows; oldrow++) {
				if(newrow == i) {
					//Skip row
					oldrow++;
				}
				newcol = 0;
				for(size_t oldcol = 0; oldcol < _cols; oldcol++) {
					if(newcol == j) {
						//Skip column
						oldcol++;
					}
					temp._entries[newrow][newcol] = _entries[oldrow][oldcol];
					newcol++;
				}
				newrow++;
			}
			return temp;
		}

		double& matrix::operator()(size_t i, size_t j) {
			if((i < _rows) && (j < _cols)) {
				return _entries[i][j];
			} else {
				throw out_of_range("Element index out of range");
			}
		}

		double matrix::operator()(size_t i, size_t j) const {
			if((i < _rows) && (j < _cols)) {
				return _entries[i][j];
			} else {
				throw out_of_range("Element index out of range");
			}
		}

		bool operator ==(const matrix& first, const matrix& second) {
			if(!first.same_size(second)) {
				//Different sizes
				return false;
			}
			//Check entries
			size_t rows = first._rows;
			size_t cols = first._cols;
			for(size_t i = 0; i < rows; i++) {
				for(size_t j = 0; j < cols; j++) {
					if(first._entries[i][j] != second._entries[i][j]) {
						return false;
					}
				}
			}
			//Did not return in loop
			return true;
		}

		bool operator !=(const matrix& first, const matrix& second) {
			return !(first == second);
		}

		matrix operator +(const matrix& first, const matrix& second) {
			if(!first.same_size(second)) {
				throw invalid_argument("Matrices are not the same size");
			}
			matrix temp = first;
			temp._add(second);
			return temp;
		}

		matrix& matrix::operator +=(const matrix& m) {
			_add(m);
			return *this;
		}

		matrix operator -(const matrix& min, const matrix& sub) {
			return (min + (-sub));
		}

		matrix& matrix::operator -=(const matrix& m) {
			return operator +=(-m);
		}

		matrix operator *(double scalar, const matrix& m) {
			matrix temp = m;
			temp._multiply(scalar);
			return temp;
		}

		matrix operator *(const matrix& m, double scalar) {
			matrix temp = m;
			temp._multiply(scalar);
			return temp;
		}

		matrix& matrix::operator *=(double scalar) {
			_multiply(scalar);
			return *this;
		}

		matrix operator /(const matrix& m, double scalar) {
			if(scalar == 0) {
				throw invalid_argument("Division by zero scalar");
			}
			matrix temp = m;
			temp._multiply(1 / scalar);
			return temp;
		}

		matrix& matrix::operator /=(double scalar) {
			if(scalar == 0) {
				throw invalid_argument("Division by zero scalar");
			}
			_multiply(1 / scalar);
			return *this;
		}

		matrix matrix::operator -() const {
			matrix temp = *this;
			temp._multiply(-1);
			return temp;
		}

		matrix operator *(const matrix& first, const matrix& second) {
			matrix temp = first;
			temp._multiply(second);
			return temp;
		}

		matrix& matrix::operator *=(const matrix& m) {
			_multiply(m);
			return *this;
		}

		matrix operator ^(const matrix& m, int exp) {
			if(!m.square()) {
				throw invalid_argument("Matrix is not square");
			}
			if(exp == 0) {
				//Return identity matrix
				return matrix(m._rows);
			} else {
				matrix temp;
				if(exp < 0) {
					//Check invertibility
					if(!m.invertible()) {
						throw invalid_argument("Matrix is not invertible");
					}
					exp = -exp;
					temp = m.inverse();
				} else {
					temp = m;
				}
				//_multiply(matrix) takes a reference,
				//so another copy is needed
				matrix temp2 = temp;
				for(int i = 0; i < exp; i++) {
					temp._multiply(temp2);
				}
				return temp;
			}
		}

		matrix& matrix::operator ^=(int exp) {
			if(!square()) {
				throw invalid_argument("Matrix is not square");
			}
			if(exp == 0) {
				//Identity matrix
				_make_identity();
			} else {
				if(exp < 0) {
					_invert();
					exp = -exp;
				}
				//_multiply(matrix) takes a reference,
				//so a copy is needed
				matrix temp = *this;
				for(int i = 0; i < exp; i++) {
					_multiply(temp);
				}
			}
			return *this;
		}

		matrix& matrix::operator =(const matrix& m) {
			if(this != &m) {
				_copy_data(m);
			} //else self-assignment: Do nothing
			return *this;
		}

		ostream& operator <<(ostream& out, const matrix& m) {
			if((m._rows > 0) && (m._cols > 0)) {
				for(size_t i = 0; i < m._rows; i++) {
					out << m._entries[i][0];
					for(size_t j = 1; j < m._cols; j++) {
						out << ", " << m._entries[i][j];
					}
					out << endl;
				}
			} else {
				out << "[]" << endl;
			}
			return out;
		}

		void matrix::_allocate(size_t rows, size_t cols) {
			_deallocate_entries();
			//Allocate row pointers
			_entries = new double*[rows];
			//Allocate each row
			for(size_t i = 0; i < rows; i++) {
				_entries[i] = new double[cols];
			}
			//Set members
			_rows = rows;
			_cols = cols;
		}

		void matrix::_deallocate_entries() {
			if(_entries != NULL) {
				//Deallocate each row
				for(size_t i = 0; i < _rows; i++) {
					delete[] _entries[i];
				}
				//Deallocate row pointers
				delete[] _entries;
			}
		}

		void matrix::_copy_data(const matrix& m) {
			_allocate(m._rows, m._cols);
			for(size_t i = 0; i < _rows; i++) {
				for(size_t j = 0; j < _cols; j++) {
					_entries[i][j] = m._entries[i][j];
				}
			}
		}

		void matrix::_make_identity() {
			if(_rows == _cols) {
				for(size_t i = 0; i < _rows; i++) {
					for(size_t j = 0; j < _cols; j++) {
						if(i == j) {
							_entries[i][j] = 1;
						} else {
							_entries[i][j] = 0;
						}
					}
				}
			} //else not square: do nothing
		}

		void matrix::_add(const matrix& m) {
			if(!same_size(m)) {
				throw invalid_argument("Matrices are not the same size");
			}
			for(size_t i = 0; i < _rows; i++) {
				for(size_t j = 0; j < _cols; j++) {
					_entries[i][j] += m._entries[i][j];
				}
			}
		}

		void matrix::_multiply(double scalar) {
			for(size_t i = 0; i < _rows; i++) {
				for(size_t j = 0; j < _cols; j++) {
					_entries[i][j] *= scalar;
				}
			}
		}

		void matrix::_multiply(const matrix& m) {
			if(_cols != m._rows) {
				throw invalid_argument("Matrices cannot be multiplied in the given order");
			}
			matrix temp(_rows, m._cols);
			double entry;
			for(size_t i = 0; i < _rows; i++) {
				for(size_t j = 0; j < m._cols; j++) {
					entry = 0;
					for(size_t k = 0; k < _cols; k++) {
						entry += (_entries[i][k] * m._entries[k][j]);
					}
					temp._entries[i][j] = entry;
				}
			}
			_copy_data(temp);
		}

		void matrix::_invert() {
			//NOT IMPLEMENTED
		}
	}
}