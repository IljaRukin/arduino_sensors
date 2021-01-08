//sending data over 433mhz module using manchester encoding
//note: the first byte of dataBuffer should be the size of the message

#include "Manchester.h"

#define TX_PIN 5

#define BUFFER_SIZE 20
uint8_t dataBuffer[BUFFER_SIZE] = {BUFFER_SIZE,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

void setup() 
{
  Serial.begin(9600);
  man.setupTransmit(TX_PIN, MAN_600);
}

void loop() 
{
  man.transmitArray(BUFFER_SIZE, dataBuffer);
  delay(500);
}
