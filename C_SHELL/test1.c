#include <stdio.h>
#include <string.h>

int main() {
    const char *str1 = "/home/dedeepya/Desktop/3rdsem/OSN/MP_1/PART_B/hello/hello/hello";
    const char *str2 = "/home/dedeepya/Desktop/3rd sem/OSN/MP_1/PART_B";

    // Use strncmp to compare the first part of str1 with str2

    char a[100];
    strncpy(a,str1,strlen(str2)-1);    
    int result = strcmp(a,str2);
    printf("%s---%s---\n",a,str2);
    // Check if str2 is a prefix of str1
    if (result == 0) {
        printf("The second string is a prefix of the first one.\n");
    } else {
        printf("The second string is NOT a prefix of the first one.\n");
    }

    return 0;
}
