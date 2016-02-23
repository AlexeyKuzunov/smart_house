//проба пер
#include "gpio_sun7i.h"
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "RF24.h"

using namespace std;

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 2;
int next_payload_size = min_payload_size;

#define DATA_DIR "/tmp/rf24_1" 

char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

// CE - PD13
// CSN - PD02
RF24 radio(SUNXI_GPB(13), SUNXI_GPI(10), "/dev/spidev0.0");

int im_fd;
int ex_fd;
mode_t mode = S_IRUSR | S_IWUSR;
int flags = O_RDWR | O_CREAT;

void setup(void)
{
    mkdir(DATA_DIR, 0777);
    radio.begin();
    // enable dynamic payloads
    radio.enableDynamicPayloads();
    // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);
    radio.setDataRate(RF24_2MBPS);
	radio.setPALevel(RF24_PA_HIGH);
	radio.setChannel(89);
    // Open pipes to other nodes for communication
    // Open 'our' pipe for writing
    // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
	radio.setCRCLength(RF24_CRC_16);
	radio.setAutoAck( true ) ;
    // Start listening
    radio.startListening();
    // Dump the configuration of the rf unit for debugging
    radio.printDetails();
}

void loop(void)
{
        ssize_t ret;
        char ex_txt[32+1];
                //создаем во временном каталоге файл для размещения команд удаленному устройству
        ex_fd = open("/tmp/rf24_1/export", flags, mode);
        if(ex_fd < 0){
            printf("Cannot open file export");
        }
        if (ex_fd > 0){
            fchmod(ex_fd, 0666);
        }

        ret = read(ex_fd, ex_txt, 2);
        if (ret < 0){
                printf("Cannot read file export");
        }
        close(ex_fd);

        // Ping out.
        // The payload will always be the same, what will change is how much of it we send.
        static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";

        // First, stop listening so we can talk.
        radio.stopListening();

        // Take the time, and send it.  This will block until complete
        printf("Now sending length %i...", next_payload_size);
	radio.write(ex_txt, strlen(ex_txt));

        // Now, continue listening
        radio.startListening();

        // Wait here until we get a response, or timeout
        long started_waiting_at = __millis();

        bool timeout = false;
        while (!radio.available() && !timeout)
                if (__millis() - started_waiting_at > 500) timeout = true;
        // Describe the results
        if (timeout) printf("Failed, response timed out.\n\r");
        else
        {
                // Grab the response, compare, and send to debugging spew
                uint8_t len = radio.getDynamicPayloadSize();
                radio.read(receive_payload, len);
                // Put a zero at the end for easy printing
                receive_payload[len] = 0;
                // Spew it
                
                //создаем во временном каталоге файл для чтения состояния удаленного устройства
                im_fd = open("/tmp/rf24_1/import", flags, mode);
                if(im_fd < 0){
                        printf("Cannot open file import");
                }
                if (im_fd > 0){
                        fchmod(im_fd, 0644);
                }
                lseek(im_fd, 0, SEEK_SET);
                write(im_fd, receive_payload, len);
                close(im_fd);
                
                printf("Got response size=%i value=%s\n\r", len, receive_payload);
        }
        // Update size for next time.
        next_payload_size += payload_size_increments_by;
        if (next_payload_size > max_payload_size)
                next_payload_size = min_payload_size;

        sleep(1);
}

int main(int argc, char** argv)
{
        setup();
        while(1) loop();

        return 0;
}
