LIBNAME=rf24-sunxi
LIBDIR=./lib/

all: main

main:
	g++ main.cpp -g -I./src/ -L${LIBDIR} -l${LIBNAME} -o sm -Wno-write-strings

clean:
	rm -f sm
