#ifndef VARS_H
#define VARS_H 

#include "../deps/map/map.h"

#define MAX_VAR_SIZE 100

typedef struct {
    char name[MAX_VAR_SIZE];
    double val;    
} Variable;

extern map_double_t initial_vars;
extern map_double_t variables;

int parse_var_str(Variable *var,  char *var_string);
int add_to_vars(const char *name, const double value);

#endif /* VARS_H */
