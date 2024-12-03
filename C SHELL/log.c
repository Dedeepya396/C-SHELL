#include "log.h"
#include "inputReq.h"
int numberOfCommands(FILE *fp)
{
    char *line = (char *)malloc(sizeof(char) * buf_size);
    int count = 0;
    while (fgets(line, buf_size, fp))
    {
        count++;
    }
    return count;
}
void logPurge(char *filename)
{
    FILE *fp = fopen(filename, "w");
    fclose(fp);
}
void logPrint(char *filename)
{
    char *line = (char *)malloc(sizeof(char) * buf_size);
    FILE *fp = fopen(filename, "r");
    while (fgets(line, buf_size, fp))
    {
        printf("%s", line);
    }
}

void logMoreThan15(const char *filename, char *input)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("\033[0;31mError opening file\033[0");
        return;
    }

    FILE *fp1 = fopen("temp.txt", "w");
    if (!fp1)
    {
        perror("\033[0;31mError opening temp file\033[0");
        fclose(fp);
        return;
    }

    char *line = (char *)malloc(buf_size);
    if (!line)
    {
        perror("Memory allocation failed");
        fclose(fp);
        fclose(fp1);
        return;
    }

    if (fgets(line, buf_size, fp) == NULL)
    {
        perror("\033[0;31mError reading the first line\033[0");
        free(line);
        fclose(fp);
        fclose(fp1);
        return;
    }

    while (fgets(line, buf_size, fp) != NULL)
    {
        fputs(line, fp1);
    }

    fclose(fp);
    fclose(fp1);

    fp = fopen(filename, "w");
    fp1 = fopen("temp.txt", "r");
    if (!fp || !fp1)
    {
        perror("\033[0;31mError reopening files\033[0");
        free(line);
        if (fp)
            fclose(fp);
        if (fp1)
            fclose(fp1);
        return;
    }

    while (fgets(line, buf_size, fp1) != NULL)
    {
        fputs(line, fp);
    }
    if (!strstr(input, "log"))
    {
        fputs(input, fp);
        fputs("\n", fp);
    }
    free(line);
    fclose(fp);
    fclose(fp1);
    remove("temp.txt");
}

void log_Write(char *input, char *filename)
{
    // initialisation
    int fd = open(filename, O_RDWR | O_APPEND, 0644);
    char *line = (char *)malloc(sizeof(char) * buf_size);
    char *latest_command = (char *)malloc(sizeof(char) * buf_size);
    strcpy(latest_command, "");
    FILE *fp = fopen(filename, "r");
    int count = 0;

    // to get latest command and number of commands
    while (fgets(line, buf_size, fp))
    {
        strcpy(latest_command, line);
        count++;
    }
    fclose(fp);

    latest_command[strcspn(latest_command, "\n")] = '\0'; // removing newline character
    input[strcspn(input, "\n")] = '\0';                   // removing newline character

    // if present command is not the same as previous command and
    // number of instructions is less than 15 and instruction does not contain log
    if (strcmp(input, latest_command) != 0 && count + 1 <= 15 && !strstr(input, "log") && strcmp(input, "\n") != 0)
    {
        write(fd, input, strlen(input));
        write(fd, "\n", 1);
    }
    if (count + 1 > 15)
    {
        logMoreThan15(filename, input);
    }
}
void log_fun(char *filename, char *input, char *home_dir, char *previous_directory)
{
    input[strcspn(input, "\n")] = 0;

    // if command is log
    if (strcmp(input, "log") == 0)
    {
        logPrint(filename);
    }

    // if command is log execute
    else if (strstr(input, "execute"))
    {
        // initialisation
        char *token = (char *)malloc(sizeof(char) * buf_size);
        char *integer = (char *)malloc(sizeof(char) * buf_size);
        char *line = (char *)malloc(sizeof(char) * buf_size);
        char *c = (char *)malloc(sizeof(char) * buf_size);

        // tokenise the input to get the index
        token = strtok(input, " \t");
        while (token != NULL)
        {
            strcpy(integer, token);
            token = strtok(NULL, " \t");
        }
        // converting char* to int
        integer[strcspn(integer, "\n")] = 0;
        int in = atoi(integer);

        // to get the command at that particular index
        FILE *fp = fopen(filename, "r");
        int numC = numberOfCommands(fp);
        in = numC - in + 1;
        if (in <= 0)
        {
            printf("\033[0;31mInvalid index\033[0\n");
            return;
        }
        int i = 0;
        fclose(fp);
        fp = fopen(filename, "r");
        while (fgets(line, buf_size, fp) != NULL)
        {
            i++;
            strcpy(c, line);
            if (i == in)
            {
                break;
            }
        }

        // send that command to execution
        dividingCommands(c, home_dir, filename, previous_directory);
        fclose(fp);
        free(token);
        free(integer);
        free(line);
        free(c);
    }

    // if command is log purge
    else if (strstr(input, "purge"))
    {
        logPurge(filename);
    }
}
