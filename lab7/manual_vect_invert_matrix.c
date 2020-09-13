#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <immintrin.h>

void delete_matrix(float **mat, int N){
    free(mat);
}

float **create_zero_matrix(int N){
    float **mat = malloc(N * sizeof(float*));
    if (!mat) return NULL;

    float *mat1 = calloc(N * N, sizeof(float));
    for (int i = 0; i < N; i++)
        mat[i] = &mat1[i * N];

    return mat;
}

float **create_user_matrix(int N){
    float **mat = create_zero_matrix(N);

    printf("Enter the matrix: \n");

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            scanf("%f", &mat[i][j]);
    }

    return mat;
}

float **create_random_matrix(int N){
    float **mat = create_zero_matrix(N);

    srand(time(NULL));

    //printf("Generated matrix: \n");

    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j) {
            mat[i][j] = rand() % 100;
            //printf("%f ", mat[i][j]);
        }
        //printf("\n");
    }

    return mat;
}


float **create_unit_matrix(int N){
    float **mat = create_zero_matrix(N);

    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            if (i == j) mat[i][j] = 1;
        }
    }

    return mat;
}

float **transpose_matrix(float **matA, int N) {
    float **matB = create_zero_matrix(N);

    for (int i = 0; i < N; ++i){
        __m128 *mm_matA = (__m128*)matA[i];
        for (int j = 0; j < N / 4; ++j) {
            __m128 buf = mm_matA[j];
            float vect[4];
            _mm_store_ps(vect, buf);
            matB[j * 4][i] = vect[0];
            matB[j * 4 + 1][i] = vect[1];
            matB[j * 4 + 2][i] = vect[2];
            matB[j * 4 + 3][i] = vect[3];
        }
    }

    return matB;
}


float rows_norm(float **mat, int N){
    float fmax = 0;

    for (int i = 0; i < N; ++i){
        __m128 sum = _mm_setzero_ps();
        __m128 *mm_mat = (__m128*)mat[i];
        __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
        for (int j = 0; j < N / 4; ++j) {
            __m128 temp = _mm_and_ps(mm_mat[j], mask);
            sum = _mm_add_ps(sum, temp);
        }
        __m128 copy_s = _mm_movehl_ps(sum, sum);
        sum = _mm_add_ps(sum, copy_s);
        __m128 copy_s2 = _mm_shuffle_ps(sum, sum,1);
        sum = _mm_add_ss(sum, copy_s2);

        float fsum;
        _mm_store_ss(&fsum, sum);

        if (fsum > fmax) fmax = fsum;
    }

    return fmax;
}

float columns_norm(float **mat, int N){
    float **mat_copy = transpose_matrix(mat, N);

    float max = rows_norm(mat_copy, N);

    delete_matrix(mat_copy, N);

    return max;
}


float **add_matrix(float **matA, float **matB, int N){
    for (int i = 0; i < N; ++i) {
        __m128 *mm_matA = (__m128*)matA[i];
        __m128 *mm_matB = (__m128*)matB[i];
        for (int j = 0; j < N / 4; ++j) {
            __m128 sum = _mm_add_ps(mm_matA[j], mm_matB[j]);
            _mm_store_ps(&matA[i][j * 4], sum);
        }
    }

    return matA;
}

float **sub_matrix(float **matA, float **matB, int N){
    for (int i = 0; i < N; ++i) {
        __m128 *mm_matA = (__m128*)matA[i];
        __m128 *mm_matB = (__m128*)matB[i];
        for (int j = 0; j < N / 4; ++j) {
            __m128 dif = _mm_sub_ps(mm_matA[j], mm_matB[j]);
            _mm_store_ps(&matA[i][j * 4], dif);
        }
    }

    return matA;
}


float **mul_matrix(float **matA, float **matB, int N){
    float **matC = create_zero_matrix(N);

    float **copy_matB = transpose_matrix(matB, N);

    for (int i = 0; i < N; ++i){
        __m128 *mm_matA = (__m128*)matA[i];
        for (int j = 0; j < N; ++j) {
            __m128 *mm_matB = (__m128*)copy_matB[j];
            __m128 s = _mm_setzero_ps();
            for (int k = 0; k < N / 4; ++k) {
                __m128 p = _mm_mul_ps(mm_matA[k], mm_matB[k]);
                s = _mm_add_ps(s,p);
            }
            __m128 copy_s = _mm_movehl_ps(s, s);
            s = _mm_add_ps(s, copy_s);
            __m128 copy_s2 = _mm_shuffle_ps(s, s,1);
            s = _mm_add_ss(s, copy_s2);

            float fsum;
            _mm_store_ss(&fsum, s);
            matC[i][j] = fsum;
        }
    }

    delete_matrix(matA, N);
    delete_matrix(copy_matB, N);

    matA = matC;

    return matA;
}



float** div_mat_by_num(float **mat, int N, float num){
    for (int i = 0; i < N; ++i){
        __m128 *mm_mat = (__m128*)mat[i];
        for (int j = 0; j < N / 4; ++j) {
            __m128 vect = _mm_set1_ps(num);
            vect = _mm_div_ps(mm_mat[j], vect);
            _mm_store_ps(&mat[i][j * 4], vect);
        }
    }

    return mat;
}


float **create_B_matrix(float **matA, int N){
    float **trans_matA = transpose_matrix(matA, N);

    float denom = columns_norm(matA, N) * rows_norm(matA, N);

    trans_matA = div_mat_by_num(trans_matA, N, denom);

    return trans_matA;

}

float **create_R_matrix(float **matA, int N) {
    float **mat_unit = create_unit_matrix(N);

    float **matB = create_B_matrix(matA, N);

    matB = mul_matrix(matB, matA, N);

    mat_unit = sub_matrix(mat_unit, matB, N);

    return mat_unit;
}



float **invert_matrix(float **matA, int N, int M){
    float **unit_mat = create_unit_matrix(N);

    float **matB = create_B_matrix(matA, N);

    float **sq_matR = create_unit_matrix(N);

    float **matR = create_R_matrix(matA, N);

    for (int i = 0; i < M - 1; ++i){
        sq_matR = mul_matrix(sq_matR, matR, N);
        unit_mat = add_matrix(unit_mat, sq_matR, N);
    }

    unit_mat = mul_matrix(unit_mat, matB, N);

    delete_matrix(matB, N);
    delete_matrix(sq_matR, N);
    delete_matrix(matR, N);

    return unit_mat;
}
