.PHONY: clean

TARGET=grubby

$(TARGET): bridge.so ruby main

main: main.go
	go build -o z main.go

ruby: ruby.c
	gcc -I /home/towski/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/ -I /home/towski/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/ruby -I /home/towski/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/x86_64-linux/ -o $@ $^ -Wl,libruby.so.2.0.0,-lm,-lcrypt,-ldl

bridge.so: bridge.c
	gcc -shared -o $@ $^ -c

clean:
	rm -f *.o *.so ruby main

gotool: main.go
	go tool cgo main.go
