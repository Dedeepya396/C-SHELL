#ifndef sys
#define sys
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "activities.h"
#define buf_size 4096
extern int curr_running;
void background_process(char *command);
void foreground_process(char *command, char *home_dir);
#endif