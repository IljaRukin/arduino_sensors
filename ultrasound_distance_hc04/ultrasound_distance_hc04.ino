int echo = 2;
int trig = 3;
long duration;
long distance;

void setup() {
  Serial.begin (9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, LOW);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  duration += 0; //offset
  distance = duration * 0.0343 / 2; // cm/ms & path twice traveled
  
  //Serial.println(duration);
  Serial.println(distance);
  delay(1000);
}
