#include "gpio.h"
#include "RF24.h"
#include "ASLibrary.h"

const uint64_t pipes[2] = {0xF0F0F0F0E1LL,0xF0F0F0F0E2LL};

RF24 radio(SUNXI_GPB(13), "/dev/spidev0.0");


void init_nrf(void)
{
	// установка прерываний пока не используем
//	gpio_export(int_gpio_num);
//	gpio_set_edge(GPIO_STR, "rising", "1"); //отслеживаем переход из неактивного состояния  активное

	radio.begin(); //старт работы
	// enable dynamic payloads
	radio.enableAckPayload(); //Разрешение отправки нетипового ответа передатчику
	radio.enableDynamicPayloads();
	radio.setAutoAck(1); //Установка режима подтверждения приема
	radio.setRetries(15, 15); //Установка интервала и количества попыток "дозвона" до приемника
	radio.setDataRate(RF24_2MBPS); //Установка минимальной скорости
	radio.setPALevel(RF24_PA_HIGH); //Установка максимальной мощности
	radio.setChannel(50); //Установка канала вещания
	radio.setCRCLength(RF24_CRC_16);
	// Open pipes to other nodes for communication
	// Open pipe for reading
	radio.openReadingPipe(0, pipes[0]);
	radio.openReadingPipe(1, pipes[1]);
	mkdir(DIR_PIPES, 0775);
	// Start listening
	radio.startListening();
	// Dump the configuration of the rf unit for debugging
	radio.printDetails();
}

int main(int argc, char** argv)
{
	init_nrf();
}
