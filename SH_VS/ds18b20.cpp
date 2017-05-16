#include "ds18b20.h"

OneWire ds(3);      //Создаем датчик DS18B20 на 2 пине

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