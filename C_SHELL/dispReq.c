#include "dispReq.h"

void trimWhitespaces(char *str) // eliminates whitespaces from the string
{
    char *end;
    // removes whitespaces from front
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return;
    // removes whitespaces from back
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';
}

void print_The_Directory(char *home_dir)
{
    char *dir = (char *)malloc(sizeof(char) * buf_size);
    char *user_name = (char *)malloc(sizeof(char) * buf_size);
    struct utsname os; // a struct used to store information about operating system and machine
    int x = 0;
    int flag = 0;
    getlogin_r(user_name, buf_size); // gets the username logged into os and stores in user_name
    getcwd(dir, buf_size);           // gets current working directory and stores in dir variable
    uname(&os);                      // fills the details of os in os struct

    char *filename = (char *)malloc(sizeof(char) * buf_size);
    strcpy(filename, home_dir);
    strcat(filename, "/test.txt");
    FILE *fp = fopen(filename, "r");
    char *line = (char *)malloc(sizeof(char) * buf_size);
    fgets(line, buf_size, fp);
    if (strcmp(line, "1\n") == 0) // a bit map to indicate whether a process (previous) took more than 2 secs or not
    {
        flag = 1; // flag will be set to 1 if it took
    }
    char *rel_dir = (char *)malloc(sizeof(char) * buf_size);

    // if the present directory is not the home directory -> parent directories of home directory
    // show absolute path of that
    // homedir = /home/JohnDoe/Project
    // dir = /home/JohnDoe/
    if (strcmp(home_dir, dir) > 0)
    {
        printf("1%s\n", dir);
        strcpy(rel_dir, dir);
    }
    // if the present directory is present in homedirectory -> scrape home directory from that path
    // scrape home dir from dir
    // homedir = /home/JohnDoe/
    // dir = /home/JohnDoe/Project
    if (strcmp(home_dir, dir) < 0)
    {
        x = strlen(home_dir);
        strcpy(rel_dir, dir + x);
    }
    // if it is the home directory represent it by ~
    if (strcmp(home_dir, dir) == 0)
    {
        strcpy(rel_dir, "~");
    }
    printf("\033[1;32m");
    printf("<%s@%s", user_name, os.nodename);
    printf("\033[1;37m:~");
    if (strcmp(rel_dir, "~") != 0) // just to account for different orders in two cases
        printf("%s ", rel_dir);
    if (flag == 1)
    {
        fgets(line, buf_size, fp);
        printf(":%s> ", line); // the name of process that took more than 2 secs
        fclose(fp);
        fp = fopen(filename, "w");
    }
    else
    {
        printf("> ");
    }
    printf("\033[0m");
}