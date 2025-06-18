#include "reveal.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int compare_names(const void *a, const void *b)
{
    struct dirent *d1 = *(struct dirent **)a;
    struct dirent *d2 = *(struct dirent **)b;
    return strcasecmp(d1->d_name, d2->d_name);
}
void reveal_permissions(char *directory, int a_flag)
{
    struct dirent **entries;
    int count = 0;
    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        perror("\033[0;31m opendir \033[0 ");
        return;
    }

    // Count the number of entries
    struct dirent *d;
    while ((d = readdir(dir)) != NULL)
    {
        count++;
    }

    // Allocate memory for the array of entries
    entries = (struct dirent **)malloc(count * sizeof(struct dirent *));
    rewinddir(dir);

    // Store entries in the array
    int i = 0;
    while ((d = readdir(dir)) != NULL)
    {
        entries[i] = d;
        i++;
    }

    // Sort the entries array
    qsort(entries, count, sizeof(struct dirent *), compare_names);

    // Process sorted entries
    for (i = 0; i < count; i++)
    {
        if (a_flag == 0 && entries[i]->d_name[0] == '.') // if there is no a flag hide hidden files
            continue;

        char filepath[1024];
        // construct file path by concatenating directory and file name
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, entries[i]->d_name);
        struct stat fileStat;

        if (stat(filepath, &fileStat) < 0)
        {
            perror("\033[0;31m stat \033[0");
            continue;
        }

        print_permissions(&fileStat);
        printf("%lu ", fileStat.st_nlink);

        struct passwd *pw = getpwuid(fileStat.st_uid);
        printf("\t%s ", pw->pw_name);

        struct group *gr = getgrgid(fileStat.st_gid);
        printf("\t%s ", gr->gr_name);

        printf("\t%5ld ", fileStat.st_size);
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
        printf("\t%s\t", timebuf);

        // set the color by the type of file
        if (S_ISDIR(fileStat.st_mode)) // is directory
        {
            printf("\033[0;34m");
        }
        else if (S_ISREG(fileStat.st_mode) && (fileStat.st_mode & S_IXUSR)) // is an executable
        {
            printf("\033[0;32m");
        }
        else if (S_ISREG(fileStat.st_mode)) // is a normal file
        {
            printf("\033[0;37m");
        }
        printf("%s\n", entries[i]->d_name); // print file name/directory name
        printf("\033[0m");
    }

    // Clean up
    free(entries);
    closedir(dir);
}
// prints only the file/directory names contained in the given path
void reveal_(char *directory, int a_flag)
{
    struct dirent **entries;
    int count = 0;
    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        perror("\033[0;31m opendir \033[0");
        return;
    }

    // Count the number of entries
    struct dirent *d;
    while ((d = readdir(dir)) != NULL)
    {
        count++;
    }

    entries = (struct dirent **)malloc(count * sizeof(struct dirent *));
    rewinddir(dir);

    int i = 0;
    while ((d = readdir(dir)) != NULL)
    {
        entries[i] = d;
        i++;
    }

    qsort(entries, count, sizeof(struct dirent *), compare_names);

    for (i = 0; i < count; i++)
    {
        if (a_flag == 0 && entries[i]->d_name[0] == '.') // skip hidden files
            continue;

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, entries[i]->d_name); // construct file path
        struct stat fileStat;

        if (stat(filepath, &fileStat) < 0)
        {
            perror("\033[0;31m stat \033[0");
            continue;
        }
        // set color acc to type of file
        if (S_ISDIR(fileStat.st_mode)) // is directory
        {
            printf("\033[0;34m");
        }
        else if (S_ISREG(fileStat.st_mode) && (fileStat.st_mode & S_IXUSR)) // is an executable file
        {
            printf("\033[0;32m");
        }
        else if (S_ISREG(fileStat.st_mode)) // is a file
        {
            printf("\033[0;37m");
        }

        printf("%s\n", entries[i]->d_name);
        printf("\033[0m");
    }

    free(entries);
    closedir(dir);
}

void print_permissions(struct stat *fileStat)
{
    printf((S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf((fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat->st_mode & S_IXOTH) ? "x" : "-");
}

void reveal_fun(char *cmd, char *home_dir)
{
    char *token = (char *)malloc(sizeof(char) * buf_size);
    int a_flag = 0, l_flag = 0;
    token = strtok(cmd, " \t");
    char *directory = (char *)malloc(sizeof(char) * buf_size);
    strcpy(directory, "");
    while (token != NULL)
    {
        if (strcmp(token, "reveal") == 0)
        {
            token = strtok(NULL, " \t");
            continue;
        }
        if (strstr(token, "-")) // if the token contains information about flag
        {
            for (int i = 0; i < strlen(token); i++)
            {
                if (token[i] == 'a') // if it is a flag
                    a_flag = 1;
                if (token[i] == 'l') // if it is l flag
                    l_flag = 1;
            }
        }
        else
        {
            strcat(directory, token); // copy the directory which we want to reveal
        }
        token = strtok(NULL, " \t");
    }
    directory[strlen(directory)] = '\0';
    char *absolute_path = (char *)malloc(sizeof(char) * buf_size); // construct absolute path
    int flag = 0;
    if (directory[0] == '~') // relative path from home dir
    {
        strcpy(directory, directory + 1);
        strcpy(absolute_path, home_dir);
        strcat(absolute_path, directory);
        flag = 1;
    }

    if (strcmp(directory, "~") == 0) // if the directory contains only ~
    {
        strcpy(absolute_path, home_dir);
    }
    else if (strcmp(directory, ".") == 0) // if the directory contains .
    {
        strcpy(absolute_path, ".");
    }
    else if (strcmp(directory, "..") == 0) // if the directory contains ..
    {
        char *dir = (char *)malloc(sizeof(char) * buf_size);
        getcwd(dir, buf_size);
        if (strcmp(home_dir, dir) == 0) // if we are in home directory we cant reveal over parent directories
            strcpy(absolute_path, ".");
        else
            strcpy(absolute_path, ".."); // else we will reveal over previous directory (parent directory)
    }
    else
    {
        if (flag == 0)
        {
            if (strncmp(directory, home_dir, strlen(home_dir) - 1) != 0)
            {
                char new_dir[buf_size];
                char *dir = (char *)malloc(sizeof(char) * buf_size);
                getcwd(dir, buf_size);
                strcpy(absolute_path, dir);
                if (directory[0] != '/')
                    strcat(absolute_path, "/");
                strcat(absolute_path, directory);
            }
            else
            {
                strcpy(absolute_path, home_dir);
                strcat(absolute_path, directory + strlen(home_dir));
            }
        }
    }
    printf("abs path: %s\n", absolute_path);
    if (l_flag) // if l flag is present
    {
        reveal_permissions(absolute_path, a_flag); // prints the files/ directories in the given path with details
    }
    else
    {
        reveal_(absolute_path, a_flag); // prints the fails/directories in the given path
    }
    free(token);
    free(directory);
    free(absolute_path);
}