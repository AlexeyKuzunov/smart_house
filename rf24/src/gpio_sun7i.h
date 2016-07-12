#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define HIGH	1
#define LOW	0

#define SUNXI_GPIO_A	0
#define SUNXI_GPIO_B	1
#define SUNXI_GPIO_C	2
#define SUNXI_GPIO_D	3
#define SUNXI_GPIO_E	4
#define SUNXI_GPIO_F	5
#define SUNXI_GPIO_G	6
#define SUNXI_GPIO_H	7
#define SUNXI_GPIO_I	8

/* GPIO bank sizes */
#define SUNXI_GPIO_A_NR		(32)
#define SUNXI_GPIO_B_NR		(32)
#define SUNXI_GPIO_C_NR		(32)
#define SUNXI_GPIO_D_NR		(32)
#define SUNXI_GPIO_E_NR		(32)
#define SUNXI_GPIO_F_NR		(32)
#define SUNXI_GPIO_G_NR		(32)
#define SUNXI_GPIO_H_NR		(32)
#define SUNXI_GPIO_I_NR		(32)

#define SUNXI_GPIO_NEXT(__gpio) ((__gpio##_START)+(__gpio##_NR)+0)

enum sunxi_gpio_number {
    SUNXI_GPIO_A_START = 0,
    SUNXI_GPIO_B_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_A),	//32
    SUNXI_GPIO_C_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_B),	//64
    SUNXI_GPIO_D_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_C),	//96
    SUNXI_GPIO_E_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_D),	//128
    SUNXI_GPIO_F_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_E),	//160
    SUNXI_GPIO_G_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_F),	//192
    SUNXI_GPIO_H_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_G),	//224
    SUNXI_GPIO_I_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_H)	//256
};

/* SUNXI GPIO number definitions */
#define SUNXI_GPA(_nr) (SUNXI_GPIO_A_START + (_nr))
#define SUNXI_GPB(_nr) (SUNXI_GPIO_B_START + (_nr))
#define SUNXI_GPC(_nr) (SUNXI_GPIO_C_START + (_nr))
#define SUNXI_GPD(_nr) (SUNXI_GPIO_D_START + (_nr))
#define SUNXI_GPE(_nr) (SUNXI_GPIO_E_START + (_nr))
#define SUNXI_GPF(_nr) (SUNXI_GPIO_F_START + (_nr))
#define SUNXI_GPG(_nr) (SUNXI_GPIO_G_START + (_nr))
#define SUNXI_GPH(_nr) (SUNXI_GPIO_H_START + (_nr))
#define SUNXI_GPI(_nr) (SUNXI_GPIO_I_START + (_nr))

#define SYSFS_GPIO_DIR  "/sys/class/gpio"

const int MAX_BUF = 64;
//const int int_gpio_num = 1;


class GPIO {
public:
	// открываем порт. pin - номер порта gpio, gpio - название порта на плате, 
	// edge - отслеживание состояния линии, active_low - уровень активного сигнала,
	// direction - направление (вход или выход) 
	GPIO(unsigned int pin, const char* const gpio, const char* const edge, const char* const active_low, const char* const direction); 
	~GPIO(void);
	
	// смотрим состояние порта gpio
	int read_gpio(void);

	//пишем в порт gpio
	int write_gpio(unsigned int val);
	int err;
private:
	int fd;

};

#endif
