/*epaper 
 *2.13inch black/white/red
 *res 212x104 landscape (right to left/top to bottom)
 *1bit=1pixel send in 1byte=8pixel packets
 *VCC --> 3V3
 *GND  --> GND
 *DC   --> D9
 *SDI  --> D11
 *CS   --> D10
 *CLK  --> D13
 *BUSY --> D7
 *Implemented: display image
 *TODO: Text with https://github.com/olikraus/u8g2/wiki/setup_tutorial
 */

#include "imagedata.c"
#include <SPI.h>

const uint8_t CS = 10;
const uint8_t DC = 9;
const uint8_t BUSY = 7;
int isbusy = 0;
int i;
char databyte;

void sendSPIbyte(char data){
  digitalWrite(CS,LOW);
  SPI.transfer(data);
  digitalWrite(CS,HIGH);
  }

void waitBUSY(){
  isbusy = 0;
  //wait untill BUSY=high
  while(isbusy == 0){
    isbusy = digitalRead(BUSY);
    }
  }

void startEPAPER(){
  digitalWrite(DC, LOW);
  sendSPIbyte(0x06);
  digitalWrite(DC, HIGH);
  sendSPIbyte(0x17);
  sendSPIbyte(0x17);
  sendSPIbyte(0x17);
  digitalWrite(DC, LOW);
  sendSPIbyte(0x04);
  }

void setupEPAPER(){
  digitalWrite(DC, LOW);
  sendSPIbyte(0x00);
  digitalWrite(DC, HIGH);
  sendSPIbyte(0x8F);
  digitalWrite(DC, LOW);
  sendSPIbyte(0x50);
  digitalWrite(DC, HIGH);
  sendSPIbyte(0x37);
  digitalWrite(DC, LOW);
  sendSPIbyte(0x61);
  digitalWrite(DC, HIGH);
  sendSPIbyte(0x68);
  sendSPIbyte(0x00);
  sendSPIbyte(0xD4);
  }

void clearEPAPER(){
  digitalWrite(DC, LOW);
  sendSPIbyte(0x10);
  digitalWrite(DC, HIGH);
  //black data
  for(i=0;i<2756;i++){
    sendSPIbyte(0xFF);
    }
  digitalWrite(DC, LOW);
  sendSPIbyte(0x13);
  digitalWrite(DC, HIGH);
  //red data
  for(i=0;i<2756;i++){
    sendSPIbyte(0xFF);
    }
  }

void imageEPAPER(const unsigned char* IMAGE_BLACK,const unsigned char* IMAGE_RED){
  digitalWrite(DC, LOW);
  sendSPIbyte(0x10);
  digitalWrite(DC, HIGH);
  //black data
  for(i=0;i<2756;i++){
    databyte = pgm_read_byte(IMAGE_BLACK+i);
    sendSPIbyte(databyte);
    }
  digitalWrite(DC, LOW);
  sendSPIbyte(0x13);
  digitalWrite(DC, HIGH);
  //red data
  for(i=0;i<2756;i++){
    databyte = pgm_read_byte(IMAGE_RED+i);
    sendSPIbyte(databyte);
    }
  }

void displayEPAPER(){
  digitalWrite(DC, LOW);
  sendSPIbyte(0x12);
  digitalWrite(DC, HIGH);
  }

void poweroffEPAPER(){
  sendSPIbyte(0x02);
  }

void sleepEPAPER(){
  sendSPIbyte(0x07);
  sendSPIbyte(0xA5);
  }

void setup() {
  //setup PINS
  pinMode(CS, OUTPUT); //low for spi transfer
  digitalWrite(CS, HIGH);
  pinMode(DC, OUTPUT); //low=command / high=data
  digitalWrite(DC, HIGH);
  pinMode(BUSY, INPUT);

  //setup SPI
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  
  startEPAPER();
  waitBUSY();
  setupEPAPER();
  clearEPAPER();
  displayEPAPER();
  waitBUSY();
  imageEPAPER(IMAGE_BLACK,IMAGE_RED);
  displayEPAPER();
  waitBUSY();
  poweroffEPAPER();
  waitBUSY();
  sleepEPAPER();

}

void loop() {
}
