#ifndef SIGNALS
#define SIGNALS
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "activities.h"
#include "systemCommands.h"
#define buf_size 4096
void getSignal(char *input);
void executeSignal(int pid, int signal);
void FG_BG_(int pid, int fg_or_bg, char *home_dir);
#endif