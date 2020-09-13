#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_SIZE 1024
#define MAX_SIZE 32000000
#define NUM_TRAV 7


void direct_traversal(size_t *arr, size_t N){
    arr[N - 1] = 0;

    for (int i = 0; i < N - 1; ++i)
        arr[i] = i + 1;
}


void back_traversal(size_t *arr, size_t N){
    arr[0] = (int)N - 1;

    for (int i = 1; i < N; ++i)
        arr[i] = i - 1;
}


void rand_traversal(size_t *arr, size_t N){

    for (size_t i = 0; i < N; ++i)
        arr[i] = i;

    size_t size = N;

    while (size > 1){
        --size;
        size_t j = rand() % size;
        int tmp = arr[j];
        arr[j] = arr[size];
        arr[size] = tmp;
    }

}


int main() {
    double fact = 2;

    srand(time(NULL));


    for (size_t arr_size = MIN_SIZE; arr_size <= MAX_SIZE; arr_size *= fact){

        if ((arr_size >= 4000000) && (arr_size <= 8000000)) fact = 1.5;

        size_t N = arr_size / sizeof(size_t);

        size_t *arr = malloc(arr_size);

        //direct_traversal(arr, N);
        rand_traversal(arr, N);

        //back_traversal(arr, N);

        size_t check = 0;
        for (size_t k = 0, j = 0; j < N; ++j) {
            check += k;
            k = arr[k];
        }

        size_t check2 = N * (N - 1) / 2;

        printf("Recd = %zu. Expected = %zu. ", check, check2);

        unsigned long long min_time = 100000;
        for (int m = 0; m <= NUM_TRAV; ++m) {


            for (size_t k = 0, j = 0; j < N; ++j)
                k = arr[k];

            asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
            end.t32.tl <<= 32;
            end.t32.th |= end.t32.tl;

            unsigned long long time = (end.t64-start.t64) / N;

            if (time < min_time) min_time = time;
        }


        size_t size = arr_size / MIN_SIZE;

        if (size < 1000) printf("Size = %zu KB.", size);
        else {
            size /= 1000;
            printf("Size = %zu MB.", size);
        }

        printf("Time taken: %llu ticks.\n", min_time);

        free(arr);

    }

    return 0;
}