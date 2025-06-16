#ifndef pipes
#define pipes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "activities.h"
#define buf_size 4096
void processPipes(char* input,char* home_dir);
int numberOfPipes(char* input);
#endif