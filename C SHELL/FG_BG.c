#include "FG_BG.h"
#include <signals_.h>
void GetID_FG_BG(char *input,char* home_dir)
{
    int i = 0;
    int fg_or_bg = -1;
    int id = -1;
    char *token = (char *)malloc(sizeof(char) * buf_size);
    token = strtok(input, " \t");
    while (token != NULL)
    {
        if (i == 0)
        {
            if (strcmp(token, "fg") == 0)
            {
                printf("fg\n");
                fg_or_bg = 1; // 1 for foreground_process
            }
            else if (strcmp(token, "bg") == 0)
            {
                printf("bg\n");
                fg_or_bg = 0;
            }
            i++;
        }
        else if (i == 1)
        {
            id = atoi(token);
            printf("%d\n", id);
            i++;
        }
        token = strtok(NULL, " \t");
    }
    printf("%d\n", fg_or_bg);
    executeFG_BG(id, fg_or_bg,home_dir);

    printf("C%d\n", id);
}
void executeFG_BG(pid_t pid, int fg_or_bg,char* home_dir)
{
    setpgid(pid, getpgid(0));
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    int status;
    tcsetpgrp(0, (int)pid);
    if (kill(pid, SIGCONT) < 0)
        printf("There is no process with given pid\n");
    FG_BG_(pid, fg_or_bg,home_dir);
    tcsetpgrp(0, (int)getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}
