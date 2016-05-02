#ifndef A_H
#define A_H


int isValidLocation(int x, int y);
int heuristic1(int x, int y, int goalX, int goalY);
int heuristic2(int x, int y, int goalX, int goalY);
int search (int x, int y, int goalX, int goalY, int cost, Heap hp);


#endif
