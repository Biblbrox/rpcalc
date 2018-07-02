#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "utils.h"
#include "calc.h"
#include "vars.h"

int isoperator(int c)
{
    return c == ADD 
        || c == SUB 
        || c == MUL 
        || c == DIV
        || c == MOD
        || c == SUP;
}

int isvariable(char *s, map_double_t *variables) 
{
    double *val = map_get(variables, s); 
    if (val)
        return 1;

    val = map_get(&initial_vars, s);

    return val ? 1 : 0;
}

void clean_file(FILE *f)
{
    char c;
    while(!feof(f))
        c = fgetc(f);
}

/**
 * Read all chars from f until EOF or '\n'
 */
void clean_linef(FILE *f)
{
    if (!feof(f)) {
        char c;
        while((c = getc(f)) != EOF && c != '\n') ;
    }
}

ssize_t getlinen(char dest[], const int n, FILE *f)
{
    if (dest == NULL)
        return -1;
    
    char c;
    int k = 1;
    while ((c = dest[k - 1] = fgetc(f)) != EOF && c != '\n' && k++ < n) ;
    dest[k - 1] = '\0';

    if (c != '\n')
        clean_linef(f);

    return k;
}

int isfunction(char *s)
{
    char *functions[] = {
        "exp",
        "sin",
        "cos",
        "pow",
        "sqrt"
    };

    unsigned i;
    size_t len = sizeof(functions) / sizeof(functions[0]);
    for (i = 0; i < len; i++)
        if (strcmp(functions[i], s) == 0)
            return 1;

    return 0;
}

int func_code(char *func)
{
    if (!isfunction(func))
        return -1;

    if (strcmp("exp", func) == 0)
        return EXP;
    else if (strcmp("sin", func) == 0) 
        return SIN;
    else if (strcmp("cos", func) == 0)
        return COS;
    else if (strcmp("pow", func) == 0)
        return POW;
    else if (strcmp("sqrt", func) == 0) 
        return SQRT;

    return -1;
}

int isspecial(char c)
{
    switch(c) {
        case QUIT:
            return 1;
            break;
        case SET_VAR_MODE:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

int iswhitespace(int c)
{
    return c == ' ' || c == '\t';
}

int sgetch(char **str)
{
    char ret = **str;
    (*str)++;
    return ret;
}

int only_letters(char *str)
{
    while(isalpha(*str)) ++str; 
    
    return *str == '\0';
}
