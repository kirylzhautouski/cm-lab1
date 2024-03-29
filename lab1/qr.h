#pragma once

void BuildQR(double** m, int rows, int columns, double** qr, double* diag_r);

void SolveQR(double** qr, double* diag_r, int rows, int columns, double* v, double* x);
