#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "time.h"
#include <omp.h>

using namespace std;

void get_cur_matrix(float* matrix, const int size)
{
	srand(time(NULL));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i * size + j] = rand() % 1000;
		}
	}
}

void get_matrix(FILE* fp, const int size)
{
	srand(time(NULL));
	float* m = new float[size*size];
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			m[i*size + j] = rand()%1000;
		}
	}
	fprintf_s(fp, "%d ", size);
	fprintf_s(fp, "\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fprintf_s(fp, "%f ", m[i*size + j]);
		}
		fprintf_s(fp, "\n");
	}
	delete[] m;
}

void write_matrix(FILE* fp, float*& matrix, int size)
{
	fprintf_s(fp, "%d ", size);
	fprintf_s(fp, "\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fprintf_s(fp, "%f ", matrix[i*size + j]);
		}
		fprintf_s(fp, "\n");
	}
}

void read_matrix(FILE* fp, float*& matrix)
{
	int size;
	fscanf_s(fp, "%u ", &size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fscanf_s(fp, "%f ", &matrix[i*size + j]);
		}
	}
}

void mul_matrix(float* matrix1, float* matrix2, float* matrixres, int size)
{
	//omp_set_num_threads(8);
#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				matrixres[i * size + j] = 0;
				for (int k = 0; k < size; k++)
				{
					matrixres[i * size + j] += matrix1[i * size + k] * matrix2[k * size + j];
				}
			}
		}
	}
}

void print_matrix(float*& matrix, int size)
{
	cout << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << matrix[i*size + j] << " ";
		}
		cout << endl;
	}
}

int main()
{
	FILE* m1, * m2, * mres;
	const int size = 100;
	float* matrix1, * matrix2, * matrixres;

	matrix1 = new float[size * size];
	matrix2 = new float[size * size];

	cout << "Open\n";

	//Открытие файла
#pragma omp parallel sections num_threads(2)
	{
#pragma omp section
		{
			fopen_s(&m1, "matrix1.txt", "w");
			get_matrix(m1, size);
			freopen_s(&m1, "matrix1.txt", "r", m1);
			read_matrix(m1, matrix1);
			fclose(m1);
		}
#pragma omp section
		{
			fopen_s(&m2, "matrix2.txt", "w");
			get_matrix(m2, size);
			freopen_s(&m2, "matrix2.txt", "r", m2);
			read_matrix(m2, matrix2);
			fclose(m2);
		}
	}

	get_cur_matrix(matrix1, size);
	get_cur_matrix(matrix2, size);
	matrixres = new float[size * size];

	cout << "Begin calculating" << endl;

	double wtime = omp_get_wtime();
	mul_matrix(matrix1, matrix2, matrixres, size);
	cout << "Time of mul:" << omp_get_wtime() - wtime << endl;
	fopen_s(&mres, "mres.txt", "w");
	write_matrix(mres, matrixres, size);
	cout<< "\nClose\n";
	delete[] matrixres;
	delete[] matrix1;
	delete[] matrix2;
	return 0;
}