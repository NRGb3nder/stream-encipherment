#include "worker.h"

DataBlock Worker::readBlock(QFile &inputFile)
{
    char result;
    inputFile.read(&result, 1);
    return result;
}

void Worker::writeBlock(DataBlock blockOut, QFile &outputFile)
{
    char byteOut = blockOut;
    outputFile.write(&byteOut, 1);
}
