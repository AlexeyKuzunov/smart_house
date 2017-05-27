#ifndef __ASLibrary__
#define __ASLibrary__

#define CommentLen			20				//Длина комментария в сообщении
#define RF24_CHANNEL	    76              // канал, на котором работает трансивер NRF24L01 RF channel for the sensor net, 0-127
#define RF24_DATARATE 	    RF24_250KBPS    //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
#define RF24_PA_LEVEL       RF24_PA_MAX    //Sensor PA Level == RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBM, and RF24_PA_MAX=0dBm
#define RF24_PA_LEVEL_GW	RF24_PA_LOW  //Gateway PA Level, defaults to Sensor net PA Level.  Tune here if using an amplified nRF2401+ in your gateway.
#define RF24_PAYLOAD 		5   		//Максимальный размер буфера в байтах для трансивера NRF24L01
										//должен буть не меньше максимальной длины передаваемых данных
										//и не больше 32 (стркутуры _AS_COMMAND и _AS_ANSWER)
 


// Команды состоят из 1 команды и 1 параметра
// 0 x - Перезагрузка
// 1 x - Получить количество датчиков
// 2 x - Выполнить измерение по всем датчикам (записать в глобальные переменные, пока не принимать)
// 3 N - Выполнить измерение по датчику N (записать в глобальные переменные, пока не принимать)
// 4 N - Получить данные с датчика N (считать из глобальных переменных)

typedef struct _AS_COMMAND {
	unsigned char id; //идентификатор клиента. Номер ячейки с адресом канала
	unsigned char  Command; //Команда
	unsigned char  Parametr; //Параметр
} AS_Command;

// Ответы состоят из 1 статуса и 1 значения
// 0 x - Ошибка
// 1 N - Данные переданы корректны, N значение

typedef struct _AS_ANSWER {
	unsigned char id; //идентификатор
	unsigned char   Status; //Статус
	unsigned long count;// счетчик передач для контроля качества канала
	float  Value; //Значение
	char   Comment[CommentLen]; //Описание
} AS_Answer;


// Состояние датчиков
// 0 x - Ошибка
// 1 N - Данные актуальны, значение N

typedef struct _AS_SENSORSTATUS
{
	unsigned char	Status; //Статус
	float	Value; //Значение
	char	Comment[CommentLen]; //Описание
} AS_SensorStatus;

#endif // __ASLibrary__
