#include "nativeprinter.h"

namespace Printers {

    NativePrinter::NativePrinter(SioWorker *sio)
        : BasePrinter(sio),
          mPainter(NULL),
          mFontMetrics(NULL)
    {
        setupPrinter();
        setupFont();
    }

    NativePrinter::~NativePrinter()
    {
        if (requiresNativePrinter())
        {
            endPrint();
        }
    }

    void NativePrinter::setupPrinter() {
        mNativePrinter = new QPrinter();
        mPainter = new QPainter();
    }

    bool NativePrinter::handleBuffer(QByteArray & /*buffer*/, int /*len*/)
    {
        if (!mPrinting)
        {
            mPrinting = true;
            beginPrint();
        }
        return true;
    }

    void NativePrinter::beginPrint() {
        if (mPrinting)
        {
            mPainter->begin(mNativePrinter);
            mPainter->setFont(mFont);
            mBoundingBox = mNativePrinter->pageRect();
            x = mBoundingBox.left();
            y = mBoundingBox.top() + mFontMetrics->lineSpacing();
        }
    }

    void NativePrinter::endPrint() {
        if (mPainter->isActive())
        {
            mPainter->end();
        }
    }

}
