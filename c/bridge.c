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

char* receive(int pid){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    key = pid + 100000;
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    if (msgrcv(msqid, &sbuf, MSGSZ, 1, 0) < 0) {
       perror("msgsnd");
       exit(1);
    }
    return sbuf.mtext;
}

void send(char* value, int pid){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = pid;
(void) fprintf(stderr, "\nmsgget: Calling msgget(%#lx, %#o)\n", key, msgflg);
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    else 
     (void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
    /*
     * We'll send message type 1
     */
    sbuf.mtype = 1;
    (void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
    
    (void) strcpy(sbuf.mtext, "Did you get this?");
    (void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
    buf_length = strlen(sbuf.mtext) + 1 ;
    /*
     * Send a message.
     */
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
   else 
      printf("Message: \"%s\" Sent\n", sbuf.mtext);
}

int start(char* value)
{
    int child_pid = fork();
    if(child_pid == 0){
        printf("hey\n");
        execl("./ruby", NULL);
        printf("no child\n");
        return;
    } else {
        Register(child_pid);
        return child_pid;
    }
}
