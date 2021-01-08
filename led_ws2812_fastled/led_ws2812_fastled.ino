#include <FastLED.h>
#define LED_PIN 8
#define NUM_LEDS 1
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
CRGB leds[NUM_LEDS];

uint8_t start_color = 0;
int8_t delta = 8;
int i = 0;
long j = 1020;

void setup(){
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

int sawtooth(long arg){
  //repeat
  while(arg>1530){
    arg=arg-1530;
  }

  //sawtooth
  if(arg>-1 && arg<256){
    return arg;
  } else if(arg>255 && arg<766){
    return 255;
  } else if(arg>765 && arg<1021){
    return (1020-arg);
  } else{
  return 0;
  }
}

void loop(){
  
/////solid color
  /*fill_solid( leds, NUM_LEDS, CRGB(50,0,200));
  FastLED.show();*/

/////solid rainbow fade
  delay(500);
  if(j>(1530+1020)){//avoid overflow
    j=1020;
  }
  j=j+51;
  fill_solid( leds, NUM_LEDS, CRGB(sawtooth(j),sawtooth(j-510),sawtooth(j-1020)));
  FastLED.show();
  
/////rainbow color
  /*fill_rainbow(leds, NUM_LEDS, start_color, delta);
  FastLED.show();*/
  
/////rainbow running
  /*fill_rainbow(leds, NUM_LEDS, start_color, delta );
  FastLED.show();
  delay(20);
  start_color = start_color + 1;
  if(start_color>255){
    start_color = 0;
  }*/
  
/////loading rainbow
  /*
  leds[i] = CRGB(255, 0, 0);
  FastLED.show();
  delay(50);
  
  leds[i+1] = CRGB(0, 255, 0);
  FastLED.show();
  delay(50);
  
  leds[i+2] = CRGB(0, 0, 255);
  FastLED.show();
  delay(50);    
  
  i=i+3;
  if(i>57){
    i = 0;
  }
  */

/////running 3 led
  /*leds[i-1] = CRGB(0, 0, 0);
  leds[i] = CRGB(255, 0, 0);  //green
  leds[i+1] = CRGB(0, 255, 0);//red
  leds[i+2] = CRGB(0, 0, 255);//blue
  FastLED.show();
  delay(100);
  
  if(i>=57){
    i = 0;
    leds[57] = CRGB(0, 0, 0);
    leds[58] = CRGB(0, 0, 0);
    leds[59] = CRGB(0, 0, 0);
    FastLED.show();
  }
  i=i+1;
  */

}
