const byte ledPin = 13;
const byte interruptPin = 2;
volatile boolean state = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), action, FALLING);
}

void loop() {
  Serial.println(state);
  delay(1000);
}

void action() {
  state = !state;
  Serial.println("action");
  Serial.println(state);
  digitalWrite(ledPin, state);
}
