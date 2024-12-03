#include "pipes.h"
#include "systemCommands.h"
int numberOfPipes(char *input)
{
    int count = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '|')
        {
            count++;
        }
    }
    return count;
}

void processPipes(char *input,char* home_dir)
{
    int n = numberOfPipes(input) + 1;
    int pipe_fd[n - 1][2];
    char *token = strtok(input, "|");
    int i = 0;

    while (token)
    {
        if (i < n - 1 && pipe(pipe_fd[i]) < 0)
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
                if (dup2(pipe_fd[i - 1][0], STDIN_FILENO) == -1)
                {
                    perror("dup2 failed for stdin");
                    exit(EXIT_FAILURE);
                }
            }

            if (i < n - 1)
            {
                if (dup2(pipe_fd[i][1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed for stdout");
                    exit(EXIT_FAILURE);
                }
            }

            for (int j = 0; j < n - 1; j++)
            {
                close(pipe_fd[j][0]);
                close(pipe_fd[j][1]);
            }
            execl("/bin/sh", "sh", "-c", token, (char *)(NULL));
            // foreground_process(token,home_dir);
            perror("Exec Failed");
            exit(EXIT_FAILURE);
        }
        appendProcessList(token, pid, 1);

        if (i > 0)
        {
            close(pipe_fd[i - 1][0]);
            close(pipe_fd[i - 1][1]);
        }

        token = strtok(NULL, "|");
        i++;
    }

    if (i == n - 1)
    {
        close(pipe_fd[i - 1][0]);
        close(pipe_fd[i - 1][1]);
    }

    for (i = 0; i < n; i++)
    {
        wait(NULL);
    }
}