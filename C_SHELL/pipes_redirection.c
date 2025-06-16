#include "pipes_redirection.h"
#include "activities.h"
void both(char *input, char *home_dir, char *previous_dir, char *log_filename,int fgbg)
{
    // int n = numberOfPipes(input) + 1; // Total number of commands separated by pipes
    // int pipe_fd[n - 1][2];            // Pipe file descriptors

    // char *command = strdup(input);    // Copy input to avoid modifying the original string
    // char *token = strtok(command, "|"); // First command in the pipeline
    // int i = 0;

    // while (token)
    // {
    //     if (i < n - 1 && pipe(pipe_fd[i]) < 0)
    //     {
    //         perror("Pipe Failed");
    //         exit(EXIT_FAILURE);
    //     }

    //     int pid = fork();
    //     if (pid < 0)
    //     {
    //         perror("Fork Failed");
    //         exit(EXIT_FAILURE);
    //     }

    //     if (pid == 0) // Child process
    //     {
    //         // If not the first command, redirect input from the previous pipe's read end
    //         if (i > 0)
    //         {
    //             dup2(pipe_fd[i - 1][0], STDIN_FILENO);
    //         }

    //         // If not the last command, redirect output to the current pipe's write end
    //         if (i < n - 1)
    //         {
    //             dup2(pipe_fd[i][1], STDOUT_FILENO);
    //         }

    //         // Close all pipe file descriptors in the child process
    //         for (int j = 0; j < n - 1; j++)
    //         {
    //             close(pipe_fd[j][0]);
    //             close(pipe_fd[j][1]);
    //         }

    //         // For the last command in the pipeline, handle redirection
    //         if (i == n - 1)
    //         {
    //             processRedirection(token, home_dir, previous_dir, log_filename, List);
    //         }
    //         else
    //         {
    //             // For intermediate commands, just execute the command
    //             execl("/bin/sh", "sh", "-c", token, (char *)(NULL));
    //             perror("Exec Failed");
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     // Parent process closes its ends of the previous pipe
    //     if (i > 0)
    //     {
    //         close(pipe_fd[i - 1][0]);
    //         close(pipe_fd[i - 1][1]);
    //     }

    //     // Move to the next command
    //     token = strtok(NULL, "|");
    //     i++;
    // }

    // // Close the last pipe's file descriptors in the parent process
    // if (i == n - 1)
    // {
    //     close(pipe_fd[i - 1][0]);
    //     close(pipe_fd[i - 1][1]);
    // }

    // // Wait for all child processes to finish
    // for (i = 0; i < n; i++)
    // {
    //     wait(NULL);
    // }

    // free(command); // Free the duplicated input string
    int n = numberOfPipes(input);
    int pipe_fd[n][2];
    char *token = strtok(input, "|");
    int i = 0;

    int print_directory = 1;

    while (token)
    {
        if (i < n && pipe(pipe_fd[i]) < 0)
        {
            perror("Pipe Failed");
            exit(EXIT_FAILURE);
        }

        int pid = fork();
        if (pid < 0)
        {
            perror("Fork Failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (i > 0)
            {
                dup2(pipe_fd[i - 1][0], STDIN_FILENO);
            }

            if (i < n)
            {
                dup2(pipe_fd[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < n; j++)
            {
                close(pipe_fd[j][0]);
                close(pipe_fd[j][1]);
            }
            processRedirection(token, home_dir, previous_dir, log_filename, fgbg);
            exit(0);
        }
        else
        {
            if (i > 0)
            {
                close(pipe_fd[i - 1][0]);
                close(pipe_fd[i - 1][1]);
            }
            token = strtok(NULL, "|");
            i++;
        }
    }

    for (i = 0; i < n; i++)
    {
        close(pipe_fd[i][0]);
        close(pipe_fd[i][1]);
    }

    for (i = 0; i <= n; i++)
    {
        wait(NULL);
    }
}