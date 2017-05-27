/* В дефайнах */
#include "gpio.h"
#include "RF24.h"
#include <iostream>
#include <cstdlib>

RF24 radio(5, "/dev/spidev0.0");
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL};

void setup(void)
{
  // Старт работы;
  radio.begin();
  radio.setAutoAck(false);
  // Разрешение отправки нетипового ответа передатчику;
  radio.enableAckPayload();
  // enable dynamic payloads
  //radio.enableDynamicPayloads();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(128);
  radio.setPALevel(RF24_PA_MAX);
  // Open pipes to other nodes for communication
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  // Start listening
  //radio.startListening();
  // Dump the configuration of the rf unit for debugging
 // radio.printDetails();
}

void loop(void)
{
  
}

int main(int argc, char** argv)
{
  setup();
  while(1) loop();

  return 0; 
}
