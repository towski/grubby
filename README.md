grubby
======

Golang + Ruby = police department


intro
======
Good cop, bad cop. Go is careful and precise, and is very resourceful. Ruby can do anything and 
everything, but it doesn't like keeping track of memory or filing paperwork. 
Put them together and you get grubby.

A grubby program starts in Go. 

goodcop.go

```go
import 'github.com/towski/grubby'
func main(){
  channel := grubby.Start("badcop.rb")
  to_send := make(map[string]string)
  to_send["hey"] = "hey"
  go func(){
    channel.Send(to_send)
    var data interface{}
    channel.Receive(&data)
    fmt.Println("done to receiving %v", data)
  }
}

```

And badcop.rb looks like this:

```ruby
Channel.receive do |hello|
  puts hello
  Channel.send(:something => "to_json")
end
```

So when you execute ./goodcop, you get a ruby process ready to receive data from go, which can also send data back.

You can start any number of grubby scripts:
```go

func main(){
  chan1 := grubby.Start("ruby/active_record.rb")
  chan2 := grubby.Start("ruby/puppet_updater.rb")
  chan3 := grubby.Start("ruby/fish_eater.rb")
}

```

and route data to them through go.

If you wanna get things done, you gotta get grubby (TM).

Don't let your ruby files get too big, and let golang do all the paperwork for you.

But don't try any of this yet, because it's not implemented

