#ifndef DISP
#define DISP
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define buf_size 4096
void print_The_Directory(char *home_dir);
void trimWhitespaces(char *str);
#endif