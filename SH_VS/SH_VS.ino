/*
 Name:		SH_VS.ino
 Created:	14.06.2016 11:39:27
 Author:	Алексей
*/

#include <OneWire.h>
#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "ASLibrary.h"
#include <avr/sleep.h>


#define NumSensors 2

OneWire ds(2);      //Создаем датчик DS18B20 на 2 пине

AS_SensorStatus MySensors[NumSensors] = {
	0,0,"18B20_temp(C)",
	0,0,"Batt_pow(V)" };

RF24 radio(8, 7);
const uint64_t pipes[2] = { 0xF0F0F0F0E2LL, 0xF0F0F0F0E1LL };

//Возвращает значение температуры с датчика 18B20
float Get_18B20_Data() {
	byte DSdata[2];
	ds.reset();
	ds.write(0xCC);
	ds.write(0x44);
	delay(1000);
	ds.reset();
	ds.write(0xCC);
	ds.write(0xBE);
	DSdata[0] = ds.read();
	DSdata[1] = ds.read();
	int Temp = (DSdata[1] << 8) + DSdata[0];
	return (float)Temp / 16;
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
	//Serial.print(MySensors[2].Value);
	return 1;
}

AS_Answer ExecuteCommand(AS_Command MyCommand) {
	AS_Answer  MyAnswer;  // Ответ
	MyAnswer.Status = 0;
	MyAnswer.Value = 0;
	char P[] = "Y";
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

void setup() {
	Serial.begin(9600);
	//MySensors[0].Status = 0;
	//MySensors[1].Status = 0;
	
	//radio.begin();
	//radio.openWritingPipe(pipes[0]);
	//radio.openReadingPipe(1, pipes[1]);
	
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//sleep_enable();//Переходим в спящий режим
}

// the loop function runs over and over again until power down or reset
void loop() {
	/*	AS_Command MyCommand; // Команда
	AS_Answer  MyAnswer;  // Ответ
	if (radio.available()) 	{
	bool done = false;
	while (!done) {
		done = radio.read(&MyCommand, sizeof(MyCommand));
		MyAnswer = ExecuteCommand(MyCommand);

		}
	}   */
	Serial.println(Get_18B20_Data());
	Serial.println(vccRead()/1000);
}
