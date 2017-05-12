#include "gpio.h"
#include <cstdlib>
#include <iostream>
#include "RF24.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
using namespace std;

#define GPIO_STR "1"
//#define SUNXI_GPB_10 4
#define SUNXI_GPB_13 5
#define MAX_BUF 64

const uint64_t pipes[2] = {0xF0F0F0F0E1LL,0xF0F0F0F0E2LL};
const int int_gpio_num = 1;
const int max_payload_size = 32;

char receive_payload[max_payload_size + 1];

// CE - PB13
// CSN - аппаратный
RF24 radio(SUNXI_GPB_13, "/dev/spidev0.0");

//#define SYSFS_GPIO_DIR "/sys/class/gpio"

GPIO* irq;

void setup(void)
{
	//Настраиваем пин IRQ
	irq = new GPIO(1, "in", "rising", "1");
	// setup interrupt
	radio.begin();
	// enable dynamic payloads
	radio.enableAckPayload();
	radio.enableDynamicPayloads();
	radio.setAutoAck(1);
	// optionally, increase the delay between retries & # of retries
	radio.setRetries(15, 15);
	radio.setDataRate(RF24_2MBPS);
	radio.setPALevel(RF24_PA_MIN);
	radio.setChannel(50);
	radio.setCRCLength(RF24_CRC_16);
	// Open pipes to other nodes for communication
	// Open pipe for reading
	radio.openReadingPipe(0, pipes[0]);
	radio.openReadingPipe(1, pipes[1]);
	// Start listening
	radio.startListening();
	// Dump the configuration of the rf unit for debugging
	radio.printDetails();
}

int gpio_fd;
uint8_t intResult;

void gotData(void){
	bool rx = 0, blnTXOK = 0, blnTXFail = 0;
	intResult = 0;
	uint8_t pipe_num = 0;
	radio.whatHappened(blnTXOK,blnTXFail,rx, &pipe_num);

	if(blnTXFail){
		intResult = 2;
	}else if(blnTXOK){
		intResult = 1;
	}else if ( rx ){
		intResult = 3;
		uint8_t len = radio.getDynamicPayloadSize();

		bool more_available = true;
		while (more_available)
		{
			// Fetch the payload, and see if this was the last one.
			more_available = radio.read( receive_payload, len );
			// Put a zero at the end for easy printing
			receive_payload[len] = 0;
			// Print received packet
			printf("[%d] Data size=%i value=%s\n\r",pipe_num, len,receive_payload);
			// next payload can be of different size
			if (more_available){
				len = radio.getDynamicPayloadSize();
			}
		break;
		}
	
	}
//	printf("intResult %d %d %d %d \n\r",blnTXOK,blnTXFail,rx,intResult);
	fflush (stdout) ;
//	radio.clearInterrupt();
}

int main(int argc, char** argv)
{
        setup();

        while(1){
		//Если приходит прерывание по IRQ пину
		if(irq->read_gpio()){
			gotData();
		}
	}
        return 0;
}