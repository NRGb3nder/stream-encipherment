#include "geffeworker.h"

GeffeWorker::GeffeWorker(QString inputFileName, QString outputFileName,
        LFSRData::ContentLFSR1 initKeyLFSR1, LFSRData::ContentLFSR2 initKeyLFSR2,
        LFSRData::ContentLFSR3 initKeyLFSR3)
{
    setInputFileName(inputFileName);
    setOutputFileName(outputFileName);
    firstReg = new LFSR<LFSRData::ContentLFSR1>(initKeyLFSR1, LFSRData::tapsLFSR1);
    secondReg = new LFSR<LFSRData::ContentLFSR2>(initKeyLFSR2, LFSRData::tapsLFSR2);
    thirdReg = new LFSR<LFSRData::ContentLFSR3>(initKeyLFSR3, LFSRData::tapsLFSR3);
}

void GeffeWorker::startWork()
{
    const int BIN_ELEM_SIZE = 9;
    const int BIN_HEADER_MAX_LENGTH = BIN_ELEM_SIZE * 32;
    const int BIN_CONTENT_MAX_LENGTH = BIN_HEADER_MAX_LENGTH * 2;
    const int KEY_QUANTITY = 3;

    QFile inputFile(getInputFileName());
    QFile outputFile(getOutputFileName());
    inputFile.open(QIODevice::ReadOnly);
    outputFile.open(QIODevice::WriteOnly);

    quint64 fileSize = inputFile.size();
    QString keyPartContent[KEY_QUANTITY];
    QString geffeKey;
    QString keyContent;
    QString sourceContent;
    QString resultContent;
    quint8 blockIn;
    quint8 blockOut;
    quint8 keyPart[KEY_QUANTITY];
    quint8 resultKeyPart;

    for (quint64 i = 0; i < fileSize; i++) {
        keyPart[0] = firstReg->getNewKey();
        keyPart[1] = secondReg->getNewKey();
        keyPart[2] = thirdReg->getNewKey();
        resultKeyPart = (keyPart[0] & keyPart[1]) | (~keyPart[0] & keyPart[2]);
        blockIn = readBlock(inputFile);
        blockOut = blockIn ^ resultKeyPart;
        writeBlock(blockOut, outputFile);

        if (sourceContent.length() >= BIN_CONTENT_MAX_LENGTH) {
            for (int k = 0; k < KEY_QUANTITY; k++) {
                keyPartContent[k].remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
            }
            geffeKey.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
            sourceContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
            resultContent.remove(BIN_HEADER_MAX_LENGTH, BIN_ELEM_SIZE);
        }
        for (int k = 0; k < KEY_QUANTITY; k++) {
            keyPartContent[k].append(QString::number(keyPart[k], 2).rightJustified(8, '0') + " ");
        }
        geffeKey.append(QString::number(resultKeyPart, 2).rightJustified(8, '0') + " ");
        sourceContent.append(QString::number(blockIn, 2).rightJustified(8, '0') + " ");
        resultContent.append(QString::number(blockOut, 2).rightJustified(8, '0') + " ");

        emit progress(100 * i/fileSize);
    }

    if (keyPartContent[0].length() >= BIN_CONTENT_MAX_LENGTH) {
        for (int k = 0; k < KEY_QUANTITY; k++) {
            keyPartContent[k].insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
        }
        geffeKey.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
        sourceContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
        resultContent.insert(BIN_HEADER_MAX_LENGTH, "\n...\n");
    }

    keyContent = "Geffe key:\n" + geffeKey +
            "\n\nLFSR1 key:\n" + keyPartContent[0] +
            "\n\nLFSR2 key:\n" + keyPartContent[1] +
            "\n\nLFSR3 key:\n" + keyPartContent[2];

    inputFile.close();
    outputFile.close();

    emit progress(100);
    emit done(sourceContent, resultContent, keyContent);
}
