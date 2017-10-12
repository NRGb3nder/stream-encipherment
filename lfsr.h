#ifndef LFSR_H
#define LFSR_H

#include <vector>

template <typename RegData>
class LFSR
{
private:
    int shiftPeriod;
    RegData key;
    std::vector<int> taps; // example: taps vector for x^12 + x^5 + 1 is {12, 5} or {5, 12}
    int getShiftPeriod();
    void generateKey();
public:
    LFSR(RegData initialValue, std::vector<int> &taps);
    RegData getNewKey();
};

template <typename RegData>
LFSR<RegData>::LFSR(RegData initialValue, std::vector<int> &taps) : key(initialValue), taps(taps)
{
    shiftPeriod = getShiftPeriod();
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

template <typename RegData>
void LFSR<RegData>::generateKey()
{
    RegData bit;

    for (int shiftNumber = 0; shiftNumber < shiftPeriod; shiftNumber++) {
        bit.value = key.value >> (taps[0] - 1);
        for (int i = 1; i < taps.size(); i++) {
            bit.value ^= key.value >> (taps[i] - 1); // xor'ing taps
        }
        bit.value &= 1;
        key.value = key.value << 1 | bit.value; // register shift
    }
}

template <typename RegData>
RegData LFSR<RegData>::getNewKey()
{
    generateKey();
    return key;
}

#endif //LFSR_H
