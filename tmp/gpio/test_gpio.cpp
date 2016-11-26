
#include "gpio_sun7i.h"

GPIO* gpio_led;
GPIO* gpio_swith;

const int int_gpio_led = 4;
const int int_gpio_swith = 3;

void __msleep(int milisec){
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = milisec * 1000000L;
	nanosleep(&req, (struct timespec *)NULL);
}

int main(int argc, char** argv) {

gpio_led = new GPIO(int_gpio_led, "out", "none", "0");
gpio_swith = new GPIO(int_gpio_swith, "in", "rising", "0");
//gpio_swith = new GPIO(int_gpio_swith, "in", "none", "0");
int flag=0;
int status_led, new_status;
//int r=0;
int i;
while(1){
//r = !r;
//int i = gpio_led->write_gpio(r);
//__msleep(100000000);
	i = gpio_swith->read_gpio();
	printf("%d", i);
	if ((i==1)&&(flag==1)){
		
		flag = 0;
		//printf("%d", gpio_swith->read_gpio());
		}
	if ((i==0)&&(flag==0)){
		//gpio_led->write_gpio(!gpio_led->read_gpio());
		status_led = gpio_led->read_gpio();
		new_status = !status_led;
		gpio_led->write_gpio(new_status);
		flag = 1;
		i = 1;
		printf("%d", new_status);
		}
//	__msleep(1000);
	}
//	return 0;
}



