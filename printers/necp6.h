#ifndef NECP6_H
#define NECP6_H

#include "centronics.h"

namespace Printers {

    class NecP6 : public Centronics
    {
    public:
        NecP6(SioWorker *sio);

        bool handleBuffer(QByteArray &buffer, int len);
    };

}
#endif // NECP6_H
