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

CAN can(6);

OutputPin ledWork(A7);
OutputPin ledWaitingMode(A3);
OutputPin ledLaunchMode(A2);
Buzzer buzzer(A6, "buzzer");

B3MSC1170A mainValve;
void openMainValve() { mainValve.setPosition(0x01, -6000, 0); }
void closeMainValve() { mainValve.setPosition(0x01, 0, 1000); }

RS405CB supplyValve(3);
void openSupplyValve() { supplyValve.move(1, 0, 200); }
void closeSupplyValve() { supplyValve.move(1, -800, 30); }

Var::ValveMode currentMode = Var::ValveMode::LAUNCH;

GseSignal gseClose(7);
GseSignal gseOpen(8);

constexpr uint16_t MODE_CHANGING_THRESHOLD = 5;
CountDetector waitingSignalCounter(MODE_CHANGING_THRESHOLD);
CountDetector launchSignalCounter(MODE_CHANGING_THRESHOLD);

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

  if (currentMode == Var::ValveMode::WAITING)
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

  if (currentMode == Var::ValveMode::LAUNCH)
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
  if (nextMode == currentMode)
    return;

  retryCount = 0;
  lastSendTime = millis();
  isVerified = false;

  if (nextMode == Var::ValveMode::LAUNCH)
  {
    ledWaitingMode.low();
    ledLaunchMode.high();
    buzzer.beep(3);

    closeSupplyValve();

    addTaskIfNotExisted("delayed-open-main-valve", &openMainValve);
    Tasks["delayed-open-main-valve"]->startOnceAfterSec(0.3);
  }

  if (nextMode == Var::ValveMode::WAITING)
  {
    ledWaitingMode.high();
    ledLaunchMode.low();
    buzzer.beepLong(2);

    closeMainValve();
    openSupplyValve();
  }

  currentMode = nextMode;
}

void processSignal()
{
  ledWork.toggle();

  waitingSignalCounter.update(gseClose.isSignaled());
  launchSignalCounter.update(gseOpen.isSignaled() && !gseClose.isSignaled());

  if (waitingSignalCounter.isExceeded())
  {
    changeMode(Var::ValveMode::WAITING);
  }

  if (launchSignalCounter.isExceeded())
  {
    changeMode(Var::ValveMode::LAUNCH);
  }

  verifyValve();
}

void sendValveMode()
{
  can.sendValveMode(currentMode == Var::ValveMode::LAUNCH);
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
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1);
  SPI.begin();

  can.begin();

  supplyValve.torqueOn(1);
  mainValve.initialize();

  Tasks.add(&processSignal)->startFps(100);
  Tasks.add(&sendValveMode)->startFps(60);
  Tasks.add(&sendValveData)->startFps(10);

  changeMode(Var::ValveMode::WAITING);
}

void loop()
{
  Tasks.update();
}
