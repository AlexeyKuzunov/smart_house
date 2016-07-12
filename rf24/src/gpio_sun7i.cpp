
#include "gpio_sun7i.h"


GPIO::GPIO(unsigned int pin, const char* const gpio, const char* const edge, const char* const active_low,
			 const char* const direction){
	int localfd, len;
	char buf[MAX_BUF];
 	
 	//открываем интерфейс GPIOLIB export и пишем в него номер GPIO, который хотим экспортировать
	localfd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (localfd < 0) {
		perror("gpio/export");
	}
 	len = snprintf(buf, sizeof(buf), "%d", pin);
	write(localfd, buf, len);
	close(localfd);

	// генерируем прерывание по всяким различным случаям: изменение уровня, 
	// установка уровня в высокое или низкое состояние для этого в
	// файл edge пишем одно из следующих значений: none, rising, falling или both. 
	// Здесь: none – выключаем отслеживание изменения состояния входящей линии; 
	// rising и falling – отслеживаем переход из неактивного состояния в активное и 
	// из активного в неактивное соответственно; both – реагируем на любое изменение состояния.

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/edge", gpio);
 	localfd = open(buf, O_WRONLY);
	if (localfd < 0) {
		perror("gpio/set-edge");
	}
 	write(localfd, edge, strlen(edge) + 1); 
	close(localfd);

	// определяем уровень активного сигнала, какое напряжение будет соответствовать
	// логическому нулю, а какое – логической единице. по-умолчанию, получается 
	// ASCII символ ‘1’, если на выводе присутствует напряжение, иначе — получаем ‘0’.
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/active_low", gpio);
 	localfd = open(buf, O_WRONLY);
	if (localfd < 0) {
		perror("gpio/set-active_low");
	}
	write(localfd, active_low, strlen(active_low) + 1); 
	close(localfd);

	//конфигурируем направление GPIO (in, out), используя интерфейс GPIOLIB direction 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/direction", gpio);
 	localfd = open(buf, O_WRONLY);
	if (localfd < 0) {
		perror("gpio/set-direction");
	}
	write(localfd, direction, strlen(direction) + 1); 
	close(localfd);

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

int GPIO::write_gpio(unsigned int val){
	int len;
	char buf[MAX_BUF];
	int err;
	len = snprintf(buf, sizeof(buf), "%d", val);
	err = write(fd, buf, len);
	return err;
}

int GPIO::read_gpio(void){
	char c;
	int err;

	lseek(fd, 0, SEEK_SET);
	err = read(fd, &c, sizeof(c));
	if (err != 1) return -1;
	return c - '0';
}
