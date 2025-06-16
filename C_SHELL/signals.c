#include "signals_.h"
void getSignal(char *input)
{
    int id = -1;
    int signal = -1;
    char *token = (char *)malloc(sizeof(char) * buf_size);
    token = strtok(input, " \t");
    int i = 0;
    while (token != NULL)
    {
        if (i == 2)
        {
            signal = atoi(token);
            i++;
        }
        else if (i == 1)
        {
            id = atoi(token);
            i++;
        }
        if (i == 0)
        {
            i++;
        }
        token = strtok(NULL, " \t");
    }
    // printf("S: %d I: %d\n", signal, id);
    executeSignal(id, signal);
}
void executeSignal(int id, int signal)
{
    int flag = 0;
    for (int i = 0; i < top; i++)
    {
        if (activities[i].pid == id)
        {
            flag = 1;
            signal = signal % 32;
            kill(id, signal);
            if (signal == 9)
            {
                activities[i].pid = -1;
            }
        }
    }
    if (flag == 0)
    {
        perror("\033[0;31mNo such process found\033[0m\n");
    }
}
void check(int num, char *home_dir)
{
    if (strncmp(activities[num].processName, "sleep", strlen("sleep")) == 0)
    {
        char command[100];
        strcpy(command, "sleep ");
        char filename[100];
        strcpy(filename, home_dir);
        strcat(filename, "/time.txt");
        char time_done[100];
        FILE *fp = fopen(filename, "r");
        fgets(time_done, 100, fp);
        int time_d = atoi(time_done);
        char *time_ptr = strstr(activities[num].processName, " ");
        if (time_ptr != NULL)
        {
            time_ptr++;
            printf("%s\n", time_ptr);
            int time_left = atoi(time_ptr) - atoi(time_done);

            char time_left_str[10];
            sprintf(time_left_str, "%d", time_left);
            strcat(command, time_left_str);
            strcpy(activities[num].processName, command);
        }
    }
}
void FG_BG_(int pid, int fg_or_bg, char *home_dir)
{
    int num = 0;
    for (int i = 0; i < top; i++)
    {
        if (activities[i].pid == pid)
        {
            num = i;
            break;
        }
    }
    if (fg_or_bg == 1)
    {
        check(num, home_dir);
        foreground_process(activities[num].processName, home_dir);
    }
    else
    {
        background_process(activities[num].processName);
    }
    top--;
    activities[top].pid = 0;
    activities[top].fg_bg = 0;
    strcpy(activities[top].processName, "");
}