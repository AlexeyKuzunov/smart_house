#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>

#include "gpio_sun7i.h"
#include <cstdlib>
#include "RF24.h"
#include <poll.h>
using namespace std;

int Daemon(void);
void gotData(void);
char* getTime();
int writeLog(char msg[256]);
char* getCommand(char command[128]);

const uint64_t pipes[2] = {0xF0F0F0F0E1LL,0xF0F0F0F0E2LL};
const int int_gpio_num = 3;
#define GPIO_STR "3"

//CE - PB13
//CSN - PI10
RF24 radio(SUNXI_GPB(13), SUNXI_GPI(10), "/dev/spidev0.0");

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

int gpio_fd;
uint8_t intResult;
char receive_payload[32+1];

int gpio_export(unsigned int gpio) { //добавление порта gpio
    int fd, len;
    char buf[MAX_BUF];
    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
	perror("gpio/export");
	return fd;
    }
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;    
}

int gpio_set_edge(char *gpio, char *edge, char* active_low) {
//установка отслеживания и состояния порта gpio
    int fd, len;
    char buf[MAX_BUF];
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/edge", gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
	perror("gpio/set-edge");
	return fd;
    }
    write(fd, edge, strlen(edge) + 1);
    close(fd);

    //set active low
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/active_low", gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
	perror("gpio/active_low");
	return fd;
    }
    write(fd, active_low, strlen(active_low) + 1);
    close(fd);
    return 0;
}

int gpio_fd_open(char *gpio) {
    int fd, len;
    char buf[MAX_BUF];
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/value", gpio);
    fd = open(buf, O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
	perror("gpio/fd_open");	
    }
    return fd;
}

int gpio_fd_close(int fd) {
    return close(fd);
}

char* getTime() { //функция возвращает форматированную дату и время
    time_t now;
    struct tm *ptr;
    static char tbuf[64];
    bzero(tbuf, 64);
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf, 64, "%Y-%m-%e %H:%M:%S", ptr);
    return tbuf;
}

char* getCommand(char command[128]) { //функция возвращает результат выполнения linux комманды
    FILE *pCom;
    static char comText[256];
    bzero(comText, 256);
    char buf[64];
    pCom = popen(command, "r"); //выполняем
    if(pCom == NULL) {
	writeLog("Error Command");
	return "";
    }
    strcpy(comText, "");
    while(fgets(buf, 64, pCom) != NULL) { //читаем результат
	strcat(comText, buf);
    }
    pclose(pCom);
    return comText;
}

int writeLog(char msg[256]) { //функция записи в лог
    FILE *pLog;
    pLog = fopen("/home/alex/smart_house/rf24/daemon.log", "a");
    if (pLog == NULL) {
	return 1;
    }
    char str[312];
    bzero(str, 312);
    strcpy(str, getTime());
    strcat(str, "============================\n");
    strcat(str, msg);
    strcat(str, "\n");
    fputs(str, pLog);
    fclose(pLog);
    return 0;
}


void setup(void) {
    gpio_export(int_gpio_num);
    gpio_set_edge(GPIO_STR, "rising", "1");
    
    radio.begin();
    //enable dynamic payloads
    radio.enableAckPayload();
    radio.enableDynamicPayloads();
    radio.setAutoAck(1);
    radio.setRetries(15, 15);
    radio.setDataRate(RF24_2MBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.setChannel(50);
    radio.setCRCLength(RF24_CRC_16);
    radio.openReadingPipe(0, pipes[0]); //
    radio.openReadingPipe(1, pipes[1]);
    
    radio.startListening(); //стартуем чтение
    //radio.printDetails(); //показываем регистры передатчика
}

int main(int argc, char* argv[]) {
    writeLog("Daemon Start");
    pid_t parpid, sid;
    parpid = fork(); //создаем дочерний процесс
    if(parpid < 0) {
	exit(1);
    } else if(parpid != 0) {
	exit(0);
    }
    umask(0); //даем права на работу с фс
    sid = setsid(); //генерируем уникальный индекс процесса
    if(sid < 0) {
	exit(1);
    }
    if((chdir("/")) < 0) { //выходим в корень фс
	exit(1);
    }
    close(STDIN_FILENO); //закрываем доступ к стандартным потокам ввода-вывода
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return Daemon();
}

int Daemon(void) { //бесконечный цикл демона
//    char *log;
    setup();
    gpio_fd = gpio_fd_open(GPIO_STR);
    struct pollfd fdset[1];
    int nfds = 1;
    int rc, timeout, len;
    char *buf[MAX_BUF];
    timeout = -1;

    while(1) {
	memset((void*)fdset, 0, sizeof(fdset));
	fdset[0].fd = gpio_fd;
	fdset[0].events = POLLPRI;
	rc = poll(fdset, nfds, timeout);
	if(rc < 0) {
	    writeLog("\npoll() failed!\n");	    
	}
	if(rc == 0) {
	    
	}

	if(fdset[0].revents & POLLPRI) {
	    len = read(fdset[0].fd, buf, MAX_BUF);
	    gotData();
	}
//	log = getCommand("who");
//	if(strlen(log) > 5) {
//	    writeLog(log);
//	}
//	sleep(600); //ждем 10 минут до следующей итерации
    }
    return 0;
}

void gotData(void) {
    bool rx = 0, blnTXOK = 0, blnTXFail = 0;
    intResult = 0;
    uint8_t pipe_num = 0;
    radio.whatHappened(blnTXOK, blnTXFail, rx, &pipe_num); //читаем регистр
    if(blnTXFail) { //если данные не отправлены
	intResult = 2;
    }else if(blnTXOK) { //если успешно отправили данные
	intResult = 1;
    }else if(rx) { //если приbняли данные для чтения
	intResult = 3;
	uint8_t len = radio.getDynamicPayloadSize();
	
	/* Пытаемся открыть папку /tmp/pipes[pipe_num]. Если ее нет, создаем.
    	   В папке создаем файлы import - для принятых данных и export для ввода команд 
    	   Помещаем в файл Import принятые данные */
	

	//Пишем принятые данные в файл /tmp/pipes[pipe_num]/import

	bool more_available = true;
	while(more_available){
	    more_available = radio.read(receive_payload, len);
	    receive_payload[len] = 0;
	    
	    //обрабатываем принятые данные
	    
	    if (more_available) {
		len = radio.getDynamicPayloadSize();
	    }
	} 

    }
    
    fflush(stdout);
}


