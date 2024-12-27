#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_TEXT 250

struct message {
    long int mtype;
    char mtext[MAX_TEXT];
    long mtask;
};

int main() {
    int msgid;
    struct message msg;
    char choice;

    msgid = msgget((key_t)1234, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    do {
        printf("Do you want the server to terminate? Press Y for Yes and N for No.\n");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            msg.mtype = 9999;
            msg.mtask = 4;
            strcpy(msg.mtext, "terminate");
            if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                    perror("msgsnd failed in child");
                    exit(1);
            }
            printf("Server termination signal sent. Exiting cleanup process.\n");
            exit(0);
        }
    } while (choice == 'N' || choice == 'n');

    return 0;
}
