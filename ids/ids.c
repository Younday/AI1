#include <stdlib.h>
#include <stdio.h>

int dls (int start, int goal, int limit, int cost) {
  int child1, child2, child3, child4, child5, child6;
  int result;
  if (limit == 0 && start == goal) {
       return start;
     } else if (limit > 0){
       child1 = start+1;
       child2= 2*start;
       child3 = 3*start;
       child4 = start-1;
       child5 = start/2;
       child6 = start/3;
       result = dls(child1, goal, limit-1, cost+1) || dls(child2, goal, limit-1, cost+2) || dls(child3, goal, limit-1, cost+2) || dls(child4, goal, limit-1, cost+1) ||
       dls(child5, goal, limit-1, cost+3) || dls(child6, goal, limit-1, cost+3);
       if (result != 0 || (goal == start && result == 0)) {
         printf("path cost is %d\n", cost);
         return 1;
         }
        }
  return 0;
}

void ids (int start, int goal) {
  int depth;
  int result;
  for (depth = 0; ;depth++){
    result = dls(start, goal, depth, 1);
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
