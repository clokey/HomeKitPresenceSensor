struct DEV_MR24HPC1_Sensor : Service::OccupancySensor {                       // OccupancySensor sensor

  SpanCharacteristic *occupied;                                         // reference to the MotionDetected Characteristic
  int sensorPin;                                                        // pin number of the sensor

  DEV_MR24HPC1_Sensor(int sensorPin) : Service::OccupancySensor() {

    this->sensorPin = sensorPin;
    pinMode(sensorPin, INPUT);
    boolean motion = digitalRead(sensorPin);
    occupied = new Characteristic::OccupancyDetected(motion);              // instantiate the MotionDetected Characteristic

  } // end constructor

  void loop() {
    boolean motion = digitalRead(sensorPin);
    if (motion != movement->getVal()) {
      movement->setVal(motion);
      if (motion == true) {
        char c[64];
        sprintf(c, "Motion was detected\n");
        LOG1(c);
      }
    }
  }
};