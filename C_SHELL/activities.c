#include "activities.h"
int top = 0;
Activities activities[1000];

char get_process_state(int pid)
{
    char proc_path[256];
    FILE *status_file;
    char processStatus = '\0';

    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);
    status_file = fopen(proc_path, "r");
    if (status_file == NULL)
    {
        return 'X';
    }
    fscanf(status_file, "%*d %*s %c", &processStatus);
    fclose(status_file);
    return processStatus;
}
void appendProcessList(char *input, int pid, int fgbg) // add the process to the list
{
    activities[top].pid = pid;
    activities[top].fg_bg = fgbg;
    strcpy(activities[top].processName, input);
    top++;
}

int compareProcesses(const void *a, const void *b)
{
    Activities *processA = (Activities *)a;
    Activities *processB = (Activities *)b;
    return processA->pid - processB->pid;
}

// Function to sort the process list based on pid
void sortProcessLists()
{
    qsort(activities, top, sizeof(Activities), compareProcesses);
}
