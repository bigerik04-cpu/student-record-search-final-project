#include "common.h"
static Student *build_array(size_t n) {
    Student *a=malloc(n*sizeof(*a));
    if(!a){perror("malloc");exit(EXIT_FAILURE);}
    for(size_t i=0;i<n;i++) a[i]=(Student){(int)(i*2),score_for(i)};
    return a;
}
__attribute__((noinline)) static int lookup(const Student *a,size_t n,int id) {
    size_t left=0,right=n;
    while(left<right) {
        size_t mid=left+(right-left)/2;
        if(a[mid].id<id) left=mid+1;
        else right=mid;
    }
    return left<n && a[left].id==id ? a[left].score : -1;
}
int main(void) {
    const size_t sizes[]={1000,10000,100000,1000000};
    puts("records,median_ns_per_query,checksum");
    for(size_t s=0;s<sizeof(sizes)/sizeof(sizes[0]);s++) {
        size_t n=sizes[s];Student *a=build_array(n);
        int *queries=malloc(QUERY_COUNT*sizeof(*queries));
        if(!queries){perror("malloc");free(a);return EXIT_FAILURE;}
        generate_queries(queries,n);
        uint64_t expected=0;
        for(size_t j=0;j<QUERY_COUNT;j++) expected+=(uint64_t)(lookup(a,n,queries[j])+1);
        double samples[REPEATS];
        for(int r=-WARMUPS;r<REPEATS;r++) {
            uint64_t check=0;double start=now_seconds();
            for(size_t j=0;j<QUERY_COUNT;j++) check+=(uint64_t)(lookup(a,n,queries[j])+1);
            double elapsed=now_seconds()-start;
            if(check!=expected){fputs("ERROR: checksum mismatch\n",stderr);return EXIT_FAILURE;}
            sink=check;
            if(r>=0) samples[r]=elapsed*1e9/QUERY_COUNT;
        }
        printf("%zu,%.3f,%llu\n",n,median(samples),(unsigned long long)expected);
        free(queries);free(a);
    }
    return 0;
}
