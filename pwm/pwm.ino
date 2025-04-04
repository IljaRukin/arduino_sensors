#define PWMpin 9 //3,5,6,9,10,11
//#define DIGITALpin 2

void setup() {
  pinMode(PWMpin, OUTPUT);
  //pinMode(DIGITALpin, OUTPUT);
  analogWrite(PWMpin,0);
  delay(2000);
  analogWrite(PWMpin,240);
  delay(2000);
  analogWrite(PWMpin,80);
  delay(3000);

}

void loop() {
  uint8_t throttle = 80;
  while (throttle < 160){
    throttle += 10;
    analogWrite(PWMpin,throttle);
    delay(1000);
  }
  while (throttle > 80){
    throttle -= 10;
    analogWrite(PWMpin,throttle);
    delay(1000);
  }
  //analogWrite(PWMpin,160);
  //delay(1000);
  //digitalWrite(DIGITALpin,0);
  //delay(1000);
  //digitalWrite(DIGITALpin,1);
  //delay(1000);
}
