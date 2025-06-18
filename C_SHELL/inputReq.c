#include "inputReq.h"
#include "systemCommands.h"
#include "log.h"
#include "proclore.h"
#include "inputReq.h"
#include "hop.h"
#include "reveal.h"
#include "seek.h"
#include "pipes.h"
#include "pipes_redirection.h"
#include "redirection.h"
#include "FG_BG.h"
#include "signals_.h"
#include "networking.h"
#include "shrc.h"
#include "neonate.h"
// detects commands containing redirection/piping
int processCommands(char *input)
{
    if (strstr(input, ">") || strstr(input, ">>") || strstr(input, "<") || strstr(input, "|"))
        return 1;
    else
        return 0;
}
// if command is "proclore id", returns if id exists or -1 otherwise
int getProcloreID(char *command)
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
    free(token1);
    return num;
}

int checkForAliases(char *input, char **modifiedInput)
{
    for (int i = 0; i < alias_top; i++)
    {
        if (strcmp(aliases[i].alias, input) == 0)
        {
            strcpy((*modifiedInput), aliases[i].actual_func);
            return 1;
        }
    }
    (*modifiedInput) = "NOTFOUND";
    return 0;
}
// handles multiple commands separated by ; or &
// tokenizes them by space, concatenates until we encounter a & or ;
// according to that we execute that command in foreground or background
// function which processes commands and redirects them to appropriate functions
void dividingCommands(char *input, char *home_dir, char *filename, char *previous_directory)
{
    input[strcspn(input, "\n")] = 0;
    char *saveptr;
    char *token = strtok_r(input, " \t", &saveptr);
    char command[buf_size] = "";
    char before_and[buf_size] = "";
    while (token != NULL)
    {
        if (strcmp(token, "&") == 0)
        {
            trimWhitespaces(command);
            if (processCommands(command)) // command should be ran in background and redirection/piping is required
            {
                if (strstr(command, "|") != NULL) // contains piping
                {
                    int remove_len = strlen(before_and) + 2;
                    int n = strlen(command) - remove_len;
                    char temp[buf_size];
                    strncpy(temp, command, n);
                    trimWhitespaces(temp);
                    strcpy(command, temp);
                }
                if ((strstr(command, ">") || strstr(command, "<")) && strstr(command, "|") == NULL) // contains only redirection
                {
                    processRedirection(command, home_dir, previous_directory, filename, 1);
                }
                // contains only piping
                if ((strstr(command, "<") == NULL && strstr(command, ">") == NULL) && strstr(command, "|") != NULL)
                {
                    processPipes(command, home_dir);
                }
                else if (((strstr(command, ">") != NULL) || (strstr(command, "<") != NULL)) && (strstr(command, "|") != NULL))
                    both(command, home_dir, previous_directory, filename, 1);
                if (strcmp(before_and, "") != 0)
                {
                    background_process(before_and);
                }
            }
            else
                background_process(command);
            strcpy(command, "");
        }
        else if (strcmp(token, ";") == 0) // if all the processes are to be run in foreground
        {
            trimWhitespaces(command);
            if (processCommands(command))
            {
                if (((strstr(command, ">") != NULL) || (strstr(command, "<") != NULL)) || (strstr(command, "|") != NULL))
                {
                    both(command, home_dir, previous_directory, filename, 1);
                }
            }
            else // command doesnt have redirection/piping
            {
                char *alias = (char *)malloc(sizeof(char) * buf_size);
                strcpy(alias, "");
                checkForAliases(command, &alias); // checks for alias functions
                if (strcmp((alias), "NOTFOUND") != 0)
                {
                    strcpy(command, ""); // if an alias is found replace the command with actual command of that alias
                    strcpy(command, (alias));
                    command[strcspn(command, "\n")] = '\0'; // remove new line in case
                }
                if (strncmp(command, "hop", 3) == 0)
                {
                    hop(command, home_dir, previous_directory);
                }
                else if (strncmp(command, "seek", 4) == 0)
                {
                    seek(command, home_dir);
                }
                else if (strncmp(command, "log", 3) == 0)
                {
                    log_fun(filename, command, home_dir, previous_directory);
                }
                else if (strncmp(command, "proclore", 8) == 0)
                {
                    // command is "proclore" or "proclore id"
                    int ID = getProcloreID(command);
                    if (ID != -1)
                        procloreID(ID);
                    else
                        proclore();
                }
                else if (strncmp(command, "reveal", 6) == 0)
                {
                    reveal_fun(command, home_dir);
                }
                else if (strncmp(command, "fg", 2) == 0 || strncmp(command, "bg", 2) == 0)
                {
                    GetID_FG_BG(command, home_dir);
                }
                else if (strncmp(command, "ping", 4) == 0)
                {
                    getSignal(command);
                }
                else if (strncmp(command, "iMan", 4) == 0)
                {
                    fetch_man_page(command);
                }
                else if (strncmp(command, "neonate -n", 10) == 0)
                {
                    neonate(command);
                }
                else
                    foreground_process(command, home_dir);
            }
            strcpy(command, "");
        }
        else
        {
            // build up the command until we encounter a ; or &
            strcat(command, token);
            strcat(command, " ");
        }
        if (strcmp(token, "|") == 0 || strcmp(token, ";") == 0)
        {
            strcpy(before_and, "");
        }
        else
        {
            strcat(before_and, token);
            strcat(before_and, " ");
        }
        token = strtok_r(NULL, " \t", &saveptr);
    }
    // if we didnt have a ; or & after a command
    // the comparisions will be same as previous ones but we dont compare for ; or &
    // as we are sure that they dont exist in this command
    if (strcmp(command, "") != 0)
    {
        trimWhitespaces(command);
        char *alias = (char *)malloc(sizeof(char) * buf_size);
        strcpy(alias, "");
        checkForAliases(command, &alias);
        if (strcmp((alias), "NOTFOUND") != 0)
        {
            strcpy(command, "");
            strcpy(command, (alias));
            command[strcspn(command, "\n")] = '\0';
        }

        if (processCommands(command))
        {
            if (((strstr(command, ">") != NULL) || (strstr(command, "<") != NULL)) || (strstr(command, "|") != NULL))
            {
                both(command, home_dir, previous_directory, filename, 1);
            }
        }
        else
        {
            if (strncmp(command, "hop", 3) == 0)
            {
                hop(command, home_dir, previous_directory);
            }
            else if (strncmp(command, "seek", 4) == 0)
            {
                seek(command, home_dir);
            }
            else if (strncmp(command, "log", 3) == 0)
            {
                log_fun(filename, command, home_dir, previous_directory);
            }
            else if (strncmp(command, "proclore", 8) == 0)
            {
                int ID = getProcloreID(command);
                if (ID != -1)
                    procloreID(ID);
                else
                    proclore();
            }
            else if (strncmp(command, "reveal", 6) == 0)
            {
                reveal_fun(command, home_dir);
            }
            else if (strncmp(command, "fg", 2) == 0 || strncmp(command, "bg", 2) == 0)
            {
                GetID_FG_BG(command, home_dir);
            }
            else if (strncmp(command, "ping", 4) == 0)
            {
                getSignal(command);
            }
            else if (strncmp(command, "iMan", 4) == 0)
            {
                fetch_man_page(command);
            }
            else if (strncmp(command, "neonate -n", 10) == 0)
            {
                neonate(command);
            }
            else
            {
                foreground_process(command, home_dir);
            }
        }
    }
}