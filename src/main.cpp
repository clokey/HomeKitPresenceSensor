#include <Arduino.h>
#include "HomeSpan.h"
#include "DEV_MR24HPC1_Sensor.h"

// MR24HPC1 simple pin definitions
int OCCUPIED_PIN = D7;
int ACTIVITY_PIN = D8;
int status_pin = D5;
int control_pin = D4;


void setup() {
  // put your setup code here, to run once:
  Serial.begin();
  Serial.println("Starting up...");

  homeSpan.setControlPin(control_pin);
  homeSpan.setStatusPin(status_pin);

  homeSpan.begin(Category::Sensors, "Occupancy Sensor");

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Manufacturer("Clokey");
  new Characteristic::Model("MR24HPC1");
  new Characteristic::Name("Occupancy Sensor");
  new DEV_MR24HPC1_Sensor(OCCUPIED_PIN, ACTIVITY_PIN);


  // Autopolling causes the second core to be used on the ESP32
  homeSpan.autoPoll();

  Serial.println("Ready.");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //homeSpan.poll();
}