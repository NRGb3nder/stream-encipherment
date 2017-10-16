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

QString Worker::getInputFileName()
{
    return inputFileName;
}

QString Worker::getOutputFileName()
{
    return outputFileName;
}

void Worker::setInputFileName(QString fileName)
{
    inputFileName = fileName;
}

void Worker::setOutputFileName(QString fileName)
{
    outputFileName = fileName;
}
