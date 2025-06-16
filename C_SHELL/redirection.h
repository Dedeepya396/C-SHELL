#ifndef REDIRECTION
#define REDIRECTION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "activities.h"
#define buf_size 4096

void processRedirection(char *input,char* home_dir,char* previous_dir,char* log_filename,int fgbg);
void foreground_process_m(char* command);
int processUserCommands(char *input);
int GetProcloreID(char *command);
#endif 