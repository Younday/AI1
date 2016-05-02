#ifndef HEAP_H
#define HEAP_H


typedef struct State {
  int x,y;
  int f,g,h;
  int formerstate;
} State;

typedef struct Heap {
  State *array;
  int front;
  int size;
} Heap;


void doubleHeapSize(Heap *hp);
Heap makeHeap ();
int isEmptyHeap (Heap h);
void heapEmptyError ();
void enqueue(int x,int y,int g, int f, Heap *hp);
State removeMin(Heap *hp);
void downheap(Heap *hp, int n);
void upHeap(Heap *hp, int n);
void swap (State *pa, State *pb);

#endif
