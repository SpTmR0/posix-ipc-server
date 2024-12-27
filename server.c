#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_TEXT 250

struct message {
    long int mtype;
    char mtext[MAX_TEXT];
    long mtask;
};

int main() {
    int msgid;
    struct message msg;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    char buffer[1000];

    while(1) {
        memset(&msg, 0, sizeof(msg));
        if (msgrcv(msgid, (void *)&msg, sizeof(msg) - sizeof(long int), 0, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        printf("Message received from client %ld: %s %ld\n", msg.mtype, msg.mtext, msg.mtask);

        pid_t pid = fork();
        if (pid == 0) {
            char *filename = msg.mtext;
            // Child process
            if (msg.mtask == 1) {
                strcpy(msg.mtext, "hello");
            } else if(msg.mtask == 2) {
                struct stat buffer;
                if (stat(filename, &buffer) == 0) {
                    strcpy(msg.mtext, "File exists");
                } else {
                    strcpy(msg.mtext, "File not found");
                }
            } else if(msg.mtask == 3) {
                int count = 0;
                char ch;
                FILE *fp = fopen(filename, "r");
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
                sprintf(msg.mtext, "%d", count);

            } else if(msg.mtask == 4) {
                strcpy(msg.mtext, "terminate");
                if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long int), 0) == -1) {
                    perror("msgsnd failed in child");
                    exit(1);
                }
                printf("Server termination signal received. Exiting server process.\n");
                exit(0);
            }

            if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long int), 0) == -1) {
                perror("msgsnd failed in child");
                exit(1);
            }
            exit(0);
        }else if(pid == -1){
            perror("fork failed");
            exit(1);
        }
    }

    return 0;
}
