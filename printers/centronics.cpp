#include "centronics.h"

namespace Printers {

    Centronics::Centronics(SioWorker *sio)
        : NativePrinter(sio)
    {}

    const QChar Centronics::translateAtascii(const char b)
    {
        if ((unsigned char)b == 155) // Translate EOL to CR
        {
            return QChar(13);
        }
        return NativePrinter::translateAtascii(b);
    }

}
