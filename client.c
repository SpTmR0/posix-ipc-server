#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/wait.h>
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
    int clientId;
    int choice;
    char filename[MAX_TEXT];

    msgid = msgget((key_t)1234, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Enter Client-ID(not 9999): ");
    scanf("%d", &clientId);

    if(clientId == 9999 || clientId < 0) {
        printf("Invalid Client-ID. Exiting...\n");
        exit(1);
    }

    while(1){
        printf("\nMenu:\n");
        printf("1. Contact the Ping Server\n");
        printf("2. Contact the File Search Server\n");
        printf("3. Contact the File Word Count Server\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        msg.mtype = clientId;
        switch(choice) {
            case 1:
                msg.mtask = 1;
                strcpy(msg.mtext, "hi");
                break;
            case 2:
                printf("Enter filename: ");
                scanf("%s", filename);
                msg.mtask = 2;
                strcpy(msg.mtext, filename);
                printf("msg.mtext: %s\n", msg.mtext);
                break;
            case 3:
                printf("Enter filename: ");
                scanf("%s", filename);
                msg.mtask = 3;
                strcpy(msg.mtext, filename);
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                continue;
        }

        if (msgsnd(msgid, (void *)&msg, sizeof(msg) - sizeof(long int), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        // Receive response from server
        if (msgrcv(msgid, (void *)&msg, sizeof(msg) - sizeof(long int), clientId, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        printf("Server Response: %s\n", msg.mtext);

        if(strcmp(msg.mtext, "terminate") == 0) {
            printf("(cleanup.out): Server termination signal received. Exiting client process.\n");
            exit(0);
        }
    }

    return 0;
}
