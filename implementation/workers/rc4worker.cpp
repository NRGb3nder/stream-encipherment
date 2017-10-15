#include "rc4worker.h"

RC4Worker::RC4Worker(QString inputFileName, QString outputFileName, std::vector<quint8> &secretKey) :
        generatorParameterI(0), generatorParameterJ(0)
{
    Worker::inputFileName = inputFileName;
    Worker::outputFileName = outputFileName;
    initializeSBoxDefault();
    initializeSBoxWithKey(secretKey);
}

void RC4Worker::swapBoxValues(int indexFirst, int indexSecond)
{
    quint8 temp = sBox[indexFirst];
    sBox[indexFirst] = sBox[indexSecond];
    sBox[indexSecond] = temp;
}

void RC4Worker::initializeSBoxDefault()
{
    for (int i = 0; i < SBOX_SIZE; i++) {
        sBox[i] = i;
    }
}

void RC4Worker::initializeSBoxWithKey(std::vector<quint8> &secretKey)
{
    int j = 0;
    for (int i = 0; i < SBOX_SIZE; i++) {
        j = (j + sBox[i] + secretKey[i % secretKey.size()]) % SBOX_SIZE;
        swapBoxValues(i, j);
    }
}

quint8 RC4Worker::getNewKey()
{
    generatorParameterI = (generatorParameterI + 1) % SBOX_SIZE;
    generatorParameterJ = (generatorParameterJ + sBox[generatorParameterI]) % SBOX_SIZE;
    swapBoxValues(generatorParameterI, generatorParameterJ);

    return sBox[(sBox[generatorParameterI] + sBox[generatorParameterJ]) % SBOX_SIZE];
}

void RC4Worker::startWork()
{
    const int BIN_ELEM_SIZE = 9;
    const int BIN_HEADER_MAX_LENGTH = BIN_ELEM_SIZE * 32;
    const int BIN_CONTENT_MAX_LENGTH = BIN_HEADER_MAX_LENGTH * 2;
    const int DEC_ELEM_SIZE = 4;
    const int DEC_HEADER_MAX_LENGTH = DEC_ELEM_SIZE * 32;
    const int DEC_CONTENT_MAX_LENGTH = DEC_HEADER_MAX_LENGTH * 2;

    QFile inputFile(inputFileName);
    QFile outputFile(outputFileName);
    inputFile.open(QIODevice::ReadOnly);
    outputFile.open(QIODevice::WriteOnly);

    quint64 fileSize = inputFile.size();
    QString keyContent;
    QString sourceContent;
    QString resultContent;
    quint8 blockIn;
    quint8 blockOut;
    quint8 keyPart;

    for (quint64 i = 0; i < fileSize; i++) {
        keyPart = getNewKey();
        blockIn = readBlock(inputFile);
        blockOut = blockIn ^ keyPart;
        writeBlock(blockOut, outputFile);

        if (keyContent.length() >= DEC_CONTENT_MAX_LENGTH) {
            keyContent.remove(DEC_HEADER_MAX_LENGTH, DEC_ELEM_SIZE);
        }
        if (sourceContent.length() >= BIN_CONTENT_MAX_LENGTH) {
            sourceContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
            resultContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
        }
        keyContent.append(QString::number(keyPart, 10).leftJustified(3) + " ");
        sourceContent.append(QString::number(blockIn, 2).rightJustified(8, '0') + " ");
        resultContent.append(QString::number(blockOut, 2).rightJustified(8, '0') + " ");

        emit progress(100 * i/fileSize);
    }

    if (keyContent.length() >= DEC_CONTENT_MAX_LENGTH) {
        keyContent.insert(DEC_HEADER_MAX_LENGTH, "\n...\n");
    }
    if (sourceContent.length() >= BIN_CONTENT_MAX_LENGTH) {
        sourceContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
        resultContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
    }

    inputFile.close();
    outputFile.close();

    emit progress(100);
    emit done(sourceContent, resultContent, keyContent);
}
