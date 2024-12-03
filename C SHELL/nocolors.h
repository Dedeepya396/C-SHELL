#ifndef NOCOLORS
#define NOCOLORS
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <fcntl.h>
#define buf_size 4096
// #include "reveal.h"
// #include "seek.h"
int compare_names_color(const void *a, const void *b);
void reveal_permissions_color(char *directory, int a_flag);
void reveal_color(char *directory, int a_flag);
void print_permissions_color(struct stat *fileStat);
void reveal_fun_color(char *cmd, char *home_dir);
int compare_s_color(char *filename, char *searchname);
void seek_color(char *input, char *home_dir);
void seek_fun_color(char *filename, int dir, int files, int exec, char *directory, int home_len, int *flag, int *numDir, int *numFiles, int *numExec);

#endif