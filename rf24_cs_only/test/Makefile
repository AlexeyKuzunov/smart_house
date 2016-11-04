PREFIX=/usr/local/

LIB=librf24-sun7i
LIBNAME=$(LIB).so.1.0
OBJDIR=obj/
LIBDIR=../lib/

all:pingtest scanner arduino_rcv_irq

pingtest:
	g++ pingtest.cpp -g -I../src/ -L${LIBDIR} -lrf24-sun7i -o pingtest

scanner:
	g++ scanner.cpp -I../src/ -L${LIBDIR} -lrf24-sun7i -o scanner

arduino_rcv_irq:
	g++ arduino_rcv_irq.cpp -g -I../src/ -L${LIBDIR} -lrf24-sun7i -o arduino_rcv_irq


clean:
	rm -f scanner pingtest arduino_rcv_irq
