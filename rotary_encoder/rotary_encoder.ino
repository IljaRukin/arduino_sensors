//rotary encoder using interrupt pin INT0 (INT1 is currently used only as digital input)
#include <avr/io.h>

long int counter = 0;

//trigger on inputA change
ISR( INT0_vect )
{
  bool inputA = PIND & (1 << PIND2);
  bool inputB = PIND & (1 << PIND3);
  if(inputA == inputB){
    counter += 1;
  }
  else{
    counter -= 1;
  }
  Serial.println(counter);
}

void setup() {
  //set interrupt INT0
  EIMSK |= (1<<INT0); //enable INT0 & INT1
  EICRA |= (1<<ISC01)|(1<<ISC00); // interrupt INT0 on any change
  //set interrupt INT1
//  EIMSK |= (1<<INT1); //enable INT0 & INT1
//  EICRA |= (1<<ISC11)|(1<<ISC10); // interrupt INT1 on any change

  //set inputs
  DDRD &= ~((1<<PD2)|(1<<PD3));     //set PD2(INT0) & PD3(INT1) as input
  PORTD &= ~((1<<PD2)|(1<<PD3));    //disable pullups

  sei();  //enable interrupts
  Serial.begin(9600);
}

void loop() {
  //do something
}
