#ifndef SIMPLECRYPTWORKER_H
#define SIMPLECRYPTWORKER_H

#include <QObject>
#include "worker.h"
#include "implementation/lfsr.h"
#include "implementation/lfsrdata.h"

class SimpleCryptWorker : public Worker
{
    Q_OBJECT
public:
    explicit SimpleCryptWorker(QString inputFileName, QString outputFileName, LFSRData::ContentLFSR1 initKey);

private:
    LFSR<LFSRData::ContentLFSR1> *reg;

public slots:
    void startWork();
};

#endif // SIMPLECRYPTWORKER_H
