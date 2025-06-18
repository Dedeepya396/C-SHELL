#ifndef REVEAL
#define REVEAL
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#define buf_size 4096

void print_permissions(struct stat *fileStat);

void reveal_permissions(char *directory, int aFlag);

void reveal_(char *directory, int aFlag);

void reveal_fun(char *input, char *home_dir);

int compare_names(const void *a, const void *b);
#endif