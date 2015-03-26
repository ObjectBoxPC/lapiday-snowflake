#include "matrix.h"
#include <iostream>
#include <cstddef> //For size_t

//lapiday = Linear Algebra Pi Day
using namespace lapiday::matrix; //For matrix code
using namespace std;

int main() {
	/*
	 * Like with arrays, row and column numbers are zero-based.
	 * So, for a 3 x 4 matrix, the rows are numbered 0, 1, and 2,
	 * and the columns are numbered 0, 1, 2, and 3.
	 */

	//size_t is an unsigned integer type (no negative values)
	//Use these constants for flexibility
	const size_t ROWS = 3;
	const size_t COLS = 4;

	//Create an identity matrix
	matrix i(ROWS);

	//Output the entries
	cout << "I_" << ROWS << ":" << endl;
	cout << i << endl;

	//Create a zero matrix with the given number rows and columns
	matrix m(ROWS, COLS);
	cout << "Zero matrix:" << endl;
	cout << m << endl;

	//Set one value
	m(0, 0) = 10;
	cout << m << endl;

	//Set all values
	for(size_t i = 0; i < ROWS; i++) {
		for(size_t j = 0; j < COLS; j++) {
			m(i, j) = 100 * i + j;
		}
	}
	cout << "All values set:" << endl;
	cout << m << endl;

	//Transpose a matrix
	matrix mt = m.transpose();
	cout << "Transpose:" << endl;
	cout << mt << endl;

	//What is the size?
	cout << "The transpose has " << mt.rows() << " rows and " << mt.cols() << " columns" << endl;

	//Create a row operation
	lapiday::matrix::rowop ro;
	ro.type = rowop_add;
	ro.row = 0;
	ro.multiplier = 3;
	ro.other_row = 1;

	//Apply the row operation to a copy of the matrix
	matrix m2 = m;
	m2.do_rowop(ro);
	cout << "After row operation (3R_1 + R_0):" << endl;
	cout << m2 << endl;

	//Alternatively, create an elementary matrix with the given size and operation
	matrix elem(ROWS, ro);
	cout << "Elementary matrix:" << endl;
	cout << elem << endl;

	//Multiply the original by the elementary matrix
	m = elem * m;

	//Catch an exception when trying to multiply the matrices in the wrong order
	try {
		matrix fail = m;
		fail *= elem;
		cout << "This should not appear. If it does, here is the result:" << endl;
		cout << fail << endl;
	} catch(invalid_argument e) {
		cout << "Could not multiply the matrices. The error was: " << e.what() << endl;
	}

	//Now try to add the matrices
	try {
		//The riddle: How can you have two variables named "fail"?
		matrix fail = m + elem;
		cout << "This should not appear. If it does, here is the result:" << endl;
		cout << fail << endl;
	} catch(invalid_argument e) {
		cout << "Could not add the matrices. The error was: " << e.what() << endl;
	}

	cout << "After multiplying by elementary matrix:" << endl;
	cout << m << endl;
	//Check: They should be the same.
	if(m == m2) {
		cout << "m and m2 are equal" << endl;
	} else {
		cout << "ALERT: m and m2 are not equal!" << endl;
	}

	//Multiply the matrix by a scalar
	//Each line does the same thing, so m is ultimately multiplied by 8
	m = m * 2;
	m = 2 * m;
	m *= 2;
	cout << "Multiplied by 8:" << endl;
	cout << m << endl;

	//Now divide (multiply by the reciprocal of the scalar)
	m = m / 4;
	m /= 2;
	cout << "Then divided by 8:" << endl;
	cout << m << endl;

	//Exponentiate a matrix (multiply it by itself)
	try {
		cout << (m ^ 2) << endl;
	} catch(invalid_argument e) {
		cout << "Oops, m isn't square." << endl;
	}
	cout << "Square of the elementary matrix:" << endl;
	cout << (elem ^ 2) << endl;
	//Alternatively
	elem ^= 2;
	cout << "Again:" << endl;
	cout << elem << endl;

	//Guess what this gives
	cout << "Anything to the zeroth power..." << endl;
	cout << (elem ^ 0) << endl;

	/*
	 * Note: It is possible to use a negative exponent,
	 * in which case the inverse is used.
	 * However, inverting a matrix has not yet been
	 * implemented, so it will not be demonstrated.
	 */

	//Get a submatrix
	cout << "Submatrix after removing row 0 and column 0:" << endl;
	cout << m.submatrix(0, 0) << endl;

	/*
	 * Extras (hopefully they don't need to be explained in great detail):
	 * - Subtracting matrices, negating matrices, subtract-assign (-=)
	 * Not yet implemented:
	 * - Gaussian/Gauss-Jordan elimination
	 * - Inverse
	 * - Determinant
	 */

	return 0;
}