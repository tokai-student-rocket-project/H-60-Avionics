#include <MsgPacketizer.h>
#include "Lib_FRAM.hpp"


FRAM fram0(A1);
FRAM fram1(A2);
FRAM fram2(2);
FRAM fram3(3);


void setup() {
  Serial.begin(115200);
  SPI.begin();

  MsgPacketizer::subscribe_manual(0x0A,
    [&](char ident, uint32_t millis, uint16_t flightTime, uint8_t flightMode, float loggerUsage,
      float accelerationX_mps2, float accelerationY_mps2, float accelerationZ_mps2,
      float gyroscopeX_dps, float gyroscopeY_dps, float gyroscopeZ_dps,
      float magnetometerX_nT, float magnetometerY_nT, float magnetometerZ_nT,
      float roll_deg, float pitch_deg, float yaw_deg,
      float forceX_N, float  jerkX_mps3,
      float pressure_kPa, float altitude_m, float verticalSpeed_mps, float verticalAcceleration_msp2, float estimated, float apogee, bool isFalling,
      float groundVoltage_V, float batteryVoltage_V, float tieVoltage_V, float busVoltage_V,
      float groundCurrent_mA, float batteryCurrent_mA, float tieCurrent_mA, float busCurrent_mA,
      float groundPower_mW, float batteryPower_mW, float tiePower_mW, float busPower_mW,
      float temperatureRegulator1_degC, float temperatureRegulator2_degC, float temperatureRegulator3_degC, float temperatureConduction_degC,
      float temperatureOutside_degC, float temperatureInside_degC,
      float temperatureVentPort_degC, float temperatureTankAtmosphere_degC,
      float sutegomaTime_ms, float sutegomeTaskRate_Hz
      ) {
        Serial.print(ident); Serial.print(",");
        Serial.print(millis); Serial.print(",");
        Serial.print(flightTime); Serial.print(",");
        Serial.print(flightMode); Serial.print(",");
        Serial.print(loggerUsage); Serial.print(",");

        Serial.print(accelerationX_mps2); Serial.print(",");
        Serial.print(accelerationY_mps2); Serial.print(",");
        Serial.print(accelerationZ_mps2); Serial.print(",");

        Serial.print(gyroscopeX_dps); Serial.print(",");
        Serial.print(gyroscopeY_dps); Serial.print(",");
        Serial.print(gyroscopeZ_dps); Serial.print(",");

        Serial.print(magnetometerX_nT); Serial.print(",");
        Serial.print(magnetometerY_nT); Serial.print(",");
        Serial.print(magnetometerZ_nT); Serial.print(",");

        Serial.print(roll_deg); Serial.print(",");
        Serial.print(pitch_deg); Serial.print(",");
        Serial.print(yaw_deg); Serial.print(",");

        Serial.print(forceX_N); Serial.print(",");
        Serial.print(jerkX_mps3); Serial.print(",");

        Serial.print(pressure_kPa); Serial.print(",");
        Serial.print(altitude_m); Serial.print(",");
        Serial.print(verticalSpeed_mps); Serial.print(",");
        Serial.print(verticalAcceleration_msp2); Serial.print(",");
        Serial.print(estimated); Serial.print(",");
        Serial.print(apogee); Serial.print(",");
        Serial.print(isFalling); Serial.print(",");

        Serial.print(groundVoltage_V); Serial.print(",");
        Serial.print(batteryVoltage_V); Serial.print(",");
        Serial.print(tieVoltage_V); Serial.print(",");
        Serial.print(busVoltage_V); Serial.print(",");

        Serial.print(groundCurrent_mA); Serial.print(",");
        Serial.print(batteryCurrent_mA); Serial.print(",");
        Serial.print(tieCurrent_mA); Serial.print(",");
        Serial.print(busCurrent_mA); Serial.print(",");

        Serial.print(groundPower_mW); Serial.print(",");
        Serial.print(batteryPower_mW); Serial.print(",");
        Serial.print(tiePower_mW); Serial.print(",");
        Serial.print(busPower_mW); Serial.print(",");

        Serial.print(temperatureRegulator1_degC); Serial.print(",");
        Serial.print(temperatureRegulator2_degC); Serial.print(",");
        Serial.print(temperatureRegulator3_degC); Serial.print(",");
        Serial.print(temperatureConduction_degC); Serial.print(",");

        Serial.print(temperatureOutside_degC); Serial.print(",");
        Serial.print(temperatureInside_degC); Serial.print(",");

        Serial.print(temperatureVentPort_degC); Serial.print(",");
        Serial.print(temperatureTankAtmosphere_degC); Serial.print(",");

        Serial.print(sutegomaTime_ms); Serial.print(",");
        Serial.print(sutegomeTaskRate_Hz); Serial.print("\n");
    }
  );

  while (!Serial);
  delay(5000);
  printHeader();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  dump(&fram0);
  dump(&fram1);
  dump(&fram2);
  dump(&fram3);
  digitalWrite(LED_BUILTIN, LOW);
}


void loop() {
}


void dump(FRAM* fram) {
  uint8_t data[4096];
  uint32_t size = 0;
  uint32_t writeAddress = 0;

  for (uint32_t address = 0; address < FRAM::LENGTH; address++) {
    data[writeAddress] = fram->read(address);
    size = writeAddress + 1;

    if (data[writeAddress] == 0x00) {
      writeAddress = 0;

      if (data[1] == 0x0A) {
        MsgPacketizer::feed(data, size);
      }
    }
    else {
      writeAddress++;
    }
  }
}


void printHeader() {
  Serial.print("ident"); Serial.print(",");
  Serial.print("millis"); Serial.print(",");
  Serial.print("flightTime"); Serial.print(",");
  Serial.print("flightMode"); Serial.print(",");
  Serial.print("loggerUsage"); Serial.print(",");

  Serial.print("accelerationX_mps2"); Serial.print(",");
  Serial.print("accelerationY_mps2"); Serial.print(",");
  Serial.print("accelerationZ_mps2"); Serial.print(",");

  Serial.print("gyroscopeX_dps"); Serial.print(",");
  Serial.print("gyroscopeY_dps"); Serial.print(",");
  Serial.print("gyroscopeZ_dps"); Serial.print(",");

  Serial.print("magnetometerX_nT"); Serial.print(",");
  Serial.print("magnetometerY_nT"); Serial.print(",");
  Serial.print("magnetometerZ_nT"); Serial.print(",");

  Serial.print("roll_deg"); Serial.print(",");
  Serial.print("pitch_deg"); Serial.print(",");
  Serial.print("yaw_deg"); Serial.print(",");

  Serial.print("forceX_N"); Serial.print(",");
  Serial.print("jerkX_mps3"); Serial.print(",");

  Serial.print("pressure_kPa"); Serial.print(",");
  Serial.print("altitude_m"); Serial.print(",");
  Serial.print("verticalSpeed_mps"); Serial.print(",");
  Serial.print("verticalAcceleration_msp2"); Serial.print(",");
  Serial.print("estimated"); Serial.print(",");
  Serial.print("apogee"); Serial.print(",");
  Serial.print("isFalling"); Serial.print(",");

  Serial.print("groundVoltage_V"); Serial.print(",");
  Serial.print("batteryVoltage_V"); Serial.print(",");
  Serial.print("tieVoltage_V"); Serial.print(",");
  Serial.print("busVoltage_V"); Serial.print(",");

  Serial.print("groundCurrent_mA"); Serial.print(",");
  Serial.print("batteryCurrent_mA"); Serial.print(",");
  Serial.print("tieCurrent_mA"); Serial.print(",");
  Serial.print("busCurrent_mA"); Serial.print(",");

  Serial.print("groundPower_mW"); Serial.print(",");
  Serial.print("batteryPower_mW"); Serial.print(",");
  Serial.print("tiePower_mW"); Serial.print(",");
  Serial.print("busPower_mW"); Serial.print(",");

  Serial.print("temperatureRegulator1_degC"); Serial.print(",");
  Serial.print("temperatureRegulator2_degC"); Serial.print(",");
  Serial.print("temperatureRegulator3_degC"); Serial.print(",");
  Serial.print("temperatureConduction_degC"); Serial.print(",");

  Serial.print("temperatureOutside_degC"); Serial.print(",");
  Serial.print("temperatureInside_degC"); Serial.print(",");

  Serial.print("temperatureVentPort_degC"); Serial.print(",");
  Serial.print("temperatureTankAtmosphere_degC"); Serial.print(",");

  Serial.print("sutegomaTime_ms"); Serial.print(",");
  Serial.print("sutegomeTaskRate_Hz"); Serial.print("\n");
}
