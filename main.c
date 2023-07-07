#include <stdio.h>
#define SV_IMPLEMENTATION
#include "sv.h"

#define MAXV    100

typedef struct edgenode {
    String_View word;
    double weight;
    struct edgenode *next;
} EdgeNode;


typedef struct {
    EdgeNode **edges;
    int *degree;
    int nvertices;
    int nedges;
    int directed;
} Graph;

void init_Graph(Graph *g, bool directed) {
    g->nvertices = 0;
    g->nedges = 0;
    g->directed = directed;

    
}

void read_Graph(Graph *g, bool directed) {
    // TODO
}




int main(int argc, char* argv) {
    printf("Hello World\n");
    return 0;
}