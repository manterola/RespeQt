#ifndef NATIVEPRINTERSUPPORT_H
#define NATIVEPRINTERSUPPORT_H

#include "baseprinter.h"
#include "sioworker.h"

namespace Printers {

    class NativePrinter : public BasePrinter
    {
        Q_OBJECT
    public:
        NativePrinter(SioWorker *sio);
        virtual ~NativePrinter();

        virtual bool requiresNativePrinter() const { return true; }
        virtual QPrinter *nativePrinter() const { return mNativePrinter; }

        virtual bool handleBuffer(QByteArray &buffer, int len);
        virtual void beginPrint();
        virtual void endPrint();


    protected:
        QPainter *mPainter;

        QFont mFont;
        int x, y;
        QRect mBoundingBox;
        QFontMetrics *mFontMetrics;
        QPrinter *mNativePrinter;

        // This should be static methods, because they are called
        // from the constructor
        void setupFont() {}
        void setupPrinter();
    };

}
#endif // NATIVEPRINTERSUPPORT_H
