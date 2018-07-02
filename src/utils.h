#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../deps/map/map.h"

int iswhitespace(int c);
int isoperator(int c);
int isfunction(char *s);
int func_code(char *func);
int isspecial(char c);
int isvariable(char *s, map_double_t *variables);

int sgetch(char **str);

int only_letters(char *str);

void clean_file(FILE *f);

void clean_linef(FILE *f);

ssize_t getlinen(char dest[], int n, FILE *f);

#endif
