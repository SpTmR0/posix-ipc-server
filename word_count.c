#include <stdio.h>

int main(int argc, char *argv[]) {
    int count = 0;
    char ch;
    printf("argv[1]: %s\n", argv[1]);
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("File not found\n");
        return 0;
    }

    while((ch = fgetc(fp)) != EOF) {
        if (ch == ' ' || ch == '\n') {
            count++;
        }
    }
    fclose(fp);

    printf("Word count: %d\n", count);
    return 0;
}