#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/poll.h>

#define PATH_MAX 64


// set edge event on specific gpio_line
int gpio_edge_set(int n, const char *edge_str)
{
    char filename[PATH_MAX];
    FILE *file;

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/edge", n);
    file = fopen(filename, "w");
    if (file == NULL) return -1;
    fprintf(file, "%s\n", edge_str);
    fclose(file);

    return 0;
}

// set GPIO line polling mode
int gpio_poll(int n)
{
    char filename[PATH_MAX];
    int fd;
    char c;
    int err;

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/value", n);
    fd = open(filename, O_RDONLY);
    if (fd < 0) return -1;

    read(fd, &c, sizeof(c));

    return fd;
}

// get GPIO line value
int gpio_get(int fd, int timeout)
{
    struct pollfd pollfd[1];
    char c;
    int err;

    pollfd[0].fd = fd;
    pollfd[0].events = POLLPRI | POLLERR;
    pollfd[0].revents = 0;

    err =  poll(pollfd, 1, timeout);
    if(err != 1) return -1;

    lseek(fd, 0, SEEK_SET);
    err = read(fd, &c, sizeof(c));
    if(err != 1) return -1;

    return atoi(&c);
}

int main(int argc, char** argv) {
    int prn;
//    printf("Test\n");
    gpio_edge_set(3, "rising");
    int fd = gpio_poll(3);
    while(1) {
        prn = gpio_get(fd, -1);
        printf("%d\n", prn);  
        }
    }