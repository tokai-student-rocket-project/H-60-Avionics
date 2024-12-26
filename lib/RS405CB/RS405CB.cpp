#include "RS405CB.hpp"

void RS405CB::initialize()
{
    _subSupply = new IcsHardSerialClass(&Serial1, 3, 115200, 10);
    _subSupply->begin();
}

bool RS405CB::torqueOn(unsigned char *id)
{
    unsigned char TxData[9];
    unsigned char CheckSum = 0;
    unsigned char rxData[0];

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

    bool flag = _subSupply->synchronize(TxData, 9, rxData, 0);
    return flag;
}

bool RS405CB::torqueOff(unsigned char *id)
{
    unsigned char txData[9];
    unsigned char CheckSum = 0;
    unsigned char rxData[0];

    // パケットデータ生成
    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x00; // FLAGS
    txData[4] = 0x24; // ADDRESS
    txData[5] = 0x01; // LENGTH
    txData[6] = 0x01; // COUNT
    txData[7] = 0x00; // DATA

    for (int i = 2; i <= 7; i++)
    {
        CheckSum = CheckSum ^ txData[i];
    }

    txData[8] = CheckSum;

    bool flag = _subSupply->synchronize(txData, 9, rxData, 0);
}

bool RS405CB::move(unsigned char *id, int angle, int speed)
{
    unsigned char TxData[12];
    unsigned char CheckSum = 0;
    unsigned char rxData[0];

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

    bool flag = _subSupply->synchronize(TxData, 12, rxData, 0);
    return flag;
}

int16_t RS405CB::readPosition(unsigned char *id)
{
    unsigned char txData[8];
    unsigned char checkSum = 0;
    byte rxData[9];
    int16_t currentPosition = 0;

    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x0F; // FLAGS
    txData[4] = 0x2A; // ADDRESS
    txData[5] = 0x02; // LENGTH
    txData[6] = 0x00; // COUNT
    for (int i = 2; i <= 6; i++)
    {
        checkSum = checkSum ^ txData[i];
    }
    txData[7] = checkSum;

    _subSupply->synchronize(txData, 8, rxData, 9);

    currentPosition = (rxData[8] << 8) | rxData[7];
    return currentPosition;
}

/// @brief サーボが指令を受信し，移動を開始してからの経過時間
/// @param id サーボID
/// @return 実際の値は 10倍して使う．

int16_t RS405CB::readTime(unsigned char *id)
{
    unsigned char txData[8];
    unsigned char checkSum = 0;
    byte rxData[9];
    int16_t currentTime = 0;

    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x0F; // FLAGS
    txData[4] = 0x2C; // ADDRESS
    txData[5] = 0x02; // LENGTH
    txData[6] = 0x00; // COUNT
    for (int i = 2; i <= 6; i++)
    {
        checkSum = checkSum ^ txData[i];
    }
    txData[7] = checkSum;

    _subSupply->synchronize(txData, 8, rxData, 9);

    currentTime = (rxData[8] << 8) | rxData[7];
    return currentTime;
}

int16_t RS405CB::readTemperature(unsigned char *id)
{
    unsigned char txData[8];
    unsigned char checkSum = 0;
    byte rxData[9];
    int16_t currentTemperature = 0;

    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x0F; // FLAGS
    txData[4] = 0x32; // ADDRESS
    txData[5] = 0x02; // LENGTH
    txData[6] = 0x00; // COUNT
    for (int i = 2; i <= 6; i++)
    {
        checkSum = checkSum ^ txData[i];
    }
    txData[7] = checkSum;

    _subSupply->synchronize(txData, 8, rxData, 9);

    currentTemperature = (rxData[8] << 8) | rxData[7];
    return currentTemperature;
}

int16_t RS405CB::readVoltage(unsigned char *id)
{
    unsigned char txData[8];
    unsigned char checkSum = 0;
    byte rxData[9];
    int16_t currentVoltage = 0;

    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x0F; // FLAGS
    txData[4] = 0x34; // ADDRESS
    txData[5] = 0x02; // LENGTH
    txData[6] = 0x00; // COUNT
    for (int i = 2; i <= 6; i++)
    {
        checkSum = checkSum ^ txData[i];
    }
    txData[7] = checkSum;

    _subSupply->synchronize(txData, 8, rxData, 9);

    currentVoltage = (rxData[8] << 8) | rxData[7];
    return currentVoltage;
}

int16_t RS405CB::readLoad(unsigned char *id)
{
    unsigned char txData[8];
    unsigned char checkSum = 0;
    byte rxData[9];
    int16_t currentLoad = 0;

    txData[0] = 0xFA; // HEADER
    txData[1] = 0xAF; // HEADER
    txData[2] = id;   // ID
    txData[3] = 0x0F; // FLAGS
    txData[4] = 0x30; // ADDRESS
    txData[5] = 0x02; // LENGTH
    txData[6] = 0x00; // COUNT
    for (int i = 2; i <= 6; i++)
    {
        checkSum = checkSum ^ txData[i];
    }
    txData[7] = checkSum;

    _subSupply->synchronize(txData, 8, rxData, 9);

    currentLoad = (rxData[8] << 8) | rxData[7];
    return currentLoad;
}