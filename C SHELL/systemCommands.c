#include "systemCommands.h"
#include "dispReq.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
// Define a map for tracking process IDs and their commands
int curr_running = -1;
#define MAX_PROCESSES 100

typedef struct
{
    pid_t pid;
    char command[256];
} ProcessInfo;

static ProcessInfo process_map[MAX_PROCESSES];
static int process_count = 0;

void add_process(pid_t pid, const char *command)
{
    if (process_count < MAX_PROCESSES)
    {
        process_map[process_count].pid = pid;
        strncpy(process_map[process_count].command, command, sizeof(process_map[process_count].command) - 1);
        process_map[process_count].command[sizeof(process_map[process_count].command) - 1] = '\0';
        process_count++;
    }
}

const char *get_command_by_pid(pid_t pid)
{
    for (int i = 0; i < process_count; i++)
    {
        if (process_map[i].pid == pid)
        {
            return process_map[i].command;
        }
    }
    return "Unknown Command";
}

void handle_sigchld(int sig)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        const char *command = get_command_by_pid(pid);
        if (WIFEXITED(status))
        {
            printf("\nBackground process %d (%s) ended normally\n", pid, command);
        }
        else if (WIFSIGNALED(status))
        {
            printf("\nBackground process %d (%s) ended abnormally\n", pid, command);
        }
    }
}

void background_process(char *command)
{
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, 0) == -1)
    {
        perror("\033[0;31m Error in sigaction \033[0;31m");
        exit(1);
    }

    int child = fork();
    if (child < 0)
    {
        perror("\033[0;31m Error in creating a child \033[0;31m");
    }
    else if (child > 0)
    {
        appendProcessList(command, child, 0);
        printf("Started background process with PID: %d\n", child);
        add_process(child, command);
    }
    else if (child == 0)
    {
        setpgid(0, 0);
        execl("/bin/sh", "sh", "-c", command, (char *)(NULL));
        // char *args[] = {
        //     "sh",
        //     "-c",
        //     (char *)command,
        //     NULL};

        // execvp("/bin/sh", args);
        // perror("execvp failed");
        perror("\033[0;31m Exec failed \033[0m");
        exit(1);
    }
}

void foreground_process(char *command, char *home_dir)
{
    time_t start_time, end_time;
    int child = fork();
    if (child < 0)
    {
        perror("\033[0;31m Error in creating a child \033[0\n");
    }

    if (child > 0)
    {
        signal(SIGTTIN, SIG_IGN), signal(SIGTTOU, SIG_IGN);
        setpgid(child, getpgid(0));
        tcsetpgrp(0, child);
        appendProcessList(command, child, 1);
        curr_running = child;
        start_time = time(NULL);
        int status;
        waitpid(child, &status, WUNTRACED);
        tcsetpgrp(0, getpgid(0));
        signal(SIGTTIN, SIG_DFL), signal(SIGTTOU, SIG_DFL);
        curr_running = -1;
        end_time = time(NULL);
        int time_taken = (int)(end_time - start_time);
        char filename1[buf_size];
        strcpy(filename1, home_dir);
        strcat(filename1, "/time.txt");
        FILE *fp = fopen(filename1, "w+");
        fprintf(fp, "%d\n", time_taken);
        fclose(fp);
        if (time_taken > 2)
        {
            printf("Process '%s' took %d seconds\n", command, time_taken);
            char *filename = (char *)malloc(sizeof(char) * buf_size);
            strcpy(filename, home_dir);
            strcat(filename, "/test.txt");
            FILE *fp = fopen(filename, "w+");
            fprintf(fp, "%d\n", 1);
            fprintf(fp, "%s", command);
            fclose(fp);
        }
    }
    else if (child == 0)
    {
        setpgid(0, 0);
        execl("/bin/sh", "sh", "-c", command, (char *)(NULL));
        // char *args[] = {
        //     "sh",
        //     "-c",
        //     (char *)command,
        //     NULL};
        // execvp("/bin/sh", args);

        perror("execvp failed");

        exit(1);
    }
}
