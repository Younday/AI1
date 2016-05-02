#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stack.h"
#include "ids.h"



int dls (Stack s, int start, int goal, int limit, int cost) {
  int child1, child2, child3, child4, child5, child6;
  int result;
  if (limit == 0 && start == goal) {
       return start;
     } else if (limit > 0){
       push(start+1, &s);
       child1 = pop(&s);
       push(2*start, &s);
       child2 = pop(&s);
       push(3*start, &s);
       child3 = pop(&s);
       push(start-1, &s);
       child4 = pop(&s);
       push(start/2, &s);
       child5 = pop(&s);
       push(start/3, &s);
       child6 = pop(&s);
       result = dls(s, child1, goal, limit-1, cost+1) || dls(s, child2, goal, limit-1, cost+2) || dls(s, child3, goal, limit-1, cost+2) || dls(s, child4, goal, limit-1, cost+1) ||
       dls(s, child5, goal, limit-1, cost+3) || dls(s, child6, goal, limit-1, cost+3);
       if (result != 0 || (goal == start && result == 0)) {
         printf("path cost is %d\n", cost);
         return 1;
         }
        }
  return 0;
}

void ids (int start, int goal) {
  Stack s;
  int depth;
  int result;
  for (depth = 0; ;depth++){
    s = newStack(50000);
    result = dls(s, start, goal, depth, 1);
    freeStack(s);
    if (result == 1) {
      printf("path length is %d\n", depth);
      break;
    }
  }
}


int main(int argc, char *argv[]) {
  int start, goal;
  scanf("%d %d", &start, &goal);
  ids(start,goal);
  return 0;
  }
