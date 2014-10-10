package grubby

// #cgo LDFLAGS: /home/towski/code/grubby/c/bridge.so -lm -lcrypt -ldl -I/usr/local/include/libbson-1.0 -L/usr/local/lib -lbson-1.0
// #include "c/bridge.h"
import "C"
import "fmt"
//import "gopkg.in/mgo.v2/bson"
import "encoding/json"
//export Register
func Register(pid int) {
}

func send(data []byte, pid int){
    str := C.CString(string(data))
    fmt.Println("sending \n", str)
    C.send(str, C.int(pid))
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
    var data interface{}
    thing := C.receive(C.int(g.Pid))
    bytes := C.GoString(thing)
    json.Unmarshal([]byte(bytes), &data)
    fmt.Println("done to receiving %v", data)
    //bson, err := bson.Unmarshal(thing)
    if false { //err != nil {
        //panic(err)
    }
    return data
}


func (g *Grubby) Marshal(data interface{}) []byte{
    bson, err := json.Marshal(data)
    if err != nil {
        panic(err)
    }
    return bson
}

