#include <stdio.h>
#include <stdlib.h>

#define NUM_FRAG 32
#define NUM_TRAV 10
#define SIZE_CACHE 32768
//#define SIZE_CACHE 262144
//#define SIZE_CACHE 6291456

int main() {

    for (size_t frag_count = 1; frag_count <= NUM_FRAG; ++frag_count){

        size_t *arr = malloc(SIZE_CACHE * frag_count);

        size_t N = (SIZE_CACHE * frag_count) / sizeof(size_t);

        size_t offset = SIZE_CACHE / (sizeof(size_t));

        for (size_t j = 0; j < N; ++j) {
            arr[j] = (j + offset);
            if (arr[j] >= N) {
                arr[j] %= N;
                arr[j]++;
            }
        }
        arr[N - 1] = 0;

        union ticks {
            unsigned long long t64;
            struct s32 {
                long th, tl;
            } t32;
        } start, end;

        unsigned long long min_time = 100000;

        for (size_t l = 0; l < NUM_TRAV; ++l) {
            asm("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl));
            start.t32.tl <<= 32;
            start.t32.th |= start.t32.tl;

            for (size_t k = 0, j = 0; j < N; ++j)
                k = arr[k];

            asm("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl));
            end.t32.tl <<= 32;
            end.t32.th |= end.t32.tl;

            unsigned long long time = (end.t64-start.t64) / N;
            if (time < min_time) min_time = time;
        }

        printf("Number of fragments = %zu. ", frag_count);

        printf("Time taken: %llu ticks.\n", min_time);

        free(arr);

    }

    return 0;
}
