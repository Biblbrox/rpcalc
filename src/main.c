#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include "calc.h"
#include "stack.h"
#include "../deps/map/map.h"
#include "utils.h"
#include "color.h"
#include "vars.h"

#define MAX_LINE_SIZE 1000

static StackNode *top = NULL;
static int fromexpr = 0;
static char expr[MAX_LINE_SIZE];

static int init(void) 
{
    printf("calc 0.1 by Biblbrox\n");
    map_init(&variables);
    map_init(&initial_vars);

    top = (StackNode *) malloc(sizeof(StackNode));

    return top == NULL ? -1 : 0;
}

static void print_promt(void)
{
    if (!fromexpr)
        printf(GREEN">>> "RESET);
}

static void clean(void)
{
    stack_destroy(top);
    map_deinit(&variables);
}

int resolve_expr(char *line)
{
    static int8_t first = 1;
    static int8_t hasPromt;

    int type;
    int run = 1;
    int ret = 0;
    double op2, power, tmp;
    char s[MAX_OP_SIZE];
    char var[MAX_VAR_SIZE];
    const size_t size_var = MAX_VAR_SIZE;;
    Variable new_var;

    char *t_line = (char*) malloc(MAX_LINE_SIZE);
    char *old_p = t_line;
    strcpy(t_line, line);

    if (!first) {
        if (!hasPromt)
            print_promt(); 
        else 
            hasPromt = 0;
        first = 1;
    } else 
        first = 0;

    while ((type = getop(s, top, &t_line, old_p)) != EOF && run) {
        switch (type) {
            case NUMBER: 
                stack_push(&top, strtod(s, NULL));
                break;
            case ADD:
                stack_push(&top, stack_pop(&top) + stack_pop(&top));
                break;
            case MUL:
                stack_push(&top, stack_pop(&top) * stack_pop(&top));
                break;
            case SUB:
                op2 = stack_pop(&top);
                stack_push(&top, stack_pop(&top) - op2);
                break;
            case DIV:
                op2 = stack_pop(&top);
                if (op2 != 0.0)
                    stack_push(&top, stack_pop(&top) / op2);
                else {
                    stack_pop(&top);
                    print_error("error: nyll division");
                }
                break;
            case SUP:
                op2 = stack_pop(&top);
                stack_push(&top, pow(stack_pop(&top), op2));
                break;
            case MOD:
                op2 = stack_pop(&top);
                if (op2 != 0)
                    stack_push(&top, (int)stack_pop(&top) % (int)op2);
                else {
                    print_error("error: null division");
                    stack_pop(&top);
                }
                break;
            case EXP:
                stack_push(&top, exp(stack_pop(&top)));
                break;
            case POW:
                power = stack_pop(&top);
                stack_push(&top, pow(stack_pop(&top), power));
                break;
            case SIN:
                stack_push(&top, sin(stack_pop(&top)));
                break;
            case COS:
                stack_push(&top, cos(stack_pop(&top)));
                break;
            case SQRT:
                stack_push(&top, sqrt(stack_pop(&top)));
                break;
            case SET_VAR_MODE:
                puts("Set variable mode");
                printf("Print variable name like " 
                        "\"set pi = 3.14\"\n"
                        CYAN">>> "RESET);

                if (getlinen(var, size_var, stdin) != -1) {
                    if (parse_var_str(&new_var, var) != 0)
                        print_error("Error parse set variable syntax\n");
                    else 
                        add_to_vars(new_var.name, new_var.val);
                } else
                    print_error("String is too long");
                break;
            case UNKNOW_TOKEN:
                print_error("Token %s is undefined\n", s);
                stack_clean(top);
                print_promt();
                hasPromt = 1;
                run = 0;
                break;
            case QUIT:
                ret = 1;
                run = 0;
                break;
            case '\0':
                if (stack_size(top) > 0) {
                    tmp = stack_pop(&top);
                    map_set(&initial_vars, "res", tmp);
                    printf("\t%.8g\n", tmp);
                }
                print_promt();
                hasPromt = 1;
                run = 0;
                break;
            case SYNTAX_ERROR:
                print_error("Syntax error\n");
                print_promt();
                hasPromt = 1;
                run = 0;
                break;
            case EOF:
                ret = 1;
                run = 0;
                break;
            default:
                print_error("ошибка: неизвестная операция %s\n", s);
                break;
        }
    }

    t_line = old_p;
    free(t_line);

    return ret;
}

static int getexpr(char *line, size_t maxlen) 
{
    static int count = 1; 
    if (fromexpr) {
        if (count) {
            strcpy(line, expr);
            --count; 
            return strlen(line);
        } else 
            return 0;
    }

    return getlinen(line, maxlen, stdin);
}

static int main_loop(void)
{
    char line[MAX_LINE_SIZE];
    const int line_size = MAX_LINE_SIZE;
    int8_t success = 1;
    int8_t code;

    print_promt();
    while (getexpr(line, line_size)) {
        code = resolve_expr(line); 
        if (code == -1) {
            success = 0;
            break;
        } else if (code == 1) 
            break; 
    }

    return success ? 0 : -1;
}

int main (int argc, char *argv[])
{
    if (argc > 2) 
        print_crit("rpcalc: too many arguments\n");

    char c;
    char *pos;
    while (--argc > 0 && (*++argv)[0] == '-') {
        c = *++argv[0];
        switch(c) {
            case 'e':
                if ((c = *++argv[0]) != '=') 
                    print_crit("rpcalc: wrong argument syntax\n");

                ++argv[0];
                pos = strchr(*argv, '\0');

                strncpy(expr, *argv, pos - *argv);
                fromexpr = 1;

                break;
            default:
                break;
        } 
    }
        
    init();

    if (main_loop() == -1) {
        clean(); 
        perror("error");
        exit(EXIT_FAILURE);
    }
    
    clean();

    return 0;
}
