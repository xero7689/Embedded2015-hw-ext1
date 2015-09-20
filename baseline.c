z#include <stdio.h>
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
        FILE *fPtr;
        fPtr = fopen("baseline.txt", "w+");
        int ds;
        double start, finish;
        for (ds = 0; ds <= DT_SIZE; ds++) {
            start = clock();
            compute_pi(ds*1000000);
            finish = clock();
            fprintf(fPtr, "%d\t%lf\n", ds, (double)(finish-start)/CLOCKS_PER_SEC);
            fflush(fPtr);
        }
    return 0;
}
