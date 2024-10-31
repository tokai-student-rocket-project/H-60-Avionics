#include "Lib_Altimeter.hpp"


void Altimeter::initialize() {
  _bme280 = new BME280();
  _bme280->setI2CAddress(0x76);
  _bme280->beginI2C();
}


float Altimeter::getPressure() {
  // BME280の仕様で気圧計測の前に気温計測を行う必要がある
  // データシートの 3.4 Measurement Flow 参照
  _bme280->readTempC();

  // readFloatPressure()は[Pa]で帰ってくるので1000で割って[kPa]にする
  _pressure_kPa = _bme280->readFloatPressure() / 1000.0;
  return _pressure_kPa;
}


float Altimeter::getTemperature() {
  return _bme280->readTempC();
}


void Altimeter::setReferencePressure() {
  setReferencePressure(getPressure());
}


void Altimeter::setReferencePressure(float referencePressure_kPa) {
  _referencePressure_kPa = referencePressure_kPa;
}


float Altimeter::getAltitude(float temperature_K) {
  float altitude_m = (((pow((_referencePressure_kPa / _pressure_kPa), (1.0 / 5.257))) - 1.0) * temperature_K) / 0.0065;
  return altitude_m;
}
