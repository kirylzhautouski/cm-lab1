#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>

#include "utilities.h"

#include "fill.h" // task1
#include "condition_number.h" // task2
#include "gauss.h" // task3
#include "lup.h" // task4
#include "cholesky.h" // task5

using namespace std;

const int N = 16;

const int ROWS = 256;
const int COLUMNS = 256;

int main() {
	// 1 TASK (Fill)
	cout << fixed << setprecision(13);

	double** A = new double*[ROWS];

	for (int i = 0; i < ROWS; i++)
		A[i] = new double[COLUMNS];

	Fill(A, ROWS, COLUMNS, N);

	double* y = new double[COLUMNS];

	Fill(y, COLUMNS, N);

	double* b = new double[ROWS];

	Multiply(A, ROWS, COLUMNS, y, COLUMNS, b);

	// 2 TASK (Condition number)
	cout << "Condition number: " << CountConditionNumber(A, ROWS, COLUMNS) << endl;

	// 3 TASK (Gauss)
	double* x_gauss = new double[COLUMNS];
	Gauss(A, ROWS, COLUMNS, b, x_gauss);

	/*
	cout << "Vector x_gauss: " << endl;
	PrintVector(x_gauss, COLUMNS);
	cout << endl << endl;

	cout << "Vector y: " << endl;
	PrintVector(y, COLUMNS);
	*/

	double* difference_gauss = new double[COLUMNS];

	Subtract(y, x_gauss, COLUMNS, difference_gauss);
	cout << "gauss max norm: " << MaxNorm(difference_gauss, COLUMNS) << endl;

	// 4 TASK (LUP)
	double** lu = new double*[ROWS];

	for (int i = 0; i < ROWS; i++)
		lu[i] = new double[COLUMNS];

	int* p = new int[ROWS];

	BuildLUP(A, ROWS, COLUMNS, lu, p);

	double* x_lup = new double[COLUMNS];

	SolveLUP(lu, p, ROWS, COLUMNS, b, x_lup);

	double* difference_lup = new double[COLUMNS];
	
	Subtract(y, x_lup, COLUMNS, difference_lup);
	cout << "lup max norm: " << MaxNorm(difference_lup, COLUMNS) << endl;

	// 5 TASK (CHOLESKY)
	double** lt = new double*[ROWS];
	double* d = new double[ROWS];

	for (int i = 0; i < ROWS; i++)
		lt[i] = new double[COLUMNS];

	BuildCholesky(A, ROWS, COLUMNS, lt, d);

	double* x_cholesky = new double[COLUMNS];

	SolveCholesky(lt, d, ROWS, COLUMNS, b, x_cholesky);

	double* difference_cholesky = new double[COLUMNS];

	Subtract(y, x_cholesky, COLUMNS, difference_cholesky);
	cout << "cholesky max norm: " << MaxNorm(difference_cholesky, COLUMNS) << endl;

	// TASK 6 (Relaxation)

	delete[] difference_cholesky;

	delete[] x_cholesky;

	delete[] d;

	for (int i = 0; i < ROWS; i++)
		delete[] lt[i];

	delete[] lt;

	delete[] difference_lup;

	delete[] x_lup;

	delete[] p;

	for (int i = 0; i < ROWS; i++)
		delete[] lu[i];

	delete[] lu;

	delete[] difference_gauss;

	delete[] x_gauss;

	delete[] b;
	delete[] y;

	for (int i = 0; i < ROWS; i++)
		delete[] A[i];

	delete[] A;

	system("pause");

	return 0;
}