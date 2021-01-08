//move stepper motor either directly (with: setup_stepper();move_stepper(int);)
//or using a stepper motor driver (with: setup_a4988();drive_a4988(int);)

//pins for direct controll
#define timeout 5000
#define A D5
#define C D6
#define B D7
#define D D8

//pins for steper motor driver
#define STEP D5
#define DIR D8

void setup(){
  Serial.begin(9600);
  //setup_stepper();
  setup_a4988();
}

void loop(){
  //move_stepper();
  drive_a4988(200);
  drive_a4988(-200);
}

void setup_a4988(void){
  pinMode(STEP,OUTPUT);
  pinMode(DIR,OUTPUT);
  digitalWrite(STEP,LOW);
  digitalWrite(DIR,LOW);
}

void drive_a4988(long int i){
  if(i<0){
    digitalWrite(DIR,LOW);
  }
  else{
    digitalWrite(DIR,HIGH);
  }
  i = abs(i);
  for(int s=0;s<i;s++){
    digitalWrite(STEP,HIGH);
    delayMicroseconds(timeout);
    digitalWrite(STEP,LOW);
    delayMicroseconds(timeout);
  }
}

void setup_stepper(void){
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  digitalWrite(A,LOW);
  digitalWrite(B,LOW);
  digitalWrite(C,LOW);
  digitalWrite(D,LOW);
}

void move_stepper(long int i){
  for(int s=0;s<i;s++){
    digitalWrite(A,LOW);
    digitalWrite(B,HIGH);
    delayMicroseconds(timeout);
    digitalWrite(C,LOW);
    digitalWrite(D,HIGH);
    delayMicroseconds(timeout);
    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    delayMicroseconds(timeout);
    digitalWrite(C,HIGH);
    digitalWrite(D,LOW);
    delayMicroseconds(timeout);
  }
}
