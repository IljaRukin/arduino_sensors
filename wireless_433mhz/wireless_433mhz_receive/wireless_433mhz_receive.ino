//receiving data over 433mhz module using manchester encoding
//note: the first byte of dataBuffer should be the size of the message

#include "Manchester.h"

#define RX_PIN 4

#define BUFFER_SIZE 20
uint8_t dataBuffer[BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_600);
  man.beginReceiveArray(BUFFER_SIZE, dataBuffer);
}

void loop() {
  if (man.receiveComplete()) 
  {
    //uint8_t receivedSize = 0;
    //receivedSize = buffer[0];
    for(uint8_t i=1; i<BUFFER_SIZE; i++){
      Serial.print(dataBuffer[i]);
      Serial.print(" ");
    }
    Serial.println();
    man.beginReceiveArray(BUFFER_SIZE, dataBuffer);
  }
}
