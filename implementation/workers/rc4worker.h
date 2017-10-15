#ifndef RC4WORKER_H
#define RC4WORKER_H

#include <QObject>
#include "worker.h"

const int SBOX_SIZE = 256;

class RC4Worker : public Worker
{
    Q_OBJECT
public:
    explicit RC4Worker(QString inputFileName, QString outputFileName, std::vector<quint8> &secretKey);

private:
    quint8 sBox[SBOX_SIZE];
    int generatorParameterI;
    int generatorParameterJ;
    void initializeSBoxDefault();
    void initializeSBoxWithKey(std::vector<quint8> &secretKey);
    quint8 getNewKey();
    void swapBoxValues(int indexFirst, int indexSecond);

public slots:
    void startWork();
};

#endif // RC4WORKER_H
