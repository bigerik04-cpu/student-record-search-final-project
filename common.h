#ifndef COMMON_H
#define COMMON_H
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#define QUERY_COUNT 200
#define REPEATS 7
#define WARMUPS 2
static volatile uint64_t sink = 0;
typedef struct { int id; int score; } Student;
static double now_seconds(void) { struct timespec ts; if (clock_gettime(CLOCK_MONOTONIC,&ts)!=0) {perror("clock_gettime"); exit(EXIT_FAILURE);} return ts.tv_sec + ts.tv_nsec*1e-9; }
static uint32_t rng_state;
static uint32_t rng_next(void) { uint32_t x=rng_state; x ^= x<<13; x ^= x>>17; x ^= x<<5; return rng_state=x; }
static void generate_queries(int *queries, size_t n) { rng_state=0x12345678u; for(size_t i=0;i<QUERY_COUNT;i++){size_t k=(size_t)(rng_next()%n); queries[i]=(int)(k*2 + ((i&1u)?1:0));} }
static int compare_double(const void *a,const void *b) { double x=*(const double*)a,y=*(const double*)b; return (x>y)-(x<y); }
static double median(double *values) { qsort(values,REPEATS,sizeof(double),compare_double); return values[REPEATS/2]; }
static int score_for(size_t i) {return (int)((i*17u+23u)%101u);}
#endif
