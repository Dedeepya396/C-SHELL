#include "hop.h"
#include "dispReq.h"

// to handle hop . or hop .. or hop - cases
void hop(int x, char *previous_directory)
{
    char *dir = (char *)malloc(sizeof(char) * buf_size);
    if (x == 0)
    {
        getcwd(dir, buf_size);
        strcpy(previous_directory, dir);
        chdir(".");
    }
    if (x == 1)
    {
        getcwd(dir, buf_size);
        strcpy(previous_directory, dir);
        chdir("..");
    }
    if (x == 2)
    {
        getcwd(dir, buf_size);
        if (chdir(previous_directory) == 0)
            ;
        strcpy(previous_directory, dir);
    }
}
int compare_(char *a)
{
    if (strcmp(a, ".") == 0 || strcmp(a, "..") == 0 || strcmp(a, "-") == 0 || strcmp(a, "~") == 0)
        return 1;
    else
        return 0;
}
int getFlags(char *command)
{
    char *token = (char *)malloc(sizeof(char) * buf_size);
    token = strtok(command, " \t");
    int i = 0;
    char *flag = (char *)malloc(sizeof(char) * buf_size);
    while (token != NULL)
    {
        if (i == 1)
            strcpy(flag, token);
        token = strtok(NULL, " \t");
        i++;
    }
    if (strcmp(flag, "..") == 0)
        return 1;
    else if (strcmp(flag, ".") == 0)
        return 0;
    else if (strcmp(flag, "-") == 0)
        return 2;
    else if (strcmp(flag, "~") == 0)
        return 3;
    else
        return 4;
}
void hop_fun(char *cmd, char *home_dir, char *previous_directory)
{
    char *dir = (char *)malloc(sizeof(char) * buf_size);
    char *start = (char *)malloc(sizeof(char) * buf_size);
    char *store = (char *)malloc(sizeof(char) * buf_size);
    char *copy = (char *)malloc(sizeof(char) * buf_size);
    cmd[strcspn(cmd, "\n")] = 0;
    cmd[strlen(cmd)] = '\0';
    trimWhitespaces(cmd);
    // printf("---%s---\n", cmd);
    if (strcmp(cmd, "hop") == 0)
    {
        getcwd(previous_directory,buf_size);
        chdir(home_dir);
        char *curr_dir = (char *)malloc(sizeof(char) * buf_size);
        getcwd(curr_dir, buf_size);
        printf("%s\n", curr_dir);
        return;
    }
    strcpy(copy, cmd);
    int Fflag = getFlags(copy);
    if (Fflag == 1)
    {
        hop(1, previous_directory);
    }
    else if (Fflag == 0)
    {
        hop(0, previous_directory);
    }
    else if (Fflag == 2)
    {
        hop(2, previous_directory);
    }
    else if (Fflag == 3)
    {
        getcwd(dir, buf_size);
        strcpy(previous_directory, dir);
        chdir(home_dir);
    }

    char *token = (char *)malloc(sizeof(char) * buf_size);
    char *temp = (char *)malloc(sizeof(char) * buf_size);
    strcpy(temp, cmd);
    token = strtok(temp, " \t");
    while (token != NULL)
    {
        if (compare_(token) || strcmp(token, "hop") == 0)
        {
            token = strtok(NULL, " \t");
            continue;
        }
        else
        {
            char *abs_dir = (char *)malloc(sizeof(char) * buf_size);
            strcpy(abs_dir, "");
            if (strstr(token, home_dir) && strcmp(token, home_dir) > 0) // token > homedir
            {
                char *dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(dir, buf_size);
                strcpy(abs_dir, token);
                int x = chdir(abs_dir);
                if (x != 0)
                    perror("\033[0;31mError:\033[0m");
                else
                    strcpy(previous_directory, dir);
                token = strtok(NULL, " \t");
                continue;
            }
            if (strstr(token, home_dir) && strcmp(token, home_dir) == 0) // token == homedir
            {
                if (chdir(token) != 0)
                    perror("\033[0;31mError:\033[0m");
                token = strtok(NULL, " \t");
                continue;
            }
            if (strstr(home_dir, token)) // token < homedir
            {

                char *dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(dir, buf_size);
                int x = chdir(abs_dir);
                if (x != 0)
                    perror("\033[0;31mError:\033[0m");
                else
                    strcpy(previous_directory, dir);
                token = strtok(NULL, " \t");
                continue;
            }
            if (!strstr(token, home_dir)) // relative path
            {
                char *dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(dir, buf_size);
                // strcpy(previous_directory, dir);
                if (token[0] == '/')
                {
                    char *current_dir = (char *)malloc(sizeof(char) * buf_size);
                    getcwd(current_dir, buf_size);
                    strcpy(abs_dir, current_dir);
                    strcat(abs_dir, token);
                    int x = chdir(abs_dir);
                    if (x != 0)
                        perror("\033[0;31mError:\033[0m");
                    else
                        strcpy(previous_directory, dir);
                    token = strtok(NULL, " \t");
                    continue;
                }
                if ((token[0] == '.' || token[0] == '~' || token[0] == '-') && token[0] != '/')
                {
                    char *dir = (char *)malloc(sizeof(char) * buf_size);
                    getcwd(dir, buf_size);
                    strcpy(previous_directory, dir);
                    if (token[0] == '~')
                    {
                        chdir(home_dir);
                        chdir(token + 2);
                    }
                    else
                    {
                        int x = chdir(abs_dir);
                        if (x != 0)
                            perror("\033[0;31mError:\033[0m");
                        else
                            strcpy(previous_directory, dir);
                    }
                    token = strtok(NULL, " \t");
                    continue;
                }
                if (token[0] != '/')
                {
                    char *current_dir = (char *)malloc(sizeof(char) * buf_size);
                    getcwd(current_dir, buf_size);
                    strcpy(abs_dir, current_dir);
                    strcat(abs_dir, "/");
                    strcat(abs_dir, token);
                    int x = chdir(abs_dir);
                    if (x != 0)
                        perror("\033[0;31mError:\033[0m");
                    else
                        strcpy(previous_directory, dir);
                    token = strtok(NULL, " \t");
                    continue;
                }
            }
        }
        token = strtok(NULL, " \t");
    }
    char *curr_dir = (char *)malloc(sizeof(char) * buf_size);
    getcwd(curr_dir, buf_size);
    printf("%s\n", curr_dir);
}
