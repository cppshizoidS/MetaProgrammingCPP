#include "shizmatrix.hpp"
#include <iostream>

using namespace shizmatrix;

int main () {
	// Make a 3x3 matrix filled with 0.5
	matrix<float> a(3, 3, 0.5); // (row, col, value)
	matrix<float> b(3, 3, 0.4);

	// Make an empty 3x3 matrix
	matrix<float> result(3, 3); // (row, col)

	// Basic arithmetic can be done easily
	result = a + b;
	result = a - b;
	result = a * b;

	// Element-wise arithmetic can be done too
	// Here, 5 is subtracted from every element of a
	result = a - 5;

	// Expressions can be as long as you want them to be
	result = 5 * a - result + b * 10.5 - result;

	// Use eval to trigger eager evaluation.
	// Here, it is beneficial to calculate (b + result)
	// in advance to prevent repeated calculations
	result = a * (b + result).eval();

	// Getting and setting elements is possible
	// All indexing is 0-based
	std::cout << result(0, 1) << '\n'; // (row, column)
	result.set_elt(0, 1, -1.5); // (row, column, new value)
	std::cout << result(0, 1) << '\n';

	// Print
	std::cout << result << '\n';
	return 0;
}
