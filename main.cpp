#include "gpio.h"
#include "RF24.h"
#include "ASLibrary.h"
#include <iostream>
#include <cstdlib>

#define ce_gpio 5 //ce пин описан в script.fex как 5 пин gpio

const uint64_t pipes[2] = {0xF0F0F0F0E1LL,0xF0F0F0F0E2LL};

RF24 radio(ce_gpio, "/dev/spidev0.0");


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
	
	//mkdir(DIR_PIPES, 0775);
	// Start listening
	//radio.startListening();
	// Dump the configuration of the rf unit for debugging
	//radio.printDetails();
}

bool set_command(AS_Command SetCommand) //отправляем команду на клиент
{
	radio.stopListening();
	return(radio.write(&SetCommand, sizeof(SetCommand)));
}

bool get_answer(AS_Answer GetAnswer) //принимаем данные с клиента
{
	long started_time = __millis();
	bool timeout = false;
	radio.startListening();
	while (!radio.available() && !timeout)
		if (__millis() - started_time > 500) timeout = true;
	if (timeout) printf("Failed, response timed out.\n\r");
	else
	{
		radio.read(&GetAnswer, sizeof(GetAnswer));
	}

}

int main(int argc, char** argv)
{
	AS_Command MyCommand;
	AS_Answer MyAnswer ;

	MyCommand.id = 1;
	MyCommand.Command = 4;
	MyCommand.Parametr = 0;

	init_nrf();
	
	if (set_command(MyCommand)) 
	{
		get_answer(MyAnswer);
		printf("[%d]/\n", MyAnswer.Value);
	}

	return 0;
}
