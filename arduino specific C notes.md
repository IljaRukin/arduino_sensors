# Arduino

#include <library> \
#include "file"

### setup/main loop

void setup{ \
}

void loop{ \
}



### GPIO

pinMode(7,INPUT); \
pinMode(7,INPUT_PULLUP); \
a = digitalRead(7)	//digital value 0/1 \
a = analogRead(7)   //vaule between 0 and 1023 (ADC)

pinMode(7,OUTPUT); \
digitalWrite(8,LOW/HIGH) \
analogWrite(8,LOW/HIGH/#value#)   //value between 0 and 255 (PWM-Signal) \
attachInterrupt(PIN,function,LOW/HIGH/CHANGE/FALLING/RISING);   //PINs: 0=D2; 1=D3;



### Serial communication

Serial: \
void setup{ \
  Serial.begin(9600); \
} \
void loop{ \
  Serial.println(); \
  Serial.print(); \
  Serial.write(); \
}



### logical operations:

eq              == \
and             && \
and_eq          &= \
bitand          & \
bitor           | \
not             ! \
not_eq          != \
or              || \
or_eq           |= \
xor             ^ \
xor_eq          ^=
