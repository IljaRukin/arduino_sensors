#include <SoftwareSerial.h>

SoftwareSerial BTSerial(8, 9); // RX | TX

void setup() {
  Serial.begin(9600); //serial to pc over usb
  BTSerial.begin(38400); //serial to hc-05
  Serial.println("to enter AT mode push button on power up");
  Serial.println("and release as led lights up");
  Serial.println("also change baud of BTSerial to 38400");
  Serial.println("in the serial monitor keep baud at 9600 and set both NL & CR on");
  Serial.println("Enter AT commands:");
}

void loop() {
 if (BTSerial.available())
    Serial.write(BTSerial.read());
 
 if (Serial.available())
    BTSerial.write(Serial.read());
}
