#include <MsgPacketizer.h>
#include "Lib_FRAM.hpp"

FRAM fram0(A1);
FRAM fram1(A2);
FRAM fram2(2);
FRAM fram3(3);

void dump(FRAM *fram)
{
  uint8_t data[4096];
  uint32_t size = 0;
  uint32_t writeAddress = 0;

  for (uint32_t address = 0; address < FRAM::LENGTH; address++)
  {
    data[writeAddress] = fram->read(address);
    size = writeAddress + 1;

    if (data[writeAddress] == 0x00)
    {
      writeAddress = 0;

      if (data[1] == 0x0A)
      {
        MsgPacketizer::feed(data, size);
      }
    }
    else
    {
      writeAddress++;
    }
  }
}

void printHeader()
{
  Serial.print(F("ident"));
  Serial.print(F(","));
  Serial.print(F("millis"));
  Serial.print(F(","));
  Serial.print(F("flightTime"));
  Serial.print(F(","));
  Serial.print(F("flightMode"));
  Serial.print(F(","));
  Serial.print(F("loggerUsage"));
  Serial.print(F(","));

  Serial.print(F("accelerationX_mps2"));
  Serial.print(F(","));
  Serial.print(F("accelerationY_mps2"));
  Serial.print(F(","));
  Serial.print(F("accelerationZ_mps2"));
  Serial.print(F(","));

  Serial.print(F("gyroscopeX_dps"));
  Serial.print(F(","));
  Serial.print(F("gyroscopeY_dps"));
  Serial.print(F(","));
  Serial.print(F("gyroscopeZ_dps"));
  Serial.print(F(","));

  Serial.print(F("magnetometerX_nT"));
  Serial.print(F(","));
  Serial.print(F("magnetometerY_nT"));
  Serial.print(F(","));
  Serial.print(F("magnetometerZ_nT"));
  Serial.print(F(","));

  Serial.print(F("roll_deg"));
  Serial.print(F(","));
  Serial.print(F("pitch_deg"));
  Serial.print(F(","));
  Serial.print(F("yaw_deg"));
  Serial.print(F(","));

  Serial.print(F("forceX_N"));
  Serial.print(F(","));
  Serial.print(F("jerkX_mps3"));
  Serial.print(F(","));

  Serial.print(F("pressure_kPa"));
  Serial.print(F(","));
  Serial.print(F("altitude_m"));
  Serial.print(F(","));
  Serial.print(F("verticalSpeed_mps"));
  Serial.print(F(","));
  Serial.print(F("verticalAcceleration_msp2"));
  Serial.print(F(","));
  Serial.print(F("estimated"));
  Serial.print(F(","));
  Serial.print(F("apogee"));
  Serial.print(F(","));
  Serial.print(F("isFalling"));
  Serial.print(F(","));

  Serial.print(F("groundVoltage_V"));
  Serial.print(F(","));
  Serial.print(F("batteryVoltage_V"));
  Serial.print(F(","));
  Serial.print(F("tieVoltage_V"));
  Serial.print(F(","));
  Serial.print(F("busVoltage_V"));
  Serial.print(F(","));

  Serial.print(F("groundCurrent_mA"));
  Serial.print(F(","));
  Serial.print(F("batteryCurrent_mA"));
  Serial.print(F(","));
  Serial.print(F("tieCurrent_mA"));
  Serial.print(F(","));
  Serial.print(F("busCurrent_mA"));
  Serial.print(F(","));

  Serial.print(F("groundPower_mW"));
  Serial.print(F(","));
  Serial.print(F("batteryPower_mW"));
  Serial.print(F(","));
  Serial.print(F("tiePower_mW"));
  Serial.print(F(","));
  Serial.print(F("busPower_mW"));
  Serial.print(F(","));

  Serial.print(F("temperatureRegulator1_degC"));
  Serial.print(F(","));
  Serial.print(F("temperatureRegulator2_degC"));
  Serial.print(F(","));
  Serial.print(F("temperatureRegulator3_degC"));
  Serial.print(F(","));
  Serial.print(F("temperatureConduction_degC"));
  Serial.print(F(","));

  Serial.print(F("temperatureOutside_degC"));
  Serial.print(F(","));
  Serial.print(F("temperatureInside_degC"));
  Serial.print(F(","));

  Serial.print(F("temperatureVentPort_degC"));
  Serial.print(F(","));
  Serial.print(F("temperatureTankAtmosphere_degC"));
  Serial.print(F("\n"));
}

void setup()
{
  Serial.begin(115200);
  SPI.begin();

  MsgPacketizer::subscribe_manual(0x0A,
                                  [&](char ident, uint32_t millis, uint16_t flightTime, uint8_t flightMode, float loggerUsage,
                                      float accelerationX_mps2, float accelerationY_mps2, float accelerationZ_mps2,
                                      float gyroscopeX_dps, float gyroscopeY_dps, float gyroscopeZ_dps,
                                      float magnetometerX_nT, float magnetometerY_nT, float magnetometerZ_nT,
                                      float roll_deg, float pitch_deg, float yaw_deg,
                                      float forceX_N, float jerkX_mps3,
                                      float pressure_kPa, float altitude_m, float verticalSpeed_mps, float verticalAcceleration_msp2, float estimated, float apogee, bool isFalling,
                                      float groundVoltage_V, float batteryVoltage_V, float tieVoltage_V, float busVoltage_V,
                                      float groundCurrent_mA, float batteryCurrent_mA, float tieCurrent_mA, float busCurrent_mA,
                                      float groundPower_mW, float batteryPower_mW, float tiePower_mW, float busPower_mW,
                                      float temperatureRegulator1_degC, float temperatureRegulator2_degC, float temperatureRegulator3_degC, float temperatureConduction_degC,
                                      float temperatureOutside_degC, float temperatureInside_degC,
                                      float temperatureVentPort_degC, float temperatureTankAtmosphere_degC
                                      )
                                  {
                                    Serial.print(ident);
                                    Serial.print(F(","));
                                    Serial.print(millis);
                                    Serial.print(F(","));
                                    Serial.print(flightTime);
                                    Serial.print(F(","));
                                    Serial.print(flightMode);
                                    Serial.print(F(","));
                                    Serial.print(loggerUsage);
                                    Serial.print(F(","));

                                    Serial.print(accelerationX_mps2);
                                    Serial.print(F(","));
                                    Serial.print(accelerationY_mps2);
                                    Serial.print(F(","));
                                    Serial.print(accelerationZ_mps2);
                                    Serial.print(F(","));

                                    Serial.print(gyroscopeX_dps);
                                    Serial.print(F(","));
                                    Serial.print(gyroscopeY_dps);
                                    Serial.print(F(","));
                                    Serial.print(gyroscopeZ_dps);
                                    Serial.print(F(","));

                                    Serial.print(magnetometerX_nT);
                                    Serial.print(F(","));
                                    Serial.print(magnetometerY_nT);
                                    Serial.print(F(","));
                                    Serial.print(magnetometerZ_nT);
                                    Serial.print(F(","));

                                    Serial.print(roll_deg);
                                    Serial.print(F(","));
                                    Serial.print(pitch_deg);
                                    Serial.print(F(","));
                                    Serial.print(yaw_deg);
                                    Serial.print(F(","));

                                    Serial.print(forceX_N);
                                    Serial.print(F(","));
                                    Serial.print(jerkX_mps3);
                                    Serial.print(F(","));

                                    Serial.print(pressure_kPa);
                                    Serial.print(F(","));
                                    Serial.print(altitude_m);
                                    Serial.print(F(","));
                                    Serial.print(verticalSpeed_mps);
                                    Serial.print(F(","));
                                    Serial.print(verticalAcceleration_msp2);
                                    Serial.print(F(","));
                                    Serial.print(estimated);
                                    Serial.print(F(","));
                                    Serial.print(apogee);
                                    Serial.print(F(","));
                                    Serial.print(isFalling);
                                    Serial.print(F(","));

                                    Serial.print(groundVoltage_V);
                                    Serial.print(F(","));
                                    Serial.print(batteryVoltage_V);
                                    Serial.print(F(","));
                                    Serial.print(tieVoltage_V);
                                    Serial.print(F(","));
                                    Serial.print(busVoltage_V);
                                    Serial.print(F(","));

                                    Serial.print(groundCurrent_mA);
                                    Serial.print(F(","));
                                    Serial.print(batteryCurrent_mA);
                                    Serial.print(F(","));
                                    Serial.print(tieCurrent_mA);
                                    Serial.print(F(","));
                                    Serial.print(busCurrent_mA);
                                    Serial.print(F(","));

                                    Serial.print(groundPower_mW);
                                    Serial.print(F(","));
                                    Serial.print(batteryPower_mW);
                                    Serial.print(F(","));
                                    Serial.print(tiePower_mW);
                                    Serial.print(F(","));
                                    Serial.print(busPower_mW);
                                    Serial.print(F(","));

                                    Serial.print(temperatureRegulator1_degC);
                                    Serial.print(F(","));
                                    Serial.print(temperatureRegulator2_degC);
                                    Serial.print(F(","));
                                    Serial.print(temperatureRegulator3_degC);
                                    Serial.print(F(","));
                                    Serial.print(temperatureConduction_degC);
                                    Serial.print(F(","));

                                    Serial.print(temperatureOutside_degC);
                                    Serial.print(F(","));
                                    Serial.print(temperatureInside_degC);
                                    Serial.print(F(","));

                                    Serial.print(temperatureVentPort_degC);
                                    Serial.print(F(","));
                                    Serial.print(temperatureTankAtmosphere_degC);
                                    Serial.print(F("\n"));
                                  });

  while (!Serial)
    ;
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

void loop()
{
}
