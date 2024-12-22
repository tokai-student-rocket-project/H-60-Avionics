#include "RS405CB.hpp"

RS405CB::RS405CB(uint8_t enablePin)
{
    _enablePin = enablePin;
    pinMode(enablePin, OUTPUT);
}

void RS405CB::torqueOn(unsigned char *id)
{
    unsigned char TxData[9];
    unsigned char CheckSum = 0;

    // パケットデータ生成
    TxData[0] = 0xFA; // HEADER
    TxData[1] = 0xAF; // HEADER
    TxData[2] = id;   // ID
    TxData[3] = 0x00; // FLAGS
    TxData[4] = 0x24; // ADDRESS
    TxData[5] = 0x01; // LENGTH
    TxData[6] = 0x01; // COUNT
    TxData[7] = 0x01; // DATA

    for (int i = 2; i <= 7; i++)
    {
        CheckSum = CheckSum ^ TxData[i];
    }

    TxData[8] = CheckSum;

    digitalWrite(_enablePin, HIGH);
    for (int i = 0; i <= 8; i++)
    {
        Serial1.write(TxData[i]);
    }

    Serial1.flush();
    digitalWrite(_enablePin, LOW);
}

void RS405CB::torqueOff(unsigned char *id)
{
    unsigned char TxData[9];
    unsigned char CheckSum = 0;

    // パケットデータ生成
    TxData[0] = 0xFA; // HEADER
    TxData[1] = 0xAF; // HEADER
    TxData[2] = id;   // ID
    TxData[3] = 0x00; // FLAGS
    TxData[4] = 0x24; // ADDRESS
    TxData[5] = 0x01; // LENGTH
    TxData[6] = 0x01; // COUNT
    TxData[7] = 0x00; // DATA

    for (int i = 2; i <= 7; i++)
    {
        CheckSum = CheckSum ^ TxData[i];
    }

    TxData[8] = CheckSum;

    digitalWrite(_enablePin, HIGH);
    for (int i = 0; i <= 8; i++)
    {
        Serial1.write(TxData[i]);
    }

    Serial1.flush();
    digitalWrite(_enablePin, LOW);
}

void RS405CB::move(unsigned char *id, int angle, int speed)
{
    unsigned char TxData[12];
    unsigned char CheckSum = 0;

    // パケットデータ生成
    TxData[0] = 0xFA; // HEADER
    TxData[1] = 0xAF; // HEADER
    TxData[2] = id;   // ID
    TxData[3] = 0x00; // FLAGS
    TxData[4] = 0x1E; // ADDRESS
    TxData[5] = 0x04; // LENGTH
    TxData[6] = 0x01; // COUNT
    // Angle
    TxData[7] = (unsigned char)0x00FF & angle;
    TxData[8] = (unsigned char)0x00FF & (angle >> 8);
    // Speed
    TxData[9] = (unsigned char)0x00FF & speed;
    TxData[10] = (unsigned char)0x00FF & (speed >> 8);
    // チェックサム計算
    for (int i = 2; i <= 10; i++)
    {
        CheckSum = CheckSum ^ TxData[i];
    }
    TxData[11] = CheckSum;

    // パケットデータ送信
    digitalWrite(_enablePin, HIGH);
    for (int i = 0; i <= 11; i++)
    {
        Serial1.write(TxData[i]);
    }
    Serial1.flush();

    digitalWrite(_enablePin, LOW);
}

int16_t RS405CB::readCurrentposition(unsigned char *id)
{
    unsigned char txData[8];
    unsigned char CheckSum = 0;
    uint8_t rxData[26];
    int rxIndex = 0;
    int16_t currentPosition;
    uint8_t dataByte = 0;
    const int packetSize = 26;
    unsigned long timeout = 1000;
    unsigned long startTime = 0;


    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x09; // FLAGS
    txData[4] = 0x00; // ADDRESS
    txData[5] = 0x00; // LENGTH
    txData[6] = 0x01; // COUNT
    for (int i = 2; i <= 6; i++)
    {
        CheckSum = CheckSum ^ txData[i];
    }
    txData[7] = CheckSum;

    digitalWrite(_enablePin, HIGH); // パケット送信
    for (uint8_t i = 0; i <= 8; i++)
    {
        Serial1.write(txData[i]);
    }
    Serial1.flush();
    digitalWrite(_enablePin, LOW); // パケット送信完了

    startTime = millis();
    while((millis() - startTime) < timeout)
    {
        if(Serial1.available())
        {
            uint8_t readByte = Serial1.read();

            if (rxIndex == 0 && readByte == 0xDF){
                rxData[rxIndex] = readByte;
                rxIndex++;
            }
            else if (rxIndex > 0 && rxIndex < 26) {
                rxData[rxIndex] = readByte;
                rxIndex++;

                if (rxIndex == packetSize)
                {
                    currentPosition = (rxData[8] << 8) | rxData[7];
                    return currentPosition / 10;
                }
            }
        }
    }
    return 255;
}
