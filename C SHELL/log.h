#ifndef LOG
#define LOG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "activities.h"
#define buf_size 4096

void log_Write(char *input, char *filename);

void log_fun(char *filename, char *input,char* home_dir,char* previous_directory);

void logPrint(char* filename);

void logPurge(char *filename);

int numberOfCommands(FILE *fp);
#endif