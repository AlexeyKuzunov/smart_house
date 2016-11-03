Ошибка "can't send spi message: Invalid argument" может возникать после обновления ядра. 
Проверить в файле /usr/include/linux/spi/spidev.h наличие в структуре spi_ioc_transfer поля __u16 interbyte_usecs

