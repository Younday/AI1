#ifndef STACK_H
#define STACK_H

 typedef struct Stack {
   int *array;
   int top;
   int size;
 } Stack;

 Stack newStack (int s) {
   Stack st;
   st.array = malloc(s*sizeof(int));
   assert(st.array != NULL);
   st.top = 0;
   st.size = s;
   return st;
 }

 void doubleStackSize (Stack *stp) {
   int newSize = 2*stp->size;
   stp->array = realloc(stp->array, newSize * sizeof(int));
   assert(stp->array != NULL);
   stp->size = newSize;
 }

void push (int value, Stack *stp) {
  if (stp->top == stp->size) {
    doubleStackSize(stp);
  }
  stp->array[stp->top] = value;
  stp->top++;
}

int isEmptyStack (Stack st) {
  return (st.top == 0);
}

void stackEmptyError () {
  abort();
}

int pop (Stack *stp) {
  if (isEmptyStack(*stp)) {
    stackEmptyError();
  }
  (stp->top)--;
  return (stp->array)[stp->top];
}

void freeStack (Stack st) {
  free(st.array);
}

#endif
