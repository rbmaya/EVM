#ifndef LAB7_BLAS_INVERT_MATRIX_H
#define LAB7_BLAS_INVERT_MATRIX_H

void delete_matrix(float **mat, int N);

float **transpose_matrix(float **matA, int N);

float columns_norm(float **mat, int N);

float rows_norm(float **mat, int N);

float** div_mat_by_num(float **mat, int N, float num);

float **invert_matrix(float **matA, int N, int M);

float **create_zero_matrix(int N);

float **create_user_matrix(int N);

float **create_random_matrix(int N);

float **create_unit_matrix(int N);

float **add_matrix(float **matA, float **matB, int N);

float **sub_matrix(float **matA, float **matB, int N);

float **mul_matrix(float **matA, float **matB, int N);

float **create_B_matrix(float **matA, int N);

float **create_R_matrix(float **matA, int N);


#endif
