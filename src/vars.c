#include <errno.h>
#include "vars.h"
#include "utils.h"
#include "calc.h"

map_double_t initial_vars;
map_double_t variables;

int parse_var_str(Variable *var,  char *var_string)
{
    char *set, *var_name, *var_value_str;
    const char *del = " =";
    char tmp[MAX_VAR_SIZE];

    strcpy(tmp, var_string);
    set = strtok(tmp, del);
    if (set == NULL || strcmp("set", set) != 0)
        return -1;

    var_name = strtok(NULL, del);
    if (var_name == NULL || !only_letters(var_name))
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

int add_to_vars(const char *name, const double value)
{
    if (name == NULL)
        return -1;

    if (map_set(&variables, name, value) != 0)
        return -1;
    
    return 0; 
}
