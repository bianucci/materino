/**
*
*	Materino Project. Hosted on https://github.com/bianucci/materino
*
*	
*
*/





#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// relay which turns on/off heater is plugged into pin 12 on the arduino
#define RELAY 12

// for testing purposes the desired water temperature is 30 degrees
#define DESIRED_TEMP 30
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// global heating state. i know this is shitty, I will improve asap.
boolean heating = false;
 
void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);   // turn off the heater 
}
 

// counters are used to make sure the measured value was at least measured twice
// just to avoid any inconsistencies at measuring
int tempLowCounter = 0;
int tempHighCounter = 0;


void loop(void)
{

  // read temperature
  sensors.requestTemperatures();  
  int temp = sensors.getTempCByIndex(0);
  Serial.print(temp);
  Serial.print("\n");
  
  // TODO replace global heating state by reading current state of pin
  if(temp < DESIRED_TEMP){
      tempLowCounter++;
      if( heating == false && tempLowCounter > 2) {
          Serial.print("TempTooLow! Turn ON heater.\n");

          digitalWrite(RELAY, LOW);   // turn on the heater
          heating = true;
          tempLowCounter = 0;
      }
  } else if (temp >= DESIRED_TEMP) {
      tempHighCounter++;
      if( heating == true && tempHighCounter > 2) {
          Serial.print("TempTooHIGH! Turn OFF heater.\n");
          digitalWrite(RELAY, HIGH);   // turn off the heater
          heating = false;
          tempHighCounter = 0;
      }
  }
}
