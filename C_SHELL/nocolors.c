#include "nocolors.h"
int compare_names_color(const void *a, const void *b)
{
    struct dirent *d1 = *(struct dirent **)a;
    struct dirent *d2 = *(struct dirent **)b;
    return strcasecmp(d1->d_name, d2->d_name);
}
void print_permissions_color(struct stat *fileStat)
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
void reveal_permissions_color(char *directory, int a_flag)
{
    struct dirent **entries;
    int count = 0;
    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        perror("opendir ");
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
    qsort(entries, count, sizeof(struct dirent *), compare_names_color);

    // Process sorted entries
    for (i = 0; i < count; i++)
    {
        if (a_flag == 0 && entries[i]->d_name[0] == '.')
            continue;

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, entries[i]->d_name);
        struct stat fileStat;

        if (stat(filepath, &fileStat) < 0)
        {
            perror("stat ");
            continue;
        }

        print_permissions_color(&fileStat);
        printf("%lu ", fileStat.st_nlink);

        struct passwd *pw = getpwuid(fileStat.st_uid);
        printf("\t%s ", pw->pw_name);

        struct group *gr = getgrgid(fileStat.st_gid);
        printf("\t%s ", gr->gr_name);

        printf("\t%5ld ", fileStat.st_size);
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
        printf("\t%s\t", timebuf);

        printf("%s\n", entries[i]->d_name);
    }

    // Clean up
    free(entries);
    closedir(dir);
}

void reveal_color(char *directory, int a_flag)
{
    struct dirent **entries;
    int count = 0;
    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        perror("opendir ");
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

    qsort(entries, count, sizeof(struct dirent *), compare_names_color);

    for (i = 0; i < count; i++)
    {
        if (a_flag == 0 && entries[i]->d_name[0] == '.')
            continue;

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, entries[i]->d_name);
        struct stat fileStat;

        if (stat(filepath, &fileStat) < 0)
        {
            perror("stat ");
            continue;
        }
        printf("%s\n", entries[i]->d_name);
    }

    free(entries);
    closedir(dir);
}


void reveal_fun_color(char *cmd, char *home_dir)
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
        if (strstr(token, "-"))
        {
            for (int i = 0; i < strlen(token); i++)
            {
                if (token[i] == 'a')
                    a_flag = 1;
                if (token[i] == 'l')
                    l_flag = 1;
            }
        }
        else
        {
            strcat(directory, token);
        }
        token = strtok(NULL, " \t");
    }
    directory[strlen(directory)] = '\0';
    char *absolute_path = (char *)malloc(sizeof(char) * buf_size);
    int flag = 0;
    if (directory[0] == '~')
    {
        char temp[buf_size];
        strcpy(temp, directory);
        strcpy(directory, temp + 1);
        strcpy(absolute_path, home_dir);
        strcat(absolute_path, directory);
        flag = 1;
    }
    printf("dir: %s\n", directory);

    if (strcmp(directory, "~") == 0)
    {
        strcpy(absolute_path, home_dir);
    }
    else if (strcmp(directory, ".") == 0)
    {
        strcpy(absolute_path, ".");
    }
    else if (strcmp(directory, "..") == 0)
    {
        char *dir = (char *)malloc(sizeof(char) * buf_size);
        getcwd(dir, buf_size);
        if (strcmp(home_dir, dir) == 0)
            strcpy(absolute_path, ".");
        else
            strcpy(absolute_path, "..");
    }
    else
    {
        if(flag==0)
        {
            if (strncmp(directory, home_dir, strlen(home_dir)) != 0)
            {
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
    printf("abs: %s\n", absolute_path);
    if (l_flag)
    {
        reveal_permissions_color(absolute_path, a_flag);
    }
    else
    {
        reveal_color(absolute_path, a_flag);
    }
    free(token);
    free(directory);
    free(absolute_path);
}

int compare_s_color(char *filename, char *searchname)
{
    char *token = strtok(filename, ".");
    while (token != NULL)
    {
        if (strcmp(token, searchname) == 0)
            return 1;
        token = strtok(NULL, ".");
    }
    return 0;
}

void seek_color(char *input, char *home_dir)
{
    char *token;
    int dir = 0, files = 0, exec = 0;
    char filename[buf_size] = "";
    char direct[buf_size] = "";

    token = strtok(input, " \t");
    while (token != NULL)
    {
        if (strcmp(token, "seek") == 0)
        {
            token = strtok(NULL, " \t");
            continue;
        }

        if (token[0] == '-')
        {
            for (int i = 1; i < strlen(token); i++)
            {
                if (token[i] == 'f')
                    files++;
                if (token[i] == 'd')
                    dir++;
                if (token[i] == 'e')
                    exec++;
            }
        }
        else if (strstr(token, "/"))
        {
            strcpy(direct, token);
        }
        else
        {
            strcpy(filename, token);
        }
        token = strtok(NULL, " \t");
    }

    if (dir > 0 && files > 0)
    {
        printf("Invalid Flags  \n");
        return;
    }
    char final_DIR[buf_size];
    strcpy(final_DIR, home_dir);
    strcat(final_DIR, direct);
    int len = strlen(final_DIR);
    int flag = 0, numDir = 0, numFiles = 0, numExec = 0;

    seek_fun_color(filename, dir, files, exec, final_DIR, len, &flag, &numDir, &numFiles, &numExec);

    if (flag == 0)
        printf("No match found! \n");
}

void seek_fun_color(char *filename, int dir, int files, int exec, char *directory, int home_len, int *flag, int *numDir, int *numFiles, int *numExec)
{
    struct dirent *d;
    DIR *Di = opendir(directory);
    if (Di == NULL)
    {
        perror("opendir ");
        return;
    }
    char eDirectory[buf_size] = "";
    char eFiles[buf_size] = "";
    while ((d = readdir(Di)) != NULL)
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
        {
            continue;
        }
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, d->d_name);
        struct stat fileStat;

        if (stat(filepath, &fileStat) < 0)
        {
            perror("stat ");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            if ((dir > 0 || (files == 0 && dir == 0)) && strcmp(d->d_name, filename) == 0)
            {
                if (exec > 0 && access(filepath, X_OK) != 0)
                {
                    printf("Missing permissions for task!\n");
                    closedir(Di);
                    *flag = 1;
                    return;
                }
                *flag = 1;
                printf("%s\n", filepath + home_len);
                (*numDir)++;
                if (*numDir == 1)
                    strcpy(eDirectory, filepath);
                if (*numDir > 1)
                    strcpy(eDirectory, "");
            }
            seek_fun_color(filename, dir, files, exec, filepath, home_len, flag, numDir, numFiles, numExec);
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            if (((files > 0 || (files == 0 && dir == 0)) && (fileStat.st_mode & S_IXUSR)) && strcmp(d->d_name, filename) == 0)
            {

                if (exec > 0 && access(filepath, R_OK) != 0)
                {
                    printf("Missing permissions for task!\n");
                    closedir(Di);
                    *flag = 1;
                    return;
                }
                *flag = 1;
                printf("%s\n", filepath + home_len);
                (*numExec)++;
            }
            else if ((files > 0 || (files == 0 && dir == 0)) && compare_s_color(d->d_name, filename))
            {
                if (exec > 0 && access(filepath, R_OK) != 0)
                {
                    printf("Missing permissions for task!\n");
                    closedir(Di);
                    *flag = 1;
                    return;
                }
                *flag = 1;
                printf("%s\n", filepath + home_len);
                (*numFiles)++;
                if (*numFiles == 1)
                    strcpy(eFiles, filepath);
                if (*numFiles > 1)
                    strcpy(eFiles, "");
            }
        }
    }
    if (*numDir == 1 && *numFiles == 0 && *numExec == 0 && exec > 0 && files == 0)
    {
        chdir(eDirectory);
        *flag = 1;
    }

    if (*numDir == 0 && *numFiles == 1 && *numExec == 0 && exec > 0 && dir == 0)
    {
        FILE *fp = fopen(eFiles, "r");
        if (fp != NULL)
        {
            char line[buf_size];
            while (fgets(line, buf_size, fp) != NULL)
            {
                printf("%s\n", line);
            }
            fclose(fp);
        }
        *flag = 1;
    }

    closedir(Di);
}

