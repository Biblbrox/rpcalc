#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>

typedef struct StackNodeStruct StackNode;

struct StackNodeStruct {
    double val;
    StackNode *next; 
};

int stack_size(StackNode *top);

StackNode* stack_push (StackNode **top, double f);

double stack_pop(StackNode **top);

void stack_destroy(StackNode *top);

double stack_peek(StackNode *top);

int stack_init(size_t default_size);

int stack_empty(StackNode *top);

void stack_dublicate(StackNode *top);

int stack_inverse_top(StackNode *top);

StackNode* stack_clean(StackNode *top);


#endif
