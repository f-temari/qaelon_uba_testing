#include <Arduino.h>
#include <Wire.h>
#include <AllSensors_DLV.h>

#define IN_PIN A0
#define WINDOW_SIZE 10

AllSensors_DLV elvh_pressure(&Wire, AllSensors_DLV::SensorType::GAUGE, 50.0);
float elvh_pressure_readings = 0.0;
float elvh_temp_readings = 0.0;

int INDEX = 0;
float VALUE = 0;
float SUM = 0;
float READINGS[WINDOW_SIZE];
float AVERAGED = 0;

int INDEX_elvh_temperature = 0;
float VALUE_elvh_temperature = 0;
float SUM_elvh_temperature = 0;
float READINGS_elvh_temperature[WINDOW_SIZE];
float AVERAGED_elvh_temperature = 0;

int INDEX_elvh_pressure = 0;
float VALUE_elvh_pressure = 0;
float SUM_elvh_pressure = 0;
float READINGS_elvh_pressure[WINDOW_SIZE];
float AVERAGED_elvh_pressure = 0;

// pin usage
const int pin = IN_PIN;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, INPUT);
  // Setting the temperature and pressure units
  elvh_pressure.setPressureUnit(AllSensors_DLV::PressureUnit::mmHg);
  elvh_pressure.setTemperatureUnit(AllSensors_DLV::TemperatureUnit::CELSIUS);
}

void loop()
{
  // put your main code here, to run repeatedly:
  float raw_data = analogRead(pin);
  raw_data = raw_data * (5.0 / 1023);
  float pressure_in_inH2O = (raw_data - 0.25) / 0.375;  // in inH20 (water inch);
  float pressure_in_mmHg = pressure_in_inH2O * 1.86832; // in mmhg

  // elvh sensor readings
  elvh_pressure.readData();
  elvh_temp_readings = elvh_pressure.temperature;
  elvh_pressure_readings = elvh_pressure.pressure;

  // Running average for LPDR Pressure sensor
  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  VALUE = pressure_in_mmHg;          // Read the next sensor value
  READINGS[INDEX] = VALUE;           // Add the newest reading to the window
  SUM = SUM + VALUE;                 // Add the newest reading to the sum
  INDEX = (INDEX + 1) % WINDOW_SIZE; // Increment the index, and wrap to 0 if it exceeds the window size
  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result

  // Running average for the elvh temperature sensor
  SUM_elvh_temperature = SUM_elvh_temperature - READINGS_elvh_temperature[INDEX_elvh_temperature]; // Remove the oldest entry from the sum
  VALUE_elvh_temperature = elvh_temp_readings;                                                     // Read the next sensor value
  READINGS_elvh_temperature[INDEX_elvh_temperature] = VALUE_elvh_temperature;                      // Add the newest reading to the window
  SUM_elvh_temperature = SUM_elvh_temperature + VALUE_elvh_temperature;                            // Add the newest reading to the sum
  INDEX_elvh_temperature = (INDEX_elvh_temperature + 1) % WINDOW_SIZE;                             // Increment the index, and wrap to 0 if it exceeds the window size
  AVERAGED_elvh_temperature = SUM_elvh_temperature / WINDOW_SIZE;                                  // Divide the sum of the window by the window size for the result*

  // Running average for the elvh pressure sensor
  SUM_elvh_pressure = SUM_elvh_pressure - READINGS_elvh_pressure[INDEX_elvh_pressure]; // Remove the oldest entry from the sum
  VALUE_elvh_pressure = elvh_pressure_readings;                                        // Read the next sensor value
  READINGS_elvh_pressure[INDEX_elvh_pressure] = VALUE_elvh_pressure;                   // Add the newest reading to the window
  SUM_elvh_pressure = SUM_elvh_pressure + VALUE_elvh_pressure;                         // Add the newest reading to the sum
  INDEX_elvh_pressure = (INDEX_elvh_pressure + 1) % WINDOW_SIZE;                       // Increment the index, and wrap to 0 if it exceeds the window size
  AVERAGED_elvh_pressure = SUM_elvh_pressure / WINDOW_SIZE;                            // Divide the sum of the window by the window size for the result

  // Serial printing
  Serial.print(AVERAGED_elvh_temperature,1);
  Serial.print(",");
  Serial.print(AVERAGED_elvh_pressure,2);
  Serial.print(",");
  Serial.println(AVERAGED,2);

  delay(1000);
}