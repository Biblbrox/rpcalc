#ifndef __UTILS_H__
#define __UTILS_H__

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
int print_error(const char *fmt, ...);
int only_letters(char *str);
int issign(char c);
void print_crit(const char *fmt, ...);
int iswrongc(char c);

void clean_file(FILE *f);
void clean_linef(FILE *f);

ssize_t getlinen(char dest[], int n, FILE *f);

#endif
