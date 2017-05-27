
#include "gpio.h"


GPIO::GPIO(unsigned int gpio, const char *direction, const char *edge, const char *active_low){
	int localfd, len;
	char buf[MAX_BUF];
	this->pin = gpio;
// 	DIR* dir;
 	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio%d", gpio);
// 	dir = opendir(buf);
	char c;
// 	if (dir == NULL){
 		//открываем интерфейс GPIOLIB export и пишем в него номер GPIO, который хотим экспортировать
		localfd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
		if (localfd < 0) {
			perror("gpio/export");
			}
 		len = snprintf(buf, sizeof(buf), "%d", gpio);
		write(localfd, buf, len);
		close(localfd);
		// генерируем прерывание по всяким различным случаям: изменение уровня, 
		// установка уровня в высокое или низкое состояние для этого в
		// файл edge пишем одно из следующих значений: none, rising, falling или both. 
		// Здесь: none – выключаем отслеживание изменения состояния входящей линии; 
		// rising и falling – отслеживаем переход из неактивного состояния в активное и 
		// из активного в неактивное соответственно; both – реагируем на любое изменение состояния.

		len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio%d/edge", gpio);
 		if (len > 0){
 			localfd = open(buf, O_WRONLY);
			if (localfd > 0) {
				write(localfd, edge, strlen(edge) + 1); 
				close(localfd);
			}
 		}

		// определяем уровень активного сигнала, какое напряжение будет соответствовать
		// логическому нулю, а какое – логической единице. по-умолчанию, получается 
		// ASCII символ ‘1’, если на выводе присутствует напряжение, иначе — получаем ‘0’.
		len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio%d/active_low", gpio);
 		localfd = open(buf, O_WRONLY);
		if (localfd < 0) {
			//perror("gpio/set-active_low");
			perror(buf);
			}
		write(localfd, active_low, strlen(active_low) + 1); 
		close(localfd);

		//конфигурируем направление GPIO (in, out), используя интерфейс GPIOLIB direction 
		len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio%d/direction", gpio);
 		localfd = open(buf, O_RDWR);
		if (localfd < 0) {
			perror("gpio/set-direction");
			}
		write(localfd, direction, strlen(direction) + 1); 
		close(localfd);
 	//	}

 	//открываем для изменения если настроено на выход значение на линии GPIO
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio%d/value", gpio);
 	GPIO::fd = open(buf,  O_RDWR);
	if (GPIO::fd < 0) {
		perror("gpio/set-value");
		}
	//считываем значение
	read(GPIO::fd, &c, sizeof(c));
//	printf("%s", c);
}

GPIO::~GPIO(void){
	printf("start distructor");
	close(fd);
}

int GPIO::write_gpio(unsigned int val){
	int len;
	char buf[MAX_BUF];
	int err;
//	int fd;
	
	len = snprintf(buf, sizeof(buf), "%d", val);
	err = write(GPIO::fd, buf, len);
	fflush(stdout);
//	close(fd);
	
	return err;
}

int GPIO::read_gpio(){
	char c;
	int err; //, len;
//	char buf[MAX_BUF];
	struct pollfd pollfd[1];
/*	int fd;
	//открываем для изменения если настроено на выход значение на линии GPIO
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio" "%d" "/value", pin);
 	fd = open(buf,  O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		perror("gpio/set-value");
	}
*/
//	memset((void*)pollfd, 0, sizeof(pollfd));
	pollfd[0].fd = GPIO::fd;
	pollfd[0].events = POLLPRI | POLLERR;
	pollfd[0].revents = 0;
	
	err =  poll(pollfd, 1, timeout);
	
	if(err != 1) {
//		close(fd);
		return -1;	
		}
	//if (pollfd[0].revents & POLLPRI) {
		lseek(GPIO::fd, 0, SEEK_SET);
		err = read(GPIO::fd, &c, sizeof(c));
		if (err != 1) {
//		    close(fd);
		    return -1;
		    }
//		close(fd);
//		fflush(stdout);
		return atoi(&c);
	//}
}
