#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char buf[256];
    char *a;

    FILE *test;
    if((test = fopen("file.txt", "r")) == NULL) {
        perror("error opening file");
        return -1;
    }

    for(int i = 0; i < 3 ; i++){
        fgets(buf, 256, test);

        a = strtok(buf, "\n");
        puts(a);
    }

    fclose(test);
    return 0;
}