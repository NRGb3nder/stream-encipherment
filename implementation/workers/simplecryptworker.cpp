#include "simplecryptworker.h"

SimpleCryptWorker::SimpleCryptWorker(QString inputFileName, QString outputFileName, LFSRData::ContentLFSR1 initKey)
{
    Worker::inputFileName = inputFileName;
    Worker::outputFileName = outputFileName;
    reg = new LFSR<LFSRData::ContentLFSR1>(initKey, LFSRData::tapsLFSR1);
}

void SimpleCryptWorker::startWork()
{
    const int BIN_ELEM_SIZE = 9;
    const int BIN_HEADER_MAX_LENGTH = BIN_ELEM_SIZE * 32;
    const int BIN_CONTENT_MAX_LENGTH = BIN_HEADER_MAX_LENGTH * 2;

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
        keyPart = reg->getNewKey();
        blockIn = readBlock(inputFile);
        blockOut = blockIn ^ keyPart;
        writeBlock(blockOut, outputFile);

        if (keyContent.length() >= BIN_CONTENT_MAX_LENGTH) {
            keyContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
            sourceContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
            resultContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
        }
        keyContent.append(QString::number(keyPart, 2).rightJustified(8, '0') + " ");
        sourceContent.append(QString::number(blockIn, 2).rightJustified(8, '0') + " ");
        resultContent.append(QString::number(blockOut, 2).rightJustified(8, '0') + " ");

        emit progress(100 * i/fileSize);
    }

    if (keyContent.length() >= BIN_CONTENT_MAX_LENGTH) {
        keyContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
        sourceContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
        resultContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
    }

    inputFile.close();
    outputFile.close();

    emit progress(100);
    emit done(sourceContent, resultContent, keyContent);
}
