package main

// #cgo LDFLAGS: /home/towski/code/grubby/bridge.so -lm -lcrypt -ldl
// #include "bridge.h"
import "C"
import "sync"

//export Register
func Register(pid int) {
    chann1 = make(chan int)
}

//export Receive
func Receive(value int) {
    // find channel, post value
    chann1 <- value
}

func receive(chann chan int){
    value := chann1
    _ = value
}

func Send(pid int){
    chann1 <- 1
}

func send(chann chan int, pid int){
    C.send(C.int(<-chann), C.int(pid))
}

var chann1 chan int

func main(){
    var wg sync.WaitGroup
    pid1 := (int)(C.start(C.CString("channels.rb")))
    _ = pid1
    wg.Add(1)
    go send(chann1, pid1)
    Send(pid1)
    wg.Wait()
}
