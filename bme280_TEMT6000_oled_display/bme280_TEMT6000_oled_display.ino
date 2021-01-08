
//sleep mode
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)

volatile int toggle = 1;

ISR(WDT_vect)
  /* Watchdog imer Interrupt Service Routine */
  {
  if(toggle == 0)
    { toggle = 1; }
  else
    { Serial.println("WDT Overrun Error!"); }
  }

void enter_sleep(void)
  /* Arduino schlafen legen */
  {
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* Es geht auch SLEEP_MODE_PWR_DOWN */
  sleep_enable();
  power_adc_disable();    /* Analog-Eingaenge abschalten */
  power_spi_disable();    /* SPI abschalten */
  power_timer0_disable(); /* Timer0 abschalten */
  power_timer2_disable(); /* Timer0 abschalten */
  power_twi_disable();    /* TWI abschalten */
  sleep_mode();
  sleep_disable();
  power_all_enable();     /* Komponenten wieder aktivieren */
  }


//bme280 sensor
#include "Seeed_BME280.h"
#include <Wire.h>
BME280 bme280;

//values
float pressure,temperature,humidity;
char txt1[4]={"T:"};
char txt2[4]={"H:"};
char txt3[4]={"P:"};
char char1[8];
char char2[8];
char char3[8];
char unit1[8]={"C"};
char unit2[8]={"%"};
char unit3[8]={"b"};

//display
#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
//#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
//#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Create display

void setup()
{
  //bme280 sensor
  !bme280.init();

  //display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //I2C adress
  delay(100);  // display initialization
  
  display.clearDisplay(); //clear buffer
  display.setTextColor(WHITE); //textcolor
  display.setTextWrap(false); //disable line change
  display.dim(0);  //brightness max=0 / dim=1
  display.setRotation(0); //rotate display (roation=0,1,2,3)
  
  
  /* Setup des Watchdog Timers */
  MCUSR &= ~(1<<WDRF);             /* WDT reset flag loeschen */
  WDTCSR |= (1<<WDCE) | (1<<WDE);  /* WDCE setzen, Zugriff auf Presclaler etc. */
  WDTCSR = 1<<WDP0 | 1<<WDP2;      /* Prescaler au 0.5s*/
  //WDTCSR = 1<<WDP0 | 1<<WDP3;      /* Prescaler auf 8.0 s */
  WDTCSR |= 1<<WDIE;               /* WDT Interrupt freigeben */
}

void loop()
{
  if(toggle)
  {
  /* LED umschalten und wieder schlafen */
  toggle = 0;
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));

  //read data
  temperature = bme280.getTemperature();
  pressure = bme280.getPressure();
  humidity = bme280.getHumidity();
  
  display.clearDisplay();
  display.setFont(NULL);
  //display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(2);
  dtostrf(temperature,3, 1, char1);
  dtostrf(humidity,3, 1, char2);
  dtostrf((pressure/100),5, 1, char3);
  //char strcat ( char char11, char char1);
  display.setCursor(0, 0);
  display.println(txt1);
  display.setCursor(25, 0);
  display.println(char1);
  display.setCursor(105, 0);
  display.println(unit1);
  display.setCursor(0, 20);
  display.println(txt2);
  display.setCursor(25, 20);
  display.println(char2);
  display.setCursor(105, 20);
  display.println(unit2);
  display.setCursor(0, 40);
  display.println(txt3);
  display.setCursor(25, 40);
  display.println(char3);
  display.setCursor(105, 40);
  display.println(unit3);
  int brightness = int(analogRead(A0)/8);
  display.fillRect(0,60,brightness,64, 1);

  
  display.display();  // Print everything we set previously
  
  
  enter_sleep();
  }

}
