#ifndef INPUT
#define INPUT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "activities.h"
#define buf_size 4096
void dividingCommands(char *input, char *home_dir, char *filename, char *previous_dir);
int processCommands(char *input);
#endif