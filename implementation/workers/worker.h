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

private:
    QString inputFileName;
    QString outputFileName;

protected:
    quint8 readBlock(QFile &inputFile);
    void writeBlock(quint8 blockOut, QFile &outputFile);
    QString getInputFileName();
    QString getOutputFileName();
    void setInputFileName(QString fileName);
    void setOutputFileName(QString fileName);

signals:
    void progress(int progressValue);
    void done(QString sourceContent, QString resultContent, QString keyContent);

public slots:
    void startWork() {}
};

#endif // WORKER_H
