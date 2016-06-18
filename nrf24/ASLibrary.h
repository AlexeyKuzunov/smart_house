#ifndef __ASLibrary__
#define __ASLibrary__
#define CommentLen 20			      //Длина комментария в сообщении
#define AS_NRF24L01_payload 5   //Максимальный размер буфера в байтах для трансивера NRF24L01
								                //должен буть не меньше максимальной длины передаваемых данных
								                //и не больше 32 (стркутуры _AS_COMMAND и _AS_ANSWER)
#define AS_NRF24L01_channel 77  //канал, на котором работает трансивер NRF24L01
								//RF Channel 0 - 127 or 0 - 84 in the US, default 0.

// Команды состоят из 1 команды и 1 параметра
// 0 x - Перезагрузка
// 1 x - Получить количество датчиков
// 2 x - Выполнить измерение по всем датчикам (записать в глобальные переменные, пока не принимать)
// 3 N - Выполнить измерение по датчику N (записать в глобальные переменные, пока не принимать)
// 4 N - Получить данные с датчика N (считать из глобальных переменных)

typedef struct _AS_COMMAND {
  byte  Module = 0; //Номер модуля если модулей больше 6 здесь 1
	byte  Command; //Команда
	byte  Parametr; //Параметр
} AS_Command;

// Ответы состоят из 1 статуса и 1 значения
// 0 x - Ошибка
// 1 N - Данные переданы корректны, N значение

typedef struct _AS_ANSWER {
	byte   Status; //Статус
	float  Value; //Значение
	char   Comment[CommentLen]; //Описание
} AS_Answer;


// Состояние датчиков
// 0 x - Ошибка
// 1 N - Данные актуальны, значение N

typedef struct _AS_SENSORSTATUS
{
	byte	Status; //Статус
	float	Value; //Значение
	char	Comment[CommentLen]; //Описание
} AS_SensorStatus;

#endif // __ASLibrary__
