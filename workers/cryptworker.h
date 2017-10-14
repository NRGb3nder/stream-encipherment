#ifndef CRYPTWORKER_H
#define CRYPTWORKER_H

#include <QObject>
#include <QFile>
#include "lfsr.h"
#include "lfsrdata.h"
#include "types.h"
#include "cryptroutines.h"

class CryptWorker : public QObject
{
    Q_OBJECT
public:
    explicit CryptWorker(QString inputFileName, QString outputFileName, LFSRData::ContentLFSR1 initKey,
            QObject *parent = nullptr);

private:
    QString inputFileName;
    QString outputFileName;
    LFSR<LFSRData::ContentLFSR1> *reg;

signals:
    void progress(int progressValue);
    void done(QString sourceContent, QString resultContent, QString keyContent);

public slots:
    void startWork();
};

#endif // CRYPTWORKER_H
