#ifndef FG_BG
#define FG_BG
#include <stdio.h>
#include "systemCommands.h"
#include "activities.h"
#include <stdlib.h>
#include <string.h>

void GetID_FG_BG(char* input,char* home_dir);
void executeFG_BG(int id,int fg_bg,char* home_dir);
#endif