byte servoID_1 = 0x00;
 
void setup(){
   pinMode(2,OUTPUT);
  Serial.begin(9600);
  Serial3.begin(57143);
  Serial.flush();
}
 
void loop(){
requestTemperature(servoID_1); // Request temperature
printBuffer(); // Output it to the Serial Monitor (set to 9600 baud)
delay(1000);
}
 
void requestTemperature(byte servoID){
  byte notchecksum = ~lowByte(servoID + 0x04 + 0x02 + 0x2B + 0x01);
  digitalWrite(2,HIGH);
  delay(1);
  Serial3.write(0xFF);
  Serial3.write(0xFF);
  Serial3.write(servoID);
  Serial3.write(0x04);
  Serial3.write(0x02);
  Serial3.write(0x2B);
  Serial3.write(0x01);
  Serial3.write(notchecksum);
  Serial3.flush();
  digitalWrite(2,LOW);
  delay(1);
}
 
void printBuffer(){
  Serial.println('---');
  delay(20);
  while(Serial3.available()){
     Serial.println(Serial3.read());
     delay(1);
  }
}
