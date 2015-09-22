CC = gcc
OPTIMIZE_FLAGS= -O3
CFLAGS = -mavx -std=c99

all: avx_simd avx_simd_pthread baseline baseline_pthread

avx_simd: avx_simd.o
	$(CC) -o avx_simd avx_simd.o

avx_simd_pthread: avx_simd_pthread.o
	$(CC) -pthread -o avx_simd_pthread avx_simd_pthread.o

baseline: baseline.o
	$(CC) -o baseline baseline.o

baseline_pthread: baseline_pthread.o
	$(CC) -pthread -o baseline_pthread baseline_pthread.o


avx_simd.o: avx_simd.c
	$(CC) $(CFLAGS) -pthread avx_simd.c -c

avx_simd_pthread.o: avx_simd_pthread.c
	$(CC) $(CFLAGS) avx_simd_pthread.c -c

baseline.o: baseline.c
	$(CC) $(CFLAGS) baseline.c -c

baseline_pthread.o: baseline_pthread.c
	$(CC) $(CFLAGS) -D_GNU_SOURCE baseline_pthread.c -c


clean:
	rm -rf avx_simd.o avx_simd_pthread.o avx_simd_opt.o baseline.o baseline_pthread.o
