#ifndef SEEK
#define SEEK
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define buf_size 4096

void seek(char* input,char* home_dir);

void seek_fun(char *filename, int dir, int files, int exec, char *directory, int home_len, int *flag, int *numDir, int *numFiles, int *numExec);

#endif
