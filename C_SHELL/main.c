#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "dispReq.h"
#include "redirection.h"
#include "pipes.h"
#include "pipes_redirection.h"
#include "inputReq.h"
#include "activities.h"
#include "log.h"
#include "reveal.h"
#include "seek.h"
#include "signals_.h"
#include "neonate.h"
#include "systemCommands.h"
#include "shrc.h"
#define buf_size1 4096

void printProcessList()
{
    for (int i = 0; i < top; i++)
    {
        char state = get_process_state(activities[i].pid);
        if (activities[i].pid != -1)
        {
            printf("%d : %s - ", activities[i].pid, activities[i].processName);
            if (state == 'R' || state == 'S' || state == 'Z')
                printf("Running\n");
            if (state == 'T' || state == 'X')
            {
                printf("Stopped\n");
            }
        }
    }
}

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        printf("\nCaught SIGINT. Terminating all running processes.\n");

        for (int i = 0; i < top; i++)
        {
            if (activities[i].pid != -1 && activities[i].fg_bg == 1)
            {
                kill(activities[i].pid, SIGINT);
            }
        }
    }
}
void signalHandler_z(int signum)
{
    if (curr_running != -1)
    {
        printf("%d\n", curr_running);
        curr_running = -1;
    }
    else
        printf("There is no process running currently\n");
}

void cleanupAndExit()
{
    for (int i = 0; i < top; i++)
    {
        if (activities[i].pid != -1 && activities[i].fg_bg == 1)
        {
            kill(activities[i].pid, SIGKILL);
        }
    }
    printf("\n");
    exit(0);
}

int checkForFunctions(char *input, char **command)
{
    int flag = 0;
    char *token = (char *)malloc(sizeof(char) * buf_size);
    char *cmd = (char *)malloc(sizeof(char) * buf_size);
    strcpy(cmd, input);
    token = strtok(cmd, " \t");
    int num = 0;
    for (int i = 0; i < func_top; i++)
    {
        if (strcmp(function[i].func, token) == 0)
        {
            num = i;
            flag = 1;
            break;
        }
    }
    token = strtok(NULL, " \t");
    char *after_commands = (char *)malloc(sizeof(char) * buf_size);
    strcpy(after_commands, "");
    while (token != NULL)
    {
        strcat(after_commands, token);
        strcat(after_commands, " ");
        token = strtok(NULL, " \t");
    }
    printf("%s\n", after_commands);

    char *actual_command = (char *)malloc(sizeof(char) * buf_size);
    strcpy(actual_command, function[num].complete_func);
    char *token2 = strtok(actual_command, " \t");
    strcpy(*command, "");
    while (token2 != NULL)
    {
        if (strstr(token2, "$1"))
        {
            strcat(*command, after_commands);
            strcat(*command, " ; ");
        }
        else
        {
            strcat(*command, token2);
            strcat(*command, " ");
        }
        token2 = strtok(NULL, " \t");
    }
    return flag;
}

int main()
{
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler_z);
    addAliases();
    char *input = (char *)malloc(sizeof(char) * buf_size);
    char *home_dir = (char *)malloc(sizeof(char) * buf_size);
    getcwd(home_dir, buf_size);
    char *previous_directory = (char *)malloc(sizeof(char) * buf_size);
    strcpy(previous_directory, home_dir);
    char *logFileName = (char *)malloc(sizeof(char) * buf_size);
    strcpy(logFileName, home_dir);
    strcat(logFileName, "/log.txt");
    while (1)
    {
        print_The_Directory(home_dir);
        if (fgets(input, buf_size1, stdin) == NULL) // if there is an error in reading input terminate
        {
            cleanupAndExit();
        }
        trimWhitespaces(input);             // remove whitespaces to get clean command
        input[strcspn(input, "\n")] = '\0'; // replace the new line character by null character
        if (strcmp(input, "bye") == 0)      // to exit from this duplicate C shell
        {
            break;
        }
        if (strcmp(input, "\n") != 0)
            log_Write(input, logFileName);
        if (strcmp(input, "activities") == 0) // a command that prints the current processes
        {
            printProcessList();
            continue;
        }
        if (strncmp(input, "mk_hop", 6) == 0 || strncmp(input, "hop_seek", 8) == 0) // aliases
        {
            char *func_command = (char *)malloc(sizeof(char) * buf_size);
            checkForFunctions(input, &func_command);
            dividingCommands(func_command, home_dir, logFileName, previous_directory);
            continue;
        }
        dividingCommands(input, home_dir, logFileName, previous_directory);
    }
}
