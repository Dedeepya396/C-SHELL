#include "shrc.h"
int alias_top = 0;
alias aliases[buf_size];

int func_top = 0;
functions function[buf_size];

void addAliases()
{
    int func_detected = 0;
    FILE *fp = fopen("myshrc", "r");
    char *line = (char *)malloc(sizeof(char) * buf_size);
    while (fgets(line, buf_size, fp) != NULL)
    {
        if (strncmp(line, "alias", 5) == 0)
        {
            int eq_flag = 0;
            char *token = (char *)malloc(sizeof(char) * buf_size);
            token = strtok(line, " \t");
            char *command = (char *)malloc(sizeof(char) * buf_size);
            strcpy(command, "");
            while (token != NULL)
            {
                if (strcmp(token, "alias") == 0)
                    ;
                else if (strcmp(token, "=") == 0)
                {
                    eq_flag = 1;
                }
                else if (eq_flag == 1)
                {
                    strcat(command, token);
                    strcat(command, " ");
                }
                else if (eq_flag == 0)
                {
                    strcpy(aliases[alias_top].alias, token);
                }
                token = strtok(NULL, " \t");
            }
            strcpy(aliases[alias_top].actual_func, command);
            alias_top++;
            strcpy(command, "");
        }
        line[strcspn(line, "\n")] = '\0';
        if (strncmp(line, "func", 4) == 0)
        {
            char *token = (char *)malloc(sizeof(char) * buf_size);
            token = strtok(line, " \t");
            while (token != NULL)
            {
                if (strcmp(token, "func") == 0)
                    ;
                else
                {
                    if (strstr(token, "()") != NULL)
                    {
                        int len = strlen(token) - 2;
                        strncpy(function[func_top].func, token, len);
                        // printf("%s\n",function[func_top].func);
                    }
                }
                token = strtok(NULL, " \t");
            }
        }
        // printf("%d-%s---\n", strcmp(line, "{"), line);
        if (strcmp(line, "{") == 0)
        {
            func_detected = 1;
        }
        else if (func_detected == 1)
        {
            char *token = (char *)malloc(sizeof(char) * buf_size);
            token = strtok(line, " \t");
            int i = 0;
            while (token != NULL)
            {
                if (strcmp(token, "}") == 0)
                {
                    func_detected = 0;
                    // func_top++;
                    break;
                }
                strcat(function[func_top].complete_func, token);
                strcat(function[func_top].complete_func, " ");
                token = strtok(NULL, " \t");
            }
        }
        if (strcmp(line, "}") == 0)
        {
            func_detected = 0;
            func_top++;
        }
    }
}
void printAliases()
{
    for (int i = 0; i < alias_top; i++)
    {
        printf("Alias: %s, actual: %s\n", aliases[i].alias, aliases[i].actual_func);
    }
}
void printProcess()
{
    for (int i = 0; i < func_top; i++)
    {
        printf("Func Name: %s, complete %s\n", function[i].func, function[i].complete_func);
    }
}