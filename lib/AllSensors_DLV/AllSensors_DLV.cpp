/*
This software is licensed under the Revised (3-clause) BSD license as follows:
Copyright (c) 2018, Jeremy Cole <jeremy@jcole.us>
All rights reserved.
See the LICENSE file for more details.
*/

//ELVH pressure sensors

#include "AllSensors_DLV.h"

AllSensors_DLV::AllSensors_DLV(TwoWire *bus, SensorType type, float pressure_max) :
  pressure_unit(PressureUnit::mmHg),
  temperature_unit(TemperatureUnit::CELSIUS)
{
  this->bus = bus;
  this->type = type;
  this->pressure_max = pressure_max;
  
  //omri's addition:
  bus->begin();

  switch (type) {
    case GAUGE:
    case ABSOLUTE:
      pressure_zero_ref = 1638;
      pressure_range = pressure_max;
      break;
    case DIFFERENTIAL:
      pressure_zero_ref = 8192;
      pressure_range = pressure_max * 2;
      break;
  }
}

bool AllSensors_DLV::readData() {
  bus->requestFrom(I2C_ADDRESS, (uint8_t) READ_LENGTH);
  for (int i=0; i < READ_LENGTH; i++) {
    raw_data[i] = bus->read();
  }
  bus->endTransmission();
  
  status = extractStatus();
  raw_p = extractIntegerPressure();
  raw_t = extractIntegerTemperature();
  
  // pressure = convertPressurefrompsi(transferPressure(raw_p));
  pressure = convertPressurefrommbar(transferPressure(raw_p));
  temperature = convertTemperature(transferTemperature(raw_t));

  return status == Status::ERROR;
}