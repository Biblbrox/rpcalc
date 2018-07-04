#include <stdlib.h>
#include "stack.h"

static double *top = NULL;

static StackNode* create_node(double data)
{
    StackNode* node = (StackNode*) malloc(sizeof(StackNode));
    node->next = NULL;
    node->val = data;
    return node;
}

void stack_destroy(StackNode *top)
{
    if (top) {
        StackNode *tmp;
        do {
            tmp = top->next;
            free(top);
        } while ((top = tmp));
        top = NULL;
    }
}

int stack_empty(StackNode *top)
{
    return !top;
}

StackNode* stack_push(StackNode **top, double f)
{
    StackNode* new_node = create_node(f);
    new_node->next = *top;
    *top = new_node;

    return new_node;
}

StackNode* stack_clean(StackNode *top)
{
    if (top && top->next) {
        StackNode *tmp;
        do {
            tmp = top->next;
            free(top);
            top = tmp;
        } while ((top->next));

        top->next = NULL;
        top->val = 0.0;

        return top;
    }

    return NULL;
}

double stack_pop(StackNode **top)
{
    if (stack_empty(*top))
        return 0;

    StackNode* tmp = *top;        
    *top = (*top)->next;

    double data = tmp->val;
    free(tmp);

    return data;
}

double stack_peek(StackNode *top)
{
    return top ? top->val : 0.0;
}

void stack_dublicate(StackNode *top)
{
    if (top) {
        StackNode *node = create_node(top->val);
        node->next = top;
        top = node;
    }
}

int stack_inverse_top(StackNode *top)
{
    if (top && top->next) {
        double tmp = top->val;
        top->val = top->next->val;
        top->next->val = tmp;

        return 0;
    } 

    return -1;
}

int stack_size(StackNode *top)
{
    if (top == NULL)
        return 0;

    int count = 0;
    StackNode *tmp = top;
    while((tmp = tmp->next))
        count++;

    return count;
}
