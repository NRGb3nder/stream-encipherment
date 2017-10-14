#ifndef TYPES_H
#define TYPES_H

#include <QDataStream>

enum ErrorType
{
    E_INVALID_INPUT_FILE,
    E_INVALID_OUTPUT_FILE,
    E_KEY_NOT_FOUND
};

enum WorkerMode
{
    MODE_ENCIPHER,
    MODE_DECIPHER
};

#endif // TYPES_H
