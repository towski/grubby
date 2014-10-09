#include "ruby.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#define MSGSZ     128

typedef struct {
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;

static VALUE rb_cDFHack;

static VALUE rb_active(VALUE self){
    //fork();
}

static VALUE rb_send(VALUE self){
    return Qfalse;
}

static VALUE rb_receive(VALUE self){
    int msqid;
    key_t key;
    message_buf  rbuf;
    int msgflg = IPC_CREAT | 0666;
    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = getpid();

    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    }
    
    /*
     * Receive an answer of message type 1.
     */
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    /*
     * Print the answer.
     */
    printf("%s\n", rbuf.mtext);
    exit(0);
    return Qfalse;
}

int main(int argc, char** argv){
    printf("starting\n");
    ruby_init();//(&argc, &argv);
    ruby_init_loadpath();
    rb_cDFHack = rb_define_module("Channel");
    rb_define_singleton_method(rb_cDFHack, "receive", rb_receive, 0);
    rb_define_singleton_method(rb_cDFHack, "send", rb_send, 0);
    int *ret;
    rb_eval_string_protect("require './channels.rb'", ret);
    //rb_eval_string_protect("Channel.new", ret);
    //ruby_script("channels.rb");
}
