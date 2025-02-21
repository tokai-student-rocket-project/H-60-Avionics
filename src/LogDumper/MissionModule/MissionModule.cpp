#include <MsgPacketizer.h>
#include "Lib_FRAM.hpp"

FRAM fram0(A6);
FRAM fram1(A5);

void dump(FRAM *fram)
{
  uint8_t data[4096];
  uint32_t size = 0;
  uint32_t writeAddress = 0;

  for (uint32_t address = 0; address < FRAM::LENGTH; address++)
  {
    data[writeAddress] = fram->read(address);
    size = writeAddress + 1;

    Serial.print(data[writeAddress], HEX);

    if (data[writeAddress] == 0x00)
    {
      writeAddress = 0;

      if (data[1] == 0x0A)
      {
        MsgPacketizer::feed(data, size);
        Serial.println();
      }
    }
    else
    {
      writeAddress++;
      Serial.print(" ");
    }
  }
}

void printHeader()
{
  Serial.print("ident");
  Serial.print(",");
  Serial.print("micros");
  Serial.print(",");
  Serial.print("flightTime");
  Serial.print(",");
  Serial.print("flightMode");
  Serial.print(",");
  Serial.print("loggerUsage");
  Serial.print(",");

  Serial.print("phase");
  Serial.print(",");
  Serial.print("accelerationX_mps2");
  Serial.print(",");
  Serial.print("accelerationY_mps2");
  Serial.print(",");
  Serial.print("accelerationZ_mps2");
  Serial.print("\n");
}

void setup()
{
  Serial.begin(115200);
  SPI.begin();

  MsgPacketizer::subscribe_manual(0x0A,
    [&](char ident, uint32_t micros, uint16_t flightTime, uint8_t flightMode, float loggerUsage,
      uint8_t phase, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint8_t z0, uint8_t z1
      ) {
        Serial.print(ident); Serial.print(F(","));
        Serial.print(micros); Serial.print(F(","));
        Serial.print(flightTime); Serial.print(F(","));
        Serial.print(flightMode); Serial.print(F(","));
        Serial.print(loggerUsage); Serial.print(F(","));

        Serial.print(phase); Serial.print(F(","));
        Serial.print((float)((int16_t)(uint16_t(x1) << 8 | uint16_t(x0))) * 0.049 * 9.80665); Serial.print(F(","));
        Serial.print((float)((int16_t)(uint16_t(y1) << 8 | uint16_t(y0))) * 0.049 * 9.80665); Serial.print(F(","));
        Serial.print((float)((int16_t)(uint16_t(z1) << 8 | uint16_t(z0))) * 0.049 * 9.80665); Serial.print("\n");
    }
  );

  while (!Serial)
    ;
  delay(5000);
  printHeader();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  dump(&fram0);
  dump(&fram1);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
}
