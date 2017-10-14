#ifndef CRYPTROUTINES_H
#define CRYPTROUTINES_H

#include <QFile>
#include "lfsr.h"

DataBlock readBlock(QFile &inputFile);
void writeBlock(DataBlock blockOut, QFile &outputFile);

#endif // CRYPTROUTINES_H
