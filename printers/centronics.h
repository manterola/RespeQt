#ifndef CENTRONICS_H
#define CENTRONICS_H

#include "nativeprinter.h"

namespace Printers {

    class Centronics : public NativePrinter
    {
    public:
        Centronics(SioWorker *sio);

        virtual const QChar translateAtascii(const char b);
    };

}
#endif // CENTRONICS_H
