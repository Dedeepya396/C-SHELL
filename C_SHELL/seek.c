#include "seek.h"

int compare_s(char *filename, char *searchname)
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

void seek(char *input, char *home_dir)
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
        printf("\033[0;31m Invalid Flags \033[0 \n");
        return;
    }
    char final_DIR[buf_size];

    strcpy(final_DIR, home_dir);
    strcat(final_DIR, direct);
    int len = strlen(final_DIR);
    int flag = 0, numDir = 0, numFiles = 0, numExec = 0;

    seek_fun(filename, dir, files, exec, final_DIR, len, &flag, &numDir, &numFiles, &numExec);

    if (flag == 0)
        printf("\033[0;31m No match found! \033[0\n");
}

void seek_fun(char *filename, int dir, int files, int exec, char *directory, int home_len, int *flag, int *numDir, int *numFiles, int *numExec)
{
    struct dirent *d;
    DIR *Di = opendir(directory);
    if (Di == NULL)
    {
        perror("\033[0;31m opendir \033[0");
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
            perror("\033[0;31m stat \033[0");
            continue;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            if ((dir > 0 || (files == 0 && dir == 0)) && strcmp(d->d_name, filename) == 0)
            {
                if (exec > 0 && access(filepath, X_OK) != 0)
                {
                    printf("\033[0;31m Missing permissions for task!\033[0m\n");
                    closedir(Di);
                    *flag = 1;
                    return;
                }
                *flag = 1;
                printf("\033[0;34m%s\n\033[0m", filepath + home_len);
                (*numDir)++;
                if (*numDir == 1)
                    strcpy(eDirectory, filepath);
                if (*numDir > 1)
                    strcpy(eDirectory, "");
            }
            seek_fun(filename, dir, files, exec, filepath, home_len, flag, numDir, numFiles, numExec);
        }
        else if (S_ISREG(fileStat.st_mode))
        {
            if (((files > 0 || (files == 0 && dir == 0)) && (fileStat.st_mode & S_IXUSR)) && strcmp(d->d_name, filename) == 0)
            {

                if (exec > 0 && access(filepath, R_OK) != 0)
                {
                    printf("\033[0;31m Missing permissions for task!\033[0m\n");
                    closedir(Di);
                    *flag = 1;
                    return;
                }
                *flag = 1;
                printf("\033[0;32m%s\n\033[0m", filepath + home_len);
                (*numExec)++;
            }
            else if ((files > 0 || (files == 0 && dir == 0)) && compare_s(d->d_name, filename))
            {
                if (exec > 0 && access(filepath, R_OK) != 0)
                {
                    printf("\033[0;31m Missing permissions for task!\033[0m\n");
                    closedir(Di);
                    *flag = 1;
                    return;
                }
                *flag = 1;
                printf("\033[0;32m%s\033[0m\n", filepath + home_len);
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
