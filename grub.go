package grubby

// #cgo LDFLAGS: /home/towski/code/grubby/c/bridge.so -lm -lcrypt -ldl
// #include "c/bridge.h"
import "C"
import "fmt"
import "gopkg.in/mgo.v2/bson"

//export Register
func Register(pid int) {
}

func send(data []byte, pid int){
    C.send(C.CString(string(data)), C.int(pid))
}

func Start(script string) (*Grubby){
    grubby := Grubby{}
    grubby.Script = script
    grubby.Pid = (int)(C.start(C.CString("channels.rb")))
    return &grubby
}

type Grubby struct {
    Channel chan interface{}
    Pid int
    Script string
}

func (g *Grubby) Send(data interface{}){
    send(g.Marshal(data), g.Pid)
}

func (g *Grubby) Receive() interface{} {
    fmt.Println("waiting to receive")
    thing := C.receive(C.int(g.Pid))
    fmt.Println("done to receiving")
    //bson, err := bson.Unmarshal(thing)
    if false { //err != nil {
        //panic(err)
    }
    return thing
}


func (g *Grubby) Marshal(data interface{}) []byte{
    bson, err := bson.Marshal(data)
    if err != nil {
        panic(err)
    }
    return bson
}

