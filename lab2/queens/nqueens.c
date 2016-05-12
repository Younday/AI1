/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

void initializeRandomGenerator() {
  /* this routine initializes the random generator. You are not
   * supposed to understand this code. You can simply use it.
   */
  time_t t;
  srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    queens[q] = (flag == 0? 0 : random()%nqueens);
  }
}

/* returns TRUE if position (row0,column0) is in
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /* on same row, */
  if (column0 == column1) return TRUE; /* column, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
  return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, queens[queen])) {
      if ((row != queen) || (col != queens[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print configuration on screen */
void printState() {
  int row, column;
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (queens[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if ((column < 0) || (column >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: column=%d "
      "(should be 0<=column<%d)...Abort.\n", column, nqueens);
    exit(-1);
  }
  queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in canMoveTo: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if(column < 0 || column >= nqueens) return FALSE;
  if (queens[queen] == column) return FALSE; /* queen already there */
  return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in columnOfQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, queens[queen], other, queens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

int selectRandom(int n) {
  int i;
  i = 0 + random() % (n-0);
  return i;
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 1000
void randomSearch() {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;

  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int pos, newpos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      newpos = pos;
      while (newpos == pos) {
        newpos = random() % nqueens;
      }
      moveQueen(queen, newpos);
    }
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}

/*************************************************************/

void hillClimbing() {
  int newqueen, newpos, pos;
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;
  int max = 0;
  int i;

  while ((evaluateState()) != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      pos = columnOfQueen(queen);
      for(i = 0; i < nqueens; i++) {
        moveQueen(queen, i);
        if(evaluateState() > max) {
          newpos = i;
          max = evaluateState();
          newqueen = queen;
        }
        if (evaluateState() == max) {
          int x = 0 + random() % (1 - 0);
          switch (x) {
            case 0:
              newpos = i;
              break;
            case 1:
              newpos = pos;
              break;
          }
          newqueen = queen;
        }
      }
      moveQueen(queen, pos);
    }
    moveQueen(newqueen,newpos);
}
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}


/*************************************************************/

int ExpMove(int dE, int iter) {
  int random1;
  float E;
  E = exp((dE/iter)/nqueens*nqueens) * 100;
  random1 = random() % 100;
  if(E > random1) {
    return 1;
  }
  else {
    return 0;
  }
}

void simulatedAnnealing() {
  int dE, newqueen, ev;
  int queen, iter = 0, i;
  int optimum = (nqueens-1)*nqueens/2;
  int max = 0, current;

  while (evaluateState() != optimum) {
    ev = evaluateState();
    printf("iteration %d: evaluation=%d\n", iter++, ev);
    if(iter == MAXITER) break;
    int newpos;
    for (queen=0; queen < nqueens; queen++) {
      int pos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      for(i = 0; i < nqueens; i++) {
        moveQueen(queen, i);
        current = evaluateState();
        if(current > max) {
          newpos = i;
          max = evaluateState();
          newqueen = queen;
        }
        dE = max - current;
        if(dE == 0) {
          if(ExpMove(dE, iter)) {
            newpos = random() % nqueens;
          }
        }
      }
      moveQueen(queen, pos);
    }
    moveQueen(newqueen, newpos);
  }
  if (optimum == ev) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}

void geneticAlgorithm() {
  /*
  First:
  Fitness: number of correct queens (evaluateState)
  Cross-over: pick a random queen n, then the positions of the queens after n of 1 parent
  and in front of n of the other parent
  Mutation: Swap two queens
  Throw away lowest fitness
  */

  int optimum = (nqueens-1)*nqueens/2;




}



int main(int argc, char *argv[]) {
  int algorithm;

  do {
    printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
    scanf ("%d", &nqueens);
  } while ((nqueens < 1) || (nqueens > MAXQ));

  do {
    printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
    printf ("(3) Simulated Annealing  (4) Genetic algorithm: ");
    scanf ("%d", &algorithm);
  } while ((algorithm < 1) || (algorithm > 4));

  initializeRandomGenerator();

  initiateQueens(1);

  printf("\nInitial state:");
  printState();

  switch (algorithm) {
  case 1: randomSearch();       break;
  case 2: hillClimbing();       break;
  case 3: simulatedAnnealing(); break;
  case 4: geneticAlgorithm();   break;
  }

  return 0;
}
