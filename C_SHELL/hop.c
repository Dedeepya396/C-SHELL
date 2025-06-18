#include "hop.h"
#include "dispReq.h"

// to handle standard cases like hop . or hop .. or hop - cases
// we store previous directory in a global string prevDir
// x denotes the kind of command ex: ./../-
void hopStandard(int x, char *prevDir)
{
    char *dir = (char *)malloc(sizeof(char) * buf_size);
    if (x == 0)
    {
        getcwd(dir, buf_size);
        strcpy(prevDir, dir);
        chdir(".");
    }
    if (x == 1)
    {
        getcwd(dir, buf_size);
        strcpy(prevDir, dir);
        chdir("..");
    }
    if (x == 2)
    {
        getcwd(dir, buf_size);
        if (chdir(prevDir) == 0)
            ;
        strcpy(prevDir, dir);
    }
}

int getFlags(char *command)
{
    char *token = (char *)malloc(sizeof(char) * buf_size);
    token = strtok(command, " \t");
    char *flag = (char *)malloc(sizeof(char) * buf_size);
    int i = 0;
    while (token != NULL)
    {
        if (i == 1)
            strcpy(flag, token);
        token = strtok(NULL, " \t");
        i++;
    }
    if (strcmp(flag, "..") == 0) // hop ..
        return 1;
    else if (strcmp(flag, ".") == 0) // hop .
        return 0;
    else if (strcmp(flag, "-") == 0) // hop -
        return 2;
    else if (strcmp(flag, "~") == 0) // hop ~
        return 3;
    else
        return 4; // command is hop /directoryName
}
// function to hop to requested directory if it is valid
void hop(char *cmd, char *home_dir, char *prevDir)
{
    char *dir = (char *)malloc(sizeof(char) * buf_size);
    char *start = (char *)malloc(sizeof(char) * buf_size);
    char *store = (char *)malloc(sizeof(char) * buf_size);
    char *copy = (char *)malloc(sizeof(char) * buf_size);
    cmd[strcspn(cmd, "\n")] = 0;
    cmd[strlen(cmd)] = '\0';
    trimWhitespaces(cmd);
    if (strcmp(cmd, "hop") == 0) // if it is just hop -> change to homedir
    {
        getcwd(prevDir, buf_size);
        chdir(home_dir);
        printf("%s\n", home_dir);
        return;
    }
    strcpy(copy, cmd);
    int Fflag = getFlags(copy);
    if (Fflag != 4)
    {
        if (Fflag == 3)
        {
            getcwd(dir, buf_size);
            strcpy(prevDir, dir);
            chdir(home_dir);
        }
        else
            hopStandard(Fflag, prevDir);
        char *curr_dir = (char *)malloc(sizeof(char) * buf_size);
        getcwd(curr_dir, buf_size);
        printf("%s\n", curr_dir);
        return;
    }

    char *token = (char *)malloc(sizeof(char) * buf_size);
    char *temp = (char *)malloc(sizeof(char) * buf_size);
    strcpy(temp, cmd);
    token = strtok(temp, " \t");
    while (token != NULL)
    {

        char *abs_dir = (char *)malloc(sizeof(char) * buf_size);
        strcpy(abs_dir, "");
        // hop to children directories of homedir
        if (strstr(token, home_dir) && strcmp(token, home_dir) > 0) // token > homedir
        {
            char *dir = (char *)malloc(sizeof(char) * buf_size);
            getcwd(dir, buf_size); // store current directory so can store in prev dir
            strcpy(abs_dir, token);
            int x = chdir(abs_dir);
            if (x != 0)
                perror("\033[0;31mError:\033[0m");
            else
                strcpy(prevDir, dir);
            free(dir);
            token = strtok(NULL, " \t");
            continue;
        }
        // hop to home directory
        if (strstr(token, home_dir) && strcmp(token, home_dir) == 0) // token == homedir
        {
            if (chdir(token) != 0)
                perror("\033[0;31mError:\033[0m");
            token = strtok(NULL, " \t");
            continue;
        }

        // hop to parent directories of home directory
        if (strstr(home_dir, token)) // token < homedir
        {
            char *dir = (char *)malloc(sizeof(char) * buf_size);
            getcwd(dir, buf_size);
            printf("DID IT COME TO CORRECT POSITION?!\n");
            int x = chdir(abs_dir);
            if (x != 0)
                perror("\033[0;31mError:\033[0m");
            else
                strcpy(prevDir, dir);
            token = strtok(NULL, " \t");
            continue;
        }
        if (!strstr(token, home_dir)) // relative path
        {
            char *dir = (char *)malloc(sizeof(char) * buf_size);
            getcwd(dir, buf_size);
            if (token[0] == '/') // if relative path starts with '/'
            {
                char *current_dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(current_dir, buf_size); // store to change to prev dir
                strcpy(abs_dir, current_dir);  // construct abs path by concatenating curr dir + abs path
                strcat(abs_dir, token);
                int x = chdir(abs_dir); // if exists change prev dir to curr dir else print error
                if (x != 0)
                    perror("\033[0;31mError:\033[0m");
                else
                    strcpy(prevDir, dir);
                token = strtok(NULL, " \t");
                continue;
            }
            // starts with ./~/- -> needs to perform them and then move to abs path
            else if ((token[0] == '.' || token[0] == '~' || token[0] == '-'))
            {
                char *dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(dir, buf_size);
                strcpy(prevDir, dir);
                if (token[0] == '~')
                {
                    chdir(home_dir);
                    chdir(token + 2); // ~/x -> chdir(x);
                }
                else
                {
                    strcpy(abs_dir, home_dir); // fill it with homedir+/x
                    strcat(abs_dir, token + 1);
                    int x = chdir(abs_dir);
                    if (x != 0)
                        perror("\033[0;31mError:\033[0m");
                    else
                        strcpy(prevDir, dir);
                }
                token = strtok(NULL, " \t");
                continue;
            }
            else if (token[0] != '/')
            {
                // a relative path that doesnt start with ~/./-/'/'
                // construct a string that is homedir+'/'+ relative path
                char *current_dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(current_dir, buf_size);
                strcpy(abs_dir, current_dir);
                strcat(abs_dir, "/");
                strcat(abs_dir, token);
                int x = chdir(abs_dir);
                if (x != 0)
                    perror("\033[0;31mError:\033[0m");
                else
                    strcpy(prevDir, dir);
                token = strtok(NULL, " \t");
                continue;
            }
        }
        token = strtok(NULL, " \t");
    }
    char *curr_dir = (char *)malloc(sizeof(char) * buf_size);
    getcwd(curr_dir, buf_size);
    // print the changed directory
    printf("%s\n", curr_dir);
}
