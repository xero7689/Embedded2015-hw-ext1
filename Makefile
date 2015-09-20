CC = gcc
OPTIMIZE_FLAGS= -O0
CFLAGS = -mavx -std=c99 $(OPTIMIZE_FLAGS)

all: avx_simd baseline

avx_simd: avx_simd.o
	$(CC) -o avx_simd avx_simd.o

baseline: baseline.o
	$(CC) -o baseline baseline.o

avx_simd.o: avx_simd.c
	$(CC) $(CFLAGS) avx_simd.c -c

baseline.o: baseline.c
	$(CC) $(CFLAGS) baseline.c -c

clean:
	rm -rf avx_simd.o baseline.o
