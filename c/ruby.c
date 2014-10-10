#include "ruby.h"
#include "ruby/encoding.h"
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
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    key = getpid() + 100000;
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    sbuf.mtype = 1;
    (void) strcpy(sbuf.mtext, "sending back");
    buf_length = strlen(sbuf.mtext) + 1 ;
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
    return Qfalse;
}

static VALUE rb_receive(VALUE self){
    int msqid;
    key_t key;
    message_buf  rbuf;
    int msgflg = IPC_CREAT | 0666;
    key = getpid();
    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    }
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("%s\n", rbuf.mtext);
    return Qfalse;
}

static void dump_rb_error(void)
{
    VALUE s, err;
    err = rb_gv_get("$!");
    s = rb_funcall(err, rb_intern("class"), 0);
    s = rb_funcall(s, rb_intern("name"), 0);
    printf("E: %s: ", rb_string_value_ptr(&s));
    s = rb_funcall(err, rb_intern("message"), 0);
    printf("%s\n", rb_string_value_ptr(&s));
    err = rb_funcall(err, rb_intern("backtrace"), 0);
    int i;
    for(i=0 ; i<8 ; ++i)
        if ((s = rb_ary_shift(err)) != Qnil)
            printf(" %s\n", rb_string_value_ptr(&s));
}

int main(int argc, char** argv){
    printf("starting\n");
    ruby_init();//(&argc, &argv);
    ruby_init_loadpath();
    rb_cDFHack = rb_define_module("Channel");
    rb_define_singleton_method(rb_cDFHack, "receive", rb_receive, 0);
    rb_define_singleton_method(rb_cDFHack, "send", rb_send, 0);
    int state = 0;
    rb_eval_string_protect("require './channels.rb'", &state);
    if(state){
        dump_rb_error();
    }
    printf("done\n");
    ruby_finalize();
    //rb_eval_string_protect("Channel.new", ret);
    //ruby_script("channels.rb");
}
