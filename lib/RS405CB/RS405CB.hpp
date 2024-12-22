#pragma once

#include <Arduino.h>
#include "IcsBaseClass.h"
#include "IcsHardSerialClass.h"

class RS405CB
{
    public:
        RS405CB(uint8_t pinNumber);
        void torqueOn(unsigned char* id);
        void torqueOff(unsigned char *id);
        void move(unsigned char* id, int angle, int speed);
        int16_t readCurrentposition(unsigned char *id);

    private:
        uint8_t _enablePin;
        IcsHardSerialClass *_subSupply;
};