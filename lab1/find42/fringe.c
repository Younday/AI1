#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "fringe.h"


void enqueue(Fringe *fringe, State s){
  int fr = fringe->front;
  fringe->states[fr] = s;
  upHeap(fringe,fr);
  fringe->front=fr+1;
}

State removeMin(Fringe *fringe){
  State s;
  s = fringe->states[1];
  fringe->front--;
  swap(&(fringe->states[1]),&(fringe->states[fringe->front]));
  downheap(fringe,1);
  return s;
}
/*
void doubleHeapSize(Fringe *fringe){
  int newSize=2*fringe->size;
  fringe->states = realloc (fringe->states, newSize * sizeof(State));
  assert(fringe->states!=NULL);
    fringe->size=newSize;
}*/

void downheap(Fringe *fringe, int n) {
  int fr = fringe->front;
  int indexMax = n;
  if (fr < 2*n + 1) {
    return;
  }
  if (fringe->states[n].cost > fringe->states[2*n].cost) {
    indexMax = 2*n;
  }
  if (fr < 2*n + 1 && fringe->states[indexMax].cost > fringe->states[2*n +1].cost) {
    indexMax = 2*n + 1;
  }
  if (indexMax != n) {
    swap(&(fringe->states[n]), &(fringe->states[indexMax]));
    downheap(fringe,indexMax);
  }
}

void upHeap(Fringe *fringe, int n){
  if (n==1){
    return;
  }
  if (fringe->states[n].cost > fringe->states[n/2].cost){
    return;
  }
  if (fringe->states[n].cost < fringe->states[n/2].cost){
    swap(&(fringe->states[n]),&(fringe->states[n/2]));
    upHeap(fringe, n/2);
  }
}

void swap (State *pa, State *pb) {
  State h = *pa;
  *pa = *pb;
  *pb = h;
}


Fringe makeFringe(int mode) {
  /* Returns an empty fringe.
   * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP)
   */
  Fringe f;
  if ((mode != LIFO) && (mode != STACK) && (mode != FIFO) &&
      (mode != PRIO) && (mode != HEAP)) {
    fprintf(stderr, "makeFringe(mode=%d): incorrect mode. ", mode);
    fprintf(stderr, "(mode <- [LIFO,STACK,FIFO,PRIO,HEAP])\n");
    exit(EXIT_FAILURE);
  }
  f.mode = mode;
  f.size = f.front = f.rear = 0; /* front+rear only used in FIFO mode */
  f.states = malloc(MAXF*sizeof(State));
  if (f.states == NULL) {
	fprintf(stderr, "makeFringe(): memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  f.maxSize = f.insertCnt = f.deleteCnt = 0;
  return f;
}

void deallocFringe(Fringe fringe) {
  /* Frees the memory allocated for the fringe */
  free(fringe.states);
}

int getFringeSize(Fringe fringe) {
  /* Returns the number of elements in the fringe
   */
  return fringe.size;
}

int isEmptyFringe(Fringe fringe) {
  /* Returns 1 if the fringe is empty, otherwise 0 */
  return (fringe.size == 0 ? 1 : 0);
}

Fringe insertFringe(Fringe fringe, State s, ...) {
  /* Inserts s in the fringe, and returns the new fringe.
   * This function needs a third parameter in PRIO(HEAP) mode.
   */
  if (fringe.size == MAXF) {
    fprintf(stderr, "insertFringe(..): fatal error, out of memory.\n");
    exit(EXIT_FAILURE);
  }
  fringe.insertCnt++;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
    fringe.states[fringe.size] = s;
    break;
  case FIFO:
    fringe.states[fringe.rear++] = s;
    fringe.rear %= MAXF;
    break;
  case PRIO: /* PRIO == HEAP */
  case HEAP:
     enqueue(&fringe, s);
     break;
  }
  fringe.size++;
  printf("%d\n", fringe.size);
  if (fringe.size > fringe.maxSize) {
    fringe.maxSize = fringe.size;
  }
  return fringe;
}

Fringe removeFringe(Fringe fringe, State *s) {
  /* Removes an element from the fringe, and returns it in s.
   * Moreover, the new fringe is returned.
   */
  if (fringe.size < 1) {
    fprintf(stderr, "removeFringe(..): fatal error, empty fringe.\n");
    exit(EXIT_FAILURE);
  }
  fringe.deleteCnt++;
  fringe.size--;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
    *s = fringe.states[fringe.size];
    break;
  case FIFO:
    *s = fringe.states[fringe.front++];
    fringe.front %= MAXF;
    break;
  case PRIO: /* PRIO == HEAP */
  case HEAP:
    *s = fringe.states[0];
    removeMin(&fringe);
    break;
  }
  return fringe;
}

void showStats(Fringe fringe) {
  /* Shows fringe statistics */
  printf("#### fringe statistics:\n");
  printf(" #size        : %7d\n", fringe.size);
  printf(" #maximum size: %7d\n", fringe.maxSize);
  printf(" #insertions  : %7d\n", fringe.insertCnt);
  printf(" #deletions   : %7d\n", fringe.deleteCnt);
  printf("####\n");
}
