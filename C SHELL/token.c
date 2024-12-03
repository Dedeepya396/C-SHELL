#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void dividingCommands(char *input)
{
    input[strcspn(input, "\n")] = 0;
    char *saveptr;
    char *token = strtok_r(input, " \t", &saveptr);
    char command[100] = "";
    char before_and[100] = "";
    while (token != NULL)
    {
        if (strcmp(token, "&") == 0 || token[strlen(token) - 1] == '&')
        {
            printf("%s\n",token);
            printf("bsdfbfuysd\n");
            strcpy(command, "");
        }
        else if (strcmp(token, ";") == 0 || token[strlen(token) - 1] == ';')
        {
            printf("%s\n",token);
            printf("bffysgfsd\n");
            strcpy(command, "");
        }
        else
        {
            strcat(command, token);
            strcat(command, " ");
        }

        token = strtok_r(NULL, " \t", &saveptr);
    }
}
int main(){
    int size = 100;
    char a[size];
    fgets(a,100,stdin);
    dividingCommands(a);
}