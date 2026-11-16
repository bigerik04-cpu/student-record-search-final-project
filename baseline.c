#include "common.h"
typedef struct Node { Student student; struct Node *next; } Node;
static Node *build_list(size_t n) {
    Node *head=NULL, *tail=NULL;
    for(size_t i=0;i<n;i++) {
        Node *node=malloc(sizeof(*node));
        if(!node) {perror("malloc"); exit(EXIT_FAILURE);}
        node->student=(Student){(int)(i*2),score_for(i)};
        node->next=NULL;
        if(tail) tail->next=node; else head=node;
        tail=node;
    }
    return head;
}
__attribute__((noinline)) static int lookup(const Node *head,int id) {
    for(const Node *p=head;p;p=p->next)
        if(p->student.id==id) return p->student.score;
    return -1;
}
static void free_list(Node *head) {while(head){Node *next=head->next;free(head);head=next;}}
int main(void) {
    const size_t sizes[]={1000,10000,100000,1000000};
    puts("records,median_ns_per_query,checksum");
    for(size_t s=0;s<sizeof(sizes)/sizeof(sizes[0]);s++) {
        size_t n=sizes[s]; Node *head=build_list(n);
        int *queries=malloc(QUERY_COUNT*sizeof(*queries));
        if(!queries){perror("malloc");free_list(head);return EXIT_FAILURE;}
        generate_queries(queries,n);
        uint64_t expected=0;
        for(size_t j=0;j<QUERY_COUNT;j++) expected+=(uint64_t)(lookup(head,queries[j])+1);
        double samples[REPEATS];
        for(int r=-WARMUPS;r<REPEATS;r++) {
            uint64_t check=0;double start=now_seconds();
            for(size_t j=0;j<QUERY_COUNT;j++) check+=(uint64_t)(lookup(head,queries[j])+1);
            double elapsed=now_seconds()-start;
            if(check!=expected){fputs("ERROR: checksum mismatch\n",stderr);return EXIT_FAILURE;}
            sink=check;
            if(r>=0) samples[r]=elapsed*1e9/QUERY_COUNT;
        }
        printf("%zu,%.3f,%llu\n",n,median(samples),(unsigned long long)expected);
        free(queries);free_list(head);
    }
    return 0;
}
