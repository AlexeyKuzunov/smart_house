PREFIX=/usr/local/

LIB=librf24-sun7i
LIBNAME=$(LIB).so.1.0
OBJDIR=obj/
LIBDIR=../lib/

all:pingtest scanner

pingtest:
	g++ pingtest.cpp -g -I../src/ -L${LIBDIR} -lrf24-sun7i -o pingtest

scanner:
	g++ scanner.cpp -I../src/ -L${LIBDIR} -lrf24-sun7i -o scanner

clean:
	rm -f scanner pingtest
