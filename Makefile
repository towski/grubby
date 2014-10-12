GO_BIN ?= go
.PHONY: clean

TARGET=grubby

$(TARGET): c/bridge.so library other

other: ruby main 

main: cmd/main.go
	$(GO_BIN) build -o z cmd/main.go

ruby: c/ruby.c
	gcc -I /home/towski/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/ -I /home/towski/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/ruby -I /home/towski/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/x86_64-linux/ -o $@ $^ -Wl,libruby.so.2.0.0,-lm,-lcrypt,-ldl,-lz 
	#$(shell pkg-config --cflags --libs libbson-1.0)

c/bridge.so: c/bridge.c
	gcc $(ARCHFLAG) -o $@ $^ -shared -c

library: grub.go
	$(GO_BIN) install github.com/towski/grubby

clean:
	rm -f *.o *.so c/ruby main c/*so c/*o /home/towski/gopath/pkg/linux_amd64/github.com/towski/grubby.a

gotool: main.go
	go tool cgo main.go
