#include "neonate.h"
#include <dirent.h>
#include <ctype.h>

void neonate(char *input)
{
    int wait_time = -1;
    char *token = (char *)malloc(sizeof(char) * buf_size);
    token = strtok(input, " \t");
    int i = 0;
    while (token != NULL)
    {
        if (i == 2)
        {
            wait_time = atoi(token);
        }
        i++;
        token = strtok(NULL, " \t");
    }
    printf("%d\n", wait_time);
    callingProcess(wait_time);
}
int is_key_pressed()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == EOF)
    {
        return 0;
    }

    return ch == 'x';
}

void executeNeonate()
{
    struct dirent *d;
    DIR *Di = opendir("/proc");
    if (Di == NULL)
    {
        printf("Error in opening proc directory\n");
        return;
    }
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp == NULL)
    {
        printf("Error in opening loadavg file\n");
        return;
    }
    char *line = (char *)malloc(sizeof(char) * 1000);
    while (fgets(line, 1000, fp) != NULL)
    {
        char *token = strtok(line, " ");
        char *last_token = (char *)malloc(sizeof(char) * 100);
        while (token != NULL)
        {
            strcpy(last_token, token);
            token = strtok(NULL, " ");
        }
        int curr_pid = atoi(last_token);

        printf("%d\n", curr_pid);
    }
}
struct termios orig_termios;

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enableRawMode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void callingProcess(int wait_time)
{
    enableRawMode();
    while (1)
    {
        if (is_key_pressed())
        {
            disableRawMode();
            break;
        }
        executeNeonate();
        if (is_key_pressed())
        {
            disableRawMode();
            break;
        }
        sleep(wait_time);
        if (is_key_pressed())
        {
            disableRawMode();
            break;
        }
    }
}