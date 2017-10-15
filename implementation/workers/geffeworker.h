#ifndef GEFFEWORKER_H
#define GEFFEWORKER_H

#include <QObject>
#include "worker.h"
#include "implementation/lfsr.h"
#include "implementation/lfsrdata.h"

class GeffeWorker : public Worker
{
    Q_OBJECT
public:
    explicit GeffeWorker(QString inputFileName, QString outputFileName,
            LFSRData::ContentLFSR1 initKeyLFSR1, LFSRData::ContentLFSR2 initKeyLFSR2,
            LFSRData::ContentLFSR3 initKeyLFSR3);

private:
    LFSR<LFSRData::ContentLFSR1> *firstReg;
    LFSR<LFSRData::ContentLFSR2> *secondReg;
    LFSR<LFSRData::ContentLFSR3> *thirdReg;

public slots:
    void startWork();
};

#endif // GEFFEWORKER_H
