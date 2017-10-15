#ifndef LFSR_H
#define LFSR_H

#include <QDataStream>
#include <vector>
#include "common.h"

template <typename T>
class LFSR
{
private:
    const std::vector<int> taps; // example: taps vector for x^12 + x^5 + 1 is {12, 5} or {5, 12}
    int shiftPeriod;
    int regPosition;
    T content;
    T key;
    int getShiftPeriod();
    void shift();
public:
    LFSR(T initialValue, const std::vector<int> &taps);
    DataBlock getNewKey();
};

template <typename T>
LFSR<T>::LFSR(T initialValue, const std::vector<int> &taps) :
        taps(taps), regPosition(0), content(initialValue), key(initialValue)
{
     shiftPeriod = getShiftPeriod();
     key.value = content.value >> (shiftPeriod - 1);
}

template <typename RegData>
int LFSR<RegData>::getShiftPeriod()
{
    int result = 0;
    RegData test;
    std::memset(&test, 0, sizeof(test));

    test.value = ~0;
    while (test.value) {
        test.value &= test.value - 1;
        result++;
    }

    return result;
}

template <typename T>
void LFSR<T>::shift()
{
    T bit;

    for (int i = 0; i < shiftPeriod; i++) {
        bit.value = content.value >> (taps[0] - 1);
        for (size_t j = 1; j < taps.size(); j++) {
            bit.value ^= content.value >> (taps[j] - 1); // xor'ing taps
        }
        bit.value &= 1;
        content.value = content.value << 1 | bit.value; // register shift
    }
}

template <typename T>
DataBlock LFSR<T>::getNewKey()
{
    DataBlock resultKey = 0;

    int dataBlockSize = static_cast<int>(sizeof(DataBlock) * 8);
    for (int i = 0; i < dataBlockSize; i++)
    {
        resultKey |= key.value;
        regPosition++;
        if (regPosition == shiftPeriod) {
            shift();
            regPosition = 0;
        }
        key.value = content.value << regPosition;
        key.value >>= shiftPeriod - 1;
        if (i < dataBlockSize - 1)
            resultKey <<= 1;
    }

    return resultKey;
}

#endif //LFSR_H
