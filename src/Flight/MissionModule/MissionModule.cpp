#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <TaskManager.h>
#include <MsgPacketizer.h>
#include "Lib_Var.hpp"
#include "Lib_CAN.hpp"
#include "Lib_Logger2.hpp"
#include "Lib_ADXL375.hpp"
#include "Lib_RateMonitor.hpp"
#include "Lib_OutputPin.hpp"

char ident = '\0';
bool doLogging = false;
uint8_t phase = 0;
uint8_t flightMode = 0;
uint16_t flightTime = 0;

CAN can(7);
Logger logger(A6, A5);

OutputPin loggerBreaker(5);

OutputPin ledWork(LED_BUILTIN);
OutputPin ledCanRx(0);
OutputPin ledCanTx(1);
OutputPin ledLoRaRx(A2);
OutputPin ledLoRaTx(A1);
OutputPin ledLogging(2);

ADXL375 adxl(15);

void task1kHz()
{
  ledWork.set(doLogging);

  uint8_t x0, x1, y0, y1, z0, z1;
  adxl.getAcceleration(&x0, &x1, &y0, &y1, &z0, &z1);

  const auto &logPacket = MsgPacketizer::encode(0x0A,
                                                ident, micros(), flightTime, flightMode, logger.getUsage(),
                                                phase, x0, x1, y0, y1, z0, z1);

  if (doLogging)
  {
    logger.write(logPacket.data.data(), logPacket.data.size());
  }
}

void startLogging()
{
  doLogging = true;
  ledLogging.high();
}

void stopLogging()
{
  doLogging = false;
  ledLogging.low();
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  SPI.begin();

  can.begin();

  loggerBreaker.high();

  adxl.begin();

  Tasks.add(&task1kHz)->startFps(1000);
  Tasks.add("stop-logging", &stopLogging);
}

void loop()
{
  Tasks.update();

  if (can.available())
  {
    switch (can.getLatestLabel())
    {
    case Var::Label::FLIGHT_DATA:
    {
      uint8_t nextFlightMode;
      bool doLoggingDontUse;
      can.receiveFlight(&nextFlightMode, &flightTime, &doLoggingDontUse, &ident);
      ledCanRx.toggle();

      if (flightMode == nextFlightMode)
        break;
      if (doLogging && flightMode != 1)
        break;

      if (nextFlightMode == 1)
      {
        // 推進前計測フェーズ
        if (flightMode == 0)
        {
          logger.reset();
        }

        phase = 1;
        startLogging();
      }

      if (nextFlightMode == 2)
      {
        // 推進計測フェーズ
        if (flightMode == 0)
        {
          logger.reset();
        }

        phase = 2;
        startLogging();
        Tasks["stop-logging"]->startOnceAfterSec(3);
      }

      if (nextFlightMode == 5)
      {
        // ドローグシュート開傘フェーズ
        phase = 3;
        startLogging();
        Tasks["stop-logging"]->startOnceAfterSec(4);
      }

      if (nextFlightMode == 6)
      {
        // メインシュート開傘フェーズ
        phase = 4;
        startLogging();
      }

      flightMode = nextFlightMode;

      break;
    }
    }
  }
}