package main

import "sync"
import "fmt"
import "github.com/towski/grubby"

func main(){
    var wg sync.WaitGroup
    channel := grubby.Start("channels.rb")
    wg.Add(1)
    marp := make(map[string]string)
    marp["hey"] = "hey"
    go func(){
        channel.Send(marp)
        data := channel.Receive()
        _ = data
        fmt.Println("done receving")
        wg.Done()
    }()
    wg.Wait()
}
