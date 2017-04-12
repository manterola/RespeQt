#ifndef ATARI1029_H
#define ATARI1029_H

#include "nativeprinter.h"

namespace Printers {

    class Atari1029 : public NativePrinter
    {
    public:
        Atari1029(SioWorker *sio);
    };

}
#endif // ATARI1029_H
