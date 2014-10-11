#include "ruby.h"
#include "ruby/encoding.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#define MSGSZ     128
typedef struct { long    mtype;
    char    mtext[MSGSZ];
} message_buf; 
static VALUE rb_cChannel;

static VALUE rb_send(VALUE self, VALUE to_send){
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
    VALUE json = rb_funcall(to_send, rb_intern("to_json"), 0);
    (void) strcpy(sbuf.mtext, rb_string_value_cstr(&json));
    buf_length = strlen(sbuf.mtext) + 1 ;
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
        printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
    return Qtrue;
}

static VALUE rb_receive(VALUE self){
    if(!rb_block_given_p())
        rb_raise(rb_eArgError, "a block is required");
    int msqid;
    key_t key;
    message_buf  rbuf;
    int msgflg = IPC_CREAT | 0666;
    key = getpid();
    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    }
    int num;
    num = msgrcv(msqid, &rbuf, MSGSZ, 1, 0);
    if (num < 0) {
        perror("msgrcv");
        exit(1);
    }
    VALUE json_class = rb_const_get(rb_cObject, rb_intern("JSON"));
    rb_yield(
        rb_funcall(
            json_class,
            rb_intern("parse"),
            1,
            rb_str_new2(rbuf.mtext)
        )
    );
 
    return Qnil;
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
    ruby_init();//sysinit(&argc, &argv);
    ruby_init_loadpath();
    rb_require("json");
    rb_require("enc/utf_16be");
    rb_require("enc/utf_16le");
    rb_require("enc/utf_32be");
    rb_require("enc/utf_32le");
    VALUE encoding_class = rb_const_get(rb_cObject, rb_intern("Encoding"));
    rb_define_class_under(encoding_class, "UTF_7", rb_cObject);
    rb_define_class_under(encoding_class, "UTF_8", rb_cObject);
    rb_cChannel = rb_define_module("Channel");
    rb_define_singleton_method(rb_cChannel, "receive", rb_receive, 0);
    rb_define_singleton_method(rb_cChannel, "send", rb_send, 1);
    ruby_script(argv[0]);
    int state = 0;
    char* script;
    sprintf(script, "require './%s'", argv[0]);
    rb_eval_string_protect(script, &state);
    if(state){
        dump_rb_error();
    }
    ruby_finalize();
}
