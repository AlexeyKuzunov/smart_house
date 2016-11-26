#ifndef _GPIO_SUN7I_H_
#define _GPIO_SUN7I_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <sys/poll.h>
#include <stdlib.h>

#define SYSFS_GPIO_DIR  "/sys/class/gpio/"
//#define TIMEOUT -1

const int MAX_BUF = 64;
//const int int_gpio_num = 1;

void __msleep(int milisec);

class GPIO {
	int pin;
	int fd;
	const int timeout = -1;
public:
	// открываем порт. pin - номер порта gpio,  
	// edge - отслеживание состояния линии, active_low - уровень активного сигнала,
	// direction - направление (вход или выход) 
	GPIO(unsigned int gpio, const char *direction, 
		const char *edge = "none", const char *active_low = "0"); 
	~GPIO(void);
	
	// смотрим состояние порта gpio
	int read_gpio(void);

	//пишем в порт gpio
	int write_gpio(unsigned int val);
};

#endif
