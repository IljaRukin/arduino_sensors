#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);
  lox.begin();
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); //false: non debug mode
  if (measure.RangeStatus != 4) { 
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  delay(100);
}
