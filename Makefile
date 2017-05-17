LIBNAME=rf24-sunxi
LIBDIR=./lib/

all: server

server:
	g++ main.cpp -g -I./src/ -L${LIBDIR} -l${LIBNAME} -o server -Wno-write-strings

clean:
	rm -f server
