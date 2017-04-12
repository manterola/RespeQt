#include "escp.h"

namespace Printers {

    EscP::EscP(SioWorker *sio)
        : Centronics(sio)
    {
        mTypeId = ESCP;
        mTypeName = "Esc/P compatible";
    }

    bool EscP::handleBuffer(QByteArray &/*buffer*/, int /*len*/)
    {
        return true;
    }

}
