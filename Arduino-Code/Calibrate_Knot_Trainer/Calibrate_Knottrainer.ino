#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 13;

//Enter your known wait here, measure it with another scale
int knownWeightInGrams = 124;

HX711 scale;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {

  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.println("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.println("Result: ");
    Serial.println(reading);
    Serial.println("Calibration Factor: ");
    Serial.println(reading / knownWeightInGrams);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
}
