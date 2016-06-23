LIBNAME=rf24-sunxi
LIBDIR=./lib/

all: swith

main:
	g++ swith.cpp -g -I./src/ -L${LIBDIR} -l${LIBNAME} -o sm -Wno-write-strings

clean:
	rm -f swith
