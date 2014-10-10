#include "bridge.h"
//#include "_obj/_cgo_export.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#define MSGSZ     128

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
} message_buf;

char* message;
char* receive(int pid){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf rbuf;
    size_t buf_length;
    key = pid + 100000;
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("got back %s\n", rbuf.mtext);
    return rbuf.mtext;
}

void send(char* value, int pid){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    key = pid;
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    sbuf.mtype = 1;
    (void) strcpy(sbuf.mtext, value);
    buf_length = strlen(sbuf.mtext) + 1 ;
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
        printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
}

int start(char* value)
{
    int child_pid = fork();
    if(child_pid == 0){
        execl("./ruby", NULL);
        return;
    } else {
        Register(child_pid);
        return child_pid;
    }
}
