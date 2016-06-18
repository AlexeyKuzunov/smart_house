#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "ASLibrary.h"

/*
NRF24L01 wiring

1 GND -> GND
2 VCC 3.3V -> 3.3V
3 CE -> D9
4 CSN -> D10
5 SCK -> D13
6 MOSI -> D11
7 MISO -> D12
*/               

RF24 radio(9, 10); // Create a Radio

int Relay = 4;
boolean stateRelay = LOW;

//Переключает реле
void Switch() {
  stateRelay = !stateRelay;
  digitalWrite(Relay, stateRelay);
}

AS_Answer ExecuteCommand(AS_Command MyCommand) {
  AS_Answer  MyAnswer;  // Ответ
  MyAnswer.Status = 0;
  MyAnswer.Value = 0;
  //char P[] = "Y";
  switch (MyCommand.Command) {
 /*   case 1: //Получиь количество датчиков
      MyAnswer.Status = 1;
      MyAnswer.Value = NumSensors;
      //Serial.println("1.");
      break;
    case 2: //Рассчитать все значения датчиков
      MyAnswer.Status = CalculateAllData();
      ///Serial.println("2.");
      break; */
    case 3:
      Switch();
      MyAnswer.Status = stateRelay;
      MyAnswer.Value = 0;
      break;
    /*case 4: //Получиь значение датчика по номеру
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
      break; */
    }
  return MyAnswer;
}

void setup()
{
  Serial.begin(115200);

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, stateRelay);
  
  radio.begin();
  radio.enableDynamicPayloads();  
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_HIGH);  
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(50);
  radio.openWritingPipe(0xF0F0F0F0D2LL);
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);

  
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck( true ) ;
  radio.powerUp();
  radio.startListening();
}

//char receive_payload[32+1]= ""; // 32 bytes is maximum payload

void loop(){
  AS_Command MyCommand; // Команда
  AS_Answer  MyAnswer;  // Ответ
  if (radio.available()){
    radio.read(&MyCommand, sizeof(MyCommand));
    Serial.println(MyCommand.Command);
    //int len = radio.getDynamicPayloadSize();
    //radio.read(receive_payload, len);
    //Serial.print("Received ");
    //Serial.print(receive_payload);
    MyAnswer = ExecuteCommand(MyCommand);
    radio.stopListening();
        
    // send it back
    Serial.println(" Sending back... ");
    //receive_payload[len]='\0';
    bool send_res = 0;
    while (send_res == 0){ // first few sends fail, so retry
     send_res = radio.write(&MyAnswer, sizeof(MyAnswer));
     Serial.println(MyAnswer.Status);
    }
    Serial.println();
    radio.startListening();
  }
  else {
//    delay(10);
  }	
}

