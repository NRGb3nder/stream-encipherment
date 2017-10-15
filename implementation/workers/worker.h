#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QFile>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker() {}
    virtual ~Worker() {}

protected:
    QString inputFileName;
    QString outputFileName;
    quint8 readBlock(QFile &inputFile);
    void writeBlock(quint8 blockOut, QFile &outputFile);

signals:
    void progress(int progressValue);
    void done(QString sourceContent, QString resultContent, QString keyContent);

public slots:
    void startWork() {}
};

#endif // WORKER_H
