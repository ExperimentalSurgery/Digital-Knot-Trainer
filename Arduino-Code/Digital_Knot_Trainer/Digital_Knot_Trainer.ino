#include <HX711.h>

  const int PIN_CONTACT_LED_GREEN = 11;

  const int PIN_WEIGHT_1_LED_RED = 10;
  const int PIN_WEIGHT_1_LED_GREEN = 9;

  const int PIN_WEIGHT_2_LED_RED = 6;
  const int PIN_WEIGHT_2_LED_GREEN = 5;

  const int PIN_LOADCELL_DATA = 14;
  const int PIN_LOADCELL_SCK = 13;

  const int PIN_CONTACT_SENSOR = 12;
  const int PIN_CONTACT_GROUND = 8;

  //If you use a RGB LED with a common anode (+), set to true, if you use a common cathode (-) LED, set to false
  const bool commonAnodeLED = true;


  HX711 scale;

  //Indiviual for each cell, use the Calibrate_Knottrainer.ino script to determine this factor
  const int Loadcell_Calibration_Factor = -215;

  //The "ideal" tension weight in grams on the trainer, should be a low weight (No tension)
  const int minWeight = 20;

  //The falloff tension weight on the trainer, should start around 300-400g
  const int fallOffWeight = -400;

  //The maximum tension weight on the trainer, should be around 500g
  const int maxWeight = -500;



void setup() {

  Serial.begin(57600);
  
  pinMode(PIN_CONTACT_LED_GREEN,   OUTPUT);

  pinMode(PIN_WEIGHT_1_LED_GREEN,   OUTPUT);
  pinMode(PIN_WEIGHT_1_LED_RED, OUTPUT);

  pinMode(PIN_WEIGHT_2_LED_GREEN,   OUTPUT);
  pinMode(PIN_WEIGHT_2_LED_RED, OUTPUT);

  pinMode(PIN_LOADCELL_DATA,   INPUT);
  pinMode(PIN_LOADCELL_SCK, INPUT);

  pinMode(PIN_CONTACT_SENSOR,  INPUT_PULLUP);
  pinMode(PIN_CONTACT_GROUND, OUTPUT);


  SetWeightLed(200, 200);
  SetContactLed(0);

  scale.begin(PIN_LOADCELL_DATA, PIN_LOADCELL_SCK);
  scale.set_scale(Loadcell_Calibration_Factor);
  scale.tare(); 

  SetWeightLed(0, 100);
  SetContactLed(100);

  delay(200);

  SetWeightLed(0, 0);
  SetContactLed(0);

}

void loop() { 

  bool contact = CheckWireContact();
  int grams = CheckWeight();

  Serial.print(contact);
  Serial.print(";"); 
  Serial.print(grams);
  Serial.print("\r"); 
  Serial.print("\n"); 
}

int CheckWeight()
{
  int grams = scale.get_units(1);

  if(grams > minWeight)
    grams = minWeight;

  if(grams < maxWeight)
    grams = maxWeight;

  int redStrength = map(grams, fallOffWeight, maxWeight, 0, 255);
  int greenStrength = map(grams, fallOffWeight, maxWeight, 255, 0);

  SetWeightLed(redStrength, greenStrength);  

  return grams;
}

bool CheckWireContact()
{
    digitalWrite(PIN_CONTACT_GROUND, LOW);
    bool contact = digitalRead(PIN_CONTACT_SENSOR);

    if(contact)
      SetContactLed(0);

    else
      SetContactLed(255);

    return contact;
}

void SetContactLed(byte green)
{
   if(commonAnodeLED)
    green = 255 - green;

  analogWrite(PIN_CONTACT_LED_GREEN, green);

}

void SetWeightLed(byte red, byte green)
{
  if(commonAnodeLED)
  {
    red = 255 - red;
    green = 255 - green;
  }

  analogWrite(PIN_WEIGHT_1_LED_GREEN,  green);
  analogWrite(PIN_WEIGHT_1_LED_RED, red);

  analogWrite(PIN_WEIGHT_2_LED_GREEN,  green);
  analogWrite(PIN_WEIGHT_2_LED_RED, red);
}
