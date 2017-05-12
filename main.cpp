/*
формат запуска

список параметров для подачи команды датчику
имя файла -m модуль -s датчик -c команда датчику

чтение показаний с одного датчика
имя файла -m модуль -s датчик

чтение показаний со всех датчиков на модуле 
имя файла -m модуль

отображение всех подключенных модулей с датчиками
имя файла

*/

#include "gpio.h"
#include "RF24.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* Имя программы. */
const char* program_name;

/* Вывод информации об использовании программы в поток STREAM
(обычно stdout или stderr) и завершение работы с выдачей кода
EXIT_CODE. Возврат в функцию main() не происходит */
void print_usage(FILE* stream, int exit_code) {
	fprintf(stream, "Usage: %s options [ inputfile ... ]\n",
	program_name);
	fprintf(stream,
	" -h --help Display this usage information.\n"
	" -m --module указание модуля.\n"
	" -s --sensor указание датчика.\n"
	" -с --command указание команды.\n");
	exit(exit_code);
}

/* Точка входа в основную программу, параметр ARGC содержит размер
списка аргументов; параметр ARGV -- это массив указателей
на аргументы. */
int main(int argc, char* argv[]) {
	int next_option;

	/* Строка с описанием возможных коротких опций. */
	const char* const short_options = "hm:s:c:";

	/* Массив с описанием возможных длинных опций. */
	const struct option long_options[] = {
		{ "help", 0, NULL, 'h' },
		{ "module", 1, NULL, 'm' },
		{ "sensor", 1, NULL, 's' },
		{ "command", 1, NULL, 'c' },
		{ NULL, 0, NULL, 0 } /* Требуется в конце массива. */
	};

	/* Имя файла, в который записываются результаты работы
	программы, или NULL, если вывод направляется в поток
	stdout. */
	const char* output_filename = NULL;

	/* Следует ли выводить развернутые сообщения. */
	int verbose = 0;

	/* Запоминаем имя программы, которое будет включаться
	в сообщения. Оно хранится в элементе argv[0] */
	program_name = argv[0];

do {
	next_option = getopt_long(argc, argv, short_options, long_options, NULL);
	switch(next_option) {
		case 'h': /* -h или --help */
			/* Пользователь запросил информацию об использовании
			программы, нужно вывести ее в поток stdout и завершить
			работу с выдачей кода 0 (нормальное завершение). */
			print_usage(stdout, 0);
			break;
		case 'm': /* */
			printf ("параметр m со значением `%s'\n", optarg);
			break;
		case 's': /* */
			printf ("параметр s со значением `%s'\n", optarg);
			break;
		case 'c': /* */
			printf ("параметр c со значением `%s'\n", optarg);
			break;
		
		case '?': /* Пользователь ввел неверную опцию. */
			/* Записываем информацию об использовании программы в поток
			stderr и завершаем работу с выдачей кода 1
			(аварийное завершение). */
			print_usage(stderr, 1);
		case -1: /* Опций больше нет. */
			break;
		default: /* Какой-то непредвиденный результат. */
	abort();
	}
}
while (next_option != -1);

/* Обработка опций завершена, переменная OPTIND указывает на
первый аргумент, не являющийся опцией. В демонстрационных
целях отображаем эти аргументы, если задан режим VERBOSE. */
if (verbose) {
	int i;
	for (i = optind; i < argc; ++i)
	printf("Argument: %s\n", argv[i]);
}
/* Далее идет основное тело программы... */
return 0;
}
