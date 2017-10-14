#ifndef LFSRDATA_H
#define LFSRDATA_H

#include <QDataStream>
#include <vector>

namespace LFSRData
{
    const int LFSR1_BITFIELD_SIZE = 25;
    const int LFSR2_BITFIELD_SIZE = 33;
    const int LFSR3_BITFIELD_SIZE = 23;

    const std::vector<int> tapsLFSR1 {25, 3};
    const std::vector<int> tapsLFSR2 {33, 13};
    const std::vector<int> tapsLFSR3 {23, 5};

    struct ContentLFSR1
    {
        quint32 value : LFSR1_BITFIELD_SIZE;
    };

    struct ContentLFSR2
    {
        quint64 value : LFSR2_BITFIELD_SIZE;
    };

    struct ContentLFSR3
    {
        quint32 value : LFSR3_BITFIELD_SIZE;
    };
}

#endif // LFSRDATA_H
