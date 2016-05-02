#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"

Heap makeHeap () {
  Heap h;
  h.array = malloc(1*sizeof(State));
  assert(h.array != NULL);
  h.front = 1;
  h.size = 1;
  return h;
}

int isEmptyHeap (Heap h) {
  return (h.front == 1);
}

void heapEmptyError () {
  printf("heap empty \n");
  abort();
}

void doubleHeapSize(Heap *hp){
  int newSize=2*hp->size;
  hp->array = realloc (hp->array, newSize * sizeof(State));
  assert(hp->array!=NULL);
    hp->size=newSize;
}

void enqueue(int x,int y,int g, int f, Heap *hp){
  int fr = hp->front;
  if (fr == hp-> size){
    doubleHeapSize(hp);
  }
  hp->array[fr].x = x;
  hp->array[fr].y = y;
  hp->array[fr].g = g;
  hp->array[fr].f = f;
  upHeap(hp,fr);
  hp->front=fr+1;
}

State removeMin(Heap *hp){
  State s;
  if (isEmptyHeap(*hp)){
    heapEmptyError();
  }
  s = hp->array[1];
  hp->front--;
  swap(&(hp->array[1]),&(hp->array[hp->front]));
  downheap(hp,1);
  return s;
}

void downheap(Heap *hp, int n) {
  int fr = hp->front;
  int indexMax = n;
  if (fr < 2*n + 1) {
    return;
  }
  if (hp->array[n].f >= hp->array[2*n].f) {
    indexMax = 2*n;
  }
  if (fr < 2*n + 1 && hp->array[indexMax].f >= hp->array[2*n +1].f) {
    indexMax = 2*n + 1;
  }
  if (indexMax != n) {
    swap(&(hp->array[n]), &(hp->array[indexMax]));
    downheap(hp,indexMax);
  }
}

void upHeap(Heap *hp, int n){
  if (n==1){
    return;
  }
  if (hp->array[n].f >= hp->array[n/2].f){
    return;
  }
  if (hp->array[n].f <= hp->array[n/2].f){
    swap(&(hp->array[n]),&(hp->array[n/2]));
    upHeap(hp, n/2);
  }
}

void swap (State *pa, State *pb) {
  State h = *pa;
  *pa = *pb;
  *pb = h;
}
