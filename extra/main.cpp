#include <iostream>
#include <thread>

#define ITER 10000000
#define SIZE 60
#define NUM_OFFSETS 30


void func(int *arr){
    for (size_t i = 0; i < ITER; i++) {
        (*arr)++;
    }
}

int main() {
    alignas(64) int arr[SIZE];

    for (int offset = 1; offset <= NUM_OFFSETS; offset++) {

        std::cout << "OFFSET = " << offset << ". ";

        std::thread thread1(func, &arr[0]);
        std::thread thread2(func, &arr[offset]);

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        thread1.join();
        thread2.join();

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        printf("Time taken: %.0lf ms.\n", (end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec))*1000);
    }

    return 0;
}
