#ifndef ACTIVITIES
#define ACTIVITIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buf_size 4096
typedef struct Activities
{
    int pid;
    char processName[buf_size];
    int fg_bg;
}Activities;
extern Activities activities[1000];
extern int top;
void appendProcessList(char* input,int pid,int fgbg);
void sortProcessLists();
char get_process_state(int pid);
// void removeProcessByPID(int pid, Activities activities);
#endif