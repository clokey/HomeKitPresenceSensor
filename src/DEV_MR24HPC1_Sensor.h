#include "HomeSpan.h"

struct DEV_MR24HPC1_Sensor : Service::OccupancySensor {                

  SpanCharacteristic *occupied;                                         // reference to the Occupied Characteristic
  int occupancyPin;

  DEV_MR24HPC1_Sensor(int occupancyPin) : Service::OccupancySensor() {

    this->occupancyPin = occupancyPin;


    pinMode(occupancyPin, INPUT);

    boolean occupied = digitalRead(occupancyPin);
    occupied = new Characteristic::OccupancyDetected(occupied);              // instantiate the OccupancyDetected Characteristic

  } // end constructor

  void loop() {
    boolean occupancy = digitalRead(occupancyPin);
    if (occupancy != occupied->getVal()) {
      occupied->setVal(occupancy);
      if (occupancy == true) {
        char c[64];
        sprintf(c, "Occupied\n");
        LOG1(c);
      }
    }
  }
};