#include <stdio.h>
#include <stdlib.h>

#define MAX 0
#define MIN 1

#define INFINITY 9999999

int negaMax(int state) {
  int move, bestmove, best = -INFINITY;
  int m;
  if (state == 1) {
    return -1;
  }
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) {
      m =  -negaMax(state - move);
      if (m > best) {
        best = m;
        bestmove = move;
      }
    }
  }
  return bestmove;
}


void playNim(int state) {
  int turn = 0;
  while (state != 1) {
    int action = negaMax(state);
    printf("%d: %s takes %d\n", state,
           (turn==MAX ? "Max" : "Min"), action);
    state = state - action;
    turn = 1 - turn;
  }
  printf("1: %s looses\n", (turn==MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
  if ((argc != 2) || (atoi(argv[1]) < 3)) {
    fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
    fprintf(stderr, "<number of sticks> must be at least 3!\n");
    return -1;
  }

  playNim(atoi(argv[1]));

  return 0;
}
