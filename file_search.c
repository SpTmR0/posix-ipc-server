#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    struct stat buffer;
    printf("argv[1]: %s\n", argv[1]);
    if (stat(argv[1], &buffer) == 0) {
        printf("File exists\n");
    } else {
        printf("File not found\n");
    }
    return 0;
}