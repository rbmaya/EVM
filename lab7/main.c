#include <stdio.h>
#include <time.h>
#include "blas_invert_matrix.h"

static void checking_invert_matrix(float **mat, float **inv_mat, int N){
    mat = mul_matrix(mat, inv_mat, N);

    printf("Check: \n");

    int flag = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) {
                if ((mat[i][j] > 1.1) || (mat[i][j] < 0.9)){
                    printf("Wrong element in main diagonal: %.6f \n", mat[i][j]);
                    flag = 1;
                }
            } else if ((mat[i][j] > 0.1) || (mat[i][j] < -0.1)) {
                printf("Just Wrong element: %.6f \n", mat[i][j]);
                flag = 1;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            printf("%f ", mat[i][j]);
        printf("\n");
    }

    if (flag) printf("Wrong result!\n");
    else printf("Right result!\n");
}

int main() {

    int N = 0;

    printf("Enter the number N: ");
    scanf("%d", &N);

    int M = 0;

    printf("Enter the number M: ");
    scanf("%d", &M);

    float **mat = create_random_matrix(N);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    float **inv_mat = invert_matrix(mat, N, M);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("Time taken: %lf sec.\n", end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));

    printf("Invert matrix: \n");

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            printf("%f ", inv_mat[i][j]);
        printf("\n");
    }

    checking_invert_matrix(mat, inv_mat, N);

    delete_matrix(mat, N);
    delete_matrix(inv_mat, N);

    return 0;
}
