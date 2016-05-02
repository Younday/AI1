#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "heap.h"
#include "a.h"


int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};

int isValidLocation(int x, int y) {
  return (0<=x && x < 500 && 0<= y && y < 500);
}


int realDistance (int travelled){
  travelled = travelled + 3;
  return travelled;
}

int heuristic1(int x, int y, int goalX, int goalY) {
  int distance = sqrt(pow((goalX-x),2) + pow((goalY-y),2));
  return distance;
}

int heuristic2(int x, int y, int goalX, int goalY) {
  int distance = abs(goalX-x) + abs(goalY-y);
  return distance;
}

int search (int x, int y, int goalX, int goalY, int cost, Heap hp) {
  State state;
  int a, h;
  int visited = 1;
  while (x != goalX || y != goalY) {
    for (a=0; a < 8; a++) {
      int newX = x + actions[a][0];
      int newY = y + actions[a][1];
      if (isValidLocation(newX, newY)){
        visited++;
        state.x = newX;
        state.y = newY;
        printf("coordinates %d,%d\n", state.x,state.y);
        state.g = realDistance(cost);
        h = heuristic1(state.x, state.y, goalX, goalY);
        state.f = state.g + h;
        /*printf("VALUE H: %d\n", h);
        printf("VALUE F: %d\n", state.f);
        printf("VALUE G: %d\n", state.g);*/
        if (state.x == goalX && state.y == goalY) {
          printf("#visited states: %d\n", visited);
          free(hp.array);
          return state.g/3;
        }
        enqueue(state.x, state.y, state.g, state.f, &hp);
      }
    }
    state = removeMin(&hp);
    x = state.x;
    y = state.y;
    cost = state.g;
  }
  printf("#visited states: %d\n", visited);
  free(hp.array);
  return 0;
}

int main(int argc, char *argv[]) {
  int x0,y0,x1,y1;
  Heap hp;
  hp = makeHeap();
  do {
    printf("Start location (x,y) = ");
    scanf("%d %d", &x0, &y0);
  } while (!isValidLocation(x0,y0));
  do {
    printf("Goal location (x,y)  = ");
    scanf("%d %d", &x1, &y1);
  } while (!isValidLocation(x1,y1));

  printf("Length shortest path: %d\n", search(x0,y0,x1,y1,0,hp));
  return 0;
}
