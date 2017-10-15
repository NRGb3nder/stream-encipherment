#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QFile>
#include "implementation/common.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker() {}
    virtual ~Worker() {}

protected:
    QString inputFileName;
    QString outputFileName;
    DataBlock readBlock(QFile &inputFile);
    void writeBlock(DataBlock blockOut, QFile &outputFile);

signals:
    void progress(int progressValue);
    void done(QString sourceContent, QString resultContent, QString keyContent);

public slots:
    void startWork() {}
};

#endif // WORKER_H
