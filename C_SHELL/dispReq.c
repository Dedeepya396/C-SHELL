#include "dispReq.h"

void trimWhitespaces(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';
}

void print_The_Directory(char *home_dir)
{
    char *dir = (char *)malloc(sizeof(char) * buf_size);
    char *user_name = (char *)malloc(sizeof(char) * buf_size);
    struct utsname os;
    int x = 0;
    int flag = 0;
    getlogin_r(user_name, buf_size);
    getcwd(dir, buf_size);
    uname(&os);

    char *filename = (char *)malloc(sizeof(char) * buf_size);
    strcpy(filename, home_dir);
    strcat(filename, "/test.txt");
    FILE *fp = fopen(filename, "r");
    char *line = (char *)malloc(sizeof(char) * buf_size);
    fgets(line, buf_size, fp);
    if (strcmp(line, "1\n") == 0)
    {
        flag = 1;
    }
    char *rel_dir = (char *)malloc(sizeof(char) * buf_size);

    // if the present directory is not the home directory scrape the home directory
    if (strcmp(home_dir, dir) > 0)
    {
        strcpy(rel_dir, dir);
    }
    if (strcmp(home_dir, dir) < 0)
    {
        x += strlen(home_dir);
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
    if (strcmp(rel_dir, "~") != 0)
        printf("%s ", rel_dir);
    if (flag == 1)
    {
        fgets(line, buf_size, fp);
        printf(":%s> ", line);
        fclose(fp);
        fp = fopen(filename, "w");
    }
    else
    {
        printf("> ");
    }
    printf("\033[0m");
}