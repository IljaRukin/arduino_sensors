
void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue1 = analogRead(A0);//-512;
  int sensorValue2 = analogRead(A1);//-512;
  int sensorValue3 = analogRead(A2);//-512;
  int sensorValue4 = analogRead(A3);//-512;
  
  Serial.print(sensorValue1);
  Serial.print(" ");
  Serial.print(sensorValue2);
  Serial.print(" ");
  Serial.print(sensorValue3);
  Serial.print(" ");
  Serial.print(sensorValue4);
  Serial.print(" ");
  Serial.println();
  
  Serial.print(1023);  // To freeze the upper limit
  //Serial.print(512);  // To freeze the upper limit
  Serial.print(" ");
  Serial.print(0);  // To freeze the lower limit
  //Serial.print(-512);  // To freeze the lower limit
  Serial.print(" ");

}
