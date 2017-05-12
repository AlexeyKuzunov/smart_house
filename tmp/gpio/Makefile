
OBJDIR=obj/

all:gpio_exec

gpio_exec: ${OBJDIR}gpio_sun7i.o ${OBJDIR}test_gpio.o
	g++ ${CFLAGS} -o gpio_exec -std=c++11 gpio_sun7i.o test_gpio.o

${OBJDIR}test_gpio.o: test_gpio.cpp src/gpio_sun7i.h
	g++ ${CFLAGS} -c -I./src/ -std=c++11 test_gpio.cpp

${OBJDIR}gpio_sun7i.o: src/gpio_sun7i.cpp src/gpio_sun7i.h
	g++ ${CFLAGS} -c -I./src/ -std=c++11 src/gpio_sun7i.cpp



clean:
	rm -f *.o src/*.o gpio_exec
