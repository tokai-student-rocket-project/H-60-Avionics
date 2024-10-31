#include <Arduino.h>
#include <SPI.h>
#include <TaskManager.h>
#include <mcp2515_can.h>
#include <Adafruit_MAX31855.h>


mcp2515_can CAN(6);
Adafruit_MAX31855 thermocouple(5);

uint32_t referenceTime;
float taskRate;


void setup() {
  Serial.begin(115200);

  CAN.begin(CAN_125KBPS, MCP_8MHz);
  thermocouple.begin();

  Tasks.add(&sendTemperatureTask)->startFps(2);
  Tasks.add(&sendPerformanceTask)->startFps(7);
  Tasks.add(&measurePerformanceTask)->startFps(10000);
}


void loop() {
  Tasks.update();
}


void sendTemperatureTask() {
  float ventportTemperature = thermocouple.readCelsius();
  float tankAtmosphereTemperature = thermocouple.readInternal();

  uint8_t data[8];
  memcpy(data + 0, &ventportTemperature, 4);
  memcpy(data + 4, &tankAtmosphereTemperature, 4);

  CAN.sendMsgBuf(0xF0, 0, 8, data);
}


void sendPerformanceTask() {
  uint32_t currentTime = millis();

  uint8_t data[8];
  memcpy(data + 0, &currentTime, 4);
  memcpy(data + 4, &taskRate, 4);

  CAN.sendMsgBuf(0xF1, 0, 8, data);
}


void measurePerformanceTask() {
  taskRate = getTaskRate(micros());
}


float getTaskRate(uint32_t time)
{
  uint32_t deltaTime = time - referenceTime;
  referenceTime = time;

  float rate = 1000000.0 / (float)deltaTime;

  return rate;
}
