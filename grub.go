package grubby

// #cgo LDFLAGS: /home/towski/code/grubby/c/bridge.so -lm -lcrypt -ldl -lz
// #include "c/bridge.h"
import "C"
//import "gopkg.in/mgo.v2/bson"
import "encoding/json"
//export Register
func Register(pid int) {
}

func send(data []byte, pid int){
    str := C.CString(string(data))
    C.send_grubby(str, C.int(pid))
}

func Start(script string) (*Grubby){
    grubby := Grubby{}
    grubby.Script = script
    grubby.Pid = (int)(C.start_grubby(C.CString(script)))
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

func (g *Grubby) Receive(data *interface{}) {
    thing := C.receive_grubby(C.int(g.Pid))
    bytes := C.GoString(thing)
    json.Unmarshal([]byte(bytes), data)
}


func (g *Grubby) Marshal(data interface{}) []byte{
    bson, err := json.Marshal(data)
    if err != nil {
        panic(err)
    }
    return bson
}

