/*
###OLED 128x64 I2C with SSD1306 driver
#libraries:
Adafruit GFX (Graphics)
Adafruit SSD1306 (includes <Wire.h> for I2C)
Pins:
 * GND = GND
 * VCC = 5V
 * SCK/SCL = A5
 * SDA = A4
I2C Address: 0x3C

###Voltage Reference 5V (Vref)
###Buzzer (D2)
###Battery voltage measurement at (A2)/(A3)
*/
#define F_CPU 16000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font
//uint8_t sensor;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Create display

#define V_MIN 1.0*1000
#define V_ZERO 0.1*1000
#define BUZZER 1
float voltage1 = 0;
float voltage2 = 0;
float capacity1 = 0;
float capacity2 = 0;
float counter = 0;

void initialize(void){
  //led
  DDRB |= (1 << PB5); //led
  PORTB &= ~(1 << PB5); //led off

  //buzzer
  DDRD |= (1 << PD2); //buzzer PD2
  PORTD &= ~(1 << PD2); //buzzer off
  
  //ADC
  DDRC &= ~0b00001100;  //adc input pins PC2-PC3
//  MCUCR |= (1 << PUD);  //disable all pull-ups (conflict with i2c)
  DIDR0 |= (1 << ADC2D) | (1 << ADC3D); //disable digital input
  ADMUX |= (0<<REFS1)|(1<<REFS0); //Vref from pin Aref
  ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //enable adc, prescaler 128

  //sleep mode
/*  SMCR |= (1 << SE);  //enable sleep modes
  MCUCR |= (1 << BODS) | (1 << BODSE);  //disable brown out detection
  //additional power save configuration
  PRR |= (0 << PRTWI) | (1 << PRTIM2) | (0 << PRTIM0) | (1 << PRUSART0);//do not disable adc (1 << PRADC) and Timer1 (1 << PRTIM1)
*/
  MCUSR = 0;
  
/*  //set timer2 as interrupt(8bit,clock:16mHz)
  TIMSK2 |= (1<<TOIE2);
  TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); // prescaler 1024 (0.016384s)
  TCNT2 = 0;*/
  //set timer1 as interrupt(16bit,clock:16mHz)
  TIMSK1 |= (1<<TOIE1);
  TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);  //prescaler: 256 (1.048576s)
  //TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10); // prescaler 1024 (4.194304s)
  TCNT1 = 0;

  sei();  //enable interrupts
  
  //oled display
  _delay_ms(100);  // display initialization
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //I2C adress
  display.clearDisplay(); //clear buffer
  display.setTextColor(WHITE); //textcolor
  display.setTextWrap(false); //disable line change
  display.dim(0);  //brightness max=0 / dim=1
  display.setRotation(0); //rotate display (roation=0,1,2,3)
  _delay_ms(10);
}

uint16_t adc_read(uint8_t selected_pin)
{
  selected_pin &= 0b00000111;   //keep only last 3 bits
  ADMUX = (ADMUX & 0xF8)|selected_pin;  //add this three bits to admux => selects pin
  ADCSRA |= (1<<ADSC);      //start adc
  while(ADCSRA & (1<<ADSC));    //wait till finished (ADSC bit becomes zero)
  return (ADC);       //return results (ADCH & ADCL registers)
}

void measureBat(){
  voltage1 = adc_read(0b00000010)*5.0/1023*1000;
  capacity1 += voltage1/10/3600/1.048576;
  
  voltage2 = adc_read(0b00000011)*5.0/1023*1000;
  capacity2 += voltage2/10/3600/1.048576;
  
  if ( ((voltage1<V_MIN) && (voltage1>V_ZERO) && BUZZER) || ((voltage2<V_MIN) && (voltage2>V_ZERO) && BUZZER) ){
    PORTD |=(1 << PD2); //buzzer on
    PORTB |=(1 << PB5); //led on
  }else{
    PORTD &=~(1 << PD2); //buzzer off
    PORTB &=~(1 << PB5); //led off
    }
}
void displayBat(){
  //PORTB |= (1 << PB5); //led on
  
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.println("mV-mah-"+String(int(counter/60)));
  display.println( String(int(voltage1))+" - "+String(int(capacity1)) );
  display.println( String(int(voltage2))+" - "+String(int(capacity2)) );
  
  display.drawRect(0,0,128,64,1);
  display.display();  // Print everything we set previously
  
  //PORTB &= ~(1 << PB5); //led off
}

//ISR( TIMER2_OVF_vect )      //execute interrupt on timer2 overflow
ISR( TIMER1_OVF_vect )      //execute interrupt on timer1 overflow
{
  asm volatile("nop");
  measureBat();
  counter += 1.048576;
}

int main(void){
  initialize();
  while(1){
    displayBat();
    _delay_ms(10);
    //SMCR = ( (1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE) );  //power down
    //SMCR = ( (0 << SM2) | (0 << SM1) | (0 << SM0) | (1 << SE) );  //idle
  }
  return 0;
}
