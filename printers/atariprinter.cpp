#include "atariprinter.h"

namespace Printers {

    AtariPrinter::AtariPrinter(SioWorker *worker)
        :NativePrinterSupport(worker),
        mInternational(false)
    {}

    const QChar AtariPrinter::translateAtascii(const char b)
    {
        if (internationalMode()) {
            return mAtasciiInternational(b);
        }
        return NativePrinterSupport::translateAtascii(b);
    }

}
