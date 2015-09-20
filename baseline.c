#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define DT_SIZE 128

double compute_pi(size_t dt)
{
    double pi = 0.0;
    double delta = 1.0 / dt;
    size_t i;
    for (i = 0; i < dt; i++) {
        double x = (double) i / dt;
        pi += delta / (1.0 + x * x);
    }
    return pi*4;
}

int main(void)
{
    double time_elapsed[DT_SIZE];
    int ds;
    double pi;
    double start, finish;
    
    for (ds = 0; ds < DT_SIZE; ds++) {
        start = clock();
        pi = compute_pi(ds*1000000);
        finish = clock();
        time_elapsed[ds] = (double)(finish-start)/CLOCKS_PER_SEC;
        //printf("pi= %lf\tdelta_size= %d\ttime= %lf\n", pi, ds, (double)(finish-start)/CLOCKS_PER_SEC);
    }
    
    FILE *fp;
    fp = fopen("baseline.txt", "w+");
    int i;
    fprintf(fp, "#delta_size\ttime\n");
    for (i = 0; i < DT_SIZE; i++) {
        fprintf(fp, "%d\t%lf\n", i, time_elapsed[i]);
    }
    fclose(fp);
    return 0;
}
