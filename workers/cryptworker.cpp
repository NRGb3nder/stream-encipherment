#include "cryptworker.h"

CryptWorker::CryptWorker(QString inputFileName, QString outputFileName, LFSRData::ContentLFSR1 initKey,
        QObject *parent) :
        QObject(parent),
        inputFileName(inputFileName),
        outputFileName(outputFileName)
{
    reg = new LFSR<LFSRData::ContentLFSR1>(initKey, LFSRData::tapsLFSR1);
}

void CryptWorker::startWork()
{
    const int ELEM_SIZE = sizeof(DataBlock) * 8 + 1;
    const int HEADER_MAX_LENGTH = ELEM_SIZE * 32;
    const int CONTENT_MAX_LENGTH = HEADER_MAX_LENGTH * 2;

    QFile inputFile(inputFileName);
    QFile outputFile(outputFileName);
    inputFile.open(QIODevice::ReadOnly);
    outputFile.open(QIODevice::WriteOnly);

    quint64 fileSize = inputFile.size();
    QString keyContent;
    QString sourceContent;
    QString resultContent;
    DataBlock blockIn;
    DataBlock blockOut;
    DataBlock keyPart;

    for (quint64 i = 0; i < fileSize; i++) {
        keyPart = reg->getNewKey();
        blockIn = readBlock(inputFile);
        blockOut = blockIn ^ keyPart;
        writeBlock(blockOut, outputFile);

        if (keyContent.length() >= CONTENT_MAX_LENGTH) {
            keyContent.remove(HEADER_MAX_LENGTH, ELEM_SIZE);
            sourceContent.remove(HEADER_MAX_LENGTH, ELEM_SIZE);
            resultContent.remove(HEADER_MAX_LENGTH, ELEM_SIZE);
        }
        keyContent.append(QString::number(keyPart, 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");
        sourceContent.append(QString::number(blockIn, 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");
        resultContent.append(QString::number(blockOut, 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");

        emit progress(100 * i/fileSize);
    }

    keyContent.insert(HEADER_MAX_LENGTH, "\n...\n");
    sourceContent.insert(HEADER_MAX_LENGTH, "\n...\n");
    resultContent.insert(HEADER_MAX_LENGTH, "\n...\n");

    inputFile.close();
    outputFile.close();

    emit progress(100);
    emit done(sourceContent, resultContent, keyContent);
}
