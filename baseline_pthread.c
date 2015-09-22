#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define DT_SIZE 128
#define THREAD_NUM 4

double pi = 0.0;
pthread_barrier_t barrier;

typedef struct thread_argument {
    double delta;
    size_t dt;
    int start_index;
    int end_index;
} thread_argument;

void* compute_pi_pthread(void *argument)
{
    thread_argument *args = argument;
    double delta = args->delta;
    size_t dt = args->dt;
    int start_index = args->start_index;
    int end_index = args->end_index;
    double x, tmp_pi = 0.0;
    for (int i = start_index; i < end_index; i++) {
        x = (double) i / dt;
        tmp_pi += delta / (1.0 + x * x);
    }
    pi += 4 * tmp_pi;
    pthread_barrier_wait(&barrier);
}

void compute_pi(size_t dt)
{
    // Init Pthread
    pthread_t thread[THREAD_NUM];  // array of thread
    thread_argument ta[THREAD_NUM];  // array of different thread argument
        
    double delta = 1.0 / dt;
    
    // count start and finish
    int block_size = dt / THREAD_NUM;
    for (int i = 0; i < THREAD_NUM; i++) {
        ta[i].delta = delta;
        ta[i].dt = dt;
        ta[i].start_index = i * block_size;
        ta[i].end_index = i * block_size + block_size - 1;
        pthread_create(&thread[i], NULL, &compute_pi_pthread, (void *)&ta[i]);
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(thread[i], NULL);
    }
    //printf("pi=%lf\n", pi);
}

int main(void)
{
    double time_elapsed[DT_SIZE];
    int ds;
    double pi;
    double start, finish;
    pthread_barrier_init(&barrier, NULL, (unsigned) THREAD_NUM);
    
    for (ds = 0; ds < DT_SIZE; ds++) {
        start = clock();
        compute_pi(ds*1000000);
        finish = clock();
        time_elapsed[ds] = (double)(finish-start)/CLOCKS_PER_SEC;
        pi = 0.0;
        //printf("pi= %lf\tdelta_size= %d\ttime= %lf\n", pi, ds, (double)(finish-start)/CLOCKS_PER_SEC);
    }
    
    FILE *fp;
    fp = fopen("baseline_pthread.txt", "w+");
    int i;
    fprintf(fp, "#delta_size\ttime\n");
    for (i = 0; i < DT_SIZE; i++) {
        fprintf(fp, "%d\t%lf\n", i, time_elapsed[i]);
    }
    fclose(fp);
    return 0;
}
