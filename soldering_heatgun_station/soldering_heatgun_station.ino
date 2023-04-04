/*
  ###OLED 128x64 I2C with SSD1306 driver
  #libraries:
  Adafruit GFX (Graphics)
  Adafruit SSD1306 (includes <Wire.h> for I2C)
  Pins:
   GND = GND
   VCC = 5V
   SCK/SCL = A5
   SDA = A4
  I2C Address: 0x3C

  #temperature sensor
   temp soldering iron = A2 (PC2)
   temp heat gun = A3 (PC3)
  #poti
   poti1 soldering iron = A0 (PC0)
   poti2 heatgun = A1 (PC1)
   poti3 = fan speed (analog controll without arduino)
  #on/off switch
   on/off soldering iron = D5 (PD5)
   on/off heatgun = D4 (PD4)
  #power
   power soldering iron = D3 (PD3)
   power heat gun = D2 (PD2)
   power fan = D6 (PD6) --- disabled
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

uint16_t set_temp_solder = 0;
uint16_t got_temp_solder = 0;
uint16_t set_temp_heatgun = 0;
uint16_t got_temp_heatgun = 0;

void initialize(void) {
  //led
  DDRB |= (1 << PB5); //led
  PORTB &= ~(1 << PB5); //output off

  //PD2-PD5
  DDRD |= 0b01001100; //output
  PORTD &= ~(0b01001100); //output off
  DDRD &= ~(0b00110000); //input
  //PORTD &= ~(0b00110000); //pull-up off
  PORTD |= (0b00110000); //pull-up on
  
  //ADC
  DDRC &= ~0b00001111;  //adc input pins PC0-PC3
  //  MCUCR |= (1 << PUD);  //disable all pull-ups (conflict with i2c)
  DIDR0 |= (1 << ADC2D) | (1 << ADC3D); //disable digital input
  ADMUX |= (0 << REFS1) | (1 << REFS0); //Vref from pin Aref
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //enable adc, prescaler 128

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
  ADMUX = (ADMUX & 0xF8) | selected_pin; //add this three bits to admux => selects pin
  ADCSRA |= (1 << ADSC);    //start adc
  while (ADCSRA & (1 << ADSC)); //wait till finished (ADSC bit becomes zero)
  return (ADC);       //return results (ADCH & ADCL registers)
}

void process() {
  PORTB |= (1 << PB5); //led on
  
  //soldering
  if ((PIND & 0b00100000)<10){
    set_temp_solder = adc_read(0b00000000); //PC0
    got_temp_solder = got_temp_solder*0.8 + 0.2*adc_read(0b00000010); //PC2
    if (set_temp_solder>got_temp_solder){
      PORTD &= ~(1 << PD3); //heating soldering iron
    }else{
      PORTD |= (1 << PD3); //cooling soldering iron
    }
  }
  else {
    set_temp_solder = 0;
    got_temp_solder = got_temp_solder*0.8 + 0.2*adc_read(0b00000010); //PC2
    PORTD |= (1 << PD3); //cooling soldering iron
  }
  
  //heatgun
  if ((PIND & 0b00010000)<10){
    set_temp_heatgun = adc_read(0b00000001); //PC1
    got_temp_heatgun = got_temp_heatgun*0.8 + 0.2*adc_read(0b0000011);  //PC3
    if (set_temp_heatgun>got_temp_heatgun){
      PORTD &= ~(1 << PD2); //heating heatgun
      //PORTD |= (1 << PD6); //start fan
    }else{
      PORTD |= (1 << PD2); //cooling heatgun
      //PORTD &= ~(1 << PD6); //stop fan
    }
  }
  else {
    set_temp_heatgun = 0;
    got_temp_heatgun = got_temp_heatgun*0.8 + 0.2*adc_read(0b0000011);  //PC3
    PORTD |= (1 << PD2); //cooling heatgun
    //PORTD &= ~(1 << PD6); //stop fan
  }

  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.println("iron-gun");
  //display.println(String((PIND & 0b00100000)>0)+"-"+String((PIND & 0b00010000)>0));
  display.println( "set:" + String(set_temp_solder) + "-" + String(set_temp_heatgun) );
  display.println( "got:" + String(got_temp_solder) + "-" + String(got_temp_heatgun) );

  display.drawRect(0, 0, 128, 64, 1);
  display.display();  // Print everything we set previously

  PORTB &= ~(1 << PB5); //led off
}

int main(void) {
  initialize();
  while (1) {
    _delay_ms(20);
    process();
  }
  return 0;
}
