#include "worker.h"

quint8 Worker::readBlock(QFile &inputFile)
{
    char result;
    inputFile.read(&result, 1);
    return result;
}

void Worker::writeBlock(quint8 blockOut, QFile &outputFile)
{
    char byteOut = blockOut;
    outputFile.write(&byteOut, 1);
}
