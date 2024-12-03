#include "redirection.h"
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include "dispReq.h"
#include "hop.h"
// #include "reveal.h"
// #include "seek.h"
#include "proclore.h"
#include "log.h"
#include "systemCommands.h"
#include "FG_BG.h"
#include "nocolors.h"
#include "networking.h"
#include "neonate.h"
int processUserCommands(char *input)
{
    if (strstr(input, "hop") || strstr(input, "log") || strstr(input, "proclore") || strstr(input, "reveal") || strstr(input, "seek") || strncmp(input, "fg", 2) == 0 || strncmp(input, "bg", 2) == 0||strncmp(input,"iMan",4)==0||strncmp(input,"neonate -n",10)==0)
        return 1;
    else
        return 0;
}
int GetProcloreID(char *command)
{
    char *token1 = (char *)malloc(sizeof(char) * buf_size);
    token1 = strtok(command, " \t");
    int i = 0;
    int num = -1;
    while (token1 != NULL)
    {
        if (i == 1)
        {
            num = atoi(token1);
        }
        token1 = strtok(NULL, " \t");
        i++;
    }
    return num;
}

void processRedirection(char *input, char *home_dir, char *previous_dir, char *log_filename, int fgbg)
{
    trimWhitespaces(input);
    char *command = (char *)malloc(sizeof(char) * buf_size);
    strcpy(command, input);
    int copy_in = dup(0);
    int copy_out = dup(1);

    int fd_in = -1, fd_out = -1;

    if (strstr(command, ">>") != NULL)
    {
        char *filename_part = strstr(command, ">>");
        filename_part[0] = '\0';
        filename_part += 2;
        trimWhitespaces(filename_part);
        char *filename = strtok(filename_part, " ;");
        fd_out = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd_out < 0)
        {
            perror("File opening failed");
            return;
        }
        if (dup2(fd_out, STDOUT_FILENO) == -1)
        {
            perror("dup2 failed for stdout");
            return;
        }
    }
    // Handle output redirection (overwrite mode: >)
    else if (strstr(command, ">") != NULL)
    {
        char *filename_part = strstr(command, ">");
        filename_part[0] = '\0';
        filename_part += 1;
        trimWhitespaces(filename_part);
        char *filename = strtok(filename_part, " ;");
        fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0)
        {
            perror("File opening failed");
            return;
        }
        if (dup2(fd_out, STDOUT_FILENO) == -1)
        {
            perror("dup2 failed for stdout");
            return;
        }
    }

    // Handle input redirection (<)
    if (strstr(command, "<") != NULL)
    {
        char *filename_part = strstr(command, "<");
        filename_part[0] = '\0';
        filename_part += 1;
        trimWhitespaces(filename_part);
        char *filename = strtok(filename_part, " ;");
        fd_in = open(filename, O_RDONLY);
        if (fd_in < 0)
        {
            perror("Input file opening failed");
            return;
        }
        if (dup2(fd_in, STDIN_FILENO) == -1)
        {
            perror("dup2 failed for stdin");
            return;
        }
    }

    // Process commands
    if (processUserCommands(command))
    {
        if (strncmp(command, "hop", 3) == 0)
        {
            hop_fun(command, home_dir, previous_dir);
        }
        else if (strncmp(command, "log", 3) == 0)
        {
            log_fun(log_filename, command, home_dir, previous_dir);
        }
        else if (strncmp(command, "seek", 4) == 0)
        {
            seek_color(command, home_dir);
        }
        else if (strncmp(command, "reveal", 6) == 0)
        {
            reveal_fun_color(command, home_dir);
        }
        else if (strncmp(command, "proclore", 8) == 0)
        {
            int ID = GetProcloreID(command);
            if (ID != -1)
            {
                procloreID(ID);
            }
            else
            {
                proclore();
            }
        }
        else if (strncmp(command, "fg", 2) == 0 || strncmp(command, "bg", 2) == 0)
        {
            GetID_FG_BG(command, home_dir);
        }
        else if (strncmp(command, "iMan", 4) == 0)
        {
            fetch_man_page(command);
        }
        else if (strncmp(command, "neonate -n", 10) == 0)
        {
            neonate(command);
        }
    }
    else
    {
        if (fgbg == 0)
        {
            background_process(command);
        }
        else
            foreground_process(command, home_dir);
    }
}