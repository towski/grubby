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
  go func(){
    _ = channel.Receive()
  }
  channel.Send("Hello")
}

```

And badcop.rb looks like this:

```ruby
Channel.receive do |hello|
  puts hello
  Channel.send "world"
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

A single grubby can open multiple channels as well, provided you give them names.
```ruby
grub_lord = Channel.new("grub lord")
grub_shaman = Channel.new("grub shaman")

grub_shaman.receive do |orders|
  ...
end

grub_lord.send(:food_order => [1,2,3])
```

and reference them in go

```go
  channel := grubby.GetChannel("grub lord")
  channel.Receive()
``

If you wanna get things done, you gotta get grubby (TM).

Don't let your ruby files get too big, and let golang do all the paperwork for you.

But don't try any of this yet, because it's not implemented

