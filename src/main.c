#include <stdio.h>
#include <stdlib.h> /* для atof() */
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

static StackNode *top = NULL;
static map_double_t variables;

static int init(void) 
{
    printf("calc 0.1 by Biblbrox\n");
    map_init(&variables);
    map_init(&initial_vars);

    top = (StackNode *) malloc(sizeof(StackNode));
    if (top == NULL)
        return -1;

    return 0;
}

static void clean(void)
{
    stack_destroy(top);
    map_deinit(&variables);
}

static void print_promt()
{
    printf(GREEN">>> "RESET);
}

static int parse_var_str(Variable *var,  char *var_string)
{
    char *set;
    char *var_name;
    char *var_value_str;
    const char *del = " =";
    
    char tmp[MAX_VAR_SIZE];

    strcpy(tmp, var_string);
    set = strtok(tmp, del);
    if (set == NULL)
        return -1;

    if (strcmp("set", set) != 0)
        return -1;

    var_name = strtok(NULL, del);
    if (var_name == NULL)
        return -1;

    if (!only_letters(var_name))
        return -1;
    
    var_value_str = strtok(NULL, del);
    if (var_value_str == NULL)
        return -1;

    errno = 0;
    double var_value = strtod(var_value_str, NULL);
    if (errno != 0)
        return -1;

    strcpy(var->name, var_name);
    var->val = var_value;

    return 0;
}

static int add_to_vars(const char *name, const double value)
{
    if (name == NULL)
        return -1;

    if (map_set(&variables, name, value) != 0)
        return -1;
    
    return 0; 
}

static void print_error(char *s)
{
    printf(RED"%s"RESET, s);
}

int resolve_expr(char *line)
{
    static int8_t first = 1;
    static int8_t hasPromt;

    int type;
    int run = 1;
    int ret = 0;
    double op2, power;
    double tmp;
    char s[MAX_OP_SIZE];
    char var[MAX_VAR_SIZE];
    char temp_str[MAX_VAR_SIZE];
    const size_t size_var = MAX_VAR_SIZE;;
    Variable new_var;

    char *t_line = (char*) malloc(1000);
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

    while ((type = getop(s, top, &variables, &t_line, old_p)) != EOF && run) {
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
                    print_error("error: nyll division");
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
                sprintf(temp_str, "Token %s is undefined\n", s);
                print_error(temp_str);
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
                sprintf(temp_str, "ошибка: неизвестная операция %s\n", s);
                print_error(temp_str);
                break;
        }
    }

    t_line = old_p;
    free(t_line);

    return ret;
}

static int main_loop(void)
{
    char line[1000];
    const int line_size = 1000;
    int8_t success = 1;
    int8_t code;

    print_promt();

    while (getlinen(line, line_size, stdin)) {
        code = resolve_expr(line); 
        if (code == -1) {
            success = 0;
            break;
        } else if (code == 1) 
            break; 
    }

    return success ? 0 : -1;
}

int main ()
{
    init();

    if (main_loop() == -1) {
        clean(); 
        perror("error");
        exit(EXIT_FAILURE);
    }
    
    clean();

    return 0;
}
