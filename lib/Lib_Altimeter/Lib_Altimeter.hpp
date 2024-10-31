#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SparkFunBME280.h>


class Altimeter {
public:
  void initialize();

  float getPressure();
  float getTemperature();

  void setReferencePressure();
  void setReferencePressure(float referencePressure_kPa);
  float getAltitude(float temperature_K);

private:
  BME280* _bme280;

  float _pressure_kPa;
  float _referencePressure_kPa;
};
