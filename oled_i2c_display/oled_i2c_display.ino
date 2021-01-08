/*
OLED 128x64 I2C with SSD1306 driver

#libraries:
Adafruit GFX (Graphics)
Adafruit SSD1306 (includes <Wire.h> for I2C)

Pins:
 * GND = GND
 * VCC = 5V
 * SCK/SCL = A5
 * SDA = A4

I2C Address: 0x3C 
*/
#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font
//uint8_t sensor;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // Create display

void setup()
{
  delay(100);  // display initialization
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //I2C adress
  
  display.clearDisplay(); //clear buffer
  display.setTextColor(WHITE); //textcolor
  display.setTextWrap(false); //disable line change
  display.dim(0);  //brightness max=0 / dim=1
  display.setRotation(0); //rotate display (roation=0,1,2,3)
}

void loop(){
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println("Hello!");
//  sensor = analogRead(A3);
//  display.println(String(sensor));
//  display.drawLine(0,0,127,63, 1);
//  display.drawPixel(0, 0, 1);
//  display.drawPixel(127, 63, 1);
  display.drawRect(0,0,128,64,1);

  
  display.display();  // Print everything we set previously
  
  delay(2000);
}
