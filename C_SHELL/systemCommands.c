#include "systemCommands.h"
#include "dispReq.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
int curr_running = -1;
#define MAX_PROCESSES 100

// Define a map for tracking process IDs and their commands
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

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) // to reap all the non terminated children
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
    // registers a sigchild handler
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld; // attaches handler function
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, 0) == -1)
    {
        perror("\033[0;31m Error in sigaction \033[0;31m");
        exit(1);
    }

    int child = fork(); // creates a new child process
    if (child < 0)
    {
        perror("\033[0;31m Error in creating a child \033[0;31m");
    }
    else if (child > 0) // parent process
    {
        // appends the process to activities list
        appendProcessList(command, child, 0);
        printf("Started background process with PID: %d\n", child);
        add_process(child, command); // appends process to processInfo
    }
    else if (child == 0)
    {
        setpgid(0, 0); // Detach child from shell and make it run on itself
        execl("/bin/sh", "sh", "-c", command, (char *)(NULL));
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
        // ignore these signals
        signal(SIGTTIN, SIG_IGN), signal(SIGTTOU, SIG_IGN);
        setpgid(child, getpgid(0));           // sets the child process in same group as shell
        tcsetpgrp(0, child);                  // gives the terminal control to the child
        appendProcessList(command, child, 1); // add the process to activities list
        curr_running = child;                 // update current running fg process
        start_time = time(NULL);
        int status;
        waitpid(child, &status, WUNTRACED);                 // parent waits until child is done
        tcsetpgrp(0, getpgid(0));                           // gives back the terminal control to shell
        signal(SIGTTIN, SIG_DFL), signal(SIGTTOU, SIG_DFL); // set the signals to default behavior
        curr_running = -1;                                  // remove the current running process
        end_time = time(NULL);                              // note down the end time
        int time_taken = (int)(end_time - start_time);      // find the time taken by the foreground process
        char filename1[buf_size];
        strcpy(filename1, home_dir);
        strcat(filename1, "/time.txt"); // open a file to note down the time taken
        FILE *fp = fopen(filename1, "w+");
        fprintf(fp, "%d\n", time_taken); // write the time taken in the file
        fclose(fp);
        if (time_taken > 2)
        {
            printf("Process '%s' took %d seconds\n", command, time_taken);
            char *filename = (char *)malloc(sizeof(char) * buf_size);
            strcpy(filename, home_dir);
            strcat(filename, "/test.txt"); // Write the flag in the file to indicate that a process took time more than 2 secs
            FILE *fp = fopen(filename, "w+");
            fprintf(fp, "%d\n", 1);
            fprintf(fp, "%s", command); // and the command that took > 2 secs
            fclose(fp);
        }
    }
    else if (child == 0)
    {
        setpgid(0, 0);
        execl("/bin/sh", "sh", "-c", command, (char *)(NULL));
        // execute the command
        perror("execvp failed");
        exit(1);
    }
}
