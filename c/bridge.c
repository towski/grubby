#include "bridge.h"
//#include "_obj/_cgo_export.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <zlib.h>
#include <stdio.h>
#include <string.h>
#define MSGSZ     65536

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
} message_buf;

char* message;
char* receive_grubby(int pid){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf rbuf;
    size_t buf_length;
    key = pid + 100000;
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
    }
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgsnd");
    }
    return rbuf.mtext;
}

void send_grubby(char* value, int pid){
    struct msqid_ds qbuf;
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    key = pid;
    char compressed[1000];
    uLong ucompSize = strlen(value)+1; 
    uLong compSize = compressBound(ucompSize);
    compress((Bytef*)compressed, &compSize, (Bytef*)value, ucompSize);
    if((msqid = msgget(key, msgflg )) < 0){
        perror("msgget");
    }
    if(msgctl(msqid, IPC_STAT, &qbuf) == -1){
        printf("unable to stat");
    } else {
        printf("queue stats: current bytes:%d, current messages:%d, max bytes:%d\n", qbuf.__msg_cbytes, qbuf.msg_qnum, qbuf.msg_qbytes);
    }
    sbuf.mtype = 1;
    compressed[compSize + 0] = (ucompSize >> 24) & 0xFF;
    compressed[compSize + 1] = (ucompSize >> 16) & 0xFF;
    compressed[compSize + 2] = (ucompSize >> 8) & 0xFF;
    compressed[compSize + 3] = ucompSize & 0xFF;
    (void) memcpy(sbuf.mtext, compressed, compSize + sizeof(int));
    buf_length = compSize + 4;
    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        printf ("%d, %d, %d\n", msqid, sbuf.mtype, buf_length);
        perror("msgsnd");
    }
}

int start_grubby(char* value)
{
    int child_pid = fork();
    if(child_pid == 0){
        execl("/home/towski/code/grubby/ruby", value, NULL);
        return 0;
    } else {
        //Register(child_pid);
        return child_pid;
    }
}
