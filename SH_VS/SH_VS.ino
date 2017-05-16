/*
 Name:		SH_VS.ino
 Created:	14.06.2016 11:39:27
 Author:	Алексей



*/


#include <RF24_config.h>
#include <RF24.h>
//#include <nRF24L01.h>
#include <SPI.h>
#include "ASLibrary.h"
#include <avr/sleep.h>
#include "ds18b20.h"


/*
NRF24L01 wiring

1 GND -> GND
2 VCC 3.3V -> 3.3V
3 CE -> D9
4 CSN -> D10
5 SCK -> D13
6 MOSI -> D11
7 MISO -> D12
*/


#define NumSensors 3



int Relay = 4;
boolean stateRelay = LOW;

AS_SensorStatus MySensors[NumSensors] = {
	0,0,"18B20_temp(C)",
	0,0,"Batt_pow(V)",
	0,0,"Swith"};

RF24 radio(9, 10);  //CE pin, CS pin
const uint64_t pipes[2] = { 0xF0F0F0F0E2LL, 0xF0F0F0F0E1LL };

//Переключает реле
void Switch() {
	stateRelay = !stateRelay;
	digitalWrite(Relay, stateRelay);
}

//Функция определения напряжения питания устройства
static float vccRead(byte us = 250) {
	ADMUX = 1 << REFS0; // опорное напряжение - Vcc
	ADMUX |= 0x0E;
	// объект измерения - внутренний источник
	// стабилизированного напряжения 1.1В
	delayMicroseconds(us);
	ADCSRA |= 1 << ADSC;			//запуск АЦ-преобразования
	while (ADCSRA & (1 << ADSC));	// и ожидание его завершения
	word x = ADC;
	//return (1125300L / x)/1000;
	return x ? (1100L * 1023) / x : -1;
}


//Вычисляет все данные и заполняет массив значений датчиков
byte CalculateAllData() {
	MySensors[0].Status = 1;
	MySensors[0].Value = Get_18B20_Data();
	MySensors[1].Status = 1;
	MySensors[1].Value = vccRead() / 1000;
	MySensors[2].Status = 1;
	MySensors[2].Value = stateRelay;
	//Serial.print(MySensors[2].Value);
	return 1;
}

AS_Answer ExecuteCommand(AS_Command MyCommand) {
	AS_Answer  MyAnswer;  // Ответ
	MyAnswer.Status = 0;
	MyAnswer.Value = 0;
	//char P[] = "Y";
	switch (MyCommand.Command) {
		case 1: //Получиь количество датчиков
			MyAnswer.Status = 1;
			MyAnswer.Value = NumSensors;
			//Serial.println("1.");
			break;
		case 2: //Рассчитать все значения датчиков
			MyAnswer.Status = CalculateAllData();
			///Serial.println("2.");
			break;
		case 3:
			Switch();
			MyAnswer.Status = 1;
			MyAnswer.Value = stateRelay;
			break;
		case 4: //Получиь значение датчика по номеру
			//Serial.println("4.");
			if ((MyCommand.Parametr + 1) > NumSensors) {
				MyAnswer.Status = 0;
				MyAnswer.Value = 0;
				memcpy(&MyAnswer.Comment, &"Too big num par.", CommentLen);
				}
			else {
				MyAnswer.Status = MySensors[MyCommand.Parametr].Status;
				MyAnswer.Value = MySensors[MyCommand.Parametr].Value;
				memcpy(&MyAnswer.Comment, &MySensors[MyCommand.Parametr].Comment, CommentLen);
				//MyAnswer.Comment[0] = P[0];
			}
			break;
		}
	return MyAnswer;
}

void Connect() {
	AS_Command MyCommand; // Команда
	AS_Answer  MyAnswer;  // Ответ	
	if (radio.available()) {
		radio.read(&MyCommand, sizeof(MyCommand));
		MyAnswer = ExecuteCommand(MyCommand);
		Serial.println("Data");
	}
	radio.stopListening();
	radio.write(&MyAnswer, sizeof(MyAnswer));
	radio.startListening();
}

void setup() {
	Serial.begin(9600);
	pinMode(Relay, OUTPUT);
	digitalWrite(Relay, stateRelay);
	attachInterrupt(0, Connect, LOW);

	MySensors[0].Status = 0;
	MySensors[1].Status = 0;
	MySensors[2].Status = 0;

	radio.begin();                          // Включение модуля;
	radio.enableAckPayload(); 			//Разрешение отправки нетипового ответа передатчику
	radio.enableDynamicPayloads();
	radio.setChannel(50);                    // Установка канала вещания;
	radio.setRetries(15, 15);                // Установка интервала и количества попыток "дозвона" до приемника;
	radio.setDataRate(RF24_2MBPS);        // Установка минимальной скорости;
	radio.setPALevel(RF24_PA_HIGH);          // Установка максимальной мощности;
	radio.setAutoAck(1);                    // Установка режима подтверждения приема;
	radio.setCRCLength(RF24_CRC_16);
	radio.openWritingPipe(pipes[0]);     // Активация данных для отправки
	radio.openReadingPipe(1, pipes[1]);   // Активация данных для чтения
	radio.startListening();                 // Слушаем эфир.
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//sleep_enable();//Переходим в спящий режим
}

// the loop function runs over and over again until power down or reset
void loop() {
	CalculateAllData();
	Connect();
	Serial.println(MySensors[0].Value);
	//Serial.println(vccRead()/1000);
}
