
#include "gpio.h"


GPIO::GPIO(unsigned int pin, char *gpio, char *edge, char* active_low, char *direction){
	int fd, len;
	char buf[MAX_BUF];
 	
 	//открываем интерфейс GPIOLIB export и пишем в него номер GPIO, который хотим экспортировать
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 	len = snprintf(buf, sizeof(buf), "%d", pin);
	write(fd, buf, len);
	close(fd);

	// генерируем прерывание по всяким различным случаям: изменение уровня, 
	// установка уровня в высокое или низкое состояние для этого в
	// файл edge пишем одно из следующих значений: none, rising, falling или both. 
	// Здесь: none – выключаем отслеживание изменения состояния входящей линии; 
	// rising и falling – отслеживаем переход из неактивного состояния в активное и 
	// из активного в неактивное соответственно; both – реагируем на любое изменение состояния.

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/edge", gpio);
 	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}
 	write(fd, edge, strlen(edge) + 1); 
	close(fd);

	// определяем уровень активного сигнала, какое напряжение будет соответствовать
	// логическому нулю, а какое – логической единице. по-умолчанию, получается 
	// ASCII символ ‘1’, если на выводе присутствует напряжение, иначе — получаем ‘0’. 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/active_low", gpio);
 	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-active_low");
		return fd;
	}	
	write(fd, active_low, strlen(active_low) + 1); 
	close(fd);

	//конфигурируем направление GPIO (in, out), используя интерфейс GPIOLIB direction 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/direction", gpio);
 	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-direction");
		return fd;
	}
	write(fd, direction, strlen(direction) + 1); 
	close(fd);

	//открываем для чтения если настроено на вход, или изменения если настроено на выход значение на линии GPIO
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/value", gpio);
 	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
}

GPIO::~GPIO(void){
	close(fd);
}