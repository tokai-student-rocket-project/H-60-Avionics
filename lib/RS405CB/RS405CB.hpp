#pragma once

#include <Arduino.h>
#include "IcsBaseClass.h"
#include "IcsHardSerialClass.h"

class RS405CB
{
    public:
        void initialize();
        bool torqueOn(unsigned char* id);
        bool torqueOff(unsigned char *id);
        bool move(unsigned char* id, int angle, int speed);
        int16_t readPosition(unsigned char *id);
        int16_t readTime(unsigned char *id);
        int16_t readTemprature(unsigned char *id);
        int16_t readVoltage(unsigned char *id);
        int16_t readLoad(unsigned char *id);

    private:
        uint8_t _enablePin;
        IcsHardSerialClass *_subSupply;
};