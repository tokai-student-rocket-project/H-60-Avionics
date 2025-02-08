#include <Arduino.h>
#include <TaskManager.h>
#include <MsgPacketizer.h>
#include "Lib_Var.hpp"
#include "Lib_CAN.hpp"
#include "Lib_GseSignal.hpp"
#include "Lib_CountDetector.hpp"
#include "Lib_Buzzer.hpp"
#include "B3MSC1170A.hpp"
#include "RS405CB.hpp"

char ident = '\0';
bool doLogging = false;
uint8_t flightMode = 0;
uint16_t flightTime = 0;

CAN can(6);

OutputPin ledWork(A7);
OutputPin ledWaitingMode(A3);
OutputPin ledLaunchMode(A2);
Buzzer buzzer(A6, "buzzer");

B3MSC1170A mainValve;
void openMainValve() { mainValve.setPosition(0x01, -6000, 0); }
void closeMainValve() { mainValve.setPosition(0x01, 0, 1000); }

// RS405CB supplyValve(3);
RS405CB supplyValve;
void openSupplyValve() { supplyValve.move(1, 0, 200); }
void closeSupplyValve() { supplyValve.move(1, -800, 30); }

Var::ValveMode currentValveMode = Var::ValveMode::LAUNCH;
Var::GseSignal currentGseSignal = Var::GseSignal::IGNITION_ON;

GseSignal gseValve(7);
GseSignal gseIgniter(8);

constexpr uint16_t MODE_CHANGING_THRESHOLD = 5;
CountDetector valveSignalCounter(MODE_CHANGING_THRESHOLD);
CountDetector igniterSignalCounter(MODE_CHANGING_THRESHOLD);

uint32_t retryCount = 0;
uint32_t lastSendTime = 0;
bool isVerified = true;

void verifyValve()
{
  if (isVerified)
  {
    return;
  }

  if (retryCount == 0 && (millis() - lastSendTime) < 300)
  {
    return;
  }

  if (retryCount >= 5)
  {
    return;
  }

  if ((millis() - lastSendTime) < 50)
  {
    return;
  }

  if (currentValveMode == Var::ValveMode::WAITING)
  {
    bool isClosed = (mainValve.readDesiredPosition(0x01) > -1000) && (mainValve.readDesiredPosition(0x01) < 1000);

    if (isClosed)
    {
      isVerified = true;
    }
    else
    {
      retryCount++;
      lastSendTime = millis();
      closeMainValve();
    }
  }

  if (currentValveMode == Var::ValveMode::LAUNCH)
  {
    bool isOpen = (mainValve.readDesiredPosition(0x01) > -7500) && (mainValve.readDesiredPosition(0x01) < -5500);

    if (isOpen)
    {
      isVerified = true;
    }
    else
    {
      retryCount++;
      lastSendTime = millis();
      openMainValve();
    }
  }
}

void addTaskIfNotExisted(const String &name, void (*callback)())
{
  if (!Tasks.exists(name))
  {
    Tasks.add(name, callback);
  }
}

void changeMode(Var::ValveMode nextMode)
{
  if (nextMode == currentValveMode)
    return;

  retryCount = 0;
  lastSendTime = millis();
  isVerified = false;

  if (nextMode == Var::ValveMode::LAUNCH)
  {
    ledWaitingMode.low();
    ledLaunchMode.high();

    closeSupplyValve();

    addTaskIfNotExisted("delayed-open-main-valve", &openMainValve);
    Tasks["delayed-open-main-valve"]->startOnceAfterSec(0.3);
    buzzer.beepLong(1);
  }

  if (nextMode == Var::ValveMode::WAITING)
  {
    ledWaitingMode.high();
    ledLaunchMode.low();
    buzzer.beep(2);

    closeMainValve();
    openSupplyValve();
  }

  currentValveMode = nextMode;
}

void changeIgnition(Var::GseSignal nextMode)
{
  if (nextMode == currentGseSignal)
    return;
  if (nextMode == Var::GseSignal::IGNITION_ON)
  {
    buzzer.beepLongOnce();
  }

  if (nextMode == Var::GseSignal::IGNITION_OFF)
  {
    buzzer.beepTwice();
  }

  currentGseSignal = nextMode;
}

void syncFlightMode()
{
  if (can.available())
  {
    switch (can.getLatestLabel())
    {
    case Var::Label::FLIGHT_DATA:
    {
      can.receiveFlight(&flightMode, &flightTime, &doLogging, &ident);
      switch (flightMode) {
        case (0): // STANDBY
        {
          igniterSignalCounter.update(gseIgniter.isSignaled());

          if (igniterSignalCounter.isExceeded())
          {
            changeIgnition(Var::GseSignal::IGNITION_ON);
          }
          else
          {
            changeIgnition(Var::GseSignal::IGNITION_OFF);
          }

          valveSignalCounter.update(gseValve.isSignaled());
          if (valveSignalCounter.isExceeded())
          {
            changeMode(Var::ValveMode::LAUNCH);
          }
          else
          {
            changeMode(Var::ValveMode::WAITING);
          }
          verifyValve();

          Serial.println("STANDBY");
          break;
        }

        case (1): // READY_TO_FLY
        {
          igniterSignalCounter.update(gseIgniter.isSignaled());

          if (igniterSignalCounter.isExceeded())
          {
            changeIgnition(Var::GseSignal::IGNITION_ON);
          }
          else
          {
            changeIgnition(Var::GseSignal::IGNITION_OFF);
          }

          valveSignalCounter.update(gseValve.isSignaled());
          if (valveSignalCounter.isExceeded())
          {
            changeMode(Var::ValveMode::LAUNCH);
          }
          else
          {
            changeMode(Var::ValveMode::WAITING);
          }
          verifyValve();

          Serial.println("READY_TO_FLY");
          break;
        }

        case (2): // POWERED_CLIMB
        {
          changeMode(Var::ValveMode::LAUNCH);
          Serial.println("POWERED_CLIMB");
          break;
        }

        case (3): // FREE_CLIMB
        {
          changeMode(Var::ValveMode::LAUNCH);
          Serial.println("FREE_CLIMB");
          break;
        }

        case (4): // FREE_DESCENT
        {
          changeMode(Var::ValveMode::LAUNCH);
          Serial.println("FREE_DESCENT");
          break;
        }

        case (5): // DROGUE_CHUTE_DESCENT
        {
          changeMode(Var::ValveMode::LAUNCH);
          Serial.println("DROGUE_CHUTE_DESCENT");
          break;
        }

        case (6): // MAIN_CHUTE_DESCENT
        {
          changeMode(Var::ValveMode::LAUNCH);
          Serial.println("MAIN_CHUTE_DESCENT");
          break;
        }

        case (7): // LANDED
        {
          changeMode(Var::ValveMode::WAITING); // or WAITING
          Serial.println("LANDED");
          break;
        }

        case (8): // SHUTDOWN
        {
          changeMode(Var::ValveMode::WAITING); // or WAITING
          supplyValve.torqueOn(0);
          mainValve.torqueOff(0x01);
          Serial.println("SHUTDOWN");
          break;
        }
      }
    }
    }
  }
}

void sendValveMode()
{
  can.sendValveMode(currentValveMode == Var::ValveMode::LAUNCH);
}

void sendIgnition()
{
  can.sendIgnition(currentGseSignal == Var::GseSignal::IGNITION_ON);
}

void sendValveData()
{
  can.sendValveDataPart1(
      mainValve.readMotorTemperature(0x01),
      mainValve.readMcuTemperature(0x01),
      mainValve.readCurrent(0x01),
      mainValve.readVoltage(0x01));

  can.sendValveDataPart2(
      mainValve.readCurrentPosition(0x01),
      mainValve.readDesiredPosition(0x01),
      mainValve.readCurrentVelosity(0x01));
  
  can.sendValveDataPart3(
      supplyValve.readPosition(1),
      supplyValve.readTemperature(1),
      supplyValve.readVoltage(1));
}

void readData()
{
  Serial.println(supplyValve.readPosition(1));
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1);
  SPI.begin();

  can.begin();

  supplyValve.initialize();
  supplyValve.torqueOn(1);
  mainValve.initialize();

  Tasks.add(&syncFlightMode)->startFps(100);
  Tasks.add(&sendValveMode)->startFps(60);
  Tasks.add(&sendIgnition)->startFps(60);
  Tasks.add(&sendValveData)->startFps(10);
  Tasks.add(&readData)->startFps(10);

  changeMode(Var::ValveMode::WAITING);
  changeIgnition(Var::GseSignal::IGNITION_OFF);
}

void loop()
{
  Tasks.update();
}