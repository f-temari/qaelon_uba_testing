#include <Arduino.h>
#include <Wire.h>
#include <AllSensors_DLV.h>

AllSensors_DLV elvh_pressure(&Wire, AllSensors_DLV::SensorType::GAUGE, 50.0);
float elvh_pressure_readings = 0.0;

// pin usage
const int pin = A0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, INPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  float raw_data = analogRead(pin);
  raw_data = raw_data * (5.0 / 1023);
  float pressure_in_inH2O = (raw_data - 0.25) / 0.375;  // in inH20 (water inch);
  float pressure_in_mmHg = pressure_in_inH2O * 1.86832; // in mmhg

  elvh_pressure.readData();
  elvh_pressure_readings = elvh_pressure.pressure;

  Serial.print(elvh_pressure_readings,2);
  Serial.print(",");
  Serial.println(pressure_in_mmHg, 2);

  delay(500);
}