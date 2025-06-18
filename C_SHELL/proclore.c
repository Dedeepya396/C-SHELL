#include "proclore.h"

void proclore()
{
    pid_t processID = getpid(); // get the pid of process
    char proc_path[buf_size];
    FILE *status_file;
    char processStatus;
    // construct process path
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", processID);
    status_file = fopen(proc_path, "r");
    if (status_file == NULL)
        perror("\033[0;31m There is an error in opening the file \033[0\n");
    fscanf(status_file, "%*d %*s %c", &processStatus);

    unsigned long virtual_memory;
    fscanf(status_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %lu", &virtual_memory);
    fclose(status_file);

    int processGroupID = getpgid(processID); // get group id of the process

    char exe_path[256], exe_realpath[256];
    sprintf(exe_path, "/proc/%d/exe", processID);
    ssize_t len = readlink(exe_path, exe_realpath, sizeof(exe_realpath) - 1);
    if (len != -1)
    {
        exe_realpath[len] = '\0';
    }
    else
    {
        printf("\033[0;31mExecutable Path: Not found \033[0\n");
    }

    printf("pid: %d\n", processID);
    printf("process Status: %c\n", processStatus);
    printf("process Group: %d\n", processGroupID);
    printf("Virtual memory: %ld\n", virtual_memory);
    printf("Executable path: %s\n", exe_realpath);
}

void procloreID(int processID)
{
    char proc_path[buf_size];
    FILE *status_file;
    char processStatus;

    // Construct the path to the stat file
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", processID);

    // Open the stat file
    status_file = fopen(proc_path, "r");
    if (status_file == NULL)
    {
        perror("\033[0;31mError opening the file \033[0;31m");
        return;
    }

    // Read the process status from the stat file
    if (fscanf(status_file, "%*d %*s %c", &processStatus) != 1)
    {
        printf("\033[0;31mError reading process status\033[0;31m\n");
        fclose(status_file);
        return;
    }

    unsigned long virtual_memory;
    // Read the virtual memory from the stat file
    if (fscanf(status_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %lu", &virtual_memory) != 1)
    {
        printf("\033[0;31mError reading virtual memory\033[0\n");
        fclose(status_file);
        return;
    }
    fclose(status_file);

    int processGroupID = getpgid(processID);
    if (processGroupID == -1)
    {
        perror("\033[0;31mError getting process group ID\033[0");
        return;
    }

    char exe_path[256], exe_realpath[256];
    snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", processID);
    ssize_t len = readlink(exe_path, exe_realpath, sizeof(exe_realpath) - 1);
    printf("pid: %d\n", processID);
    printf("process Status: %c\n", processStatus);
    printf("process Group: %d\n", processGroupID);
    printf("Virtual memory: %ld\n", virtual_memory);
    if (len != -1)
    {
        exe_realpath[len] = '\0';
        printf("Executable path: %s\n", exe_realpath);
    }
    else
    {
        printf("\033[0;31mExecutable path : Not found\033[0\n");
    }
}