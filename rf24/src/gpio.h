#ifndef _GPIO_H_
#define _GPIO_H_


const char SYSFS_GPIO_DIR "/sys/class/gpio";
const int MAX_BUF 64;
const int HIGH    1;
const int LOW     0;

class GPIO {
public:
	// открываем порт. pin - номер порта gpio, gpio - название порта на плате, 
	// edge - отслеживание состояния линии, active_low - уровень активного сигнала,
	// direction - направление (вход или выход) 
	GPIO(unsigned int pin, char *gpio, char *edge, char* active_low, char *direction); 
	~GPIO(void);
	
	// смотрим состояние порта gpio
	int GPIO::read(void);

	//пишем в порт gpio
	int GPIO::write(unsigned int val);
private:
	int fd;
};

#endif
