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

// uint16_t RS405CB::getCurrentposition(byte *id)
// {
//     unsigned char txData[8];
//     uint8_t rxData[100];
//     unsigned char CheckSum = 0;
//     uint32_t offset = 0;
//     // uint32_t readBytes;

//     txData[0] = 0xFA; // HEADER
//     txData[1] = 0xAF; // HEADER
//     txData[2] = id;   // ID
//     txData[3] = 0x09; // FLAGS
//     txData[4] = 0x00; // ADDRESS
//     txData[5] = 0x00; // LENGTH
//     txData[6] = 0x01; // COUNT
//     // TxData[7] = 0x00; // SUM
//     for (int i = 2; i <= 6; i++)
//     {
//         CheckSum = CheckSum ^ txData[i];
//     }
//     txData[7] = CheckSum;
//     Serial.print("\e[4;1H");
//     Serial.print("\e[0K");
//     Serial.print("CheckSum: ");
//     Serial.println(txData[7]);

//     // Serial1.flush();
//     digitalWrite(_enablePin, HIGH);
//     for (uint8_t i = 0; i <= 8; i++)
//     {
//         Serial1.write(txData[i]);
//     }
//     Serial1.flush();

//     while (Serial1.available() > 0)
//     {
//         uint8_t data = Serial1.read();
//         if (data == 0xFD)
//         {
//             Serial.println("パケット完成したよ．");
//             // offset = 0;
//         }
//         rxData[offset] = data;
//         offset++;
//         Serial.print("\e[2;1H");
//         Serial.print("\e[0K");
//         Serial.print(rxData[0], HEX);

//         Serial.print("\e[2;4H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[1], HEX);

//         Serial.print("\e[2;7H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[2], HEX);

//         Serial.print("\e[2;10H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[3], HEX);

//         Serial.print("\e[2;13H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[4], HEX);

//         Serial.print("\e[2;16H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[5], HEX);

//         Serial.print("\e[2;19H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[6], HEX);

//         Serial.print("\e[2;22H");
//         Serial.print("\e[0K");
//         Serial.println(rxData[7], HEX);

//         Serial.print("\e[3;1H");
//         Serial.print("\e[0K");
//         Serial.println(offset);
//     }
//     digitalWrite(_enablePin, LOW);

//     return rxData;
// }

// TO DO
/*
uint32_t offset = 0;
uint8_t packet[1024];
while (Serial1.available() > 0) {
    uint8_t data = Serial1.read();
    if (data == 0xFD) {
        Serial.print("前のパケットが完成したよ");
        // 前のパケットを使った処理
        offset = 0;
    {
    packet[offset] = data;
    offset++;
    }
}
*/
