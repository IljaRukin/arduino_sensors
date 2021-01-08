//control servo by pwm
//50Hz and 5-10% duty cycle (pin high)

#include <avr/io.h>
#include <util/delay.h>

#define FCPU 16000000
#define FQ 50
#define PRESCALER 8
#define TOPBIT (uint16_t)(FCPU/(PRESCALER*FQ)-1)

void PWMsetDutyCycle(uint8_t pos){
  OCR1A = (uint16_t)(TOPBIT*(0.03 + 0.09*pos/180));
}

int main(void) {

  // PB1 (D9) as output
  DDRB |= (1 << DDB1);

  //set pin to low
  PORTB &= ~(1<<PB1);

  //clear timer counter
  TCNT1 = 0;

  // overflow bit (to reset timer) -> sets frequency
  ICR1 = TOPBIT;

  //pwm: clear on compare
  TCCR1A |= (1 << COM1A1);

  // fast pwm, none-inverting mode, top bit is ICR1
  TCCR1A |= (1<<WGM11);
  TCCR1B |= (1<<WGM13)|(1<<WGM12);

  // 8x prescaler
  TCCR1B |= (1<<CS11);
  
  // duty cycle (set to 7.5%)
  OCR1A = (uint16_t)(TOPBIT*0.075);

  uint8_t pos = 0;
  while (1) {
    //loop
    for (pos; pos < 180; pos += 1) {
      PWMsetDutyCycle(pos);
      _delay_ms(15);
    }
    for (pos; pos > 0; pos -= 1) {
      PWMsetDutyCycle(pos);
      _delay_ms(15);
    }
  }
}
