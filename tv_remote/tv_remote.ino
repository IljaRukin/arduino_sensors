//arduino TV remote

//#define IR_RECEIVE_PIN 2 //PD2 (for receive dump)
#define IR_SEND_PIN 3 //PD3

#include <Arduino.h>
#include <IRremote.hpp>

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#include <IRremote.hpp> // include the library

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    /*
     * The IR library setup. That's all!
     */
//    IrSender.begin();
}

int sAddress = 0x8;
int sCommand = 0;
int sRepeats = 1;

void loop() {
    //if(Serial.available() != 0) {
    //sCommand = Serial.readStringUntil('\n').toInt();
    for(int sCommand=0; sCommand<80; sCommand++){
    Serial.println();
    Serial.print(F("send: address=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(", command=0x"));
    Serial.print(sCommand, HEX);
    Serial.print(F(", repeats="));
    Serial.print(sRepeats);
    Serial.println();
    Serial.flush();

    Serial.println("sendNEC");
    Serial.flush();
    IrSender.sendNEC(sAddress & 0xFFF, sCommand, sRepeats);
    delay(500);
    Serial.println("sendJVC");
    Serial.flush();
    IrSender.sendJVC(sAddress & 0xFFF, sCommand, sRepeats);
    delay(500);
//    Serial.println("sendSAMSUNG");
//    Serial.flush();
//    IrSender.sendSAMSUNG(sAddress & 0xFFF, sCommand, sRepeats);
//    delay(500);
    Serial.println("sendSony");
    Serial.flush();
    IrSender.sendSony(sAddress & 0xFFF, sCommand, sRepeats);
    delay(500);
    Serial.println("sendPanasonic");
    Serial.flush();
    IrSender.sendPanasonic(sAddress & 0xFFF, sCommand, sRepeats);
    delay(500);
    }
    //delay(500);
}
