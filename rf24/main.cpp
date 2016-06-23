#include <stdio.h>
#include <string.h>
#include "ASLibrary.h"
#include "gpio_sun7i.h"
#include "RF24.h"
#include <cstdlib>
 
 
/*
nrf24 [-m название модуля] [-d название датчика] значение для датчика 
Если нет значения читаем и выводим на консоль значения датчика.
Если есть значение устанавливаем его и выводим установленное значение в консоль 
*/

#define DIR_PIPES "/tmp/rf24_1"

const uint64_t pipes[2] = {0xF0F0F0F0E2LL,0xF0F0F0F0E1LL};
const int int_gpio_num = 3;

// CE - PB13
// CSN - PI10
RF24 radio(SUNXI_GPB(13), SUNXI_GPI(10), "/dev/spidev0.0");

void setup(void) {
	// setup interrupt
	//gpio_export(int_gpio_num);
	//gpio_set_edge(GPIO_STR, "rising", "1"); //отслеживаем переход из неактивного состояния  активное
	radio.begin();						 //старт работы
	radio.enableAckPayload(); 			//Разрешение отправки нетипового ответа передатчику
	radio.enableDynamicPayloads();
	radio.setAutoAck(1); 				//Установка режима подтверждения приема
	radio.setRetries(15, 15); 			//Установка интервала и количества попыток "дозвона" до приемника
	radio.setDataRate(RF24_2MBPS); 		//Установка минимальной скорости
	radio.setPALevel(RF24_PA_HIGH); 	//Установка максимальной мощности
	radio.setChannel(50); 				//Установка канала вещания
	radio.setCRCLength(RF24_CRC_16);
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1, pipes[0]); // Активация данных для чтения
	// Активация данных для отправки
	//radio.openReadingPipe(1, pipes[1]);
	//mkdir(DIR_PIPES, 0775);
	// Start listening
	radio.startListening();				// Слушаем эфир
	radio.printDetails();
}

//Получает параметр номер Num с утройства с именем addr
AS_Answer Connect(AS_Command MyCommand) {
	AS_Answer  MyAnswer;
	bool timeout = false;
	long started_waiting_at = __millis();
	radio.stopListening();
	if (radio.write(&MyCommand, sizeof(MyCommand)))	{
		radio.startListening();
		while (!radio.available() && !timeout)
			if (__millis() - started_waiting_at > 500) timeout = true;
		if (timeout) 
			printf("Failed, response timed out. \n\r");
		else {
			radio.read(&MyAnswer, sizeof(MyAnswer));
			return MyAnswer;
		}
	}
	else printf("Failed, write timed out. \n\r");		
}

main (int argc, char **argv) { 
	AS_Answer  MyAnswer;
	AS_Command MyCommand;
	setup();
	MyCommand.Command = 3;
	MyAnswer.Value = 0;
	MyAnswer = Connect(MyCommand);
	printf("%d", MyAnswer.Value);
}
