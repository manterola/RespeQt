#include "atariprinter.h"

namespace Printers {

    AtariPrinter::AtariPrinter(SioWorker *worker)
        :NativePrinter(worker),
        mInternational(false)
    {}

    const QChar AtariPrinter::translateAtascii(const char b)
    {
        if (internationalMode()) {
            return mAtasciiInternational(b);
        }
        return NativePrinter::translateAtascii(b);
    }

}
