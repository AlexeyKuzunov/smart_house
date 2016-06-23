#include "gpio_sun7i.h"
#include <cstdlib>
#include <iostream>
#include "RF24.h"
#include "ASLibrary.h"

using namespace std;

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};


// CE - PD13
// CSN - PD02
RF24 radio(SUNXI_GPB(13), SUNXI_GPI(10), "/dev/spidev0.0");

void setup(void)
{
        radio.begin();
        // enable dynamic payloads
        radio.enableDynamicPayloads();
        // optionally, increase the delay between retries & # of retries
        radio.setRetries(15, 15);
        radio.setDataRate(RF24_2MBPS);
	radio.setPALevel(RF24_PA_HIGH);
	radio.setChannel(50);
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
        //radio.printDetails();
}

void loop(void)
{
        AS_Answer  MyAnswer;
        AS_Command MyCommand;
        MyCommand.Command = 3;
        MyAnswer.Value = 0;
        radio.stopListening();
        // Take the time, and send it.  This will block until complete
        radio.write(&MyCommand, sizeof(MyCommand));
        // Now, continue listening
        radio.startListening();
        // Wait here until we get a response, or timeout
        long started_waiting_at = __millis();
        bool timeout = false;
        while (!radio.available() && !timeout) {
                if (__millis() - started_waiting_at > 500) timeout = true;
        }
        // Describe the results
        if (timeout) printf("Failed, response timed out.\n\r");
        else {
              radio.read(&MyAnswer, sizeof(MyAnswer));
              printf("Got response value=%i\n\r", MyAnswer.Status);
        }
        sleep(1);
}

int main(int argc, char** argv)
{
        setup();
        loop();

        return 0;
}
