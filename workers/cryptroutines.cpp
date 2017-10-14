#include "cryptroutines.h"

DataBlock readBlock(QFile &inputFile)
{
    char result;
    inputFile.read(&result, 1);
    return result;
}

void writeBlock(DataBlock blockOut, QFile &outputFile)
{
    char byteOut = blockOut;
    outputFile.write(&byteOut, 1);
}
