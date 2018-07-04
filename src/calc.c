#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "calc.h"
#include "stack.h"
#include "utils.h"
#include "vars.h"

static uint8_t wrong_syntax(char *str)
{
    int i = 0;
    while(str[i] != '\0') {
        if (iswrongc(str[i]))
            return 1;

        if (isdigit(str[i])) {
            if( str[i + 1] != '\0'
            && !isoperator(str[i + 1])
            && !iswhitespace(str[i + 1])
            && !isdigit(str[i + 1])
            && str[i + 1] != '.') 
                return 1;
            
            if (i != 0 
            && !isoperator(str[i - 1])
            && !iswhitespace(str[i - 1])
            && !issign(str[i - 1])
            && str[i - 1] != '.'
            && !isdigit(str[i - 1]))
                return 1;
        } 

        if (isalpha(str[i]) 
        && str[i + 1] != '\0'
        && issign(str[i + 1])) 
            return 1;

        i++;
    }

    return 0;
}

int 
getop(char s[], StackNode *top, char **line, char *old_ptr)
{
    int i, c;

    if (*line == old_ptr)
        if (wrong_syntax(*line))
            return SYNTAX_ERROR;

    while (iswhitespace(s[0] = c = sgetch(line)))
            ;

    if (c == '\0' || c == EOF)
        return c;

    if (iswrongc(c))
        return SYNTAX_ERROR;

    s[1] = '\0';

    if (isalpha(c)) {
        if (isspecial(c) && stack_size(top) == 0)
            return c;

        char op[MAX_OP_SIZE];
        int k = 1;
        op[0] = s[0];
        while (isalpha(op[k] = c = sgetch(line))) 
            k++;

        op[k] = '\0';

        (*line)--;

        if (isfunction(op))
            return func_code(op);

        if (isvariable(op, &variables)) {
            double *val1 = map_get(&variables, op);
            double *val2 = map_get(&initial_vars, op);
            sprintf(s, "%f", val2 ? *val2 : *val1); // predefined variable is more convinient
            return NUMBER;
        }

        sprintf(s, "%s", op);
        return UNKNOW_TOKEN;
    }

    if (isoperator(c))
        return c;

    // c is digit or sign or dot
    i = 0; 
    if (c == SIGN_NEG || c == SIGN_POS) {
        s[i] = c == SIGN_NEG ? '-' : '+';
        s[++i] = c = *(*line)++;
    }

    if (isdigit(c))
        while (isdigit(s[++i] = c = sgetch(line)))
            ;
    
    if (c == '.')
        while (isdigit(s[++i] = c = sgetch(line)))
            ;

    s[i] = '\0';
    if (c != EOF && c != '\0')
        (*line)--;

    return NUMBER;
}
