#include "geffeworker.h"

GeffeWorker::GeffeWorker(QString inputFileName, QString outputFileName,
        LFSRData::ContentLFSR1 initKeyLFSR1, LFSRData::ContentLFSR2 initKeyLFSR2,
        LFSRData::ContentLFSR3 initKeyLFSR3)
{
    Worker::inputFileName = inputFileName;
    Worker::outputFileName = outputFileName;
    firstReg = new LFSR<LFSRData::ContentLFSR1>(initKeyLFSR1, LFSRData::tapsLFSR1);
    secondReg = new LFSR<LFSRData::ContentLFSR2>(initKeyLFSR2, LFSRData::tapsLFSR2);
    thirdReg = new LFSR<LFSRData::ContentLFSR3>(initKeyLFSR3, LFSRData::tapsLFSR3);
}

void GeffeWorker::startWork()
{
    const int ELEM_SIZE = sizeof(DataBlock) * 8 + 1;
    const int HEADER_MAX_LENGTH = ELEM_SIZE * 32;
    const int CONTENT_MAX_LENGTH = HEADER_MAX_LENGTH * 2;
    const int KEY_QUANTITY = 3;

    QFile inputFile(inputFileName);
    QFile outputFile(outputFileName);
    inputFile.open(QIODevice::ReadOnly);
    outputFile.open(QIODevice::WriteOnly);

    quint64 fileSize = inputFile.size();
    QString keyPartContent[KEY_QUANTITY];
    QString geffeKey;
    QString keyContent;
    QString sourceContent;
    QString resultContent;
    DataBlock blockIn;
    DataBlock blockOut;
    DataBlock keyPart[KEY_QUANTITY];
    DataBlock resultKeyPart;

    for (quint64 i = 0; i < fileSize; i++) {
        keyPart[0] = firstReg->getNewKey();
        keyPart[1] = secondReg->getNewKey();
        keyPart[2] = thirdReg->getNewKey();
        resultKeyPart = (keyPart[0] & keyPart[1]) | (~keyPart[0] & keyPart[2]);
        blockIn = readBlock(inputFile);
        blockOut = blockIn ^ resultKeyPart;
        writeBlock(blockOut, outputFile);

        if (sourceContent.length() >= CONTENT_MAX_LENGTH) {
            for (int k = 0; k < KEY_QUANTITY; k++) {
                keyPartContent[k].remove(HEADER_MAX_LENGTH, ELEM_SIZE);
            }
            geffeKey.remove(HEADER_MAX_LENGTH, ELEM_SIZE);
            sourceContent.remove(HEADER_MAX_LENGTH, ELEM_SIZE);
            resultContent.remove(HEADER_MAX_LENGTH, ELEM_SIZE);
        }
        for (int k = 0; k < KEY_QUANTITY; k++) {
            keyPartContent[k].append(QString::number(keyPart[k], 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");
        }
        geffeKey.append(QString::number(resultKeyPart, 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");
        sourceContent.append(QString::number(blockIn, 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");
        resultContent.append(QString::number(blockOut, 2).rightJustified(sizeof(DataBlock) * 8, '0') + " ");

        emit progress(100 * i/fileSize);
    }

    for (int k = 0; k < KEY_QUANTITY; k++) {
        keyPartContent[k].insert(HEADER_MAX_LENGTH, "\n...\n");
    }
    geffeKey.insert(HEADER_MAX_LENGTH, "\n...\n");
    keyContent = "Geffe key:\n" + geffeKey +
            "\n\nLFSR1 key:\n" + keyPartContent[0] +
            "\n\nLFSR2 key:\n" + keyPartContent[1] +
            "\n\nLFSR3 key:\n" + keyPartContent[2];
    sourceContent.insert(HEADER_MAX_LENGTH, "\n...\n");
    resultContent.insert(HEADER_MAX_LENGTH, "\n...\n");

    inputFile.close();
    outputFile.close();

    emit progress(100);
    emit done(sourceContent, resultContent, keyContent);
}
