#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>
#include <chrono>

#include "utilities.h"

#include "fill.h" // task1
#include "condition_number.h" // task2
#include "gauss.h" // task3
#include "lup.h" // task4
#include "cholesky.h" // task5
#include "relaxation.h" // task6
#include "qr.h" // task7
#include "least_squares.h" // task8
#include "gmres.h" // task9
#include "gmres_arnoldi.h" // task10

using namespace std;

const int N = 3;

const int ROWS = 256;
const int COLUMNS = 256;

const int NUMBER_OF_ITERATIONS = 100;

int main() {
	ofstream fout("report.txt");

	double** A = new double*[ROWS];

	for (int i = 0; i < ROWS; i++)
		A[i] = new double[COLUMNS];

	double* y = new double[COLUMNS];

	double* b = new double[ROWS];

	double* x = new double[COLUMNS];

	double* difference = new double[COLUMNS];

	// TASK 2 
	double min_condition_number = numeric_limits<double>::max(), max_condition_number = 0, 
		average_condition_number = 0;

	double average_find_inverse_time = 0;

	double** A_inverse = new double*[ROWS];

	for (int i = 0; i < ROWS; i++)
		A_inverse[i] = new double[COLUMNS];

	// TASK 3
	double min_norm_gauss = numeric_limits<double>::max(), max_norm_gauss = 0, average_norm_gauss = 0;

	double average_gauss_time = 0;

	// TASK 4
	double min_norm_lup = numeric_limits<double>::max(), max_norm_lup = 0, average_norm_lup = 0;

	double average_build_lup_time = 0, average_solve_lup_time = 0;

	double** lu = new double*[ROWS];

	for (int i = 0; i < ROWS; i++)
		lu[i] = new double[COLUMNS];

	int* p = new int[ROWS];

	// TASK 5
	double min_norm_cholesky = numeric_limits<double>::max(), max_norm_cholesky = 0, average_norm_cholesky = 0;

	double average_cholesky_time = 0;

	double** lt = new double*[ROWS];
	double* d = new double[ROWS];

	for (int i = 0; i < ROWS; i++)
		lt[i] = new double[COLUMNS];

	// TASK 6
	double min_norm_relaxation = numeric_limits<double>::max(), max_norm_relaxation = 0, 
		average_norm_relaxation = 0;

	double average_relaxation_time = 0;

	// TASK 7
	double min_norm_qr = numeric_limits<double>::max(), max_norm_qr = 0,
		average_norm_qr = 0;

	double average_qr_time = 0;

	double** qr = new double*[ROWS];
	double* diag_r = new double[ROWS];

	for (int i = 0; i < ROWS; i++)
		qr[i] = new double[COLUMNS];

	// TASK 8
	double min_norm_least_squares = numeric_limits<double>::max(), max_norm_least_squares = 0, 
		average_norm_least_squares = 0;

	double average_least_squares_time = 0;

	double* Ax = new double[ROWS];

	double* discrepancy = new double[ROWS];

	// TASK 9
	double min_norm_gmres = numeric_limits<double>::max(), max_norm_gmres = 0, average_norm_gmres = 0;

	double average_gmres_time = 0;

	// TASK 10
	double min_norm_arnoldi = numeric_limits<double>::max(), max_norm_arnoldi = 0, average_norm_arnoldi = 0;

	double average_arnoldi_time = 0;

	chrono::high_resolution_clock::time_point start, finish;
	chrono::duration<double, std::milli> fp_ms;
	for (int i = 0; i < NUMBER_OF_ITERATIONS; i++) {
		// TASK 1
		Fill(A, ROWS, COLUMNS, N);
		Fill(y, COLUMNS, N);

		Multiply(A, ROWS, COLUMNS, y, COLUMNS, b);

		// TASK 2
		
		double condition_number = CountConditionNumber(A, ROWS, COLUMNS);
		
		min_condition_number = min(min_condition_number, condition_number);
		max_condition_number = max(max_condition_number, condition_number);
		average_condition_number += condition_number / NUMBER_OF_ITERATIONS;

		start = chrono::high_resolution_clock::now();
		GaussJordanInverse(A, ROWS, COLUMNS, A_inverse);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_find_inverse_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		// TASK 3
		start = chrono::high_resolution_clock::now();
		SolveGauss(A, ROWS, COLUMNS, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_gauss_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Subtract(x, y, COLUMNS, difference);

		double gauss_norm = MaxNorm(difference, COLUMNS);
		min_norm_gauss = min(min_norm_gauss, gauss_norm);
		max_norm_gauss = max(max_norm_gauss, gauss_norm);

		average_norm_gauss += gauss_norm / NUMBER_OF_ITERATIONS;

		// TASK 4
		start = chrono::high_resolution_clock::now();
		BuildLUP(A, ROWS, COLUMNS, lu, p);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_build_lup_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		start = chrono::high_resolution_clock::now();
		SolveLUP(lu, p, ROWS, COLUMNS, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_solve_lup_time += fp_ms.count() / NUMBER_OF_ITERATIONS;
		
		Subtract(x, y, COLUMNS, difference);

		double lup_norm = MaxNorm(difference, COLUMNS);
		min_norm_lup = min(min_norm_lup, lup_norm);
		max_norm_lup = max(max_norm_lup, lup_norm);

		average_norm_lup += lup_norm / NUMBER_OF_ITERATIONS;

		// TASK 5
		start = chrono::high_resolution_clock::now();
		BuildCholesky(A, ROWS, COLUMNS, lt, d);
		SolveCholesky(lt, d, ROWS, COLUMNS, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_cholesky_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Subtract(x, y, COLUMNS, difference);

		double cholesky_norm = MaxNorm(difference, COLUMNS);
		min_norm_cholesky = min(min_norm_cholesky, cholesky_norm);
		max_norm_cholesky = max(max_norm_cholesky, cholesky_norm);

		average_norm_cholesky += cholesky_norm / NUMBER_OF_ITERATIONS;

		// TASK 6
		start = chrono::high_resolution_clock::now();
		SolveRelaxation(A, ROWS, COLUMNS, b, 0.8, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_relaxation_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Subtract(x, y, COLUMNS, difference);

		double relaxation_norm = MaxNorm(difference, COLUMNS);
		min_norm_relaxation = min(min_norm_relaxation, relaxation_norm);
		max_norm_relaxation = max(max_norm_relaxation, relaxation_norm);

		average_norm_relaxation += relaxation_norm / NUMBER_OF_ITERATIONS;
		
		// TASK 7	
		start = chrono::high_resolution_clock::now();
		BuildQR(A, ROWS, COLUMNS, qr, diag_r);
		SolveQR(qr, diag_r, ROWS, COLUMNS, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_qr_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Subtract(x, y, COLUMNS, difference);

		double qr_norm = MaxNorm(difference, COLUMNS);
		min_norm_qr = min(min_norm_qr, qr_norm);
		max_norm_qr = max(max_norm_qr, qr_norm);

		average_norm_qr += qr_norm / NUMBER_OF_ITERATIONS;

		// TASK 8
		start = chrono::high_resolution_clock::now();
		SolveLeastSquares(A, ROWS, 20 * N, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_least_squares_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Multiply(A, ROWS, 20 * N, x, 20 * N, Ax);
		Subtract(Ax, b, ROWS, discrepancy);

		double least_squares_norm = EuclideanNorm(discrepancy, ROWS);
		min_norm_least_squares = min(min_norm_least_squares, least_squares_norm);
		max_norm_least_squares = max(max_norm_least_squares, least_squares_norm);

		average_norm_least_squares += least_squares_norm / NUMBER_OF_ITERATIONS;

		// TASK 9
		start = chrono::high_resolution_clock::now();
		SolveGMRES(A, ROWS, COLUMNS, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_gmres_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Subtract(x, y, COLUMNS, difference);

		double gmres_norm = MaxNorm(difference, ROWS);
		min_norm_gmres = min(min_norm_gmres, gmres_norm);
		max_norm_gmres = max(max_norm_gmres, gmres_norm);

		average_norm_gmres += gmres_norm / NUMBER_OF_ITERATIONS;
		
		// TASK 10
		start = chrono::high_resolution_clock::now();
		SolveGMRESArnoldi(A, ROWS, COLUMNS, b, x);
		finish = chrono::high_resolution_clock::now();

		fp_ms = finish - start;
		average_arnoldi_time += fp_ms.count() / NUMBER_OF_ITERATIONS;

		Subtract(x, y, COLUMNS, difference);

		double arnoldi_norm = MaxNorm(difference, ROWS);
		min_norm_arnoldi = min(min_norm_arnoldi, arnoldi_norm);
		max_norm_arnoldi = max(max_norm_arnoldi, arnoldi_norm);

		average_norm_arnoldi += arnoldi_norm / NUMBER_OF_ITERATIONS;
	}
	
	// write results to file
	fout << "N = " << N << endl << endl;
	fout << "EPS_RELAXATION = " << EPS_RELAXATION << endl;
	fout << "EPS_GMRES = " << scientific << EPS_GMRES << endl;
	fout.unsetf(ios_base::floatfield);
	fout << "EPS_GMRES_ARNOLDI = " << EPS_GMRES_ARNOLDI << endl << endl;
	fout << "ROWS = " << ROWS << " COLUMNS = " << COLUMNS << endl;
	fout << "NUMBER OF ITERATIONS = " << NUMBER_OF_ITERATIONS << endl << endl << endl;

	fout << "TASK 2 (condition number and inverse matrix)" << endl;
	fout << "min condition number: " << min_condition_number << endl;
	fout << "max condition number: " << max_condition_number << endl;
	fout << "average condition number: " << average_condition_number << endl << endl;

	fout << "average time to find inverse matrix: " << average_find_inverse_time << "ms" << endl << endl << endl;

	fout << "TASK 3 (Gauss)" << endl;
	fout << "min gauss ||x - y|| max norm: " << min_norm_gauss << endl;
	fout << "max gauss ||x - y|| max norm: " << max_norm_gauss << endl;
	fout << "average gauss ||x - y|| max norm: " << average_norm_gauss << endl << endl;

	fout << "average time to solve gauss: " << average_gauss_time << "ms" << endl << endl << endl;

	fout << "TASK 4 (LUP)" << endl;
	fout << "min lup ||x - y|| max norm: " << min_norm_lup << endl;
	fout << "max lup ||x - y|| max norm: " << max_norm_lup << endl;
	fout << "average lup ||x - y|| max norm: " << average_norm_lup << endl << endl;

	fout << "average time to build lup: " << average_build_lup_time << "ms" << endl;
	fout << "average time to solve lup: " << average_solve_lup_time << "ms" << endl << endl << endl;

	fout << "TASK 5 (Cholesky)" << endl;
	fout << "min cholesky ||x - y|| max norm: " << min_norm_cholesky << endl;
	fout << "max cholesky ||x - y|| max norm: " << max_norm_cholesky << endl;
	fout << "average cholesky ||x - y|| max norm: " << average_norm_cholesky << endl << endl;

	fout << "average time to solve cholesky: " << average_cholesky_time << "ms" << endl << endl << endl;

	fout << "TASK 6 (Relaxation)" << endl;
	fout << "min relaxation ||x - y|| max norm: " << min_norm_relaxation << endl;
	fout << "max relaxation ||x - y|| max norm: " << max_norm_relaxation << endl;
	fout << "average relaxation ||x - y|| max norm: " << average_norm_relaxation << endl << endl;

	fout << "average time to solve relaxation: " << average_relaxation_time << "ms" << endl << endl << endl;

	fout << "TASK 7 (QR)" << endl;
	fout << "min qr ||x - y|| max norm: " << min_norm_qr << endl;
	fout << "max qr ||x - y|| max norm: " << max_norm_qr << endl;
	fout << "average qr ||x - y|| max norm: " << average_norm_qr << endl << endl;

	fout << "average time to solve qr: " << average_qr_time << "ms" << endl << endl << endl;
	
	fout << "TASK 8 (Least squares)" << endl;
	fout << "min least squares ||Ay - b|| euclide norm: " << min_norm_least_squares << endl;
	fout << "max least squares ||Ay - b|| euclide norm: " << max_norm_least_squares << endl;
	fout << "average least squares ||Ay - b|| euclide norm: " << average_norm_least_squares << endl << endl;

	fout << "average time to solve least squares: " << average_least_squares_time << "ms" << endl << endl << endl;

	fout << "TASK 9 (GMRES)" << endl;
	fout << "min GMRES ||x - y|| max norm: " << min_norm_gmres << endl;
	fout << "max GMRES ||x - y|| euclide norm: " << max_norm_gmres << endl;
	fout << "average GMRES ||x - y|| euclide norm: " << average_norm_gmres << endl << endl;

	fout << "average time to solve GMRES: " << average_gmres_time << "ms" << endl << endl << endl;

	fout << "TASK 10 (GMRES Arnoldi)" << endl;
	fout << "min GMRES Arnoldi ||x - y|| max norm: " << min_norm_arnoldi << endl;
	fout << "max GMRES Arnoldi ||x - y|| euclide norm: " << max_norm_arnoldi << endl;
	fout << "average GMRES Arnoldi ||x - y|| euclide norm: " << average_norm_arnoldi << endl << endl;

	fout << "average time to solve GMRES Arnoldi: " << average_arnoldi_time << "ms" << endl << endl << endl;

	// TASK 8
	delete[] discrepancy;

	delete[] Ax;

	// TASK 7
	delete[] diag_r;

	for (int i = 0; i < ROWS; i++)
		delete[] qr[i];

	// TASK 5
	delete[] d;

	for (int i = 0; i < ROWS; i++)
		delete[] lt[i];

	// TASK 4
	delete[] p;

	for (int i = 0; i < ROWS; i++)
		delete[] lu[i];

	delete[] lu;

	// TASK 1

	for (int i = 0; i < ROWS; i++)
		delete[] A_inverse[i];

	delete[] A_inverse;

	delete[] difference;

	delete[] x;

	delete[] b;

	delete[] y;

	for (int i = 0; i < ROWS; i++)
		delete[] A[i];

	delete[] A;

	fout.close();

	system("pause");

	return 0;
}