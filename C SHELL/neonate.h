#ifndef NEONATE
#define NEONATE
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "activities.h"
void neonate(char* input);
void executeNeonate();
void callingProcess(int wait_time);
int is_key_pressed();
#endif