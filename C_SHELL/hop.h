#ifndef HOP
#define HOP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define buf_size 4096

void hop(int x,char* previous_directory);

int compare_(char* a);

int getFlags(char *command);

void hop_fun(char *input, char *home_dir,char* previous_directory);
#endif