LIBNAME=rf24-sunxi
LIBDIR=./lib/

all: arduino_rcv_irq

arduino_rcv_irq:
	g++ arduino_rcv_irq.cpp -g -I./src/ -L${LIBDIR} -l${LIBNAME} -o arduino_rcv_irq -Wno-write-strings

clean:
	rm -f arduino_rcv_irq
