#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"

#define RANGE 1000000

Fringe insertValidSucc(Fringe fringe, int value, int prev) {
  State s;
  if ((value < 0) || (value > RANGE)) {
    /* ignore states that are out of bounds */
    return fringe;
  }
  s.value = value;
  s.prev = prev;
  return insertFringe(fringe, s);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int i;
  int goalReached = 0;
  int visited = 0;
  int value, prev1;
  int *prev;
  prev = malloc((3*goal)*sizeof(int));

  fringe = makeFringe(mode);
  state.value = start;
  fringe = insertFringe(fringe, state);
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    visited++;
    /* is state the goal? */
    value = state.value;
    prev1 = state.prev;
    prev[prev1] = value;
    for(i = 0; i < 3*goal; i++) {
      if(prev[i] <= value && prev[i] > 0) {
        printf("%d ", prev[i]);
      }
    }
    if (value == goal) {
      goalReached = 1;
      break;
    }
    /* insert neighbouring states */
    fringe = insertValidSucc(fringe, value+1, value); /* rule n->n + 1      */
    if (value != 0) {
      fringe = insertValidSucc(fringe, 2*value, value); /* rule n->2*n        */
      fringe = insertValidSucc(fringe, 3*value, value); /* rule n->3*n        */
      fringe = insertValidSucc(fringe, value-1, value); /* rule n->n - 1      */
      if (value != 1 && value % 3 ==0) {
          fringe = insertValidSucc(fringe, value/2, value); /* rule n->floor(n/2) */
          fringe = insertValidSucc(fringe, value/3, value); /* rule n->floor(n/3) */
        }
	}
  }

  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);
  free(prev);
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
