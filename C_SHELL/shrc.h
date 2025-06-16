#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buf_size 4096
typedef struct
{
    char alias[100];
    char actual_func[100];
} alias;
extern alias aliases[buf_size];
extern int alias_top;

typedef struct
{
    char func[100];
    char complete_func[100];
} functions;

extern functions function[buf_size];
extern int func_top;

void addAliases();


