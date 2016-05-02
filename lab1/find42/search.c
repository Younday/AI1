#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "state.h"
#include "fringe.h"


#define RANGE 1000000


typedef struct Stack{
	int *array;
	int top;
	int size;
} Stack;

Stack newStack(int size){
	Stack st;
	st.array = malloc(size * sizeof(int));
	if(st.array == NULL){
		printf("ERROR, array could not be made\n");
		exit(-1);
	}
	st.top = 0;
	st.size = size;
	return st;
}


void push(int value, Stack *stp){
	stp->array[stp->top]= value;
	stp->top++;
}

int pop(Stack *stp){
	(stp->top)--;
	return (stp->array)[stp->top];
}

void findPath(int length, int index, int cost, int start) {
  Stack s;
  int mode;
  int current;
  int j = 0;
  int buffer;
  int *array;
  array = malloc(length*sizeof(int));
  assert(array != NULL);
  s = newStack(length);
  while(index > 0) {
    mode = index%6;
    push(mode,&s);
    index = index/6;
    if (mode ==0) {
      index--;
    }
  }
  buffer = s.top;

  while((s.top) >= 0) {
    array[j] = pop(&s);
    j++;
  }
  printf("%d ", start);
  current = start;
  for (j=0; j < buffer; j++) {
    switch (array[j]) {
      case 1:
        current += 1;
        printf("(+1) -> %d ", current);
        break;
      case 2:
        current *= 2;
        printf("(*2) -> %d ", current);
        break;
      case 3:
        current *= 3;
        printf("(*3) -> %d ", current);
        break;
      case 4:
        current -= 1;
        printf("(-1) -> %d ", current);
        break;
      case 5:
        current /= 2;
        printf("(/2) -> %d ", current);
        break;
      case 6:
        current /= 3;
        printf("(/3) -> %d ", current);
        break;
    }
  }
  printf("\nlength: %d, cost: %d\n", length, cost);
}


Fringe insertValidSucc(Fringe fringe, int value, int index, int length, int cost) {
  State s;
  if ((value < 0) || (value > RANGE)) {
    /* ignore states that are out of bounds */
    return fringe;
  }
  s.value = value;
  s.index = index;
  s.length = length;
  s.cost = cost;
  return insertFringe(fringe, s);
}



void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int i = 0;
  int goalReached = 0;
  int visited = 0;
  int value;
  int length = 0;
  int cost = 0;

	if(mode == PRIO || mode == HEAP) {
		fringe.size++;
	}

  fringe = makeFringe(mode);
  state.value = start;
  state.length = 0;
  state.cost = 0;
  fringe = insertFringe(fringe, state);

  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    visited++;
    /* is state the goal? */
    value = state.value;
    i = state.index;
    length = state.length;
    cost = state.cost;
    if (value == goal) {
      goalReached = 1;
      break;
    }
    /* insert neighbouring states */
    fringe = insertValidSucc(fringe, value+1, 6*i+1, length+1, cost+1); /* rule n->n + 1      */
    if (value != 0) {
      fringe = insertValidSucc(fringe, 2*value, 6*i+2, length+1, cost+2); /* rule n->2*n        */
      fringe = insertValidSucc(fringe, 3*value, 6*i+3, length+1, cost+2); /* rule n->3*n        */
      fringe = insertValidSucc(fringe, value-1, 6*i+4, length+1, cost+1); /* rule n->n - 1      */
      if (value != 1) {
          fringe = insertValidSucc(fringe, value/2, 6*i+5, length+1, cost+3); /* rule n->floor(n/2) */
          fringe = insertValidSucc(fringe, value/3, 6*i+6, length+1, cost+3); /* rule n->floor(n/3) */
        }
	     }
  }

  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
    findPath(length, i,cost, start);
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);
}



int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;

  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }

  start = 0;
  goal = 42;
  if (argc == 3) {
    goal = atoi(argv[2]);
  } else if (argc == 4) {
    start = atoi(argv[2]);
    goal = atoi(argv[3]);
  }

  printf("Problem: route from %d to %d\n", start, goal);
  search(fringetype, start, goal);
  return EXIT_SUCCESS;
}
