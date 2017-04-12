#ifndef ESCP_H
#define ESCP_H

#include "centronics.h"

namespace Printers {

    class EscP : public Centronics
    {
    public:
        EscP(SioWorker *sio);

        virtual bool handleBuffer(QByteArray &buffer, int len);
    };

}

#endif // ESCP_H
