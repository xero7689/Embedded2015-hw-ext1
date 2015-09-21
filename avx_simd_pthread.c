#include <immintrin.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <malloc.h>
#include <time.h>

#define DT_SIZE 128
#define THREAD_NUM 1

double pi = 0.0;

typedef struct thread_argument {
    double delta;
    int start_index;
    int end_index;
} thread_argument;

void* compute_pi_pthread(void *argument)
{
    thread_argument *args = argument;
    double delta = args->delta;
    int start_index = args->start_index;
    int end_index = args->end_index;
    printf("delta=%lf, bsi=%d, esi=%d\n", delta, start_index, end_index);
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4;
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(delta);
    ymm2 = _mm256_set_pd(delta * 3 + start_index, \
                         delta * 2 + start_index, \
                         delta * 1 + start_index, \
                         0.0 + start_index);
    ymm4 = _mm256_setzero_pd();
    
    for (int i = start_index; i <= end_index - 4; i += 4) {
        ymm3 = _mm256_set1_pd(i * delta);
        ymm3 = _mm256_add_pd(ymm3, ymm2);
        ymm3 = _mm256_mul_pd(ymm3, ymm3);
        ymm3 = _mm256_add_pd(ymm0, ymm3);
        ymm3 = _mm256_div_pd(ymm1, ymm3);
        ymm4 = _mm256_add_pd(ymm4, ymm3);
    }
    
    //double tmp[4] __attribute__((aligned(32)));
    double *tmp = (double *)memalign(32, 4*sizeof(double));
    double tmp_pi;
    _mm256_store_pd(tmp, ymm4);
    tmp_pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    printf("tmp_pi= %lf\n", tmp_pi);
    pi += tmp_pi;
    free(tmp);
    pthread_exit(NULL);
}

double compute_pi_re(size_t dt)
{
    // Init Pthread
    pthread_t thread[THREAD_NUM];  // array of thread
    thread_argument ta[THREAD_NUM];  // array of different thread argument
        
    double delta = 1.0 / dt;
    
    // count start and finish
    int block_size = dt / THREAD_NUM;
    for (int i = 0; i < THREAD_NUM; i++) {
        ta[i].delta = delta;
        ta[i].start_index = i * block_size;
        ta[i].end_index = i * block_size + block_size - 1;
        pthread_create(&thread[i], NULL, &compute_pi_pthread, (void *)&ta[i]);
        pthread_join(thread[i], NULL);
    }
    printf("pi=%lf\n", 4*pi);
}

/*
double compute_pi(size_t dt)
{
    double pi = 0.0;
    double delta = 1.0 / dt;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4;                                                                          
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(delta);
    ymm2 = _mm256_set_pd(delta * 3, delta * 2, delta * 1, 0.0);
    ymm4 = _mm256_setzero_pd();

    for (int i = 0; i <= dt - 4; i += 4) {
        ymm3 = _mm256_set1_pd(i * delta);
        ymm3 = _mm256_add_pd(ymm3, ymm2);
        ymm3 = _mm256_mul_pd(ymm3, ymm3);
        ymm3 = _mm256_add_pd(ymm0, ymm3);
        ymm3 = _mm256_div_pd(ymm1, ymm3);
        ymm4 = _mm256_add_pd(ymm4, ymm3);
    }
    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, ymm4);
    pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    return pi * 4.0;
}
*/

int main(void) {
    compute_pi_re(10000000);
    /*
    double time_elapsed[DT_SIZE];
    int ds;
    double start, finish;
    
    for (ds = 1; ds <= DT_SIZE; ds++) {
        start = clock();
        compute_pi(ds*1000000);
        finish = clock();
        time_elapsed[ds] = (double)(finish-start)/CLOCKS_PER_SEC;
    }
    
    FILE *fp;
    fp = fopen("avx_simd.txt", "w+");
    int i;
    fprintf(fp, "#delta_size\ttime\n");
    for (i = 0; i < DT_SIZE; i++) {
        fprintf(fp, "%d\t%lf\n", i, time_elapsed[i]);
    }
    fclose(fp);
    */
    return 0;
}
