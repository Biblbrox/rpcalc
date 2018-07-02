#ifndef __CALC_H__
#define __CALC_H__

#define NUMBER '0' /* is number */

#define SIGN_NEG '#'
#define SIGN_POS '@'

// operators
#define ADD '+'
#define MUL '*'
#define DIV '/'
#define MOD '%'
#define SUP '^'
#define SUB '-'

//special symbols
#define QUIT 'q'
#define SET_VAR_MODE 'n'
#define UNKNOW_TOKEN 'u'
#define SYNTAX_ERROR '&'

//functions
#define EXP  'e'
#define POW  'p'
#define SIN  's'
#define COS  'c'
#define SQRT 't'

#define MAX_OP_SIZE 100
#define MAX_VAR_SIZE 100

#include "stack.h"
#include "../deps/map/map.h"

int getop(char [], StackNode *top, map_double_t *variables, char **line, char *old_ptr);

typedef struct {
    char name[MAX_VAR_SIZE];
    double val;    
} Variable;

#endif
