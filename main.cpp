#include <Arduino.h>
#include "HomeSpan.h"

// MR24HPC1 simple pin definitions
#define OCCUPIED_PIN D7
#define ACTIVITY_PIN D8


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");
  while(Serial.available() == 0) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  

  homeSpan.setControlPin(D4);
  homeSpan.setStatusPin(D5);

  homeSpan.begin(Category::Sensors, "HomeSpan Occupancy Sensor");


  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();

  new Service::OccupancySensor();
  new Characteristic::OccupancyDetected();

  // Autopolling causes the second core to be used on the ESP32
  //homeSpan.autoPoll();

  Serial.println("Ready.");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  homeSpan.poll();
  }