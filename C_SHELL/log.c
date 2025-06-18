#include "log.h"
#include "inputReq.h"
int numberOfCommands(FILE *fp) // number of log commands in the file
{
    char *line = (char *)malloc(sizeof(char) * buf_size);
    int count = 0;
    while (fgets(line, buf_size, fp))
    {
        count++;
    }
    return count;
}
void logPurge(char *logFilename) // clears log file
{
    FILE *fp = fopen(logFilename, "w");
    fclose(fp);
}
void logPrint(char *logFilename) // prints all the commands
{
    char *line = (char *)malloc(sizeof(char) * buf_size);
    FILE *fp = fopen(logFilename, "r");
    while (fgets(line, buf_size, fp))
    {
        printf("%s", line);
    }
}
// writing into file if the command contains more 15 commands
void logMoreThan15(const char *logFilename, char *input)
{
    FILE *fp = fopen(logFilename, "r");
    if (!fp)
    {
        perror("\033[0;31mError opening file\033[0");
        return;
    }

    FILE *fp1 = fopen("temp.txt", "w"); // store all the commands in a new file  -> text.txt
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
    // skip one command which has to be removed -> first line of the file is last entered command
    if (fgets(line, buf_size, fp) == NULL)
    {
        perror("\033[0;31mError reading the first line\033[0");
        free(line);
        fclose(fp);
        fclose(fp1);
        return;
    }
    // copy all the commands into a temporary file
    while (fgets(line, buf_size, fp) != NULL)
    {
        fputs(line, fp1);
    }

    fclose(fp);
    fclose(fp1);
    // open the log file in write mode to clear everything
    fp = fopen(logFilename, "w");
    fp1 = fopen("temp.txt", "r"); // reopen temporary file in read mode to copy remaining files
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
    // refill the log file with remaining commands
    while (fgets(line, buf_size, fp1) != NULL)
    {
        fputs(line, fp);
    }
    // if the command is not log -> enter that in the file
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

void log_Write(char *input, char *logFilename)
{
    // initialisation
    int fd = open(logFilename, O_RDWR | O_APPEND, 0644); // open the file in read/append mode
    char *line = (char *)malloc(sizeof(char) * buf_size);
    char *latest_command = (char *)malloc(sizeof(char) * buf_size);
    strcpy(latest_command, "");
    FILE *fp = fopen(logFilename, "r");
    int count = 0;

    // to get latest command and number of commands
    while (fgets(line, buf_size, fp))
    {
        strcpy(latest_command, line); // collect the latest command
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
        logMoreThan15(logFilename, input);
    }
}
void log_fun(char *logFilename, char *input, char *home_dir, char *previous_directory)
{
    input[strcspn(input, "\n")] = 0;

    // if command is log
    if (strcmp(input, "log") == 0)
    {
        logPrint(logFilename);
        return;
    }

    // if command is log execute
    else if (strstr(input, "execute")) // log execute num -> execute num command in log file
    {
        // initialisation
        char *token = (char *)malloc(sizeof(char) * buf_size);
        char *integer = (char *)malloc(sizeof(char) * buf_size);
        char *line = (char *)malloc(sizeof(char) * buf_size);
        char *c = (char *)malloc(sizeof(char) * buf_size);

        // tokenise the input to get the index
        // assuming it will be of form log execute num
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
        FILE *fp = fopen(logFilename, "r"); // open the log file
        int numC = numberOfCommands(fp);    // get number of commands the current file is holding
        in = numC - in + 1;                 // get the index of the command that we need to execute
        if (in <= 0)                        // if index > number of commands in log -> invalid index
        {
            printf("\033[0;31mInvalid index\033[0\n");
            return;
        }
        int i = 0;
        fclose(fp);
        fp = fopen(logFilename, "r");
        while (fgets(line, buf_size, fp) != NULL) // traverse to the command that needs to be executed
        {
            i++;
            strcpy(c, line);
            if (i == in)
            {
                break;
            }
        }
        // execute that command through
        // send that command to execution
        dividingCommands(c, home_dir, logFilename, previous_directory);
        fclose(fp);
        free(token);
        free(integer);
        free(line);
        free(c);
    }

    // if command is log purge
    else if (strstr(input, "purge")) // delete the content of the log file
    {
        logPurge(logFilename);
    }
}
