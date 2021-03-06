#ifndef CENTRONICS_H
#define CENTRONICS_H

#include "baseprinter.h"

namespace Printers
{
    class Centronics : public BasePrinter
    {
    public:
        Centronics(SioWorker *sio);

        virtual const QChar translateAtascii(const char b);
    };
}
#endif // CENTRONICS_H
