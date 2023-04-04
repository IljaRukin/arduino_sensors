#include <Arduino.h>
#include <IRremote.hpp>

//#define IR_RECEIVE_PIN 2 //PD2 (for receive dump)
#define IR_SEND_PIN 3 //PD3

/*
 * commands
on-off - 3D
vol up - 20
vol down - 21
silent - 32
AV - 5
TV - 30
1,2,3,4,5,6,7,8,9,0 - 10-19
prog up - 34
prog down - 35
ok - 49
*/

void setup() {
    IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
}

uint16_t sAddress = 0x8;
uint8_t sCommand = 0x32;
uint8_t sRepeats = 0;

void loop() {
    IrSender.sendPanasonic(sAddress & 0xFFF, sCommand, sRepeats);
    delay(500);
}
