#ifndef pipesRedirection
#define pipesRedirection
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "dispReq.h"
#include "pipes.h"
#include "redirection.h"
#include "activities.h"
#define buf_size 4096

void both(char* input,char* home_dir,char* previous_dir,char* log_filename,int fgbg);
#endif